#ifndef ETHERCAT_ECATDEMOCOMMON_H
#define ETHERCAT_ECATDEMOCOMMON_H

/*-----------------------------------------------------------------------------
 * ecatDemoCommon.h
 * Copyright                acontis technologies GmbH, Weingarten, Germany
 * Response                 Stefan Zintgraf
 * Description              header for demo common parts
 *---------------------------------------------------------------------------*/

#ifndef __ECATDEMOCOMMON_H__
#define __ECATDEMOCOMMON_H__  1

/*-INCLUDES------------------------------------------------------------------*/
#include <EcInterfaceCommon.h>
#include "SlaveInfo.h"
#include "Logging.h"

/*-DEFINES-------------------------------------------------------------------*/
#define ETHERCAT_STATE_CHANGE_TIMEOUT   15000   /* master state change timeout in ms */
#define ETHERCAT_SCANBUS_TIMEOUT        5000    /* scanbus timeout in ms */

#define ETHERCAT_DC_TIMEOUT             12000   /* DC initialization timeout in ms */
#define ETHERCAT_DC_ARMW_BURSTCYCLES    10000   /* DC burst cycles (static drift compensation) */
#define ETHERCAT_DC_ARMW_BURSTSPP       12      /* DC burst bulk (static drift compensation) */
#define ETHERCAT_DC_DEV_LIMIT           13      /* DC deviation limit (highest bit tolerate by the broadcast read) */
#define ETHERCAT_DC_SETTLE_TIME         1500    /* DC settle time in ms */

#define APP_ERROR         -1
#define SYNTAX_ERROR      -2
#define APP_OUT_OF_MEMORY -3
#define APP_NOERROR        0

/* tag names for DemoConfig.xml file */
#define DEMO_CFG_DEFAULT_FILENAME       (EC_T_CHAR*)"DemoConfig.xml"
#define DEMO_CFG_TAG_ENI_FILENAME       (EC_T_CHAR*)"Config\\Common\\ENIFileName"
#define DEMO_CFG_TAG_LOG_FILEPREFIX     (EC_T_CHAR*)"Config\\Common\\LogFilePrefix"
#define DEMO_CFG_TAG_LINK_LAYER         (EC_T_CHAR*)"Config\\Common\\LinkLayer"
#define DEMO_CFG_TAG_LINK_LAYER2        (EC_T_CHAR*)"Config\\Common\\LinkLayer2"
#define DEMO_CFG_TAG_DURATION           (EC_T_CHAR*)"Config\\Common\\DemoDuration"
#define DEMO_CFG_TAG_CPU_AFFINITY       (EC_T_CHAR*)"Config\\Common\\CpuAffinity"
#define DEMO_CFG_TAG_VERBOSITY_LVL      (EC_T_CHAR*)"Config\\Common\\VerbosityLevel"
#define DEMO_CFG_TAG_PERF_MEASURE       (EC_T_CHAR*)"Config\\Common\\PerfMeasurement"
#define DEMO_CFG_TAG_RAS_ENABLED        (EC_T_CHAR*)"Config\\Common\\RASEnabled"
#define DEMO_CFG_TAG_RAS_PORT           (EC_T_CHAR*)"Config\\Common\\RASPort"
#define DEMO_CFG_TAG_AUXCLK             (EC_T_CHAR*)"Config\\Common\\AuxClk"
#define DEMO_CFG_TAG_BUSCYCLETIME       (EC_T_CHAR*)"Config\\Common\\BusCycleTime"

/*-FORWARD DECLARATIONS------------------------------------------------------*/
class CEmNotification;
struct _EC_T_MBXTFER;

/*-TYPEDEFS------------------------------------------------------------------*/
typedef struct _T_DEMO_THREAD_PARAM
{
    EC_T_DWORD          dwMasterID;             /**< [in]   Master instance */
    EC_T_BOOL           bJobThreadRunning;      /**< [in]   Job Thread is running */
    EC_T_BOOL           bJobThreadShutdown;     /**< [in]   Job Thread Shutdown requested */
    EC_T_VOID*          pvTimingEvent;          /**< [in]   Event create for timing purposes */
    EC_T_DWORD          dwCpuIndex;             /**< [in]   SMP systems: CPU index */
    CAtEmLogging*       pLogInst;               /**< [in]   common base class pointer */
    CEmNotification*    pNotInst;               /**< [in]   Notification class pointer */
    EC_T_VOID*          pvApplSpecParm1;        /**< [in]   application specific parameters 1 */
    EC_T_VOID*          pvApplSpecParm2;        /**< [in]   application specific parameters 2 */
    EC_T_DWORD          dwDcmCtlCycleDivider;   /**< [in]   DCM controller cycle divider*/
    EC_T_BOOL           bDcmCtlInitialized;     /**< [in]   EC_TRUE if DCM controller is initialized */
    EC_T_DWORD          dwBusCycleTimeUsec;     /**< [in]   bus cycle time in usec */
} T_DEMO_THREAD_PARAM;

/*-FUNCTION DECLARATION------------------------------------------------------*/
EC_T_DWORD CoeReadObjectDictionary(
    EC_T_DWORD    dwInstanceID 
   ,CAtEmLogging* poLog
   ,EC_T_INT      nVerbosePrinting   /**< [in]   Print messages */
   ,EC_T_BOOL*    pbStopReading   /**< [in]   Pointer to "Stop Reading" flag */
   ,EC_T_DWORD    dwClientId      /**< [in]   Current client ID */
   ,EC_T_DWORD    dwNodeId        /**< [in]   Slave Id to query ODL from  */
   ,EC_T_BOOL     bPerformUpload  /**< [in]   EC_TRUE: do SDO Upload */
   ,EC_T_DWORD    dwTimeout       /**< [in]   Individual call time-out */
   );
EC_T_VOID SetCycErrorNotifyMask(
    EC_T_DWORD    dwInstanceID
   ,EC_T_DWORD    dwCycErrorNotifyMask
   );
EC_T_VOID LinkDbgMsg(
    EC_T_DWORD    dwInstanceID
   ,EC_T_BYTE     byEthTypeByte0
   ,EC_T_BYTE     byEthTypeByte1
   ,EC_T_CHAR*    szMsg
   );
EC_T_DWORD SetFrameLoss(
    EC_T_DWORD    dwInstanceID
   ,CAtEmLogging* poLog
   ,EC_T_DWORD    dwNumGoodFramesAfterStart
   ,EC_T_DWORD    dwLikelihoodPpm
   ,EC_T_DWORD    dwFixedLossNumGoodFrms
   ,EC_T_DWORD    dwFixedLossNumBadFrms
   ,EC_T_BOOL     bSuppressCyclicMessage
   );
EC_T_BOOL PrintSlaveInfos(
    EC_T_DWORD    dwInstanceID
   ,CAtEmLogging* poLog
   );
EC_T_VOID PrintCfgSlavesInfo(
    EC_T_DWORD    dwInstanceID
   ,CAtEmLogging* poLog
   );
EC_T_VOID PrintBusSlavesInfo(
    EC_T_DWORD    dwInstanceID
   ,CAtEmLogging* poLog
   );
EC_T_BOOL FindSlaveGetFixedAddr(
    EC_T_DWORD    dwInstanceID
   ,CAtEmLogging* poLog
   ,EC_T_DWORD    dwSlaveInstance       /**< [in]   Slave instance (0 = first matching, 1 = second, ...) */
   ,EC_T_DWORD    dwVendorId            /**< [in]   Vendor Id of slave to search */
   ,EC_T_DWORD    dwProductCode         /**< [in]   Product Code of slave to search */
   ,EC_T_WORD*    pwPhysAddr            /**< [out]  Station address of slave */
   );
EC_T_DWORD DoBusScan(
    EC_T_DWORD    dwInstanceID
   ,CAtEmLogging* poLog
   ,EC_T_SB_STATUS_NTFY_DESC* poStatus
   ,EC_T_BOOL     bWait
   ,EC_T_BOOL     bNtfyActive
   ,EC_T_DWORD    dwScanBustimeout = ETHERCAT_SCANBUS_TIMEOUT
   );
EC_T_CHAR* SlaveVendorText(T_eEtherCAT_Vendor EV);
EC_T_CHAR* SlaveProdCodeText(T_eEtherCAT_Vendor EV, T_eEtherCAT_ProductCode EPC);
EC_T_CHAR* ESCTypeText(EC_T_BYTE byESCType);
EC_T_VOID  HandleMbxTferReqError(
    CAtEmLogging* poLog
   ,EC_T_CHAR*    szErrMsg
   ,EC_T_DWORD    dwErrorCode
   ,struct _EC_T_MBXTFER* pMbxTfer
   );


/*-GLOBAL VARIABLES-----------------------------------------------------------*/
extern volatile EC_T_BOOL bRun;             /* global helper variable to shutdown the application */

#endif /*__ECATDEMOCOMMON_H__*/

/*-END OF SOURCE FILE--------------------------------------------------------*/
#endif // ETHERCAT_ECATDEMOCOMMON_H