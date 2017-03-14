/*-----------------------------------------------------------------------------
 * AtEthercat.h
 * Copyright                acontis technologies GmbH, Weingarten, Germany
 * Response                 Stefan Zintgraf
 * Description              interface to the ethercat master
 *---------------------------------------------------------------------------*/

#ifndef INC_ATETHERCAT
#define INC_ATETHERCAT 1

/*-INCLUDE-------------------------------------------------------------------*/
#ifndef INC_ECOS
#include "EcOs.h"
#endif
#ifndef INC_ECERROR
#include "EcError.h"
#endif
#ifndef INC_ECINTERFACECOMMON
#include "EcInterfaceCommon.h"
#endif
#ifndef INC_ECVERSION
#include "EcVersion.h"
#endif
#ifndef INC_ECLINK
#include "EcLink.h"
#endif
#ifndef INC_ETHERNETSERVICES
#include "EthernetServices.h"
#endif

/*-DEFINES/MACROS------------------------------------------------------------*/

#define ATECAT_SIGNATURE_PATTERN                    0xAEC00000

#define ATECAT_SIGNATURE (  ATECAT_SIGNATURE_PATTERN       \
                         | (ATECAT_VERS_MAJ         << 16) \
                         | (ATECAT_VERS_MIN         << 12) \
                         | (ATECAT_VERS_SERVICEPACK <<  8) \
                         | (ATECAT_VERS_BUILD       <<  0) \
                         )

/* EtherCat specific control codes */
#define EC_IOCTL_GENERIC                            0x00000000
#define EC_IOCTL_DC                                 0x00030000
#define EC_IOCTL_SB                                 0x00050000
#define EC_IOCTL_HC                                 0x00060000
#define EC_IOCTL_DCM                                0x00070000
#define EC_IOCTL_USER                               0x00F00000  /* for user extension */
#define EC_IOCTL_PRIVATE                            0x00FF0000  /* private, internal IOCTL values */
#define EC_IOCTL_LINKLAYER                          0xCA000000
#define EC_IOCTL_LINKLAYER_MAIN                     EC_IOCTL_LINKLAYER
#define EC_IOCTL_LINKLAYER_RED                      0xCB000000
#define EC_IOCTL_LINKLAYER_LAST                     0xCBFFFFFF
#define EC_IOCTL_PRIVATE2                           0xCC000000  /* private, internal IOCTL values */

#define EC_IOCTL_REGISTERCLIENT                     (EC_IOCTL_GENERIC |  2)
#define EC_IOCTL_UNREGISTERCLIENT                   (EC_IOCTL_GENERIC |  3)

#define EC_IOCTL_ISLINK_CONNECTED                   (EC_IOCTL_GENERIC |  6)

#define EC_IOCTL_SET_CYC_ERROR_NOTIFY_MASK          (EC_IOCTL_GENERIC |  8)

#define EC_IOCTL_LINKLAYER_DBG_MSG                  (EC_IOCTL_GENERIC | 10)

#define EC_IOCTL_RESET_SLAVE                        (EC_IOCTL_GENERIC | 13)
#define EC_IOCTL_SLAVE_LINKMESSAGES                 (EC_IOCTL_GENERIC | 14)
#define EC_IOCTL_GET_CYCLIC_CONFIG_INFO             (EC_IOCTL_GENERIC | 15)
#define EC_IOCTL_GET_LINKLAYER_MODE                 (EC_IOCTL_GENERIC | 16)
#define EC_IOCTL_IS_SLAVETOSLAVE_COMM_CONFIGURED    (EC_IOCTL_GENERIC | 17)
#define EC_IOCTL_INITIATE_UPDATE_ALL_SLAVE_STATE    (EC_IOCTL_GENERIC | 19)
#define EC_IOCTL_ADD_BRD_SYNC_WINDOW_MONITORING     (EC_IOCTL_GENERIC | 20)
#define EC_IOCTL_ONLY_PROCESS_DATA_IN_IMAGE         (EC_IOCTL_GENERIC | 21)
#define EC_IOCTL_REGISTER_CYCFRAME_RX_CB            (EC_IOCTL_GENERIC | 22)
#define EC_IOCTL_SET_PD_OFFSET_COMPAT_MODE          (EC_IOCTL_GENERIC | 23)
#define EC_IOCTL_IS_MAIN_LINK_CONNECTED             (EC_IOCTL_GENERIC | 24)
#define EC_IOCTL_IS_RED_LINK_CONNECTED              (EC_IOCTL_GENERIC | 25)
#define EC_IOCTL_ADD_COE_INITCMD                    (EC_IOCTL_GENERIC | 26)

    /* Memory Provider */
#define EC_IOCTL_GET_PDMEMORYSIZE                   (EC_IOCTL_GENERIC | 40)
#define EC_IOCTL_REGISTER_PDMEMORYPROVIDER          (EC_IOCTL_GENERIC | 41)

#define EC_IOCTL_FORCE_BROADCAST_DESTINATION        (EC_IOCTL_GENERIC | 42)     /* obsolete */

    /* Slave Statistics Retrieval */
#if (defined INCLUDE_SLAVE_STATISTICS)
#define EC_IOCTL_SET_SLVSTAT_PERIOD                 (EC_IOCTL_GENERIC | 43)
#define EC_IOCTL_FORCE_SLVSTAT_COLLECTION           (EC_IOCTL_GENERIC | 44)
#define EC_IOCTL_GET_SLVSTATISTICS                  (EC_IOCTL_GENERIC | 45)
#define EC_IOCTL_CLR_SLVSTATISTICS                  (EC_IOCTL_GENERIC | 46)
#endif

#define EC_IOCTL_SET_MBX_RETRYACCESS_COUNT          (EC_IOCTL_GENERIC | 47)
#define EC_IOCTL_SET_MBX_RETRYACCESS_PERIOD         (EC_IOCTL_GENERIC | 48)

#define EC_IOCTL_ALL_SLAVES_MUST_REACH_MASTER_STATE (EC_IOCTL_GENERIC | 49)

#define EC_IOCTL_SET_NOTIFICATION_CTL               (EC_IOCTL_GENERIC | 50)

#define EC_IOCTL_MASTEROD_SET_VALUE                 (EC_IOCTL_GENERIC | 51)
#define EC_IOCTL_SET_CYCFRAME_LAYOUT                (EC_IOCTL_GENERIC | 52)

#define EC_IOCTL_SET_NOTIFICATION_ENABLED           (EC_IOCTL_GENERIC | 53)
#define EC_IOCTL_GET_NOTIFICATION_ENABLED           (EC_IOCTL_GENERIC | 54)

#define EC_IOCTL_SET_MASTER_DEFAULT_TIMEOUTS                  (EC_IOCTL_GENERIC | 55)
#define EC_IOCTL_SET_COPYINFO_IN_SENDCYCFRAMES                (EC_IOCTL_GENERIC | 56)
#define EC_IOCTL_SET_BUS_CYCLE_TIME                           (EC_IOCTL_GENERIC | 57)
#define EC_IOCTL_ADDITIONAL_VARIABLES_FOR_SPECIFIC_DATA_TYPES (EC_IOCTL_GENERIC | 58)
#define EC_IOCTL_SET_IGNORE_INPUTS_ON_WKC_ERROR               (EC_IOCTL_GENERIC | 59)
#define EC_IOCTL_SET_GENENI_ASSIGN_EEPROM_BACK_TO_ECAT        (EC_IOCTL_GENERIC | 60)
#define EC_IOCTL_SET_AUTO_ACK_AL_STATUS_ERROR_ENABLED         (EC_IOCTL_GENERIC | 61)
#define EC_IOCTL_SET_AUTO_ADJUST_CYCCMD_WKC_ENABLED           (EC_IOCTL_GENERIC | 62)
#define EC_IOCTL_CLEAR_MASTER_INFO_COUNTERS                   (EC_IOCTL_GENERIC | 63)

#if (defined INCLUDE_DC_SUPPORT)
    /* Distributed Clocks (DC) */
#define EC_IOCTL_REG_DC_SLV_SYNC_NTFY               (EC_IOCTL_DC |  3)
#define EC_IOCTL_UNREG_DC_SLV_SYNC_NTFY             (EC_IOCTL_DC |  4)
#define EC_IOCTL_DC_SLV_SYNC_STATUS_GET             (EC_IOCTL_DC |  5)
#define EC_IOCTL_DC_SLV_SYNC_DEVLIMIT_SET           (EC_IOCTL_DC |  6)
#define EC_IOCTL_DC_SLV_SYNC_DEVLIMIT_GET           (EC_IOCTL_DC |  7)
#define EC_IOCTL_DC_SHIFT_SYSTIME                   (EC_IOCTL_DC | 16)
#define EC_IOCTL_DC_SETSYNCSTARTOFFSET              (EC_IOCTL_DC | 17)
#define EC_IOCTL_DC_FIRST_DC_SLV_AS_REF_CLOCK       (EC_IOCTL_DC | 18)

    /* DC Master Sync (DCM) */
#define EC_IOCTL_DCM_REGISTER_TIMESTAMP             (EC_IOCTL_DCM |  1)
#define EC_IOCTL_DCM_UNREGISTER_TIMESTAMP           (EC_IOCTL_DCM |  2)
#define EC_IOCTL_DCM_REGISTER_STARTSO_CALLBACK      (EC_IOCTL_DCM |  3)
#define EC_IOCTL_DCM_GET_LOG                        (EC_IOCTL_DCM |  4)
#endif /* INCLUDE_DC_SUPPORT */

    /* Scan Bus (SB) */
#define EC_IOCTL_SB_RESTART                         (EC_IOCTL_SB | 1)           /* 0x00050001 */
#define EC_IOCTL_SB_STATUS_GET                      (EC_IOCTL_SB | 2)           /* 0x00050002 */
#define EC_IOCTL_SB_SET_BUSCNF_VERIFY               (EC_IOCTL_SB | 3)           /* 0x00050003 */
#define EC_IOCTL_SB_SET_BUSCNF_VERIFY_PROP          (EC_IOCTL_SB | 4)           /* 0x00050004 */
#define EC_IOCTL_SB_BUSCNF_GETSLAVE_INFO            (EC_IOCTL_SB | 5)           /* 0x00050005 */
#define EC_IOCTL_SB_BUSCNF_GETSLAVE_INFO_EEP        (EC_IOCTL_SB | 6)           /* 0x00050006 */
#define EC_IOCTL_SB_ENABLE                          (EC_IOCTL_SB | 7)           /* 0x00050007 */
#define EC_IOCTL_SB_BUSCNF_GETSLAVE_INFO_EX         (EC_IOCTL_SB | 9)           /* 0x00050009 */
#define EC_IOCTL_SLV_ALIAS_ENABLE                   (EC_IOCTL_SB |10)           /* 0x0005000A */
#define EC_IOCTL_SB_SET_BUSCNF_READ_PROP            (EC_IOCTL_SB |12)           /* 0x0005000C */
#define EC_IOCTL_SB_SET_TOPOLOGY_CHANGED_DELAY      (EC_IOCTL_SB |13)           /* 0x0005000D */
#define EC_IOCTL_SB_SET_ERROR_ON_CROSSED_LINES      (EC_IOCTL_SB |14)           /* 0x0005000E */
#define EC_IOCTL_SB_SET_TOPOLOGY_CHANGE_AUTO_MODE   (EC_IOCTL_SB |15)           /* 0x0005000F */
#define EC_IOCTL_SB_ACCEPT_TOPOLOGY_CHANGE          (EC_IOCTL_SB |16)           /* 0x00050010 */
#define EC_IOCTL_SB_NOTIFY_UNEXPECTED_BUS_SLAVES    (EC_IOCTL_SB |17)           /* 0x00050011 */
#define EC_IOCTL_SB_SET_RED_ENHANCED_LINE_CROSSED_DETECTION_ENABLED \
                                                    (EC_IOCTL_SB |18)           /* 0x00050012 */
#define EC_IOCTL_SB_SET_NOTIFY_NOT_CONNECTED_PORT_A (EC_IOCTL_SB |19)           /* 0x00050013 */
#define EC_IOCTL_SB_SET_NOTIFY_UNEXPECTED_CONNECTED_PORT \
                                                    (EC_IOCTL_SB |20)           /* 0x00050014 */
#define EC_IOCTL_SB_SET_JUNCTION_REDUNDANCY_ENABLED \
                                                    (EC_IOCTL_SB |21)           /* 0x00050015 */
#define EC_IOCTL_SB_GET_BUS_SLAVE_PORTS_INFO \
                                                    (EC_IOCTL_SB |22)           /* 0x00050016 */

    /* Hot Connect (HC) */
#define EC_IOCTL_HC_SETMODE                         (EC_IOCTL_HC | 1)
#define EC_IOCTL_HC_GETMODE                         (EC_IOCTL_HC | 2)
#define EC_IOCTL_HC_CONFIGURETIMEOUTS               (EC_IOCTL_HC | 3)

    /* private (PRIVATE) */
#define EC_IOCTL_SET_FRAME_LOSS_SIMULATION          (EC_IOCTL_PRIVATE | 1)
#define EC_IOCTL_SET_RXFRAME_LOSS_SIMULATION        (EC_IOCTL_PRIVATE | 2)
#define EC_IOCTL_SET_TXFRAME_LOSS_SIMULATION        (EC_IOCTL_PRIVATE | 3)
#define EC_IOCTL_GET_FAST_CONTEXT                   (EC_IOCTL_PRIVATE | 4)
#define EC_IOCTL_SET_OEM_KEY                        (EC_IOCTL_PRIVATE | 5)
#define EC_IOCTL_CHECK_OEM_KEY                      (EC_IOCTL_PRIVATE | 6)

/* EtherCat CoE entry description value information bit definitions */
#define EC_COE_ENTRY_ObjAccess                      0x01
#define EC_COE_ENTRY_ObjCategory                    0x02
#define EC_COE_ENTRY_PdoMapping                     0x04
#define EC_COE_ENTRY_UnitType                       0x08
#define EC_COE_ENTRY_DefaultValue                   0x10
#define EC_COE_ENTRY_MinValue                       0x20
#define EC_COE_ENTRY_MaxValue                       0x40

/* EtherCat CoE entry access bit definitions */
#define EC_COE_ENTRY_Access_R_PREOP                 0x01
#define EC_COE_ENTRY_Access_R_SAFEOP                0x02
#define EC_COE_ENTRY_Access_R_OP                    0x04
#define EC_COE_ENTRY_Access_W_PREOP                 0x08
#define EC_COE_ENTRY_Access_W_SAFEOP                0x10
#define EC_COE_ENTRY_Access_W_OP                    0x20

#define INVALID_SLAVE_ID                            ((EC_T_DWORD)0xFFFFFFFF)
#define INVALID_BUS_INDEX                           ((EC_T_DWORD)0xFFFFFFFF)
#define INVALID_FIXED_ADDR                          ((EC_T_WORD)0x0)
#define INVALID_AUTO_INC_ADDR                       ((EC_T_WORD)0x1)

#ifndef MAX_NUMOF_MASTER_INSTANCES
#define MAX_NUMOF_MASTER_INSTANCES                  12               /* maximum number of master instances */
#endif

#ifndef INSTANCE_MASTER_DEFAULT                     /* can be overriden in AtEmRasClnt.h */
#define INSTANCE_MASTER_DEFAULT                     ((EC_T_DWORD)0)  /* default master instance to be used by emXXXXX API to access same instance as ecatXXXXX API */
#endif

#if (defined INCLUDE_TRACE_DATA)
#define TRACE_DATA_CMD_ADO                          ((EC_T_WORD)0x4154)
#endif /* INCLUDE_TRACE_DATA */

/*-FORWARD DECLARATIONS------------------------------------------------------*/
struct _EC_T_LINK_PARMS;
struct _EC_T_NOTIFYPARMS;
struct _EC_T_OS_PARMS;

/*-TYPEDEFS/ENUMS------------------------------------------------------------*/

/* EtherCat callback function pointer */
typedef EC_T_DWORD (*EC_PF_NOTIFY)(                 EC_T_DWORD dwCode,
                                                    struct _EC_T_NOTIFYPARMS* pParms    );

/* EtherCAT ioctl parameters */
#include EC_PACKED_INCLUDESTART(8)
typedef struct _EC_T_IOCTLOPARMS
{
    EC_T_BYTE*      pbyInBuf;                       /*< [in] input data buffer */
    EC_T_DWORD      dwInBufSize;                    /*< [in] size of input data buffer in byte */
    EC_T_BYTE*      pbyOutBuf;                      /*< [out] output data buffer */
    EC_T_DWORD      dwOutBufSize;                   /*< [in] size of output data buffer in byte */
    EC_T_DWORD*     pdwNumOutData;                  /*< [out] number of output data bytes stored in output data buffer */
} EC_PACKED(8) EC_T_IOCTLPARMS;
#include EC_PACKED_INCLUDESTOP

/* Client register parameters */
typedef struct _EC_T_REGISTERPARMS
{
    EC_T_VOID*      pCallerData;                    /*< [in] used by all callback functions */
    EC_PF_NOTIFY    pfnNotify;                      /*< [in] notify callback function pointer */
}/*EC_PACKED */EC_T_REGISTERPARMS; /* Packed function pointer causes error with VxWorks DIAB compiler */

/* Client register result */
#include EC_PACKED_INCLUDESTART(1)
typedef struct _EC_T_REGISTERRESULTS
{
    EC_T_DWORD      dwClntId;                       /*< [out] Client ID */
    EC_T_BYTE*      pbyPDIn;                        /*< [out] Pointer to process data input memory */
    EC_T_DWORD      dwPDInSize;                     /*< [out] Size of process data input memory (in bytes) */
    EC_T_BYTE*      pbyPDOut;                       /*< [out] Pointer to process data output memory */
    EC_T_DWORD      dwPDOutSize;                    /*< [out] Size of process data output memory (in bytes) */
} EC_PACKED(1) EC_T_REGISTERRESULTS;
#include EC_PACKED_INCLUDESTOP

/* Msg logging */
typedef EC_T_BOOL (*EC_T_PFLOGMSG_CB)(const EC_T_CHAR* szFormat, EC_T_VALIST vaArgs);

/* Frame logging */
#define EC_LOG_FRAME_FLAG_MASTERSTATE_MASK 0xFFFF   /* Bit 0 to 15 */
#define EC_LOG_FRAME_FLAG_ACYC_FRAME       (1<<16)  /* Bit 16 (0x00010000): 0=cyclic frame, 1=acyclic frame */
#define EC_LOG_FRAME_FLAG_DBG_FRAME        (1<<17)  /* Bit 17 (0x00020000): 0=EtherCAT frame, 1=debug frame */
#define EC_LOG_FRAME_FLAG_RED_FRAME        (1<<18)  /* Bit 18 (0x00040000): 0=main frame, 1=red frame */
#define EC_LOG_FRAME_FLAG_RX_FRAME         (1<<19)  /* Bit 19 (0x00080000): 0=TX frame, 1=RX frame */
typedef EC_T_VOID (*EC_T_PFLOGFRAME_CB)(EC_T_VOID* pvContext, EC_T_DWORD dwLogFlags, EC_T_DWORD dwFrameSize, EC_T_BYTE* pbyFrame);

/* Master initialization parameters, see ecatInitMaster */
#include EC_PACKED_INCLUDESTART(8)
typedef struct _EC_T_INIT_MASTER_PARMS
{
    EC_T_DWORD  dwSignature;                        /*< [in] set to ATECAT_SIGNATURE */
    EC_T_DWORD  dwSize;                             /*< [in] set to sizeof(EC_T_INIT_MASTER_PARMS) */

    struct _EC_T_OS_PARMS*      pOsParms;           /*< [in] OS layer parameters */
    struct _EC_T_LINK_PARMS*    pLinkParms;         /*< [in] Link layer parameters */
    struct _EC_T_LINK_PARMS*    pLinkParmsRed;      /*< [in] Link layer parameters for red device */

    EC_T_DWORD  dwBusCycleTimeUsec;                 /*< [in] [usec] bus cycle time in microseconds */

    /* memory */
    EC_T_DWORD  dwMaxBusSlaves;                     /*< [in] maximum pre-allocated bus slave objects */
    EC_T_DWORD  dwMaxQueuedEthFrames;               /*< [in] maximum queued Ethernet frames */
    EC_T_DWORD  dwAdditionalEoEEndpoints;           /*< [in] additional EoE endpoints */

    /* bus load */
    EC_T_DWORD  dwMaxSlaveCmdPerFrame;              /*< [in] maximum slave commands per frame */
    EC_T_DWORD  dwMaxSentQueuedFramesPerCycle;      /*< [in] maximum queued frames sent with eUsrJob_SendAcycFrames within one cycle */

    /* CPU load */
    EC_T_DWORD  dwMaxSlavesProcessedPerCycle;       /*< [in] maximum slave-related state machine calls per cycle */

    /* retry and timeouts */
    EC_T_DWORD  dwEcatCmdMaxRetries;                /*< [in] maximum retries to send pending ethercat command frames */
    EC_T_DWORD  dwEcatCmdTimeout;                   /*< [in] timeout to send pending ethercat command frames */
    EC_T_DWORD  dwEoETimeout;                       /*< [in] timeout sending EoE frames */
    EC_T_DWORD  dwFoEBusyTimeout;                   /*< [in] obsolete */

    /* VLAN */
    EC_T_BOOL   bVLANEnable;                        /*< [in] E=enable (1/0) */
    EC_T_WORD   wVLANId;                            /*< [in] I=VLAN Id (12Bit)*/
    EC_T_BYTE   byVLANPrio;                         /*< [in] P=Prio (3Bit) */

    /* logging */
    EC_T_DWORD  dwLogLevel;                         /*< [in] log level. See EC_LOG_LEVEL_... */
    EC_T_PFLOGMSG_CB    pfLogMsgCallBack;           /*< [in] optional call back function to log msg from the EC-Master. set to EC_NULL if not used. */
    EC_T_BOOL   bLogToLinkLayer;                    /*< [in] EC_TRUE: error messages are sent to the link layer */

    EC_T_DWORD  dwReserved[4];

} EC_PACKED(8) EC_T_INIT_MASTER_PARMS;
#include EC_PACKED_INCLUDESTOP

/* link layer debug message */
#include EC_PACKED_INCLUDESTART(1)
typedef struct _EC_T_LINKLAYER_DBG_MSG_DESC
{
    EC_T_BYTE   byEthTypeByte0;                     /*< [in] Ethernet type byte 0 */
    EC_T_BYTE   byEthTypeByte1;                     /*< [in] Ethernet type byte 0 */
    EC_T_WORD   wRes;
    EC_T_CHAR*  szMsg;                              /*< [in] message to send to link layer */
} EC_PACKED(1) EC_T_LINKLAYER_DBG_MSG_DESC;
#include EC_PACKED_INCLUDESTOP

/* Distributed clocks */
#if (defined INCLUDE_DC_SUPPORT)
#include EC_PACKED_INCLUDESTART(8)
typedef struct _EC_T_DC_SYNCSO_CB_PARM
{
    EC_T_VOID*              pvContext;              /*< [in] Calling environment context */
    EC_T_DWORD              dwTimeStamp;            /*< [in] Pre-Send Timestamp */
    EC_T_DWORD              dwPostTimeStamp;        /*< [in] Post-Send Timestamp */
    EC_T_DWORD              dwBusTimeLo;            /*< [in] Bus Time Lower 32 Bit */
    EC_T_DWORD              dwBusTimeHi;            /*< [in] Bus Time Upper 32 Bit */
    EC_T_DWORD              dwTimeStampResult;      /*< [in] Result of stamp */

    EC_T_DWORD	            dwSyncPeriodLength;     /*< [in] Time between two consecutive SYNC0 signals of clock master slave in nsec. */

    EC_T_DWORD              dwStartSyncTimeLo;      /*< [out] Sync Start Time Lower 32 Bit */
    EC_T_DWORD              dwStartSyncTimeHi;      /*< [out] Sync Start Time Upper 32 Bit */

} EC_PACKED(8)  EC_T_DC_SYNCSO_CB_PARM, *EC_PT_DC_SYNCSO_CB_PARM;
#include EC_PACKED_INCLUDESTOP

typedef EC_T_DWORD (*EC_T_PFSYNCSO_CB)(EC_T_DC_SYNCSO_CB_PARM* pParm);

#include EC_PACKED_INCLUDESTART(8)
typedef struct _EC_T_DC_SYNCSO_REGDESC
{
    EC_T_DC_SYNCSO_CB_PARM*     pCallbackParm;      /*< [in] Callback Parameter */
    EC_T_PFSYNCSO_CB            pfnCallback;        /*< [in] Callback Function */
} EC_PACKED(8)  EC_T_DC_SYNCSO_REGDESC, *EC_PT_DC_SYNCSO_REGDESC;
#include EC_PACKED_INCLUDESTOP

#include EC_PACKED_INCLUDESTART(8)
typedef struct _EC_T_DC_CONFIGURE
{
    EC_T_DWORD                  dwClntId;                   /*< [in] Reserved */
    EC_T_DWORD                  dwTimeout;                  /*< [in] DC Timeout value in msec */
    EC_T_DWORD                  dwDevLimit;                 /*< [in] deviation limit value */
    EC_T_DWORD                  dwSettleTime;               /*< [in] config values for settle time */
    EC_T_DWORD                  dwTotalBurstLength;         /*< [in] Total size of the initialization burst */
    EC_T_DWORD                  dwBurstBulk;                /*< [in] Amount of burst frames per cycle during initialization burst */
    EC_T_BOOL                   bBulkInLinkLayer;           /*< [in] If EC_TRUE, bulk is realized by link layer (link layer must support it), otherwise by master */
    EC_T_BOOL                   bAcycDistributionDisabled;  /*< [in] If EC_TRUE, acyclic distribution is disabled */
    EC_T_DWORD                  dwDcStartTimeGrid;          /*< [in] Time grid in nsec to align DC start time */
    EC_T_DWORD                  dwReserved[2];              /*< [in/out] reserved */
} EC_PACKED(8)  EC_T_DC_CONFIGURE, *EC_PT_DC_CONFIGURE;
#include EC_PACKED_INCLUDESTOP

typedef enum _EC_T_DCM_MODE
{
    eDcmMode_Off               = 0,
    eDcmMode_BusShift          = 1,
    eDcmMode_MasterShift       = 2,
    eDcmMode_LinkLayerRefClock = 3,
    eDcmMode_MasterRefClock    = 4,
    eDcmMode_Dcx               = 5,

    /* Borland C++ datatype alignment correction */
    eDcmMode_BCppDummy      = 0xFFFFFFFF
} EC_T_DCM_MODE;

#include EC_PACKED_INCLUDESTART(8)
typedef struct _EC_T_DCM_CONFIG_BUSSHIFT
{
    EC_T_INT                    nCtlSetVal;         /*< [in] controller set val in nsec */
    EC_T_INT                    nCtlGain;           /*< [in] controller gain */
    EC_T_INT                    nCtlDriftErrorGain; /*< [in] controller drift error gain */
    EC_T_INT                    nMaxValidVal;       /*< [in] max valid input value */
    EC_T_BOOL                   bLogEnabled;        /*< [in] EC_TRUE if logging information should be generated */
    EC_T_DWORD                  dwInSyncLimit;      /*< [in] limit in nsec for InSync monitoring */
    EC_T_DWORD                  dwInSyncSettleTime; /*< [in] settle time in msec for InSync monitoring */
    EC_T_BOOL                   bCtlOff;            /*< [in] EC_TRUE if controller should not adjust the reference clock (for diagnostic only) */
} EC_PACKED(8)  EC_T_DCM_CONFIG_BUSSHIFT, *EC_PT_DCM_CONFIG_BUSSHIFT;
#include EC_PACKED_INCLUDESTOP

#include EC_PACKED_INCLUDESTART(8)
typedef struct _EC_T_DCM_CONFIG_MASTERSHIFT
{
    EC_T_INT                    nCtlSetVal;         /*< [in] controller set val in nsec */
    EC_T_INT                    nCtlGain;           /*< [in] controller gain */
    EC_T_INT                    nCtlDriftErrorGain; /*< [in] controller drift error gain */
    EC_T_INT                    nMaxValidVal;       /*< [in] max valid input value */
    EC_T_BOOL                   bLogEnabled;        /*< [in] EC_TRUE if logging information should be generated */
    EC_T_DWORD                  dwInSyncLimit;      /*< [in] limit in nsec for InSync monitoring */
    EC_T_DWORD                  dwInSyncSettleTime; /*< [in] settle time in msec for InSync monitoring */
    EC_T_BOOL                   bCtlOff;            /*< [in] EC_TRUE if controller should not adjust the reference clock (for diagnostic, or self adjust using emDcmGetAdjust()) */
} EC_PACKED(8)  EC_T_DCM_CONFIG_MASTERSHIFT, *EC_PT_DCM_CONFIG_MASTERSHIFT;
#include EC_PACKED_INCLUDESTOP

#include EC_PACKED_INCLUDESTART(8)
typedef struct _EC_T_DCM_CONFIG_LINKLAYERREFCLOCK
{
    EC_T_DWORD                  dwUnused;
} EC_PACKED(8)  EC_T_DCM_CONFIG_LINKLAYERREFCLOCK, *EC_PT_DCM_CONFIG_LINKLAYERREFCLOCK;
#include EC_PACKED_INCLUDESTOP

#include EC_PACKED_INCLUDESTART(8)
typedef struct _EC_T_DCM_CONFIG_MASTERREFCLOCK
{
    EC_T_INT                    nCtlSetVal;         /*< [in] controller set val in nsec */
    EC_T_BOOL                   bLogEnabled;        /*< [in] EC_TRUE if logging information should be generated */
    EC_T_DWORD                  dwInSyncLimit;      /*< [in] limit in nsec for InSync monitoring */
    EC_T_DWORD                  dwInSyncSettleTime; /*< [in] settle time in msec for InSync monitoring */
} EC_PACKED(8)  EC_T_DCM_CONFIG_MASTERREFCLOCK, *EC_PT_DCM_CONFIG_MASTERREFCLOCK;
#include EC_PACKED_INCLUDESTOP

#include EC_PACKED_INCLUDESTART(8)
typedef struct _EC_T_DCM_CONFIG_DCX
{
    EC_T_DCM_CONFIG_MASTERSHIFT MasterShift;
    EC_T_INT                    nCtlSetVal;             /*< [in] controller set val in nsec */
    EC_T_INT                    nCtlGain;               /*< [in] controller gain */
    EC_T_INT                    nCtlDriftErrorGain;     /*< [in] controller drift error gain */
    EC_T_INT                    nMaxValidVal;           /*< [in] max valid input value */
    EC_T_BOOL                   bLogEnabled;            /*< [in] EC_TRUE if logging information should be generated */
    EC_T_DWORD                  dwInSyncLimit;          /*< [in] limit in nsec for InSync monitoring */
    EC_T_DWORD                  dwInSyncSettleTime;     /*< [in] settle time in msec for InSync monitoring */
    EC_T_BOOL                   bCtlOff;                /*< [in] EC_TRUE if controller should not adjust the reference clock (for diagnostic, or self adjust using emDcmGetAdjust()) */
    EC_T_WORD                   wExtClockFixedAddr;     /*< [in] Fixed address of external clock slave (publishing PDO 0x10F4) (optional if ENI is generated by EcEngineer) */
    EC_T_DWORD                  dwExtClockTimeout;      /*< [in] Wait timeout for external clock slave */
} EC_PACKED(8)  EC_T_DCM_CONFIG_DCX, *EC_PT_DCM_CONFIG_DCX;
#include EC_PACKED_INCLUDESTOP

#include EC_PACKED_INCLUDESTART(8)
typedef struct _EC_T_DCM_CONFIG
{
    EC_T_DCM_MODE               eMode;
    union {
    EC_T_DCM_CONFIG_BUSSHIFT          BusShift;
    EC_T_DCM_CONFIG_MASTERSHIFT       MasterShift;
    EC_T_DCM_CONFIG_LINKLAYERREFCLOCK LinkLayerRefClock;
    EC_T_DCM_CONFIG_MASTERREFCLOCK    MasterRefClock;
    EC_T_DCM_CONFIG_DCX               Dcx;
    EC_T_DWORD                        adwReserved[32];
    } EC_PACKED(8) u;
} EC_PACKED(8)  EC_T_DCM_CONFIG, *EC_PT_DCM_CONFIG;
#include EC_PACKED_INCLUDESTOP

#include EC_PACKED_INCLUDESTART(8)
typedef struct _EC_T_DCM_LOG
{
    EC_T_DWORD                  dwMsecCounter;
    EC_T_INT                    nCtlSetVal;
    EC_T_DWORD                  dwBusTime;
    EC_T_INT                    nCtlErrorNsec;
    EC_T_INT                    nDrift;
    EC_T_DWORD                  dwErrorCode;
    EC_T_BOOL                   bDcmInSync;
    EC_T_BOOL                   bDcInSync;
    EC_T_INT                    nSystemTimeDifference;
} EC_PACKED(8)  EC_T_DCM_LOG, *EC_PT_DCM_LOG;
#include EC_PACKED_INCLUDESTOP
#else
struct _EC_T_DC_CONFIGURE;
struct _EC_T_DCM_CONFIG;
#endif /* INCLUDE_DC_SUPPORT */

/* ecatGetSlaveInfo */
#include EC_PACKED_INCLUDESTART(1)
typedef struct _EC_T_GET_SLAVE_INFO
{
    EC_T_DWORD                  dwScanBusStatus;    /* 0x00 */  /*< Status during last Bus Scan */

    EC_T_DWORD                  dwVendorId;         /* 0x01 */  /*< Vendor Identification */
    EC_T_DWORD                  dwProductCode;      /* 0x02 */  /*< Product Code */
    EC_T_DWORD                  dwRevisionNumber;   /* 0x03 */  /*< Revision Number */
    EC_T_DWORD                  dwSerialNumber;     /* 0x04 */  /*< Serial Number */

    EC_T_WORD                   wPortState;         /* 0x05 */  /*< [out] port link state (SB Instance)*/
    EC_T_WORD                   wReserved;                      /*< Res */

    EC_T_BOOL                   bDcSupport;         /* 0x06 */  /*< [out] slave does support DC*/
    EC_T_BOOL                   bDc64Support;       /* 0x07 */  /*< [out] slave does support 64Bit DC*/

    EC_T_WORD                   wAliasAddress;      /* 0x08 */  /*< [out] slave alias address*/
    EC_T_WORD                   wPhysAddress;                   /*< [out] slave station address*/

    EC_T_DWORD                  dwPdOffsIn;         /* 0x09 */  /*< [out] process data offset of Input Data (in bits)*/
    EC_T_DWORD                  dwPdSizeIn;         /* 0x0A */  /*< [out] process data size of Input Data (in bits)*/
    EC_T_DWORD                  dwPdOffsOut;        /* 0x0B */  /*< [out] process data offset of Output Data (in bits)*/
    EC_T_DWORD                  dwPdSizeOut;        /* 0x0C */  /*< [out] process data size of Output Data*/
    EC_T_DWORD                  dwPdOffsIn2;        /* 0x0D */  /*< [out] process data offset of Input data (in bits)*/
    EC_T_DWORD                  dwPdSizeIn2;        /* 0x0E */  /*< [out] process data size of Input Data (in bits)*/
    EC_T_DWORD                  dwPdOffsOut2;       /* 0x0F */  /*< [out] process data offset of Output Data (in bits)*/
    EC_T_DWORD                  dwPdSizeOut2;       /* 0x10 */  /*< [out] process data size of Output Data*/
    EC_T_DWORD                  dwPdOffsIn3;        /* 0x11 */  /*< [out] process data offset of Input Data (in bits)*/
    EC_T_DWORD                  dwPdSizeIn3;        /* 0x12 */  /*< [out] process data size of Input Data (in bits)*/
    EC_T_DWORD                  dwPdOffsOut3;       /* 0x13 */  /*< [out] process data offset of Output Data (in bits)*/
    EC_T_DWORD                  dwPdSizeOut3;       /* 0x14 */  /*< [out] process data size of Output Data*/
    EC_T_DWORD                  dwPdOffsIn4;        /* 0x15 */  /*< [out] process data offset of Input Data (in bits)*/
    EC_T_DWORD                  dwPdSizeIn4;        /* 0x16 */  /*< [out] process data size of Input Data (in bits)*/
    EC_T_DWORD                  dwPdOffsOut4;       /* 0x17 */  /*< [out] process data offset of Output Data (in bits)*/
    EC_T_DWORD                  dwPdSizeOut4;       /* 0x18 */  /*< [out] process data size of Output Data*/

    EC_T_WORD                   wCfgPhyAddress;     /* 0x19 */  /*< [out] slave configured station address*/
    EC_T_WORD                   wReserved2;                     /*< reserved */

    EC_T_CHAR                   abyDeviceName[ECAT_DEVICE_NAMESIZE];
                                                    /* 0x1A */  /*< [out] slave name of configuration*/
    EC_T_BOOL                   bIsMailboxSlave;    /* 0x2E */  /*< [out] whether slave support mailboxes*/
    EC_T_DWORD                  dwMbxOutSize;       /* 0x2F */  /*< [out] mailbox 1 output size*/
    EC_T_DWORD                  dwMbxInSize;        /* 0x30 */  /*< [out] mailbox 1 input size*/
    EC_T_DWORD                  dwMbxOutSize2;      /* 0x31 */  /*< [out] mailbox 2 output size*/
    EC_T_DWORD                  dwMbxInSize2;       /* 0x32 */  /*< [out] mailbox 2 input size*/

    EC_T_DWORD                  dwErrorCode;        /* 0x33 */  /*< [out] last return code*/
    EC_T_DWORD                  dwSBErrorCode;      /* 0x34 */  /*< [out] last return value from SB*/

    EC_T_BYTE                   byPortDescriptor;   /* 0x35 */  /*< [out] Port Descriptor (ESC register 0x0007) */
    EC_T_BYTE                   byESCType;                      /*< [out] ESC Node Type */
    EC_T_WORD                   wSupportedMbxProtocols;         /*< [out] supported mailbox protocols: AoE, EoE, CoE, FoE, SoE */

    EC_T_WORD                   wAlStatusValue;     /* 0x36 */  /*< [out] AL Status Register Value (ESC register 0x0130) */
    EC_T_WORD                   wAlStatusCode;                  /*< [out] AL Status Code (ESC register 0x0134) */

    EC_T_BOOL                   bIsOptional;        /* 0x37 */  /*< [out] slave is in an optional hot connect group */
    EC_T_BOOL                   bIsPresent;         /* 0x38 */  /*< [out] slave is currently present on bus */

    EC_T_WORD                   wNumProcessVarsInp; /* 0x39 */  /*< [out] number of output process data variables*/
    EC_T_WORD                   wNumProcessVarsOutp;            /*< [out] number of input process data variables */

    EC_T_DWORD                  dwSlaveId;          /* 0x3A */  /*< [out] slave ID */
    EC_T_BOOL                   bIsHCGroupPresent;  /* 0x3B */  /*< [out] the hot connect group of the slave is present */

    EC_T_DWORD                  aPortSlaveIds[ESC_PORT_COUNT];  /* 0x3C */  /*< [out] slave IDs connected to ports */

    EC_T_DWORD                  dwSystemTimeDifference;         /* 0x40   < out System time difference (ESC register 0x092C) */

    EC_T_DWORD                  adwReserved[18];    /* 0x41 */  /*< [out] Reserved*/

} EC_PACKED(1)  EC_T_GET_SLAVE_INFO, *EC_PT_GET_SLAVE_INFO; /* Size = 0x53 * DWORD */
#include EC_PACKED_INCLUDESTOP

/* emGetSlaveInpVarInfo / emGetSlaveOutpVarInfo*/
#include EC_PACKED_INCLUDESTART(1)
typedef struct _EC_T_PROCESS_VAR_INFO
{
    EC_T_CHAR                   szName[MAX_PROCESS_VAR_NAME_LEN];   /**< [out] Name of the found process variable */
    EC_T_WORD                   wDataType;                          /**< [out] Data type of the found process variable */
    EC_T_WORD                   wFixedAddr;                         /**< [out] Station address of the slave that is owner of this variable */
    EC_T_INT                    nBitSize;                           /**< [out] Size in bit of the found process variable */
    EC_T_INT                    nBitOffs;                           /**< [out] Bit offset in the process data image */
    EC_T_BOOL                   bIsInputData;                       /**< [out] Determines whether the found process variable is an input variable or an output variable */
} EC_PACKED(1) EC_T_PROCESS_VAR_INFO, *EC_PT_PROCESS_VAR_INFO;
#include EC_PACKED_INCLUDESTOP

#include EC_PACKED_INCLUDESTART(1)
typedef struct _EC_T_PROCESS_VAR_INFO_EX
{
    EC_T_CHAR                   szName[MAX_PROCESS_VAR_NAME_LEN_EX]; /**< [out] Name of the found process variable */
    EC_T_WORD                   wDataType;                          /**< [out] Data type of the found process variable */
    EC_T_WORD                   wFixedAddr;                         /**< [out] Station address of the slave that is owner of this variable */
    EC_T_INT                    nBitSize;                           /**< [out] Size in bit of the found process variable */
    EC_T_INT                    nBitOffs;                           /**< [out] Bit offset in the process data image */
    EC_T_BOOL                   bIsInputData;                       /**< [out] Determines whether the found process variable is an input variable or an output variable */
    EC_T_WORD                   wIndex;                             /**< [out] object index */
    EC_T_WORD                   wSubIndex;                          /**< [out] object subindex */
    EC_T_WORD                   wPdoIndex;                          /**< [out] PDO Index */
    EC_T_WORD                   wWkcStateDiagOffs;                  /**< [out] Bit offset in the diagnostic image (ecatGetDiagnosisImagePtr()) */
    EC_T_DWORD                  dwRes1;
    EC_T_DWORD                  dwRes2;
} EC_PACKED(1) EC_T_PROCESS_VAR_INFO_EX, *EC_PT_PROCESS_VAR_INFO_EX;
#include EC_PACKED_INCLUDESTOP

/* EtherCAT CoE OD list type values */
typedef enum _EC_T_COE_ODLIST_TYPE
{
    eODListType_Lengths     = 0,                    /*< lengths of each list type */
    eODListType_ALL         = 1,                    /*< all objects */
    eODListType_RxPdoMap    = 2,                    /*< only pdo mappable objects */
    eODListType_TxPdoMap    = 3,                    /*< only pdo mappable objects that can be changed */
    eODListType_StoredFRepl = 4,                    /*< only stored for a device replacement objects */
    eODListType_StartupParm = 5,                    /*< only startup parameter objects */

    /* Borland C++ datatype alignment correction */
    eODListType_BCppDummy   = 0xFFFFFFFF
} EC_T_COE_ODLIST_TYPE;
#define CoeOdListTypeText(EType)                                \
    ((EType)==eODListType_Lengths?"Lengths":                    \
     ((EType)==eODListType_ALL?"All":                           \
      ((EType)==eODListType_RxPdoMap?"RxPDO":                   \
       ((EType)==eODListType_TxPdoMap?"TxPDO":                  \
        ((EType)==eODListType_StoredFRepl?"Device Replacement": \
         ((EType)==eODListType_StartupParm?"Startup Param":     \
         "Unknown"                                              \
    ))))))


/* EtherCAT CoE OD list */
#include EC_PACKED_INCLUDESTART(1)
typedef struct _EC_T_COE_ODLIST
{
    EC_T_COE_ODLIST_TYPE    eOdListType;            /*< type of OD list */
    EC_T_WORD               wLen;                   /*< length of the OD list */
    EC_T_WORD               wRes;
    EC_T_WORD*              pwOdList;               /*< OD list */
} EC_PACKED(1) EC_T_COE_ODLIST;
#include EC_PACKED_INCLUDESTOP

/* EtherCAT CoE Object description */
#include EC_PACKED_INCLUDESTART(1)
typedef struct _EC_T_COE_OBDESC
{
    EC_T_WORD   wObIndex;                           /*< object index */
    EC_T_WORD   wDataType;                          /*< see DEFTYPE_..., refer to "Extended Data Type Area" in ETG.1000.6 */
    EC_T_BYTE   byObjCode;                          /*< see ECAT_COE_INFO_OBJCODE_..., defined in CiA DS 301, "Object code usage" */
    EC_T_BYTE   byObjCategory;                      /*< category: 0=optional, 1=mandatory */
    EC_T_BYTE   byMaxNumSubIndex;                   /*< maximum number of sub-indexes */
    EC_T_BYTE   byReserve;
    EC_T_WORD   wObNameLen;                         /*< length of the object name */
    EC_T_WORD   wRes;
    EC_T_CHAR*  pchObName;                          /*< object name (not a NULL terminated string!) */
} EC_PACKED(1) EC_T_COE_OBDESC;
#include EC_PACKED_INCLUDESTOP

/* EtherCAT CoE Object Entry description */
#include EC_PACKED_INCLUDESTART(1)
typedef struct _EC_T_COE_ENTRYDESC
{
    EC_T_WORD   wObIndex;                           /*< object index */
    EC_T_BYTE   byObSubIndex;                       /*< object sub-index */
    EC_T_BYTE   byValueInfo;                        /*< value information, determines which of the following elements are valid */
    EC_T_WORD   wDataType;                          /*< data type */
    EC_T_WORD   wBitLen;                            /*< bit length of the object */
    EC_T_BYTE   byObAccess;                         /*< object access */
    EC_T_BYTE   byReserved[3];
    EC_T_BOOL   bRxPdoMapping;                      /*< EC_TRUE if it is PDO mappable */
    EC_T_BOOL   bTxPdoMapping;                      /*< EC_TRUE if PDO mappable object can be changed */
    EC_T_BOOL   bObCanBeUsedForBackup;              /*< Parameter may be back upped */
    EC_T_BOOL   bObCanBeUsedForSettings;            /*< Parameter may be used for Settings */
    EC_T_WORD   wReserved;                          /*< unit type of the object, only valid if EC_COE_ENTRY_UnitType bit is set in byValueInfo */
    EC_T_WORD   wDataLen;                           /*< length of the data */
    EC_T_BYTE*  pbyData;                            /*< rest of the entry description, content see below */
    /*EC_T_DWORD  dwUnitType;                        *< unit type of the object, only valid if EC_COE_ENTRY_UnitType bit is set in byValueInfo */
    /*EC_T_BYTE   pbyDefaultValue[];                 *< default value, only valid if EC_COE_ENTRY_DefaultValue bit is set in byValueInfo */
    /*EC_T_BYTE   pbyMinValue[];                     *< minimum value, only valid if EC_COE_ENTRY_MinValue bit is set in byValueInfo */
    /*EC_T_BYTE   pbyMaxValue[];                     *< maximum value, only valid if EC_COE_ENTRY_MaxValue bit is set in byValueInfo */
    /*EC_T_BYTE   pbyDescription[];                  *< object description */
} EC_PACKED(1) EC_T_COE_ENTRYDESC;
#include EC_PACKED_INCLUDESTOP

/* EtherCAT CoE emergency request */
#define EC_COE_EMERGENCY_DATASIZE 5
#include EC_PACKED_INCLUDESTART(1)
typedef struct _EC_T_COE_EMERGENCY
{
    EC_T_WORD   wErrorCode;                             /*< error code, see specification for details */
    EC_T_BYTE   byErrorRegister;                        /*< error register */
    EC_T_BYTE   abyData[EC_COE_EMERGENCY_DATASIZE];     /*< error data */
    EC_T_WORD   wStationAddress;                        /*< which slave caused the emergency request */
} EC_PACKED(1) EC_T_COE_EMERGENCY;
#include EC_PACKED_INCLUDESTOP

#include EC_PACKED_INCLUDESTART(1)
typedef struct _EC_T_MBX_DATA_COE_INITCMD
{
    EC_T_SLAVE_PROP SlaveProp;                          /*< Slave properties */
    EC_T_DWORD      dwHandle;                           /*< Handle passed by EC_IOCTL_ADD_COE_INITCMD, otherwise zero */
    EC_T_WORD       wTransition;                        /*< Transition, e.g. ECAT_INITCMD_I_P */
    EC_T_CHAR       szComment[MAX_STD_STRLEN];          /*< Comment (ENI) */
    EC_T_DWORD      dwErrorCode;                        /*< InitCmd result */
    EC_T_BOOL       bFixed;                             /*< Fixed flag (ENI) */
    EC_T_BYTE       byCcs;                              /*< Client command specifier (read or write access) */
    EC_T_BOOL       bCompleteAccess;                    /*< Complete access */
    EC_T_WORD       wIndex;                             /*< Object Index */
    EC_T_BYTE       bySubIndex;                         /*< Object SubIndex */
    EC_T_DWORD      dwDataLen;                          /*< InitCmd data length */
    EC_T_BYTE*      pbyData;                            /*< InitCmd data */
} EC_PACKED(1) EC_T_MBX_DATA_COE_INITCMD;
#include EC_PACKED_INCLUDESTOP

#include EC_PACKED_INCLUDESTART(8)
typedef struct _EC_T_MBX_DATA_COE
{
    EC_T_WORD       wStationAddress;
    EC_T_WORD       wIndex;                             /*< object index */
    EC_T_BYTE       bySubIndex;                         /*< object subindex */
    EC_T_BOOL       bCompleteAccess;                    /*< complete access */
} EC_PACKED(8)  EC_T_MBX_DATA_COE;
#include EC_PACKED_INCLUDESTOP

#if (defined INCLUDE_FOE_SUPPORT)
#include EC_PACKED_INCLUDESTART(1)
typedef struct _EC_T_MBX_DATA_FOE
{
    EC_T_DWORD  dwTransferredBytes;                         /*< eMbxTferType_FOE_FILE_DOWNLOAD, eMbxTferType_FOE_SEG_DOWNLOAD: amount of transferred bytes */
    EC_T_DWORD  dwRequestedBytes;                           /*< eMbxTferType_FOE_SEG_DOWNLOAD: amount of bytes to be provided by application */

    EC_T_DWORD  dwBusyDone;                                 /*< If slave is busy: 0 ... dwBusyEntire */
    EC_T_DWORD  dwBusyEntire;                               /*< If dwBusyEntire > 0: Slave is busy */
#define EC_FOE_BUSY_COMMENT_SIZE    32
    EC_T_CHAR   szBusyComment[EC_FOE_BUSY_COMMENT_SIZE];    /*< Busy Comment from slave */
    EC_T_DWORD  dwFileSize;                                 /*< File size */
} EC_PACKED(1) EC_T_MBX_DATA_FOE;
#include EC_PACKED_INCLUDESTOP
#endif /* INCLUDE_FOE_SUPPORT */

#if (defined INCLUDE_SOE_SUPPORT)
#include EC_PACKED_INCLUDESTART(1)
typedef struct _EC_T_MBX_DATA_SOE
{
    EC_T_BYTE   byElementFlags;
} EC_PACKED(1) EC_T_MBX_DATA_SOE;
#include EC_PACKED_INCLUDESTOP

/* EtherCAT SoE notification */
#define EC_SOE_NOTIFICATION_DATASIZE 5
#include EC_PACKED_INCLUDESTART(1)
typedef struct _EC_T_SOE_NOTIFICATION
{
    EC_T_WORD   wHeader;                                /*< SoE Header */
    EC_T_WORD   wIdn;                                   /*< IDN number of the appendent call */
    EC_T_BYTE   abyData[EC_SOE_NOTIFICATION_DATASIZE];  /*< error data */
    EC_T_WORD   wStationAddress;                        /*< which slave caused the emergency request */
} EC_PACKED(1) EC_T_SOE_NOTIFICATION;
#include EC_PACKED_INCLUDESTOP

/* EtherCAT SoE emergency request */
#define EC_SOE_EMERGENCY_DATASIZE 5
#include EC_PACKED_INCLUDESTART(1)
typedef struct _EC_T_SOE_EMERGENCY
{
    EC_T_WORD   wHeader;                                /*< SoE Header */
    EC_T_BYTE   abyData[EC_SOE_EMERGENCY_DATASIZE];     /*< error data */
    EC_T_WORD   wStationAddress;                        /*< which slave caused the emergency request */
} EC_PACKED(1) EC_T_SOE_EMERGENCY;
#include EC_PACKED_INCLUDESTOP
#endif /* INCLUDE_SOE_SUPPORT */

/* AoE NetID */
#include EC_PACKED_INCLUDESTART(1)
typedef struct _EC_T_AOE_NETID
{
    EC_T_BYTE   aby[6];
} EC_PACKED(1) EC_T_AOE_NETID;
#define EC_T_AOE_NETID_SIZE         (6)
#include EC_PACKED_INCLUDESTOP

/* AoE mailbox response error codes */
#include EC_PACKED_INCLUDESTART(1)
typedef struct _EC_T_AOE_RESPONSE_CODES
{
    EC_T_DWORD   dwErrorCode;
    EC_T_DWORD   dwCmdResult;
    EC_T_DWORD   dwRsvd;
} EC_PACKED(1) EC_T_AOE_CMD_RESPONSE;
#include EC_PACKED_INCLUDESTOP

/* EtherCAT mailbox data */
#include EC_PACKED_INCLUDESTART(1)
typedef union _EC_T_MBX_DATA
{
#if (defined INCLUDE_AOE_SUPPORT)
    EC_T_AOE_CMD_RESPONSE   AoE_Response;               /*< AoE response information */
#endif
    EC_T_MBX_DATA_COE       CoE;
    EC_T_COE_ODLIST         CoE_ODList;                 /*< CoE object dictionary list */
    EC_T_COE_OBDESC         CoE_ObDesc;                 /*< CoE object description */
    EC_T_COE_ENTRYDESC      CoE_EntryDesc;              /*< CoE object entry description */
    EC_T_COE_EMERGENCY      CoE_Emergency;              /*< CoE emergency request */
    EC_T_MBX_DATA_COE_INITCMD CoE_InitCmd;              /*< CoE InitCmd */
#if (defined INCLUDE_FOE_SUPPORT)
    EC_T_DWORD              dwFoETransferredBytes;      /*< eMbxTferType_FOE_FILE_DOWNLOAD: FoE number of transferred bytes. Obsolete. Use FoE.dwTransferredBytes instead */
    EC_T_MBX_DATA_FOE       FoE;                        /*< FoE transfer data, e.g. progress information */
#endif
#if (defined INCLUDE_SOE_SUPPORT)
    EC_T_MBX_DATA_SOE       SoE;
    EC_T_SOE_NOTIFICATION   SoE_Notification;           /*< SoE notification request */
    EC_T_SOE_EMERGENCY      SoE_Emergency;              /*< SoE emergency request */
#endif
} EC_PACKED(1) EC_T_MBX_DATA;
#include EC_PACKED_INCLUDESTOP

/* EtherCAT mailbox type values */
typedef enum _EC_T_MBXTFER_TYPE
{
    eMbxTferType_COE_SDO_DOWNLOAD   = 0,                /*< CoE SDO download                    */
    eMbxTferType_COE_SDO_UPLOAD     = 1,                /*< CoE SDO upload                      */
    eMbxTferType_COE_GETODLIST      = 2,                /*< CoE Get object dictionary list      */
    eMbxTferType_COE_GETOBDESC      = 3,                /*< CoE Get object description          */
    eMbxTferType_COE_GETENTRYDESC   = 4,                /*< CoE Get object entry description    */
    eMbxTferType_COE_EMERGENCY      = 5,                /*< CoE emergency request               */
#if (defined INCLUDE_COE_PDO_SUPPORT)
    eMbxTferType_COE_RX_PDO         = 6,                /*< CoE RxPDO                           */
#endif
    eMbxTferType_FOE_FILE_UPLOAD    = 7,                /*< FoE upload                          */
    eMbxTferType_FOE_FILE_DOWNLOAD  = 8,                /*< FoE download                        */
#if (defined INCLUDE_SOE_SUPPORT)
    eMbxTferType_SOE_READREQUEST    = 9,                /*< SoE read request                    */
    eMbxTferType_SOE_READRESPONSE   = 10,               /*< SoE read response                   */
    eMbxTferType_SOE_WRITEREQUEST   = 11,               /*< SoE write request                   */
    eMbxTferType_SOE_WRITERESPONSE  = 12,               /*< SoE write response                  */
    eMbxTferType_SOE_NOTIFICATION   = 13,               /*< SoE notification                    */
    eMbxTferType_SOE_EMERGENCY      = 14,               /*< SoE emergency                       */
#endif
#if (defined INCLUDE_VOE_SUPPORT)
    eMbxTferType_VOE_MBX_READ       = 15,               /*< VoE read                            */
    eMbxTferType_VOE_MBX_WRITE      = 16,               /*< VoE write                           */
#endif
#if (defined INCLUDE_AOE_SUPPORT)
    eMbxTferType_AOE_READ           = 17,               /*< AoE read                            */
    eMbxTferType_AOE_WRITE          = 18,               /*< AoE write                           */
    eMbxTferType_AOE_READWRITE      = 19,               /*< AoE read/write                      */
    eMbxTferType_AOE_WRITECONTROL   = 20,               /*< AoE write control                   */
#endif
#if (defined INCLUDE_RAWMBX_SUPPORT)
    eMbxTferType_RAWMBX             = 21,               /*< Raw mbx                             */
#endif
    eMbxTferType_FOE_SEG_DOWNLOAD   = 22,               /*< FoE segmented download              */
    eMbxTferType_FOE_SEG_UPLOAD     = 23,               /*< FoE segmented upload                */

    /* Borland C++ datatype alignment correction */
    eMbxTferType_BCppDummy          = 0xFFFFFFFF
} EC_T_MBXTFER_TYPE;


static EC_INLINESTART const EC_T_CHAR* MbxTferTypeText(EC_T_MBXTFER_TYPE EType)
{
    switch (EType)
    {
    case eMbxTferType_COE_SDO_DOWNLOAD: return "CoE SDO download";
    case eMbxTferType_COE_SDO_UPLOAD:   return "CoE SDO upload";
    case eMbxTferType_COE_GETODLIST:    return "CoE Get object dictionary list";
    case eMbxTferType_COE_GETOBDESC:    return "CoE Get object description";
    case eMbxTferType_COE_GETENTRYDESC: return "CoE Get object entry description";
    case eMbxTferType_COE_EMERGENCY:    return "CoE emergency request"; 
#if (defined INCLUDE_COE_PDO_SUPPORT)
    case eMbxTferType_COE_RX_PDO:       return "CoE RxPDO";               
#endif
    case eMbxTferType_FOE_FILE_UPLOAD:  return "FoE upload";
    case eMbxTferType_FOE_FILE_DOWNLOAD:return "FoE download";
#if (defined INCLUDE_SOE_SUPPORT)
    case eMbxTferType_SOE_READREQUEST:  return "SoE read request";
    case eMbxTferType_SOE_READRESPONSE: return "SoE read response";
    case eMbxTferType_SOE_WRITEREQUEST: return "SoE write request";
    case eMbxTferType_SOE_WRITERESPONSE:return "SoE write response";
    case eMbxTferType_SOE_NOTIFICATION: return "SoE notification";
    case eMbxTferType_SOE_EMERGENCY:    return "SoE emergency";
#endif
#if (defined INCLUDE_VOE_SUPPORT)
    case eMbxTferType_VOE_MBX_READ:     return "VoE read";
    case eMbxTferType_VOE_MBX_WRITE:    return "VoE write";
#endif
#if (defined INCLUDE_AOE_SUPPORT)
    case eMbxTferType_AOE_READ:         return "AoE read";
    case eMbxTferType_AOE_WRITE:        return "AoE write";
    case eMbxTferType_AOE_READWRITE:    return "AoE read/write";
    case eMbxTferType_AOE_WRITECONTROL: return "AoE write control";
#endif
#if (defined INCLUDE_RAWMBX_SUPPORT)
    case eMbxTferType_RAWMBX:           return "Raw mbx";
#endif
    case eMbxTferType_FOE_SEG_DOWNLOAD: return "FoE segmented download";
    case eMbxTferType_FOE_SEG_UPLOAD:   return "FoE segmented upload";
    case eMbxTferType_BCppDummy:        return "Dummy EC_T_MBXTFER_TYPE";
    }
    OsDbgAssert(EC_FALSE);              return "Unknown EC_T_MBXTFER_TYPE";
} EC_INLINESTOP

/* EtherCAT mailbox transfer descriptor */
#include EC_PACKED_INCLUDESTART(1)
typedef struct _EC_T_MBXTFER_DESC
{
    EC_T_DWORD          dwMaxDataLen;               /*< maximum number of bytes to be transferred (allocated buffer len) */
    EC_T_BYTE*          pbyMbxTferDescData;         /*< Data buffer to hold transfer data */
} EC_PACKED(1) EC_T_MBXTFER_DESC;
#include EC_PACKED_INCLUDESTOP

/* EtherCAT mailbox status values */
typedef enum _EC_T_MBXTFER_STATUS
{
    eMbxTferStatus_Idle                     = 0,    /*< Mailbox transfer object currently idle (owned by application), no transfer running */
    eMbxTferStatus_Pend                     = 1,    /*< Mailbox transfer object owned by Master */
    eMbxTferStatus_TferDone                 = 2,    /*< Transfer successfully executed, object owned by application */
    eMbxTferStatus_TferReqError             = 3,    /*< Transfer request error occurred, object owned by application */
    eMbxTferStatus_TferWaitingForContinue   = 4,    /*< Transfer waiting for continue, object owned by application */

    /* Borland C++ datatype alignment correction */
    eMbxTferStatus_BCppDummy    = 0xFFFFFFFF
} EC_T_MBXTFER_STATUS;
#define MbxTferStatusText(EStatus)                                                  \
    ((EStatus)==eMbxTferStatus_Idle?"Idle":                                         \
     ((EStatus)==eMbxTferStatus_Pend?"Pend":                                        \
      ((EStatus)==eMbxTferStatus_TferDone?"TferDone":                               \
       ((EStatus)==eMbxTferStatus_TferReqError?"TferReqError":                      \
        ((EStatus)==eMbxTferStatus_TferWaitingForContinue?"TferWaitingForContinue": \
          "Unknown")))))

/* EtherCAT mailbox transfer object */
#include EC_PACKED_INCLUDESTART(1)
typedef struct _EC_T_MBXTFER
{
    EC_T_DWORD          dwClntId;                   /*< client ID */
    EC_T_MBXTFER_DESC   MbxTferDesc;                /*< mailbox transfer descriptor */
    EC_T_MBXTFER_TYPE   eMbxTferType;               /*< mailbox transfer type */
    EC_T_DWORD          dwDataLen;                  /*< available/reserved length within mailbox data buffer (scope: API call) */
    EC_T_BYTE*          pbyMbxTferData;             /*< pointer to mailbox data buffer */
    EC_T_MBXTFER_STATUS eTferStatus;                /*< current transfer status */
    EC_T_DWORD          dwErrorCode;                /*< transfer error code */
    EC_T_DWORD          dwTferId;                   /*< unique transfer ID */
    EC_T_MBX_DATA       MbxData;                    /*< mailbox data */
} EC_PACKED(1) EC_T_MBXTFER;
#include EC_PACKED_INCLUDESTOP

/* Supported EtherCAT commands for the ecatTferSingleRawCmd() function */
typedef enum _EC_T_RAWCMD
{
    eRawCmd_APRD    = EC_CMD_TYPE_APRD,             /* auto-increment physical read */
    eRawCmd_APWR    = EC_CMD_TYPE_APWR,             /* auto-increment physical write */
    eRawCmd_APRW    = EC_CMD_TYPE_APRW,             /* auto-increment physical read/write */
    eRawCmd_BRD     = EC_CMD_TYPE_BRD,              /* broadcast (wire-or) read */
    eRawCmd_BWR     = EC_CMD_TYPE_BWR,              /* broadcast write */
    eRawCmd_BRW     = EC_CMD_TYPE_BRW,              /* broadcast read/write */
    eRawCmd_LRD     = EC_CMD_TYPE_LRD,              /* logical read */
    eRawCmd_LWR     = EC_CMD_TYPE_LWR,              /* logical write */
    eRawCmd_LRW     = EC_CMD_TYPE_LRW,              /* logical read/write */
    eRawCmd_ARMW    = EC_CMD_TYPE_ARMW,             /* auto-increment physical read, multiple write */
    eRawCmd_FPRD    = EC_CMD_TYPE_FPRD,             /* fixed address physical read */
    eRawCmd_FPWR    = EC_CMD_TYPE_FPWR,             /* fixed address physical write */
    eRawCmd_FPRW    = EC_CMD_TYPE_FPRW,             /* fixed address physical read/write */

    /* Borland C++ datatype alignment correction */
    eRawCmd_BCppDummy   = 0xFFFFFFFF
} EC_T_RAWCMD;

/* Descriptor for EC_IOCTL_GET_PDMEMORYSIZE call */
#include EC_PACKED_INCLUDESTART(1)
typedef struct _EC_T_MEMREQ_DESC
{
    EC_T_DWORD  dwPDOutSize;                        /* Size of the output process data image */
    EC_T_DWORD  dwPDInSize;                         /* Size of the input  process data image */
} EC_PACKED(1) EC_T_MEMREQ_DESC, *EC_PT_MEMREQ_DESC;
#include EC_PACKED_INCLUDESTOP

/* Descriptor for EC_IOCTL_SET_MASTER_DEFAULT_TIMEOUTS */
#include EC_PACKED_INCLUDESTART(1)
typedef struct _EC_T_MASTERDEFAULTTIMEOUTS_DESC
{
    EC_T_DWORD  dwMasterStateChange;                /* Timeout to SetMasterState if called with EC_NOWAIT */
    EC_T_DWORD  dwInitCmdRetry;                     /* Timeout to retry InitCmds */
    EC_T_DWORD  dwMbxCmd;                           /* Timeout to retry mailbox commands */
    EC_T_DWORD  dwMbxPolling;                       /* Mailbox polling cycle */
    EC_T_DWORD  dwDcmInSync;                        /* Timeout to wait for DCM InSync in SetMasterState() */
    EC_T_DWORD  dwReserved[11];
} EC_PACKED(1) EC_T_MASTERDEFAULTTIMEOUTS_DESC;
#include EC_PACKED_INCLUDESTOP

/* Descriptor for EC_IOCTL_HC_CONFIGURETIMEOUTS call */
#include EC_PACKED_INCLUDESTART(1)
typedef struct _EC_T_HC_CONFIGURETIMEOUTS_DESC
{
    EC_T_DWORD  dwDetectionTimeout;                 /* [in] Timeout for Group detection */
    EC_T_DWORD  dwTopologyChangeTimeout;            /* [in] Topology Change Timeout (usually larger than dwDetectionTimeout) */
    EC_T_DWORD  dwRsvd[2];
} EC_PACKED(1) EC_T_HC_CONFIGURETIMEOUTS_DESC;
#include EC_PACKED_INCLUDESTOP

/* EtherCAT user controlled execution job */
typedef enum _EC_T_USER_JOB
{
    eUsrJob_ProcessAllRxFrames              = 1,    /*< polling mode: receive frames and process all received data */
    eUsrJob_SendAllCycFrames                = 2,    /*< send all cyclic frames */
    eUsrJob_RunMcSm                         = 3,    /*< obsolete */
    eUsrJob_MasterTimer                     = 4,    /*< run master timer routine (generic management) */
    eUsrJob_FlushQueuedCmds                 = 5,    /*< obsolete */
    eUsrJob_SendAcycFrames                  = 6,    /*< send acyclic frames (number of frames can be limited by master parameter) */
    eUsrJob_SendCycFramesByTaskId           = 7,    /*< send cyclic frames related to a specific task id (TaskId entry in the XML file) */
    eUsrJob_MasterTimerMinimal              = 8,    /*< run minimal master timer routine: no state change possible  */

    eUsrJob_StampSendAllCycFrames           = 22,   /*< send all cyclic frames (stamped) */
    eUsrJob_StampSendCycFramesByTaskId      = 27,   /*< send cyclic frames related to a specific task id (TaskId entry in the XML file) */

    /* Borland C++ datatype alignment correction */
    eUsrJob_BCppDummy                       = 0xFFFFFFFF
} EC_T_USER_JOB;

typedef EC_T_VOID (*EC_PF_CYCFRAME_RECV)(EC_T_VOID*);
#include EC_PACKED_INCLUDESTART(1)
typedef struct _EC_T_CYCFRAME_RX_CBDESC
{
    EC_T_VOID*                  pCallbackParm;      /*< [in] Callback Parameter */
    EC_PF_CYCFRAME_RECV         pfnCallback;        /*< [in] Callback Function */
} EC_PACKED(1)  EC_T_CYCFRAME_RX_CBDESC;
#include EC_PACKED_INCLUDESTOP

typedef EC_T_BOOL (*EC_PF_COE_INITCMD_CALLBACK)(EC_T_VOID* pvParm, EC_T_DWORD dwSlaveId, EC_T_DWORD dwHandle, EC_T_WORD wIndex, EC_T_BYTE bySubIndex, EC_T_DWORD wDataLen, EC_T_BYTE* pbyData);
#include EC_PACKED_INCLUDESTART(8)
typedef struct _EC_T_ADD_COE_INITCMD_DESC_ENTRY
{
    EC_T_DWORD      dwHandle;                           /*< 32Bit handle passed to registered pfnCallback */
    EC_T_WORD       wTransition;                        /*< transition e.g. ECAT_INITCMD_I_P */    
    EC_T_CHAR       szComment[MAX_STD_STRLEN];          /*< comment (ENI) */
    EC_T_WORD       wTimeout;                           /*< timeout */
    EC_T_BOOL       bIgnoreFailure;                     /*< continue to process InitCmd on error */
    EC_T_BYTE       byCcs;                              /*< client command specifier (read or write access) */
    EC_T_BOOL       bCompleteAccess;                    /*< complete access */
    EC_T_WORD       wIndex;                             /*< object index */
    EC_T_BYTE       bySubIndex;                         /*< object subindex */
    EC_T_DWORD      wDataLen;                           /*< length of the data */
    EC_T_BYTE*      pbyData;
} EC_PACKED(8)  EC_T_ADD_COE_INITCMD_DESC_ENTRY;
#include EC_PACKED_INCLUDESTOP

#include EC_PACKED_INCLUDESTART(8)
typedef struct _EC_T_ADD_COE_INITCMD_DESC
{
    EC_T_DWORD dwSlaveId;
    EC_T_WORD  wCount;
    EC_T_ADD_COE_INITCMD_DESC_ENTRY* pbCoeInitCmds;
    EC_T_VOID* pvCallbackParm;
    EC_PF_COE_INITCMD_CALLBACK pfnCallback;
} EC_PACKED(8)  EC_T_ADD_COE_INITCMD_DESC;
#include EC_PACKED_INCLUDESTOP

typedef enum _EC_T_CYCFRAME_LAYOUT
{
    eCycFrameLayout_STANDARD    = 0,    /* Frame layout according ENI with command add/reordering, no relationship to PD */
    eCycFrameLayout_DYNAMIC     = 1,    /* Frame layout is dynamically modified to send as less as possible cyclic commands in as less as possible frames */
    eCycFrameLayout_FIXED       = 2,    /* Frame layout strictly match ENI, frame buffers and PD area overlapped */
    eCycFrameLayout_IN_DMA      = 3,    /* Frame layout strictly match ENI, frame buffers and PD area overlapped, frame buffers in DMA */
    /* Borland C++ datatype alignment correction */
    eCycFrameLayout_BCppDummy   = 0xFFFFFFFF
} EC_T_CYCFRAME_LAYOUT;

/* pass through server states */
typedef enum _EC_PTS_STATE
{
    ePtsStateNone                  = 0x0000,
    ePtsStateNotRunning            = 0x0001,
    ePtsStateRunningDisabled       = 0x0002,
    ePtsStateRunningEnabled        = 0x0003,

    /* Borland C++ datatype alignment correction */
    ePtsStateDummy                 = 0xFFFFFFFF
} EC_PTS_STATE;


/* Start parameter for the Pass-Through-Server */
#include EC_PACKED_INCLUDESTART(1)
typedef struct _EC_T_PTS_SRV_START_PARMS
{
    EC_T_IPADDR oIpAddr;
    EC_T_DWORD  dwPtsThreadPriority;
    EC_T_WORD   wPort;
    EC_T_WORD   wReserved;
} EC_PACKED(1)  EC_T_PTS_SRV_START_PARMS;
#include EC_PACKED_INCLUDESTOP

#define EC_NOTIFICATION_DISABLED    (0)
#define EC_NOTIFICATION_ENABLED     (1)
#define EC_NOTIFICATION_DEFAULT     (2)
#define EC_ALL_NOTIFICATIONS        (0xffffffff)
#include EC_PACKED_INCLUDESTART(1)
typedef struct _EC_T_SET_NOTIFICATION_ENABLED_PARMS
{
    EC_T_DWORD dwClientId;  /*< [in] Client ID, 0: Master */
    EC_T_DWORD dwCode;      /*< [in] Notification code */
    EC_T_DWORD dwEnabled;   /*< [in] EC_NOTIFICATION_DISABLED, EC_NOTIFICATION_ENABLED, EC_NOTIFICATION_DEFAULT */
} EC_PACKED(1) EC_T_SET_NOTIFICATION_ENABLED_PARMS;
#include EC_PACKED_INCLUDESTOP

#include EC_PACKED_INCLUDESTART(1)
typedef struct _EC_T_GET_NOTIFICATION_ENABLED_PARMS
{
    EC_T_DWORD dwClientId;  /*< [in] Client ID, 0: Master */
    EC_T_DWORD dwCode;      /*< [in] Notification code */
} EC_PACKED(1) EC_T_GET_NOTIFICATION_ENABLED_PARMS;
#include EC_PACKED_INCLUDESTOP

#include EC_PACKED_INCLUDESTART(8)
typedef struct _EC_T_CLEAR_MASTER_INFO_COUNTERS_PARMS
{
    EC_T_DWORD  dwClearBusDiagnosisCounters;
    EC_T_UINT64 qwMailboxStatisticsClearCounters;
    EC_T_DWORD  dwReserved[6];
} EC_PACKED(8) EC_T_CLEAR_MASTER_INFO_COUNTERS_PARMS;
#include EC_PACKED_INCLUDESTOP

#include EC_PACKED_INCLUDESTART(1)
typedef struct _EC_T_MASTEROD_OBJECT_PARMS
{
    EC_T_WORD  wIndex;      /*< Object's index, e.g. 0x1018 */
    EC_T_BYTE  bySubindex;  /*< Object's sub-index, e.g. 1 */
    EC_T_BYTE  byReserved;
    EC_T_BYTE* pbyData;     /*< Pointer to object's data to be written */
    EC_T_DWORD dwLength;    /*< Data length to be written */
    EC_T_DWORD dwReserved;
} EC_PACKED(1) EC_T_MASTEROD_OBJECT_PARMS;
#include EC_PACKED_INCLUDESTOP

/* Start parameters for ADS Adapter */
#include EC_PACKED_INCLUDESTART(1)
typedef struct _EC_T_ADS_ADAPTER_START_PARMS
{
    EC_T_DWORD      dwSize;             /*< [in] structure size */
    EC_T_DWORD      dwThreadPriority;
    EC_T_AOE_NETID  targetNetID;
    EC_T_WORD       targetPort;
    EC_T_WORD       wReserved;
} EC_PACKED(1) EC_T_ADS_ADAPTER_START_PARMS;
#include EC_PACKED_INCLUDESTOP

/*-FUNCTION DECLARATIONS-----------------------------------------------------*/

#ifdef __cplusplus
extern "C"
{
#endif

ATECAT_API const EC_T_CHAR* ecatGetText(            EC_T_DWORD              dwTextId        );
ATECAT_API const EC_T_CHAR* ecatGetNotifyText(      EC_T_DWORD              dwNotifyCode    );

ATECAT_API EC_T_VOID ecatPerfMeasInit( EC_T_TSC_MEAS_DESC* pTscMeasDesc, EC_T_UINT64 dwlFreqSet, EC_T_DWORD dwNumMeas, EC_T_FNMESSAGE pfnMessage);
ATECAT_API EC_T_VOID ecatPerfMeasDeinit( EC_T_TSC_MEAS_DESC* pTscMeasDesc );
ATECAT_API EC_T_VOID ecatPerfMeasEnable( EC_T_TSC_MEAS_DESC* pTscMeasDesc );
ATECAT_API EC_T_VOID ecatPerfMeasDisable( EC_T_TSC_MEAS_DESC* pTscMeasDesc );
ATECAT_API EC_T_VOID ecatPerfMeasStart( EC_T_TSC_MEAS_DESC* pTscMeasDesc, EC_T_DWORD dwIndex );
ATECAT_API EC_T_TSC_TIME* ecatPerfMeasEnd( EC_T_TSC_MEAS_DESC* pTscMeasDesc, EC_T_DWORD dwIndex );
ATECAT_API EC_T_VOID ecatPerfMeasReset( EC_T_TSC_MEAS_DESC* pTscMeasDesc, EC_T_DWORD dwIndex );
ATECAT_API EC_T_VOID ecatPerfMeasShow( EC_T_TSC_MEAS_DESC* pTscMeasDesc, EC_T_DWORD dwIndex, EC_T_CHAR** aszMeasInfo );

/* Multi instance API functions */

ATECAT_API             EC_T_DWORD emInitMaster(     EC_T_DWORD              dwInstanceID,
                                                    EC_T_INIT_MASTER_PARMS* pParms          );
static EC_INLINESTART  EC_T_DWORD ecatInitMaster(   EC_T_INIT_MASTER_PARMS* pParms          )
{
    return emInitMaster(INSTANCE_MASTER_DEFAULT, pParms);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD emDeinitMaster(   EC_T_DWORD              dwInstanceID    );
static EC_INLINESTART  EC_T_DWORD ecatDeinitMaster( EC_T_VOID                               )
{
    return emDeinitMaster(INSTANCE_MASTER_DEFAULT);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD emGetMasterParms( EC_T_DWORD              dwInstanceID,
                                                    EC_T_INIT_MASTER_PARMS* pParms,
                                                    EC_T_DWORD              dwParmsBufSize);
static EC_INLINESTART  EC_T_DWORD ecatGetMasterParms(EC_T_INIT_MASTER_PARMS* pParms, EC_T_DWORD dwParmsBufSize)
{
    return emGetMasterParms(INSTANCE_MASTER_DEFAULT, pParms, dwParmsBufSize);
} EC_INLINESTOP
ATECAT_API             EC_T_DWORD emSetMasterParms( EC_T_DWORD              dwInstanceID,
                                                    EC_T_INIT_MASTER_PARMS* pParms          );
static EC_INLINESTART  EC_T_DWORD ecatSetMasterParms(EC_T_INIT_MASTER_PARMS* pParms         )
{
    return emSetMasterParms(INSTANCE_MASTER_DEFAULT, pParms);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD emIoControl(      EC_T_DWORD              dwInstanceID,
                                                    EC_T_DWORD              dwCode,
                                                    EC_T_IOCTLPARMS*        pParms          );
static EC_INLINESTART  EC_T_DWORD ecatIoControl(    EC_T_DWORD              dwCode,
                                                    EC_T_IOCTLPARMS*        pParms          )
{
    return emIoControl(INSTANCE_MASTER_DEFAULT, dwCode, pParms);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD emScanBus(        EC_T_DWORD              dwInstanceID,
                                                    EC_T_DWORD              dwTimeout       );
static EC_INLINESTART  EC_T_DWORD ecatScanBus(      EC_T_DWORD              dwTimeout       )
{
    return emScanBus(INSTANCE_MASTER_DEFAULT, dwTimeout);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD emRescueScan(     EC_T_DWORD               dwInstanceID,
                                                    EC_T_DWORD               dwTimeout      );
static EC_INLINESTART  EC_T_DWORD ecatRescueScan(   EC_T_DWORD               dwTimeout      )
{
    return emRescueScan(INSTANCE_MASTER_DEFAULT, dwTimeout);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD emGetMasterInfo(  EC_T_DWORD               dwInstanceID,
                                                    EC_T_MASTER_INFO*        pMasterInfo    );
static EC_INLINESTART  EC_T_DWORD ecatGetMasterInfo(EC_T_MASTER_INFO*        pMasterInfo    )
{
    return emGetMasterInfo(INSTANCE_MASTER_DEFAULT, pMasterInfo);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD emConfigureMaster(EC_T_DWORD              dwInstanceID,
                                                    EC_T_CNF_TYPE           eCnfType,
                                                    EC_T_PBYTE              pbyCnfData,
                                                    EC_T_DWORD              dwCnfDataLen    );
static EC_INLINESTART  EC_T_DWORD ecatConfigureMaster(EC_T_CNF_TYPE         eCnfType,
                                                    EC_T_PBYTE              pbyCnfData,
                                                    EC_T_DWORD              dwCnfDataLen    )
{
    return emConfigureMaster(INSTANCE_MASTER_DEFAULT, eCnfType, pbyCnfData, dwCnfDataLen);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD emConfigLoad(     EC_T_DWORD              dwInstanceID,
                                                    EC_T_CNF_TYPE           eCnfType,
                                                    EC_T_PBYTE              pbyCnfData,
                                                    EC_T_DWORD              dwCnfDataLen    );
static EC_INLINESTART  EC_T_DWORD ecatConfigLoad(   EC_T_CNF_TYPE           eCnfType,
                                                    EC_T_PBYTE              pbyCnfData,
                                                    EC_T_DWORD              dwCnfDataLen    )
{
    return emConfigLoad(INSTANCE_MASTER_DEFAULT, eCnfType, pbyCnfData, dwCnfDataLen);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD emConfigExcludeSlave(EC_T_DWORD           dwInstanceID,
                                                    EC_T_WORD               wPhysAddress    );
static EC_INLINESTART  EC_T_DWORD ecatConfigExcludeSlave(EC_T_WORD          wPhysAddress    )
{
    return emConfigExcludeSlave(INSTANCE_MASTER_DEFAULT, wPhysAddress);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD emConfigIncludeSlave(EC_T_DWORD           dwInstanceID,
                                                    EC_T_WORD               wPhysAddress    );
static EC_INLINESTART  EC_T_DWORD ecatConfigIncludeSlave(EC_T_WORD          wPhysAddress    )
{
    return emConfigIncludeSlave(INSTANCE_MASTER_DEFAULT, wPhysAddress);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD emConfigSetPreviousPort(EC_T_DWORD        dwInstanceID,
                                                    EC_T_WORD               wPhysAddress,
                                                    EC_T_WORD               wPhysAddressPrev,
                                                    EC_T_WORD               wPortPrev       );
static EC_INLINESTART  EC_T_DWORD ecatConfigSetPreviousPort(EC_T_WORD       wPhysAddress,
                                                    EC_T_WORD               wPhysAddressPrev,
                                                    EC_T_WORD               wPortPrev       )
{
    return emConfigSetPreviousPort(INSTANCE_MASTER_DEFAULT, wPhysAddress, wPhysAddressPrev, wPortPrev);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD emConfigApply(    EC_T_DWORD              dwInstanceID    );
static EC_INLINESTART  EC_T_DWORD ecatConfigApply(  EC_T_VOID                               )
{
    return emConfigApply(INSTANCE_MASTER_DEFAULT);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD emConfigExtend(   EC_T_DWORD              dwInstanceID,
                                                    EC_T_BOOL               bResetConfig,
                                                    EC_T_DWORD              dwTimeout       );
static EC_INLINESTART  EC_T_DWORD ecatConfigExtend( EC_T_BOOL               bResetConfig,
                                                    EC_T_DWORD              dwTimeout       )
{
    return emConfigExtend(INSTANCE_MASTER_DEFAULT, bResetConfig, dwTimeout);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD emSetMasterState( EC_T_DWORD              dwInstanceID,
                                                    EC_T_DWORD              dwTimeout,
                                                    EC_T_STATE              eReqState       );
static EC_INLINESTART  EC_T_DWORD ecatSetMasterState(EC_T_DWORD             dwTimeout,
                                                    EC_T_STATE              eReqState       )
{
    return emSetMasterState(INSTANCE_MASTER_DEFAULT, dwTimeout, eReqState);
} EC_INLINESTOP

ATECAT_API             EC_T_STATE emGetMasterState( EC_T_DWORD              dwInstanceID    );
static EC_INLINESTART  EC_T_STATE ecatGetMasterState(EC_T_VOID                               )
{
    return emGetMasterState(INSTANCE_MASTER_DEFAULT);
} EC_INLINESTOP


ATECAT_API             EC_T_DWORD emStart(          EC_T_DWORD              dwInstanceID,
                                                    EC_T_DWORD              dwTimeout       );
static EC_INLINESTART  EC_T_DWORD ecatStart(        EC_T_DWORD              dwTimeout       )
{
    return emStart(INSTANCE_MASTER_DEFAULT, dwTimeout);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD emStop(           EC_T_DWORD              dwInstanceID,
                                                    EC_T_DWORD              dwTimeout       );
static EC_INLINESTART  EC_T_DWORD ecatStop(         EC_T_DWORD              dwTimeout       )
{
    return emStop(INSTANCE_MASTER_DEFAULT, dwTimeout);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD emGetSlaveId(     EC_T_DWORD              dwInstanceID,
                                                    EC_T_WORD               wStationAddress );
static EC_INLINESTART  EC_T_DWORD ecatGetSlaveId(   EC_T_WORD               wStationAddress )
{
    return emGetSlaveId(INSTANCE_MASTER_DEFAULT, wStationAddress);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD emGetSlaveFixedAddr(EC_T_DWORD            dwInstanceID,
                                                    EC_T_DWORD              dwSlaveId,
                                                    EC_T_WORD*              pwFixedAddr     );
static EC_INLINESTART  EC_T_DWORD ecatGetSlaveFixedAddr( EC_T_DWORD         dwSlaveId,
                                                    EC_T_WORD*              pwFixedAddr     )
{
    return emGetSlaveFixedAddr(INSTANCE_MASTER_DEFAULT, dwSlaveId, pwFixedAddr);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD emGetSlaveIdAtPosition(EC_T_DWORD         dwInstanceID,
                                                    EC_T_WORD               wAutoIncAddress );
static EC_INLINESTART  EC_T_DWORD ecatGetSlaveIdAtPosition(EC_T_WORD        wAutoIncAddress )
{
    return emGetSlaveIdAtPosition(INSTANCE_MASTER_DEFAULT, wAutoIncAddress);
} EC_INLINESTOP

ATECAT_API             EC_T_BOOL  emGetSlaveProp(   EC_T_DWORD              dwInstanceID,
                                                    EC_T_DWORD              dwSlaveId,
                                                    EC_T_SLAVE_PROP*        pSlaveProp      );
static EC_INLINESTART  EC_T_BOOL  ecatGetSlaveProp( EC_T_DWORD              dwSlaveId,
                                                    EC_T_SLAVE_PROP*        pSlaveProp      )
{
    return emGetSlaveProp(INSTANCE_MASTER_DEFAULT, dwSlaveId, pSlaveProp);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD emGetSlavePortState(EC_T_DWORD            dwInstanceID,
                                                    EC_T_DWORD              dwSlaveId,
                                                    EC_T_WORD*              pwPortState     );
static EC_INLINESTART  EC_T_DWORD ecatGetSlavePortState( EC_T_DWORD         dwSlaveId,
                                                    EC_T_WORD*              pwPortState     )
{
    return emGetSlavePortState(INSTANCE_MASTER_DEFAULT, dwSlaveId, pwPortState);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD emGetSlaveState(  EC_T_DWORD              dwInstanceID,
                                                    EC_T_DWORD              dwSlaveId,
                                                    EC_T_WORD*              pwCurrDevState,
                                                    EC_T_WORD*              pwReqDevState   );
static EC_INLINESTART  EC_T_DWORD ecatGetSlaveState(EC_T_DWORD              dwSlaveId,
                                                    EC_T_WORD*              pwCurrDevState,
                                                    EC_T_WORD*              pwReqDevState   )
{
    return emGetSlaveState(INSTANCE_MASTER_DEFAULT, dwSlaveId, pwCurrDevState, pwReqDevState);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD emSetSlaveState(  EC_T_DWORD              dwInstanceID,
                                                    EC_T_DWORD              dwSlaveId,
                                                    EC_T_WORD               wNewReqDevState,
                                                    EC_T_DWORD              dwTimeout       );
static EC_INLINESTART  EC_T_DWORD ecatSetSlaveState(EC_T_DWORD              dwSlaveId,
                                                    EC_T_WORD               wNewReqDevState,
                                                    EC_T_DWORD              dwTimeout       )
{
    return emSetSlaveState(INSTANCE_MASTER_DEFAULT, dwSlaveId, wNewReqDevState, dwTimeout);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD emTferSingleRawCmd(EC_T_DWORD             dwInstanceID,
                                                    EC_T_DWORD              dwSlaveId,
                                                    EC_T_BYTE               byCmd,
                                                    EC_T_DWORD              dwMemoryAddress,
                                                    EC_T_VOID*              pvData,
                                                    EC_T_WORD               wLen,
                                                    EC_T_DWORD              dwTimeout       );
static EC_INLINESTART  EC_T_DWORD ecatTferSingleRawCmd(EC_T_DWORD           dwSlaveId,
                                                    EC_T_BYTE               byCmd,
                                                    EC_T_DWORD              dwMemoryAddress,
                                                    EC_T_VOID*              pvData,
                                                    EC_T_WORD               wLen,
                                                    EC_T_DWORD              dwTimeout       )
{
    return emTferSingleRawCmd(INSTANCE_MASTER_DEFAULT, dwSlaveId, byCmd, dwMemoryAddress, pvData, wLen, dwTimeout);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD emReadSlaveRegister(EC_T_DWORD            dwInstanceID,
                                                    EC_T_BOOL               bFixedAddressing,
                                                    EC_T_WORD               wSlaveAddress,
                                                    EC_T_WORD               wRegisterOffset,
                                                    EC_T_VOID*              pvData,
                                                    EC_T_WORD               wLen,
                                                    EC_T_DWORD              dwTimeout       );
static EC_INLINESTART  EC_T_DWORD ecatReadSlaveRegister(EC_T_BOOL           bFixedAddressing,
                                                    EC_T_WORD               wSlaveAddress,
                                                    EC_T_WORD               wRegisterOffset,
                                                    EC_T_VOID*              pvData,
                                                    EC_T_WORD               wLen,
                                                    EC_T_DWORD              dwTimeout       )
{
   return emReadSlaveRegister(
    		INSTANCE_MASTER_DEFAULT, bFixedAddressing, wSlaveAddress, wRegisterOffset, pvData, wLen, dwTimeout);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD emWriteSlaveRegister(EC_T_DWORD           dwInstanceID,
                                                    EC_T_BOOL               bFixedAddressing,
                                                    EC_T_WORD               wSlaveAddress,
                                                    EC_T_WORD               wRegisterOffset,
                                                    EC_T_VOID*              pvData,
                                                    EC_T_WORD               wLen,
                                                    EC_T_DWORD              dwTimeout       );
static EC_INLINESTART  EC_T_DWORD ecatWriteSlaveRegister(EC_T_BOOL          bFixedAddressing,
                                                    EC_T_WORD               wSlaveAddress,
                                                    EC_T_WORD               wRegisterOffset,
                                                    EC_T_VOID*              pvData,
                                                    EC_T_WORD               wLen,
                                                    EC_T_DWORD              dwTimeout       )
{
   return emWriteSlaveRegister(
    		INSTANCE_MASTER_DEFAULT, bFixedAddressing,wSlaveAddress, wRegisterOffset, pvData, wLen, dwTimeout);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD emQueueRawCmd(    EC_T_DWORD              dwInstanceID,
                                                    EC_T_DWORD              dwSlaveId,
                                                    EC_T_WORD               wInvokeId,
                                                    EC_T_BYTE               byCmd,
                                                    EC_T_DWORD              dwMemoryAddress,
                                                    EC_T_VOID*              pvData,
                                                    EC_T_WORD               wLen            );
static EC_INLINESTART  EC_T_DWORD ecatQueueRawCmd(  EC_T_DWORD              dwSlaveId,
                                                    EC_T_WORD               wInvokeId,
                                                    EC_T_BYTE               byCmd,
                                                    EC_T_DWORD              dwMemoryAddress,
                                                    EC_T_VOID*              pvData,
                                                    EC_T_WORD               wLen            )
{
    return emQueueRawCmd(INSTANCE_MASTER_DEFAULT, dwSlaveId, wInvokeId, byCmd, dwMemoryAddress, pvData, wLen);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD emClntQueueRawCmd(EC_T_DWORD              dwInstanceID,
                                                    EC_T_DWORD              dwClntId,
                                                    EC_T_DWORD              dwSlaveId,
                                                    EC_T_WORD               wInvokeId,
                                                    EC_T_BYTE               byCmd,
                                                    EC_T_DWORD              dwMemoryAddress,
                                                    EC_T_VOID*              pvData,
                                                    EC_T_WORD               wLen            );
static EC_INLINESTART  EC_T_DWORD ecatClntQueueRawCmd(EC_T_DWORD            dwClntId,
                                                    EC_T_DWORD              dwSlaveId,
                                                    EC_T_WORD               wInvokeId,
                                                    EC_T_BYTE               byCmd,
                                                    EC_T_DWORD              dwMemoryAddress,
                                                    EC_T_VOID*              pvData,
                                                    EC_T_WORD               wLen            )
{
    return emClntQueueRawCmd(INSTANCE_MASTER_DEFAULT, dwClntId, dwSlaveId, wInvokeId, byCmd, dwMemoryAddress, pvData, wLen);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD emGetNumConfiguredSlaves(EC_T_DWORD       dwInstanceID    );
static EC_INLINESTART  EC_T_DWORD ecatGetNumConfiguredSlaves(EC_T_VOID                      )
{
    return emGetNumConfiguredSlaves(INSTANCE_MASTER_DEFAULT);
} EC_INLINESTOP

ATECAT_API             EC_T_MBXTFER* emMbxTferCreate(EC_T_DWORD             dwInstanceID,
                                                    EC_T_MBXTFER_DESC*      pMbxTferDesc    );
static EC_INLINESTART  EC_T_MBXTFER* ecatMbxTferCreate(EC_T_MBXTFER_DESC*   pMbxTferDesc    )
{
    return emMbxTferCreate(INSTANCE_MASTER_DEFAULT, pMbxTferDesc);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD    emMbxTferAbort(EC_T_DWORD              dwInstanceID,
                                                    EC_T_MBXTFER*           pMbxTfer        );
static EC_INLINESTART  EC_T_DWORD    ecatMbxTferAbort(EC_T_MBXTFER*         pMbxTfer        )
{
    return emMbxTferAbort(INSTANCE_MASTER_DEFAULT, pMbxTfer);
} EC_INLINESTOP

ATECAT_API             EC_T_VOID     emMbxTferDelete(EC_T_DWORD             dwInstanceID,
                                                    EC_T_MBXTFER*           pMbxTfer        );
static EC_INLINESTART  EC_T_VOID     ecatMbxTferDelete(  EC_T_MBXTFER*      pMbxTfer        )
{
    emMbxTferDelete(INSTANCE_MASTER_DEFAULT, pMbxTfer);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD    emClntSendRawMbx(EC_T_DWORD            dwInstanceID,
                                                    EC_T_DWORD              dwClntId,
                                                    EC_T_BYTE*              pbyMbxCmd,
                                                    EC_T_DWORD              dwMbxCmdLen,
                                                    EC_T_DWORD              dwTimeout       );
static EC_INLINESTART  EC_T_DWORD    ecatClntSendRawMbx(EC_T_DWORD          dwClntId,
                                                    EC_T_BYTE*              pbyMbxCmd,
                                                    EC_T_DWORD              dwMbxCmdLen,
                                                    EC_T_DWORD              dwTimeout       )
{
    return emClntSendRawMbx(INSTANCE_MASTER_DEFAULT, dwClntId, pbyMbxCmd, dwMbxCmdLen, dwTimeout);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD  emCoeSdoDownloadReq(EC_T_DWORD           dwInstanceID,
                                                    EC_T_MBXTFER*           pMbxTfer,
                                                    EC_T_DWORD              dwSlaveId,
                                                    EC_T_WORD               wObIndex,
                                                    EC_T_BYTE               byObSubIndex,
                                                    EC_T_DWORD              dwTimeout,
                                                    EC_T_DWORD              dwFlags         );
static EC_INLINESTART  EC_T_DWORD  ecatCoeSdoDownloadReq(EC_T_MBXTFER*      pMbxTfer,
                                                    EC_T_DWORD              dwSlaveId,
                                                    EC_T_WORD               wObIndex,
                                                    EC_T_BYTE               byObSubIndex,
                                                    EC_T_DWORD              dwTimeout,
                                                    EC_T_DWORD              dwFlags         )
{
    return emCoeSdoDownloadReq(INSTANCE_MASTER_DEFAULT, pMbxTfer, dwSlaveId, wObIndex, byObSubIndex, dwTimeout, dwFlags);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD   emCoeSdoDownload(EC_T_DWORD             dwInstanceID
                                                   ,EC_T_DWORD              dwSlaveId
                                                   ,EC_T_WORD               wObIndex
                                                   ,EC_T_BYTE               byObSubIndex
                                                   ,EC_T_BYTE*              pbyData
                                                   ,EC_T_DWORD              dwDataLen
                                                   ,EC_T_DWORD              dwTimeout
                                                   ,EC_T_DWORD              dwFlags         );

static EC_INLINESTART  EC_T_DWORD   ecatCoeSdoDownload(EC_T_DWORD           dwSlaveId
                                                   ,EC_T_WORD               wObIndex
                                                   ,EC_T_BYTE               byObSubIndex
                                                   ,EC_T_BYTE*              pbyData
                                                   ,EC_T_DWORD              dwDataLen
                                                   ,EC_T_DWORD              dwTimeout
                                                   ,EC_T_DWORD              dwFlags         )
{
    return emCoeSdoDownload(INSTANCE_MASTER_DEFAULT, dwSlaveId, wObIndex, byObSubIndex, pbyData, dwDataLen, dwTimeout, dwFlags);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD  emCoeSdoUploadReq(EC_T_DWORD             dwInstanceID,
                                                    EC_T_MBXTFER*           pMbxTfer,
                                                    EC_T_DWORD              dwSlaveId,
                                                    EC_T_WORD               wObIndex,
                                                    EC_T_BYTE               byObSubIndex,
                                                    EC_T_DWORD              dwTimeout,
                                                    EC_T_DWORD              dwFlags         );
static EC_INLINESTART  EC_T_DWORD  ecatCoeSdoUploadReq(EC_T_MBXTFER*        pMbxTfer,
                                                    EC_T_DWORD              dwSlaveId,
                                                    EC_T_WORD               wObIndex,
                                                    EC_T_BYTE               byObSubIndex,
                                                    EC_T_DWORD              dwTimeout,
                                                    EC_T_DWORD              dwFlags         )
{
    return emCoeSdoUploadReq(INSTANCE_MASTER_DEFAULT, pMbxTfer, dwSlaveId, wObIndex, byObSubIndex, dwTimeout, dwFlags);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD  emCoeSdoUpload(  EC_T_DWORD              dwInstanceID
                                                   ,EC_T_DWORD              dwSlaveId
                                                   ,EC_T_WORD               wObIndex
                                                   ,EC_T_BYTE               byObSubIndex
                                                   ,EC_T_BYTE*              pbyData
                                                   ,EC_T_DWORD              dwDataLen
                                                   ,EC_T_DWORD*             pdwOutDataLen
                                                   ,EC_T_DWORD              dwTimeout
                                                   ,EC_T_DWORD              dwFlags         );
static EC_INLINESTART  EC_T_DWORD  ecatCoeSdoUpload(EC_T_DWORD              dwSlaveId
                                                   ,EC_T_WORD               wObIndex
                                                   ,EC_T_BYTE               byObSubIndex
                                                   ,EC_T_BYTE*              pbyData
                                                   ,EC_T_DWORD              dwDataLen
                                                   ,EC_T_DWORD*             pdwOutDataLen
                                                   ,EC_T_DWORD              dwTimeout
                                                   ,EC_T_DWORD              dwFlags         )
{
    return emCoeSdoUpload(INSTANCE_MASTER_DEFAULT, dwSlaveId, wObIndex, byObSubIndex, pbyData, dwDataLen, pdwOutDataLen, dwTimeout, dwFlags);
} EC_INLINESTOP


ATECAT_API             EC_T_DWORD  emCoeGetODList(  EC_T_DWORD              dwInstanceID,
                                                    EC_T_MBXTFER*           pMbxTfer,
                                                    EC_T_DWORD              dwSlaveId,
                                                    EC_T_COE_ODLIST_TYPE    eListType,
                                                    EC_T_DWORD              dwTimeout       );
static EC_INLINESTART  EC_T_DWORD  ecatCoeGetODList(EC_T_MBXTFER*           pMbxTfer,
                                                    EC_T_DWORD              dwSlaveId,
                                                    EC_T_COE_ODLIST_TYPE    eListType,
                                                    EC_T_DWORD              dwTimeout       )
{
    return emCoeGetODList(INSTANCE_MASTER_DEFAULT, pMbxTfer, dwSlaveId, eListType, dwTimeout);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD  emCoeGetObjectDesc(EC_T_DWORD            dwInstanceID,
                                                    EC_T_MBXTFER*           pMbxTfer,
                                                    EC_T_DWORD              dwSlaveId,
                                                    EC_T_WORD               wObIndex,
                                                    EC_T_DWORD              dwTimeout       );
static EC_INLINESTART  EC_T_DWORD  ecatCoeGetObjectDesc(EC_T_MBXTFER*       pMbxTfer,
                                                    EC_T_DWORD              dwSlaveId,
                                                    EC_T_WORD               wObIndex,
                                                    EC_T_DWORD              dwTimeout       )
{
    return emCoeGetObjectDesc(INSTANCE_MASTER_DEFAULT, pMbxTfer, dwSlaveId, wObIndex, dwTimeout);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD  emCoeGetEntryDesc(EC_T_DWORD             dwInstanceID,
                                                    EC_T_MBXTFER*           pMbxTfer,
                                                    EC_T_DWORD              dwSlaveId,
                                                    EC_T_WORD               wObIndex,
                                                    EC_T_BYTE               byObSubIndex,
                                                    EC_T_BYTE               byValueInfo,
                                                    EC_T_DWORD              dwTimeout       );

static EC_INLINESTART  EC_T_DWORD  ecatCoeGetEntryDesc(EC_T_MBXTFER*        pMbxTfer,
                                                    EC_T_DWORD              dwSlaveId,
                                                    EC_T_WORD               wObIndex,
                                                    EC_T_BYTE               byObSubIndex,
                                                    EC_T_BYTE               byValueInfo,
                                                    EC_T_DWORD              dwTimeout       )
{
    return emCoeGetEntryDesc(INSTANCE_MASTER_DEFAULT, pMbxTfer, dwSlaveId, wObIndex, byObSubIndex, byValueInfo, dwTimeout);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD emCoeRxPdoTfer(   EC_T_DWORD              dwInstanceID,
                                                    EC_T_MBXTFER*           pMbxTfer,
                                                    EC_T_DWORD              dwSlaveId,
                                                    EC_T_DWORD              dwNumber,
                                                    EC_T_DWORD              dwTimeout       );

static EC_INLINESTART  EC_T_DWORD ecatCoeRxPdoTfer( EC_T_MBXTFER*           pMbxTfer,
                                                    EC_T_DWORD              dwSlaveId,
                                                    EC_T_DWORD              dwNumber,
                                                    EC_T_DWORD              dwTimeout       )
{
    return emCoeRxPdoTfer(INSTANCE_MASTER_DEFAULT, pMbxTfer, dwSlaveId, dwNumber, dwTimeout);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD  emFoeFileUpload( EC_T_DWORD              dwInstanceID,
                                                    EC_T_DWORD              dwSlaveId,
                                                    EC_T_CHAR*              achFileName,
                                                    EC_T_DWORD              dwFileNameLen,
                                                    EC_T_BYTE*              pbyData,
                                                    EC_T_DWORD              dwDataLen,
                                                    EC_T_DWORD*             pdwOutDataLen,
                                                    EC_T_DWORD              dwPassword,
                                                    EC_T_DWORD              dwTimeout       );

static EC_INLINESTART  EC_T_DWORD  ecatFoeFileUpload(EC_T_DWORD             dwSlaveId,
                                                    EC_T_CHAR*              achFileName,
                                                    EC_T_DWORD              dwFileNameLen,
                                                    EC_T_BYTE*              pbyData,
                                                    EC_T_DWORD              dwDataLen,
                                                    EC_T_DWORD*             pdwOutDataLen,
                                                    EC_T_DWORD              dwPassword,
                                                    EC_T_DWORD              dwTimeout       )
{
    return emFoeFileUpload(INSTANCE_MASTER_DEFAULT, dwSlaveId, achFileName, dwFileNameLen, pbyData, dwDataLen, pdwOutDataLen, dwPassword, dwTimeout);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD  emFoeFileDownload(EC_T_DWORD             dwInstanceID,
                                                    EC_T_DWORD              dwSlaveId,
                                                    EC_T_CHAR*              achFileName,
                                                    EC_T_DWORD              dwFileNameLen,
                                                    EC_T_BYTE*              pbyData,
                                                    EC_T_DWORD              dwDataLen,
                                                    EC_T_DWORD              dwPassword,
                                                    EC_T_DWORD              dwTimeout       );

static EC_INLINESTART  EC_T_DWORD  ecatFoeFileDownload(EC_T_DWORD           dwSlaveId,
                                                    EC_T_CHAR*              achFileName,
                                                    EC_T_DWORD              dwFileNameLen,
                                                    EC_T_BYTE*              pbyData,
                                                    EC_T_DWORD              dwDataLen,
                                                    EC_T_DWORD              dwPassword,
                                                    EC_T_DWORD              dwTimeout       )
{
    return emFoeFileDownload(INSTANCE_MASTER_DEFAULT, dwSlaveId, achFileName, dwFileNameLen, pbyData, dwDataLen, dwPassword, dwTimeout);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD  emFoeUploadReq(  EC_T_DWORD              dwInstanceID,
                                                    EC_T_MBXTFER*           pMbxTfer,
                                                    EC_T_DWORD              dwSlaveId,
                                                    EC_T_CHAR*              achFileName,
                                                    EC_T_DWORD              dwFileNameLen,
                                                    EC_T_DWORD              dwPassword,
                                                    EC_T_DWORD              dwTimeout       );

static EC_INLINESTART  EC_T_DWORD  ecatFoeUploadReq(EC_T_MBXTFER*           pMbxTfer,
                                                    EC_T_DWORD              dwSlaveId,
                                                    EC_T_CHAR*              achFileName,
                                                    EC_T_DWORD              dwFileNameLen,
                                                    EC_T_DWORD              dwPassword,
                                                    EC_T_DWORD              dwTimeout       )
{
    return emFoeUploadReq(INSTANCE_MASTER_DEFAULT, pMbxTfer, dwSlaveId, achFileName, dwFileNameLen, dwPassword, dwTimeout);
} EC_INLINESTOP

ATECAT_API EC_T_DWORD  emFoeSegmentedUploadReq(EC_T_DWORD     dwInstanceID,
                                                    EC_T_MBXTFER*           pMbxTfer,
                                                    EC_T_DWORD              dwSlaveId,
                                                    EC_T_CHAR*              szFileName,
                                                    EC_T_DWORD              dwFileNameLen,
                                                    EC_T_DWORD              dwFileSize,
                                                    EC_T_DWORD              dwPassword,
                                                    EC_T_DWORD              dwTimeout       );
static EC_INLINESTART  EC_T_DWORD  ecatFoeSegmentedUploadReq(EC_T_MBXTFER* pMbxTfer,
                                                    EC_T_DWORD              dwSlaveId,
                                                    EC_T_CHAR*              szFileName,
                                                    EC_T_DWORD              dwFileNameLen,
                                                    EC_T_DWORD              dwFileSize,
                                                    EC_T_DWORD              dwPassword,
                                                    EC_T_DWORD              dwTimeout       )
{
    return emFoeSegmentedUploadReq(INSTANCE_MASTER_DEFAULT, pMbxTfer, dwSlaveId, szFileName, dwFileNameLen, dwFileSize, dwPassword, dwTimeout);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD  emFoeDownloadReq(EC_T_DWORD              dwInstanceID,
                                                    EC_T_MBXTFER*           pMbxTfer,
                                                    EC_T_DWORD              dwSlaveId,
                                                    EC_T_CHAR*              achFileName,
                                                    EC_T_DWORD              dwFileNameLen,
                                                    EC_T_DWORD              dwPassword,
                                                    EC_T_DWORD              dwTimeout       );

static EC_INLINESTART  EC_T_DWORD  ecatFoeDownloadReq(EC_T_MBXTFER*         pMbxTfer,
                                                    EC_T_DWORD              dwSlaveId,
                                                    EC_T_CHAR*              achFileName,
                                                    EC_T_DWORD              dwFileNameLen,
                                                    EC_T_DWORD              dwPassword,
                                                    EC_T_DWORD              dwTimeout       )
{
    return emFoeDownloadReq(INSTANCE_MASTER_DEFAULT, pMbxTfer, dwSlaveId, achFileName, dwFileNameLen, dwPassword, dwTimeout);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD  emFoeSegmentedDownloadReq(EC_T_DWORD     dwInstanceID,
                                                    EC_T_MBXTFER*           pMbxTfer,
                                                    EC_T_DWORD              dwSlaveId,
                                                    EC_T_CHAR*              szFileName,
                                                    EC_T_DWORD              dwFileNameLen,
                                                    EC_T_DWORD              dwFileSize,
                                                    EC_T_DWORD              dwPassword,
                                                    EC_T_DWORD              dwTimeout       );
static EC_INLINESTART  EC_T_DWORD  ecatFoeSegmentedDownloadReq(EC_T_MBXTFER* pMbxTfer,
                                                    EC_T_DWORD              dwSlaveId,
                                                    EC_T_CHAR*              szFileName,
                                                    EC_T_DWORD              dwFileNameLen,
                                                    EC_T_DWORD              dwFileSize,
                                                    EC_T_DWORD              dwPassword,
                                                    EC_T_DWORD              dwTimeout       )
{
    return emFoeSegmentedDownloadReq(INSTANCE_MASTER_DEFAULT, pMbxTfer, dwSlaveId, szFileName, dwFileNameLen, dwFileSize, dwPassword, dwTimeout);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD  emEoeRegisterEndpoint(EC_T_DWORD         dwInstanceID,
                                                    EC_T_CHAR*              szEoEDrvIdent,
                                                    EC_T_LINK_DRV_DESC*     pLinkDrvDesc    );
static EC_INLINESTART  EC_T_DWORD  ecatEoeRegisterEndpoint(EC_T_CHAR*       szEoEDrvIdent,
                                                    EC_T_LINK_DRV_DESC*     pLinkDrvDesc    )
{
    return emEoeRegisterEndpoint(INSTANCE_MASTER_DEFAULT, szEoEDrvIdent, pLinkDrvDesc);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD  emEoeUnregisterEndpoint(EC_T_DWORD       dwInstanceID,
                                                    EC_T_LINK_DRV_DESC*     pLinkDrvDesc    );
static EC_INLINESTART  EC_T_DWORD  ecatEoeUnregisterEndpoint(EC_T_LINK_DRV_DESC* pLinkDrvDesc)
{
    return emEoeUnregisterEndpoint(INSTANCE_MASTER_DEFAULT, pLinkDrvDesc);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD  emSoeWrite(      EC_T_DWORD              dwInstanceID,
                                                    EC_T_DWORD              dwSlaveId,
                                                    EC_T_BYTE               byDriveNo,
                                                    EC_T_BYTE*              pbyElementFlags,
                                                    EC_T_WORD               wIDN,
                                                    EC_T_BYTE*              pbyData,
                                                    EC_T_DWORD              dwDataLen,
                                                    EC_T_DWORD              dwTimeout        );
static EC_INLINESTART   EC_T_DWORD ecatSoeWrite(    EC_T_DWORD              dwSlaveId,
                                                    EC_T_BYTE               byDriveNo,
                                                    EC_T_BYTE*              pbyElementFlags,
                                                    EC_T_WORD               wIDN,
                                                    EC_T_BYTE*              pbyData,
                                                    EC_T_DWORD              dwDataLen,
                                                    EC_T_DWORD              dwTimeout        )
{
    return emSoeWrite(INSTANCE_MASTER_DEFAULT, dwSlaveId, byDriveNo, pbyElementFlags, wIDN, pbyData, dwDataLen, dwTimeout);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD  emSoeRead(       EC_T_DWORD              dwInstanceID,
                                                    EC_T_DWORD              dwSlaveId,
                                                    EC_T_BYTE               byDriveNo,
                                                    EC_T_BYTE*              pbyElementFlags,
                                                    EC_T_WORD               wIDN,
                                                    EC_T_BYTE*              pbyData,
                                                    EC_T_DWORD              dwDataLen,
                                                    EC_T_DWORD*             pdwOutDataLen,
                                                    EC_T_DWORD              dwTimeout        );
static EC_INLINESTART   EC_T_DWORD ecatSoeRead(     EC_T_DWORD              dwSlaveId,
                                                    EC_T_BYTE               byDriveNo,
                                                    EC_T_BYTE*              pbyElementFlags,
                                                    EC_T_WORD               wIDN,
                                                    EC_T_BYTE*              pbyData,
                                                    EC_T_DWORD              dwDataLen,
                                                    EC_T_DWORD*             pdwOutDataLen,
                                                    EC_T_DWORD              dwTimeout        )
{
    return emSoeRead(INSTANCE_MASTER_DEFAULT, dwSlaveId, byDriveNo, pbyElementFlags, wIDN, pbyData, dwDataLen, pdwOutDataLen, dwTimeout);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD  emSoeAbortProcCmd(EC_T_DWORD             dwInstanceID,
                                                    EC_T_DWORD              dwSlaveId,
                                                    EC_T_BYTE               byDriveNo,
                                                    EC_T_BYTE*              pbyElementFlags,
                                                    EC_T_WORD               wIDN,
                                                    EC_T_DWORD              dwTimeout        );
static EC_INLINESTART   EC_T_DWORD ecatSoeAbortProcCmd(EC_T_DWORD           dwSlaveId,
                                                    EC_T_BYTE               byDriveNo,
                                                    EC_T_BYTE*              pbyElementFlags,
                                                    EC_T_WORD               wIDN,
                                                    EC_T_DWORD              dwTimeout        )
{
    return emSoeAbortProcCmd(INSTANCE_MASTER_DEFAULT, dwSlaveId, byDriveNo, pbyElementFlags, wIDN, dwTimeout);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD  emSoeWriteReq(   EC_T_DWORD              dwInstanceID,
                                                    EC_T_MBXTFER*           pMbxTfer,
                                                    EC_T_DWORD              dwSlaveId,
                                                    EC_T_BYTE               byDriveNo,
                                                    EC_T_BYTE*              pbyElementFlags,
                                                    EC_T_WORD               wIDN,
                                                    EC_T_DWORD              dwTimeout        );
static EC_INLINESTART   EC_T_DWORD ecatSoeWriteReq( EC_T_MBXTFER*           pMbxTfer,
                                                    EC_T_DWORD              dwSlaveId,
                                                    EC_T_BYTE               byDriveNo,
                                                    EC_T_BYTE*              pbyElementFlags,
                                                    EC_T_WORD               wIDN,
                                                    EC_T_DWORD              dwTimeout        )
{
    return emSoeWriteReq(INSTANCE_MASTER_DEFAULT, pMbxTfer, dwSlaveId, byDriveNo, pbyElementFlags, wIDN, dwTimeout);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD  emSoeReadReq(    EC_T_DWORD              dwInstanceID,
                                                    EC_T_MBXTFER*           pMbxTfer,
                                                    EC_T_DWORD              dwSlaveId,
                                                    EC_T_BYTE               byDriveNo,
                                                    EC_T_BYTE*              pbyElementFlags,
                                                    EC_T_WORD               wIDN,
                                                    EC_T_DWORD              dwTimeout        );
static EC_INLINESTART   EC_T_DWORD ecatSoeReadReq(  EC_T_MBXTFER*           pMbxTfer,
                                                    EC_T_DWORD              dwSlaveId,
                                                    EC_T_BYTE               byDriveNo,
                                                    EC_T_BYTE*              pbyElementFlags,
                                                    EC_T_WORD               wIDN,
                                                    EC_T_DWORD              dwTimeout        )
{
    return emSoeReadReq(INSTANCE_MASTER_DEFAULT, pMbxTfer, dwSlaveId, byDriveNo, pbyElementFlags, wIDN, dwTimeout);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD emAoeGetSlaveNetId(EC_T_DWORD             dwInstanceID,
                                                    EC_T_DWORD              dwSlaveId,
                                                    EC_T_AOE_NETID*         poAoeNetId       );
static EC_INLINESTART  EC_T_DWORD ecatAoeGetSlaveNetId(EC_T_DWORD           dwSlaveId,
                                                    EC_T_AOE_NETID*         poAoeNetId       )
{
    return emAoeGetSlaveNetId(INSTANCE_MASTER_DEFAULT, dwSlaveId, poAoeNetId);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD  emAoeRead(       EC_T_DWORD              dwInstanceID,
                                                    EC_T_DWORD              dwSlaveId,
                                                    EC_T_AOE_NETID*         poTargetNetId,
                                                    EC_T_WORD               wTargetPort,
                                                    EC_T_DWORD              dwIndexGroup,
                                                    EC_T_DWORD              dwIndexOffset,
                                                    EC_T_DWORD              dwDataLen,
                                                    EC_T_BYTE*              pbyData,
                                                    EC_T_DWORD*             pdwDataOutLen,
                                                    EC_T_DWORD*             pdwErrorCode,
                                                    EC_T_DWORD*             pdwCmdResult,
                                                    EC_T_DWORD              dwTimeout        );
static EC_INLINESTART  EC_T_DWORD  ecatAoeRead(     EC_T_DWORD              dwSlaveId,
                                                    EC_T_AOE_NETID*         poTargetNetId,
                                                    EC_T_WORD               wTargetPort,
                                                    EC_T_DWORD              dwIndexGroup,
                                                    EC_T_DWORD              dwIndexOffset,
                                                    EC_T_DWORD              dwDataLen,
                                                    EC_T_BYTE*              pbyData,
                                                    EC_T_DWORD*             pdwDataOutLen,
                                                    EC_T_DWORD*             pdwErrorCode,
                                                    EC_T_DWORD*             pdwCmdResult,
                                                    EC_T_DWORD              dwTimeout        )
{
    return emAoeRead(INSTANCE_MASTER_DEFAULT, dwSlaveId, poTargetNetId, wTargetPort, dwIndexGroup, dwIndexOffset, dwDataLen, pbyData, pdwDataOutLen, pdwErrorCode, pdwCmdResult, dwTimeout);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD  emAoeReadReq(    EC_T_DWORD              dwInstanceID,
                                                    EC_T_MBXTFER*           pMbxTfer,
                                                    EC_T_DWORD              dwSlaveId,
                                                    EC_T_AOE_NETID*         poTargetNetId,
                                                    EC_T_WORD               wTargetPort,
                                                    EC_T_DWORD              dwIndexGroup,
                                                    EC_T_DWORD              dwIndexOffset,
                                                    EC_T_DWORD              dwTimeout       );
static EC_INLINESTART  EC_T_DWORD  ecatAoeReadReq(  EC_T_MBXTFER*           pMbxTfer,
                                                    EC_T_DWORD              dwSlaveId,
                                                    EC_T_AOE_NETID*         poTargetNetId,
                                                    EC_T_WORD               wTargetPort,
                                                    EC_T_DWORD              dwIndexGroup,
                                                    EC_T_DWORD              dwIndexOffset,
                                                    EC_T_DWORD              dwTimeout       )
{
    return emAoeReadReq(INSTANCE_MASTER_DEFAULT, pMbxTfer, dwSlaveId, poTargetNetId, wTargetPort, dwIndexGroup, dwIndexOffset, dwTimeout);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD  emAoeWrite(      EC_T_DWORD              dwInstanceID,
                                                    EC_T_DWORD              dwSlaveId,
                                                    EC_T_AOE_NETID*         poTargetNetId,
                                                    EC_T_WORD               wTargetPort,
                                                    EC_T_DWORD              dwIndexGroup,
                                                    EC_T_DWORD              dwIndexOffset,
                                                    EC_T_DWORD              dwDataLen,
                                                    EC_T_BYTE*              pbyData,
                                                    EC_T_DWORD*             pdwErrorCode,
                                                    EC_T_DWORD*             pdwCmdResult,
                                                    EC_T_DWORD              dwTimeout       );
static EC_INLINESTART  EC_T_DWORD  ecatAoeWrite(    EC_T_DWORD              dwSlaveId,
                                                    EC_T_AOE_NETID*         poTargetNetId,
                                                    EC_T_WORD               wTargetPort,
                                                    EC_T_DWORD              dwIndexGroup,
                                                    EC_T_DWORD              dwIndexOffset,
                                                    EC_T_DWORD              dwDataLen,
                                                    EC_T_BYTE*              pbyData,
                                                    EC_T_DWORD*             pdwErrorCode,
                                                    EC_T_DWORD*             pdwCmdResult,
                                                    EC_T_DWORD              dwTimeout       )
{
    return emAoeWrite(INSTANCE_MASTER_DEFAULT, dwSlaveId, poTargetNetId, wTargetPort, dwIndexGroup, dwIndexOffset, dwDataLen, pbyData, pdwErrorCode, pdwCmdResult, dwTimeout);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD emAoeWriteReq(    EC_T_DWORD              dwInstanceID,
                                                    EC_T_MBXTFER*           pMbxTfer,
                                                    EC_T_DWORD              dwSlaveId,
                                                    EC_T_AOE_NETID*         poTargetNetId,
                                                    EC_T_WORD               wTargetPort,
                                                    EC_T_DWORD              dwIndexGroup,
                                                    EC_T_DWORD              dwIndexOffset,
                                                    EC_T_DWORD              dwTimeout       );
static EC_INLINESTART  EC_T_DWORD ecatAoeWriteReq(  EC_T_MBXTFER*           pMbxTfer,
                                                    EC_T_DWORD              dwSlaveId,
                                                    EC_T_AOE_NETID*         poTargetNetId,
                                                    EC_T_WORD               wTargetPort,
                                                    EC_T_DWORD              dwIndexGroup,
                                                    EC_T_DWORD              dwIndexOffset,
                                                    EC_T_DWORD              dwTimeout       )
{
    return emAoeWriteReq(INSTANCE_MASTER_DEFAULT, pMbxTfer, dwSlaveId, poTargetNetId, wTargetPort, dwIndexGroup, dwIndexOffset, dwTimeout);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD  emAoeReadWrite(  EC_T_DWORD              dwInstanceID,
                                                    EC_T_DWORD              dwSlaveId,
                                                    EC_T_AOE_NETID*         poTargetNetId,
                                                    EC_T_WORD               wTargetPort,
                                                    EC_T_DWORD              dwIndexGroup,
                                                    EC_T_DWORD              dwIndexOffset,
                                                    EC_T_DWORD              dwReadDataLen,
                                                    EC_T_DWORD              dwWriteDataLen,
                                                    EC_T_BYTE*              pbyData,
                                                    EC_T_DWORD*             pdwDataOutLen,
                                                    EC_T_DWORD*             pdwErrorCode,
                                                    EC_T_DWORD*             pdwCmdResult,
                                                    EC_T_DWORD              dwTimeout        );
static EC_INLINESTART  EC_T_DWORD  ecatAoeReadWrite(EC_T_DWORD              dwSlaveId,
                                                    EC_T_AOE_NETID*         poTargetNetId,
                                                    EC_T_WORD               wTargetPort,
                                                    EC_T_DWORD              dwIndexGroup,
                                                    EC_T_DWORD              dwIndexOffset,
                                                    EC_T_DWORD              dwReadDataLen,
                                                    EC_T_DWORD              dwWriteDataLen,
                                                    EC_T_BYTE*              pbyData,
                                                    EC_T_DWORD*             pdwDataOutLen,
                                                    EC_T_DWORD*             pdwErrorCode,
                                                    EC_T_DWORD*             pdwCmdResult,
                                                    EC_T_DWORD              dwTimeout        )
{
    return emAoeReadWrite(INSTANCE_MASTER_DEFAULT, dwSlaveId, poTargetNetId, wTargetPort, dwIndexGroup, dwIndexOffset,
        dwReadDataLen, dwWriteDataLen, pbyData, pdwDataOutLen, pdwErrorCode, pdwCmdResult, dwTimeout);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD  emAoeWriteControl(EC_T_DWORD              dwInstanceID,
                                                    EC_T_DWORD              dwSlaveId,
                                                    EC_T_AOE_NETID*         poTargetNetId,
                                                    EC_T_WORD               wTargetPort,
                                                    EC_T_WORD               wAoEState,
                                                    EC_T_WORD               wDeviceState,
                                                    EC_T_DWORD              dwDataLen,
                                                    EC_T_BYTE*              pbyData,
                                                    EC_T_DWORD*             pdwErrorCode,
                                                    EC_T_DWORD*             pdwCmdResult,
                                                    EC_T_DWORD              dwTimeout       );
static EC_INLINESTART  EC_T_DWORD  ecatAoeWriteControl(EC_T_DWORD           dwSlaveId,
                                                    EC_T_AOE_NETID*         poTargetNetId,
                                                    EC_T_WORD               wTargetPort,
                                                    EC_T_WORD               wAoEState,
                                                    EC_T_WORD               wDeviceState,
                                                    EC_T_DWORD              dwDataLen,
                                                    EC_T_BYTE*              pbyData,
                                                    EC_T_DWORD*             pdwErrorCode,
                                                    EC_T_DWORD*             pdwCmdResult,
                                                    EC_T_DWORD              dwTimeout       )
{
    return emAoeWriteControl(INSTANCE_MASTER_DEFAULT, dwSlaveId, poTargetNetId, wTargetPort, wAoEState, wDeviceState, dwDataLen, pbyData, pdwErrorCode, pdwCmdResult, dwTimeout);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD  emVoeRead(       EC_T_DWORD              dwInstanceID,
                                                    EC_T_DWORD              dwSlaveId,
                                                    EC_T_BYTE*              pbyData,
                                                    EC_T_DWORD              dwDataLen,
                                                    EC_T_DWORD*             pdwOutDataLen,
                                                    EC_T_DWORD              dwTimeout       );
static EC_INLINESTART  EC_T_DWORD  ecatVoeRead(          EC_T_DWORD         dwSlaveId,
                                                    EC_T_BYTE*              pbyData,
                                                    EC_T_DWORD              dwDataLen,
                                                    EC_T_DWORD*             pdwOutDataLen,
                                                    EC_T_DWORD              dwTimeout       )
{
    return emVoeRead(INSTANCE_MASTER_DEFAULT, dwSlaveId, pbyData, dwDataLen, pdwOutDataLen, dwTimeout);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD  emVoeWrite(      EC_T_DWORD              dwInstanceID,
                                                    EC_T_DWORD              dwSlaveId,
                                                    EC_T_BYTE*              pbyData,
                                                    EC_T_DWORD              dwDataLen,
                                                    EC_T_DWORD              dwTimeout       );
static EC_INLINESTART  EC_T_DWORD  ecatVoeWrite(    EC_T_DWORD              dwSlaveId,
                                                    EC_T_BYTE*              pbyData,
                                                    EC_T_DWORD              dwDataLen,
                                                    EC_T_DWORD              dwTimeout       )
{
    return emVoeWrite(INSTANCE_MASTER_DEFAULT, dwSlaveId, pbyData, dwDataLen, dwTimeout);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD  emVoeWriteReq(   EC_T_DWORD              dwInstanceID,
                                                    EC_T_MBXTFER*           pMbxTfer,
                                                    EC_T_DWORD              dwSlaveId,
                                                    EC_T_DWORD              dwTimeout       );
static EC_INLINESTART  EC_T_DWORD  ecatVoeWriteReq( EC_T_MBXTFER*           pMbxTfer,
                                                    EC_T_DWORD              dwSlaveId,
                                                    EC_T_DWORD              dwTimeout       )
{
    return emVoeWriteReq(INSTANCE_MASTER_DEFAULT, pMbxTfer, dwSlaveId, dwTimeout);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD  emExecJob(       EC_T_DWORD              dwInstanceID,
                                                    EC_T_USER_JOB           eUserJob,
                                                    EC_T_PVOID              pvParam         );
static EC_INLINESTART  EC_T_DWORD  ecatExecJob(     EC_T_USER_JOB           eUserJob,
                                                    EC_T_PVOID              pvParam         )
{
    return emExecJob(INSTANCE_MASTER_DEFAULT, eUserJob, pvParam);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD  emGetProcessData(EC_T_DWORD              dwInstanceID,
                                                    EC_T_BOOL               bOutputData,
                                                    EC_T_DWORD              dwOffset,
                                                    EC_T_BYTE*              pbyData,
                                                    EC_T_DWORD              dwLength,
                                                    EC_T_DWORD              dwTimeout       );
static EC_INLINESTART  EC_T_DWORD  ecatGetProcessData(EC_T_BOOL             bOutputData,
                                                    EC_T_DWORD              dwOffset,
                                                    EC_T_BYTE*              pbyData,
                                                    EC_T_DWORD              dwLength,
                                                    EC_T_DWORD              dwTimeout       )
{
    return emGetProcessData(INSTANCE_MASTER_DEFAULT, bOutputData, dwOffset, pbyData, dwLength, dwTimeout);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD  emSetProcessData(EC_T_DWORD              dwInstanceID,
                                                    EC_T_BOOL               bOutputData,
                                                    EC_T_DWORD              dwOffset,
                                                    EC_T_BYTE*              pbyData,
                                                    EC_T_DWORD              dwLength,
                                                    EC_T_DWORD              dwTimeout       );
static EC_INLINESTART  EC_T_DWORD  ecatSetProcessData(EC_T_BOOL             bOutputData,
                                                    EC_T_DWORD              dwOffset,
                                                    EC_T_BYTE*              pbyData,
                                                    EC_T_DWORD              dwLength,
                                                    EC_T_DWORD              dwTimeout       )
{
    return emSetProcessData(INSTANCE_MASTER_DEFAULT, bOutputData, dwOffset, pbyData, dwLength, dwTimeout);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD emSetProcessDataBits(EC_T_DWORD           dwInstanceID,
                                                    EC_T_BOOL               bOutputData,
                                                    EC_T_DWORD              dwBitOffsetPd,
                                                    EC_T_BYTE*              pbyDataSrc,
                                                    EC_T_DWORD              dwBitLengthSrc,
                                                    EC_T_DWORD              dwTimeout       );
static EC_INLINESTART  EC_T_DWORD ecatSetProcessDataBits(EC_T_BOOL          bOutputData,
                                                    EC_T_DWORD              dwBitOffsetPd,
                                                    EC_T_BYTE*              pbyDataSrc,
                                                    EC_T_DWORD              dwBitLengthSrc,
                                                    EC_T_DWORD              dwTimeout       )
{
    return emSetProcessDataBits(INSTANCE_MASTER_DEFAULT, bOutputData, dwBitOffsetPd, pbyDataSrc, dwBitLengthSrc, dwTimeout);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD emGetProcessDataBits(EC_T_DWORD           dwInstanceID,
                                                    EC_T_BOOL               bOutputData,
                                                    EC_T_DWORD              dwBitOffsetPd,
                                                    EC_T_BYTE*              pbyDataDst,
                                                    EC_T_DWORD              dwBitLengthDst,
                                                    EC_T_DWORD              dwTimeout       );
static EC_INLINESTART  EC_T_DWORD ecatGetProcessDataBits(EC_T_BOOL          bOutputData,
                                                    EC_T_DWORD              dwBitOffsetPd,
                                                    EC_T_BYTE*              pbyDataDst,
                                                    EC_T_DWORD              dwBitLengthDst,
                                                    EC_T_DWORD              dwTimeout       )
{
    return emGetProcessDataBits(INSTANCE_MASTER_DEFAULT, bOutputData, dwBitOffsetPd, pbyDataDst, dwBitLengthDst, dwTimeout);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD emForceProcessDataBits(EC_T_DWORD         dwInstanceID,
                                                    EC_T_DWORD              dwClientId,
                                                    EC_T_BOOL               bOutputData,
                                                    EC_T_DWORD              dwBitOffsetPd,
                                                    EC_T_WORD               wBitLength,
                                                    EC_T_BYTE*              pbyData,
                                                    EC_T_DWORD              dwTimeout);
static EC_INLINESTART  EC_T_DWORD ecatForceProcessDataBits(EC_T_DWORD       dwClientId,
                                                    EC_T_BOOL               bOutputData,
                                                    EC_T_DWORD              dwBitOffsetPd,
                                                    EC_T_WORD               wBitLength,
                                                    EC_T_BYTE*              pbyData,
                                                    EC_T_DWORD              dwTimeout)
{
    return emForceProcessDataBits(INSTANCE_MASTER_DEFAULT, dwClientId, bOutputData, dwBitOffsetPd, wBitLength, pbyData, dwTimeout);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD emReleaseProcessDataBits(EC_T_DWORD       dwInstanceID,
                                                    EC_T_DWORD              dwClientId,
                                                    EC_T_BOOL               bOutputData,
                                                    EC_T_DWORD              dwBitOffsetPd,
                                                    EC_T_WORD               wBitLength,
                                                    EC_T_DWORD              dwTimeout       );
static EC_INLINESTART  EC_T_DWORD ecatReleaseProcessDataBits(EC_T_DWORD		dwClientId,
                                                    EC_T_BOOL               bOutputData,
                                                    EC_T_DWORD              dwBitOffsetPd,
                                                    EC_T_WORD               wBitLength,
                                                    EC_T_DWORD              dwTimeout       )
{
    return emReleaseProcessDataBits(INSTANCE_MASTER_DEFAULT, dwClientId, bOutputData, dwBitOffsetPd, wBitLength, dwTimeout);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD emReleaseAllProcessDataBits(EC_T_DWORD       dwInstanceID,
                                                    EC_T_DWORD              dwClientId,
                                                    EC_T_DWORD              dwTimeout       );
static EC_INLINESTART  EC_T_DWORD ecatReleaseAllProcessDataBits(EC_T_DWORD	dwClientId,
                                                    EC_T_DWORD              dwTimeout       )
{
    return emReleaseAllProcessDataBits(INSTANCE_MASTER_DEFAULT, dwClientId, dwTimeout);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD  emGetNumConnectedSlaves(EC_T_DWORD       dwInstanceID    );
static EC_INLINESTART  EC_T_DWORD  ecatGetNumConnectedSlaves(EC_T_VOID                      )

{
    return emGetNumConnectedSlaves(INSTANCE_MASTER_DEFAULT);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD  emGetNumConnectedSlavesMain(EC_T_DWORD   dwInstanceID    );
static EC_INLINESTART  EC_T_DWORD  ecatGetNumConnectedSlavesMain(EC_T_VOID                  )
{
    return emGetNumConnectedSlavesMain(INSTANCE_MASTER_DEFAULT);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD  emGetNumConnectedSlavesRed(EC_T_DWORD    dwInstanceID    );
static EC_INLINESTART  EC_T_DWORD  ecatGetNumConnectedSlavesRed(EC_T_VOID                   )
{
    return emGetNumConnectedSlavesRed(INSTANCE_MASTER_DEFAULT);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD  emReadSlaveEEPRom(EC_T_DWORD             dwInstanceID,
                                                    EC_T_BOOL               bFixedAddressing,
                                                    EC_T_WORD               wSlaveAddress,
                                                    EC_T_WORD               wEEPRomStartOffset,
                                                    EC_T_WORD*              pwReadData,
                                                    EC_T_DWORD              dwReadLen,
                                                    EC_T_DWORD*             pdwNumOutData,
                                                    EC_T_DWORD              dwTimeout       );
static EC_INLINESTART  EC_T_DWORD  ecatReadSlaveEEPRom(EC_T_BOOL            bFixedAddressing,
                                                    EC_T_WORD               wSlaveAddress,
                                                    EC_T_WORD               wEEPRomStartOffset,
                                                    EC_T_WORD*              pwReadData,
                                                    EC_T_DWORD              dwReadLen,
                                                    EC_T_DWORD*             pdwNumOutData,
                                                    EC_T_DWORD              dwTimeout       )
{
    return emReadSlaveEEPRom(INSTANCE_MASTER_DEFAULT, bFixedAddressing, wSlaveAddress, wEEPRomStartOffset, pwReadData, dwReadLen, pdwNumOutData, dwTimeout);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD  emWriteSlaveEEPRom(EC_T_DWORD            dwInstanceID,
                                                    EC_T_BOOL               bFixedAddressing,
                                                    EC_T_WORD               wSlaveAddress,
                                                    EC_T_WORD               wEEPRomStartOffset,
                                                    EC_T_WORD*              pwWriteData,
                                                    EC_T_DWORD              dwWriteLen,
                                                    EC_T_DWORD              dwTimeout       );
static EC_INLINESTART  EC_T_DWORD ecatWriteSlaveEEPRom(EC_T_BOOL            bFixedAddressing,
                                                    EC_T_WORD               wSlaveAddress,
                                                    EC_T_WORD               wEEPRomStartOffset,
                                                    EC_T_WORD*              pwWriteData,
                                                    EC_T_DWORD              dwWriteLen,
                                                    EC_T_DWORD              dwTimeout       )
{
    return emWriteSlaveEEPRom(INSTANCE_MASTER_DEFAULT, bFixedAddressing, wSlaveAddress, wEEPRomStartOffset, pwWriteData, dwWriteLen, dwTimeout);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD emReloadSlaveEEPRom(EC_T_DWORD            dwInstanceID,
                                                    EC_T_BOOL               bFixedAddressing,
                                                    EC_T_WORD               wSlaveAddress,
                                                    EC_T_DWORD              dwTimeout       );
static EC_INLINESTART  EC_T_DWORD ecatReloadSlaveEEPRom(EC_T_BOOL           bFixedAddressing,
                                                    EC_T_WORD               wSlaveAddress,
                                                    EC_T_DWORD              dwTimeout       )
{
    return emReloadSlaveEEPRom(INSTANCE_MASTER_DEFAULT, bFixedAddressing, wSlaveAddress, dwTimeout);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD emResetSlaveController(EC_T_DWORD         dwInstanceID,
                                                    EC_T_BOOL               bFixedAddressing,
                                                    EC_T_WORD               wSlaveAddress,
                                                    EC_T_DWORD              dwTimeout       );
static EC_INLINESTART  EC_T_DWORD ecatResetSlaveController(EC_T_BOOL        bFixedAddressing,
                                                    EC_T_WORD               wSlaveAddress,
                                                    EC_T_DWORD              dwTimeout       )
{
    return emResetSlaveController(INSTANCE_MASTER_DEFAULT, bFixedAddressing, wSlaveAddress, dwTimeout);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD emAssignSlaveEEPRom(EC_T_DWORD            dwInstanceID,
                                                    EC_T_BOOL               bFixedAddressing,
                                                    EC_T_WORD               wSlaveAddress,
                                                    EC_T_BOOL               bSlavePDIAccessEnable,
                                                    EC_T_BOOL               bForceAssign,
                                                    EC_T_DWORD              dwTimeout       );
static EC_INLINESTART  EC_T_DWORD ecatAssignSlaveEEPRom(EC_T_BOOL           bFixedAddressing,
                                                    EC_T_WORD               wSlaveAddress,
                                                    EC_T_BOOL               bSlavePDIAccessEnable,
                                                    EC_T_BOOL               bForceAssign,
                                                    EC_T_DWORD              dwTimeout       )
{
    return emAssignSlaveEEPRom(INSTANCE_MASTER_DEFAULT, bFixedAddressing, wSlaveAddress, bSlavePDIAccessEnable, bForceAssign, dwTimeout);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD emActiveSlaveEEPRom(EC_T_DWORD            dwInstanceID,
                                                    EC_T_BOOL               bFixedAddressing,
                                                    EC_T_WORD               wSlaveAddress,
                                                    EC_T_BOOL*              pbSlavePDIAccessActive,
                                                    EC_T_DWORD              dwTimeout       );
static EC_INLINESTART  EC_T_DWORD ecatActiveSlaveEEPRom(EC_T_BOOL           bFixedAddressing,
                                                    EC_T_WORD               wSlaveAddress,
                                                    EC_T_BOOL*              pbSlavePDIAccessActive,
                                                    EC_T_DWORD              dwTimeout       )
{
    return emActiveSlaveEEPRom(INSTANCE_MASTER_DEFAULT, bFixedAddressing, wSlaveAddress, pbSlavePDIAccessActive, dwTimeout);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD emHCAcceptTopoChange(EC_T_DWORD           dwInstanceID    );
static EC_INLINESTART  EC_T_DWORD ecatHCAcceptTopoChange(EC_T_VOID                          )
{
    return emHCAcceptTopoChange(INSTANCE_MASTER_DEFAULT);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD emHCGetNumGroupMembers(EC_T_DWORD         dwInstanceID,
                                                    EC_T_DWORD              dwGroupIndex    );
static EC_INLINESTART  EC_T_DWORD ecatHCGetNumGroupMembers(EC_T_DWORD       dwGroupIndex    )
{
    return emHCGetNumGroupMembers( INSTANCE_MASTER_DEFAULT, dwGroupIndex );
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD emHCGetSlaveIdsOfGroup(EC_T_DWORD         dwInstanceID,
                                                    EC_T_DWORD              dwGroupIndex,
                                                    EC_T_DWORD*             adwSlaveId,
                                                    EC_T_DWORD              dwMaxNumSlaveIds );
static EC_INLINESTART  EC_T_DWORD ecatHCGetSlaveIdsOfGroup(EC_T_DWORD       dwGroupIndex,
                                                    EC_T_DWORD*             adwSlaveId,
                                                    EC_T_DWORD              dwMaxNumSlaveIds )
{
    return emHCGetSlaveIdsOfGroup( INSTANCE_MASTER_DEFAULT, dwGroupIndex, adwSlaveId, dwMaxNumSlaveIds );
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD emSetSlavePortState(EC_T_DWORD            dwInstanceID,
                                                    EC_T_DWORD              dwSlaveId,
                                                    EC_T_WORD               wPort,
                                                    EC_T_BOOL               bClose,
                                                    EC_T_BOOL               bForce,
                                                    EC_T_DWORD              dwTimeout       );
static EC_INLINESTART  EC_T_DWORD ecatSetSlavePortState(EC_T_DWORD          dwSlaveId,
                                                    EC_T_WORD               wPort,
                                                    EC_T_BOOL               bClose,
                                                    EC_T_BOOL               bForce,
                                                    EC_T_DWORD              dwTimeout       )
{
    return emSetSlavePortState(INSTANCE_MASTER_DEFAULT, dwSlaveId, wPort, bClose, bForce, dwTimeout);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD  emSlaveSerializeMbxTfers(EC_T_DWORD      dwInstanceID,
                                                    EC_T_DWORD              dwSlaveId       );
static EC_INLINESTART  EC_T_DWORD  ecatSlaveSerializeMbxTfers(EC_T_DWORD    dwSlaveId       )
{
    return emSlaveSerializeMbxTfers(INSTANCE_MASTER_DEFAULT, dwSlaveId);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD  emSlaveParallelMbxTfers(EC_T_DWORD       dwInstanceID,
                                                    EC_T_DWORD              dwSlaveId       );
static EC_INLINESTART  EC_T_DWORD  ecatSlaveParallelMbxTfers(EC_T_DWORD     dwSlaveId       )
{
    return emSlaveParallelMbxTfers(INSTANCE_MASTER_DEFAULT, dwSlaveId);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD emRegisterClient( EC_T_DWORD              dwInstanceID,
                                                    EC_PF_NOTIFY            pfnNotify,
                                                    EC_T_VOID*              pCallerData,
                                                    EC_T_REGISTERRESULTS*   pRegResults     );
static EC_INLINESTART  EC_T_DWORD ecatRegisterClient(EC_PF_NOTIFY           pfnNotify,
                                                    EC_T_VOID*              pCallerData,
                                                    EC_T_REGISTERRESULTS*   pRegResults     )
{
    return emRegisterClient(INSTANCE_MASTER_DEFAULT, pfnNotify, pCallerData, pRegResults);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD emUnregisterClient(EC_T_DWORD             dwInstanceID,
                                                    EC_T_DWORD              dwClntId        );
static EC_INLINESTART  EC_T_DWORD ecatUnregisterClient(EC_T_DWORD           dwClntId        )
{
    return emUnregisterClient(INSTANCE_MASTER_DEFAULT, dwClntId);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD emDcEnable(       EC_T_DWORD              dwInstanceID    );
static EC_INLINESTART  EC_T_DWORD ecatDcEnable(     EC_T_VOID                               )
{
    return emDcEnable(INSTANCE_MASTER_DEFAULT);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD emDcDisable(      EC_T_DWORD              dwInstanceID    );
static EC_INLINESTART  EC_T_DWORD ecatDcDisable(    EC_T_VOID                               )
{
    return emDcDisable(INSTANCE_MASTER_DEFAULT);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD emDcIsEnabled(    EC_T_DWORD              dwInstanceID,
                                                    EC_T_BOOL*              pbDcIsEnabled);
static EC_INLINESTART  EC_T_DWORD ecatDcIsEnabled(  EC_T_BOOL*           pbDcIsEnabled)
{
    return emDcIsEnabled(INSTANCE_MASTER_DEFAULT, pbDcIsEnabled);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD emDcConfigure(    EC_T_DWORD              dwInstanceID,
                                                    struct _EC_T_DC_CONFIGURE* pDcConfigure );
static EC_INLINESTART  EC_T_DWORD ecatDcConfigure(  struct _EC_T_DC_CONFIGURE* pDcConfigure )
{
    return emDcConfigure(INSTANCE_MASTER_DEFAULT, pDcConfigure);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD emDcContDelayCompEnable(EC_T_DWORD        dwInstanceID    );
static EC_INLINESTART  EC_T_DWORD ecatDcContDelayCompEnable(EC_T_VOID                       )
{
    return emDcContDelayCompEnable(INSTANCE_MASTER_DEFAULT);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD emDcContDelayCompDisable(EC_T_DWORD       dwInstanceID    );
static EC_INLINESTART  EC_T_DWORD ecatDcContDelayCompDisable(EC_T_VOID                      )
{
    return emDcContDelayCompDisable(INSTANCE_MASTER_DEFAULT);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD emDcmConfigure(   EC_T_DWORD              dwInstanceID,
                                                    struct _EC_T_DCM_CONFIG* pDcmConfig,
                                                    EC_T_DWORD              dwInSyncTimeout );
static EC_INLINESTART  EC_T_DWORD ecatDcmConfigure(struct _EC_T_DCM_CONFIG* pDcmConfig,
                                                    EC_T_DWORD              dwInSyncTimeout )
{
    return emDcmConfigure(INSTANCE_MASTER_DEFAULT, pDcmConfig, dwInSyncTimeout);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD emDcmGetStatus(   EC_T_DWORD              dwInstanceID,
                                                    EC_T_DWORD*             pdwErrorCode,
                                                    EC_T_INT*               pnDiffCur,
                                                    EC_T_INT*               pnDiffAvg,
                                                    EC_T_INT*               pnDiffMax       );
static EC_INLINESTART  EC_T_DWORD ecatDcmGetStatus( EC_T_DWORD*             pdwErrorCode,
                                                    EC_T_INT*               pnDiffCur,
                                                    EC_T_INT*               pnDiffAvg,
                                                    EC_T_INT*               pnDiffMax       )
{
    return emDcmGetStatus(INSTANCE_MASTER_DEFAULT, pdwErrorCode, pnDiffCur, pnDiffAvg, pnDiffMax);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD emDcxGetStatus(   EC_T_DWORD              dwInstanceID,
                                                    EC_T_DWORD*             pdwErrorCode,
                                                    EC_T_INT*               pnDiffCur,
                                                    EC_T_INT*               pnDiffAvg,
                                                    EC_T_INT*               pnDiffMax,
                                                    EC_T_INT64*             pnTimeStampDiff);

static EC_INLINESTART  EC_T_DWORD ecatDcxGetStatus( EC_T_DWORD*             pdwErrorCode,
                                                    EC_T_INT*               pnDiffCur,
                                                    EC_T_INT*               pnDiffAvg,
                                                    EC_T_INT*               pnDiffMax,
                                                    EC_T_INT64*             pnTimeStampDiff)
{
    return emDcxGetStatus(INSTANCE_MASTER_DEFAULT, pdwErrorCode, pnDiffCur, pnDiffAvg, pnDiffMax, pnTimeStampDiff);
}

ATECAT_API             EC_T_DWORD emDcmResetStatus(  EC_T_DWORD             dwInstanceID    );
static EC_INLINESTART  EC_T_DWORD ecatDcmResetStatus(EC_T_VOID                              )
{
    return emDcmResetStatus(INSTANCE_MASTER_DEFAULT);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD emDcmGetBusShiftConfigured(  EC_T_DWORD             dwInstanceID,
                                                               EC_T_BOOL*             pbBusShiftConfigured);
static EC_INLINESTART  EC_T_DWORD ecatDcmGetBusShiftConfigured(EC_T_BOOL*             pbBusShiftConfigured)
{
    return emDcmGetBusShiftConfigured(INSTANCE_MASTER_DEFAULT, pbBusShiftConfigured);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD emDcmGetLog(      EC_T_DWORD              dwInstanceID,
                                                    EC_T_CHAR**             pszLog          );
static EC_INLINESTART  EC_T_DWORD ecatDcmGetLog(    EC_T_CHAR**             pszLog          )
{
    return emDcmGetLog(INSTANCE_MASTER_DEFAULT, pszLog);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD emDcmShowStatus(  EC_T_DWORD              dwInstanceID    );
static EC_INLINESTART  EC_T_DWORD ecatDcmShowStatus(EC_T_VOID                               )
{
    return emDcmShowStatus(INSTANCE_MASTER_DEFAULT);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD emDcmGetAdjust(   EC_T_DWORD              dwInstanceID,
                                                    EC_T_INT*               pnAdjustPermil  );
static EC_INLINESTART  EC_T_DWORD ecatDcmGetAdjust( EC_T_INT*               pnAdjustPermil  )
{
    return emDcmGetAdjust(INSTANCE_MASTER_DEFAULT, pnAdjustPermil);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD emGetSlaveInfo(   EC_T_DWORD              dwInstanceID,
                                                    EC_T_BOOL               bFixedAddressing,
                                                    EC_T_WORD               wSlaveAddress,
                                                    EC_T_GET_SLAVE_INFO*    pGetSlaveInfo   );
static EC_INLINESTART  EC_T_DWORD ecatGetSlaveInfo( EC_T_BOOL               bFixedAddressing,
                                                    EC_T_WORD               wSlaveAddress,
                                                    EC_T_GET_SLAVE_INFO*    pGetSlaveInfo   )
{
    return emGetSlaveInfo(INSTANCE_MASTER_DEFAULT, bFixedAddressing, wSlaveAddress, pGetSlaveInfo);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD emGetCfgSlaveInfo(EC_T_DWORD              dwInstanceID,
                                                    EC_T_BOOL               bStationAddress,
                                                    EC_T_WORD               wSlaveAddress,
                                                    EC_T_CFG_SLAVE_INFO*    pSlaveInfo      );
static EC_INLINESTART  EC_T_DWORD ecatGetCfgSlaveInfo(EC_T_BOOL             bStationAddress,
                                                    EC_T_WORD               wSlaveAddress,
                                                    EC_T_CFG_SLAVE_INFO*    pSlaveInfo      )
{
    return emGetCfgSlaveInfo(INSTANCE_MASTER_DEFAULT, bStationAddress, wSlaveAddress, pSlaveInfo);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD emGetBusSlaveInfo(EC_T_DWORD              dwInstanceID,
                                                    EC_T_BOOL               bStationAddress,
                                                    EC_T_WORD               wSlaveAddress,
                                                    EC_T_BUS_SLAVE_INFO*    pSlaveInfo      );
static EC_INLINESTART  EC_T_DWORD  ecatGetBusSlaveInfo(EC_T_BOOL            bStationAddress,
                                                    EC_T_WORD               wSlaveAddress,
                                                    EC_T_BUS_SLAVE_INFO*    pSlaveInfo      )
{
    return emGetBusSlaveInfo(INSTANCE_MASTER_DEFAULT, bStationAddress, wSlaveAddress, pSlaveInfo);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD emGetSlaveInpVarInfoNumOf(EC_T_DWORD      dwInstanceID,
                                                    EC_T_BOOL               bFixedAddressing,
                                                    EC_T_WORD               wSlaveAddress,
                                                    EC_T_WORD*              pwSlaveInpVarInfoNumOf);
static EC_INLINESTART  EC_T_DWORD ecatGetSlaveInpVarInfoNumOf(EC_T_BOOL     bFixedAddressing,
                                                    EC_T_WORD               wSlaveAddress,
                                                    EC_T_WORD*              pwSlaveInpVarInfoNumOf)
{
    return emGetSlaveInpVarInfoNumOf(INSTANCE_MASTER_DEFAULT, bFixedAddressing, wSlaveAddress, pwSlaveInpVarInfoNumOf);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD emGetSlaveOutpVarInfoNumOf(EC_T_DWORD     dwInstanceID,
                                                    EC_T_BOOL               bFixedAddressing,
                                                    EC_T_WORD               wSlaveAddress,
                                                    EC_T_WORD*              pwSlaveOutpVarInfoNumOf );
static EC_INLINESTART  EC_T_DWORD ecatGetSlaveOutpVarInfoNumOf(EC_T_BOOL    bFixedAddressing,
                                                    EC_T_WORD               wSlaveAddress,
                                                    EC_T_WORD*              pwSlaveOutpVarInfoNumOf)
{
    return emGetSlaveOutpVarInfoNumOf(INSTANCE_MASTER_DEFAULT, bFixedAddressing, wSlaveAddress, pwSlaveOutpVarInfoNumOf);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD emGetSlaveInpVarInfo(EC_T_DWORD           dwInstanceID,
                                                    EC_T_BOOL               bFixedAddressing,
                                                    EC_T_WORD               wSlaveAddress,
                                                    EC_T_WORD               wNumOfVarsToRead,
                                                    EC_T_PROCESS_VAR_INFO*  pSlaveProcVarInfoEntries,
                                                    EC_T_WORD*              pwReadEntries    );
static EC_INLINESTART  EC_T_DWORD ecatGetSlaveInpVarInfo(EC_T_BOOL          bFixedAddressing,
                                                    EC_T_WORD               wSlaveAddress,
                                                    EC_T_WORD               wNumOfVarsToRead,
                                                    EC_T_PROCESS_VAR_INFO*  pSlaveProcVarInfoEntries,
                                                    EC_T_WORD*              pwReadEntries    )
{
    return emGetSlaveInpVarInfo(INSTANCE_MASTER_DEFAULT, bFixedAddressing, wSlaveAddress, wNumOfVarsToRead, pSlaveProcVarInfoEntries, pwReadEntries);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD emGetSlaveInpVarInfoEx(EC_T_DWORD         dwInstanceID,
                                                    EC_T_BOOL               bFixedAddressing,
                                                    EC_T_WORD               wSlaveAddress,
                                                    EC_T_WORD               wNumOfVarsToRead,
                                                    EC_T_PROCESS_VAR_INFO_EX* pSlaveProcVarInfoEntries,
                                                    EC_T_WORD*              pwReadEntries    );
static EC_INLINESTART  EC_T_DWORD ecatGetSlaveInpVarInfoEx(EC_T_BOOL        bFixedAddressing,
                                                    EC_T_WORD               wSlaveAddress,
                                                    EC_T_WORD               wNumOfVarsToRead,
                                                    EC_T_PROCESS_VAR_INFO_EX* pSlaveProcVarInfoEntries,
                                                    EC_T_WORD*              pwReadEntries    )
{
    return emGetSlaveInpVarInfoEx(INSTANCE_MASTER_DEFAULT, bFixedAddressing, wSlaveAddress, wNumOfVarsToRead, pSlaveProcVarInfoEntries, pwReadEntries);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD emGetSlaveOutpVarInfo(EC_T_DWORD          dwInstanceID,
                                                    EC_T_BOOL               bFixedAddressing,
                                                    EC_T_WORD               wSlaveAddress,
                                                    EC_T_WORD               wNumOfVarsToRead,
                                                    EC_T_PROCESS_VAR_INFO*  pSlaveProcVarInfoEntries,
                                                    EC_T_WORD*              pwReadEntries    );
static EC_INLINESTART  EC_T_DWORD ecatGetSlaveOutpVarInfo(EC_T_BOOL         bFixedAddressing,
                                                    EC_T_WORD               wSlaveAddress,
                                                    EC_T_WORD               wNumOfVarsToRead,
                                                    EC_T_PROCESS_VAR_INFO*  pSlaveProcVarInfoEntries,
                                                    EC_T_WORD*              pwReadEntries    )
{
    return emGetSlaveOutpVarInfo(INSTANCE_MASTER_DEFAULT, bFixedAddressing, wSlaveAddress, wNumOfVarsToRead, pSlaveProcVarInfoEntries, pwReadEntries);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD emGetSlaveOutpVarInfoEx(EC_T_DWORD        dwInstanceID,
                                                    EC_T_BOOL               bFixedAddressing,
                                                    EC_T_WORD               wSlaveAddress,
                                                    EC_T_WORD               wNumOfVarsToRead,
                                                    EC_T_PROCESS_VAR_INFO_EX* pSlaveProcVarInfoEntries,
                                                    EC_T_WORD*              pwReadEntries    );
static EC_INLINESTART  EC_T_DWORD ecatGetSlaveOutpVarInfoEx(EC_T_BOOL       bFixedAddressing,
                                                    EC_T_WORD               wSlaveAddress,
                                                    EC_T_WORD               wNumOfVarsToRead,
                                                    EC_T_PROCESS_VAR_INFO_EX* pSlaveProcVarInfoEntries,
                                                    EC_T_WORD*              pwReadEntries    )
{
    return emGetSlaveOutpVarInfoEx(INSTANCE_MASTER_DEFAULT, bFixedAddressing, wSlaveAddress, wNumOfVarsToRead, pSlaveProcVarInfoEntries, pwReadEntries);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD emFindOutpVarByName(EC_T_DWORD            dwInstanceID,
                                                    EC_T_CHAR*              szVariableName,
                                                    EC_T_PROCESS_VAR_INFO*  pSlaveOutpVarInfo);
static EC_INLINESTART  EC_T_DWORD ecatFindOutpVarByName(EC_T_CHAR*          szVariableName,
                                                    EC_T_PROCESS_VAR_INFO*  pProcessVarInfoEntry)
{
    return emFindOutpVarByName(INSTANCE_MASTER_DEFAULT, szVariableName, pProcessVarInfoEntry);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD emFindOutpVarByNameEx(EC_T_DWORD          dwInstanceID,
                                                    EC_T_CHAR*              szVariableName,
                                                    EC_T_PROCESS_VAR_INFO_EX* pProcessVarInfoEntry);
static EC_INLINESTART  EC_T_DWORD ecatFindOutpVarByNameEx(EC_T_CHAR*        szVariableName,
                                                    EC_T_PROCESS_VAR_INFO_EX* pProcessVarInfoEntry)
{
    return emFindOutpVarByNameEx(INSTANCE_MASTER_DEFAULT, szVariableName, pProcessVarInfoEntry);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD emFindInpVarByName(EC_T_DWORD             dwInstanceID,
                                                    EC_T_CHAR*              szVariableName,
                                                    EC_T_PROCESS_VAR_INFO*  pProcessVarInfoEntry);
static EC_INLINESTART  EC_T_DWORD ecatFindInpVarByName(EC_T_CHAR*           szVariableName,
                                                    EC_T_PROCESS_VAR_INFO*  pProcessVarInfoEntry)
{
    return emFindInpVarByName(INSTANCE_MASTER_DEFAULT, szVariableName, pProcessVarInfoEntry);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD emFindInpVarByNameEx(         EC_T_DWORD              dwInstanceID,
                                                    EC_T_CHAR*              szVariableName,
                                                    EC_T_PROCESS_VAR_INFO_EX* pProcessVarInfoEntry);
static EC_INLINESTART  EC_T_DWORD ecatFindInpVarByNameEx(EC_T_CHAR*              szVariableName,
                                                    EC_T_PROCESS_VAR_INFO_EX* pProcessVarInfoEntry)
{
    return emFindInpVarByNameEx(INSTANCE_MASTER_DEFAULT, szVariableName, pProcessVarInfoEntry);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD emEthDbgMsg(      EC_T_DWORD              dwInstanceID,
                                                    EC_T_BYTE               byEthTypeByte0,
                                                    EC_T_BYTE               byEthTypeByte1,
                                                    EC_T_CHAR*              szMsg           );
static EC_INLINESTART  EC_T_DWORD ecatEthDbgMsg(    EC_T_BYTE               byEthTypeByte0,
                                                    EC_T_BYTE               byEthTypeByte1,
                                                    EC_T_CHAR*              szMsg           )
{
    return emEthDbgMsg(INSTANCE_MASTER_DEFAULT, byEthTypeByte0, byEthTypeByte1, szMsg);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD emBlockNode(      EC_T_DWORD              dwInstanceID,
                                                    EC_T_SB_MISMATCH_DESC*  pMisMatch,
                                                    EC_T_DWORD              dwTimeout       );
static EC_INLINESTART  EC_T_DWORD ecatBlockNode(    EC_T_SB_MISMATCH_DESC*  pMisMatch,
                                                    EC_T_DWORD              dwTimeout       )
{
    return emBlockNode(INSTANCE_MASTER_DEFAULT, pMisMatch, dwTimeout);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD emOpenBlockedPorts(EC_T_DWORD             dwInstanceID,
                                                    EC_T_DWORD              dwTimeout       );
static EC_INLINESTART  EC_T_DWORD ecatOpenBlockedPorts(EC_T_DWORD           dwTimeout       )
{
    return emOpenBlockedPorts(INSTANCE_MASTER_DEFAULT, dwTimeout);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD emForceTopologyChange(EC_T_DWORD          dwInstanceID    );
static EC_INLINESTART  EC_T_DWORD ecatForceTopologyChange(EC_T_VOID                         )
{
    return emForceTopologyChange(INSTANCE_MASTER_DEFAULT);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD emIsTopologyChangeDetected(EC_T_DWORD     dwInstanceID,
                                                             EC_T_BOOL*     pbTopologyChangeDetected);
static EC_INLINESTART  EC_T_DWORD ecatIsTopologyChangeDetected(EC_T_BOOL*   pbTopologyChangeDetected)
{
    return emIsTopologyChangeDetected(INSTANCE_MASTER_DEFAULT, pbTopologyChangeDetected);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD emIsSlavePresent( EC_T_DWORD              dwInstanceID,
                                                    EC_T_DWORD              dwSlaveId,
                                                    EC_T_BOOL*              pbPresence      );
static EC_INLINESTART  EC_T_DWORD ecatIsSlavePresent(EC_T_DWORD             dwSlaveId,
                                                    EC_T_BOOL*              pbPresence      )
{
    return emIsSlavePresent(INSTANCE_MASTER_DEFAULT, dwSlaveId, pbPresence);
} EC_INLINESTOP

ATECAT_API             EC_PTS_STATE emPassThroughSrvGetStatus(EC_T_DWORD    dwInstanceID    );
static EC_INLINESTART  EC_PTS_STATE ecatPassThroughSrvGetStatus(EC_T_VOID                   )
{
    return emPassThroughSrvGetStatus(INSTANCE_MASTER_DEFAULT);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD emPassThroughSrvStart(EC_T_DWORD          dwInstanceID,
                                                    EC_T_PTS_SRV_START_PARMS* poPtsStartParams,
                                                    EC_T_DWORD              dwTimeout       );
static EC_INLINESTART  EC_T_DWORD ecatPassThroughSrvStart(EC_T_PTS_SRV_START_PARMS* poPtsStartParams,
                                                    EC_T_DWORD              dwTimeout       )
{
    return emPassThroughSrvStart(INSTANCE_MASTER_DEFAULT, poPtsStartParams, dwTimeout);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD emPassThroughSrvStop(EC_T_DWORD           dwInstanceID,
                                                    EC_T_DWORD              dwTimeout       );
static EC_INLINESTART  EC_T_DWORD ecatPassThroughSrvStop(EC_T_DWORD         dwTimeout       )
{
    return emPassThroughSrvStop(INSTANCE_MASTER_DEFAULT, dwTimeout);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD emPassThroughSrvEnable(EC_T_DWORD         dwInstanceID,
                                                    EC_T_DWORD              dwTimeout       );
static EC_INLINESTART  EC_T_DWORD ecatPassThroughSrvEnable(EC_T_DWORD       dwTimeout       )
{
    return emPassThroughSrvEnable(INSTANCE_MASTER_DEFAULT, dwTimeout);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD emPassThroughSrvDisable(EC_T_DWORD        dwInstanceID,
                                                    EC_T_DWORD              dwTimeout       );
static EC_INLINESTART  EC_T_DWORD ecatPassThroughSrvDisable(EC_T_DWORD      dwTimeout       )
{
    return emPassThroughSrvDisable(INSTANCE_MASTER_DEFAULT, dwTimeout);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD emAdsAdapterStart(EC_T_DWORD              dwInstanceID,
                                                    EC_T_ADS_ADAPTER_START_PARMS* poStartParams,
                                                    EC_T_DWORD              dwTimeout       );
static EC_INLINESTART  EC_T_DWORD ecatAdsAdapterStart(EC_T_ADS_ADAPTER_START_PARMS* poStartParams,
                                                    EC_T_DWORD              dwTimeout       )
{
    return emAdsAdapterStart(INSTANCE_MASTER_DEFAULT, poStartParams, dwTimeout);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD emAdsAdapterStop(EC_T_DWORD               dwInstanceID,
                                                    EC_T_DWORD              dwTimeout       );
static EC_INLINESTART  EC_T_DWORD ecatAdsAdapterStop(EC_T_DWORD             dwTimeout       )
{
    return emAdsAdapterStop(INSTANCE_MASTER_DEFAULT, dwTimeout);
} EC_INLINESTOP

ATECAT_API             EC_T_BYTE* emGetProcessImageInputPtr(EC_T_DWORD      dwInstanceID    );
static EC_INLINESTART  EC_T_BYTE* ecatGetProcessImageInputPtr(EC_T_VOID                     )
{
    return emGetProcessImageInputPtr(INSTANCE_MASTER_DEFAULT);
} EC_INLINESTOP

ATECAT_API             EC_T_BYTE* emGetProcessImageOutputPtr(EC_T_DWORD     dwInstanceID    );
static EC_INLINESTART  EC_T_BYTE* ecatGetProcessImageOutputPtr(EC_T_VOID                    )
{
    return emGetProcessImageOutputPtr(INSTANCE_MASTER_DEFAULT);
} EC_INLINESTOP

ATECAT_API             EC_T_BYTE* emGetDiagnosisImagePtr(EC_T_DWORD         dwInstanceID    );
static EC_INLINESTART  EC_T_BYTE* ecatGetDiagnosisImagePtr(EC_T_VOID                        )
{
    return emGetDiagnosisImagePtr(INSTANCE_MASTER_DEFAULT);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD emNotifyApp(      EC_T_DWORD              dwInstanceID,
                                                    EC_T_DWORD              dwCode,
                                                    EC_T_NOTIFYPARMS*       pParms          );
static EC_INLINESTART  EC_T_DWORD ecatNotifyApp(    EC_T_DWORD              dwCode,
                                                    EC_T_NOTIFYPARMS*       pParms          )
{
    return emNotifyApp(INSTANCE_MASTER_DEFAULT, dwCode, pParms);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD emLogFrameEnable( EC_T_DWORD              dwInstanceID,
                                                    EC_T_PFLOGFRAME_CB      pvLogFrameCallBack,
                                                    EC_T_VOID*              pvContext       );
static EC_INLINESTART  EC_T_DWORD ecatLogFrameEnable(EC_T_PFLOGFRAME_CB     pvLogFrameCallBack,
                                                    EC_T_VOID*              pvContext       )
{
    return emLogFrameEnable(INSTANCE_MASTER_DEFAULT, pvLogFrameCallBack, pvContext);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD emLogFrameDisable(EC_T_DWORD              dwInstanceID    );
static EC_INLINESTART  EC_T_DWORD ecatLogFrameDisable(EC_T_VOID                             )
{
    return emLogFrameDisable(INSTANCE_MASTER_DEFAULT);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD emGetSrcMacAddress(EC_T_DWORD             dwInstanceID,
                                                    ETHERNET_ADDRESS*       pMacSrc         );
static EC_INLINESTART  EC_T_DWORD ecatGetSrcMacAddress(ETHERNET_ADDRESS*    pMacSrc         )
{
    return emGetSrcMacAddress(INSTANCE_MASTER_DEFAULT, pMacSrc);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD emSetLicenseKey(  EC_T_DWORD              dwInstanceID,
                                                    EC_T_CHAR*              pszLicenseKey   );
static EC_INLINESTART  EC_T_DWORD ecatSetLicenseKey(EC_T_CHAR*              pszLicenseKey   )
{
    return emSetLicenseKey(INSTANCE_MASTER_DEFAULT, pszLicenseKey);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD emGetMasterProperties(EC_T_DWORD          dwInstanceID,
                                                    EC_T_DWORD*             pdwMasterPropNumEntries,
                                                    EC_T_MASTER_PROP_DESC** ppaMasterPropEntries);
static EC_INLINESTART  EC_T_DWORD ecatGetMasterProperties(EC_T_DWORD*       pdwMasterPropNumEntries,
                                                    EC_T_MASTER_PROP_DESC** ppaMasterPropEntries)
{
    return emGetMasterProperties(INSTANCE_MASTER_DEFAULT, pdwMasterPropNumEntries, ppaMasterPropEntries);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD emGetVersion(     EC_T_DWORD              dwInstanceID,
                                                    EC_T_DWORD*             pdwVersion      );
static EC_INLINESTART  EC_T_DWORD ecatGetVersion(   EC_T_DWORD*             pdwVersion      )
{
    return emGetVersion(INSTANCE_MASTER_DEFAULT, pdwVersion);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD emTraceDataConfig(    EC_T_DWORD          dwInstanceID,
                                                        EC_T_WORD           wTraceDataSize  );
static EC_INLINESTART  EC_T_DWORD ecatTraceDataConfig(  EC_T_WORD           wTraceDataSize  )
{
    return emTraceDataConfig(INSTANCE_MASTER_DEFAULT, wTraceDataSize);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD emTraceDataGetInfo(   EC_T_DWORD          dwInstanceID,
                                                        EC_T_TRACE_DATA_INFO* pTraceDataInfo);
static EC_INLINESTART  EC_T_DWORD ecatTraceDataGetInfo( EC_T_TRACE_DATA_INFO* pTraceDataInfo)
{
    return emTraceDataGetInfo(INSTANCE_MASTER_DEFAULT, pTraceDataInfo);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD emFastModeInit(       EC_T_DWORD          dwInstanceID);
static EC_INLINESTART  EC_T_DWORD ecatFastModeInit(EC_T_VOID)
{
    return emFastModeInit(INSTANCE_MASTER_DEFAULT);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD emFastSendAllCycFrames(EC_T_DWORD         dwInstanceID);
static EC_INLINESTART  EC_T_DWORD ecatFastSendAllCycFrames(EC_T_VOID)
{
    return emFastSendAllCycFrames(INSTANCE_MASTER_DEFAULT);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD  emFastProcessAllRxFrames(EC_T_DWORD      dwInstanceID,
                                                            EC_T_BOOL*      pbAreAllCycFramesProcessed);
static EC_INLINESTART  EC_T_DWORD ecatFastProcessAllRxFrames(EC_T_BOOL*     pbAreAllCycFramesProcessed)
{
    return emFastProcessAllRxFrames(INSTANCE_MASTER_DEFAULT, pbAreAllCycFramesProcessed);
} EC_INLINESTOP


/*-INLINE METHODS------------------------------------------------------------*/
static EC_INLINESTART const EC_T_CHAR* ecatStateToStr(EC_T_STATE eState)
{
    return eState == eEcatState_INIT ? "INIT":
     (eState == eEcatState_PREOP ? "PREOP":
      (eState == eEcatState_SAFEOP ? "SAFEOP":
       (eState == eEcatState_OP ? "OP":
        (eState == eEcatState_BOOTSTRAP ? "BOOTSTRAP":
         (eState == eEcatState_UNKNOWN ? "UNKNOWN":
          "STATE INVALID")))));
} EC_INLINESTOP
#define ecatDeviceStateText(eState)     ecatGetText(((EC_T_DWORD)(EC_TXT_DEVICE_STATE_BASE+(eState))))

static EC_INLINESTART const EC_T_CHAR* ecatSlaveStateText(EC_T_WORD nState)
{
    if (nState & DEVICE_STATE_ERROR)
        return ecatGetText(((EC_T_DWORD)(EC_TXT_SLAVE_STATE_ERROR_BASE+(nState & DEVICE_STATE_MASK))));
    else
        return ecatGetText(((EC_T_DWORD)(EC_TXT_DEVICE_STATE_BASE+(nState & DEVICE_STATE_MASK))));
} EC_INLINESTOP


static EC_INLINESTART EC_T_DWORD emIoCtl(
    EC_T_DWORD      dwInstanceID,
    EC_T_DWORD      dwCode,                         /*< in  see EC_IOCTL_... */
    EC_T_VOID*      pbyInBuf,                       /*< in  input data buffer */
    EC_T_DWORD      dwInBufSize,                    /*< in  size of input data buffer in byte */
    EC_T_VOID*      pbyOutBuf,                      /*< out output data buffer */
    EC_T_DWORD      dwOutBufSize,                   /*< in  size of output data buffer in byte */
    EC_T_DWORD*     pdwNumOutData                   /*< out number of output data bytes stored in output data buffer */                                           
                                           ) 
{
    EC_T_IOCTLPARMS oIoCtlParms;
    EC_T_DWORD      dwNumOutData;

    oIoCtlParms.pbyInBuf        = (EC_T_BYTE*)pbyInBuf;
    oIoCtlParms.dwInBufSize     = dwInBufSize;
    oIoCtlParms.pbyOutBuf       = (EC_T_BYTE*)pbyOutBuf;
    oIoCtlParms.dwOutBufSize    = dwOutBufSize;
    oIoCtlParms.pdwNumOutData   = (EC_NULL != pdwNumOutData)?pdwNumOutData:&dwNumOutData;
    return emIoControl(dwInstanceID, dwCode, &oIoCtlParms);
} EC_INLINESTOP
static EC_INLINESTART  EC_T_DWORD ecatIoCtl(
    EC_T_DWORD      dwCode,                         /*< in  see EC_IOCTL_... */
    EC_T_VOID*      pbyInBuf,                       /*< in  input data buffer */
    EC_T_DWORD      dwInBufSize,                    /*< in  size of input data buffer in byte */
    EC_T_VOID*      pbyOutBuf,                      /*< out output data buffer */
    EC_T_DWORD      dwOutBufSize,                   /*< in  size of output data buffer in byte */
    EC_T_DWORD*     pdwNumOutData                   /*< out number of output data bytes stored in output data buffer */                                           
                                           ) 
{
    return emIoCtl(INSTANCE_MASTER_DEFAULT, dwCode, pbyInBuf, dwInBufSize, pbyOutBuf, dwOutBufSize, pdwNumOutData);
} EC_INLINESTOP

static EC_INLINESTART  EC_T_DWORD emSetOemKey(      EC_T_DWORD              dwInstanceID,
                                                    EC_T_UINT64             qwOemKey        )
{
    return emIoCtl(dwInstanceID, EC_IOCTL_SET_OEM_KEY, &qwOemKey, sizeof(EC_T_UINT64), EC_NULL, 0, EC_NULL);
} EC_INLINESTOP
static EC_INLINESTART  EC_T_DWORD ecatSetOemKey(    EC_T_UINT64             qwOemKey        )
{
    return emSetOemKey(INSTANCE_MASTER_DEFAULT, qwOemKey);
} EC_INLINESTOP

static EC_INLINESTART  EC_T_DWORD emCheckOemKey(    EC_T_DWORD              dwInstanceID,
                                                    EC_T_UINT64             qwOemKey        )
{
    return emIoCtl(dwInstanceID, EC_IOCTL_CHECK_OEM_KEY, &qwOemKey, sizeof(EC_T_UINT64), EC_NULL, 0, EC_NULL);
} EC_INLINESTOP
static EC_INLINESTART  EC_T_DWORD ecatCheckOemKey(  EC_T_UINT64             qwOemKey        )
{
    return emCheckOemKey(INSTANCE_MASTER_DEFAULT, qwOemKey);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD emReadSlaveIdentification(   EC_T_DWORD   dwInstanceID,
                                                               EC_T_BOOL    bFixedAddressing,
                                                               EC_T_WORD    wSlaveAddress,
                                                               EC_T_WORD    wAdo,
                                                               EC_T_WORD*   pwValue,
                                                               EC_T_DWORD   dwTimeout       );
static EC_INLINESTART  EC_T_DWORD ecatReadSlaveIdentification( EC_T_BOOL    bFixedAddressing,
                                                               EC_T_WORD    wSlaveAddress,
                                                               EC_T_WORD    wAdo,
                                                               EC_T_WORD*   pwValue,
                                                               EC_T_DWORD   dwTimeout       )
{
    return emReadSlaveIdentification(INSTANCE_MASTER_DEFAULT, bFixedAddressing, wSlaveAddress, wAdo, pwValue, dwTimeout);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD emSetSlaveDisabled(  EC_T_DWORD           dwInstanceID,
                                                       EC_T_BOOL            bFixedAddressing,
                                                       EC_T_WORD            wSlaveAddress,
                                                       EC_T_BOOL            bDisabled);
static EC_INLINESTART  EC_T_DWORD ecatSetSlaveDisabled(EC_T_BOOL            bFixedAddressing,
                                                       EC_T_WORD            wSlaveAddress,
                                                       EC_T_BOOL            bDisabled       )
{
    return emSetSlaveDisabled(INSTANCE_MASTER_DEFAULT, bFixedAddressing, wSlaveAddress, bDisabled);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD emSetSlaveDisconnected(  EC_T_DWORD       dwInstanceID,
                                                           EC_T_BOOL        bFixedAddressing,
                                                           EC_T_WORD        wSlaveAddress,
                                                           EC_T_BOOL        bDisconnected   );
static EC_INLINESTART  EC_T_DWORD ecatSetSlaveDisconnected(EC_T_BOOL        bFixedAddressing,
                                                           EC_T_WORD        wSlaveAddress,
                                                           EC_T_BOOL        bDisconnected   )
{
    return emSetSlaveDisconnected(INSTANCE_MASTER_DEFAULT, bFixedAddressing, wSlaveAddress, bDisconnected);
} EC_INLINESTOP

ATECAT_API             EC_T_DWORD emGetMemoryUsage(  EC_T_DWORD         dwInstanceID,
                                                     EC_T_DWORD*        pdwCurrentUsage,
                                                     EC_T_DWORD*        pdwMaxUsage);
static EC_INLINESTART  EC_T_DWORD ecatGetMemoryUsage(EC_T_DWORD*        pdwCurrentUsage,
                                                     EC_T_DWORD*        pdwMaxUsage)
{
    return emGetMemoryUsage(INSTANCE_MASTER_DEFAULT, pdwCurrentUsage, pdwMaxUsage);
} EC_INLINESTOP


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* INC_ATETHERCAT */

/*-END OF SOURCE FILE--------------------------------------------------------*/
