/*-----------------------------------------------------------------------------
 * EtherCATStack.cpp            
 * Copyright                acontis technologies GmbH, Weingarten, Germany
 * Response                 Stefan Zintgraf
 * Description              EtherCAT Master demo application
 *---------------------------------------------------------------------------*/
/*-INCLUDES------------------------------------------------------------------*/
#include <AtEthercat.h>

#include <EtherCATMain.hpp>
#include <EtherCATStack.h>
#include <Logging.h>

#ifdef ATEMRAS_SERVER 
#include <AtEmRasSrv.h>
#endif

/*-DEFINES-------------------------------------------------------------------*/
#define DCM_ENABLE_LOGFILE
#define LogMsg      S_poLog->LogMsg
#define LogError    S_poLog->LogError
#define LogDcm      S_poLog->LogDcm

/*-LOCAL VARIABLES-----------------------------------------------------------*/
static EC_T_DWORD          S_dwClntId        = 0;
static CAtEmLogging*       S_poLog           = EC_NULL;
static T_DEMO_THREAD_PARAM S_DemoThreadParam = {0};
static EC_T_PVOID          S_pvtJobThread    = EC_NULL;
#ifdef ATEMRAS_SERVER 
static EC_T_BOOL           S_bRasSrvStarted  = EC_FALSE;
static EC_T_PVOID          S_pvRemoteApiSrvH = EC_NULL;
#endif
static EC_T_BOOL           S_bEnaPerfJobs    = EC_FALSE;
static EC_T_TSC_MEAS_DESC  S_TscMeasDesc;
static EC_T_CHAR*          S_aszMeasInfo[MAX_JOB_NUM] =
{
    (EC_T_CHAR*)"JOB_ProcessAllRxFrames",
    (EC_T_CHAR*)"JOB_SendAllCycFrames  ",
    (EC_T_CHAR*)"JOB_MasterTimer       ",
    (EC_T_CHAR*)"JOB_SendAcycFrames    ",
    (EC_T_CHAR*)"Cycle Time            ",
    (EC_T_CHAR*)"myAppWorkPd           ",
    (EC_T_CHAR*)"Write DCM logfile     "
};

/*-FORWARD DECLARATIONS------------------------------------------------------*/
static EC_T_DWORD ecatNotifyCallback(EC_T_DWORD dwCode, EC_T_NOTIFYPARMS* pParms);
#if (defined ATEMRAS_SERVER)
static EC_T_DWORD RasNotifyWrapper(EC_T_DWORD dwCode, EC_T_NOTIFYPARMS* pParms);
#endif
static EC_T_VOID  tEcJobTask(EC_T_VOID* pvThreadParamDesc);

/*-MYAPP---------------------------------------------------------------------*/
/* Demo code: Remove/change this in your application */
static EC_T_DWORD myAppInit     (CAtEmLogging*           poLog, EC_T_INT nVerbose);
static EC_T_DWORD myAppPrepare  (CAtEmLogging*           poLog, EC_T_INT nVerbose);
static EC_T_DWORD myAppSetup    (CAtEmLogging*           poLog, EC_T_INT nVerbose, EC_T_DWORD dwClntId);
static EC_T_DWORD myAppWorkpd   (CAtEmLogging*           poLog, EC_T_INT nVerbose, EC_T_BYTE* pbyPDIn, EC_T_BYTE* pbyPDOut);
static EC_T_DWORD myAppDiagnosis(CAtEmLogging*           poLog, EC_T_INT nVerbose);
static EC_T_DWORD myAppNotify   (EC_T_DWORD dwCode, EC_T_NOTIFYPARMS* pParms);
/* Demo code: End */

/*-FUNCTION DEFINITIONS------------------------------------------------------*/

/********************************************************************************/
/** \brief  EtherCAT Master DC demo Application.
*
* This is a EtherCAT Master DC demo application.
*
* \return  Status value.
*/
// ethercat::callbackFctPtr
void (*callbackFct)(EC_T_BYTE*, EC_T_BYTE*);

EC_T_DWORD EtherCATStack(
    CAtEmLogging*       poLog
   ,EC_T_CNF_TYPE       eCnfType                /* [in] Enum type of configuration data provided */
   ,EC_T_PBYTE          pbyCnfData              /* [in] Configuration data */                      
   ,EC_T_DWORD          dwCnfDataLen            /* [in] Length of configuration data in byte */    
   ,EC_T_DWORD          dwBusCycleTimeUsec      /* [in]  bus cycle time in usec */
   ,EC_T_INT            nVerbose                /* [in]  verbosity level */
   ,EC_T_DWORD          dwDuration              /* [in]  test duration in msec (0 = forever) */
   ,EC_T_LINK_PARMS*    poLinkParms             /* [in]  pointer to link parameter */
   ,EC_T_VOID*          pvTimingEvent           /* [in]  Timing event handle */
   ,EC_T_DWORD          dwCpuIndex              /* [in]  SMP only: CPU index */
   ,EC_T_BOOL           bEnaPerfJobs            /* [in]  Performance measurement */
#ifdef ATEMRAS_SERVER 
   ,EC_T_WORD           wServerPort             /* [in]   Remote API Server Port */
#endif
   ,EC_T_LINK_PARMS*    poLinkParmsRed          /* [in]  Redundancy Link Layer Parameter */
   ,EC_T_OS_PARMS*      poOsParms               /* [in]  pointer to OS parameters  */
   ,EC_T_DCM_MODE       eDcmMode                /* [in]  DCM mode */
   ,EC_T_BOOL           bDcmCtlOff              /* [in]  Disables / Enables the control loop for diagnosis */
   ,void (*callbackFctPtr)(EC_T_BYTE*, EC_T_BYTE*)
   ,char*				licenseKey
)
{
	
	callbackFct = callbackFctPtr;
    EC_T_DWORD       dwRetVal = EC_E_NOERROR;
    EC_T_DWORD       dwRes    = EC_E_NOERROR;
    EC_T_BOOL        bRes     = EC_FALSE;
    CEcTimer         oTimeout;
    CEmNotification* pNotification = EC_NULL;
    CEcTimer         oDcmStatusTimer;
    EC_T_BOOL        bFirstDcmStatus = EC_TRUE;
    EC_T_INT         nDcmCtlSetVal = ((dwBusCycleTimeUsec*2)/3)*1000; /* set value in nanosec, 66% of the bus cycle */

    /* store parameters */
    S_poLog = poLog;
    S_bEnaPerfJobs = bEnaPerfJobs;
    S_DemoThreadParam.pvTimingEvent = pvTimingEvent;

	LogMsg("nVerbose: %d\n", nVerbose);
    /* check if interrupt mode is selected */
    if (poLinkParms->eLinkMode != EcLinkMode_POLLING)
    {
        dwRetVal = EC_E_INVALIDPARM;
        LogError("Error: Link layer in 'interrupt' mode is not supported by EcMasterDemoDc. Please select 'polling' mode.");
        goto Exit;
    }
    /* set thread affinity */
    {
        EC_T_CPUSET CpuSet;

        EC_CPUSET_ZERO(CpuSet);
        EC_CPUSET_SET(CpuSet, dwCpuIndex);
        bRes = OsSetThreadAffinity(EC_NULL, CpuSet);
        if (!bRes)
        {
            dwRetVal = EC_E_INVALIDPARM;
            LogError("Error: Set thread affinitiy, invalid CPU index %d\n", dwCpuIndex);
            goto Exit;
        }
    }
    /* create notification context */
    pNotification = EC_NEW(CEmNotification(INSTANCE_MASTER_DEFAULT, poLog));
    if (EC_NULL == pNotification)
    {
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }
    pNotification->Verbose(nVerbose);

    if (S_bEnaPerfJobs)
    {
        PERF_MEASURE_JOBS_INIT(EC_NULL);
    }
    
    /* Demo code: Remove/change this in your application: Initialize application */
    /*****************************************************************************/
    dwRes = myAppInit(poLog, nVerbose);
    if (EC_E_NOERROR != dwRes)
    {
        LogError( (EC_T_CHAR*)"myAppInit failed, error code: 0x%x", dwRes );
        dwRetVal = dwRes;
        goto Exit;
    }
#ifdef ATEMRAS_SERVER
    /*******************************/
    /* Start RAS server if enabled */
    /*******************************/
    if (0xFFFF != wServerPort)
    {
        ATEMRAS_T_SRVPARMS oRemoteApiConfig;

        OsMemset(&oRemoteApiConfig, 0, sizeof(ATEMRAS_T_SRVPARMS));
        oRemoteApiConfig.oAddr.dwAddr       = 0;                    /* INADDR_ANY */
        oRemoteApiConfig.wPort              = wServerPort;
        oRemoteApiConfig.dwCycleTime        = REMOTE_CYCLE_TIME;    /* 2 msec */
        oRemoteApiConfig.dwWDTOLimit        = (REMOTE_WD_TO_LIMIT/REMOTE_CYCLE_TIME); /* WD Timeout after 10secs */
        oRemoteApiConfig.dwReConTOLimit     = 6000;                 /* Reconnect Timeout after 6000*2msec + 10secs */
        oRemoteApiConfig.dwMasterPrio       = MAIN_THREAD_PRIO;
        oRemoteApiConfig.dwClientPrio       = MAIN_THREAD_PRIO;
        oRemoteApiConfig.pvNotifCtxt        = pNotification;        /* Notification context */
        oRemoteApiConfig.pfNotification     = RasNotifyWrapper;     /* Notification function for emras Layer */
        oRemoteApiConfig.dwConcNotifyAmount = 100;                  /* for the first pre-allocate 100 Notification spaces */
        oRemoteApiConfig.dwMbxNotifyAmount  = 50;                   /* for the first pre-allocate 50 Notification spaces */
        oRemoteApiConfig.dwMbxUsrNotifySize = 3000;                 /* 3K user space for Mailbox Notifications */
        oRemoteApiConfig.dwCycErrInterval   = 500;                  /* span between to consecutive cyclic notifications of same type */
        if (0 != nVerbose) LogMsg("Start Remote API Server now\n");
        dwRes = emRasSrvStart(oRemoteApiConfig, &S_pvRemoteApiSrvH);
        if (EC_E_NOERROR != dwRes)
        {
            LogError("ERROR: Cannot spawn Remote API Server\n");
        }
        S_bRasSrvStarted = EC_TRUE;
    }
#endif
    /******************************/
    /* Initialize EtherCAT master */
    /******************************/
    if (0 != nVerbose) LogMsg( "==========================" );
    if (0 != nVerbose) LogMsg( "Initialize EtherCAT Master" );
    if (0 != nVerbose) LogMsg( "==========================" );
    {
        EC_T_INIT_MASTER_PARMS oInitParms;

        OsMemset(&oInitParms, 0, sizeof(EC_T_INIT_MASTER_PARMS));
        oInitParms.dwSignature                   = ATECAT_SIGNATURE;
        oInitParms.dwSize                        = sizeof(EC_T_INIT_MASTER_PARMS);
        oInitParms.pLinkParms                    = poLinkParms;
        oInitParms.pLinkParmsRed                 = poLinkParmsRed;
        oInitParms.dwBusCycleTimeUsec            = dwBusCycleTimeUsec;
        oInitParms.dwMaxBusSlaves                = MASTER_CFG_ECAT_MAX_BUS_SLAVES;
        oInitParms.dwMaxQueuedEthFrames          = MASTER_CFG_MAX_QUEUED_ETH_FRAMES;
        oInitParms.dwMaxSlaveCmdPerFrame         = MASTER_CFG_MAX_SLAVECMD_PER_FRAME;
        if (dwBusCycleTimeUsec < 1000)
        {
            oInitParms.dwMaxSentQueuedFramesPerCycle = 1;
        }
        else
        {
            oInitParms.dwMaxSentQueuedFramesPerCycle = MASTER_CFG_MAX_SENT_QUFRM_PER_CYC;
        }
        oInitParms.dwEcatCmdMaxRetries           = MASTER_CFG_ECAT_CMD_MAX_RETRIES;
        oInitParms.dwEoETimeout                  = MASTER_CFG_EOE_TIMEOUT;
        oInitParms.dwFoEBusyTimeout              = MASTER_CFG_FOE_BUSY_TIMEOUT;
        oInitParms.dwLogLevel                    = nVerbose;
        oInitParms.pfLogMsgCallBack              = CAtEmLogging::OsDbgMsgHookWrapper;
        oInitParms.pOsParms                      = poOsParms;
        dwRes = ecatInitMaster(&oInitParms);        
        if (EC_E_NOERROR != dwRes)
        {
            dwRetVal = dwRes;
            LogError("Cannot initialize EtherCAT-Master! (Result = %s 0x%x)", ecatGetText(dwRes), dwRes);
            goto Exit;
        }
        
        if (strlen(licenseKey) > 1) {
			LogMsg("LicenseKey: %s",licenseKey);
			dwRes = ecatSetLicenseKey(licenseKey);
			if (EC_E_NOERROR == dwRes) LogMsg("ACTIVATION SUCCESSFULL");
			else 	 					LogError("ACTIVATION FAILED (Result = %s 0x%x)", ecatGetText(dwRes), dwRes);
		}
    }
    
    /* Create cyclic task to trigger master jobs */
    /*********************************************/
    S_DemoThreadParam.bJobThreadRunning  = EC_FALSE;
    S_DemoThreadParam.bJobThreadShutdown = EC_FALSE;
    S_DemoThreadParam.pLogInst           = S_poLog;
    S_DemoThreadParam.pNotInst           = pNotification;
    S_DemoThreadParam.dwCpuIndex         = dwCpuIndex;
    S_DemoThreadParam.dwBusCycleTimeUsec = dwBusCycleTimeUsec;
    S_pvtJobThread = OsCreateThread((EC_T_CHAR*)"tEcJobTask", tEcJobTask,
#if !(defined EC_VERSION_GO32)
                                    JOBS_THREAD_PRIO,
#else
                                    dwBusCycleTimeUsec,
#endif
                                    JOBS_THREAD_STACKSIZE, (EC_T_VOID*)&S_DemoThreadParam);
#ifdef RTAI
    OsMakeThreadPeriodic(S_pvtJobThread, dwBusCycleTimeUsec);
#endif
    /* wait until thread is running */
    oTimeout.Start(2000);
    while (!oTimeout.IsElapsed() && !S_DemoThreadParam.bJobThreadRunning)
    {
        OsSleep(10);
    }
    if (!S_DemoThreadParam.bJobThreadRunning)
    {
        dwRetVal = EC_E_TIMEOUT;
        LogError("Timeout starting JobTask");
        goto Exit;
    }
    oTimeout.Stop();

    /* Configure master */
    dwRes = ecatConfigureMaster(eCnfType, pbyCnfData, dwCnfDataLen);
    if (EC_E_NOERROR != dwRes)
    {
        dwRetVal = dwRes;
        LogError("Cannot configure EtherCAT-Master! %s (Result = 0x%x)", ecatGetText(dwRes), dwRes);
        goto Exit;
    }
    /* configure DC/DCM if master is started with ENI */
    if (pbyCnfData != EC_NULL)
    {
        /* configure DC */
        {
            EC_T_DC_CONFIGURE oDcConfigure;

            OsMemset(&oDcConfigure, 0, sizeof(EC_T_DC_CONFIGURE));
            oDcConfigure.dwTimeout          = ETHERCAT_DC_TIMEOUT;
            oDcConfigure.dwDevLimit         = ETHERCAT_DC_DEV_LIMIT;
            oDcConfigure.dwSettleTime       = ETHERCAT_DC_SETTLE_TIME;
            if (eDcmMode_MasterRefClock == eDcmMode)
            {
                oDcConfigure.dwTotalBurstLength = 10000; 
                oDcConfigure.dwBurstBulk        = 1;
            }
            else
            {
                oDcConfigure.dwTotalBurstLength = ETHERCAT_DC_ARMW_BURSTCYCLES; 
                if (dwBusCycleTimeUsec < 1000)
                {
                    /* if the cycle time is below 1000 usec, we have to reduce the number of frames sent within one cycle */
                    oDcConfigure.dwBurstBulk = ETHERCAT_DC_ARMW_BURSTSPP / 2;
                }
                else
                {
                    oDcConfigure.dwBurstBulk = ETHERCAT_DC_ARMW_BURSTSPP;
                }
            }
            if (eDcmMode_Dcx == eDcmMode)
            {
                oDcConfigure.bAcycDistributionDisabled = EC_FALSE; /* Enable acyclic distribution if cycle time is above 1000 usec to get DCX in sync */
            }
            else
            {
                oDcConfigure.bAcycDistributionDisabled = EC_TRUE;
            }
            dwRes = ecatDcConfigure(&oDcConfigure);
            if (dwRes != EC_E_NOERROR )
            {
                dwRetVal = dwRes;
                LogError("Cannot configure DC! (Result = 0x%x)", dwRes);
                goto Exit;
            }
        }
        /* configure DCM */
        {
            EC_T_DCM_CONFIG oDcmConfig;
            EC_T_BOOL       bLogEnabled = EC_FALSE;
#ifdef DCM_ENABLE_LOGFILE
            if (pNotification->Verbose() >= 3)
            {
                bLogEnabled = EC_TRUE;
            }
#endif
            if (bDcmCtlOff)
            {
                if (0 != nVerbose) LogMsg("DCM control loop disabled for diagnosis!\n");
            }
            OsMemset(&oDcmConfig, 0, sizeof(EC_T_DCM_CONFIG));
            switch (eDcmMode)
            {
            case eDcmMode_Off:
                oDcmConfig.eMode = eDcmMode_Off;
                break;
            case eDcmMode_BusShift:
                oDcmConfig.eMode = eDcmMode_BusShift;
                oDcmConfig.u.BusShift.nCtlSetVal    = nDcmCtlSetVal;
                oDcmConfig.u.BusShift.dwInSyncLimit = (dwBusCycleTimeUsec*1000)/5;    /* 20 % limit in nsec for InSync monitoring */
                oDcmConfig.u.BusShift.bLogEnabled = bLogEnabled;
                oDcmConfig.u.BusShift.bCtlOff = bDcmCtlOff;
                break;
             case eDcmMode_MasterShift:
                oDcmConfig.eMode = eDcmMode_MasterShift;
                oDcmConfig.u.MasterShift.nCtlSetVal    = nDcmCtlSetVal;
                oDcmConfig.u.MasterShift.dwInSyncLimit = (dwBusCycleTimeUsec*1000)/5;    /* 20 % limit in nsec for InSync monitoring */
                oDcmConfig.u.MasterShift.bLogEnabled = bLogEnabled;
                oDcmConfig.u.MasterShift.bCtlOff = bDcmCtlOff;
                break;
            case eDcmMode_MasterRefClock:
                oDcmConfig.eMode = eDcmMode_MasterRefClock;
                oDcmConfig.u.MasterRefClock.nCtlSetVal  = nDcmCtlSetVal;
                oDcmConfig.u.MasterRefClock.bLogEnabled = bLogEnabled;
                break;
            case eDcmMode_Dcx:
                oDcmConfig.eMode = eDcmMode_Dcx;
                /* Mastershift */
                oDcmConfig.u.Dcx.MasterShift.nCtlSetVal = nDcmCtlSetVal;
                oDcmConfig.u.Dcx.MasterShift.dwInSyncLimit = (dwBusCycleTimeUsec * 1000) / 5;    /* 20 % limit in nsec for InSync monitoring */
                oDcmConfig.u.Dcx.MasterShift.bLogEnabled = bLogEnabled;
                oDcmConfig.u.Dcx.MasterShift.bCtlOff = bDcmCtlOff;
                /* Dcx Busshift */
                oDcmConfig.u.Dcx.nCtlSetVal = nDcmCtlSetVal;
                oDcmConfig.u.Dcx.dwInSyncLimit = (dwBusCycleTimeUsec * 1000) / 50;   /* 2 % limit in nsec for InSync monitoring */
                oDcmConfig.u.Dcx.bLogEnabled = bLogEnabled;
                oDcmConfig.u.Dcx.bCtlOff = bDcmCtlOff;
                oDcmConfig.u.Dcx.dwExtClockTimeout = 1000;
                oDcmConfig.u.Dcx.wExtClockFixedAddr = 0; /* 0 only when clock adjustment in external mode configured by EcEngineer */
                break;
            default:
                dwRetVal = EC_E_NOTSUPPORTED;
                LogError("DCM mode is not supported!");
                goto Exit;

            }
            dwRes = ecatDcmConfigure(&oDcmConfig, 0);
            switch (dwRes)
            {
            case EC_E_NOERROR:
                break;
            case EC_E_FEATURE_DISABLED:
                LogError("Cannot configure DCM mode!");
                LogError("Start with -dcmmode off to run the DC demo without DCM, or prepare the ENI file to support the requested DCM mode");
                LogError("In ET9000 for example, select under ""Advanced settings\\Distributed clocks"" ""DC in use"" and ""Slave Mode""");
                LogError("to support BusShift and MasterRefClock modes.");
                LogError("Please refer to the class A manual for further information");
                dwRetVal = dwRes;
                goto Exit;
            default:
                LogError("Cannot configure DCM mode! %s (Result = 0x%x)", ecatGetText(dwRes), dwRes);
                dwRetVal = dwRes;
                goto Exit;
            }
        }
    }
    /* Register client */
    {
        EC_T_REGISTERRESULTS oRegisterResults;

        OsMemset(&oRegisterResults, 0, sizeof(EC_T_REGISTERRESULTS));
        dwRes = ecatRegisterClient(ecatNotifyCallback, pNotification, &oRegisterResults);
        if (EC_E_NOERROR != dwRes)
        {
            dwRetVal = dwRes;
            LogError("Cannot register client! (Result = 0x%x)", dwRes);
            goto Exit;
        }
        S_dwClntId = oRegisterResults.dwClntId;
        pNotification->SetClientID(S_dwClntId);
    }
    
    /* Print found slaves */
    if (nVerbose >= 2)
    {
        dwRes = ecatScanBus(ETHERCAT_SCANBUS_TIMEOUT);
        switch (dwRes)
        {
        case EC_E_NOERROR:
        case EC_E_BUSCONFIG_MISMATCH:
        case EC_E_LINE_CROSSED:
            PrintSlaveInfos(INSTANCE_MASTER_DEFAULT, poLog);
            break;
        default:
            LogError("Cannot scan bus: %s (0x%lx)", ecatGetText(dwRes), dwRes);
            break;
        }
    }

    /* Print MAC address */
    if (nVerbose > 0)
    {
        ETHERNET_ADDRESS oSrcMacAddress;

        dwRes = ecatGetSrcMacAddress(&oSrcMacAddress);
        if (EC_E_NOERROR != dwRes)
        {
            LogError("Cannot get MAC address! (Result = 0x%x)", dwRes);
        }
        LogMsg("EtherCAT network adapter MAC: %02X-%02X-%02X-%02X-%02X-%02X\n",
            oSrcMacAddress.b[0], oSrcMacAddress.b[1], oSrcMacAddress.b[2], oSrcMacAddress.b[3], oSrcMacAddress.b[4], oSrcMacAddress.b[5]);
    }
    
    /* Start EtherCAT bus --> set Master state to OPERATIONAL if ENI file provided */
    /*******************************************************************************/
    if (0 != nVerbose) LogMsg( "=====================" );
    if (0 != nVerbose) LogMsg( "Start EtherCAT Master" );
    if (0 != nVerbose) LogMsg( "=====================" );

    /* set master and bus state to INIT */
    dwRes = ecatSetMasterState(ETHERCAT_STATE_CHANGE_TIMEOUT, eEcatState_INIT);
    pNotification->ProcessNotificationJobs();
    if (EC_E_NOERROR != dwRes)
    {
        LogError("Cannot start set master state to INIT (Result = %s (0x%lx))", ecatGetText(dwRes), dwRes);
        dwRetVal = dwRes;
        goto Exit;
    }

    /******************************************************/
    /* Demo code: Remove/change this in your application  */
    /******************************************************/
    dwRes = myAppPrepare(poLog, nVerbose);
    if (EC_E_NOERROR != dwRes)
    {
        LogError((EC_T_CHAR*)"myAppPrepare failed, error code: 0x%x", dwRes);
        dwRetVal = dwRes;
        goto Exit;
    }
    /* set master and bus state to PREOP */
    dwRes = ecatSetMasterState(ETHERCAT_STATE_CHANGE_TIMEOUT, eEcatState_PREOP);
    pNotification->ProcessNotificationJobs();
    if (EC_E_NOERROR != dwRes)
    {
        LogError("Cannot start set master state to PREOP (Result = %s (0x%lx))", ecatGetText(dwRes), dwRes);
        dwRetVal = dwRes;
        goto Exit;
    }
    /* skip this step if demo started without ENI */
    if (pbyCnfData != EC_NULL)
    {
        /******************************************************/
        /* Demo code: Remove/change this in your application  */
        /******************************************************/
        dwRes = myAppSetup(poLog, nVerbose, S_dwClntId);
        if (EC_E_NOERROR != dwRes)
        {
            LogError((EC_T_CHAR*)"myAppSetup failed, error code: 0x%x", dwRes);
            dwRetVal = dwRes;
            goto Exit;
        }
        /* set master and bus state to SAFEOP */
        dwRes = ecatSetMasterState(ETHERCAT_STATE_CHANGE_TIMEOUT, eEcatState_SAFEOP);
        pNotification->ProcessNotificationJobs();
        if (EC_E_NOERROR != dwRes)
        {
            LogError("Cannot start set master state to SAFEOP (Result = %s (0x%lx))", ecatGetText(dwRes), dwRes);

            /* most of the time SAFEOP is not reachable due to DCM */
            if (eDcmMode_Off != eDcmMode)
            {
            EC_T_DWORD dwDcmStatus = 0;
            EC_T_INT   nDiffCur = 0, nDiffAvg = 0, nDiffMax = 0;

                dwRes = ecatDcmGetStatus(&dwDcmStatus, &nDiffCur, &nDiffAvg, &nDiffMax);
                if (dwRes == EC_E_NOERROR)
                {
                    if (dwDcmStatus != EC_E_NOERROR)
                    {
                        LogError("DCM Status: %s (0x%08X)\n", ecatGetText(dwDcmStatus), dwDcmStatus);
                    }
                }
                else
                {
                    LogError("Cannot get DCM status! %s (0x%08X)\n", ecatGetText(dwRes), dwRes);
                }
            }
            dwRetVal = dwRes;
            goto Exit;
        }
        /* set master and bus state to OP */
        dwRes = ecatSetMasterState(ETHERCAT_STATE_CHANGE_TIMEOUT, eEcatState_OP);
        pNotification->ProcessNotificationJobs();
        if (EC_E_NOERROR != dwRes)
        {
            LogError("Cannot start set master state to OP (Result = %s (0x%lx))", ecatGetText(dwRes), dwRes);
            dwRetVal = dwRes;
            goto Exit;
        }
    }
    else
    {
        if (0 != nVerbose) LogMsg("No ENI file provided. EC-Master started with generated ENI file.");
    }

    if (S_bEnaPerfJobs)
    {
        LogMsg("");
        LogMsg("Job times during startup <INIT> to <%s>:", ecatStateToStr(ecatGetMasterState()));
        PERF_MEASURE_JOBS_SHOW();       /* show job times */
        LogMsg("");
        ecatPerfMeasReset(&S_TscMeasDesc, 0xFFFFFFFF);        /* clear job times of startup phase */
    }

#if (defined DEBUG) && (defined XENOMAI)
    /* Enabling mode switch warnings for shadowed task */
    dwRes = rt_task_set_mode(0, T_WARNSW, NULL);
    if (0 != dwRes)
    {
        OsDbgMsg("EnableRealtimeEnvironment: rt_task_set_mode returned an error\n");
        OsDbgAssert(EC_FALSE);
    }
#endif

    /* run the demo */
    if (dwDuration != 0)
    {
        oTimeout.Start(dwDuration);
    }
    while (bRun && (!oTimeout.IsStarted() || !oTimeout.IsElapsed()))
    {
        if (nVerbose >= 2)
        {
            PERF_MEASURE_JOBS_SHOW();       /* show job times */
        }
        bRun = !OsTerminateAppRequest();/* check if demo shall terminate */

        /*****************************************************************************************/
        /* Demo code: Remove/change this in your application: Do some diagnosis outside job task */
        /*****************************************************************************************/
        myAppDiagnosis(poLog, nVerbose);

        /* check DCM status if master is started with ENI */
        if (pbyCnfData != EC_NULL)
        {
            if (eDcmMode_Off != eDcmMode)
            {
            EC_T_DWORD dwDcmStatus = 0;
            EC_T_INT   nDiffCur = 0, nDiffAvg = 0, nDiffMax = 0;
            EC_T_BOOL  bLogDcmStatus = EC_FALSE;
            EC_T_BOOL  bResetDcmStatus = bFirstDcmStatus;

                if (!oDcmStatusTimer.IsStarted() || oDcmStatusTimer.IsElapsed())
                {
                    oDcmStatusTimer.Start(5000);
                    if (nVerbose >= 3)
                    {
                        bLogDcmStatus = EC_TRUE;
                    }
                }
                dwRes = ecatDcmGetStatus(&dwDcmStatus, &nDiffCur, &nDiffAvg, &nDiffMax);
                if (dwRes == EC_E_NOERROR)
                {
                    if (bFirstDcmStatus)
                    {
                        LogMsg("DCM during startup (<INIT> to <%s>)", ecatStateToStr(ecatGetMasterState()));
                    }
                    if ((dwDcmStatus != EC_E_NOTREADY) && (dwDcmStatus != EC_E_BUSY) && (dwDcmStatus != EC_E_NOERROR))
                    {
                        LogMsg("DCM Status: %s (0x%08X)\n", ecatGetText(dwDcmStatus), dwDcmStatus);
                    }
                    if (bLogDcmStatus)
                    {
                        LogMsg("DCM Diff(ns): Cur=%7d, Avg=%7d, Max=%7d", nDiffCur, nDiffAvg, nDiffMax);
                    }
                }
                else
                {
                    if ((eEcatState_OP == ecatGetMasterState()) || (eEcatState_SAFEOP == ecatGetMasterState()))
                    {
                        LogError("Cannot get DCM status! %s (0x%08X)\n", ecatGetText(dwRes), dwRes);
                    }
                    bResetDcmStatus = EC_FALSE;
                }
                if (eDcmMode_Dcx == eDcmMode)
                {
                EC_T_INT64 nTimeStampDiff = 0;

                    dwRes = ecatDcxGetStatus(&dwDcmStatus, &nDiffCur, &nDiffAvg, &nDiffMax, &nTimeStampDiff);
                    if (EC_E_NOERROR == dwRes)
                    {
                        if (bFirstDcmStatus)
                        {
                            LogMsg("DCX during startup (<INIT> to <%s>)", ecatStateToStr(ecatGetMasterState()));
                        }
                        if ((dwDcmStatus != EC_E_NOTREADY) && (dwDcmStatus != EC_E_BUSY) && (dwDcmStatus != EC_E_NOERROR))
                        {
                            LogMsg("DCX Status: %s (0x%08X)\n", ecatGetText(dwDcmStatus), dwDcmStatus);
                        }
                        if (bLogDcmStatus)
                        {
                            LogMsg("DCX Diff(ns): Cur=%7d, Avg=%7d, Max=%7d, TimeStamp=%7d", nDiffCur, nDiffAvg, nDiffMax, nTimeStampDiff);
                        }
                    }
                    else
                    {
                        if ((eEcatState_OP == ecatGetMasterState()) || (eEcatState_SAFEOP == ecatGetMasterState()))
                        {
                            LogError("Cannot get DCX status! %s (0x%08X)\n", ecatGetText(dwRes), dwRes);
                        }
                        bResetDcmStatus = EC_FALSE;
                    }
                }
                if (bResetDcmStatus)
                {
#if (defined ATECAT_VERSION) && (ATECAT_VERSION >= 0x02040106)
                    ecatDcmResetStatus();
#endif
                    bFirstDcmStatus = EC_FALSE;
                }
            }
        }
        /* process notification jobs */
        pNotification->ProcessNotificationJobs();

        OsSleep(5);
    }

    if (S_bEnaPerfJobs)
    {
        LogMsg("");
        LogMsg("Job times before shutdown");
        PERF_MEASURE_JOBS_SHOW();       /* show job times */
    }

Exit:
    if (0 != nVerbose) LogMsg( "========================" );
    if (0 != nVerbose) LogMsg( "Shutdown EtherCAT Master" );
    if (0 != nVerbose) LogMsg( "========================" );

    /* Stop EtherCAT bus --> Set Master state to INIT */
    dwRes = ecatSetMasterState(ETHERCAT_STATE_CHANGE_TIMEOUT, eEcatState_INIT);
    if (EC_E_NOERROR != dwRes)
    {
        LogError("Cannot stop EtherCAT-Master! %s (0x%lx)", ecatGetText(dwRes), dwRes);
    }
    /* Unregister client */
    if (S_dwClntId != 0)
    {
        dwRes = ecatUnregisterClient(S_dwClntId); 
        if (EC_E_NOERROR != dwRes)
        {
            LogError("Cannot unregister client! %s (0x%lx)", ecatGetText(dwRes), dwRes);
        }
        S_dwClntId = 0;
    }

#if (defined DEBUG) && (defined XENOMAI)
    /* Disable PRIMARY to SECONDARY MODE switch warning */
    dwRes = rt_task_set_mode(T_WARNSW, 0, NULL);
    if (dwRes != 0)
    {
        OsDbgMsg("OsCreateThread: rt_task_set_mode returned error %d\n", dwRes);
        OsDbgAssert(EC_FALSE);
    }
#endif /* XENOMAI */

    /* Shutdown tEcJobTask */
    S_DemoThreadParam.bJobThreadShutdown = EC_TRUE;
    oTimeout.Start(2000);
    while (S_DemoThreadParam.bJobThreadRunning && !oTimeout.IsElapsed())
    {
        OsSleep(10);
    }
    if (S_pvtJobThread != EC_NULL)
    {
        OsDeleteThreadHandle(S_pvtJobThread);
        S_pvtJobThread = EC_NULL;
    }

#ifdef ATEMRAS_SERVER
    /* Stop RAS server */
    if (S_bRasSrvStarted)
    {
        LogMsg("Stop Remote Api Server\n");
        
        if (EC_E_NOERROR != emRasSrvStop(S_pvRemoteApiSrvH, 2000))
        {
            LogError("Remote API Server shutdown failed\n");
        }
    }
#endif

    /* Deinitialize master */
    LogMsg( "ecatDeinitMaster() should be called here." );
    LogMsg( "But this would provoke a ' double free or corruption (out):' error." );
//     dwRes = ecatDeinitMaster();
    
    if (EC_E_NOERROR != dwRes)
    {
        LogError("Cannot de-initialize EtherCAT-Master! %s (0x%lx)", ecatGetText(dwRes), dwRes);
    }

    if (S_bEnaPerfJobs)
    {
        PERF_MEASURE_JOBS_DEINIT();
    }
    /* delete notification context */
    SafeDelete(pNotification);

    return dwRetVal;
}


/********************************************************************************/
/** \brief  Trigger jobs to drive master, and update process data.
*
* \return N/A
*/
static EC_T_VOID tEcJobTask(EC_T_VOID* pvThreadParamDesc)
{
    EC_T_DWORD           dwRes             = EC_E_ERROR;
    T_DEMO_THREAD_PARAM* pDemoThreadParam  = (T_DEMO_THREAD_PARAM*)pvThreadParamDesc;
    EC_T_CPUSET          CpuSet;
    EC_T_BOOL            bPrevCycProcessed = EC_FALSE;
    EC_T_INT             nOverloadCounter  = 0;               /* counter to check if cycle time is to short */
    EC_T_BOOL            bOk;

    EC_CPUSET_ZERO(CpuSet);
    EC_CPUSET_SET(CpuSet, pDemoThreadParam->dwCpuIndex);
    bOk = OsSetThreadAffinity(EC_NULL, CpuSet);
    if (!bOk)
    {
        LogError("Error: Set job task affinitiy, invalid CPU index %d\n", pDemoThreadParam->dwCpuIndex);
        goto Exit;
    }

    /* Start periodic task right before where it is used,
     * otherwise, OsSleepTillTick() might fail if this thread is suspended before being able to call OsMakeThreadPeriodic() */
#if (defined XENOMAI)
    OsMakeThreadPeriodic(rt_task_self(), pDemoThreadParam->dwBusCycleTimeUsec);
#endif

    /* demo loop */
    pDemoThreadParam->bJobThreadRunning = EC_TRUE;
    do
    {
        /* wait for next cycle (event from scheduler task) */
#if (defined RTAI) || (defined XENOMAI)
        OsSleepTillTick(); /* period is set after creating jobtask */
#else
        OsWaitForEvent(pDemoThreadParam->pvTimingEvent, EC_WAITINFINITE);
#endif /* !RTAI && !XENOMAI*/

        PERF_JOB_END(PERF_CycleTime);
        PERF_JOB_START(PERF_CycleTime);

        /* process all received frames (read new input values) */
        PERF_JOB_START(JOB_ProcessAllRxFrames);
        dwRes = ecatExecJob(eUsrJob_ProcessAllRxFrames, &bPrevCycProcessed);
        if (EC_E_NOERROR != dwRes && EC_E_INVALIDSTATE != dwRes && EC_E_LINK_DISCONNECTED != dwRes)
        {
            LogError("ERROR: ecatExecJob( eUsrJob_ProcessAllRxFrames): %s (0x%lx)", ecatGetText(dwRes), dwRes);
        }
        PERF_JOB_END(JOB_ProcessAllRxFrames);

        if (EC_E_NOERROR == dwRes)
        {
            if (!bPrevCycProcessed)
            {
                /* it is not reasonable, that more than 5 continuous frames are lost */
                nOverloadCounter += 10;
                if (nOverloadCounter >= 50)
                {
                    LogError( "Error: System overload: Cycle time too short or huge jitter!" );
                }
                else
                {
                    LogError( "eUsrJob_ProcessAllRxFrames - not all previously sent frames are received/processed (frame loss)!" );
                }
            }
            else
            {
                /* everything o.k.? If yes, decrement overload counter */
                if (nOverloadCounter > 0)    nOverloadCounter--;
            }
        }
        /* handle DCM logging */
#ifdef DCM_ENABLE_LOGFILE
        PERF_JOB_START(PERF_DCM_Logfile);
        {
            EC_T_CHAR* pszLog = EC_NULL;

            ecatDcmGetLog(&pszLog);
            if (EC_NULL != pszLog)
            {
                LogDcm(pszLog);
            }
        }
        PERF_JOB_END(PERF_DCM_Logfile);
#endif

        /*****************************************************/
        /* Demo code: Remove/change this in your application: Working process data cyclic call */
        /*****************************************************/
        PERF_JOB_START(PERF_myAppWorkpd);
        myAppWorkpd(pDemoThreadParam->pLogInst, pDemoThreadParam->pNotInst->Verbose(), ecatGetProcessImageInputPtr(), ecatGetProcessImageOutputPtr());
        PERF_JOB_END(PERF_myAppWorkpd);

        /* write output values of current cycle, by sending all cyclic frames */
        PERF_JOB_START(JOB_SendAllCycFrames);
        dwRes = ecatExecJob( eUsrJob_SendAllCycFrames, EC_NULL );
        if (EC_E_NOERROR != dwRes && EC_E_INVALIDSTATE != dwRes && EC_E_LINK_DISCONNECTED != dwRes)
        {
            LogError("ecatExecJob( eUsrJob_SendAllCycFrames,    EC_NULL ): %s (0x%lx)", ecatGetText(dwRes), dwRes);
        }
        PERF_JOB_END(JOB_SendAllCycFrames);

        /* remove this code when using licensed version */
        if (EC_E_EVAL_EXPIRED == dwRes )
        {
            bRun = EC_FALSE;        /* set shutdown flag */
        }

        /* Execute some administrative jobs. No bus traffic is performed by this function */
        PERF_JOB_START(JOB_MasterTimer);
        dwRes = ecatExecJob(eUsrJob_MasterTimer, EC_NULL);
        if (EC_E_NOERROR != dwRes && EC_E_INVALIDSTATE != dwRes)
        {
            LogError("ecatExecJob(eUsrJob_MasterTimer, EC_NULL): %s (0x%lx)", ecatGetText(dwRes), dwRes);
        }
        PERF_JOB_END(JOB_MasterTimer);

        /* send queued acyclic EtherCAT frames */
        PERF_JOB_START(JOB_SendAcycFrames);
        dwRes = ecatExecJob(eUsrJob_SendAcycFrames, EC_NULL);
        if (EC_E_NOERROR != dwRes && EC_E_INVALIDSTATE != dwRes && EC_E_LINK_DISCONNECTED != dwRes)
        {
            LogError("ecatExecJob(eUsrJob_SendAcycFrames, EC_NULL): %s (0x%lx)", ecatGetText(dwRes), dwRes);
        }
        PERF_JOB_END(JOB_SendAcycFrames);

#if !(defined NO_OS)
    } while (!pDemoThreadParam->bJobThreadShutdown);

    PERF_MEASURE_JOBS_SHOW();

    pDemoThreadParam->bJobThreadRunning = EC_FALSE;
#else
    /* in case of NO_OS the job task function is called cyclically within the timer ISR */
    } while (EC_FALSE);
    pDemoThreadParam->bJobThreadRunning = !pDemoThreadParam->bJobThreadShutdown;
#endif

Exit:
#if (defined EC_VERSION_RTEMS)
    rtems_task_delete(RTEMS_SELF);
#endif
    return;
}

/********************************************************************************/
/** \brief  Handler for master notifications
*
* \return  Status value.
*/
static EC_T_DWORD ecatNotifyCallback(
    EC_T_DWORD         dwCode,  /**< [in]   Notification code */
    EC_T_NOTIFYPARMS*  pParms   /**< [in]   Notification parameters */
                                         )
{
    EC_T_DWORD         dwRetVal                = EC_E_NOERROR;
    CEmNotification*   pNotifyInstance;

    if ((EC_NULL == pParms)||(EC_NULL==pParms->pCallerData))
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    /* notification for application ? */
    if ((dwCode >= EC_NOTIFY_APP) && (dwCode <= EC_NOTIFY_APP+EC_NOTIFY_APP_MAX_CODE))
    {
        /*****************************************************/
        /* Demo code: Remove/change this in your application */
        /* to get here the API ecatNotifyApp(dwCode, pParms) has to be called */
        /*****************************************************/
        dwRetVal = myAppNotify(dwCode-EC_NOTIFY_APP, pParms);
    }
    else
    {
        pNotifyInstance = (CEmNotification*)pParms->pCallerData;

        /* call the default handler */
        dwRetVal = pNotifyInstance->ecatNotify(dwCode, pParms);
    }

Exit:
    return dwRetVal;
}


/********************************************************************************/
/** \brief  Handler for master RAS notifications
*
*
* \return  Status value.
*/
#ifdef ATEMRAS_SERVER 
static EC_T_DWORD RasNotifyWrapper(
                            EC_T_DWORD         dwCode, 
                            EC_T_NOTIFYPARMS*  pParms
                            )
{
    EC_T_DWORD                      dwRetVal                = EC_E_NOERROR;
    CEmNotification*                pNotInst                = EC_NULL;
    
    if ((EC_NULL == pParms)||(EC_NULL==pParms->pCallerData))
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    
    pNotInst = (CEmNotification*)(pParms->pCallerData);
    dwRetVal = pNotInst->emRasNotify(dwCode, pParms);
Exit:
    
    return dwRetVal;
}
#endif

/*-MYAPP---------------------------------------------------------------------*/
#define MBX_TIMEOUT                         5000

#define EL4132_INDEX_USER_SCALE             0x40A2
#define EL4132_SUBINDEX_USRSCL_NUMELEM         0
#define EL4132_SUBINDEX_USRSCL_OFFSET          1
#define EL4132_SUBINDEX_USRSCL_GAIN            2

#define DEMO_MAX_NUM_OF_SLAVES  5
#define SLAVE_NOT_FOUND         0xFFFF
static EC_T_DWORD               S_dwAppFoundSlaves = 0;
static EC_T_CFG_SLAVE_INFO      S_aSlaveList[DEMO_MAX_NUM_OF_SLAVES];
static EC_T_DWORD               S_dwSlaveIdx14        = SLAVE_NOT_FOUND;
static EC_T_DWORD               S_dwSlaveIdx24        = SLAVE_NOT_FOUND;
static EC_T_DWORD               S_dwSlaveIdx4132      = SLAVE_NOT_FOUND;
static EC_T_DWORD               S_dwSlaveIdxETCio100  = SLAVE_NOT_FOUND;

/***************************************************************************************************/
/**
\brief  Initialize Application

\return EC_E_NOERROR on success, error code otherwise.
*/
static EC_T_DWORD myAppInit(
    CAtEmLogging*       poLog,          /* [in]  Logging instance */ 
    EC_T_INT            nVerbose        /* [in]  Verbosity level */
    )
{
    EC_UNREFPARM(poLog);
    EC_UNREFPARM(nVerbose);

    S_dwAppFoundSlaves = 0;
    OsMemset(S_aSlaveList, 0, DEMO_MAX_NUM_OF_SLAVES*sizeof(EC_T_CFG_SLAVE_INFO));
    S_dwSlaveIdx14       = SLAVE_NOT_FOUND;
    S_dwSlaveIdx24       = SLAVE_NOT_FOUND;
    S_dwSlaveIdx4132     = SLAVE_NOT_FOUND;
    S_dwSlaveIdxETCio100 = SLAVE_NOT_FOUND;

    return EC_E_NOERROR;
}

/***************************************************************************************************/
/**
\brief  Initialize Slave Instance.

Find slave parameters.
\return EC_E_NOERROR on success, error code otherwise.
*/
static EC_T_DWORD myAppPrepare(
    CAtEmLogging*       poLog,          /* [in]  Logging instance */ 
    EC_T_INT            nVerbose        /* [in]  Verbosity level */
    )
{
	LogMsg( "prepare" );
	
	
EC_T_WORD wFixedAddress = 0;

    EC_UNREFPARM(nVerbose);

    /* Searching for: EL1004, EL1012, EL1014                                       */
    /* search for the first device at the bus and return its fixed (EtherCAT) address */
    if (FindSlaveGetFixedAddr(INSTANCE_MASTER_DEFAULT, poLog, 0, ecvendor_beckhoff, ecprodcode_beck_EL1004, &wFixedAddress))
    {
        S_dwSlaveIdx14 = S_dwAppFoundSlaves;
    }
    else if (FindSlaveGetFixedAddr(INSTANCE_MASTER_DEFAULT, poLog, 0, ecvendor_beckhoff, ecprodcode_beck_EL1012, &wFixedAddress))
    {
        S_dwSlaveIdx14 = S_dwAppFoundSlaves;
    }
    else if (FindSlaveGetFixedAddr(INSTANCE_MASTER_DEFAULT, poLog, 0, ecvendor_beckhoff, ecprodcode_beck_EL1014, &wFixedAddress))
    {
        S_dwSlaveIdx14 = S_dwAppFoundSlaves;
    }
    if (S_dwSlaveIdx14 != SLAVE_NOT_FOUND)
    {
        S_dwAppFoundSlaves++;

        /* now get the offset of this device in the process data buffer and some other infos */
        if (ecatGetCfgSlaveInfo(EC_TRUE, wFixedAddress, &S_aSlaveList[S_dwSlaveIdx14]) != EC_E_NOERROR)
        {
            LogError("ERROR: ecatGetCfgSlaveInfo() returns with error.");
        }
    }
    /* Searching for: EL2002, EL2004, EL2008                                       */
    /* search for the first device at the bus and return its fixed (EtherCAT) address */
    if (FindSlaveGetFixedAddr(INSTANCE_MASTER_DEFAULT, poLog, 0, ecvendor_beckhoff, ecprodcode_beck_EL2008, &wFixedAddress))
    {
        S_dwSlaveIdx24 = S_dwAppFoundSlaves;
    }
    else if (FindSlaveGetFixedAddr(INSTANCE_MASTER_DEFAULT, poLog, 0, ecvendor_beckhoff, ecprodcode_beck_EL2004, &wFixedAddress))
    {
        S_dwSlaveIdx24 = S_dwAppFoundSlaves;
    }
    else if (FindSlaveGetFixedAddr(INSTANCE_MASTER_DEFAULT, poLog, 0, ecvendor_beckhoff, ecprodcode_beck_EL2002, &wFixedAddress))
    {
        S_dwSlaveIdx24 = S_dwAppFoundSlaves;
    }
    if (S_dwSlaveIdx24 != SLAVE_NOT_FOUND)
    {
        S_dwAppFoundSlaves++;

        /* now get the offset of this device in the process data buffer and some other infos */
        if (ecatGetCfgSlaveInfo(EC_TRUE, wFixedAddress, &S_aSlaveList[S_dwSlaveIdx24]) != EC_E_NOERROR)
        {
            LogError("ERROR: ecatGetCfgSlaveInfo() returns with error.");
        }
    }

    /* Searching for: EL4132                                                       */
    /* search for the first device at the bus and return its fixed (EtherCAT) address */
    if (FindSlaveGetFixedAddr(INSTANCE_MASTER_DEFAULT, poLog, 0, ecvendor_beckhoff, ecprodcode_beck_EL4132, &wFixedAddress))
    {
        S_dwSlaveIdx4132 = S_dwAppFoundSlaves;
        S_dwAppFoundSlaves++;
        /* now get the offset of this device in the process data buffer and some other infos */
        if (ecatGetCfgSlaveInfo(EC_TRUE, wFixedAddress, &S_aSlaveList[S_dwSlaveIdx4132]) != EC_E_NOERROR)
        {
            LogError("ERROR: ecatGetCfgSlaveInfo() returns with error.");
        }
    }
    /* Searching for: IXXAT ETCio100                                                    */
    /* search for the first device at the bus and return its fixed (EtherCAT) address */
    if (FindSlaveGetFixedAddr(INSTANCE_MASTER_DEFAULT, poLog, 0, ecvendor_ixxat, ecprodcode_ixx_ETCio100, &wFixedAddress))
    {
        S_dwSlaveIdxETCio100 = S_dwAppFoundSlaves;
        S_dwAppFoundSlaves++;
        /* now get the offset of this device in the process data buffer and some other infos */
        if (ecatGetCfgSlaveInfo(EC_TRUE, wFixedAddress, &S_aSlaveList[S_dwSlaveIdxETCio100]) != EC_E_NOERROR)
        {
            LogError("ERROR: ecatGetCfgSlaveInfo() returns with error.");
        }
    }
    return EC_E_NOERROR;
}

/***************************************************************************************************/
/**
\brief  Setup slave parameters (normally done in PREOP state

  - SDO up- and Downloads
  - Read Object Dictionary

\return EC_E_NOERROR on success, error code otherwise.
*/
static EC_T_DWORD myAppSetup(
    CAtEmLogging*      poLog,           /* [in]  Logging instance */     
    EC_T_INT           nVerbose,        /* [in]  verbosity level */
    EC_T_DWORD         dwClntId         /* [in]  EtherCAT master client id */
    )
{
    EC_T_DWORD           dwRes    = EC_E_ERROR;
    EC_T_CFG_SLAVE_INFO* pMySlave = EC_NULL;

    if (S_dwSlaveIdx4132 != SLAVE_NOT_FOUND)
    {
        EC_T_BOOL  bStopReading  = EC_FALSE;      /* Flag to stop object dictionary reading */
        EC_T_BYTE  byNumElements;
        EC_T_DWORD dwSize;
        EC_T_WORD  wOffsetTmp    = 0;
        EC_T_DWORD dwGainTmp     = 0;
        EC_T_WORD  wOffset;
        EC_T_DWORD dwGain;

        pMySlave = &S_aSlaveList[S_dwSlaveIdx4132];    

        /* demo: simple CoE SDO upload                              */
        /*       - synchronous: block until upload has finished     */
        dwRes = ecatCoeSdoUpload(pMySlave->dwSlaveId, EL4132_INDEX_USER_SCALE, EL4132_SUBINDEX_USRSCL_NUMELEM,
            &byNumElements, sizeof(EC_T_BYTE), &dwSize, MBX_TIMEOUT, 0);

        if (dwRes == EC_E_NOERROR)
        {
            if (nVerbose >= 2) LogMsg("tEl4132Mbx: EL4132 user scale: num elements = %d", (int)byNumElements);
        }
        else
        {
            LogError("tEl4132Mbx: error in COE SDO Upload! %s (0x%x)", ecatGetText(dwRes), dwRes);
        }

        dwRes = ecatCoeSdoUpload(pMySlave->dwSlaveId, EL4132_INDEX_USER_SCALE, EL4132_SUBINDEX_USRSCL_OFFSET,
            (EC_T_BYTE*)&wOffsetTmp, sizeof(EC_T_WORD), &dwSize, MBX_TIMEOUT, 0);
        wOffset = EC_NTOHS(wOffsetTmp);

        if (dwRes == EC_E_NOERROR)
        {
            if (nVerbose >= 2) LogMsg("tEl4132Mbx: EL4132 offset = 0x%x", (EC_T_DWORD)wOffset);
        }
        else
        {
            LogError("tEl4132Mbx: error in COE SDO Upload! %s (0x%x)", ecatGetText(dwRes), dwRes);
        }

        dwRes = ecatCoeSdoUpload(pMySlave->dwSlaveId, EL4132_INDEX_USER_SCALE, EL4132_SUBINDEX_USRSCL_GAIN,
            (EC_T_BYTE*)&dwGainTmp, sizeof(EC_T_DWORD), &dwSize, MBX_TIMEOUT, 0);
        dwGain = EC_NTOHL(dwGainTmp);

        if (dwRes == EC_E_NOERROR)
        {
            if (nVerbose >= 2) LogMsg("tEl4132Mbx: EL4132 gain = 0x%x", dwGain);
        }
        else
        {
            LogError("tEl4132Mbx: error in COE SDO Upload! %s (0x%x)", ecatGetText(dwRes), dwRes);
        }

        /* demo: simple CoE SDO download                            */
        /*       - synchronous: block until download has finished   */
        wOffset++;                  /* change user scale offset value */
        if (wOffset > 0x1000)
        {
            wOffset = 0;
        }
        wOffsetTmp = EC_HTONS(wOffset);
        dwRes = ecatCoeSdoDownload(pMySlave->dwSlaveId, EL4132_INDEX_USER_SCALE, EL4132_SUBINDEX_USRSCL_OFFSET, 
            (EC_T_BYTE*)&wOffsetTmp, sizeof(EC_T_WORD), MBX_TIMEOUT, 0);

        if (EC_E_NOERROR != dwRes)
        {
            LogError("tEl4132Mbx: error in COE SDO Download! %s (0x%x)", ecatGetText(dwRes), dwRes);
        }
        dwGain += 0x1000;
        if (dwGain > 0x10000000)
        {
            dwGain = 0;
        }
        
        dwGainTmp = EC_HTONL(dwGain);
        dwRes = ecatCoeSdoDownload(pMySlave->dwSlaveId, EL4132_INDEX_USER_SCALE, EL4132_SUBINDEX_USRSCL_GAIN,
            (EC_T_BYTE*)&dwGainTmp, sizeof(EC_T_DWORD), MBX_TIMEOUT, 0);
        
        if (EC_E_NOERROR != dwRes)
        {
            LogError("tEl4132Mbx: error in COE SDO Download! %s (0x%x)", ecatGetText(dwRes), dwRes);
        }

        /* now read object dict */
        /* In a real application this is typically not necessary */
        dwRes = CoeReadObjectDictionary(INSTANCE_MASTER_DEFAULT, poLog, nVerbose, &bStopReading, dwClntId, pMySlave->dwSlaveId, EC_TRUE, MBX_TIMEOUT);
    }
    return EC_E_NOERROR;
}

/***************************************************************************************************/
/**
\brief  demo application working process data function.

  This function is called in every cycle after the the master stack is started.
  
*/
static EC_T_DWORD myAppWorkpd(
    CAtEmLogging*       poLog,          /* [in]  Logging instance */ 
    EC_T_INT            nVerbose,       /* [in]  Verbosity level */
    EC_T_BYTE*          pbyPDIn,        /* [in]  pointer to process data input buffer */
    EC_T_BYTE*          pbyPDOut        /* [in]  pointer to process data output buffer */
    )
{
    EC_T_CFG_SLAVE_INFO* pMySlave      = EC_NULL;

    static EC_T_DWORD    s_dwWorkpdCnt = 0;
    EC_T_BYTE            byVal         = 0;
    EC_T_BYTE*           pbyVal        = EC_NULL;
    EC_T_WORD*           pwVal         = EC_NULL;
    EC_T_WORD            wValue        = 0;

    EC_UNREFPARM(poLog);
    EC_UNREFPARM(nVerbose);



// 	callbackFct( pbyPDIn, pbyPDOut );
	if ( callbackFct ) callbackFct( pbyPDIn, pbyPDOut );
	else LogError("callbackFct is not defined");
    
//     static int counter;
//     if ( counter % 250 == 0 ) {
// 		counter = 0;
// 		LogMsg( "Log out every 250 cycle" );
// 		
// 		// crude, but works	
// 		int32_t myValue;
// 		char myValueChar[16];
// 		
// // 		myValue = *( pbyPDIn+2 );
// // 		myValue |= *( pbyPDIn+3 ) << 8;
// // 		myValue |= *( pbyPDIn+4 ) << 16;
// // 		myValue |= *( pbyPDIn+5 ) << 24;
// // 		sprintf(myValueChar, "%d", myValue );
// // 		LogMsg( "my Value 0:" );
// // 		LogMsg( myValueChar );
// 		
// // 		// better way: (see 'EC-Master_ClassB.pdf' 4.4.3)
// // 		EC_T_DWORD myValueDWORD;
// // 		myValueDWORD = EC_GET_FRM_DWORD( pbyPDIn + 0 );
// // 		sprintf(myValueChar, "%d", myValueDWORD );
// // 		LogMsg( "my Value 1:" );
// // 		LogMsg( myValueChar );
// // 		
// // 		myValue = EC_GET_FRM_DWORD( pbyPDIn + 0 );
// // 		sprintf(myValueChar, "%d", myValue );
// // 		LogMsg( "my Value 2:" );
// // 		LogMsg( myValueChar );
// 		
// 	}
// 	counter ++;



    /* process data are not modified not every cycle */
    s_dwWorkpdCnt++;
    if ((s_dwWorkpdCnt % 100) != 0)
    {
        goto Exit;
    }
	
	
	
    /* Digital input slave available ? */
    if ((S_dwSlaveIdx14 != SLAVE_NOT_FOUND) && (pbyPDIn != EC_NULL))
    {
    static EC_T_BYTE s_byDigInputLastVal = 0;


        /* get slave information */
        pMySlave = &S_aSlaveList[S_dwSlaveIdx14];

        /* monitor input change if ENI was NOT generated with GenPreopENI */
        if (((EC_T_DWORD)-1) != pMySlave->dwPdOffsIn)
        {
            EC_GETBITS(pbyPDIn, &byVal, pMySlave->dwPdOffsIn, pMySlave->dwPdSizeIn);
            if (byVal != s_byDigInputLastVal)
            {
                if (nVerbose >= EC_LOG_LEVEL_INFO)
                {
                    LogMsg("Input Value updated : Old : 0x%x -> New : 0x%x", s_byDigInputLastVal, byVal);
                    s_byDigInputLastVal = byVal;
                }
            }
        }
    }
    /* Digital output device */
    if ((S_dwSlaveIdx24 != SLAVE_NOT_FOUND) && (pbyPDOut != EC_NULL))
    {
        /* get slave information */
        pMySlave = &S_aSlaveList[S_dwSlaveIdx24];

        /* flash output if ENI was NOT generated with GenPreopENI */
        if (((EC_T_DWORD)-1) != pMySlave->dwPdOffsOut)
        {
            EC_GETBITS(pbyPDOut, &byVal, pMySlave->dwPdOffsOut, pMySlave->dwPdSizeOut);
            byVal++;
            EC_SETBITS(pbyPDOut, &byVal, pMySlave->dwPdOffsOut, pMySlave->dwPdSizeOut);
        }
    }

    /* EL4132 */
    if ((S_dwSlaveIdx4132 != SLAVE_NOT_FOUND) && (pbyPDOut != EC_NULL))
    {
        /* get slave information */
        pMySlave = &S_aSlaveList[S_dwSlaveIdx4132];    

        /* do some upcounting on channel 1 */
        if (((EC_T_DWORD)-1) != pMySlave->dwPdOffsOut)
        {
            pwVal = (EC_T_WORD*)&pbyPDOut[pMySlave->dwPdOffsOut/8];
            wValue = EC_GET_FRM_WORD(pwVal); /* for big endian systems we have to swap */
            wValue++;
            EC_SET_FRM_WORD(pwVal, wValue);

            /* do some downcounting on channel 2 */
            pwVal = (EC_T_WORD*)&pbyPDOut[(pMySlave->dwPdOffsOut+16)/8];
            wValue = EC_GET_FRM_WORD(pwVal); /* for big endian systems we have to swap */
            wValue--;
            EC_SET_FRM_WORD(pwVal, wValue);
        }
    }
    /* IXXAT ETCio100 */
    if ((S_dwSlaveIdxETCio100 != SLAVE_NOT_FOUND) && (pbyPDOut != EC_NULL))
    {
        /* get slave information */
        pMySlave = &S_aSlaveList[S_dwSlaveIdxETCio100];    

        /* flash digital output if ENI was NOT generated with GenPreopENI */
        if (((EC_T_DWORD)-1) != pMySlave->dwPdOffsOut)
        {
            pbyVal = (EC_T_BYTE*)&pbyPDOut[pMySlave->dwPdOffsOut/8];
            *pbyVal = (EC_T_BYTE)((*pbyVal) + 1);
        }

        /* increase analog output 1 if ENI was NOT generated with GenPreopENI */
        /* analog outputs are 12 Bit values at the ETCio 100 */
        if (((EC_T_DWORD)-1) != pMySlave->dwPdOffsOut)
        {
            pwVal = (EC_T_WORD*)&pbyPDOut[(pMySlave->dwPdOffsOut+8)/8];
            wValue = EC_GET_FRM_WORD(pwVal);  /* for big endian systems we have to swap */
            wValue = (EC_T_WORD)(wValue + 0x100);  /* to get a well measurable value difference modify last value by 0x100 */
            if(wValue & 0xF000)  /* 12 Bit value. The 4 highest bits are not relevant for the analog value */
            {
                wValue = 0;
            }
            EC_SET_FRM_WORD(pwVal, wValue);
        }

        /* decrease analog output 2 if ENI was NOT generated with GenPreopENI */
        /* analog outputs are 12 Bit values at the ETCio 100 */
        if (((EC_T_DWORD)-1) != pMySlave->dwPdOffsOut)
        {
            pwVal = (EC_T_WORD*)&pbyPDOut[(pMySlave->dwPdOffsOut+24)/8];
            wValue = EC_GET_FRM_WORD(pwVal); /* for big endian systems we have to swap */
            wValue = (EC_T_WORD)(wValue - 0x100);  /* to get a well measurable value difference modify last value by 0x100 */
            if(wValue & 0xF000)  /* 12 Bit value. The 4 highest bits are not relevant for the analog value */
            {
                wValue = 0x0F00;
            }
            EC_SET_FRM_WORD(pwVal, wValue);
        }
    }

Exit:
    return EC_E_NOERROR;
}

/***************************************************************************************************/
/**
\brief  demo application doing some diagnostic tasks

  This function is called in sometimes from the main demo task
*/
static EC_T_DWORD myAppDiagnosis(
    CAtEmLogging*       poLog,          /* [in]  Logging instance */ 
    EC_T_INT            nVerbose        /* [in]  Verbosity level */
    )
{
    EC_T_DWORD           dwRes    = EC_E_ERROR;
    EC_T_CFG_SLAVE_INFO* pMySlave = EC_NULL;

    EC_UNREFPARM(poLog);
    EC_UNREFPARM(nVerbose);

    if (S_dwSlaveIdx4132 != SLAVE_NOT_FOUND)
    {
        EC_T_DWORD dwSize  = 0;
        EC_T_DWORD dwValue = 0;

        pMySlave = &S_aSlaveList[S_dwSlaveIdx4132];    

        dwRes = ecatCoeSdoUpload(pMySlave->dwSlaveId, 0x1018, 1,
            (EC_T_BYTE*)&dwValue, sizeof(EC_T_DWORD), &dwSize, MBX_TIMEOUT, 0);

        if (EC_E_NOERROR != dwRes)
        {
            LogError("myAppDiagnosis: error in COE SDO Upload of object 0x1018! %s (0x%x)", ecatGetText(dwRes), dwRes);
        }
    }

    return EC_E_NOERROR;
}

/********************************************************************************/
/** \brief  Handler for application notifications
*
*  !!! No blocking API shall be called within this function!!! 
*  !!! Function is called by cylic task                    !!! 
*
* \return  Status value.
*/
static EC_T_DWORD myAppNotify(
    EC_T_DWORD              dwCode,     /* [in]  Application notification code */
    EC_T_NOTIFYPARMS*       pParms      /* [in]  Notification parameters */
    )
{
    EC_T_DWORD dwRetVal = EC_E_ERROR;

    EC_UNREFPARM(pParms);

    /* dispatch notification code */
    switch(dwCode)
    {
    case 1:
        LogMsg("Application notification code=%d received\n", dwCode);
        /* dwRetVal = EC_E_NOERROR; */
        break;
    case 2:
        break;
    default:
        break;
    }

    return dwRetVal;
}

/*-END OF SOURCE FILE--------------------------------------------------------*/
