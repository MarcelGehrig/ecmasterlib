/*-----------------------------------------------------------------------------
 * Logging.cpp
 * Copyright                acontis technologies GmbH, Weingarten, Germany
 * Response                 Stefan Zintgraf
 * Description              EtherCAT Master application logging
 *---------------------------------------------------------------------------*/

/*-INCLUDES------------------------------------------------------------------*/
#include <Logging.h>
#include "EcError.h"
#include <string.h>
#include <stdlib.h>

#ifdef VXWORKS
#include "vxWorks.h"
#include "sysLib.h"
#include "tickLib.h"
#if ((defined _WRS_VXWORKS_MAJOR) && (defined _WRS_VXWORKS_MINOR) && ( (_WRS_VXWORKS_MAJOR >= 7) || ((_WRS_VXWORKS_MAJOR == 6) && (_WRS_VXWORKS_MINOR >= 5)) ))
#else
#include <ifLib.h>
#endif
#endif /* VXWORKS */

#if (defined __MET__)
#include <ConfigerTasks.h>
#endif

/*-MACROS--------------------------------------------------------------------*/
/*#define NOPRINTF    1*/

/*-DEFINES-------------------------------------------------------------------*/
#if !(defined EC_DEMO_TINY)

#define MAX_MESSAGE_SIZE             512 /* maximum size of a single message */
#define DEFAULT_ERR_MSG_BUFFER_SIZE  500 /* number of buffered messages */
#define DEFAULT_LOG_MSG_BUFFER_SIZE 1000 /* number of buffered messages */
#define DEFAULT_DCM_MSG_BUFFER_SIZE 1000 /* number of buffered messages */

#else

#define MAX_MESSAGE_SIZE             100 /* maximum size of a single message */
#define DEFAULT_ERR_MSG_BUFFER_SIZE  20 /* number of buffered messages */
#define DEFAULT_LOG_MSG_BUFFER_SIZE  30 /* number of buffered messages */
#define DEFAULT_DCM_MSG_BUFFER_SIZE  4 /* number of buffered messages */

#endif /* !(defined EC_DEMO_TINY) */

#if (defined UNDER_RTSS) || (defined __INTIME__)
    #define ABSOLUTE_LOG_FILE_PATH       "C:\\"
#else
    #define ABSOLUTE_LOG_FILE_PATH       ""
#endif
#if (defined EC_VERSION_ECOS)
	#define FILESYS_8_3
#endif
#ifdef FILESYS_8_3
#define REM_ERRLOG_FILNAM       "rer"
#define LOC_ERRLOG_FILNAM       "er"
#define REM_MASTER_LOG_FILNAM   "rma"
#define LOC_MASTER_LOG_FILNAM   "ma"
#define REM_DCM_LOG_FILNAM      "rdc"
#define LOC_DCM_LOG_FILNAM      "dc"
#else
#define REM_ERRLOG_FILNAM       "rerror"
#define LOC_ERRLOG_FILNAM       (EC_T_CHAR*)"error"
#define REM_MASTER_LOG_FILNAM   "recmaster"
#define LOC_MASTER_LOG_FILNAM   (EC_T_CHAR*)"ecmaster"
#define REM_DCM_LOG_FILNAM      "rdcmlog"
#define LOC_DCM_LOG_FILNAM      (EC_T_CHAR*)"dcmlog"
#endif

#if (defined INCLUDE_ATEMRAS)
#define ERRLOG_FILNAM       REM_ERRLOG_FILNAM
#define MASTER_LOG_FILNAM   REM_MASTER_LOG_FILNAM
#define DCM_LOG_FILNAM      REM_DCM_LOG_FILNAM
#else
#define ERRLOG_FILNAM       LOC_ERRLOG_FILNAM
#define MASTER_LOG_FILNAM   LOC_MASTER_LOG_FILNAM
#define DCM_LOG_FILNAM      LOC_DCM_LOG_FILNAM
#endif


/*-GLOBAL VARIABLES-----------------------------------------------------------*/
#if (defined VXWORKS) || (defined __TKERNEL) || (defined RTAI)\
    || (defined EC_VERSION_SYSBIOS) || (defined EC_VERSION_RIN32M3) || (defined EC_VERSION_XILINX_STANDALONE)\
    || (defined EC_VERSION_ETKERNEL) || (defined EC_VERSION_RZT1) || (defined EC_VERSION_RZGNOOS) || (defined EC_VERSION_ECOS) ||\
    (defined EC_VERSION_JSLWARE)
EC_T_BOOL bLogFileEnb = EC_FALSE;
#else
EC_T_BOOL bLogFileEnb = EC_TRUE;
#endif

/*-LOCAL VARIABLES-----------------------------------------------------------*/

CAtEmLogging* G_pOsDbgMsgLoggingInst = EC_NULL;


/*-FORWARD DECLARATIONS------------------------------------------------------*/


/***************************************************************************************************/
/**
\brief  Create CAtEmLogging instance

\return -
*/
CAtEmLogging::CAtEmLogging(EC_T_VOID)
{
    m_pvLogThreadObj    = EC_NULL;
    m_bLogTaskRunning = EC_FALSE;
    m_bShutdownLogTask = EC_FALSE;
    m_bDbgMsgHookEnable = EC_TRUE;
    m_bSettling = EC_FALSE;
    m_dwNumMsgsSinceMsrmt = 0;
    m_poInsertMsgLock = EC_NULL;
    m_poProcessMsgLock = EC_NULL;
    m_pchTempbuffer = EC_NULL;
    m_pFirstMsgBufferDesc = EC_NULL;
    m_pLastMsgBufferDesc = EC_NULL;
    m_pAllMsgBufferDesc = EC_NULL;
    m_pErrorMsgBufferDesc = EC_NULL;
    m_pDcmMsgBufferDesc = EC_NULL;
    m_pchLogDir[0] = '\0';
    m_pchLogDir[MAX_PATH_LEN - 1] = '\0';
}


/********************************************************************************/
/** \brief Initialize logging
*
* \return N/A
*/
EC_T_VOID CAtEmLogging::InitLogging(
    EC_T_DWORD  dwMasterID,
    EC_T_WORD   wRollOver,
    EC_T_DWORD  dwPrio,
    EC_T_DWORD  dwCpuIndex,
    EC_T_CHAR*  szFilenamePrefix,
    EC_T_DWORD  dwStackSize
    )
{
    EC_T_CPUSET CpuSet;
#if !(defined XENOMAI)
    EC_T_BOOL   bOk;
#endif
    EC_T_CHAR   szLogFilename[256];

    if (EC_NULL == G_pOsDbgMsgLoggingInst )
    {
        G_pOsDbgMsgLoggingInst = this;
    }
    m_poInsertMsgLock = OsCreateLockTyped(eLockType_SPIN);
    m_poProcessMsgLock = OsCreateLock();
    m_pchTempbuffer = (EC_T_CHAR*)OsMalloc(2*MAX_MESSAGE_SIZE);
    if (EC_NULL == m_pchTempbuffer)
    {
        LogError("InitLogging: not enough memory for m_pchTempbuffer\n");
    }

    /* zero-terminate log msg (snprintf doesn't include this) */
    m_pchTempbuffer[0] = '\0';
    m_pchTempbuffer[2*MAX_MESSAGE_SIZE - 1] = '\0';

    if ((EC_NULL != szFilenamePrefix) && ('\0' != szFilenamePrefix[0]))
    {
        OsSnprintf(szLogFilename, sizeof(szLogFilename) - 1, "%s_err", szFilenamePrefix);
    }
    else
    {
        OsSnprintf(szLogFilename, sizeof(szLogFilename) - 1, "%s", ERRLOG_FILNAM);
    }
    m_pErrorMsgBufferDesc = (MSG_BUFFER_DESC*)AddLogBuffer(
                                                    dwMasterID,
                                                    wRollOver,
                                                    DEFAULT_ERR_MSG_BUFFER_SIZE,
                                                    EC_TRUE,            /* skip duplicates */
                                                    (EC_T_CHAR*)"Err",  /* name of the logging (identification) */
                                                    szLogFilename,
                                                    (EC_T_CHAR*)"log",  /* log file extension */
                                                    EC_FALSE,           /* print message on console? */
                                                    EC_TRUE );          /* logging with time stamp? */
    if (EC_NULL == m_pErrorMsgBufferDesc)
    {
        LogError("InitLogging: not enough memory for m_pErrorMsgBufferDesc\n");
    }

    if ((EC_NULL != szFilenamePrefix) && ('\0' != szFilenamePrefix[0]))
    {
        OsSnprintf(szLogFilename, sizeof(szLogFilename) - 1, "%s", szFilenamePrefix);
    }
    else
    {
        OsSnprintf(szLogFilename, sizeof(szLogFilename) - 1, "%s", MASTER_LOG_FILNAM);
    }
    m_pAllMsgBufferDesc = (MSG_BUFFER_DESC*)AddLogBuffer(
                                                    dwMasterID,
                                                    wRollOver,
                                                    DEFAULT_LOG_MSG_BUFFER_SIZE,
                                                    EC_TRUE,            /* skip duplicates */
                                                    (EC_T_CHAR*)"Log",  /* name of the logging (identification) */
                                                    szLogFilename,
                                                    (EC_T_CHAR*)"log",  /* log file extension */
                                                    EC_TRUE,            /* print message on console? */
                                                    EC_TRUE );          /* logging with time stamp? */
    if (EC_NULL == m_pAllMsgBufferDesc)
    {
        LogError("InitLogging: not enough memory for m_pAllMsgBufferDesc\n");
    }

    if ((EC_NULL != szFilenamePrefix) && ('\0' != szFilenamePrefix[0]))
    {
        OsSnprintf(szLogFilename, sizeof(szLogFilename) - 1, "%s_dcm", szFilenamePrefix);
    }
    else
    {
        OsSnprintf(szLogFilename, sizeof(szLogFilename) - 1, "%s", DCM_LOG_FILNAM);
    }
    m_pDcmMsgBufferDesc = (MSG_BUFFER_DESC*)AddLogBuffer(
                                                    dwMasterID,
                                                    wRollOver,
                                                    DEFAULT_DCM_MSG_BUFFER_SIZE,
                                                    EC_FALSE,           /* do not skip duplicates */
                                                    (EC_T_CHAR*)"DCM",  /* name of the logging (identification) */
                                                    szLogFilename,
                                                    (EC_T_CHAR*)"csv",  /* log file extension */
                                                    EC_FALSE,           /* print message on console? */
                                                    EC_FALSE );         /* logging with time stamp? */
    if (EC_NULL == m_pDcmMsgBufferDesc)
    {
        LogError("InitLogging: not enough memory for m_pDcmMsgBufferDesc\n");
    }

    OsDbgAssert(!m_bLogTaskRunning);
    m_bShutdownLogTask = EC_FALSE;
    EC_CPUSET_ZERO(CpuSet);
    EC_CPUSET_SET(CpuSet, dwCpuIndex);
#ifndef NO_OS
#if (defined __MET__)
    m_pvLogThreadObj = (EC_T_VOID*)_task_create((EC_T_WORD)0, (EC_T_DWORD)TECAT_LOGGING_INDEX, (EC_T_DWORD)this);
#elif (defined XENOMAI)
    /* for Xenomai, pass the CPU affinity as upper 16bit of thread priority */
    m_pvLogThreadObj = OsCreateThread( (EC_T_CHAR*)"tAtEmLog", tAtEmLogWrapper, (CpuSet << 16) | dwPrio, dwStackSize, this );
#else
    m_pvLogThreadObj = OsCreateThread( (EC_T_CHAR*)"tAtEmLog", tAtEmLogWrapper, dwPrio, dwStackSize, this );
#endif
    while (!m_bLogTaskRunning) OsSleep(1);
#endif

#if !(defined XENOMAI)
    /* for Xenomai, the CPU affinity is set during task creating, see above */
    bOk = OsSetThreadAffinity( m_pvLogThreadObj, CpuSet );
    if (!bOk)
    {
        LogError("Error: Set log task affinitiy, invalid CPU index %d\n", 0);
    }
#endif /* !XENOMAI */
}

/********************************************************************************/
/** \brief Initialize logging
*
* \return message buffer descriptor
*/
struct _MSG_BUFFER_DESC* CAtEmLogging::AddLogBuffer(
    EC_T_DWORD  dwMasterID,
    EC_T_WORD   wRollOver,
    EC_T_DWORD  dwBufferSize,       /* [in]  buffer size (number of buffered messages) */
    EC_T_BOOL   bSkipDuplicates,    /* [in]  EC_TRUE if duplicate messages shall be skipped */
    EC_T_CHAR*  szLogName,          /* [in]  name of the logging (identification) */
    EC_T_CHAR*  szLogFilename,      /* [in]  log filename */
    EC_T_CHAR*  szLogFileExt,       /* [in]  log file extension */
    EC_T_BOOL   bPrintConsole,      /* [in]  print message on console? */
    EC_T_BOOL   bPrintTimestamp     /* [in]  logging with time stamp? */
    )
{
    EC_T_CHAR   szLogFileNameTmp[MAX_PATH_LEN];
    MSG_BUFFER_DESC* pNewMsgBufferDesc = EC_NULL;
    EC_T_BOOL bLocked = EC_FALSE;
    EC_T_BOOL bOk = EC_FALSE;

    OsLock( m_poProcessMsgLock );
    bLocked = EC_TRUE;

    /* create buffers */
    pNewMsgBufferDesc = (MSG_BUFFER_DESC*)OsMalloc(sizeof(MSG_BUFFER_DESC));
    if (pNewMsgBufferDesc == EC_NULL )
    {
        LogError( "AddLogBuffer: cannot allocate message buffer descriptor\n" );
        goto Exit;
    }
    OsMemset(pNewMsgBufferDesc,   0, sizeof(MSG_BUFFER_DESC));

    if (m_pchLogDir[0] != '\0')
    {
        OsSnprintf(szLogFileNameTmp, sizeof(szLogFileNameTmp)-1, "%s%s%d", m_pchLogDir, szLogFilename, dwMasterID);
    }
    else
    {
        OsSnprintf(szLogFileNameTmp, sizeof(szLogFileNameTmp)-1, "%s%s%d", ABSOLUTE_LOG_FILE_PATH, szLogFilename, dwMasterID);
    }
    bOk = InitMsgBuffer( pNewMsgBufferDesc,
                         MAX_MESSAGE_SIZE,
                         dwBufferSize,
                         bSkipDuplicates,
                         bPrintConsole,
                         bPrintTimestamp,
                         szLogFileNameTmp,
                         szLogFileExt,
                         wRollOver,
                         szLogName
                        );
    if (!bOk)
    {
        goto Exit;
    }

    /* link buffer together */
    if (m_pLastMsgBufferDesc == EC_NULL )
    {
        /* create first buffer */
        OsDbgAssert( m_pFirstMsgBufferDesc == EC_NULL );
        m_pFirstMsgBufferDesc = m_pLastMsgBufferDesc = pNewMsgBufferDesc;
        pNewMsgBufferDesc->pNextMsgBuf = EC_NULL;
    }
    else
    {
        /* append to last buffer */
        OsDbgAssert( m_pLastMsgBufferDesc->pNextMsgBuf == EC_NULL );
        m_pLastMsgBufferDesc->pNextMsgBuf = pNewMsgBufferDesc;
        m_pLastMsgBufferDesc = pNewMsgBufferDesc;
    }
    bOk = EC_TRUE;

Exit:
    if (!bOk)
    {
        if (pNewMsgBufferDesc!=EC_NULL) OsFree(pNewMsgBufferDesc);
        pNewMsgBufferDesc = EC_NULL;
    }

    if (bLocked)
        OsUnlock( m_poProcessMsgLock );
    return pNewMsgBufferDesc;
}


/********************************************************************************/
/** \brief set log message buffer
*
* \return N/A
*/
EC_T_VOID CAtEmLogging::SetMsgBuf(
    MSG_BUFFER_DESC* pMsgBufferDesc,
    EC_T_BYTE* pbyLogMem,
    EC_T_DWORD dwSize )
{
    pMsgBufferDesc->pbyNextLogMsg = pbyLogMem;
    pMsgBufferDesc->dwLogMemorySize = dwSize;
    pMsgBufferDesc->bLogBufferFull = EC_FALSE;
    pMsgBufferDesc->bNewLine = EC_TRUE;
    OsMemset(pbyLogMem, 0, dwSize);
    pMsgBufferDesc->pbyLogMemory = pbyLogMem;   /* initialize last as it will be active immediately after! */
}
EC_T_VOID CAtEmLogging::SetLogMsgBuf( EC_T_BYTE* pbyLogMem, EC_T_DWORD dwSize )
{
    SetMsgBuf( m_pAllMsgBufferDesc, pbyLogMem, dwSize );
}
EC_T_VOID CAtEmLogging::SetLogErrBuf( EC_T_BYTE* pbyLogMem, EC_T_DWORD dwSize )
{
    SetMsgBuf( m_pErrorMsgBufferDesc, pbyLogMem, dwSize );
}
EC_T_VOID CAtEmLogging::SetLogDcmBuf( EC_T_BYTE* pbyLogMem, EC_T_DWORD dwSize )
{
    SetMsgBuf( m_pDcmMsgBufferDesc, pbyLogMem, dwSize );
}

/********************************************************************************/
/** \brief De-initialize logging
*
* \return N/A
*/
EC_T_VOID CAtEmLogging::DeinitLogging( EC_T_VOID )
{
MSG_BUFFER_DESC* pCurrMsgBuf;
MSG_BUFFER_DESC* pNextMsgBuf;

    if (G_pOsDbgMsgLoggingInst == this)
    {
        G_pOsDbgMsgLoggingInst = EC_NULL;
    }

    m_bShutdownLogTask = EC_TRUE;

    while (m_bLogTaskRunning) OsSleep(1);

    /* shutdown all message buffers */
    pNextMsgBuf = m_pFirstMsgBufferDesc;
    while (EC_NULL != pNextMsgBuf)
    {
        DeinitMsgBuffer(pNextMsgBuf);
        pNextMsgBuf = pNextMsgBuf->pNextMsgBuf;
    }

#if (defined __MET__)
    _task_destroy( (_task_id) m_pvLogThreadObj);
#else
    OsDeleteThreadHandle(m_pvLogThreadObj);
#endif

    m_pvLogThreadObj = EC_NULL;

    /* free all message buffers */
    pNextMsgBuf = m_pFirstMsgBufferDesc;
    while (EC_NULL != pNextMsgBuf)
    {
        pCurrMsgBuf = pNextMsgBuf;
        pNextMsgBuf = pCurrMsgBuf->pNextMsgBuf;
        OsFree(pCurrMsgBuf);
    }
    /* unlink buffers */
    m_pFirstMsgBufferDesc = EC_NULL;
    m_pLastMsgBufferDesc = EC_NULL;
    m_pAllMsgBufferDesc = EC_NULL;
    m_pErrorMsgBufferDesc = EC_NULL;
    m_pDcmMsgBufferDesc = EC_NULL;

    OsDeleteLock(m_poInsertMsgLock);
    OsDeleteLock(m_poProcessMsgLock);
    SafeOsFree(m_pchTempbuffer);
    m_pchTempbuffer = EC_NULL;
}


/********************************************************************************/
/** \brief logging thread
*
* \return N/A
*/
EC_T_VOID CAtEmLogging::tAtEmLogWrapper(EC_T_VOID* pvParm)
{
    CAtEmLogging *pInst = (CAtEmLogging*)pvParm;

    OsDbgAssert(EC_NULL != pInst);
    if (pInst)
    {
        pInst->tAtEmLog(EC_NULL);
    }
}


/********************************************************************************/
/** \brief cyclically process all messages
*
* \return N/A
*/
EC_T_VOID CAtEmLogging::tAtEmLog(EC_T_VOID* pvParm)
{
    EC_UNREFPARM(pvParm);

    m_bLogTaskRunning = EC_TRUE;
    while (!m_bShutdownLogTask)
    {
        ProcessAllMsgs();
        OsSleep(1);
    }
    ProcessAllMsgs();
    m_bLogTaskRunning = EC_FALSE;
#if (defined EC_VERSION_RTEMS)
    rtems_task_delete(RTEMS_SELF);
#endif
}

EC_T_VOID CAtEmLogging::ProcessAllMsgs(EC_T_VOID)
{
MSG_BUFFER_DESC* pNextMsgBuf;

    pNextMsgBuf = m_pFirstMsgBufferDesc;
    while (EC_NULL != pNextMsgBuf)
    {
        ProcessMsgs(pNextMsgBuf);
        pNextMsgBuf = pNextMsgBuf->pNextMsgBuf;
    }
}

/********************************************************************************/
/** \brief Initialize message buffer
*
* \return N/A
*/
EC_T_BOOL CAtEmLogging::InitMsgBuffer
(MSG_BUFFER_DESC*   pMsgBufferDesc      /* [in]  pointer to message buffer descriptor */
,EC_T_DWORD         dwMsgSize           /* [in]  size of a single message */
,EC_T_DWORD         dwNumMsgs           /* [in]  number of messages */
,EC_T_BOOL          bSkipDuplicates     /* [in]  EC_TRUE if duplicate messages shall be skipped */
,EC_T_BOOL          bPrintConsole       /* [in]  print message on console? */
,EC_T_BOOL          bPrintTimestamp     /* [in]  logging with time stamp? */
,EC_T_CHAR*         szMsgLogFileName    /* [in]  message log file name */
,EC_T_CHAR*         szMsgLogFileExt     /* [in]  message log file name */
,EC_T_WORD          wRollOver           /* [in]  roll over counter */
,EC_T_CHAR*         szBufferName        /* [in]  name of the logging buffer */
)
{
    EC_T_BOOL  bOk = EC_FALSE;
    EC_T_CHAR* pchMsgBuffer = EC_NULL;
    EC_T_DWORD dwBufSiz;
    EC_T_DWORD dwCnt;
#if (!(defined __RCX__) || (defined __MET__)) && !(defined RTAI)
    EC_T_CHAR  szfileNameTemp[MAX_PATH_LEN] = {0};
#endif

    pMsgBufferDesc->dwMsgSize = dwMsgSize;
    pMsgBufferDesc->dwNumMsgs = dwNumMsgs;
    pMsgBufferDesc->bPrintTimestamp = bPrintTimestamp;
    pMsgBufferDesc->bPrintConsole = bPrintConsole;
    pMsgBufferDesc->dwNextEmptyMsgIndex = 0;
    pMsgBufferDesc->dwNextPrintMsgIndex = 0;
    pMsgBufferDesc->wEntryCounter       = 0;
    pMsgBufferDesc->pbyLogMemory = EC_NULL;
    pMsgBufferDesc->dwLogMemorySize = 0;
    pMsgBufferDesc->pbyNextLogMsg = EC_NULL;
    pMsgBufferDesc->bLogBufferFull = EC_FALSE;
    pMsgBufferDesc->bSkipDuplicateMessages = bSkipDuplicates;
    pMsgBufferDesc->dwNumDuplicates = 0;
    pMsgBufferDesc->pszLastMsg = EC_NULL;

    pMsgBufferDesc->paMsg = (LOG_MSG_DESC*)OsMalloc(dwNumMsgs*sizeof(LOG_MSG_DESC));
    if (pMsgBufferDesc->paMsg == EC_NULL)
    {
        OsPrintf("CAtEmLogging::InitMsgBuffer: cannot get memory for logging buffer '%s'\n", szBufferName);
        goto Exit;
    }
    OsMemset(pMsgBufferDesc->paMsg, 0, dwNumMsgs*sizeof(LOG_MSG_DESC));

    dwBufSiz = dwNumMsgs * (dwMsgSize + 1);
    pchMsgBuffer = (EC_T_CHAR*)OsMalloc(dwBufSiz);
    if (pchMsgBuffer == EC_NULL)
    {
        OsPrintf("CAtEmLogging::InitMsgBuffer: cannot get memory for logging buffer '%s'\n", szBufferName);
        goto Exit;
    }

    /* Same as below. Needed to prevent false positive from static code analysis. */
    pMsgBufferDesc->paMsg[0].szMsgBuffer = pchMsgBuffer;

    OsMemset(pchMsgBuffer,0,dwBufSiz);
    for( dwCnt=0; dwCnt < dwNumMsgs; dwCnt++ )
    {
        pMsgBufferDesc->paMsg[dwCnt].szMsgBuffer = &pchMsgBuffer[dwCnt*(dwMsgSize+1)];
        pMsgBufferDesc->paMsg[dwCnt].bMsgCrLf = EC_TRUE;
    }

#if (defined __RCX__) || (defined __MET__) || (defined RTAI)
    pMsgBufferDesc->pfMsgFile = EC_NULL;
#else
    pMsgBufferDesc->wLogFileIndex    = 0;
    pMsgBufferDesc->wEntryCounterLimit = wRollOver;
    OsStrncpy(pMsgBufferDesc->szMsgLogFileName, szMsgLogFileName, sizeof(pMsgBufferDesc->szMsgLogFileName) - 1);
    OsStrncpy(pMsgBufferDesc->szMsgLogFileExt,  szMsgLogFileExt,  sizeof(pMsgBufferDesc->szMsgLogFileExt) - 1);

    if (0 != pMsgBufferDesc->wEntryCounterLimit )
    {
#ifdef FILESYS_8_3
        OsSnprintf(szfileNameTemp, sizeof(szfileNameTemp) - 1, "%s_%x.%s", pMsgBufferDesc->szMsgLogFileName, pMsgBufferDesc->wLogFileIndex, pMsgBufferDesc->szMsgLogFileExt);
#else
        OsSnprintf(szfileNameTemp, sizeof(szfileNameTemp) - 1, "%s.%x.%s", pMsgBufferDesc->szMsgLogFileName, pMsgBufferDesc->wLogFileIndex, pMsgBufferDesc->szMsgLogFileExt);
#endif
    }
    else
    {
        OsSnprintf(szfileNameTemp, sizeof(szfileNameTemp) - 1, "%s.%s", pMsgBufferDesc->szMsgLogFileName, pMsgBufferDesc->szMsgLogFileExt);
    }

    if (bLogFileEnb)
    {
        pMsgBufferDesc->pfMsgFile = OsFopen( szfileNameTemp, "w+" );
        if (pMsgBufferDesc->pfMsgFile == EC_NULL )
        {
#if !(defined NOPRINTF)
            OsPrintf( "ERROR: cannot create EtherCAT log file %s\n", szfileNameTemp );
#endif
            OsSleep(3000);
        }
    }
    else
    {
        pMsgBufferDesc->pfMsgFile = EC_NULL;
        pMsgBufferDesc->szMsgLogFileName[0] = 0;
    }
    OsStrncpy(pMsgBufferDesc->szLogName, szBufferName, MAX_PATH_LEN - 1);

#endif
    pMsgBufferDesc->bIsInitialized = EC_TRUE;
    bOk = EC_TRUE;

Exit:
    if (!bOk)
    {
        if (pMsgBufferDesc->paMsg!=EC_NULL)
        {
            OsFree(pMsgBufferDesc->paMsg);
        }
        if (pchMsgBuffer == EC_NULL)
        {
            OsFree(pchMsgBuffer);
        }
    }
    return bOk;
}


/********************************************************************************/
/** \brief De-Init message buffer
*
* \return N/A
*/
EC_T_VOID CAtEmLogging::DeinitMsgBuffer
(MSG_BUFFER_DESC*   pMsgBufferDesc
)
{
CEcTimer oTimeout;

    if (pMsgBufferDesc->bIsInitialized)
    {
        /* let the log task print out all messages */
        if (pMsgBufferDesc->dwNextPrintMsgIndex != pMsgBufferDesc->dwNextEmptyMsgIndex)
        {
            OsPrintf("Store unsaved messages in '%s' message/logging buffer...", pMsgBufferDesc->szLogName);
            oTimeout.Start(3000);
            while (pMsgBufferDesc->dwNextPrintMsgIndex != pMsgBufferDesc->dwNextEmptyMsgIndex)
            {
                ProcessAllMsgs();
                OsSleep(100);
                if (oTimeout.IsElapsed())
                {
                    OsPrintf(".");
                    oTimeout.Start(3000);
                }
            }
            OsPrintf(" done!\n");
        }

        OsFree(pMsgBufferDesc->paMsg[0].szMsgBuffer);
        OsFree(pMsgBufferDesc->paMsg);

        if (EC_NULL != pMsgBufferDesc->pfMsgFile)
        {
            OsFclose(pMsgBufferDesc->pfMsgFile);
        }

        pMsgBufferDesc->pfMsgFile = EC_NULL;
        pMsgBufferDesc->dwNextEmptyMsgIndex = 0;
        pMsgBufferDesc->dwNextPrintMsgIndex = 0;
        pMsgBufferDesc->bIsInitialized = EC_FALSE;
        pMsgBufferDesc->pbyLogMemory = EC_NULL;
        pMsgBufferDesc->dwLogMemorySize = 0;
        pMsgBufferDesc->pbyNextLogMsg = EC_NULL;
        pMsgBufferDesc->bLogBufferFull = EC_FALSE;
        pMsgBufferDesc->pszLastMsg = EC_NULL;
    }
}

/********************************************************************************/
/** \brief Insert a new message into message buffer
*
* \return N/A
*/
EC_T_DWORD CAtEmLogging::InsertNewMsg
(MSG_BUFFER_DESC*   pMsgBufferDesc
,const
 EC_T_CHAR*         szFormat
,EC_T_VALIST        vaArgs
,EC_T_BOOL          bDoCrLf
,EC_T_BOOL          bOsDbgMsg
)
{
    EC_T_DWORD      dwRes = EC_E_NOERROR;
    EC_T_BOOL       bBufferFull = EC_FALSE;
    EC_T_DWORD      dwTimeStamp     = 0;
    LOG_MSG_DESC*   pNewMsg = EC_NULL;

    if (pMsgBufferDesc == EC_NULL )
        goto Exit;
    if (!pMsgBufferDesc->bIsInitialized)
    {
        dwRes = EC_E_INVALIDSTATE;
        goto Exit;
    }

    if (m_bShutdownLogTask )
        goto Exit;

    OsLock(m_poInsertMsgLock);
    {
        EC_T_DWORD  dwNewNextEmpty  = 0;

        pNewMsg = &pMsgBufferDesc->paMsg[pMsgBufferDesc->dwNextEmptyMsgIndex];

        dwNewNextEmpty = pMsgBufferDesc->dwNextEmptyMsgIndex + 1;
        if (dwNewNextEmpty >= pMsgBufferDesc->dwNumMsgs )
        {
            dwNewNextEmpty = 0;
        }

        /* check if message buffer is full ? */
        if (dwNewNextEmpty == pMsgBufferDesc->dwNextPrintMsgIndex )
        {
            bBufferFull = EC_TRUE;
        }
        else
        {
            pMsgBufferDesc->dwNextEmptyMsgIndex = dwNewNextEmpty;
        }
    }
    OsUnlock(m_poInsertMsgLock);


    if (bBufferFull)
    {
        dwRes = EC_E_NOMEMORY;
        goto Exit;
    }


    if (pMsgBufferDesc->bPrintTimestamp )
    {
        dwTimeStamp = OsQueryMsecCount();
    }
    pNewMsg->dwMsgTimestamp  = dwTimeStamp;
    pNewMsg->bOsDbgMsg       = bOsDbgMsg;
    pNewMsg->bMsgCrLf        = bDoCrLf;

    pNewMsg->dwLen = EcVsnprintf(pNewMsg->szMsgBuffer, pMsgBufferDesc->dwMsgSize, szFormat,vaArgs);

    /* mark entry as complete */
    OsMemoryBarrier();
    pNewMsg->bValid = EC_TRUE;

Exit:
    return dwRes;
}


/********************************************************************************/
/** \brief Forward to next logging buffer (memory logging)
*
* \return N/A
*/
EC_T_VOID CAtEmLogging::SelectNextLogMemBuffer
(MSG_BUFFER_DESC*   pMsgBufferDesc )
{
    pMsgBufferDesc->pbyNextLogMsg = pMsgBufferDesc->pbyNextLogMsg + OsStrlen(pMsgBufferDesc->pbyNextLogMsg);
    if (pMsgBufferDesc->pbyNextLogMsg >= (pMsgBufferDesc->pbyLogMemory + pMsgBufferDesc->dwLogMemorySize - 3*MAX_MESSAGE_SIZE) )
    {
        /* stop logging if memory is full */
        OsDbgMsg( "logging buffer %s is full, logging stopped!\n", pMsgBufferDesc->szLogName );
        pMsgBufferDesc->bLogBufferFull = EC_TRUE;
    }

    /* zero-terminate log msg (snprintf doesn't include this) */
    pMsgBufferDesc->pbyNextLogMsg[0] = '\0';
    pMsgBufferDesc->pbyNextLogMsg[MAX_MESSAGE_SIZE - 1] = '\0';
}

/********************************************************************************/
/** \brief Process all messages of a message buffer
*
* \return N/A
*/
EC_T_VOID CAtEmLogging::ProcessMsgs
(MSG_BUFFER_DESC*   pMsgBufferDesc )
{
    EC_T_DWORD  dwNewNextPrint=0;
    LOG_MSG_DESC*   pCurrMsg = EC_NULL;
    EC_T_BOOL bLocked = EC_FALSE;
#if (!(defined __RCX__) || (defined __MET__)) && !(defined RTAI)
    FILE*       pFileHandle     = EC_NULL;
    EC_T_BOOL   bRollOver       = EC_FALSE;
    EC_T_CHAR  szfileNameTemp[MAX_PATH_LEN];
#endif
    EC_T_DWORD  dwNumMsgLeft = 20;
    EC_T_BOOL   bSkipDuplicate;
    EC_T_DWORD  dwNumDuplicatesBeforeNewMsg;

    szfileNameTemp[0] = '\0';

    if (pMsgBufferDesc->bIsInitialized )
    {
        OsLock(m_poProcessMsgLock);
        bLocked = EC_TRUE;
        while( pMsgBufferDesc->dwNextPrintMsgIndex != pMsgBufferDesc->dwNextEmptyMsgIndex )
        {
            OsDbgAssert(pMsgBufferDesc->bIsInitialized);

            // return after maximum number of processed messages
            if (dwNumMsgLeft == 0 )
                break;

            pCurrMsg = &pMsgBufferDesc->paMsg[pMsgBufferDesc->dwNextPrintMsgIndex];
            /* wait til message is complete */
            if (!pCurrMsg->bValid)
            {
                break;
            }

#if (!(defined __RCX__) || (defined __MET__)) && !(defined RTAI)
            if (bLogFileEnb && (pMsgBufferDesc->pfMsgFile != EC_NULL) )
            {
                pFileHandle = pMsgBufferDesc->pfMsgFile;

                pMsgBufferDesc->wEntryCounter++;
                if (0 != pMsgBufferDesc->wEntryCounterLimit && pMsgBufferDesc->pfMsgFile != EC_NULL )
                {
                    if (pMsgBufferDesc->wEntryCounter >= pMsgBufferDesc->wEntryCounterLimit )
                    {
                        bRollOver = EC_TRUE;
                        pMsgBufferDesc->wEntryCounter = 0;
                        pMsgBufferDesc->wLogFileIndex++;

#ifdef FILESYS_8_3
                        OsSnprintf(szfileNameTemp, sizeof(szfileNameTemp) - 1, "%s_%x.%s", pMsgBufferDesc->szMsgLogFileName, pMsgBufferDesc->wLogFileIndex, pMsgBufferDesc->szMsgLogFileExt);
#else
                        OsSnprintf(szfileNameTemp, sizeof(szfileNameTemp) - 1, "%s.%x.%s", pMsgBufferDesc->szMsgLogFileName, pMsgBufferDesc->wLogFileIndex, pMsgBufferDesc->szMsgLogFileExt);
#endif
                    }
                    else
                    {
                        bRollOver = EC_FALSE;
                    }

                }
            }
#endif

            /* handle skipping duplicates */
            bSkipDuplicate = EC_FALSE;
            dwNumDuplicatesBeforeNewMsg = 0;
            if (pMsgBufferDesc->bSkipDuplicateMessages )
            {
                if (pMsgBufferDesc->pszLastMsg == EC_NULL )
                {
                    /* first message */
                    pMsgBufferDesc->pszLastMsg = pCurrMsg->szMsgBuffer;
                }
                else if (OsStrncmp(pMsgBufferDesc->pszLastMsg, pCurrMsg->szMsgBuffer, pMsgBufferDesc->dwMsgSize) == 0 )
                {
                    /* same message as before, just increment duplicate pointer */
                    pMsgBufferDesc->dwNumDuplicates++;
                    bSkipDuplicate = EC_TRUE;;
                }
                else
                {
                    /* new message */
                    dwNumDuplicatesBeforeNewMsg = pMsgBufferDesc->dwNumDuplicates;
                    pMsgBufferDesc->dwNumDuplicates = 0;
                    pMsgBufferDesc->pszLastMsg = pCurrMsg->szMsgBuffer;
                    if (dwNumDuplicatesBeforeNewMsg>0 && pMsgBufferDesc->pszLastMsg[0]=='\0')
                    {
                        /* ignore empty duplicates... */
                        dwNumDuplicatesBeforeNewMsg = 0;
                    }
                }
            }
            if (!bSkipDuplicate) dwNumMsgLeft--;

            if (pMsgBufferDesc->bPrintConsole && !bSkipDuplicate)
            {
#if !(defined NOPRINTF)
                /* print skip messages */
                if (dwNumDuplicatesBeforeNewMsg > 0)
                {
                    if (pMsgBufferDesc->bPrintTimestamp)
                    {
                        OsPrintf("%06d : ", (EC_T_INT)pCurrMsg->dwMsgTimestamp);
                    }
                    OsPrintf( "%d identical messages skipped\n", dwNumDuplicatesBeforeNewMsg);
                }
                /* print timestamp */
                if ((pMsgBufferDesc->bPrintTimestamp) && (pMsgBufferDesc->bNewLine))
                {
                    OsPrintf("%06d : ", (EC_T_INT)pCurrMsg->dwMsgTimestamp);
                }
                /* print message */
                if (pCurrMsg->bMsgCrLf)
                {
                    /* e.g. RTX64 requires terminating new-line directly appended to the message */
                    OsPrintf("%s\n", pCurrMsg->szMsgBuffer);
                }
                else
                {
                    OsPrintf("%s", pCurrMsg->szMsgBuffer);
                }
#endif
            }

#if !(defined __RCX__) && !(defined __MET__) && !(defined RTAI)
            if (pMsgBufferDesc->pbyLogMemory != EC_NULL )
            {
                if (!pMsgBufferDesc->bLogBufferFull && !bSkipDuplicate)
                {
                EC_T_DWORD dwWritten = 0;

                    /* print skip messages */
                    if (dwNumDuplicatesBeforeNewMsg > 0 )
                    {
                        if (pMsgBufferDesc->bPrintTimestamp)
                        {
                            dwWritten = dwWritten + OsSnprintf((EC_T_CHAR*)(pMsgBufferDesc->pbyNextLogMsg + dwWritten), MAX_MESSAGE_SIZE - dwWritten - 1, "%06d : ", (EC_T_INT) pCurrMsg->dwMsgTimestamp);
                        }
                        dwWritten = dwWritten + OsSnprintf((EC_T_CHAR*)(pMsgBufferDesc->pbyNextLogMsg + dwWritten), MAX_MESSAGE_SIZE - dwWritten - 1, "%d identical messages skipped\n", dwNumDuplicatesBeforeNewMsg );
                        SelectNextLogMemBuffer(pMsgBufferDesc);
                    }
                    if (!pMsgBufferDesc->bLogBufferFull)
                    {
                        dwWritten = 0;

                        /* memory logging */
                        if ((pMsgBufferDesc->bPrintTimestamp) && (pMsgBufferDesc->bNewLine))
                        {
                            dwWritten = dwWritten + OsSnprintf((EC_T_CHAR*)pMsgBufferDesc->pbyNextLogMsg + dwWritten, MAX_MESSAGE_SIZE - dwWritten - 1, "%06d : ", (EC_T_INT)pCurrMsg->dwMsgTimestamp);
                        }
                        /* print message */
                        dwWritten = dwWritten + OsSnprintf((EC_T_CHAR*)(pMsgBufferDesc->pbyNextLogMsg + dwWritten), MAX_MESSAGE_SIZE - dwWritten - 1, "%s", pCurrMsg->szMsgBuffer);

                        /* add new line */
                        if (pCurrMsg->bMsgCrLf)
                        {
                            OsSnprintf((EC_T_CHAR*)(pMsgBufferDesc->pbyNextLogMsg + dwWritten), MAX_MESSAGE_SIZE - dwWritten - 1, "%s", "\n");
                        }
                        SelectNextLogMemBuffer(pMsgBufferDesc);
                    }
                }
            }
            else if (EC_NULL != pFileHandle)
            {
                /* don't use fprintf, some platforms don't support it! */

                if (!bSkipDuplicate)
                {
                    /* print skipped messages */
                    if (dwNumDuplicatesBeforeNewMsg > 0)
                    {
                        if (pMsgBufferDesc->bPrintTimestamp)
                        {
                            OsSnprintf(m_pchTempbuffer, 2*MAX_MESSAGE_SIZE - 1, "%06d : ", (EC_T_INT) pCurrMsg->dwMsgTimestamp);
                            OsFwrite(m_pchTempbuffer, OsStrlen(m_pchTempbuffer), 1, pFileHandle);
                        }
                        OsSnprintf(m_pchTempbuffer, 2*MAX_MESSAGE_SIZE - 1, "%d identical messages skipped\n", dwNumDuplicatesBeforeNewMsg);
                        OsFwrite(m_pchTempbuffer, OsStrlen(m_pchTempbuffer), 1, pFileHandle);
                    }
                    /* now fprintf, OsFflush, OsFclose etc. */
                    if ((pMsgBufferDesc->bPrintTimestamp) && (pMsgBufferDesc->bNewLine))
                    {
                        OsSnprintf(m_pchTempbuffer, 2*MAX_MESSAGE_SIZE - 1, "%06d : ", (EC_T_INT) pCurrMsg->dwMsgTimestamp);
                        OsFwrite(m_pchTempbuffer, OsStrlen(m_pchTempbuffer), 1, pFileHandle);
                    }
                    /* print message */
					OsSnprintf(m_pchTempbuffer, 2*MAX_MESSAGE_SIZE - 1, "%s", pCurrMsg->szMsgBuffer);
					OsFwrite(m_pchTempbuffer, OsStrlen(m_pchTempbuffer), 1, pFileHandle);

                    /* add new line */
                    if (pCurrMsg->bMsgCrLf)
                    {
                        OsSnprintf(m_pchTempbuffer, 2*MAX_MESSAGE_SIZE - 1, "\n");
					    OsFwrite(m_pchTempbuffer, OsStrlen(m_pchTempbuffer), 1, pFileHandle);
                    }
                    OsFflush(pFileHandle);
                }
            }
#endif
            /* check for new line */
            pMsgBufferDesc->bNewLine = EC_FALSE;
            if (pCurrMsg->bOsDbgMsg)
            {
                /* currently all messages generated within master stack contain '\n' */
                /* check for CrLf in message */
                if (*((EC_T_CHAR*)(pCurrMsg->szMsgBuffer + pCurrMsg->dwLen - 1)) == '\n')
                {
                    pMsgBufferDesc->bNewLine = EC_TRUE;
                }
            }
            else
            {
                if (pCurrMsg->bMsgCrLf)
                {
                    pMsgBufferDesc->bNewLine = EC_TRUE;
                }
            }
            pCurrMsg->bValid = EC_FALSE;
            OsMemoryBarrier();
            dwNewNextPrint = pMsgBufferDesc->dwNextPrintMsgIndex + 1;
            if (dwNewNextPrint >= pMsgBufferDesc->dwNumMsgs )
            {
                dwNewNextPrint = 0;
            }
            pMsgBufferDesc->dwNextPrintMsgIndex = dwNewNextPrint;

#if !(defined __RCX__) && !(defined __MET__) && !(defined RTAI)
            if (bRollOver)
            {
                /* do roll over */
                OsFclose(pFileHandle);

                pFileHandle = OsFopen( szfileNameTemp, "w+" );
                if (pFileHandle == EC_NULL )
                {
#if !(defined NOPRINTF)
                    OsPrintf( "ERROR: cannot create EtherCAT log file %s\n", szfileNameTemp );
#endif
                    OsSleep( 3000 );

                    pMsgBufferDesc->pfMsgFile = pFileHandle;
                }
                else
                {
                    pMsgBufferDesc->pfMsgFile = pFileHandle;
                }

                bRollOver = EC_FALSE;
            }
#endif
        }
    }
    if (bLocked )
        OsUnlock(m_poProcessMsgLock);

    return;
}


/********************************************************************************/
/** \brief Turn on/off OsDbgMsg hook printout
*
* \return N/A
*/
EC_T_BOOL CAtEmLogging::OsDbgMsgHookEnable(EC_T_BOOL bEnable)
{
    return m_bDbgMsgHookEnable = bEnable;
}


/********************************************************************************/
/** \brief Hook for OsDbgMsg function
*
* NOTE: in real-time operation this function should return EC_FALSE to eliminate
*       an impact to the system behavior.
*       Here we store those messages into memory first and print them at a
*       later time from a lower priority thread for debugging purposes.
*
* \return EC_TRUE if OsDbgMsg() shall print the message, EC_FALSE if not
*/
EC_T_BOOL CAtEmLogging::OsDbgMsgHookWrapper(const EC_T_CHAR* szFormat, EC_T_VALIST vaArgs)
{
EC_T_BOOL bPrint = EC_TRUE;

    if (EC_NULL != G_pOsDbgMsgLoggingInst)
    {
        bPrint = G_pOsDbgMsgLoggingInst->OsDbgMsgHook(szFormat, vaArgs);
    }
    if (bPrint)
    {
        OsVprintf(szFormat, vaArgs);
    }
    return EC_FALSE;
}

EC_T_BOOL CAtEmLogging::OsDbgMsgHook(const EC_T_CHAR* szFormat, EC_T_VALIST vaArgs)
{
EC_T_BOOL bPrint;

    bPrint = !m_bDbgMsgHookEnable;  // print messages outside as long as hook is not enabled
    if (m_bDbgMsgHookEnable)
    {
        if (m_pAllMsgBufferDesc != EC_NULL )
        {
            if (m_pAllMsgBufferDesc->bIsInitialized )
            {
                InsertNewMsg( m_pAllMsgBufferDesc, szFormat, vaArgs, EC_FALSE, EC_TRUE );
            }
            else
            {
                bPrint = EC_TRUE;
            }
        }
        else
        {
            bPrint = EC_TRUE;
        }
/*
        else
        {
            EC_T_CHAR   szMsg[256];

            OsVsnprintf(szMsg, 256, szFormat, vaArgs);
#ifdef VXWORKS
            logMsg( "%s", (int)szMsg, 2,3,4,5,6 );
#else
#if !(defined NOPRINTF)
            OsPrintf( "%s", szMsg );
#endif
#endif
        }
*/
    }
    return bPrint;
}

/********************************************************************************/
/** \brief application error message function
*
* \return N/A
*/
EC_T_DWORD CAtEmLogging::LogError(const EC_T_CHAR* szFormat, ...)
{
EC_T_VALIST vaArgs;
EC_T_DWORD  dwRes = EC_E_NOERROR;


    EC_VASTART(vaArgs, szFormat);
    dwRes = InsertNewMsg(m_pAllMsgBufferDesc, szFormat, vaArgs);

    /* important note: on VxWorks 6.1 PowerPC: re-using vaArgs in the next function call may fail without calling EC_VAEND/EC_VASTART!! */
    EC_VAEND(vaArgs);
    EC_VASTART(vaArgs, szFormat);

    dwRes = InsertNewMsg( m_pErrorMsgBufferDesc, szFormat, vaArgs);
    EC_VAEND(vaArgs);

    return dwRes;
}

/********************************************************************************/
/** \brief application error message function
*
* \return N/A
*/
EC_T_DWORD CAtEmLogging::LogErrorAdd(const EC_T_CHAR* szFormat, ...)
{
EC_T_VALIST vaArgs;
EC_T_DWORD  dwRes = EC_E_NOERROR;

    EC_VASTART(vaArgs, szFormat);
    dwRes = InsertNewMsg( m_pAllMsgBufferDesc, szFormat, vaArgs, EC_FALSE);

    /* important note: on VxWorks 6.1 PowerPC: re-using vaArgs in the next function call may fail without calling EC_VAEND/EC_VASTART!! */
    EC_VAEND(vaArgs);
    EC_VASTART(vaArgs, szFormat);

    dwRes = InsertNewMsg( m_pErrorMsgBufferDesc, szFormat, vaArgs, EC_FALSE);
    EC_VAEND(vaArgs);
    return dwRes;
}

/********************************************************************************/
/** \brief application error message function
*
* \return N/A
*/
EC_T_DWORD CAtEmLogging::LogMsg(const EC_T_CHAR* szFormat, ...)
{
EC_T_VALIST vaArgs;
EC_T_DWORD  dwRes = EC_E_NOERROR;

    EC_VASTART(vaArgs, szFormat);
    dwRes = InsertNewMsg(m_pAllMsgBufferDesc, szFormat, vaArgs);
    EC_VAEND(vaArgs);
    return dwRes;
}

/********************************************************************************/
/** \brief application error message function
*
* \return N/A
*/
EC_T_DWORD CAtEmLogging::LogMsgAdd(const EC_T_CHAR* szFormat, ...)
{
EC_T_VALIST vaArgs;
EC_T_DWORD  dwRes = EC_E_NOERROR;

    EC_VASTART(vaArgs, szFormat);
    dwRes = InsertNewMsg(m_pAllMsgBufferDesc, szFormat, vaArgs, EC_FALSE);
    EC_VAEND(vaArgs);
    return dwRes;
}

/********************************************************************************/
/** \brief application DCM message function
*
* \return N/A
*/
EC_T_DWORD CAtEmLogging::LogDcm(const EC_T_CHAR* szFormat, ...)
{
EC_T_VALIST vaArgs;
EC_T_DWORD  dwRes = EC_E_NOERROR;

    EC_VASTART(vaArgs, szFormat);
#ifdef RTAI
    static EC_T_DWORD dwLogDcmMsgCount = 0;
    if ((dwLogDcmMsgCount % 100) == 0)
    {
        OsDbgMsgHookWrapper(szFormat, vaArgs);
        OsDbgMsg("\n");
        dwLogDcmMsgCount = 0;
    }
    dwLogDcmMsgCount++;
#else
    dwRes = InsertNewMsg(m_pDcmMsgBufferDesc, szFormat, vaArgs);
#endif
    EC_VAEND(vaArgs);
    return dwRes;
}

/********************************************************************************/
/** \brief application DCM message function
*
* \return N/A
*/
EC_T_DWORD CAtEmLogging::LogDcmAdd(const EC_T_CHAR* szFormat, ...)
{
EC_T_VALIST vaArgs;
EC_T_DWORD  dwRes = EC_E_NOERROR;

    EC_VASTART(vaArgs, szFormat);
    dwRes = InsertNewMsg(m_pDcmMsgBufferDesc, szFormat, vaArgs, EC_FALSE);
    EC_VAEND(vaArgs);
    return dwRes;
}

/********************************************************************************/
/** \brief set log thread affinity
*
* \return N/A
*/
EC_T_BOOL CAtEmLogging::SetLogThreadAffinity(EC_T_DWORD dwCpuIndex)
{
EC_T_CPUSET CpuSet;
EC_T_BOOL bOk = EC_TRUE;
EC_UNREFPARM(dwCpuIndex);

    if (m_pvLogThreadObj != EC_NULL)
    {
        EC_CPUSET_ZERO(CpuSet);
        EC_CPUSET_SET(CpuSet, dwCpuIndex);
        bOk = OsSetThreadAffinity(m_pvLogThreadObj, CpuSet);
    }
    return bOk;
}

/********************************************************************************/
/** \brief set log directory
*
* \return EC_E_NOERROR or EC_E_NOMEMORY if szLogDir too long
*/
EC_T_DWORD CAtEmLogging::SetLogDir(EC_T_CHAR* szLogDir)
{
    if (OsStrlen(szLogDir) >= MAX_PATH_LEN)
    {
        return EC_E_NOMEMORY;
    }
    OsStrncpy(m_pchLogDir, szLogDir, OsStrlen(szLogDir) + 1);
    return EC_E_NOERROR;
}

/*-END OF SOURCE FILE--------------------------------------------------------*/
