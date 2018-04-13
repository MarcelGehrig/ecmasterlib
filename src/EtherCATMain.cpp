/*-----------------------------------------------------------------------------
 * DCDemoMain.cpp            
 * Copyright                acontis technologies GmbH, Weingarten, Germany
 * Response                 Stefan Zintgraf
 * Description              EtherCAT Master demo main entrypoint
 *---------------------------------------------------------------------------*/



/*-INCLUDES------------------------------------------------------------------*/
#include <AtEthercat.h>
#include <EcCommon.h>
#include <EcInterfaceCommon.h>

#include <EtherCATMain.hpp>
#include <EtherCATStack.h>
#include <selectLinkLayer.h>
#include <Logging.h>


using namespace ethercat;
void callbackFct(EC_T_BYTE* pbyPDInPtr, EC_T_BYTE* pbyPDOutPtr)
{
	EtherCATMain* inst = EtherCATMain::getInstance();
	//TODO error if null
	
	static int counter = 0;
	static constexpr int maxRetries = 100;
	
// 	if (counter == 0) {
// 		std::cout << "EtherCATMain, counter == 0: " << std::endl;
// 		std::cout << "inst: " << inst << std::endl;
// 		std::cout << "inst->getBufferSize(): " << inst->getBufferSize() << std::endl;
// 		std::cout << "inst->getInBuffer():   " << (void*)inst->getInBuffer() << std::endl;
// 		std::cout << "pbyPDInPtr:            " << (void*)pbyPDInPtr << std::endl;
// 		std::cout << "inst->getOutBuffer():  " << (void*)inst->getOutBuffer() << std::endl;
// 		std::cout << "pbyPDOutPtr:           " << (void*)pbyPDOutPtr << std::endl;
// 		std::cout << std::endl;
// 	}
// 	
// 	if (counter == 100) {
// 		std::cout << "EtherCATMain, counter == 100: " << std::endl;
// 		std::cout << "inst: " << inst << std::endl;
// 		std::cout << "inst->getBufferSize(): " << inst->getBufferSize() << std::endl;
// 		std::cout << "inst->getInBuffer():   " << (void*)inst->getInBuffer() << std::endl;
// 		std::cout << "pbyPDInPtr:            " << (void*)pbyPDInPtr << std::endl;
// 		std::cout << "inst->getOutBuffer():  " << (void*)inst->getOutBuffer() << std::endl;
// 		std::cout << "pbyPDOutPtr:           " << (void*)pbyPDOutPtr << std::endl;
// 		std::cout << std::endl;
// 	}
// 	
// 	if (counter == 150) {
// 		std::cout << "EtherCATMain, counter == 150: " << std::endl;
// 		std::cout << "inst: " << inst << std::endl;
// 		std::cout << "inst->getBufferSize(): " << inst->getBufferSize() << std::endl;
// 		std::cout << "inst->getInBuffer():   " << (void*)inst->getInBuffer() << std::endl;
// 		std::cout << "pbyPDInPtr:            " << (void*)pbyPDInPtr << std::endl;
// 		std::cout << "inst->getOutBuffer():  " << (void*)inst->getOutBuffer() << std::endl;
// 		std::cout << "pbyPDOutPtr:           " << (void*)pbyPDOutPtr << std::endl;
// 		std::cout << std::endl;
// 	}
	
	
	if (counter > 200) {		// 10 are not enough
		
		// copy to inBuffer from stack
		int counterRetries = 0;
		while ( (inst->lockInBuffer.lockCount != 0) && (counterRetries < maxRetries) ) {
			usleep(1);
			counterRetries++;
		}
		if (inst->lockInBuffer.lockCount != 0) {
			std::cout << "WARNING: lockInBuffer.lockCount = " << inst->lockInBuffer.lockCount << "     retries: " << maxRetries << std::endl;
		}
		else {
			memcpy(inst->getInBuffer(), pbyPDInPtr, inst->getBufferSize());
		}
		
		
		// copy to stack from outBuffer
		counterRetries = 0;
		while ( (inst->lockOutBuffer.lockCount != 0) && (counterRetries < maxRetries) ) {
			usleep(1);
			counterRetries++;
		}
		if (inst->lockOutBuffer.lockCount != 0) {
			std::cout << "WARNING: lockOutBuffer.lockCount = " << inst->lockOutBuffer.lockCount << "     retries: " << maxRetries << std::endl;
		}
		else {
			memcpy(pbyPDOutPtr, inst->getOutBuffer(), inst->getBufferSize());
		}
	}
	
	if (counter<1000) counter++;
	// wake conditional variable
	inst->getConditionalVariable()->notify_one();
}


#if (defined ATEMRAS_SERVER)
#include <AtEmRasSrv.h>
#endif

#if (defined WIN32)
 #if (defined UNDER_CE)
  #include "pkfuncs.h"
  #include "nkintr.h"
  #if (_WIN32_WCE >= 0x600)
   #if (defined ECWIN_CE)
    #include <CeWin.h>
    #include <RtosLib.h>
   #else
    #include <AuxClk.h>
   #endif
   #include <VirtIO.h>
  #endif

 #elif (defined RTOS_32)
   #ifdef ECWIN_RTOS32
    #include <Rtvmf.h>
    #include <rtk32.h>
    /* STRICT and _CRT_SECURE_NO_WARNINGS will be set again by Rteos.h included by vmfInterface.h */
    #if !(defined _RTEOS_H)
      #undef STRICT
      #undef _CRT_SECURE_NO_WARNINGS
    #endif /* !_RTEOS_H */
    #include <vmfInterface.h>
    #include <RtosLib.h>
    #include <RtosLibFs.h>

    /* IP Sockets are always reference by EC-Master for RTOS-32. Link IP stack if available or stubs. */
    #ifdef EC_SOCKET_IP_SUPPORTED
      #pragma comment(lib, "Rtip.lib")
      #pragma comment(lib, "Netvmf.lib")
    #else
      #include <RtipDummy.cpp>
    #endif /* EC_SOCKET_IP_SUPPORTED */
  #else
      #include <rttarget.h>
      #include <rtk32.h>
      #include <clock.h>
      #include <timer.h>
  #endif
  #if (defined ATEMRAS_SERVER) && (defined EC_SOCKET_IP_SUPPORTED) && (defined INCLUDE_RTIP)
    #include <NetRTOS32Init.cpp>
  #endif
 #else /* WINDOWS, RTX, ... */
  #include <warn_dis.h>
  #include <windows.h>
  #include <tchar.h>
  #include <warn_ena.h>
  #endif
#endif /* WIN32 */

#if (defined VXWORKS)
 #include <vxWorks.h>
 #include <sysLib.h>
 #include <tickLib.h>
#if ((defined _WRS_VXWORKS_MAJOR) && (defined _WRS_VXWORKS_MINOR) && ( (_WRS_VXWORKS_MAJOR >= 7) || ((_WRS_VXWORKS_MAJOR == 6) && (_WRS_VXWORKS_MINOR >= 5)) ))
#else
 #include <ifLib.h>
#endif
#endif /* VXWORKS */

#if (defined LINUX)
 #include <sys/mman.h>
 #include <sys/utsname.h>
 #include <signal.h>
#endif /* LINUX */

#if defined __INTEGRITY
 #include <unistd.h>
#endif

#if (defined __RCX__)
 #include "tlr_includes.h"     /* TLR includes */
 #include "AtemDemo_Functionlist.h"
#endif

#if (defined EC_VERSION_GO32)
#include <dmpcfg.h>
#include <io.h>
#include <irq.h>
#endif

#if (defined EC_VERSION_RTEMS)
#include <rtems.h>
#include <rtems/bdpart.h>
#include <rtems/fsmount.h>
#include <rtems/monitor.h>
#include <rtems/blkdev.h>
#include <rtems/tod.h>
#include "RtemsConf.h"
#endif

#include <stdio.h>
#include <stdlib.h>

#if (defined XENOMAI)
#include <execinfo.h> /* for backtracing functions */
#endif

/*-DEFINES-------------------------------------------------------------------*/

#define LogMsg      oLogging.LogMsg
#define LogError    oLogging.LogError

#define COMMAND_LINE_BUFFER_LENGTH 512

#if !defined(ATECAT_DLL)
/* For operating systems don't supporting dynamic libraries, the link layers */
/* are linked statically to the application. Select the link layer(s) here   */
#if (defined RTAI)
  #define LINKLAYER_I8254X
#endif
#if (defined RTOS_32) || (defined __INTEGRITY) || (defined __TKERNEL)
  #define LINKLAYER_I8254X
//#define LINKLAYER_I8255X
//#define LINKLAYER_RTL8139
//#define LINKLAYER_RTL8169
#endif
#if (defined ECWIN_RTOS32)
/* all link layers are available */
  #define LINKLAYER_I8255X
  #define LINKLAYER_RTL8139
  #define LINKLAYER_RTL8169
#endif
#if (defined __RCX__)
  #define LINKLAYER_HNX
#endif
#if (defined EC_VERSION_ETKERNEL)
  #define LINKLAYER_FSLFEC
#endif
#if (defined EC_VERSION_SYSBIOS)
  #define LINKLAYER_CPSW
#endif /* EC_VERSION_SYSBIOS */
#if (defined EC_VERSION_RTEMS)
//#define RTEMS_USE_TIMER_SERVER
  #define LINKLAYER_I8254X
  #define LINKLAYER_RTL8169
  #define LINKLAYER_CCAT
#endif /* EC_VERSION_RTEMS */
#if (defined EC_VERSION_ECOS)
  #define LINKLAYER_I8254X
#endif /* EC_VERSION_ECOS */

#endif /* !ATECAT_DLL */

#if (defined RTAI)
extern "C" long int G_dwBusCycleTimeUsec;
/** created from MasterENI.xml with "objcopy -B i386 -I binary -O elf32-i386 MasterENI.xml MasterENI.o" */
extern EC_T_BYTE MasterENI_xml_data[]      asm("_binary_MasterENI_xml_start");
extern EC_T_BYTE MasterENI_xml_data_size[] asm("_binary_MasterENI_xml_size");
extern EC_T_BYTE MasterENI_xml_data_end[]  asm("_binary_MasterENI_xml_end");
#define STATIC_MASTERENI_XML_DATA          MasterENI_xml_data
#define STATIC_MASTERENI_XML_DATA_SIZE     (size_t)((EC_T_VOID*)MasterENI_xml_data_size)
#endif /* RTAI */

#if (defined EC_VERSION_SYSBIOS) || (defined EC_VERSION_RIN32M3) ||\
    (defined EC_VERSION_XILINX_STANDALONE) || (defined EC_VERSION_RZT1) || (defined EC_VERSION_ETKERNEL) ||\
    (defined EC_VERSION_RZGNOOS) || (defined EC_VERSION_JSLWARE)
extern EC_T_BYTE  MasterENI_xml_data[];
extern EC_T_DWORD MasterENI_xml_data_size;

#define STATIC_MASTERENI_XML_DATA          MasterENI_xml_data
#define STATIC_MASTERENI_XML_DATA_SIZE     MasterENI_xml_data_size
#endif

#if (defined EC_VERSION_WINDOWS) || (defined VXWORKS) || ((defined UNDER_CE) && (_WIN32_WCE >= 0x600)) ||\
    (defined UNDER_RTSS) || (defined STARTERWARE_NOOS) ||\
    (defined EC_VERSION_SYSBIOS) || (defined EC_VERSION_RIN32M3) || (defined EC_VERSION_XILINX_STANDALONE) ||\
    (defined EC_VERSION_RZT1) || (defined EC_VERSION_RZGNOOS) || ((defined LINUX) && (!defined RTAI) && (!defined XENOMAI)) ||\
    (defined EC_VERSION_JSLWARE)
#define AUXCLOCK_SUPPORTED
#endif

/*-TYPEDEFS------------------------------------------------------------------*/
typedef struct _EC_T_TIMING_DESC
{
    EC_T_VOID*          pvTimingEvent;      /* event handle */
    EC_T_DWORD          dwBusCycleTimeUsec; /* cycle time in usec */
    EC_T_BOOL           bShutdown;          /* EC_TRUE if aux thread shall shut down */
    EC_T_BOOL           bIsRunning;         /* EC_TRUE if the aux thread is running */
#if (defined AUXCLOCK_SUPPORTED)
    EC_T_BOOL           bUseAuxClock;       /* Either connect to IRQ or use sleep */
    EC_T_VOID*          pvAuxClkEvent;      /* event handle */
#endif
    EC_T_DWORD          dwCpuIndex;         /* SMP systems: CPU index */
#if ((defined UNDER_CE) && (_WIN32_WCE >= 0x600))
    HANDLE              hVirtualDrv;        /* virtual IO driver handle */
    VI_T_INTERRUPTDESC  oIrqDesc;           /* Irq descriptor for virtual IO driver */
#endif
} EC_T_TIMING_DESC;

/*-GLOBAL VARIABLES-----------------------------------------------------------*/
volatile EC_T_BOOL bRun = EC_TRUE;

/*-LOCAL FUNCTIONS-----------------------------------------------------------*/
#if (defined WIN32) && !(defined UNDER_CE)
/********************************************************************************/
/** \brief  CTRL-C handler.
*
* \return N/A
*/
static BOOL WINAPI win32CtrlCHandler(DWORD dwCtrlType)
{
    UNREFERENCED_PARAMETER( dwCtrlType );

    bRun = EC_FALSE;
    return TRUE;        // don't handle events
}
#endif /* WIN32 && !UNDER_CE */

#if (defined VXWORKS)
/********************************************************************************/
/** \brief  Change VxWorks task priority.
*
* \return  N/A.
*/
EC_T_VOID vxTaskPrioChange( EC_T_CHAR* szTaskName )
{
#if (_WRS_VXWORKS_MAJOR < 7) || ! defined(_WRS_VXWORKS_MAJOR)
int nTid = taskNameToId(szTaskName);
    if (-1 != nTid)
#else
TASK_ID nTid = taskNameToId( szTaskName );
    if (ERROR != taskIdVerify(nTid))
#endif
    {
    int nPrio;

        taskPriorityGet( nTid, &nPrio );
        if( nPrio < 10 )
        {
            taskPrioritySet( nTid, nPrio + 10 );
        }
    }
}
/********************************************************************************/
/** \brief  VxWorks auxiliary clock ISR.
*
* \return  N/A.
*/
EC_T_VOID vxAuxClkIsr( EC_T_INT nEvent )
{
    EC_T_VOID* pvEvent = (EC_T_VOID*)nEvent;
    if( EC_NULL != pvEvent )
    {
        OsSetEvent(pvEvent);
    }
}
#endif /* VXWORKS */

#if (defined(RTOS_32) && !defined(_WINDLL))
#if (defined ECWIN_RTOS32)
static RTFileSystem Console = { RT_FS_CONSOLE, 0, 0, &RTConsoleFileSystem };
static RTFileSystem LPTFiles = { RT_FS_LPT_DEVICE, 0, 0, &RTLPTFileSystem };
static RTFileSystem RAMFiles = { RT_FS_FILE, 1 << ('D' - 'A'), 0, &RTRAMFileSystem };
static RTFileSystem RtosLibFiles = { RT_FS_FILE | RT_FS_IS_DEFAULT, 1 << ('C' - 'A'), 0, &RTRtosLibFileSystem, 0, 0 };
RTFileSystem * RTFileSystemList[] =
{
       &Console,
       &LPTFiles,
       &RAMFiles,
       &RtosLibFiles, 
       NULL
};
#endif /* ECWIN_RTOS32 */
extern "C" int __rttMaxPCIBusses;
extern "C" {
    RTK32Config RTKConfig = {
            sizeof(RTK32Config),           // StructureSize
#if MP
            DF_IDLE_HALT |                 // DriverFlags (MP kernel)
#endif
            0,                             // DriverFlags
            0,                             // UserDriverFlags
            RF_PREEMPTIVE |                // preemptive
            RF_AUTOINIT | RF_NAMED_WIN32CS,// Flags
            16*1024,                       // DefaultTaskStackSize
#if MP
            1024,                          // DefaultIntStackSize
#else
            512,                           // DefaultIntStackSize
#endif
            5,                             // MainPriority
            0,                             // DefaultPriority
            0,                             // HookedInterrupts (none)
            256,                           // TaskStackOverhead
            0                              // TimeSlice (0 == off)
    } ;
#ifdef _MSC_VER
extern "C" __declspec(dllexport) void          Init(void)
#else
extern "C"                       void __export Init(void)
#endif
{
   __rttMaxPCIBusses = 255;
   RTSetFlags(RT_MM_VIRTUAL | RT_CLOSE_FIND_HANDLES, 1);
#ifdef ECWIN_RTOS32
   RTVmfInit();
   RTVmfExtendHeap();
#else
   RTCMOSExtendHeap();
#endif
}
}
#endif /* (RTOS_32 && !_WINDLL) */

#if (defined LINUX) && !(defined RTAI)
/********************************************************************************/
/** \brief  signal handler.
*
* \return N/A
*/
static void SignalHandler( int nSignal )
{
    bRun = EC_FALSE;
}
#endif

#if (defined LINUX) && !(defined RTAI) && !(defined XENOMAI)
/********************************************************************************/
/** Enable real-time environment
*
* Return: EC_E_NOERROR in case of success, EC_E_ERROR in case of failure.
*/
EC_T_DWORD EnableRealtimeEnvironment( EC_T_VOID )
{
   struct utsname SystemName;
   int nMaj, nMin, nSub;
   struct timespec ts;
   int nRetval;
   EC_T_DWORD dwResult = EC_E_ERROR;
   EC_T_BOOL bHighResTimerAvail;
   struct sched_param schedParam;

   /* master only tested on >= 2.6 kernel */
   nRetval = uname( &SystemName );
   if (nRetval != 0)
   {
      OsPrintf( "ERROR calling uname(), required Linux kernel >= 2.6\n" );
      dwResult = EC_E_ERROR;
      goto Exit;
   }
   sscanf( SystemName.release, "%d.%d.%d", &nMaj, &nMin, &nSub );
   if (!(((nMaj == 2) && (nMin == 6)) || (nMaj >= 3)))
   {
      OsPrintf( "ERROR - detected kernel = %d.%d.%d, required Linux kernel >= 2.6\n", nMaj, nMin, nSub );
      dwResult = EC_E_ERROR;
      goto Exit;
   }

   /* request realtime scheduling for the current process
    * This value is overwritten for each individual task
    */
   schedParam.sched_priority = MAIN_THREAD_PRIO; /* 1 lowest priority, 99 highest priority */
   nRetval = sched_setscheduler( 0, SCHED_FIFO, &schedParam );
   if (nRetval == -1)
   {
      OsPrintf( "ERROR - cannot change scheduling policy!\n"
                "root privilege is required or realtime group has to be joined!\n" );
      goto Exit;
   }

   /* disable paging */
   nRetval = mlockall( MCL_CURRENT | MCL_FUTURE );
   if (nRetval == -1)
   {
      OsPrintf( "ERROR - cannot disable paging!\n" );
      dwResult = EC_E_ERROR;
      goto Exit;
   }

   /* check if high resolution timers are available */
   if (clock_getres(CLOCK_MONOTONIC, &ts))
   {
      bHighResTimerAvail = EC_FALSE;
   }
   else
   {
      bHighResTimerAvail = !(ts.tv_sec != 0 || ts.tv_nsec != 1);
   }
   if( !bHighResTimerAvail )
   {
      OsPrintf( "WARNING: High resolution timers not available\n" );
   }

   /* set type of OsSleep implementation  (eSLEEP_USLEEP, eSLEEP_NANOSLEEP or eSLEEP_CLOCK_NANOSLEEP) */
   OsSleepSetType( eSLEEP_CLOCK_NANOSLEEP );

   dwResult = EC_E_NOERROR;
Exit:
    return dwResult;
}
#endif /* LINUX && !RTAI && !XENOMAI */

#if (defined XENOMAI)
#if !(defined CONFIG_XENO_MERCURY) 
static const char *reason_str[] = {
    [SIGDEBUG_UNDEFINED] = "undefined",
    [SIGDEBUG_MIGRATE_SIGNAL] = "received signal",
    [SIGDEBUG_MIGRATE_SYSCALL] = "invoked syscall",
    [SIGDEBUG_MIGRATE_FAULT] = "triggered fault",
    [SIGDEBUG_MIGRATE_PRIOINV] = "affected by priority inversion",
    [SIGDEBUG_NOMLOCK] = "missing mlockall",
    [SIGDEBUG_WATCHDOG] = "runaway thread",
};

void warn_upon_switch(int sig, siginfo_t *si, void *context)
{
    unsigned int reason = si->si_value.sival_int;
    void *bt[32];
    int nentries;

    printf("\nSIGDEBUG received, reason %d: %s\n", reason,
    reason <= SIGDEBUG_WATCHDOG ? reason_str[reason] : "<unknown>");
    /* Dump a backtrace of the frame which caused the switch to
       secondary mode: */
    nentries = backtrace(bt,sizeof(bt) / sizeof(bt[0]));
    backtrace_symbols_fd(bt,nentries,fileno(stdout));
}
#endif /* !CONFIG_XENO_MERCURY */

/********************************************************************************/
/** Enable real-time environment
*
* Return: EC_E_NOERROR in case of success, EC_E_ERROR in case of failure.
*/
EC_T_DWORD EnableRealtimeEnvironment( EC_T_VOID )
{
	int nRetval = 0;
	EC_T_DWORD dwRetVal = EC_E_ERROR;
#if !(defined CONFIG_XENO_MERCURY)
    struct sigaction sa;

    /* Register callback for catching switches to Xenomai secondary mode */
    sigemptyset(&sa.sa_mask);
    sa.sa_sigaction = warn_upon_switch;
    sa.sa_flags = SA_SIGINFO;
    sigaction(SIGDEBUG, &sa, NULL);
#endif /* !CONFIG_XENO_MERCURY */

    /* disable paging */
    nRetval = mlockall( MCL_CURRENT | MCL_FUTURE );
    if (-1 == nRetval)
    {
	    OsPrintf( "ERROR - cannot disable paging!\n" );
	    dwRetVal = EC_E_ERROR;
	    goto Exit;
    }

    /* Turns the current Linux task into a native Xenomai task.
     *
     * Using shadow with NULL pointer as RT_TASK is only available in
     * recent Xenomai implementations!
     */
    nRetval = rt_task_shadow(NULL, "tEcMasterDemo", MAIN_THREAD_PRIO, 0);
    if (0 != nRetval)
    {
        OsPrintf("ERROR - cannot make the current thread a realtime task (%d)\n", nRetval);
        goto Exit;
    }

    /* Disable PRIMARY to SECONDARY MODE switch warning */
    nRetval = rt_task_set_mode(T_WARNSW, 0, NULL);
    if (nRetval != 0)
    {
        OsPrintf("OsCreateThread: rt_task_set_mode returned error %d\n", nRetval);
        OsDbgAssert(EC_FALSE);
    }

    dwRetVal = EC_E_NOERROR;
Exit:
    return dwRetVal;
}
#endif /* XENOMAI */

#if (defined UNDER_RTSS)
VOID RTFCNDCL RtxAuxClkIsr(PVOID pvEvent)
{
    if( EC_NULL != pvEvent )
    {
        OsSetEvent(pvEvent);
    }
}
#endif

#if (defined ECWIN_RTOS32)
/*******************************************************************************
*
* RdDbgPortChar - Read character from Debug PORT
*
* Return: TRUE if character was available, FALSE if not.
*/
BOOL RdDbgPortChar( UINT8* pbyData )
{
UINT32 dwNumRead;

    vmfVioRead( 0, FALSE, pbyData, 1, &dwNumRead);
    return (dwNumRead == 1);
}

extern volatile EC_T_BOOL bRun;             /* global helper variable to shutdown the application */
/*-----------------------------------*/
#define CTRL_C  3
#define ESC    27
void tTerminationReq(void)
{
UINT8 byData;

    /* flush input first */
    while( RdDbgPortChar(&byData) )
    {
        Sleep(1);
    }
    for(;;)
    {
        Sleep(100);
        if( RdDbgPortChar(&byData) )
        {
            if( (byData == CTRL_C) || (byData == ESC) )
            {
                bRun = 0;
                break;
            }
        }
    }
}

/* used for DCM MasterShift */
static UINT32 S_dwTimerHwInitialCount = 0;
static INT32  S_nTimerHwLastModification = 0; /* holds last modification from S_dwTimerHwInitialCount */

EC_T_DWORD Rtos32HwTimerGetInputFrequency(EC_T_DWORD *pdwFrequencyHz)
{
    UINT32 dwRes = vmfTimerHwGetInputFreq(pdwFrequencyHz);
    if (dwRes != 0)
        return EC_E_NOTSUPPORTED;

    dwRes = vmfTimerHwGetInitialCount(&S_dwTimerHwInitialCount);
    if (dwRes != 0)
        return EC_E_NOTSUPPORTED;

    return EC_E_NOERROR;
}

EC_T_DWORD Rtos32HwTimerModifyInitialCount(EC_T_INT nAdjustPermil)
{
    INT32 nNewModification = nAdjustPermil * ((INT32)S_dwTimerHwInitialCount / 1000);

    /* vmfTimerHwModifyInitialCount modifies original value of vmfTimerHwGetInitialCount
       so we should return it back what nAdjustPermil is 0 */
    INT32 nModification = nNewModification - S_nTimerHwLastModification;
    if (nModification != 0)
    {
        UINT32 dwRes = vmfTimerHwModifyInitialCount(nModification);
        if (dwRes != 0)
            return EC_E_NOTSUPPORTED;
    }
    S_nTimerHwLastModification = nNewModification;
    
    return EC_E_NOERROR;
}

#endif /* ECWIN_RTOS32 */

#if (defined EC_VERSION_RTEMS)
/********************************************************************************/
/** Timer ISR to trigger the job task
*
* Return: N/A
*/
rtems_timer_service_routine rtemsTimerIsr(rtems_id timerId, EC_T_VOID* pvEvent)
{
	if(EC_NULL != pvEvent)
	{
		OsSetEvent(pvEvent);
	}
	rtems_timer_reset(timerId);
}
/********************************************************************************/
/** Mount Filesystems
 *  required to read ENI-File
 *  Maybe extend/change pDevname and fs_table to your needs
 *
 * Return: N/A
*/
EC_T_BOOL rtemsMountFilesystems()
{
    rtems_status_code status = RTEMS_NOT_DEFINED;
    EC_T_BOOL bRes = EC_TRUE;
    EC_T_INT rv;

    const char* pDevname[] =
    {
            "/dev/hda",
            "/dev/hdb"
    };
    fstab_t fsTable[] =
    {
        {"/dev/hda1", "/mnt/hda1", "dosfs", RTEMS_FILESYSTEM_READ_WRITE,
                    FSMOUNT_MNT_OK | FSMOUNT_MNTPNT_CRTERR | FSMOUNT_MNT_FAILED, 0 },
        {"/dev/hdb1", "/mnt/hdb1", "dosfs", RTEMS_FILESYSTEM_READ_WRITE,
                    FSMOUNT_MNT_OK | FSMOUNT_MNTPNT_CRTERR | FSMOUNT_MNT_FAILED, 0 }
    };
    const EC_T_INT nNumDevs = 2;
    /* Register partitions as logical disks */
    for(EC_T_INT i = 0; i < nNumDevs; i++)
    {
        status = rtems_bdpart_register_from_disk(pDevname[i]);
        if(status != RTEMS_SUCCESSFUL && status != RTEMS_INVALID_NUMBER)
        {
            OsDbgMsg("ATEMDemoMain: Failed to register %s\n", pDevname[i]);
            OsDbgMsg("RTEMS returned: %s\n", rtems_status_text(status));
            continue;
        }
    }
    /* Mounts the file systems listed in the file system mount table */
    rv = rtems_fsmount(fsTable, sizeof(fsTable) / sizeof(fsTable[0]), NULL);
    if (rv != 0)
    {
        OsDbgMsg("ATEMDemoMain: cannot mount file system\n");
        bRes = EC_FALSE;
    }
    return bRes;
}

/********************************************************************************/
/** Sync Block Device Buffers to write data to the disk media
*
* Return: N/A
*/
void rtemsSyncBDBuffers(){
    rtems_status_code sc = RTEMS_SUCCESSFUL;
    dev_t dev = (dev_t) -1;
    rtems_disk_device *dd = NULL;

    while(sc == RTEMS_SUCCESSFUL && (dd = rtems_disk_next(dev)) != NULL)
    {
        /* Synchronize all modified buffers for device dd */
        sc = rtems_bdbuf_syncdev(dd);
        if( sc != RTEMS_SUCCESSFUL)
        {
            OsDbgMsg("Device: %s sync buffers failed\n", dd->name);
        }
        /* get next dev for rtems_disk_next iterator */
        dev = rtems_disk_get_device_identifier(dd);
        sc = rtems_disk_release(dd);
    }
    /* Sleep a while to be ensure that the buffers are synced */
    OsSleep(5000);
}
#endif /* EC_VERSION_RTEMS */

/********************************************************************************/
/** Show syntax
*
* Return: N/A
*/
static EC_T_VOID ShowSyntax(EC_T_VOID)
{
    OsDbgMsg("Syntax:\n");
    OsDbgMsg("EcMasterDemoDc [-f ENI-FileName] [-t time] [-b time] [-a affinity] [-v lvl] [-perf] [-log Prefix]");
#if (defined AUXCLOCK_SUPPORTED)
    OsDbgMsg(" [-auxclk period]");
#endif
#if (defined ATEMRAS_SERVER)
    OsDbgMsg(" [-sp [port]]");
#endif
    ShowLinkLayerSyntax1();
    OsDbgMsg("   -f                Use given ENI file\n");
    OsDbgMsg("     FileName        file name .xml\n");
    OsDbgMsg("   -t                Demo duration\n");
    OsDbgMsg("     time            Time in msec, 0 = forever (default = 120000)\n");
    OsDbgMsg("   -b                Bus cycle time\n");
    OsDbgMsg("     cycle time      Cycle time in usec\n");
    OsDbgMsg("   -a                CPU affinity\n");
    OsDbgMsg("     affinity        0 = first CPU, 1 = second, ...\n");
    OsDbgMsg("   -v                Set verbosity level\n");
    OsDbgMsg("     lvl             Level: 0=off, 1...n=more messages, 3(default) generate dcmlog file\n");
    OsDbgMsg("   -perf             Enable job measurement\n");
    OsDbgMsg("   -log              Use given file name prefix for log files\n");
    OsDbgMsg("     Prefix          prefix\n");
#if (defined AUXCLOCK_SUPPORTED)
    OsDbgMsg("   -auxclk           use auxiliary clock\n");
    OsDbgMsg("     period          clock period in usec\n" );
#endif
#if (defined ATEMRAS_SERVER)
    OsDbgMsg("   -sp               Server port binding\n");
    OsDbgMsg("     port            port (default = %d)\n", ATEMRAS_DEFAULT_PORT);
#endif
    OsDbgMsg("   -dcmmode          Set DCM mode\n");
    OsDbgMsg("     off             Off\n");
    OsDbgMsg("     busshift        BusShift mode (default)\n");
    OsDbgMsg("     mastershift     MasterShift mode\n");
    OsDbgMsg("     masterrefclock  MasterRefClock mode\n");
    OsDbgMsg("     dcx             External synchronization mode\n");
    OsDbgMsg("   -ctloff           Disable DCM control loop for diagnosis\n");

    ShowLinkLayerSyntax2();

    return;
}

#if !(defined RTAI) && !(defined XENOMAI)
/********************************************************************************/
/* \brief Set event according to periodical sleep or aux clock
 * Cyclically sets an event for thread synchronization purposes.
 * Either use OsSleep() or use the aux clock by means of:
 * - Enable AUX clock if selected.
 * - Wait for IRQ, aknowledge IRQ, SetEvent in loop until shutdown
 * - Disable AUX clock
 * Return: N/A
 */
static EC_T_VOID tEcTimingTask( EC_T_VOID* pvThreadParamDesc )
{
EC_T_TIMING_DESC* pTimingDesc = (EC_T_TIMING_DESC*)pvThreadParamDesc;
EC_T_CPUSET       CpuSet;
#if (defined QNX6)
    struct _clockperiod oClockPeriod = {0};
#elif (defined __INTIME__)
    RTHANDLE hTimingAlarm;
#endif

    EC_CPUSET_ZERO( CpuSet );
    EC_CPUSET_SET( CpuSet, pTimingDesc->dwCpuIndex );
    OsSetThreadAffinity( EC_NULL, CpuSet );

#if ((defined UNDER_CE) && (_WIN32_WCE >= 0x600))
    /* enable auxilary clock */
    if (pTimingDesc->bUseAuxClock)
    {
    DWORD dwAuxClkFreq = 1000000 / pTimingDesc->dwBusCycleTimeUsec;

        if (!KernelIoControl((DWORD)IOCTL_AUXCLK_ENABLE, &dwAuxClkFreq, sizeof(DWORD), NULL, 0, NULL))
        {
            OsDbgMsg("Error calling KernelIoControl(IOCTL_AUXCLK_ENABLE) (0x%08X)!\n", GetLastError());
            goto Exit;
        }
    }
#elif (defined QNX6)
    /* Set the clock period to bus cycle time */
    oClockPeriod.nsec = pTimingDesc->dwBusCycleTimeUsec * 1000;
    if(ClockPeriod(CLOCK_REALTIME, &oClockPeriod, EC_NULL, 0) == -1)
    {
        OsPrintf("tEcTimingTask:: Cannot set the clock period! Error %i", errno);
    }
#elif (defined __INTIME__)
    hTimingAlarm = CreateRtAlarm(KN_REPEATER, pTimingDesc->dwBusCycleTimeUsec);
#endif

    /* timing task started */
    pTimingDesc->bIsRunning = EC_TRUE;

    /* periodically generate events as long as the application runs */
    while (!pTimingDesc->bShutdown)
    {
        /* wait for the next cycle */
#if ((defined UNDER_CE) && (_WIN32_WCE >= 0x600))

        if (pTimingDesc->bUseAuxClock)
        {
            /* wait for auxclk event */
            OsWaitForEvent(pTimingDesc->pvAuxClkEvent, EC_WAITINFINITE);

            if (pTimingDesc->bShutdown)
            {
                /* disable auxilary clock */
                if (!KernelIoControl((DWORD)IOCTL_AUXCLK_DISABLE, NULL, 0, NULL, 0, NULL))
                {
                    OsDbgMsg("Error calling KernelIoControl(IOCTL_AUXCLK_DISABLE) (0x%08X)!\n", GetLastError());
                }
            }
            /* Acknowledge the IRQ */
            if (pTimingDesc->hVirtualDrv != NULL)
            {
                DeviceIoControl(pTimingDesc->hVirtualDrv, (DWORD)IOCTL_VIRTDRV_INTERRUPT_DONE, &(pTimingDesc->oIrqDesc), sizeof(VI_T_INTERRUPTDESC), NULL, 0, NULL, NULL );
            }
        }
        else
        {
            /* wait for next cycle (no cycle below 1ms) */
            OsSleep(EC_MAX(pTimingDesc->dwBusCycleTimeUsec / 1000, 1));
        }
 /* UNDER_CE, _WIN32_WCE < 0x600 */
#elif (defined RTOS_32)
        {
            /* wait for next cycle (sleep of zero allow cycle below 1ms) */
            OsSleep(pTimingDesc->dwBusCycleTimeUsec / 1000);
        }
 /* RTOS_32 */
#elif (defined QNX6)
        {
            /* wait for next cycle */
            OsSleep(1);
        }
 /* QNX6 */
#elif (defined __INTIME__)
        {
            WaitForRtAlarm(hTimingAlarm, 2*pTimingDesc->dwBusCycleTimeUsec);
        }
#else
        {
            /* wait for next cycle (no cycle below 1ms) */
            OsSleep(EC_MAX(pTimingDesc->dwBusCycleTimeUsec / 1000, 1));
        }
#endif
        /* trigger jobtask */
        OsSetEvent( pTimingDesc->pvTimingEvent );
    }

#if ((defined UNDER_CE) && (_WIN32_WCE >= 0x600))
Exit:
#endif
#if (defined __INTIME__)
    if (NULL != hTimingAlarm)
    {
         DeleteRtAlarm(hTimingAlarm);
    }
#endif
    pTimingDesc->bIsRunning = EC_FALSE;
#if (defined EC_VERSION_RTEMS)
    rtems_task_delete(RTEMS_SELF);
#endif
    return;
}
#endif /* !RTAI && !XENOMAI */

/********************************************************************************/
/** \brief  Demo Application entry point.
*
* \return  Value 0 is returned.
*/
#if (defined VXWORKS)
extern "C" int dcDemo(char* lpCmdLine)
#elif (defined __TKERNEL)
EXPORT ER main( INT nArgc, UB *ppArgv[] )
#elif (defined RTOS_32)
# if (defined _WINDLL)
#  ifdef _MSC_VER
extern "C" __declspec(dllexport) int          main(void)
#  else /* _MSC_VER */
extern "C"                       int __export main(void)
#  endif /* else _MSC_VER */
# else /* _WINDLL */
                                 int          main(void)
# endif /* else _WINDLL */
#elif (defined __RCX__)
TLR_RESULT dcDemo(TLR_VOID FAR* pvInit)
#elif (defined STARTERWARE_NOOS)
extern "C" int EcMasterDemoDc(int nArgc, char* ppArgv[])
#elif (defined RTAI)
extern "C" void EcMasterDemoDcMain(long int t)
#elif (defined UNDER_CE)
int _tmain(int nArgc, _TCHAR* ppArgv[])
#elif (defined EC_VERSION_SYSBIOS) || (defined EC_VERSION_RIN32M3) || (defined EC_VERSION_XILINX_STANDALONE) ||\
    (defined EC_VERSION_ETKERNEL) || (defined EC_VERSION_RZT1) || (defined EC_VERSION_RZGNOOS) || (defined EC_VERSION_JSLWARE)
extern "C" int EcMasterDemo(void)
#elif (defined EC_VERSION_UCOS)
int ucosiiiMain(int nArgc, char** ppArgv)
#elif (defined EC_VERSION_RTEMS)
rtems_task Main(rtems_task_argument rtemsArg)
#else
/* QNX6, LINUX, __INTEGRITY, ... */
// int mainEtherCAT(int nArgc, char* ppArgv[], void (*callbackFctPtr)(EC_T_BYTE* pbyPDIn, EC_T_BYTE* pbyPDOut))
int mainEtherCAT(int nArgc, char* ppArgv[])
#endif
{
// 	ethercat::callbackFctPtr = callbackFctPtr;
    int                     nRetVal             = APP_ERROR;
    EC_T_DWORD              dwRes               = EC_E_ERROR;
    EC_T_BOOL               bLogInitialized     = EC_FALSE;
    EC_T_CHAR               szCommandLine[COMMAND_LINE_BUFFER_LENGTH];
    EC_T_CHAR               szFullCommandLine[COMMAND_LINE_BUFFER_LENGTH];
    EC_T_BOOL               bGetNextWord        = EC_TRUE;
    EC_T_CHAR*              ptcWord             = EC_NULL;
    EC_T_CHAR               tcStorage           = '\0';

    EC_T_CHAR               szLogFileprefix[256] = {'\0'};
    EC_T_CNF_TYPE           eCnfType            = eCnfType_Unknown;
    EC_T_PBYTE              pbyCnfData          = 0;
    EC_T_DWORD              dwCnfDataLen        = 0;
    EC_T_CHAR               szENIFilename[256]  = {'\0'};
//     EC_T_DWORD              dwDuration          = 120000;
    EC_T_DWORD              dwDuration          = 0;	// 0 = no timeout
    EC_T_DWORD              dwNumLinkLayer     = 0;
    EC_T_LINK_PARMS*        apLinkParms[MAX_LINKLAYER];
#ifdef ATEMRAS_SERVER
    EC_T_WORD               wServerPort = 0xFFFF;
#endif
    CAtEmLogging            oLogging;
    EC_T_DWORD              dwCpuIndex          = 0;
    EC_T_CPUSET             CpuSet;
    EC_T_BOOL               bEnaPerfJobs        = EC_FALSE;  /* enable job measurements */
    EC_T_TIMING_DESC        TimingDesc;
#if !(defined XENOMAI)
    EC_T_BOOL               bStartTimingTask    = EC_FALSE;
#endif
    EC_T_INT                nVerbose            = 0;
#if (defined UNDER_CE) && (_WIN32_WCE >= 0x600)
    BOOL                    bRes                = FALSE;
    DWORD                   dwAuxClkSysIntr     = 0;
    DWORD                   dwWinRes            = EC_E_ERROR;
#endif
#if (defined UNDER_RTSS)
    HANDLE                  hTimer              = NULL;
    LARGE_INTEGER           liTimer;
#endif
#if (defined EC_VERSION_RTEMS)
    rtems_id                timerId;
    rtems_status_code       status;
#endif /* EC_VERSION_RTEMS */
    EC_T_OS_PARMS           oOsParms;
    EC_T_DCM_MODE           eDcmMode            = eDcmMode_BusShift;
    EC_T_BOOL               bCtlOff             = EC_FALSE;
	char* 					licenseKey			= "";

    OsMemset(apLinkParms, 0, sizeof(apLinkParms));
    OsMemset(&TimingDesc, 0, sizeof(TimingDesc));
    OsMemset(&oOsParms, 0, sizeof(EC_T_OS_PARMS));

    szCommandLine[0] = '\0';

    /* OS specific initialization */
    oOsParms.dwSize                         = sizeof(EC_T_OS_PARMS);
    oOsParms.dwSupportedFeatures            = 0xFFFFFFFF;
#if (defined WIN32) && !(defined UNDER_CE)
    SetConsoleCtrlHandler( (PHANDLER_ROUTINE)win32CtrlCHandler, TRUE );
#endif

#if (defined RTOS_32)
#ifdef ECWIN_RTOS32
    RTKernelInit(0);
    oOsParms.pfHwTimerGetInputFrequency     = Rtos32HwTimerGetInputFrequency;
    oOsParms.pfHwTimerModifyInitialCount    = Rtos32HwTimerModifyInitialCount;
#else
    RTKernelInit(4);
    RTCMOSSetSystemTime();

#ifdef RTOS_32_APIC
    if (!RTMPSetAPICMode(0))
    {
    static BYTE MPTableBuffer[1024]; // some space for the ACPI-generated table

        if (!RTMPACPITableCreate(MPTableBuffer, sizeof(MPTableBuffer), 0))
        {
            printf("RTMPACPITableCreate failed!\n");
        }
        if (!RTMPSetAPICMode(0))
        {
            printf("RTMPSetAPICMode failed!\n");
            nRetVal = APP_ERROR;
            goto Exit;
        }
    }
#endif /* RTOS_32_APIC */
    {
        UINT CPUClock         = RTSMBIOSGetCPUSpeed();
        UINT BusClockBIOS     = RTSMBIOSGetBusSpeed();
        UINT BusClockMeasured = RTMPBusFromCPUClock(CPUClock, 40000000);    /* 40M, that's 1/100 of a second on a 4GHz machine */

        OsDbgMsg("CPU Clock         : %uMHz\n""Bus Clock         : %uMHz\n""Bus Clock measured: %uMHz\n\n",
                    CPUClock,BusClockBIOS,BusClockMeasured);
    }

    /* workaround for IDE driver */
    RTMPSetIOAPICVector(0, 5, 5, 1, 1);

    /* prevent warning message "IDE driver in PIO mode */
    for (int nDevIndex = 0; nDevIndex < 8; nDevIndex++)
    {
        if (RTFDeviceList[nDevIndex].DeviceType == RTF_DEVICE_FDISK)
        {
            RTFDeviceList[nDevIndex].DeviceFlags |= RTF_DEVICE_NO_DIAG_MSG;
        }
        else if (RTFDeviceList[nDevIndex].DeviceType == 0)
        {
            break;
        }
    }
#endif
    RTKPreemptionsON();
#if (defined ATEMRAS_SERVER) && (defined EC_SOCKET_IP_SUPPORTED) && (defined INCLUDE_RTIP)
    NetInitialize();
#endif
#endif /* RTOS_32 */
#if (defined ECWIN_RTOS32)
    {
    void* pvReg = NULL;

        /* protect memory area at address 0 (detect NULL pointer access) */
        pvReg = NULL;
        RTReserveVirtualAddress(&pvReg, 0x1000, RT_MAP_NO_RELOCATE);

        /* start thread waiting for termination request */
        RTKRTLCreateThread( (RTKThreadFunction)tTerminationReq, 64, 0x1000, TF_NO_MATH_CONTEXT, NULL, "tTerminationReq" );

        printf( "you can stop the demo by typing CTRL-C or ESCAPE in the debug console\n");
    }
#endif /* ECWIN_RTOS32 */
#if (defined ECWIN)
    /* must be call before any RtosLib API call */
    RtosLibInit();
    
    /* must be call before any file open (RtosLibFile) call */
    RtosCommStart();
#endif /* ECWIN */

#if (defined VXWORKS)
    /* change VxWorks standard task priorities to optimize EtherCAT performance */
#ifndef VXWORKS_NORMAL_PRIO
    vxTaskPrioChange( "tJobTask" );
    vxTaskPrioChange( "tLogTask" );
    vxTaskPrioChange( "tNbioLog" );
    vxTaskPrioChange( "tShell" );
    vxTaskPrioChange( "tShell0" );
    vxTaskPrioChange( "tShellRem1" );
    vxTaskPrioChange( "tShellRem2" );
    vxTaskPrioChange( "tWdbTask" );
    vxTaskPrioChange( "tTelnetd" );
#endif
#if ((defined _WRS_VXWORKS_MAJOR) && (defined _WRS_VXWORKS_MINOR) && ( (_WRS_VXWORKS_MAJOR >= 7) || ((_WRS_VXWORKS_MAJOR == 6) && (_WRS_VXWORKS_MINOR >= 5)) ))
    taskPrioritySet( taskNameToId("tNet0"), RECV_THREAD_PRIO );
#else
    taskPrioritySet( taskNameToId("tNetTask"), RECV_THREAD_PRIO );
#endif
    /* redirect outputs to target shell console device */
#if (_WRS_VXWORKS_MAJOR < 7) || ! defined(_WRS_VXWORKS_MAJOR)
    int nTidTargetShell = taskNameToId("tShellRem1");
    if (-1 != nTidTargetShell)
#else
    TASK_ID nTidTargetShell = taskNameToId("tShellRem1");
    if (ERROR != taskIdVerify(nTidTargetShell))
#endif
    {
        /* redirect all outputs at the target shell */
        ioGlobalStdSet (STD_OUT, ioTaskStdGet(nTidTargetShell,STD_OUT));
        ioGlobalStdSet (STD_ERR, ioTaskStdGet(nTidTargetShell,STD_ERR));
    }
    /* sysclk always set to 1msec */
    sysClkRateSet(1000);
#endif /* VXWORKS */

#if (defined LINUX) && !(defined RTAI)
	
	
	
	
    dwRes = EnableRealtimeEnvironment();
    if (EC_E_NOERROR != dwRes)
    {
        goto Exit;
    }
    {
        sigset_t SigSet;
        int nSigNum = SIGALRM;
        sigemptyset(&SigSet);
        sigaddset(&SigSet, nSigNum);
        sigprocmask(SIG_BLOCK, &SigSet, NULL);
        signal(SIGINT, SignalHandler);
        signal(SIGTERM, SignalHandler);
    }
#endif /* LINUX && !RTAI */

#if (defined __INTEGRITY)
    WaitForFileSystemInitialization();
#endif

#if (defined __RCX__)
    ATEMDEMO_STARTUPPARAMETER_T* pStartup = ((ATEMDEMO_STARTUPPARAMETER_T*)pvInit);

    TCHAR* lpCmdLine = (TCHAR*)(pStartup->szParamList);
    lpCmdLine = (lpCmdLine==EC_NULL)?(TCHAR*)" ":lpCmdLine;
    tcStorage = *lpCmdLine;
#endif

#if (defined EC_VERSION_GO32)
    if (!io_Init())
    {
        OsDbgMsg("Fail to initialize Vortex86 I/O library\n");
        nRetVal = APP_ERROR;
        goto Exit;
    }
    if (!irq_Init())
    {
        OsDbgMsg("Fail to initialize Vortex86 IRQ library\n");
        io_Close();     /* Deinit IOs */
        nRetVal = APP_ERROR;
        goto Exit;
    }
#endif

#if !(defined UNDER_RTSS) && !(defined RTAI)
    /* Seed the random-number generator with current time so that
     * the numbers will be different every time we run.
     */
    srand((unsigned)OsQueryMsecCount());
#endif
    /* set running flag */
    bRun = EC_TRUE;

    /* add hook to log all EtherCAT messages. Without hook no messages will be generated! */
    OsAddDbgMsgHook(CAtEmLogging::OsDbgMsgHookWrapper);

    /* Initialize Timing Event descriptor */
    TimingDesc.bShutdown          = EC_FALSE;
    TimingDesc.bIsRunning         = EC_FALSE;
#if (defined RTAI)
    TimingDesc.dwBusCycleTimeUsec = G_dwBusCycleTimeUsec;
#else
    TimingDesc.dwBusCycleTimeUsec = CYCLE_TIME * 1000;
#endif

    /* prepare command line */
#if (defined VXWORKS)
    OsStrncpy(szCommandLine, lpCmdLine, sizeof(szCommandLine) - 1);
#elif (defined RTOS_32)
#if (defined ECWIN_RTOS32)
    {
    VMF_HANDLE hEcatKey;
    VMF_CONFIG_ADDDATA AddData;
    UINT32 dwLength = 0;

        dwRes = vmfConfigRegKeyOpenA(VMF_CONFIGREG_HKEY_OS_CURRENT, "Ecat", &hEcatKey);
        if( dwRes == RTE_SUCCESS )
        {

            dwLength = sizeof(szCommandLine);
            vmfConfigRegValueQueryA(hEcatKey, "CommandLine", NULL, NULL, (UINT8*)&szCommandLine[0], &dwLength );
            vmfConfigRegKeyClose(hEcatKey);
        }
        if ('\0' == szCommandLine[0])
        {
            /* for compatibility */
            dwLength = sizeof(szCommandLine);
            dwRes = vmfConfigQueryValue( "Ecat", "CommandLine", VMF_CONFIG_SZ_TYPE, (UINT8*)&szCommandLine[0], &dwLength, &AddData);
            if( dwRes != RTE_SUCCESS )
            {
                printf( "Cannot read EtherCAT demo command line, (EcatShm.config)\n" );
                printf( "Please, enter command line (e.g. atemDemo -v 2 -i8255x 1 1):\n" );
            }
        }
    }
#else
    OsStrncpy(szCommandLine, GetCommandLine(), sizeof(szCommandLine) - 1);
#endif /* !ECWIN_RTOS32 */
#elif (defined RTAI)
    OsStrncpy(szCommandLine, "-i8254x 1 1 -v 1 -t 15000 ", sizeof(szCommandLine) - 1);
#elif (defined __TKERNEL) && (defined __arm__)
    /*-fixed command line for T-Kernel on ARM */
    OsStrncpy(szCommandLine, "-l9218i 1 -v 1 -t 0 -f eni.xml", sizeof(szCommandLine) - 1);
#elif (defined EC_VERSION_SYSBIOS) || (defined EC_VERSION_RIN32M3) || (defined EC_VERSION_XILINX_STANDALONE) ||\
    (defined EC_VERSION_ETKERNEL) || (defined EC_VERSION_RZT1) || (defined EC_VERSION_RZGNOOS) || (defined EC_VERSION_JSLWARE)
    OsStrncpy(szCommandLine, DEMO_PARAMETERS, sizeof(szCommandLine) - 1);
#elif (defined EC_VERSION_RTEMS)
    /* copy cmdline without the applications name(first token) */
    OsStrncpy(szCommandLine, strchr(bsp_boot_cmdline,' '),sizeof(szCommandLine) - 1);
#else
    /* build szCommandLine from argument list */
    {
    EC_T_CHAR* pcStrCur   = szCommandLine;
    EC_T_INT   nStrRemain = COMMAND_LINE_BUFFER_LENGTH;
#if (defined UNDER_CE)
    EC_T_CHAR  szStrFormat[] = "%S"; /* convert UNICODE to multibyte */
#else
    EC_T_CHAR  szStrFormat[] = "%s";
#endif
        /* build szCommandLine from argument list, skipping executable name */
        for (nArgc--, ppArgv++; nArgc > 0; nArgc--, ppArgv++)
        {
        EC_T_BOOL bIsFileName = EC_FALSE;

            /* insert next argument */
            OsSnprintf(pcStrCur, nStrRemain - 1, szStrFormat, *ppArgv);

            /* check for file name */
            if (0 == OsStrcmp(pcStrCur, "-f"))
            {
                bIsFileName = EC_TRUE;
            }
            /* adjust string cursor */
            nStrRemain -= (EC_T_INT)OsStrlen(pcStrCur);
            pcStrCur = pcStrCur + OsStrlen(pcStrCur);

            /* insert space */
            OsStrncpy(pcStrCur, " ", nStrRemain - 1); nStrRemain--; pcStrCur++;

            if (bIsFileName && (1 < nArgc))
            {
                /* move to next arg (ENI file name) */
                nArgc--; ppArgv++;

                /* insert quotation mark */
                OsStrncpy(pcStrCur, "\"", nStrRemain - 1); nStrRemain--; pcStrCur++;

                /* insert ENI file name */
                OsSnprintf(pcStrCur, nStrRemain - 1, szStrFormat, *ppArgv); nStrRemain -= (EC_T_INT)OsStrlen(pcStrCur);
                pcStrCur = pcStrCur + OsStrlen(pcStrCur);

                /* insert quotation mark */
                OsStrncpy(pcStrCur, "\" ", nStrRemain - 1); nStrRemain--; pcStrCur++;
            }
        }
    }
#endif
    /* backup full command line */
    OsStrncpy(szFullCommandLine, szCommandLine, sizeof(szFullCommandLine) - 1);

    /* parse command line */
    for (ptcWord = OsStrtok(szCommandLine, " "); ptcWord != EC_NULL;)
    {
        if( 0 == OsStricmp( ptcWord, "-f") )
        {
            EC_T_INT nPtcWordIndex = 3;

            /* Search for the start of the config file name. The config file
               name may start with quotation marks because of blanks in the filename */
            while(ptcWord[nPtcWordIndex] != '\0')
            {
                if(ptcWord[nPtcWordIndex] == '\"' || ptcWord[nPtcWordIndex] != ' ')
                {
                    break;
                }
                nPtcWordIndex++;
            }

            /* Depending of a config file name within quotation marks (or without
               quotation marks) extract the filename */
            if(ptcWord[nPtcWordIndex] == '\"')
            {
                /* Check if the strtok position is already correct */
                if(nPtcWordIndex > 3)
                {
                    /* More than 1 blank before -f. Correct strtok position. */
                    OsStrtok(EC_NULL,"\"");
                }

                /* Now extract the config file name */
                ptcWord = OsStrtok(EC_NULL,"\"");
            }
            else
            {
                /* Extract the config file name if it was not set within quotation marks */
                ptcWord = GetNextWord((EC_T_CHAR**)&szCommandLine, &tcStorage );
            }

            if (
                (ptcWord == EC_NULL)
             || (OsStrncmp( ptcWord, "-", 1) == 0)
               )
            {
                nRetVal = SYNTAX_ERROR;
                goto Exit;
            }
            OsSnprintf(szENIFilename, sizeof(szENIFilename) - 1, "%s", ptcWord);
        }
        else if( 0 == OsStricmp( ptcWord, "-log") )
        {
            EC_T_INT nPtcWordIndex = 4;

            /* Search for the start of the config file name. The config file
               name may start with quotation marks because of blanks in the filename */
            while(ptcWord[nPtcWordIndex] != '\0')
            {
                if(ptcWord[nPtcWordIndex] == '\"' || ptcWord[nPtcWordIndex] != ' ')
                {
                    break;
                }
                nPtcWordIndex++;
            }

            /* Depending of a config file name within quotation marks (or without
               quotation marks) extract the filename */
            if(ptcWord[nPtcWordIndex] == '\"')
            {
                /* Check if the strtok position is already correct */
                if(nPtcWordIndex > 3)
                {
                    /* More than 1 blank before -f. Correct strtok position. */
                    OsStrtok(EC_NULL,"\"");
                }

                /* Now extract the config file name */
                ptcWord = OsStrtok(EC_NULL,"\"");
            }
            else
            {
                /* Extract the config file name if it was not set within quotation marks */
                ptcWord = GetNextWord((EC_T_CHAR**)&szCommandLine, &tcStorage );
            }

            if (
                (ptcWord == EC_NULL)
             || (OsStrncmp( ptcWord, "-", 1) == 0)
               )
            {
                nRetVal = SYNTAX_ERROR;
                goto Exit;
            }
            OsSnprintf(szLogFileprefix, sizeof(szLogFileprefix) - 1, "%s", ptcWord);
        }
        else if (OsStricmp( ptcWord, "-t") == 0)
        {
            ptcWord = GetNextWord((EC_T_CHAR**)&szCommandLine, &tcStorage );
            if (
                (ptcWord == EC_NULL)
             || (OsStrncmp( ptcWord, "-", 1) == 0)
               )
            {
                nRetVal = SYNTAX_ERROR;
                goto Exit;
            }
            dwDuration = OsStrtol(ptcWord, EC_NULL, 0);
        }
        else if( OsStricmp( ptcWord, "-auxclk") == 0)
        {
#if (defined AUXCLOCK_SUPPORTED)
            TimingDesc.bUseAuxClock = EC_TRUE;
            ptcWord = GetNextWord((EC_T_CHAR**)&szCommandLine, &tcStorage);
            if (
                (ptcWord == EC_NULL)
                || (OsStrncmp( ptcWord, "-", 1) == 0)
                )
            {
                nRetVal = SYNTAX_ERROR;
                goto Exit;
            }
            TimingDesc.dwBusCycleTimeUsec = OsStrtol(ptcWord, EC_NULL, 0);
            if( TimingDesc.dwBusCycleTimeUsec < 10 )
            {
                TimingDesc.dwBusCycleTimeUsec = 10;
            }
#else
            OsDbgMsg( "Auxiliary clock not supported by this operating system!)\n" );
            goto Exit;
#endif
        }
        else if (OsStricmp( ptcWord, "-b") == 0)
        {
#if (defined AUXCLOCK_SUPPORTED)
            if( EC_TRUE == TimingDesc.bUseAuxClock )
            {
                OsDbgMsg( "Using bus cycle time %d usec from auxclock parameter\n", TimingDesc.dwBusCycleTimeUsec );
            }
            else
#endif
            {
                ptcWord = GetNextWord((EC_T_CHAR**)&szCommandLine, &tcStorage );
                if (
                    (ptcWord == EC_NULL)
                 || (OsStrncmp( ptcWord, "-", 1) == 0)
                   )
                {
                    nRetVal = SYNTAX_ERROR;
                    goto Exit;
                }
                TimingDesc.dwBusCycleTimeUsec = OsStrtol(ptcWord, EC_NULL, 0);
            }
        }
        else if (OsStricmp( ptcWord, "-a") == 0)
        {
            ptcWord = GetNextWord((EC_T_CHAR**)&szCommandLine, &tcStorage);
            if (
                (ptcWord == EC_NULL)
                || (OsStrncmp( ptcWord, "-", 1) == 0)
                )
            {
                nRetVal = SYNTAX_ERROR;
                goto Exit;
            }
            dwCpuIndex = OsStrtol(ptcWord, EC_NULL, 0);
        }
        else if (OsStricmp( ptcWord, "-v") == 0)
        {
            ptcWord = GetNextWord((EC_T_CHAR**)&szCommandLine, &tcStorage);
            if (
                (ptcWord == EC_NULL)
                || (OsStrncmp( ptcWord, "-", 1) == 0)
                )
            {
                nRetVal = SYNTAX_ERROR;
                goto Exit;
            }
            nVerbose = OsStrtol(ptcWord, EC_NULL, 10);
        }
        else if (OsStricmp( ptcWord, "-perf") == 0)
        {
            bEnaPerfJobs = EC_TRUE;
        }
#if (defined ATEMRAS_SERVER)
        else if( OsStricmp(ptcWord, "-sp") == 0 )
        {
            ptcWord = GetNextWord((EC_T_CHAR**)&szCommandLine, &tcStorage);
            if ((ptcWord == NULL) || (OsStrncmp(ptcWord, "-", 1) == 0))
            {
                wServerPort  = ATEMRAS_DEFAULT_PORT;

                /* optional sub parameter not found, use the current word for the next parameter */
                bGetNextWord = EC_FALSE;
            }
            else
            {
                wServerPort = (EC_T_WORD)OsStrtol(ptcWord, EC_NULL, 10);
            }
        }
#endif
        else if (OsStricmp(ptcWord, "-dcmmode") == 0)
        {
            /* Extract the config file name if it was not set within quotation marks */
            ptcWord = GetNextWord((EC_T_CHAR**)&szCommandLine, &tcStorage);

            if      ((EC_NULL != ptcWord) && (OsStricmp(ptcWord, "off") == 0))           eDcmMode = eDcmMode_Off;
            else if ((EC_NULL != ptcWord) && (OsStricmp(ptcWord, "busshift") == 0))       eDcmMode = eDcmMode_BusShift;
            else if ((EC_NULL != ptcWord) && (OsStricmp(ptcWord, "mastershift") == 0))    eDcmMode = eDcmMode_MasterShift;
            else if ((EC_NULL != ptcWord) && (OsStricmp(ptcWord, "masterrefclock") == 0)) eDcmMode = eDcmMode_MasterRefClock;
#if (defined INCLUDE_DCX)
            else if ((EC_NULL != ptcWord) && (OsStricmp(ptcWord, "dcx") == 0))            eDcmMode = eDcmMode_Dcx;
#endif
            else
            {
                nRetVal = SYNTAX_ERROR;
                goto Exit;
            }
        }
        else if (OsStricmp( ptcWord, "-ctloff") == 0)
        {
            bCtlOff = EC_TRUE;
        }
        else if (OsStricmp(ptcWord, "-key") == 0)
        {
            /* Extract the config file name if it was not set within quotation marks */
            ptcWord = GetNextWord((EC_T_CHAR**)&szCommandLine, &tcStorage);

            if      (EC_NULL != ptcWord)           licenseKey = ptcWord;
            else
            {
                nRetVal = SYNTAX_ERROR;
                goto Exit;
            }
        }
        else
        {
           dwRes = CreateLinkParmsFromCmdLine(&ptcWord, (EC_T_CHAR**)&szCommandLine, &tcStorage, &bGetNextWord, &apLinkParms[dwNumLinkLayer]);
           if (EC_E_NOERROR != dwRes)
           {
               nRetVal = SYNTAX_ERROR;
               goto Exit;
           }
           if (dwNumLinkLayer > 1)
           {
               nRetVal = SYNTAX_ERROR;
               goto Exit;
           }
           else
           {
#ifdef LINUX
               EC_CPUSET_ZERO(CpuSet);
               EC_CPUSET_SET(CpuSet, dwCpuIndex);
               apLinkParms[dwNumLinkLayer]->dwIstPriority = (CpuSet << 16) | RECV_THREAD_PRIO;
#else
               apLinkParms[dwNumLinkLayer]->dwIstPriority = RECV_THREAD_PRIO;
#endif
               dwNumLinkLayer++;
           }
        }
        /* get next word */
        if (bGetNextWord)
        {
            ptcWord = GetNextWord((EC_T_CHAR**)&szCommandLine, &tcStorage );
        }
        bGetNextWord = EC_TRUE;
    }
    /* initialize master logging */
    oLogging.InitLogging(0, LOG_ROLLOVER, LOG_THREAD_PRIO, dwCpuIndex, szLogFileprefix, LOG_THREAD_STACKSIZE);
    bLogInitialized = EC_TRUE;
#if !(defined XENOMAI) || (defined CONFIG_XENO_COBALT) || (defined CONFIG_XENO_MERCURY) 
    oLogging.SetLogThreadAffinity(dwCpuIndex);
#endif /* !XENOMAI || CONFIG_XENO_COBALT || CONFIG_XENO_MERCURY */
    OsAddDbgMsgHook(CAtEmLogging::OsDbgMsgHookWrapper);
    LogMsg("Full command line: %s\n", szFullCommandLine);

    /* determine master configuration type */
#if (defined __RCX__)
    eCnfType     = eCnfType_Data;
    pbyCnfData   = pStartup->pBecFile;
    dwCnfDataLen = pStartup->dwBecFileLen;
#else /* default */
    if ('\0' != szENIFilename[0])
    {
        eCnfType     = eCnfType_Filename;
        pbyCnfData   = (EC_T_BYTE*)&szENIFilename[0];
        dwCnfDataLen = 256;
    }
    else
    {
#if (defined STATIC_MASTERENI_XML_DATA)
        eCnfType     = eCnfType_Data;
        pbyCnfData   = STATIC_MASTERENI_XML_DATA;
        dwCnfDataLen = STATIC_MASTERENI_XML_DATA_SIZE;
#else
        eCnfType     = eCnfType_GenPreopENI;
#endif
    }
#endif
    if (0 == dwNumLinkLayer)
    {
        OsDbgMsg("Syntax error: missing link layer command line parameter\n");
        nRetVal = SYNTAX_ERROR;
        goto Exit;
    }
#if !(defined RTAI)
    /* for multi core cpus: select cpu number for this thread */
    EC_CPUSET_ZERO( CpuSet );
    EC_CPUSET_SET( CpuSet, dwCpuIndex );
    if( ! OsSetThreadAffinity(EC_NULL, CpuSet) )
    {
       OsDbgMsg("ERROR: Set Affinity Failed!\n");
    }
#endif
#if !(defined RTAI) && !(defined XENOMAI)
    TimingDesc.dwCpuIndex = dwCpuIndex;

    /* create timing event to trigger the job task */
    TimingDesc.pvTimingEvent = OsCreateEvent();
    if( EC_NULL == TimingDesc.pvTimingEvent )
    {
        OsDbgMsg("ERROR: insufficient memory to create timing event!\n");
        goto Exit;
    }

#if ((defined LINUX) && (defined AUXCLOCK_SUPPORTED))
    TimingDesc.bUseAuxClock = EC_TRUE;
#endif /* LINUX && AUXCLOCK_SUPPORTED */

#if (defined AUXCLOCK_SUPPORTED)
    /* initialize auxiliary clock */
    if( EC_TRUE == TimingDesc.bUseAuxClock )
    {
#if (defined VXWORKS)
        sysAuxClkDisable();
        if (OK != sysAuxClkRateSet(1000000 / TimingDesc.dwBusCycleTimeUsec))
        {
            OsDbgMsg("Error calling sysAuxClkRateSet!\n");
            goto Exit;
        }
        sysAuxClkConnect((FUNCPTR)vxAuxClkIsr, ((EC_T_INT)TimingDesc.pvTimingEvent));
        sysAuxClkEnable( );
        OsSleep(2000);

#elif ((defined UNDER_CE) && (_WIN32_WCE >= 0x600))
        /* get auxilary clock sysintr */
        bRes = KernelIoControl((DWORD)IOCTL_AUXCLK_GET_SYSINTR, (DWORD)NULL, (DWORD)0, &dwAuxClkSysIntr, (DWORD)sizeof(DWORD), &dwWinRes);
        if (!bRes)
        {
            OsDbgMsg("Error calling KernelIoControl(IOCTL_AUXCLK_GET_SYSINTR) (0x%08X)!\n", GetLastError());
            goto Exit;
        }
        /* open VirtualDrv for interrupt management */
        TimingDesc.hVirtualDrv = CreateFile(TEXT("VIR1:"),
                                 GENERIC_READ | GENERIC_WRITE,
                                 FILE_SHARE_READ | FILE_SHARE_WRITE,
                                 NULL,
                                 OPEN_EXISTING,
                                 FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
                                 INVALID_HANDLE_VALUE);
        if ((TimingDesc.hVirtualDrv == NULL) || (TimingDesc.hVirtualDrv == INVALID_HANDLE_VALUE))
        {
            OsDbgMsg("Error calling CreateFile(""VIR1:"") (0x%08X)!\n", GetLastError());
            TimingDesc.hVirtualDrv = NULL;
            goto Exit;
        }
        /* connect auxilary clock interrupt */
        TimingDesc.oIrqDesc.dwSysIrq = dwAuxClkSysIntr;
        swprintf(TimingDesc.oIrqDesc.szEventName, TEXT("%s"), TEXT("AUXCLK"));
        bRes = DeviceIoControl(TimingDesc.hVirtualDrv, (DWORD)IOCTL_VIRTDRV_INTERRUPT_INIT, &(TimingDesc.oIrqDesc), sizeof(VI_T_INTERRUPTDESC), NULL, 0, NULL, NULL );
        if (!bRes)
        {
            OsDbgMsg("Error calling DeviceIoControl(IOCTL_VIRTDRV_INTERRUPT_INIT) (0x%08X)!\n", GetLastError());
            goto Exit;
        }
        /* create auxilary clock interrupt event */
        TimingDesc.pvAuxClkEvent = (VOID*)CreateEvent(NULL, FALSE, FALSE, TEXT("AUXCLK"));
        if ((TimingDesc.pvAuxClkEvent == NULL) || (TimingDesc.pvTimingEvent == INVALID_HANDLE_VALUE))
        {
            OsDbgMsg("Error creating AuxClk event (0x%08X)!\n", GetLastError());
            TimingDesc.pvAuxClkEvent = NULL;
            goto Exit;
        }
        /* auxiliary clock event handled within timing task */
        bStartTimingTask = EC_TRUE;

#elif (defined UNDER_RTSS)
        hTimer = RtCreateTimer(NULL, 0, RtxAuxClkIsr, (PVOID)TimingDesc.pvTimingEvent, RT_PRIORITY_MAX, CLOCK_2);
        liTimer.QuadPart = (LONGLONG)10*TimingDesc.dwBusCycleTimeUsec;
        RtSetTimerRelative(hTimer, &liTimer, &liTimer);
#else /* !UNDER_RTSS */
        dwRes = OsAuxClkInit( dwCpuIndex, 1000000 / TimingDesc.dwBusCycleTimeUsec, TimingDesc.pvTimingEvent );
        if( EC_E_NOERROR != dwRes )
        {
            OsDbgMsg( "ERROR at auxiliary clock initialization!\n" );
            goto Exit;
        }
#endif /* !UNDER_RTSS */
    } // if( EC_TRUE == TimingDesc.bUseAuxClock )
    else
#endif
    {
#if (defined RTOS_32)
        CLKSetTimerIntVal( TimingDesc.dwBusCycleTimeUsec );
        RTKDelay( 1 );
#endif /* RTOS_32 */
#if !(defined NO_OS)
        bStartTimingTask = EC_TRUE;
#endif /* !NO_OS */

#if (defined EC_VERSION_RTEMS)
        status = rtems_timer_create(rtems_build_name('E', 'C', 'T', 'T'), &timerId);
        if(RTEMS_SUCCESSFUL != status)
        {
        	OsDbgMsg("ATEMDemoMain: cannot create timer\nRTEMS returned: %s\n",
        			rtems_status_text(status));
        	goto Exit;
        }
#if (defined RTEMS_USE_TIMER_SERVER)
        status = rtems_timer_initiate_server(TIMER_THREAD_PRIO,TIMER_THREAD_STACKSIZE,0);
        if(RTEMS_SUCCESSFUL != status)
        {
        	OsDbgMsg("ATEMDemoMain: cannot initialise timer\nRTEMS returned: %s\n",
        			rtems_status_text(status));
        	goto Exit;
        }
        status = rtems_timer_server_fire_after(timerId,
        RTEMS_MICROSECONDS_TO_TICKS(TimingDesc.dwBusCycleTimeUsec),
        rtemsTimerIsr, TimingDesc.pvTimingEvent);
#else
        status = rtems_timer_fire_after(timerId,
        		RTEMS_MICROSECONDS_TO_TICKS(TimingDesc.dwBusCycleTimeUsec),
        		rtemsTimerIsr, TimingDesc.pvTimingEvent);
#endif /* RTEMS_USE_TIMER_SERVER */
        if(RTEMS_SUCCESSFUL != status)
        {
        	OsDbgMsg("ATEMDemoMain: cannot initialise timer\nRTEMS returned: %s\n",
        			rtems_status_text(status));
        	goto Exit;
        }
        bStartTimingTask = EC_FALSE; //No timing task needed
#endif /* EC_VERSION_RTEMS */
    }
    /* create timing task if needed */
    if (bStartTimingTask)
    {
        OsCreateThread( (EC_T_CHAR*)"tEcTimingTask", (EC_PF_THREADENTRY)tEcTimingTask, TIMER_THREAD_PRIO, LOG_THREAD_STACKSIZE, (EC_T_VOID*)&TimingDesc );
        while( !TimingDesc.bIsRunning )
        {
            OsSleep( 1 );
        }
    }
#endif /* !RTAI && !XENOMAI*/
    OsDbgMsg( "Run demo now with cycle time %d usec\n", TimingDesc.dwBusCycleTimeUsec);
#if (defined AUXCLOCK_SUPPORTED)
    OsDbgMsg("Using %s\n",
          (TimingDesc.bUseAuxClock ? "AuxClock" : "Sleep"));
#endif
// 	void callbackFct(EC_T_BYTE* pbyPDInPtr, EC_T_BYTE* pbyPDOutPtr);
    dwRes = EtherCATStack(   &oLogging,
                      eCnfType, pbyCnfData, dwCnfDataLen,
                      TimingDesc.dwBusCycleTimeUsec, nVerbose, dwDuration,
//                       TimingDesc.dwBusCycleTimeUsec, 0, dwDuration,
                      apLinkParms[0],
                      TimingDesc.pvTimingEvent, dwCpuIndex,
                      bEnaPerfJobs
#if (defined ATEMRAS_SERVER)
                      ,wServerPort
#endif
                      ,((2 == dwNumLinkLayer)?apLinkParms[1]:EC_NULL)
                      , &oOsParms
                      , eDcmMode
                      , bCtlOff
                      ,&callbackFct
                      ,licenseKey
                    );
    if (EC_E_NOERROR != dwRes)
    {
        goto Exit;
    }
    /* no errors */
    nRetVal = APP_NOERROR;

Exit:
    if (nRetVal == SYNTAX_ERROR)
    {
        ShowSyntax();
    }
    OsDbgMsg("EcMasterDemoDc stop.\n");
#if !(defined VXWORKS)
    if (nRetVal != APP_NOERROR)
    {
        OsSleep(5000);
    }
#endif
    /* stop timing task if running */
#if (defined EC_VERSION_RTEMS)
    rtems_timer_delete(timerId);
#else
    if (EC_TRUE == TimingDesc.bIsRunning)
    {
        TimingDesc.bShutdown = EC_TRUE;
        while( TimingDesc.bIsRunning )
        {
            OsSleep( 1 );
        }
    }
#endif
#if (defined AUXCLOCK_SUPPORTED)
    /* clean up auxclk */
    if( TimingDesc.bUseAuxClock )
    {
#if (defined EC_VERSION_WINDOWS) || (defined LINUX)
        OsAuxClkDeinit(0);

#elif (defined VXWORKS)
        sysAuxClkDisable();

#elif ((defined UNDER_CE) && (_WIN32_WCE >= 0x600))
        if( NULL != TimingDesc.hVirtualDrv )
        {
            /* deinit the auxilary clock interrupt and close the handle to it */
            TimingDesc.oIrqDesc.dwSysIrq = dwAuxClkSysIntr;
            bRes = DeviceIoControl(TimingDesc.hVirtualDrv, (DWORD)IOCTL_VIRTDRV_INTERRUPT_DEINIT, &(TimingDesc.oIrqDesc), sizeof(VI_T_INTERRUPTDESC), NULL, 0, NULL, NULL );
            if (!bRes)
            {
                printf("Error calling DeviceIoControl(IOCTL_VIRTDRV_INTERRUPT_DEINIT) (0x%08X)!\n", GetLastError());
            }
            CloseHandle(TimingDesc.hVirtualDrv);
            TimingDesc.hVirtualDrv = NULL;
        }
        /* Close the AUXCLK-TimingTask synchronization handle */
        if( EC_NULL != TimingDesc.pvAuxClkEvent )
        {
            CloseHandle(TimingDesc.pvAuxClkEvent);
            TimingDesc.pvAuxClkEvent = EC_NULL;
        }
#elif (defined UNDER_RTSS)
        if (NULL != hTimer)
        {
             RtCancelTimer(hTimer, &liTimer);
             RtDeleteTimer(hTimer);
        }
#endif
    }
#endif
    /* delete the timing event */
    if( EC_NULL != TimingDesc.pvTimingEvent )
    {
        OsDeleteEvent( TimingDesc.pvTimingEvent );
        TimingDesc.pvTimingEvent = EC_NULL;
    }

    if (bLogInitialized)
    {
        /* de-initialize message logging */
        oLogging.DeinitLogging();
    }
    /* final OS layer cleanup */
    OsDeinit();

    /* free link parms created by CreateLinkParmsFromCmdLine() */
    for (;dwNumLinkLayer != 0; dwNumLinkLayer--)
    {
        if (EC_NULL != apLinkParms[dwNumLinkLayer-1])
        {
            OsFree(apLinkParms[dwNumLinkLayer-1]);
            apLinkParms[dwNumLinkLayer-1] = EC_NULL;
        }
    }

#if (defined EC_VERSION_GO32)
    irq_Close();    /* close the Vortex86 IRQ library */
    io_Close();     /* close the Vortex86 I/O library */
#endif

#if (defined EC_VERSION_RTEMS)
    rtemsSyncBDBuffers();
    exit(nRetVal);
#elif !(defined RTAI)
    return nRetVal;
#endif
}

#if (defined EC_VERSION_RTEMS)
rtems_task Init(rtems_task_argument arg)
{
	rtems_id   Task_id;
	rtems_name Task_name = rtems_build_name('M','A','I','N');

	/* read time of day from rtc device and set it to rtems */
	setRealTimeToRTEMS();
	/* Mount file systems */
	rtemsMountFilesystems();
	/* create and start main task */
	rtems_task_create(Task_name, MAIN_THREAD_PRIO,
			RTEMS_MINIMUM_STACK_SIZE * 4, RTEMS_DEFAULT_MODES,
			RTEMS_FLOATING_POINT | RTEMS_DEFAULT_ATTRIBUTES, &Task_id);
	rtems_task_start(Task_id, Main, 1);
//	rtems_monitor_init(0);
	rtems_task_delete( RTEMS_SELF );
}
#endif /* EC_VERSION_RTEMS */

/*-Handle static linked link layers -----------------------------------------*/
#if !defined(ATECAT_DLL)
#if (defined RTOS_32) || (defined __INTEGRITY) || ((defined __TKERNEL)&& (!defined __arm__)) || (defined __RCX__) || (defined RTAI) || (defined EC_VERSION_ETKERNEL) || (defined EC_VERSION_SYSBIOS) || (defined EC_VERSION_RTEMS)
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */
#include "EcLink.h"
extern EC_T_DWORD emllRegisterI8255x (EC_T_LINK_DRV_DESC* pLinkDrvDesc, EC_T_DWORD dwLinkDrvDescSize);
extern EC_T_DWORD emllRegisterI8254x (EC_T_LINK_DRV_DESC* pLinkDrvDesc, EC_T_DWORD dwLinkDrvDescSize);
extern EC_T_DWORD emllRegisterRTL8139(EC_T_LINK_DRV_DESC* pLinkDrvDesc, EC_T_DWORD dwLinkDrvDescSize);
extern EC_T_DWORD emllRegisterRTL8169(EC_T_LINK_DRV_DESC* pLinkDrvDesc, EC_T_DWORD dwLinkDrvDescSize);
extern EC_T_DWORD emllRegisterHnx    (EC_T_LINK_DRV_DESC* pLinkDrvDesc, EC_T_DWORD dwLinkDrvDescSize);
extern EC_T_DWORD emllRegisterCPSW   (EC_T_LINK_DRV_DESC* pLinkDrvDesc, EC_T_DWORD dwLinkDrvDescSize);
extern EC_T_DWORD emllRegisterCCAT   (EC_T_LINK_DRV_DESC* pLinkDrvDesc, EC_T_DWORD dwLinkDrvDescSize);
extern EC_T_DWORD emllRegisterICSS   (EC_T_LINK_DRV_DESC* pLinkDrvDesc, EC_T_DWORD dwLinkDrvDescSize);

EC_PF_LLREGISTER OsGetLinkLayerRegFunc(EC_T_CHAR* szDriverIdent)
{
EC_PF_LLREGISTER pfLlRegister = EC_NULL;

#if (defined LINKLAYER_I8254X)
    if (0 == OsStrcmp("I8254x", szDriverIdent))
    {
        pfLlRegister = (EC_PF_LLREGISTER)emllRegisterI8254x;
    } else
#endif
#if (defined LINKLAYER_I8255X)
    if (0 == OsStrcmp("I8255x", szDriverIdent))
    {
        pfLlRegister = (EC_PF_LLREGISTER)emllRegisterI8255x;
    } else
#endif
#if (defined LINKLAYER_RTL8139)
    if (0 == OsStrcmp("RTL8139", szDriverIdent))
    {
        pfLlRegister = (EC_PF_LLREGISTER)emllRegisterRTL8139;
    } else
#endif
#if (defined LINKLAYER_RTL8169)
    if (0 == OsStrcmp("RTL8169", szDriverIdent))
    {
        pfLlRegister = (EC_PF_LLREGISTER)emllRegisterRTL8169;
    } else
#endif
#if (defined LINKLAYER_HNX)
    if (0 == OsStrcmp("Hnx", szDriverIdent))
    {
        pfLlRegister = (EC_PF_LLREGISTER)emllRegisterHnx;
    } else
#endif
#if (defined LINKLAYER_FSLFEC)
    if (0 == OsStrcmp(EC_LINK_PARMS_IDENT_FSLFEC, szDriverIdent))
    {
        pfLlRegister = (EC_PF_LLREGISTER)emllRegisterFslFec;
    } else
#endif
#if (defined LINKLAYER_CPSW)
    if (0 == OsStrcmp(EC_LINK_PARMS_IDENT_CPSW, szDriverIdent))
    {
        pfLlRegister = (EC_PF_LLREGISTER)emllRegisterCPSW;
    } else
#endif
#if (defined LINKLAYER_ICSS)
    if (0 == OsStrcmp(EC_LINK_PARMS_IDENT_ICSS, szDriverIdent))
    {
        pfLlRegister = (EC_PF_LLREGISTER)emllRegisterICSS;
    } else
#endif
#if (defined LINKLAYER_CCAT)
    if (0 == OsStrcmp(EC_LINK_PARMS_IDENT_CCAT, szDriverIdent))
    {
        pfLlRegister = (EC_PF_LLREGISTER)emllRegisterCCAT;
    } else
#endif
    {
        pfLlRegister = EC_NULL;
    }
    return pfLlRegister;
}
#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */
#endif /* RTOS_32 ||  __INTEGRITY || __TKERNEL || __RCX__ || RTAI */
#endif /* !ATECAT_DLL */

/*-END OF SOURCE FILE--------------------------------------------------------*/


EtherCATMain::EtherCATMain(int nArgc, char* ppArgv[], int bufferSize) :
nArgc(nArgc), ppArgv(ppArgv), bufferSize(bufferSize),
thread(mainEtherCAT, nArgc, ppArgv )
{ 
	lockInBuffer.lockCount=0;
	lockOutBuffer.lockCount=0;
	inBuffer = new uint8_t[bufferSize];
	outBuffer = new uint8_t[bufferSize];
	ecatGetMasterState();
}

EtherCATMain::~EtherCATMain() {
	delete []inBuffer;
	delete []outBuffer;
}

EtherCATMain* EtherCATMain::instance;

EtherCATMain* EtherCATMain::createInstance(int nArgc, char* ppArgv[], int bufferSize) {
	if(instance == NULL) instance = new EtherCATMain(nArgc, ppArgv, bufferSize);
	return instance;
}

EtherCATMain* EtherCATMain::getInstance() {
	return instance;
}


bool EtherCATMain::isRunning()
{
	return bRun;
}

void EtherCATMain::join()
{
	thread.join();
}

void EtherCATMain::stop()
{
	bRun = EC_FALSE;
}


masterState EtherCATMain::getMasterState()
{
	switch ( ecatGetMasterState() ) 
	{
		case eEcatState_UNKNOWN		: return ethercat::UNKNOWN;
		case eEcatState_INIT		: return ethercat::INIT;
		case eEcatState_PREOP		: return ethercat::PREOP;
		case eEcatState_SAFEOP		: return ethercat::SAFEOP;
		case eEcatState_OP			: return ethercat::OP;
		case eEcatState_BOOTSTRAP	: return ethercat::BOOTSTRAP;
	}
}


// get from buffer functions
EC_T_WORD EtherCATMain::getFrmWord(uint8_t* address)
{
	lockInBuffer.lock();
	EC_T_WORD val = EC_GET_FRM_WORD( address );
	lockInBuffer.unlock();
	return val;
}

EC_T_DWORD EtherCATMain::getFrmDWord(uint8_t* address)
{
	lockInBuffer.lock();
	EC_T_DWORD val = EC_GET_FRM_DWORD( address );
	lockInBuffer.unlock();
	return val;
}



// set to buffer funcitons
void EtherCATMain::setByte(uint8_t* address, EC_T_BYTE val)
{
	lockOutBuffer.lock();
// 	EC_T_BYTE *pointer = static_cast<EC_T_BYTE*>( address )
// 	*(pointer) = val;
	*(address) = val;
	lockOutBuffer.unlock();
}

void EtherCATMain::setWord(uint8_t* address, EC_T_WORD val)
{
	lockOutBuffer.lock();
	EC_SETWORD( address, val );
	lockOutBuffer.unlock();
}

void EtherCATMain::setDWord(uint8_t* address, EC_T_DWORD val)
{
	lockOutBuffer.lock();
	EC_SETDWORD( address, val );
	lockOutBuffer.unlock();
}

