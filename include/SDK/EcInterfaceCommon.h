/*-----------------------------------------------------------------------------
 * EcInterfaceCommon.h
 * Copyright                acontis technologies GmbH, Weingarten, Germany
 * Response                 Stefan Zintgraf
 * Description              EtherCAT Master common definitions for interfaces
 *---------------------------------------------------------------------------*/

#ifndef INC_ECINTERFACECOMMON
#define INC_ECINTERFACECOMMON

/*-INCLUDES------------------------------------------------------------------*/
#ifndef INC_ECESCREG
#include "EcEscReg.h"
#endif

/*-COMPILER SETTINGS---------------------------------------------------------*/
#ifdef __cplusplus
extern "C"
{
#endif

/*-DEFINES/MACROS------------------------------------------------------------*/
/* configuration for OD */
#define ECAT_DEVICE_NAMESIZE            80
#define MAX_SLAVE_DEVICENAME            80                      /* 0x3xxx Slave Device name len */
#define EC_OD_MAX_DRIVER_IDENT_LEN      40                      /* maximum length of link layer name */
#define EC_CFG_SLAVE_PD_SECTIONS        ((EC_T_DWORD)4)         /* amount of recv/send entries per slave processdata */

#define HISTORY_OBJECT_DIAGELE_SIZE     ((EC_T_WORD)0x0100)     /* size in bytes */
#define MAX_DIAG_MSG                    200
#define MAX_NOTIFICATIONS               50                      /* maximum length of link layer name */
#define NOTIFICATION_MEMBER_COUNT       4                       /* maximum length of link layer name */
#define NOTIFICATION_TEXT_CODE          "Notification Message %03d - Code"  /* maximum length of link layer name */
#define NOTIFICATION_TEXT_COUNT         "Notification Message %03d - Count" /* maximum length of link layer name */

#define NOTIFICATION_FLAGS_CLEAR        0x1                     /* Diagnosis flag: Send as emergency */

#define   DEFTYPE_BUSDIAGNOSTIC         0x0040                  /* Object 0x2002  Bus Diagnosis Object */
#define   DEFTYPE_MACADDRESS            0x0041                  /* Object 0x2005  MAC Address Object */
#define   DEFTYPE_MASTERINITPARM        0x0042                  /* Object 0x2020  Master Initialization parameters */
#define   DEFTYPE_SLAVECFGINFO          0x0043                  /* Object 0x3000 - 0x3FFF Slave Objects */
#define   DEFTYPE_BUSLOADBASE           0x0044                  /* Object 0x2200  Bus Load Base Object */
#define   DEFTYPE_SLAVECFG              0x0045                  /* Object 0x8000 - 0x8FFF Slave Objects (configured slaves) */
#define   DEFTYPE_SLAVEINF              0x0046                  /* Object 0x9000 - 0x9FFF Slave Objects (connected slaves) */
#define   DEFTYPE_SLAVEDIAG             0x0047                  /* Object 0xA000 - 0xAFFF Diagnosis Data */
#define   DEFTYPE_DEVICEPROFILE         0x0048                  /* Object 0xA000 - 0xAFFF Diagnosis Data */
#define   DEFTYPE_DETECTMODCMD          0x0049                  /* Object 0xF002  Detect Modules Command */
#define   DEFTYPE_CONFADDRLIST          0x0050                  /* Object 0xF02x  Configured address list */
#define   DEFTYPE_CONNADDRLIST          0x0051                  /* Object 0xF04x  Detected address list */
#define   DEFTYPE_REDUNDANCY            0x0052                  /* Object 0x2003  Redundancy Diagnosis Object */
#define   DEFTYPE_NOTIFY_COUNTER        0x0053                  /* Object 0x2004  Notification Counter Object */
#define   DEFTYPE_MAILBOX_STATISTICS    0x0054                  /* Object 0x2006  Mailbox Statistics Object */
#define   DEFTYPE_DCM_BUS_SHIFT         0x0055                  /* Object 0x2102  DCM Bus Shift Object  */

#define SETDIAGNUMBER(number) \
    ((EC_T_DWORD)(((((EC_T_DWORD)(number))&0xFFF)|(0xE<<12)|((((((EC_T_DWORD)(number))>>12)==0)?1:((((EC_T_DWORD)(number))>>12)==0xFFFF)?0xFFFE:(((EC_T_DWORD)(number))>>12))<<16))))

#define DIAG_MSG_TYPE_MASK              0xF                     /* Diagnosis message type mask */
#define DIAG_MSG_TYPE_INFO              0x0                     /* Diagnosis message type info */
#define DIAG_MSG_TYPE_WARNING           0x1                     /* Diagnosis message type warning */
#define DIAG_MSG_TYPE_ERROR             0x2                     /* Diagnosis message type error */
#define DIAGFLAGINFO                    DIAG_MSG_TYPE_INFO
#define DIAGFLAGWARN                    DIAG_MSG_TYPE_WARNING
#define DIAGFLAGERROR                   DIAG_MSG_TYPE_ERROR

#define DIAG_MSG_PARAM_TYPE_MASK    0xF000 /* Diagnosis parameter type mask*/
#define DIAG_MSG_PARAM_TYPE_OFFSET  12 /* Diagnosis parameter type offset*/
#define DIAG_MSG_PARAM_TYPE_DATA    0x0 /* Diagnosis parameter type data*/
#define DIAG_MSG_PARAM_TYPE_B_ARRY  0x1 /* Diagnosis parameter type Byte Array*/
#define DIAG_MSG_PARAM_TYPE_ASCII   0x2 /* Diagnosis parameter type ASCII*/
#define DIAG_MSG_PARAM_TYPE_UNICODE 0x3 /* Diagnosis parameter type UNICODE*/
#define DIAG_MSG_PARAM_TYPE_TEXTID  0x4 /* Diagnosis parameter type Text ID */
/* this defines are only for compatibility reasons */
#define DIAGPARMTYPEDATATYPE    ((EC_T_WORD)(DIAG_MSG_PARAM_TYPE_DATA<<12))
#define DIAGPARMTYPEBYTEARRAY   ((EC_T_WORD)(DIAG_MSG_PARAM_TYPE_B_ARRY<<12))
#define DIAGPARMTYPEASCIISTRG   ((EC_T_WORD)(DIAG_MSG_PARAM_TYPE_ASCII<<12))
#define DIAGPARMTYPEUNICODESTRG ((EC_T_WORD)(DIAG_MSG_PARAM_TYPE_UNICODE<<12))
#define DIAGPARMTYPETEXTID      ((EC_T_WORD)(DIAG_MSG_PARAM_TYPE_TEXTID<<12))

/*0x1F03 SI5 flag values/masks*/
#define DIAG_SEND_AS_EMCY           0x1 /* Diagnosis flag : Send as emergency*/
#define DIAG_DISABLE_INFO_MSG       0x2 /* Diagnosis flag : Disable Info messages*/
#define DIAG_DISABLE_WARNING_MSG    0x4 /* Diagnosis flag : Disable Warning messages*/
#define DIAG_DISABLE_ERROR_MSG      0x8 /* Diagnosis flag : Disable Error messages*/
#define DIAG_OPERATION_MODE         0x10 /* Indicates the diagnosis history mode (0 "overwrite" Mode; 1 "acknowledge" Mode)*/
#define DIAG_OVERWRITE_DISCARD      0x20 /* Indicates if messages were overwritten ("overwrite" mode) or new messages were discard ("acknowledge" mode)*/

#define DIAG_MSG_DEFAULT_LEN        16   /* Default Length: DiagCode + Flags + TextID + TimeStamp*/

#define COEOBJID_0x800              ((EC_T_WORD)0x0800) /* Object 0x0800  ENUM */

#define COEOBJID_HISTORY_OBJECT     ((EC_T_WORD)0x10F3) /* Object 0x10F3  History Object */

#define COEOBJID_MAST_STATECHNG     ((EC_T_WORD)0x2000) /* Object 0x2000  Master State change Command */
#define COEOBJID_MAST_STATESUMMARY  ((EC_T_WORD)0x2001) /* Object 0x2000  Master State change Command */
#define COEOBJID_BUS_DIAGNOSIS      ((EC_T_WORD)0x2002) /* Object 0x2002  Bus Diagnosis Object */
#define COEOBJID_REDUNDANCY         ((EC_T_WORD)0x2003) /* Object 0x2003  Redundancy Diagnosis Object */
#define COEOBJID_NOTIFY_COUNTER     ((EC_T_WORD)0x2004) /* Object 0x2004  Notification Counter Object */
#define COEOBJID_MAC_ADDRESS        ((EC_T_WORD)0x2005) /* Object 0x2005  MAC Address Object */
#define COEOBJID_MAILBOX_STATISTICS ((EC_T_WORD)0x2006) /* Object 0x2006  Mailbox Statistics Object */
#define COEOBJID_DEBUG_REGISTER     ((EC_T_WORD)0x2010) /* Object 0x2010  Debug Register */
#define COEOBJID_MASTER_INIT_PARM   ((EC_T_WORD)0x2020) /* Object 0x2020  Master Initialization Parameters */
#define COEOBJID_DC_DEVIATION_LIMIT ((EC_T_WORD)0x2100) /* Object 0x2100  DC Deviation Limit */
#define COEOBJID_DC_CURDEVIATION    ((EC_T_WORD)0x2101) /* Object 0x2101  DC Current Deviation */
#define COEOBJID_DCM_BUSSHIFT       ((EC_T_WORD)0x2102) /* Object 0x2102  DCM Bus Shift */


#define COEOBJID_BUSLOAD_BASE       ((EC_T_WORD)0x2200) /* Object 0x2200  Bus Load Base */

#define COEOBJID_SLAVECFGINFOBASE   ((EC_T_WORD)0x3000) /* Object 0x3000 - 0x3FFF Slave Objects */

#define COEOBJID_SLAVECFGBASE       ((EC_T_WORD)0x8000) /* Object 0x8000 - 0x8FFF Slave Objects (configured slaves) "Modular Device Profiles" */
#define COEOBJID_SLAVEINFBASE       ((EC_T_WORD)0x9000) /* Object 0x9000 - 0x9FFF Slave Objects (connected slaves) "Modular Device Profiles" */
#define COEOBJID_SLAVEDIAGBASE      ((EC_T_WORD)0xA000) /* Object 0xA000 - 0xAFFF Diagnosis Data "Modular Device Profiles" */
#define COEOBJID_DEVICEPROFILE      ((EC_T_WORD)0xF000) /* Object 0xF002  Detect Modules Command "Modular Device Profiles" */
#define COEOBJID_DETECTMODCMD       ((EC_T_WORD)0xF002) /* Object 0xF002  Detect Modules Command "Modular Device Profiles" */
#define COEOBJID_CONFADDRLISTBASE   ((EC_T_WORD)0xF020) /* Object 0xF002  Detect Modules Command "Modular Device Profiles" */
#define COEOBJID_CONNADDRLISTBASE   ((EC_T_WORD)0xF040) /* Object 0xF04x  Detected address list "Modular Device Profiles" */


#define DEVICETYPE_ETHERNET_GATEWAY     ((EC_T_WORD)1000)
#define DEVICETYPE_ETHERCAT_MASTER      ((EC_T_WORD)1100)
#define DEVICETYPE_ETHERCAT_SLAVE       ((EC_T_WORD)1110)
#define DEVICETYPE_KBUS_MASTER          ((EC_T_WORD)1120)
#define DEVICETYPE_PROFIBUS_MASTER      ((EC_T_WORD)3100)
#define DEVICETYPE_PROFIBUS_SLAVE       ((EC_T_WORD)3110)
#define DEVICETYPE_INTERBUS_MASTER      ((EC_T_WORD)4100)
#define DEVICETYPE_INTERBUS_SLAVE       ((EC_T_WORD)4110)
#define DEVICETYPE_CANOPEN_MASTER       ((EC_T_WORD)5100)
#define DEVICETYPE_CANOPEN_SLAVE        ((EC_T_WORD)5110)
#define DEVICETYPE_DEVICENET_MASTER     ((EC_T_WORD)5200)
#define DEVICETYPE_DEVICENET_SLAVE      ((EC_T_WORD)5210)
#define DEVICETYPE_ASI_MASTER           ((EC_T_WORD)6200)
#define DEVICETYPE_IOLINK_MASTER        ((EC_T_WORD)6220)


static EC_INLINESTART EC_T_VOID SAFE_STRCPY(EC_T_CHAR* szTargetString, EC_T_CHAR* szSourceString, EC_T_INT nMaxSize)
{
    if (nMaxSize != 0)
    {
        OsMemset(szTargetString, 0, nMaxSize);
        OsStrncpy(szTargetString, szSourceString, nMaxSize - 1);
    }
    else
    {
        szTargetString[0] = '\0';
    }
} EC_INLINESTOP

#define MAX_ERRINFO_STRLEN  8   /* maximum length of error notification info strings */
#define MAX_SHORT_STRLEN    20  /* maximum length of short info string */
#define MAX_STD_STRLEN      80  /* maximum length of standard info string */

#define MAX_FILE_NAME_SIZE  32  /* maximum length of file name for FoE Download/Upload */

#define MASTER_SLAVE_ID     ((EC_T_DWORD)0x10000)
#define MASTER_RED_SLAVE_ID ((EC_T_DWORD)0x20000)
#define EL9010_SLAVE_ID     ((EC_T_DWORD)0x30000)
#define FRAMELOSS_SLAVE_ID  ((EC_T_DWORD)0x40000)

/* EtherCAT state */
typedef enum _EC_T_STATE
{
    eEcatState_UNKNOWN  = 0,                        /*< unknown */
    eEcatState_INIT     = 1,                        /*< init */
    eEcatState_PREOP    = 2,                        /*< pre-operational */
    eEcatState_SAFEOP   = 4,                        /*< safe operational */
    eEcatState_OP       = 8,                        /*< operational */

    eEcatState_BOOTSTRAP = 3,                       /*< BootStrap */
    
    /* Borland C++ datatype alignment correction */
    eEcatState_BCppDummy   = 0xFFFFFFFF
} EC_T_STATE;

typedef enum _EC_T_STATEMACH_RES
{
    eStateMachRes_Unknown   =   0,
    eStateMachRes_Pending   =   1,
    eStateMachRes_Done      =   2,
    eStateMachRes_Error     =   4,

    eStateMachRes_BCppDummy =   0xFFFFFFFF
}   EC_T_STATEMACH_RES;

typedef enum EC_MAILBOX_FLAG
{
    EC_MAILBOX_FLAG_SDO_COMPLETE			= 1,
    
    /* Borland C++ datatype alignment correction */
    EC_MAILBOX_FLAG_BCppDummy                               = 0xFFFFFFFF
} EC_MAILBOX_FLAG;

/* EtherCat specific notify codes */
#define EC_NOTIFY_GENERIC                       0x00000000
#define EC_NOTIFY_ERROR                         0x00010000                  /* 0x00010000 ... 0x0001ffff */

#define EC_NOTIFY_MBOXRCV                       0x00020000
#define EC_SZTXT_NOTIFY_MBOXRCV                 "EC_NOTIFY_MBOXRCV"

#define EC_NOTIFY_SCANBUS                       0x00030000
#define EC_NOTIFY_HOTCONNECT                    0x00040000        

#define EC_NOTIFY_APP                           0x00080000                  /* application specific codes used by ecatNotifyApp() */
#define EC_NOTIFY_APP_MAX_CODE                  0x0000FFFF                  /* max number app notify codes */

#define ATEMRAS_NOTIFY_GENERIC					0x00100000
#define ATEMRAS_NOTIFY_ERROR					0x00110000

#define EC_NOTIFY_STATECHANGED                  (EC_NOTIFY_GENERIC | 1)     /* 0x00000001: EtherCAT operational state change */
#define EC_SZTXT_NOTIFY_STATECHANGED			"EC_NOTIFY_STATECHANGED"

#define EC_NOTIFY_ETH_LINK_CONNECTED            (EC_NOTIFY_GENERIC | 2)     /* 0x00000002: Ethernet link (cable) connected */
#define EC_SZTXT_NOTIFY_ETH_LINK_CONNECTED		"EC_NOTIFY_ETH_LINK_CONNECTED"

#define EC_NOTIFY_SB_STATUS                     (EC_NOTIFY_GENERIC | 3)     /* 0x00000003: ScanBus finished */ 
#define EC_SZTXT_NOTIFY_SB_STATUS		        "EC_NOTIFY_SB_STATUS"

#define EC_NOTIFY_DC_STATUS                     (EC_NOTIFY_GENERIC | 4)     /* 0x00000004: Distributed clocks initialized */
#define EC_SZTXT_NOTIFY_DC_STATUS			    "EC_NOTIFY_DC_STATUS"

/* Distributed clocks (DC) */
#define EC_NOTIFY_DC_SLV_SYNC                   (EC_NOTIFY_GENERIC | 5)     /* 0x00000005: DC Slave Synchronization deviation notification */
#define EC_SZTXT_NOTIFY_DC_SLV_SYNC		        "EC_NOTIFY_DC_SLV_SYNC"

/* Distributed Clocks Latching (DCL) */
#define EC_NOTIFY_DCL_STATUS                    (EC_NOTIFY_GENERIC | 8)     /* 0x00000008: DCL initialized */
#define EC_SZTXT_NOTIFY_DCL_STATUS		        "EC_NOTIFY_DCL_STATUS"

/* Distributed clocks master sync (DCM) */
#define EC_NOTIFY_DCM_SYNC                      (EC_NOTIFY_GENERIC | 9)     /* 0x00000009: DCM InSync */
#define EC_SZTXT_NOTIFY_DCM_SYNC	            "EC_NOTIFY_DCM_SYNC"

#define EC_NOTIFY_DCX_SYNC                      (EC_NOTIFY_GENERIC | 10)    /* 0x00000009: DCX InSync */
#define EC_SZTXT_NOTIFY_DCX_SYNC	            "EC_NOTIFY_DCX_SYNC"

#define EC_NOTIFY_SLAVE_STATECHANGED            (EC_NOTIFY_GENERIC | 21)    /* 0x00000015: Slave finished successfully state transition */
#define EC_SZTXT_NOTIFY_SLAVE_STATECHANGED      "EC_NOTIFY_SLAVE_STATECHANGED"

#define EC_NOTIFY_SLAVES_STATECHANGED           (EC_NOTIFY_GENERIC | 22)    /* 0x00000016: Slaves finished successfully state transition */
#define EC_SZTXT_NOTIFY_SLAVES_STATECHANGED     "EC_NOTIFY_SLAVES_STATECHANGED"

#define EC_NOTIFY_RAWCMD_DONE                   (EC_NOTIFY_GENERIC | 100)   /* 0x00000064: Queue Raw Command Response Notification */
#define EC_SZTXT_NOTIFY_RAWCMD_DONE		        "EC_NOTIFY_RAWCMD_DONE"

#define EC_NOTIFY_SLAVE_PRESENCE                (EC_NOTIFY_GENERIC | 101)   /* 0x00000065: Slave (dis)appeared */
#define EC_SZTXT_NOTIFY_SLAVE_PRESENCE          "EC_NOTIFY_SLAVE_PRESENCE"

#define EC_NOTIFY_SLAVES_PRESENCE               (EC_NOTIFY_GENERIC | 102)   /* 0x00000066: Slaves (dis)appeared */
#define EC_SZTXT_NOTIFY_SLAVES_PRESENCE         "EC_NOTIFY_SLAVES_PRESENCE"

#define EC_NOTIFY_REFCLOCK_PRESENCE             (EC_NOTIFY_GENERIC | 103)   /* 0x00000067: Reference clock (dis)appeared */
#define EC_SZTXT_NOTIFY_REFCLOCK_PRESENCE       "EC_NOTIFY_REFCLOCK_PRESENCE"

#ifdef INCLUDE_COE_PDO_SUPPORT
#define EC_NOTIFY_COE_TX_PDO                    (EC_NOTIFY_MBOXRCV | 1)     /* 0x00020001: TxPDO notification */
#define EC_SZTXT_NOTIFY_COE_TX_PDO			    "EC_NOTIFY_COE_TX_PDO"
#endif

#define EC_NOTIFY_RAWMBX_DONE                   (EC_NOTIFY_MBOXRCV | 2)     /* 0x00020002: Raw mailbox transfer response */
#define EC_SZTXT_NOTIFY_RAWMBX_DONE             "EC_NOTIFY_RAWMBX_DONE"

#define EC_NOTIFY_COE_INIT_CMD                  (EC_NOTIFY_MBOXRCV | 3)     /* 0x00020003: CoE init command */
#define EC_SZTXT_EC_NOTIFY_COE_INIT_CMD         "EC_NOTIFY_COE_INIT_CMD"

/* errors */
#define EC_NOTIFY_CYCCMD_WKC_ERROR              (EC_NOTIFY_ERROR | 1)       /* 0x00010001: cyclic command: working counter error */
#define EC_SZTXT_NOTIFY_CYCCMD_WKC_ERROR		"EC_NOTIFY_CYCCMD_WKC_ERROR"

#define EC_NOTIFY_MASTER_INITCMD_WKC_ERROR      (EC_NOTIFY_ERROR | 2)       /* 0x00010002: master init command: working counter error */
#define EC_SZTXT_NOTIFY_MASTER_INITCMD_WKC_ERROR "EC_NOTIFY_MASTER_INITCMD_WKC_ERROR"

#define EC_NOTIFY_SLAVE_INITCMD_WKC_ERROR       (EC_NOTIFY_ERROR | 3)       /* 0x00010003: slave init command: working counter error */
#define EC_SZTXT_NOTIFY_SLAVE_INITCMD_WKC_ERROR	"EC_NOTIFY_SLAVE_INITCMD_WKC_ERROR"

#define EC_NOTIFY_EOE_MBXSND_WKC_ERROR          (EC_NOTIFY_ERROR | 7)       /* 0x00010007: EoE mbox send: working counter error */
#define EC_SZTXT_NOTIFY_EOE_MBXSND_WKC_ERROR	"EC_NOTIFY_EOE_MBXSND_WKC_ERROR"

#define EC_NOTIFY_COE_MBXSND_WKC_ERROR          (EC_NOTIFY_ERROR | 8)       /* 0x00010008: CoE mbox send: working counter error */
#define EC_SZTXT_NOTIFY_COE_MBXSND_WKC_ERROR	"EC_NOTIFY_COE_MBXSND_WKC_ERROR"

#define EC_NOTIFY_FOE_MBXSND_WKC_ERROR          (EC_NOTIFY_ERROR | 9)      /* 0x00010009: FoE mbox send: working counter error */
#define EC_SZTXT_NOTIFY_FOE_MBXSND_WKC_ERROR	"EC_NOTIFY_FOE_MBXSND_WKC_ERROR"

#define EC_NOTIFY_FRAME_RESPONSE_ERROR          (EC_NOTIFY_ERROR | 10)      /* 0x0001000a: Got no response on a sent Ethernet frame */
#define EC_SZTXT_NOTIFY_FRAME_RESPONSE_ERROR	"EC_NOTIFY_FRAME_RESPONSE_ERROR"

#define EC_NOTIFY_SLAVE_INITCMD_RESPONSE_ERROR  (EC_NOTIFY_ERROR | 11)      /* 0x0001000b: Got no or unexpected response on a sent ecat init command from slave */
#define EC_SZTXT_NOTIFY_SLAVE_INITCMD_RESPONSE_ERROR "EC_NOTIFY_SLAVE_INITCMD_RESPONSE_ERROR"

#define EC_NOTIFY_MASTER_INITCMD_RESPONSE_ERROR (EC_NOTIFY_ERROR | 12)      /* 0x0001000c: Got no response on a sent ecat master init command */
#define EC_SZTXT_NOTIFY_MASTER_INITCMD_RESPONSE_ERROR "EC_NOTIFY_MASTER_INITCMD_RESPONSE_ERROR"

#define EC_NOTIFY_MBSLAVE_INITCMD_TIMEOUT       (EC_NOTIFY_ERROR | 14)      /* 0x0001000e: Timeout when waiting for mailbox init command response */
#define EC_SZTXT_NOTIFY_MBSLAVE_INITCMD_TIMEOUT	"EC_NOTIFY_MBSLAVE_INITCMD_TIMEOUT"

#define EC_NOTIFY_NOT_ALL_DEVICES_OPERATIONAL   (EC_NOTIFY_ERROR | 15)      /* 0x0001000f: Not all slave devices are in operational state when receiving cyclic frames */
#define EC_SZTXT_NOTIFY_NOT_ALL_DEVICES_OPERATIONAL	"EC_NOTIFY_NOT_ALL_DEVICES_OPERATIONAL"

#define EC_NOTIFY_ETH_LINK_NOT_CONNECTED        (EC_NOTIFY_ERROR | 16)      /* 0x00010010: Ethernet link (cable) not connected */
#define EC_SZTXT_NOTIFY_ETH_LINK_NOT_CONNECTED	"EC_NOTIFY_ETH_LINK_NOT_CONNECTED"

#define EC_NOTIFY_RED_LINEBRK                   (EC_NOTIFY_ERROR | 18)      /* 0x00010012: Redundancy: line break detected */
#define EC_SZTXT_NOTIFY_RED_LINEBRK	            "EC_NOTIFY_RED_LINEBRK"

#define EC_NOTIFY_STATUS_SLAVE_ERROR            (EC_NOTIFY_ERROR | 19)      /* 0x00010013: At least one slave is in error state when receiving cyclic frames (BRD AL-STATUS) */
#define EC_SZTXT_NOTIFY_STATUS_SLAVE_ERROR		"EC_NOTIFY_STATUS_SLAVE_ERROR"

#define EC_NOTIFY_SLAVE_ERROR_STATUS_INFO       (EC_NOTIFY_ERROR | 20)      /* 0x00010014: Slave error (AL status code) */
#define EC_SZTXT_NOTIFY_SLAVE_ERROR_STATUS_INFO "EC_NOTIFY_SLAVE_ERROR_STATUS_INFO"

#define EC_NOTIFY_SLAVE_NOT_ADDRESSABLE         (EC_NOTIFY_ERROR | 21)      /* 0x00010015: Station address lost (or slave missing) - FPRD to AL_STATUS failed  */
#define EC_SZTXT_NOTIFY_SLAVE_NOT_ADDRESSABLE	"EC_NOTIFY_SLAVE_NOT_ADDRESSABLE"

#define EC_NOTIFY_SOE_MBXSND_WKC_ERROR          (EC_NOTIFY_ERROR | 23)      /* 0x00010017: SoE mbox send: working counter error */
#define EC_SZTXT_NOTIFY_SOE_MBXSND_WKC_ERROR	"EC_NOTIFY_SOE_MBXSND_WKC_ERROR"

#define EC_NOTIFY_SOE_WRITE_ERROR               (EC_NOTIFY_ERROR | 24)      /* 0x00010018: SoE mbox write responded with an error */
#define EC_SZTXT_NOTIFY_SOE_WRITE_ERROR	        "EC_NOTIFY_SOE_WRITE_ERROR"

#define EC_NOTIFY_MBSLAVE_COE_SDO_ABORT         (EC_NOTIFY_ERROR | 25)      /* 0x00010019: CoE mbox SDO abort */
#define EC_SZTXT_NOTIFY_MBSLAVE_COE_SDO_ABORT	"EC_NOTIFY_MBSLAVE_COE_SDO_ABORT"

#define EC_NOTIFY_CLIENTREGISTRATION_DROPPED    (EC_NOTIFY_ERROR | 26)      /* 0x0001001a: Client registration dropped, possibly call to ecatConfigureMaster by other thread (RAS) */
#define EC_SZTXT_NOTIFY_CLIENTREGISTRATION_DROPPED "EC_NOTIFY_CLIENTREGISTRATION_DROPPED"

#define EC_NOTIFY_RED_LINEFIXED                 (EC_NOTIFY_ERROR | 27)      /* 0x0001001b: Redundancy: line is repaired */
#define EC_SZTXT_NOTIFY_RED_LINEFIXED	        "EC_NOTIFY_RED_LINEFIXED"

#define EC_NOTIFY_FOE_MBSLAVE_ERROR             (EC_NOTIFY_ERROR | 28)      /* 0x0001001c: FoE mbox abort */
#define EC_SZTXT_NOTIFY_FOE_MBSLAVE_ERROR		"EC_NOTIFY_FOE_MBSLAVE_ERROR"

#define EC_NOTIFY_MBXRCV_INVALID_DATA           (EC_NOTIFY_ERROR | 29)      /* 0x0001001d: invalid mail box data received */
#define EC_SZTXT_NOTIFY_MBXRCV_INVALID_DATA		"EC_NOTIFY_MBXRCV_INVALID_DATA"

#define EC_NOTIFY_PDIWATCHDOG                   (EC_NOTIFY_ERROR | 30)      /* 0x0001001e: PDI Watchdog expired on slave, thrown by IST */
#define EC_SZTXT_NOTIFY_PDIWATCHDOG	            "EC_NOTIFY_PDIWATCHDOG"

#define EC_NOTIFY_SLAVE_NOTSUPPORTED            (EC_NOTIFY_ERROR | 31)      /* 0x0001001f: slave not supported (if redundancy is activated and slave doesn't fully support autoclose */
#define EC_SZTXT_NOTIFY_SLAVE_NOTSUPPORTED		"EC_NOTIFY_SLAVE_NOTSUPPORTED"

#define EC_NOTIFY_SLAVE_UNEXPECTED_STATE        (EC_NOTIFY_ERROR | 32)      /* 0x00010020: slave in unexpected state */
#define EC_SZTXT_NOTIFY_SLAVE_UNEXPECTED_STATE  "EC_NOTIFY_SLAVE_UNEXPECTED_STATE"

#define EC_NOTIFY_ALL_DEVICES_OPERATIONAL       (EC_NOTIFY_ERROR | 33)      /* 0x00010021: All slave devices are in operational state */
#define EC_SZTXT_NOTIFY_ALL_DEVICES_OPERATIONAL	"EC_NOTIFY_ALL_DEVICES_OPERATIONAL"

#define EC_NOTIFY_VOE_MBXSND_WKC_ERROR          (EC_NOTIFY_ERROR | 34)      /* 0x00010022: VOE mbox send: working counter error */
#define EC_SZTXT_NOTIFY_VOE_MBXSND_WKC_ERROR	"EC_NOTIFY_VOE_MBXSND_WKC_ERROR"

#define EC_NOTIFY_EEPROM_CHECKSUM_ERROR         (EC_NOTIFY_ERROR | 35)      /* 0x00010023: EEPROM checksum error detected */
#define EC_SZTXT_NOTIFY_EEPROM_CHECKSUM_ERROR	"EC_NOTIFY_EEPROM_CHECKSUM_ERROR"

#define EC_NOTIFY_LINE_CROSSED                  (EC_NOTIFY_ERROR | 36)      /* 0x00010024: Crossed lines detected */
#define EC_SZTXT_NOTIFY_LINE_CROSSED	        "EC_NOTIFY_LINE_CROSSED"

#define EC_NOTIFY_JUNCTION_RED_CHANGE           (EC_NOTIFY_ERROR | 37)      /* 0x00010025: Junction redundancy change */
#define EC_SZTXT_NOTIFY_JUNCTION_RED_CHANGE	    "EC_NOTIFY_JUNCTION_RED_CHANGE"

#define EC_NOTIFY_SLAVES_UNEXPECTED_STATE       (EC_NOTIFY_ERROR | 38)      /* 0x00010026: slaves in unexpected state */
#define EC_SZTXT_NOTIFY_SLAVES_UNEXPECTED_STATE "EC_NOTIFY_SLAVES_UNEXPECTED_STATE"

#define EC_NOTIFY_SLAVES_ERROR_STATUS           (EC_NOTIFY_ERROR | 39)      /* 0x00010027: Slaves error (AL status code) */
#define EC_SZTXT_NOTIFY_SLAVES_ERROR_STATUS     "EC_NOTIFY_SLAVES_ERROR_STATUS"

#define EC_NOTIFY_FRAMELOSS_AFTER_SLAVE         (EC_NOTIFY_ERROR | 40)      /* 0x00010028: Frameloss after Slave */
#define EC_SZTXT_NOTIFY_FRAMELOSS_AFTER_SLAVE   "EC_NOTIFY_FRAMELOSS_AFTER_SLAVE"

#define EC_NUM_ERROR_NOTIFICATIONS                                 40       /* number of error notifications in 0x00010000 ... 0x0001fff */

/* ScanBus Notification */
#define EC_NOTIFY_SB_MISMATCH                   (EC_NOTIFY_SCANBUS | 2)     /* 0x00030002: ScanBus mismatch */
#define EC_SZTXT_NOTIFY_SB_MISMATCH             "EC_NOTIFY_SB_MISMATCH"

#define EC_NOTIFY_SB_DUPLICATE_HC_NODE          (EC_NOTIFY_SCANBUS | 3)     /* 0x00030003: ScanBus mismatch. A duplicate HC group was detected equal product code, vendor id 
                                                                               and alias address (or switch id)  */
#define EC_SZTXT_NOTIFY_SB_DUPLICATE_HC_NODE	"EC_NOTIFY_SB_DUPLICATE_HC_NODE"


/* Hot Connect Action Results */
/* #define EC_NOTIFY_HC_DETECTALLGROUPS         (EC_NOTIFY_HOTCONNECT | 1)   * 0x00040001: HC Detect All Groups done */
#define EC_NOTIFY_HC_DETECTADDGROUPS            (EC_NOTIFY_HOTCONNECT | 2)  /* 0x00040002: HC Enhance Detect All Groups done */
#define EC_SZTXT_NOTIFY_HC_DETECTADDGROUPS		"EC_NOTIFY_HC_DETECTADDGROUPS"

#define EC_NOTIFY_HC_PROBEALLGROUPS             (EC_NOTIFY_HOTCONNECT | 3)  /* 0x00040003: HC Probe All Groups done */
#define EC_SZTXT_NOTIFY_HC_PROBEALLGROUPS	    "EC_NOTIFY_HC_PROBEALLGROUPS"

#define EC_NOTIFY_HC_TOPOCHGDONE                (EC_NOTIFY_HOTCONNECT | 4)  /* 0x00040004: HC Topology Change done */
#define EC_SZTXT_NOTIFY_HC_TOPOCHGDONE	        "EC_NOTIFY_HC_TOPOCHGDONE"

/* Replaced by EC_NOTIFY_SLAVE_PRESENCE */
#define EC_NOTIFY_SLAVE_DISAPPEARS              (EC_NOTIFY_HOTCONNECT | 5)  /* 0x00040005: Slave disappears */
#define EC_NOTIFY_HC_SLAVE_PART EC_NOTIFY_SLAVE_DISAPPEARS
#define EC_SZTXT_NOTIFY_SLAVE_DISAPPEARS		"EC_NOTIFY_SLAVE_DISAPPEARS"

/* Replaced by EC_NOTIFY_SLAVE_PRESENCE */
#define EC_NOTIFY_SLAVE_APPEARS                 (EC_NOTIFY_HOTCONNECT | 6)  /* 0x00040006: Slave appears */
#define EC_NOTIFY_HC_SLAVE_JOIN EC_NOTIFY_SLAVE_APPEARS
#define EC_SZTXT_NOTIFY_SLAVE_APPEARS		    "EC_NOTIFY_SLAVE_APPEARS"

/* Remote API */
#define ATEMRAS_NOTIFY_CONNECTION               (ATEMRAS_NOTIFY_GENERIC|0x0001) /* 0x00100001 */
#define EC_SZTXT_ATEMRAS_NOTIFY_CONNECTION	    "ATEMRAS_NOTIFY_CONNECTION"

#define ATEMRAS_NOTIFY_REGISTER                 (ATEMRAS_NOTIFY_GENERIC|0x0002) /* 0x00100002 */
#define EC_SZTXT_ATEMRAS_NOTIFY_REGISTER	    "ATEMRAS_NOTIFY_REGISTER"

#define ATEMRAS_NOTIFY_UNREGISTER               (ATEMRAS_NOTIFY_GENERIC|0x0003) /* 0x00100003 */
#define EC_SZTXT_ATEMRAS_NOTIFY_UNREGISTER	    "ATEMRAS_NOTIFY_UNREGISTER"

#define ATEMRAS_NOTIFY_MARSHALERROR             (ATEMRAS_NOTIFY_ERROR  |0x0001) /* 0x00110001 */
#define EC_SZTXT_ATEMRAS_NOTIFY_MARSHALERROR    "ATEMRAS_NOTIFY_MARSHALERROR"

#define ATEMRAS_NOTIFY_ACKERROR                 (ATEMRAS_NOTIFY_ERROR  |0x0002) /* 0x00110002 */
#define EC_SZTXT_ATEMRAS_NOTIFY_ACKERROR	    "ATEMRAS_NOTIFY_ACKERROR"

#define ATEMRAS_NOTIFY_NONOTIFYMEMORY           (ATEMRAS_NOTIFY_ERROR  |0x0003) /* 0x00110003 */
#define EC_SZTXT_ATEMRAS_NOTIFY_NONOTIFYMEMORY	"ATEMRAS_NOTIFY_NONOTIFYMEMORY"

#define ATEMRAS_NOTIFY_STDNOTIFYMEMORYSMALL     (ATEMRAS_NOTIFY_ERROR  |0x0004) /* 0x00110004 */
#define EC_SZTXT_ATEMRAS_NOTIFY_STDNOTIFYMEMORYSMALL	"ATEMRAS_NOTIFY_STDNOTIFYMEMORYSMALL"

#define ATEMRAS_NOTIFY_MBXNOTIFYMEMORYSMALL     (ATEMRAS_NOTIFY_ERROR  |0x0005) /* 0x00110005 */
#define EC_SZTXT_ATEMRAS_NOTIFY_MBXNOTIFYMEMORYSMALL	"ATEMRAS_NOTIFY_MBXNOTIFYMEMORYSMALL"


/* cyclic error notification bit masks */
#define EC_CYC_ERR_MASK_NOT_ALL_DEVICES_OPERATIONAL     0x00000001      /* mask for cyclic EC_NOTIFY_NOT_ALL_DEVICES_OPERATIONAL notifications */
#define EC_CYC_ERR_MASK_STATUS_SLAVE_ERROR              0x00000002      /* mask for cyclic EC_NOTIFY_STATUS_SLAVE_ERROR notifications */
#define EC_CYC_ERR_MASK_CYCCMD_WKC_ERROR                0x00000004      /* mask for cyclic EC_NOTIFY_CYCCMD_WKC_ERROR notifications */
#define EC_CYC_ERR_MASK_UNEXPECTED_FRAME_RESPONSE       0x00000008      /* mask for cyclic EC_NOTIFY_FRAME_RESPONSE_ERROR notifications with error type eRspErr_UNEXPECTED */
#define EC_CYC_ERR_MASK_NO_FRAME_RESPONSE_ERROR         0x00000010      /* mask for cyclic EC_NOTIFY_FRAME_RESPONSE_ERROR notifications with error type eRspErr_NO_RESPONSE */
#define EC_ERR_MASK_ETH_LINK_NOT_CONNECTED              0x00000020      /* mask for EC_NOTIFY_ETH_LINK_NOT_CONNECTED notifications */
#define EC_CYC_ERR_MASK_ALL                             0xFFFFFFFF      /* mask for all cyclic error notifications */

/* Supported mailbox protocols */
#define EC_MBX_PROTOCOL_AOE 0x01
#define EC_MBX_PROTOCOL_EOE 0x02
#define EC_MBX_PROTOCOL_COE 0x04
#define EC_MBX_PROTOCOL_FOE 0x08
#define EC_MBX_PROTOCOL_SOE 0x10
#define EC_MBX_PROTOCOL_VOE 0x20

/* CoE debug bit masks */
#define EC_COE_DBG_MASK_SDO_DOWNLOAD        0x00000001  /* mask for SDO download transfer debug support */
#define EC_COE_DBG_MASK_SDO_UPLOAD          0x00000002  /* mask for SDO upload transfer debug support */
#define EC_COE_DBG_MASK_GET_ODLIST          0x00000004  /* mask for CoE get object dictionary list transfer debug support */
#define EC_COE_DBG_MASK_GET_OBDESC          0x00000008  /* mask for CoE get object description transfer debug support */
#define EC_COE_DBG_MASK_GET_ENTRYDESC       0x00000010  /* mask for CoE get entry description transfer debug support */
#ifdef INCLUDE_COE_PDO_SUPPORT
#define EC_COE_DBG_MASK_RX_PDO              0x00000020  /* mask for RxPDO transfer debug support */
#define EC_COE_DBG_MASK_TX_PDO              0x00000040  /* mask for TxPDO transfer debug support */
#endif
#define EC_COE_DBG_MASK_ALL                 0x00000FFF  /* mask for all CoE transfers debug support */

/* FoE debug bit masks */
#define EC_FOE_DBG_MASK_FILE_DOWNLOAD       0x00001000  /* mask for FoE download transfer debug support */
#define EC_FOE_DBG_MASK_FILE_UPLOAD         0x00002000  /* mask for FoE download transfer debug support */
#define EC_FOE_DBG_MASK_ALL                 0x000FF000  /* mask for all FoE transfers debug support */

#define EC_MBX_DBG_MASK_ALL                 0x000FFFFF  /* mask for all CoE transfers debug support */


/* SoE debug bit masks */
#ifdef INCLUDE_SOE_SUPPORT
#define EC_SOE_DBG_MASK_IDN_WRITE        0x00000001  /* mask for SoE download transfer debug support */
#define EC_SOE_DBG_MASK_IDN_READ         0x00000002  /* mask for SoE upload transfer debug support */
#define EC_SOE_DBG_MASK_ALL              0x00000FFF  /* mask for all SoE transfers debug support */
#endif

/* VoE debug bit masks */
#ifdef INCLUDE_VOE_SUPPORT
#define EC_VOE_DBG_MASK_MBX_DOWNLOAD     0x00100000  /* mask for VoE write transfer debug support */
#define EC_VOE_DBG_MASK_MBX_UPLOAD       0x00200000  /* mask for VoE read transfer debug support */
#define EC_VOE_DBG_MASK_ALL              0x0FF00000  /* mask for all VoE transfers debug support */
#endif

#define MAX_PROCESS_VAR_NAME_LEN            ((EC_T_DWORD)72)  /* STD: Maximum length of a process variable name */
#define MAX_PROCESS_VAR_NAME_LEN_EX         ((EC_T_DWORD)128) /* EX:  Maximum length of a process variable name */

/* EtherCAT commands */
typedef enum 
{
    EC_CMD_TYPE_NOP     = 0x00,
    EC_CMD_TYPE_APRD    = 0x01,
    EC_CMD_TYPE_APWR    = 0x02,
    EC_CMD_TYPE_APRW    = 0x03,
    EC_CMD_TYPE_FPRD    = 0x04,
    EC_CMD_TYPE_FPWR    = 0x05,
    EC_CMD_TYPE_FPRW    = 0x06,
    EC_CMD_TYPE_BRD     = 0x07,
    EC_CMD_TYPE_BWR     = 0x08,
    EC_CMD_TYPE_BRW     = 0x09,
    EC_CMD_TYPE_LRD     = 0x0A,
    EC_CMD_TYPE_LWR     = 0x0B,
    EC_CMD_TYPE_LRW     = 0x0C,
    EC_CMD_TYPE_ARMW    = 0x0D,
    EC_CMD_TYPE_FRMW    = 0x0E,
    EC_CMD_TYPE_EXT     = 0xFF,

    /* Borland C++ datatype alignment correction */
    EC_CMD_TYPE_BCppDummy   = 0xFFFFFFFF
} EC_CMD_TYPE;

/* textual description of EtherCat commands */
#define EcatCmdShortText(byCmd)                     \
    ((byCmd)==EC_CMD_TYPE_NOP?"NOP":                \
     ((byCmd)==EC_CMD_TYPE_APRD?"APRD":             \
      ((byCmd)==EC_CMD_TYPE_APWR?"APWR":            \
       ((byCmd)==EC_CMD_TYPE_APRW?"APRW":           \
        ((byCmd)==EC_CMD_TYPE_FPRD?"FPRD":          \
         ((byCmd)==EC_CMD_TYPE_FPWR?"FPWR":         \
          ((byCmd)==EC_CMD_TYPE_FPRW?"FPRW":        \
           ((byCmd)==EC_CMD_TYPE_BRD?"BRD":         \
            ((byCmd)==EC_CMD_TYPE_BWR?"BWR":        \
             ((byCmd)==EC_CMD_TYPE_BRW?"BRW":       \
              ((byCmd)==EC_CMD_TYPE_LRD?"LRD":      \
               ((byCmd)==EC_CMD_TYPE_LWR?"LWR":     \
                ((byCmd)==EC_CMD_TYPE_LRW?"LRW":    \
                 ((byCmd)==EC_CMD_TYPE_ARMW?"ARMW": \
                  ((byCmd)==EC_CMD_TYPE_FRMW?"FRMW":\
                  "INVALID ECAT CMD VALUE!!!"       \
    )))))))))))))))


/*-TYPEDEFS------------------------------------------------------------------*/
#include EC_PACKED_INCLUDESTART(1)

/* Object 0x10F3  History Object - Generic diagnosis message structure (0x10F3.SI6 : 0x10F3.MaxSubindex) */
/* Basic structure to handle message parameter */
typedef struct 
{
   EC_T_WORD        wParamFlags;                    /* Parameter flags */
   EC_T_BYTE        aData[4];                       /* Parameter buffer: length depending on parameters */
} EC_PACKED(1) EC_T_DIAGMSGPARAM;
typedef struct _EC_T_OBJ10F3_DIAGMSG
{
   EC_T_DWORD           dwDiagNumber;               /* Message code */
   EC_T_WORD            wFlags;                     /* Message flags */
   EC_T_WORD            wTextId;                    /* Text ID */
#ifdef  __TMS470__
   EC_T_DWORD  dwTimeStampLo;                          /* 0x08 */
   EC_T_DWORD  dwTimeStampHi;                          /* 0x08 */
#else
   EC_T_UINT64 qwTimeStamp;                            /* 0x08 */
#endif
   EC_T_DIAGMSGPARAM    oParameter;                 /* Handler of the first parameter */
} EC_PACKED(1) EC_T_OBJ10F3_DIAGMSG;
typedef struct _EC_T_OBJ10F3
{
    EC_T_WORD               wSubIndex0;             /* Subindex 000 */
    EC_T_BYTE               byMaxDiagMessages;      /* Subindex 001 */
    EC_T_BYTE               byNewestMessage;        /* Subindex 002 */
    EC_T_BYTE               byNewestAckMessage;     /* Subindex 003 */
    EC_T_BYTE               byNewDiagMessages;          /* Subindex 004 */
    EC_T_WORD               wFlags;                 /* Subindex 005 */
    /* EC_T_DIAGMESSAGE */                          /* Subindex 006ff */
} EC_PACKED(1) EC_T_OBJ10F3;

/* Object 0x2000  Master State change Command */
typedef enum _EC_T_OBJ2000_VALUES
{
    eMastStChng_init        = 1,
    eMastStChng_preop       = 2,
    eMastStChng_safeop      = 4,
    eMastStChng_op          = 8,

    /* Borland C++ datatype alignment correction */
    eMastStChng_BCppDummy   = 0xFFFFFFFF
} EC_T_OBJ2000_VALUES;

/* Object 0x2001  Master State Summary (EC_T_DWORD) */
#define OBJ2001_STATE_SUM_MASTER_OK    0x00000001      /* Bit 0: = 1 Master o.k. */
                                                       /* Bit 1-3: Reserved */
#define OBJ2001_STATE_SUM_MASTER_STATE 0x000000F0      /* Bit 4-7: Master State */
#define OBJ2001_STATE_SUM_SLAVE_REQ    0x00000100      /* Bit 8: Slaves in requested State */
#define OBJ2001_STATE_SUM_MASTER_REQ   0x00000200      /* Bit 9: Master in requested State */
#define OBJ2001_STATE_SUM_BUS_MATCH    0x00000400      /* Bit 10: Bus Scan Match */
#define OBJ2001_STATE_SUM_RES11        0x00000800      /* Bit 11: Reserved */
#define OBJ2001_STATE_SUM_DC_ENA       0x00001000      /* Bit 12: DC is enabled */
#define OBJ2001_STATE_SUM_DC_SYNC      0x00002000      /* Bit 13: DC In-Sync */
#define OBJ2001_STATE_SUM_DC_BUSY      0x00004000      /* Bit 14: DC Busy */
#define OBJ2001_STATE_SUM_RES15        0x00008000      /* Bit 15: Reserved */
#define OBJ2001_STATE_SUM_LINK_UP      0x00010000      /* Bit 16: Link Up  */
                                                       /* Bit 17-31: Reserved */

                                                             /* dword : .... .... .... ...L  .bID .BMS ssss ...O */
#define OBJ2001_STATE_SUM_MASK1  ((EC_T_DWORD)0x000107f0)    /* mask  : 0000 0000 0000 0001  0000 0111 1111 0000 */
#define OBJ2001_STATE_SUM_VALUE1 ((EC_T_DWORD)0x00010780)    /* value : 0000 0000 0000 0001  0000 0111 1000 0000 */
#define OBJ2001_STATE_SUM_MASK2  ((EC_T_DWORD)0x00007000)    /* mask2 : 0000 0000 0000 0000  0111 0000 0000 0000 */
#define OBJ2001_STATE_SUM_VALUE2 ((EC_T_DWORD)0x00003000)    /* value : 0000 0000 0000 0000  0011 0000 0000 0000 */

/* Object 0x2002  Bus Diagnosis Object */
#define OBJ2002_TXFRM_OFFSET    0x18
#define OBJ2002_RXFRM_OFFSET    0x1C
#define OBJ2002_LOSFRM_OFFSET   0x20
#define OBJ2002_CYCFRM_OFFSET   0x24
#define OBJ2002_CYCDGR_OFFSET   0x28
#define OBJ2002_ACYCFRM_OFFSET  0x2C
#define OBJ2002_ACYCDGR_OFFSET  0x30
typedef struct _EC_T_OBJ2002
{
    EC_T_WORD               wSubIndex0;             /* 0x00 */  /* Subindex 000 */
    EC_T_WORD               wReserved;              /* 0x02 */  /* Subindex 001 */
    EC_T_DWORD              dwCRC32ConfigCheckSum;  /* 0x04 */  /* Subindex 002 */
    EC_T_DWORD              dwNumSlavesFound;       /* 0x08 */  /* Subindex 003 */
    EC_T_DWORD              dwNumDCSlavesFound;     /* 0x0C */  /* Subindex 004 */
    EC_T_DWORD              dwNumCfgSlaves;         /* 0x10 */  /* Subindex 005 */
    EC_T_DWORD              dwNumMbxSlaves;         /* 0x14 */  /* Subindex 006 */
    
    EC_T_DWORD              dwTXFrames;             /* 0x18 */  /* Subindex 007 */
    EC_T_DWORD              dwRXFrames;             /* 0x1C */  /* Subindex 008 */
    EC_T_DWORD              dwLostFrames;           /* 0x20 */  /* Subindex 009 */
    
    EC_T_DWORD              dwCyclicFrames;         /* 0x24 */  /* Subindex 010 */
    EC_T_DWORD              dwCyclicDatagrams;      /* 0x28 */  /* Subindex 011 */
    EC_T_DWORD              dwAcyclicFrames;        /* 0x2C */  /* Subindex 012 */
    EC_T_DWORD              dwAcyclicDatagrams;     /* 0x30 */  /* Subindex 013 */
    EC_T_DWORD              dwClearCounters;        /* 0x34 */  /* Subindex 014 */
} EC_PACKED(1) EC_T_OBJ2002;

/* Object 0x2003  Redundancy Diagnosis Object */
typedef struct _EC_T_OBJ2003
{
    EC_T_WORD               wSubIndex0;             /* 0x00 */  /* Subindex 000 */
    EC_T_BYTE               byRedEnabled;           /* 0x01 */  /* Subindex 001 */
    EC_T_WORD               wNumOfMainSlaves;       /* 0x03 */  /* Subindex 002 */
    EC_T_WORD               wNumOfRedSlaves;        /* 0x05 */  /* Subindex 003 */
    EC_T_BYTE               byLineBreak;            /* 0x06 */  /* Subindex 004 */
} EC_PACKED(1) EC_T_OBJ2003;

/* Object 0x2004  Notification Counter Object */
typedef struct _EC_T_OBJ2004_NOTIFYMSG
{
  EC_T_DWORD              dwCode;
  EC_T_DWORD              dwCount;
} EC_PACKED(1) EC_T_OBJ2004_NOTIFYMSG; 
typedef struct _EC_T_OBJ2004
{
  EC_T_WORD               wSubIndex0;               /* Subindex 000 */
  EC_T_BYTE               byMaxMessages;            /* Subindex 001 */
  EC_T_BYTE               byMessageCount;           /* Subindex 002 */
  EC_T_BYTE               byFlags;                  /* Subindex 003 */
  /* EC_T_OBJ2004_NOTIFYMSG */                      /* Subindex 004ff */
} EC_PACKED(1) EC_T_OBJ2004;

/* Object 0x2005  MAC Address Object */
typedef struct _EC_T_OBJ2005
{
    EC_T_WORD               wSubIndex0;             /* Subindex 000 */
    EC_T_BYTE               abyHardware[6];         /* Subindex 001 */
    EC_T_BYTE               abyRedHardware[6];      /* Subindex 002 */
    EC_T_BYTE               abyCfgSource[6];        /* Subindex 003 */
    EC_T_BYTE               abyCfgDestination[6];   /* Subindex 004 */
} EC_PACKED(1) EC_T_OBJ2005;

/* Object 0x2006  Mailbox Statistics Object */
typedef struct EC_T_OBJ2006
{
    EC_T_WORD               wSubIndex0;             /* Subindex 000 */
    EC_T_DWORD              dwCnt[64];              /* Subindex 001...064 Mailbox Statistics Counters (Read/Write, Total/Last Second) */
    EC_T_UINT64             qwClearCounters;        /* Subindex 065 Clear Counters */
} EC_PACKED(1) EC_T_OBJ2006;

/* Object 0x2020  Master Initialization parameters */
typedef struct _EC_T_OBJ2020
{
    EC_T_WORD               wSubIndex0;             /* Subindex 000 */
    EC_T_DWORD              dwApplicationVersion;   /* Subindex 001 */
    EC_T_DWORD              dwMasterVersion;        /* Subindex 002 */
    EC_T_DWORD              dwMaxSlavesProcessedPerCycle; /* Subindex 003 */
    EC_T_DWORD              dwEcatCmdTimeout;       /* Subindex 004 */
    EC_T_DWORD              dwEcatCmdMaxRetries;    /* Subindex 005 */
    EC_T_DWORD              dwBusCycleTimeUsec;     /* Subindex 006 */
    EC_T_DWORD              dwEoeTimeout;           /* Subindex 007 */
    EC_T_DWORD              dwFoeBusyTimeout;       /* Subindex 008. Obsolete. */
    EC_T_DWORD              dwMaxQueuedEthFrames;   /* Subindex 009 */
    EC_T_DWORD              dwMaxSlaveCmdPerFrame;  /* Subindex 010 */
    EC_T_DWORD              dwMaxBusSlave;          /* Subindex 011 */
    EC_T_DWORD              dwReserved2;            /* Subindex 012 */
    EC_T_DWORD              dwStateChangeDebug;     /* Subindex 013 */
    EC_T_CHAR               szDriverIdent[EC_OD_MAX_DRIVER_IDENT_LEN];
                                                    /* Subindex 014 */
    EC_T_BOOL               bPollingModeActive;     /* Subindex 015 */
    EC_T_BOOL               bAllocSendFrameActive;  /* Subindex 016 */
} EC_PACKED(1) EC_T_OBJ2020;

/* Object 0x2102  DCM Bus Shift */
typedef struct _EC_T_OBJ2102
{
    EC_T_WORD               wSubIndex0;             /* Subindex 000 */
    EC_T_DWORD              dwErrorCode;            /* Subindex 001: Error Code */
    EC_T_BOOL               bDcInSync;              /* Subindex 002: DC synchronized */
    EC_T_BOOL               bDcmInSync;             /* Subindex 003: DCM controller synchronized */
    EC_T_INT                nCtlSetVal;             /* Subindex 004: Controller Set Value [nsec] */
    EC_T_INT                nCtlErrorFilt;          /* Subindex 005: Controller Error Filtered [nsec] */
    EC_T_INT                nCtlErrorAvg;           /* Subindex 006: Controller Error Average [nsec] */
    EC_T_INT                nCtlErrorMax;           /* Subindex 007: Controller Error Maximum [nsec] */
} EC_PACKED(1) EC_T_OBJ2102;

/* Object 0x2200  Bus Load Base */
typedef struct _EC_T_OBJ2200
{
    EC_T_WORD               wSubIndex0;             /* Subindex 000 */

    EC_T_DWORD              dwBytesPerSecondAct;    /* Subindex 001: TX bytes/second actual value */
    EC_T_DWORD              dwBytesPerSecondMin;    /* Subindex 002: TX bytes/second min. value   */
    EC_T_DWORD              dwBytesPerSecondMax;    /* Subindex 003: TX bytes/second max. value   */

    EC_T_DWORD              dwBytesPerCycleAct;     /* Subindex 004: TX bytes/cycle actual value  */
    EC_T_DWORD              dwBytesPerCycleMin;     /* Subindex 005: TX bytes/cycle min. value    */
    EC_T_DWORD              dwBytesPerCycleMax;     /* Subindex 006: TX bytes/cycle max. value    */
} EC_PACKED(1) EC_T_OBJ2200;

/* Object 0x3000 - 0x3FFF Slave Objects */
typedef struct _EC_T_OBJ3XXX
{
    EC_T_WORD               wSubIndex0;                         /* Subindex 000: (Offset   0) */
    EC_T_BOOL               bEntryValid;                        /* Subindex 001: (Offset   2) */
    EC_T_DWORD              dwVendorID;                         /* Subindex 002: (Offset   6) */
    EC_T_DWORD              dwProductCode;                      /* Subindex 003: (Offset  10) */
    EC_T_DWORD              dwRevisionNo;                       /* Subindex 004: (Offset  14) */
    EC_T_DWORD              dwSerialNo;                         /* Subindex 005: (Offset  18) */

    EC_T_CHAR               szDeviceName[MAX_SLAVE_DEVICENAME]; /* Subindex 006: (Offset  22) */
    EC_T_WORD               wAutoIncAddr;                       /* Subindex 007: (Offset 102) */
    EC_T_WORD               wPhysAddr;                          /* Subindex 008: (Offset 104) */
    EC_T_WORD               wConfigPhysAddr;                    /* Subindex 009: (Offset 106) */
    EC_T_WORD               wAliasAddr;                         /* Subindex 010: (Offset 108) */
    EC_T_WORD               wPortState;                         /* Subindex 011: (Offset 110) */
    EC_T_BOOL               bDCSupport;                         /* Subindex 012: (Offset 112) */
    EC_T_BOOL               bDC64Support;                       /* Subindex 013: (Offset 116) */

    EC_T_BOOL               bMailboxSupport;                    /* Subindex 014: (Offset 120) */
    EC_T_DWORD              dwReqState;                         /* Subindex 015: (Offset 124) */
    EC_T_DWORD              dwCurState;                         /* Subindex 016: (Offset 128) */
    EC_T_BOOL               bErrFlagSet;                        /* Subindex 017: (Offset 132) */
    EC_T_BOOL               bEnableLinkMsgs;                    /* Subindex 018: (Offset 136) */
    EC_T_DWORD              dwErrorCode;                        /* Subindex 019: (Offset 140) */
    EC_T_BOOL               bSyncPulseActive;                   /* Subindex 020: (Offset 144) */
    EC_T_DWORD              dwDCSync0Period;                    /* Subindex 021: (Offset 148) */
    EC_T_DWORD              dwDCSync1Period;                    /* Subindex 022: (Offset 152) */
    EC_T_DWORD              dwSBErrorCode;                      /* Subindex 023: (Offset 156) */

    EC_T_WORD               wRxErrorCounter0;                   /* Subindex 024: (Offset 160) */
    EC_T_WORD               wRxErrorCounter1;                   /* Subindex 025: (Offset 162) */
    EC_T_WORD               wRxErrorCounter2;                   /* Subindex 026: (Offset 164) */
    EC_T_WORD               wRxErrorCounter3;                   /* Subindex 027: (Offset 166) */
    EC_T_BYTE               byFwdRxErrorCounter0;               /* Subindex 028: (Offset 168) */
    EC_T_BYTE               byFwdRxErrorCounter1;               /* Subindex 029: (Offset 169) */
    EC_T_BYTE               byFwdRxErrorCounter2;               /* Subindex 030: (Offset 170) */
    EC_T_BYTE               byFwdRxErrorCounter3;               /* Subindex 031: (Offset 171) */
    
    EC_T_BYTE               byEcatProcUnitErrorCounter;         /* Subindex 032: (Offset 172) */
    EC_T_BYTE               byPDIErrorCounter;                  /* Subindex 033: (Offset 173) */
    EC_T_WORD               wMbxSupportedProtocols;             /* Subindex 034: (Offset 174) */
    EC_T_BYTE               byLostLinkCounter0;                 /* Subindex 035: (Offset 176) */
    EC_T_BYTE               byLostLinkCounter1;                 /* Subindex 036: (Offset 177) */
    EC_T_BYTE               byLostLinkCounter2;                 /* Subindex 037: (Offset 178) */
    EC_T_BYTE               byLostLinkCounter3;                 /* Subindex 038: (Offset 179) */
    EC_T_BYTE               byFmmusSupported;                   /* Subindex 039: (Offset 180) */
    EC_T_BYTE               bySyncManagersSupported;            /* Subindex 040: (Offset 181) */
    EC_T_BYTE               byRamSizeKb;                        /* Subindex 041: (Offset 182) */
    EC_T_BYTE               byPortDescriptor;                   /* Subindex 042: (Offset 183) */

    EC_T_BYTE               byESCType;                          /* Subindex 043: (Offset 184) */
    
    EC_T_BOOL               bSlaveIsOptional;                   /* Subindex 044: (Offset 185) */
    EC_T_BOOL               bSlaveIsPresent;                    /* Subindex 045: (Offset 189) */
    EC_T_DWORD              dwHotConnectGroupId;                /* Subindex 046: (Offset 193) */
    EC_T_DWORD              dwSystemTimeDifference;             /* Subindex 047: (Offset 197) */
    EC_T_DWORD              dwPdOffsIn;                         /* Subindex 048: (Offset 201) Process data offset of input data (in Bits) */
    EC_T_DWORD              dwPdSizeIn;                         /* Subindex 049: (Offset 205) Process data size of input data (in Bits) */
    EC_T_DWORD              dwPdOffsOut;                        /* Subindex 050: (Offset 209) Process data offset of output data (in Bits) */
    EC_T_DWORD              dwPdSizeOut;                        /* Subindex 051: (Offset 213) Process data size of output data (in Bits) */
} EC_PACKED(1) EC_T_OBJ3XXX;

/* Object 0x8000 - 0x8FFF Slave Objects (configured slaves) "Modular Device Profiles" */
typedef struct _EC_T_OBJ8XXX
{
    EC_T_WORD               wSubIndex0;                         /*   0 */       /* Subindex 000 */
    EC_T_WORD               wFixedStationAddr;                  /*   2 */       /* Subindex 001: Station Address of the first EtherCAT Slave (same value as 0xF020:01) */
    EC_T_CHAR               szType[64];                         /*   4 */       /* Subindex 002: Type of the first EtherCAT Slave configured */
    EC_T_CHAR               szName[64];                         /*  68 */       /* Subindex 003: Name of the first EtherCAT Slave configured (object 0x1008 of the EtherCAT slave) */
    EC_T_DWORD              dwDeviceType;                       /* 132 */       /* Subindex 004: Device Type of the first EtherCAT Slave configured (object 0x1000 of the EtherCAT slave) */
    EC_T_DWORD              dwVendorID;                         /* 136 */       /* Subindex 005: Vendor ID of the first EtherCAT Slave configured (entry 0x1018:01 of the EtherCAT slave) */
    EC_T_DWORD              dwProductCode;                      /* 140 */       /* Subindex 006: Product Code of the first EtherCAT Slave configured (entry 0x1018:02 of the EtherCAT slave) */
    EC_T_DWORD              dwRevision;                         /* 144 */       /* Subindex 007: Revision of the first EtherCAT Slave configured (entry 0x1018:03 of the EtherCAT slave) */
    EC_T_DWORD              dwSerial;                           /* 148 */       /* Subindex 008: Serial No of the first EtherCAT Slave configured (entry 0x1018:04 of the EtherCAT slave) */
    EC_T_WORD               wMailboxOutSize;                    /* 152 */       /* Subindex 033: Mailbox Write Size (SM0) of the first EtherCAT Slave configured */
    EC_T_WORD               wMailboxInSize;                     /* 154 */       /* Subindex 034: Mailbox Read Size (SM1) of the first EtherCAT Slave configured */
} EC_PACKED(1) EC_T_OBJ8XXX;              

/* Modular Device Profiles: EtherCAT Master  - internal slave object element (connected slaves) */
typedef struct _EC_T_OBJ9XXX
{
    EC_T_WORD               wSubIndex0;                         /*   0 */       /* Subindex 000 */
    EC_T_WORD               wFixedStationAddr;                  /*   2 */       /* Subindex 001: Fixed Station Address of the first EtherCAT Slave (same value as 0xF020:01) */
    EC_T_DWORD              dwVendorID;                         /*   4 */       /* Subindex 005: Vendor ID of the first EtherCAT Slave configured (entry 0x1018:01 of the EtherCAT slave) */
    EC_T_DWORD              dwProductCode;                      /*   8 */       /* Subindex 006: Product Code of the first EtherCAT Slave configured (entry 0x1018:02 of the EtherCAT slave) */
    EC_T_DWORD              dwRevision;                         /*  12 */       /* Subindex 007: Revision of the first EtherCAT Slave configured (entry 0x1018:03 of the EtherCAT slave) */
    EC_T_DWORD              dwSerial;                           /*  16 */       /* Subindex 008: Serial No. of the first EtherCAT Slave configured (entry 0x1018:04 of the EtherCAT slave) */
    EC_T_WORD               wDLStatus;                          /*  20 */       /* Subindex 032: DL Status (Register 0x110-0x111) of the first EtherCAT found */
} EC_PACKED(1) EC_T_OBJ9XXX;              

/* Modular Device Profiles: EtherCAT Master  - internal slave object element (slave diagnosis) */
typedef struct _EC_T_OBJAXXX
{
    EC_T_WORD               wSubIndex0;                         /*   0 */       /* Subindex 000 */
    EC_T_WORD               wALStatus;                          /*   2 */       /* Subindex 001: AL Status (Register 0x130-0x131) of the first EtherCAT slave configured */
    EC_T_WORD               wALControl;                         /*   4 */       /* Subindex 002: AL Control (Register 0x120-0x121) of the first EtherCAT slave configured */
} EC_PACKED(1) EC_T_OBJAXXX;              

/* Modular Device Profiles: EtherCAT Master  - modular device profile */
typedef struct _EC_T_OBJF000
{
    EC_T_WORD               wSubIndex0;                         /*   0 */       /* Subindex 000 */
    EC_T_WORD               wIndexDistance;                     /*   2 */       /* Subindex 001: Index distance between two modules = 0x01 */
    EC_T_WORD               wMaxModuleCnt;                      /*   4 */       /* Subindex 002: Maximum number of EtherCAT Slaves connected to the EtherCAT Master = 4080 */
    EC_T_DWORD              dwGeneralCfg;                       /*   8 */       /* Subindex 003: Available entries in objects 0x8nn0 = 0x000000FF */
    EC_T_DWORD              dwGeneralInfo;                      /*  12 */       /* Subindex 004: Available entries in objects 0x9nn0 = 0x000000F1 (if information data supported) or 0x00000000 (if information data not supported) */
} EC_PACKED(1) EC_T_OBJF000;              

/* Modular Device Profiles: EtherCAT Master  - detect modules command */
typedef struct _EC_T_OBJF002
{
    EC_T_WORD               wSubIndex0;                         /*   0 */       /* Subindex 000 */
    EC_T_BYTE               abyCmdRequest[2];                   /*   1 */       /* Subindex 001: When this subindex is written, the EtherCAT Master shall scan the EtherCAT bus and update the objects 0xF04x and 0x9nnn */
    EC_T_BYTE               byCmdStatus;                        /*   3 */       /* Subindex 002: 1: command is finished, no error
                                                                                                 3: command is finished, error
                                                                                           100-199: 0-99% of the command is done
                                                                                               255: command is executing */
    EC_T_BYTE               abyCmdResponse[6];                  /*   4 */       /* Subindex 003: Byte 0: like Subindex 2
                                                                                                 Byte 1: always 0
                                                                                                 Byte 2-3: 0: no error, > 0: vendor specific error code
                                                                                                 Byte 4-5: number of EtherCAT slaves found */
} EC_PACKED(1) EC_T_OBJF002;              

/* Modular Device Profiles: EtherCAT Master  - configured address list */
typedef struct _EC_T_OBJF02X
{
    EC_T_WORD               wSubIndex0;                         /*   0 */       /* Subindex 000 */
    EC_T_WORD               wStationAddr[255];                  /*   2 */       /* Subindex 001 - 255 */
} EC_PACKED(1) EC_T_OBJF02X;              

/* Modular Device Profiles: EtherCAT Master  - detected address list */
typedef struct _EC_T_OBJF04X
{
    EC_T_WORD               wSubIndex0;                         /*   0 */       /* Subindex 000 */
    EC_T_WORD               wStationAddr[255];                  /*   2 */       /* Subindex 001 - 255 */
} EC_PACKED(1) EC_T_OBJF04X; 
#include EC_PACKED_INCLUDESTOP/*(1)*/

#include EC_PACKED_INCLUDESTART(4)

/* EtherCAT slave properties */
typedef struct _EC_T_SLAVE_PROP
{
    EC_T_WORD   wStationAddress;        /*< station address, 
                                         *  set by XML configuration file (Config/Slave/Info/PhysAddr) */
    EC_T_WORD   wAutoIncAddr;           /*< auto increment address */
    EC_T_CHAR   achName[MAX_STD_STRLEN];/*< device name */
} EC_PACKED(4) EC_T_SLAVE_PROP;


/* EtherCAT notify parameters */
typedef struct _EC_T_NOTIFYPARMS
{
    EC_T_VOID*      pCallerData;        /*< in  caller data */
    EC_T_BYTE*      pbyInBuf;           /*< in  input data buffer */
    EC_T_DWORD      dwInBufSize;        /*< in  size of input data buffer in byte */
    EC_T_BYTE*      pbyOutBuf;          /*< out output data buffer */
    EC_T_DWORD      dwOutBufSize;       /*< in  size of output data buffer in byte */
    EC_T_DWORD*     pdwNumOutData;      /*< out number of output data bytes stored in output data buffer */
} EC_PACKED(4) EC_T_NOTIFYPARMS, *PEC_T_NOTIFYPARMS;

/* EtherCAT command WKC error descriptor */
typedef struct _EC_T_WKCERR_DESC
{
    EC_T_SLAVE_PROP SlaveProp;          /*< slave properties if applicable */
    EC_T_BYTE       byCmd;              /*< EtherCAT command where the error occured */
    EC_T_BYTE       byRsvd[3];          /*< Alignment */
    EC_T_DWORD      dwAddr;             /*< logical or physical (adp/ado) address */
    EC_T_WORD       wWkcSet;            /*< working counter set value */
    EC_T_WORD       wWkcAct;            /*< working counter actual value */
} EC_PACKED(4) EC_T_WKCERR_DESC;

/* EtherCAT frame response error descriptor */
typedef enum _EC_T_FRAME_RSPERR_TYPE
{
    eRspErr_UNDEFINED       = 0,        /*< undefined */
    eRspErr_NO_RESPONSE     = 1,        /*< no response (timeout)            obsolete for acyclic frames*/
    eRspErr_WRONG_IDX       = 2,        /*< frame with wrong IDX received    obsolete for acyclic frames*/
    eRspErr_UNEXPECTED      = 3,        /*< unexpected frame received */
    eRspErr_FRAME_RETRY     = 4,        /*< ethernet frame has to be re-sent due to an timeout (only acyclic frames)  obsolete*/
    eRspErr_RETRY_FAIL      = 5,        /*< all retry mechanism fails to re-sent acyclic frames*/
    /* Borland C++ datatype alignment correction */
    eRspErr_BCppDummy   = 0xFFFFFFFF
} EC_T_FRAME_RSPERR_TYPE;

typedef struct _EC_T_FRAME_RSPERR_DESC
{
    EC_T_BOOL               bIsCyclicFrame;         /*< EC_TRUE if it is a frame containing cyclic commands */
    EC_T_FRAME_RSPERR_TYPE  EErrorType;             /*< type of error */
    EC_T_BYTE               byEcCmdHeaderIdxSet;    /*< Expected index value */
    EC_T_BYTE               byEcCmdHeaderIdxAct;    /*< Actual index value */
} EC_PACKED(4) EC_T_FRAME_RSPERR_DESC;

/* EtherCAT init command response error descriptor */
typedef enum _EC_T_INITCMD_ERR_TYPE
{
    eInitCmdErr_UNDEFINED        = 0,    /*< undefined */
    eInitCmdErr_NO_RESPONSE      = 1,    /*< no response */
    eInitCmdErr_VALIDATION_ERR   = 2,    /*< validation error */
    eInitCmdErr_FAILED           = 3,    /*< init command failed, requested state could not be reached */

    /* Borland C++ datatype alignment correction */
    eInitCmdErr_BCppDummy       = 0xFFFFFFFF
} EC_T_INITCMD_ERR_TYPE;

typedef struct _EC_T_INITCMD_ERR_DESC
{
    EC_T_SLAVE_PROP       SlaveProp;                              /*< slave properties */
    EC_T_CHAR             achStateChangeName[MAX_SHORT_STRLEN];   /*< description of the current ecat state change */
    EC_T_INITCMD_ERR_TYPE EErrorType;                             /*< type of error */
    EC_T_CHAR             szComment[MAX_STD_STRLEN];              /*< comment (ENI) */
} EC_PACKED(4) EC_T_INITCMD_ERR_DESC;

/* EtherCAT missing command descriptor */
typedef struct _EC_T_MISSCMD_DESC
{
    EC_T_SLAVE_PROP SlaveProp;          /*< slave properties */
    EC_T_DWORD      dwCmdIndex;         /*< ethercat command/telegram index in ethernet frame that is missing */
    EC_T_BOOL       bMasterCmd;         /*< EC_TRUE if it is a master command or EC_FALSE if it is a slave command */
} EC_PACKED(4) EC_T_MISSCMD_DESC;

/* EtherCAT Slave error status info descriptor */
typedef struct _EC_T_SLAVE_ERROR_INFO_DESC
{
    EC_T_SLAVE_PROP SlaveProp;          /*< slave properties */
    EC_T_WORD       wStatus;            /*< Slave Status (AL Status) */
    EC_T_WORD       wStatusCode;        /*< Error status code (AL Control Status) */
} EC_PACKED(4) EC_T_SLAVE_ERROR_INFO_DESC;

/* Slaves error status descriptor */
#ifndef MAX_SLAVES_ERROR_NTFY_ENTRIES
#define MAX_SLAVES_ERROR_NTFY_ENTRIES 128
#endif
typedef struct _EC_T_SLAVES_ERROR_DESC_ENTRY
{
    EC_T_WORD wStationAddress;
    EC_T_WORD wStatus;                /*< Slave Status (AL Status) */
    EC_T_WORD wStatusCode;            /*< Error status code (AL Control Status) */
    EC_T_WORD wRes;
} EC_PACKED(4) EC_T_SLAVES_ERROR_DESC_ENTRY;
typedef struct _EC_T_SLAVES_ERROR_DESC
{
    EC_T_WORD wCount;
    EC_T_WORD wRes;
    EC_T_SLAVES_ERROR_DESC_ENTRY SlaveError[MAX_SLAVES_ERROR_NTFY_ENTRIES];
} EC_PACKED(4) EC_T_SLAVES_ERROR_DESC;

/* EtherCAT Sdo abort */
typedef struct _EC_T_MBOX_SDO_ABORT_DESC
{
    EC_T_SLAVE_PROP SlaveProp;          /*< slave properties */
    EC_T_DWORD      dwErrorCode;        /*< error code EC_E_ */
    EC_T_WORD       wObjIndex;          /*< SDO object index */
    EC_T_BYTE       bySubIndex;         /*< SDO object sub index */
} EC_PACKED(4) EC_T_MBOX_SDO_ABORT_DESC;

/* EtherCAT FoE error */
typedef struct _EC_T_MBOX_FOE_ABORT_DESC
{
    EC_T_SLAVE_PROP SlaveProp;          /*< slave properties */
    EC_T_DWORD      dwErrorCode;        /*< error code EC_E_ */
    EC_T_CHAR       achErrorString[MAX_STD_STRLEN];   /*< FoE error string */
} EC_PACKED(4) EC_T_MBOX_FOE_ABORT_DESC;

/* Invalid mailbox data received error */
typedef struct _EC_T_MBXRCV_INVALID_DATA_DESC
{
    EC_T_SLAVE_PROP SlaveProp;          /*< slave properties */
} EC_PACKED(4) EC_T_MBXRCV_INVALID_DATA_DESC;

/* PDI Watchdog expired */
typedef struct _EC_T_PDIWATCHDOG_DESC
{
    EC_T_SLAVE_PROP SlaveProp;          /*< slave properties */
} EC_PACKED(4) EC_T_PDIWATCHDOG_DESC;

/* Slave not supported */
typedef struct _EC_T_SLAVE_NOTSUPPORTED_DESC
{
    EC_T_SLAVE_PROP SlaveProp;          /*< slave properties */
} EC_PACKED(4) EC_T_SLAVE_NOTSUPPORTED_DESC;

/* Slave in unexpected state descriptor */
typedef struct _EC_T_SLAVE_UNEXPECTED_STATE_DESC
{
    EC_T_SLAVE_PROP SlaveProp;          /*< slave properties */
    EC_T_STATE      curState;
    EC_T_STATE      expState;
} EC_PACKED(4) EC_T_SLAVE_UNEXPECTED_STATE_DESC;

/* Slaves in unexpected state descriptor */ 
#ifndef MAX_SLAVES_UNEXPECTED_STATE_NTFY_ENTRIES
#define MAX_SLAVES_UNEXPECTED_STATE_NTFY_ENTRIES 128
#endif
typedef struct _EC_T_SLAVES_UNEXPECTED_STATE_DESC_ENTRY
{
    EC_T_WORD  wStationAddress;
    EC_T_STATE curState;
    EC_T_STATE expState;
} EC_PACKED(4) EC_T_SLAVES_UNEXPECTED_STATE_DESC_ENTRY;
typedef struct _EC_T_SLAVES_UNEXPECTED_STATE_DESC
{
    EC_T_WORD wCount;
    EC_T_WORD wRes;
    EC_T_SLAVES_UNEXPECTED_STATE_DESC_ENTRY SlaveStates[MAX_SLAVES_UNEXPECTED_STATE_NTFY_ENTRIES];
} EC_PACKED(4) EC_T_SLAVES_UNEXPECTED_STATE_DESC;

/* Slave EEPROM checksum error */
typedef struct _EC_T_EEPROM_CHECKSUM_ERROR_DESC
{
    EC_T_SLAVE_PROP SlaveProp;          /*< slave properties */
} EC_PACKED(4) EC_T_EEPROM_CHECKSUM_ERROR_DESC;

/* Redundancy break/fixed notification */
typedef struct _EC_T_RED_CHANGE_DESC
{
    EC_T_WORD       wNumOfSlavesMain;    /*< Number of Slaves on Main Line */
    EC_T_WORD       wNumOfSlavesRed;     /*< Number of Slaves on Red Line  */
} EC_PACKED(4) EC_T_RED_CHANGE_DESC;

typedef struct _EC_T_JUNCTION_RED_CHANGE_DESC
{
    EC_T_SLAVE_PROP SlaveProp;          /*< slave properties of the slave with disconnected port 0 */
    EC_T_BOOL       bLineBreak;         /*< EC_TRUE for line break, EC_FALSE for line fixed */
} EC_PACKED(4) EC_T_JUNCTION_RED_CHANGE_DESC;

/* Distributed clocks (DC) Reference Clock Presence */
typedef struct _EC_T_REFCLOCK_PRESENCE_NTFY_DESC
{
    EC_T_BOOL  bPresent;
    EC_T_SLAVE_PROP SlaveProp;          /*< slave properties if present */
} EC_PACKED(4) EC_T_REFCLOCK_PRESENCE_NTFY_DESC;

/* Distributed clocks (DC) */
typedef struct _EC_T_DC_SYNC_NTFY_DESC
{
    EC_T_DWORD      IsInSync;           /*< EC_TRUE if all meant objects are in defined sync range, EC_FALSE otherwise */
    EC_T_DWORD      IsNegative;         /*< Deviation value is a negative value */
    EC_T_DWORD      dwDeviation;        /*< absolute deviation in nsecs (depending on notification nsec / wire or'ed value */
    EC_T_SLAVE_PROP SlaveProp;          /*< slave properties in case of "not in sync" */
} EC_PACKED(4) EC_T_DC_SYNC_NTFY_DESC;

/* Distributed clocks master sync (DCM) */
typedef struct _EC_T_DCM_SYNC_NTFY_DESC
{
    EC_T_DWORD      IsInSync;           /*< EC_TRUE if time of master and reference clock are in sync */
    EC_T_INT        nCtlErrorNsecCur;   /*< current DCM controller error in nsec */
    EC_T_INT        nCtlErrorNsecAvg;   /*< average DCM controller error in nsec */
    EC_T_INT        nCtlErrorNsecMax;   /*< maximum DCM controller error in nsec */
} EC_PACKED(4) EC_T_DCM_SYNC_NTFY_DESC;

/* Distributed clocks master external sync (DCX) */
typedef struct _EC_T_DCX_SYNC_NTFY_DESC
{
    EC_T_DWORD      IsInSync;           /*< EC_TRUE if external(other Ethercat segment) and internal reference clock are in sync respectivley */
    EC_T_INT        nCtlErrorNsecCur;   /*< current DCX controller error in nsec */
    EC_T_INT        nCtlErrorNsecAvg;   /*< average DCX controller error in nsec */
    EC_T_INT        nCtlErrorNsecMax;   /*< maximum DCX controller error in nsec */
    EC_T_INT64      nTimeStampDiff;     /*< difference between external and internal time stamp */
    EC_T_DWORD      dwErrorCode;        /*< Dcx external clock error code */
} EC_PACKED(4) EC_T_DCX_SYNC_NTFY_DESC;

/* Slave state change descriptor */
typedef struct _EC_T_SLAVE_STATECHANGED_NTFY_DESC
{
    EC_T_SLAVE_PROP SlaveProp;          /*< slave properties */
    EC_T_STATE      newState;           /*< new state */
} EC_PACKED(4) EC_T_SLAVE_STATECHANGED_NTFY_DESC;
#include EC_PACKED_INCLUDESTOP/*(4)*/

#include EC_PACKED_INCLUDESTART(2)
/* Slaves state change descriptor */
#ifndef MAX_SLAVES_STATECHANGED_NTFY_ENTRIES
#define MAX_SLAVES_STATECHANGED_NTFY_ENTRIES 128
#endif
typedef struct _EC_T_SLAVES_STATECHANGED_NTFY_DESC_ENTRY
{
    EC_T_WORD  wStationAddress;
    EC_T_BYTE  byState;
} EC_PACKED(2) EC_T_SLAVES_STATECHANGED_NTFY_DESC_ENTRY;

typedef struct _EC_T_SLAVES_STATECHANGED_NTFY_DESC
{
    EC_T_WORD wCount;
    EC_T_SLAVES_STATECHANGED_NTFY_DESC_ENTRY SlaveStates[MAX_SLAVES_STATECHANGED_NTFY_ENTRIES];
} EC_PACKED(2) EC_T_SLAVES_STATECHANGED_NTFY_DESC;
#define SIZEOF_EC_T_SLAVES_STATECHANGED_NTFY_DESC(wCount) (sizeof(EC_T_WORD)+wCount*sizeof(EC_T_SLAVES_STATECHANGED_NTFY_DESC_ENTRY))
#include EC_PACKED_INCLUDESTOP/*(2)*/

#include EC_PACKED_INCLUDESTART(4)

typedef struct _EC_T_FRAMELOSS_AFTER_SLAVE_NTFY_DESC
{
    /* Location of frameloss */
    EC_T_SLAVE_PROP SlaveProp;  /*< slave properties */
    EC_T_WORD       wPort;      /*< port */
} EC_PACKED(4) EC_T_FRAMELOSS_AFTER_SLAVE_NTFY_DESC;

typedef struct _EC_T_DC_BURSTCONFIG_REQ
{
    EC_T_DWORD	dwTotalBurstLength;
    EC_T_DWORD	dwBurstBulk;
} EC_PACKED(4) EC_T_DC_BURSTCONFIG_REQ;

typedef struct _EC_T_DC_SHIFTSYSTIME_DESC
{
    EC_T_DWORD	    dwCyclesToShift;    /*< Amount of cycles to apply shift */
    EC_T_INT        nShiftTime;         /*< Shift Time in nsecs (signed) */
} EC_PACKED(4) EC_T_DC_SHIFTSYSTIME_DESC;

typedef  struct _EC_T_DC_STARTCYCSAFETY_DESC    /*< Start SYNC Cyc Safety in nsec (64Bit)*/
{
    EC_T_DWORD      dwStartCycSafetyLo;
    EC_T_DWORD      dwStartCycSafetyHi;
}   EC_T_DC_STARTCYCSAFETY_DESC;
           
typedef struct _EC_T_SB_STATUS_NTFY_DESC
{
    EC_T_DWORD      dwResultCode;       /*< Result of scanbus */
    EC_T_DWORD      dwSlaveCount;       /*< Number of slaves handles during scanbus */
} EC_PACKED(4) EC_T_SB_STATUS_NTFY_DESC;

typedef struct _EC_T_SB_MISMATCH_DESC
{
    /* Location of mismatch */
    EC_T_WORD           wPrevFixedAddress;      /*< Previous fixed address */
    EC_T_WORD           wPrevPort;              /*< Previous port */
    EC_T_WORD           wPrevAIncAddress;       /*< Previous Auto-Inc address */

    /* Unexpected bus slave */
    EC_T_WORD           wBusAIncAddress;        /*< Bus Auto-Inc address */
    EC_T_DWORD          dwBusVendorId;          /*< Bus Vendor ID */
    EC_T_DWORD          dwBusProdCode;          /*< Bus Product code */
    EC_T_DWORD          dwBusRevisionNo;        /*< Bus Revision Number */
    EC_T_DWORD          dwBusSerialNo;          /*< Bus Serial Number */
    EC_T_WORD           wBusFixedAddress;       /*< Bus Station Address */
    EC_T_WORD           wIdentificationVal;     /*< Bus Last read identification value */

    /* Missing config slave */
    EC_T_WORD           wCfgFixedAddress;       /*< Config Station Address */
    EC_T_WORD           wCfgAIncAddress;        /*< Config Auto-Increment Address */
    EC_T_DWORD          dwCfgVendorId;          /*< Config Vendor ID */
    EC_T_DWORD          dwCfgProdCode;          /*< Config Product code */
    EC_T_DWORD          dwCfgRevisionNo;        /*< Config Revision Number */
    EC_T_DWORD          dwCfgSerialNo;          /*< Config Serial Number */

    /* Hot connect */
    EC_T_BOOL           bIdentValidationError;  /*< HotConnect Identification command validation error */
    EC_T_WORD           oIdentCmdHdr[5];        /*< HotConnect Identification command header (if bIdentValidationError) */
    EC_T_DWORD          dwCmdData;              /*< First DWORD of Data portion in identify command (received data) */
    EC_T_DWORD          dwCmdVMask;             /*< First DWORD of Validation mask in identify command */
    EC_T_DWORD          dwCmdVData;             /*< First DWORD of Validation data in identify command (expected data) */
} EC_PACKED(4) EC_T_SB_MISMATCH_DESC;

typedef struct _EC_T_LINE_CROSSED_DESC
{
    EC_T_SLAVE_PROP     SlaveProp;       /*< slave properties */
    EC_T_WORD           wInputPort;      /*< port where frame was received */
} EC_PACKED(4) EC_T_LINE_CROSSED_DESC;

typedef struct _EC_T_HC_DETECTALLGROUP_NTFY_DESC
{
    EC_T_DWORD  dwResultCode;       /*< Result of Group detection */
    EC_T_DWORD  dwGroupCount;       /*< Number of Groups */
    EC_T_DWORD  dwGroupsPresent;    /*< Number of connected groups */
    EC_T_DWORD  dwGroupMask;        /*< Bitmask of first 32   Groups 1 = present, 0 = absent */
    EC_T_DWORD  adwGroupMask[100];  /*< Bitmask of first 3200 Groups */
} EC_PACKED(4) EC_T_HC_DETECTALLGROUP_NTFY_DESC;

typedef struct _EC_T_RAWCMDRESPONSE_NTFY_DESC
{
    EC_T_DWORD	dwInvokeId;
    EC_T_DWORD	dwResult;
    EC_T_DWORD	dwWkc;
    EC_T_DWORD	dwCmdIdx;
    EC_T_DWORD	dwAddr;
    EC_T_DWORD	dwLength;
    EC_T_BYTE*  pbyData;
} EC_PACKED(4) EC_T_RAWCMDRESPONSE_NTFY_DESC;
                                                   
#ifdef INCLUDE_COE_PDO_SUPPORT
typedef struct _EC_T_TX_PDO_NTFY_DESC
{
    EC_T_DWORD  wPhysAddr;          /* station address */
    EC_T_DWORD  dwNumber;           /* PDO number */
    EC_T_DWORD  wLen;               /* PDO size */
    EC_T_BYTE*  pbyData;
} EC_PACKED(4) EC_T_TX_PDO_NTFY_DESC;
#endif

typedef struct _EC_T_SLAVE_LINKMSG_DESC
{
    EC_T_DWORD  dwSlaveId;          /* Slave Id */
    EC_T_BOOL   bEnableLogging;     /* EC_TRUE=> Enable LinkMsgs, EC_FALSE=> Disable */
} EC_PACKED(4) EC_T_SLAVE_LINKMSG_DESC;
                                      

/* EtherCAT state change */                         
typedef struct _EC_T_STATECHANGE                    
{                                                   
    EC_T_STATE    oldState;                         /*< old state */
    EC_T_STATE    newState;                         /*< new state */
} EC_PACKED(4) EC_T_STATECHANGE;                                 

typedef struct _EC_T_HC_SLAVE_CHANGE_DESC
{
    EC_T_SLAVE_PROP     SlaveProp;
} EC_PACKED(4) EC_T_HC_SLAVE_CHANGE_DESC;
#include EC_PACKED_INCLUDESTOP/*(4)*/

#include EC_PACKED_INCLUDESTART(2)
/* Slaves presence descriptor */
#ifndef MAX_SLAVES_PRESENCE_NTFY_ENTRIES
#define MAX_SLAVES_PRESENCE_NTFY_ENTRIES 128
#endif
typedef struct _EC_T_SLAVE_PRESENCE_NTFY_DESC
{
    EC_T_WORD  wStationAddress;
    EC_T_BYTE  bPresent;
} EC_PACKED(2) EC_T_SLAVE_PRESENCE_NTFY_DESC;
typedef struct _EC_T_SLAVES_PRESENCE_NTFY_DESC
{
    EC_T_WORD wCount;
    EC_T_SLAVE_PRESENCE_NTFY_DESC SlavePresence[MAX_SLAVES_PRESENCE_NTFY_ENTRIES];
} EC_PACKED(2) EC_T_SLAVES_PRESENCE_NTFY_DESC;
#define SIZEOF_EC_T_SLAVES_PRESENCE_NTFY_DESC(wCount) (sizeof(EC_T_WORD)+wCount*sizeof(EC_T_SLAVE_PRESENCE_NTFY_DESC))
#include EC_PACKED_INCLUDESTOP/*(2)*/

#include EC_PACKED_INCLUDESTART(4)
/* EtherCAT notification descriptor */
typedef struct _EC_T_NOTIFICATION_DESC
{
    union _EC_T_NOTIFICATION_PARM
    {                
        EC_T_DWORD                           StatusCode;               /* Generic status code only notification */
        EC_T_REFCLOCK_PRESENCE_NTFY_DESC     RefClockPresenceNtfyDesc; /* DC Reference Clock Presence Notification descriptor */
        EC_T_DC_SYNC_NTFY_DESC               SyncNtfyDesc;             /* DC Master / Slave Sync Notification descriptor */
        EC_T_RAWCMDRESPONSE_NTFY_DESC        RawCmdRespNtfyDesc;       /* Queue Raw Cmd Response notification descriptor */
        EC_T_SB_STATUS_NTFY_DESC             ScanBusNtfyDesc;          /* Scanbus Result notification descriptor */
        EC_T_SB_MISMATCH_DESC                ScanBusMismatch;          /* Scan Bus Mismatch notification descriptor */
        EC_T_STATECHANGE                     StatusChngNtfyDesc;       /* Master EtherCAT State changed notification */
#ifdef INCLUDE_COE_PDO_SUPPORT                                         
        EC_T_TX_PDO_NTFY_DESC                TxPdoNtfyDesc;            /* TxPDO transfer notification */
#endif                                                                 
#if (defined INCLUDE_HOTCONNECT)                                       
        EC_T_HC_DETECTALLGROUP_NTFY_DESC     HCDetAllGrps;             /* HC Group Detection */
        EC_T_HC_SLAVE_CHANGE_DESC            HCSlvChgDesc;             /* HotConnect Slave State Change. Obsolete, see SlavePresenceDesc. */
#endif                                                                 
        EC_T_SLAVE_PRESENCE_NTFY_DESC        SlavePresenceDesc;        /* Slave (dis-)appeared */
        EC_T_SLAVES_PRESENCE_NTFY_DESC       SlavesPresenceDesc;       /* Slaves (dis-)appeared */
        EC_T_LINE_CROSSED_DESC               CrossedLineDesc;          /* Line crossed */
        EC_T_DCM_SYNC_NTFY_DESC              DcmInSyncDesc;            
        EC_T_DCX_SYNC_NTFY_DESC              DcxInSyncDesc;            
        EC_T_SLAVE_STATECHANGED_NTFY_DESC    SlaveStateChangedDesc;    /* Slave finished successfully state transition descriptor */
        EC_T_SLAVES_STATECHANGED_NTFY_DESC   SlavesStateChangedDesc;   /* Slaves finished successfully state transition descriptor */
    } EC_PACKED(4) desc;
} EC_PACKED(4) EC_T_NOTIFICATION_DESC;

/* EtherCAT error notification descriptor */
typedef struct _EC_T_ERROR_NOTIFICATION_DESC
{
    EC_T_DWORD  dwNotifyErrorCode;                  /* notification code */
    EC_T_CHAR   achErrorInfo[MAX_ERRINFO_STRLEN];   /* additional error information string (NULL terminated) */
    union _EC_T_ERROR_NOTIFICATION_PARM
    {
        EC_T_WKCERR_DESC                        WkcErrDesc;                 /* WKC error descriptor */
        EC_T_FRAME_RSPERR_DESC                  FrameRspErrDesc;            /* Frame response error descriptor */
        EC_T_INITCMD_ERR_DESC                   InitCmdErrDesc;             /* Master/Slave init command error descriptor */
        EC_T_MISSCMD_DESC                       MissCmdDesc;                /* EtherCAT missing command descriptor */
        EC_T_SLAVE_ERROR_INFO_DESC              SlaveErrInfoDesc;           /* Slave Error Info Descriptor */
        EC_T_SLAVES_ERROR_DESC                  SlavesErrDesc;              /* Slaves Error Descriptor */
        EC_T_MBOX_SDO_ABORT_DESC                SdoAbortDesc;               /* SDO Abort */
        EC_T_RED_CHANGE_DESC                    RedChangeDesc;              /* Redundancy Descriptor */
        EC_T_MBOX_FOE_ABORT_DESC                FoeErrorDesc;               /* FoE error code and string */
        EC_T_MBXRCV_INVALID_DATA_DESC           MbxRcvInvalidDataDesc;      /* Invalid mailbox data received descriptor */
        EC_T_PDIWATCHDOG_DESC                   PdiWatchdogDesc;            /* PDI Watchodg expired */
        EC_T_SLAVE_NOTSUPPORTED_DESC            SlaveNotSupportedDesc;      /* Slave not supported */
        EC_T_SLAVE_UNEXPECTED_STATE_DESC        SlaveUnexpectedStateDesc;   /* Slave in unexpected state */
        EC_T_SLAVES_UNEXPECTED_STATE_DESC       SlavesUnexpectedStateDesc;  /* Slaves in unexpected state */
        EC_T_EEPROM_CHECKSUM_ERROR_DESC         EEPROMChecksumErrorDesc;    /* EEPROM checksum error */
        EC_T_JUNCTION_RED_CHANGE_DESC           JunctionRedChangeDesc;      /* Junction redundancy change descriptor */
        EC_T_FRAMELOSS_AFTER_SLAVE_NTFY_DESC    FramelossAfterSlaveDesc;    /* Frameloss after Slave descriptor */
    } EC_PACKED(4) desc;
} EC_PACKED(4) EC_T_ERROR_NOTIFICATION_DESC;

#define SIZEOF_EC_T_ERROR_NOTIFICATION_HEADER (sizeof(EC_T_DWORD)/*dwNotifyErrorCode*/+8/*achErrorInfo*/)

#define SIZEOF_EC_T_ERROR_NOTIFICATION_WKCERR                       (SIZEOF_EC_T_ERROR_NOTIFICATION_HEADER+sizeof(EC_T_WKCERR_DESC))
#define SIZEOF_EC_T_ERROR_NOTIFICATION_FRAME_RSPERR                 (SIZEOF_EC_T_ERROR_NOTIFICATION_HEADER+sizeof(EC_T_FRAME_RSPERR_DESC))
#define SIZEOF_EC_T_ERROR_NOTIFICATION_SLAVE_INITCMD_RESPONSE_ERROR (SIZEOF_EC_T_ERROR_NOTIFICATION_HEADER+sizeof(EC_T_INITCMD_ERR_DESC))
#define SIZEOF_EC_T_ERROR_NOTIFICATION_MISSCMD                      (SIZEOF_EC_T_ERROR_NOTIFICATION_HEADER+sizeof(EC_T_MISSCMD_DESC))
#define SIZEOF_EC_T_ERROR_NOTIFICATION_SLAVE_ERROR_INFO             (SIZEOF_EC_T_ERROR_NOTIFICATION_HEADER+sizeof(EC_T_SLAVE_ERROR_INFO_DESC))
#define SIZEOF_EC_T_ERROR_NOTIFICATION_MBOX_SDO_ABORT               (SIZEOF_EC_T_ERROR_NOTIFICATION_HEADER+sizeof(EC_T_MBOX_SDO_ABORT_DESC))
#define SIZEOF_EC_T_ERROR_NOTIFICATION_RED_CHANGE                   (SIZEOF_EC_T_ERROR_NOTIFICATION_HEADER+sizeof(EC_T_RED_CHANGE_DESC))
#define SIZEOF_EC_T_ERROR_NOTIFICATION_MBOX_FOE_ABORT               (SIZEOF_EC_T_ERROR_NOTIFICATION_HEADER+sizeof(EC_T_MBOX_FOE_ABORT_DESC))
#define SIZEOF_EC_T_ERROR_NOTIFICATION_MBXRCV_INVALID_DATA          (SIZEOF_EC_T_ERROR_NOTIFICATION_HEADER+sizeof(EC_T_MBXRCV_INVALID_DATA_DESC))
#define SIZEOF_EC_T_ERROR_NOTIFICATION_PDIWATCHDOG                  (SIZEOF_EC_T_ERROR_NOTIFICATION_HEADER+sizeof(EC_T_PDIWATCHDOG_DESC))
#define SIZEOF_EC_T_ERROR_NOTIFICATION_SLAVE_NOTSUPPORTED           (SIZEOF_EC_T_ERROR_NOTIFICATION_HEADER+sizeof(EC_T_SLAVE_NOTSUPPORTED_DESC))
#define SIZEOF_EC_T_ERROR_NOTIFICATION_SLAVE_UNEXPECTED_STATE       (SIZEOF_EC_T_ERROR_NOTIFICATION_HEADER+sizeof(EC_T_SLAVE_UNEXPECTED_STATE_DESC))
#define SIZEOF_EC_T_ERROR_NOTIFICATION_EEPROM_CHECKSUM_ERROR        (SIZEOF_EC_T_ERROR_NOTIFICATION_HEADER+sizeof(EC_T_EEPROM_CHECKSUM_ERROR_DESC))
#define SIZEOF_EC_T_ERROR_NOTIFICATION_JUNCTION_RED_CHANGE          (SIZEOF_EC_T_ERROR_NOTIFICATION_HEADER+sizeof(EC_T_JUNCTION_RED_CHANGE_DESC))
#define SIZEOF_EC_T_ERROR_NOTIFICATION_FRAMELOSS_AFTER_SLAVE        (SIZEOF_EC_T_ERROR_NOTIFICATION_HEADER+sizeof(EC_T_FRAMELOSS_AFTER_SLAVE_NTFY_DESC))

#define SIZEOF_EC_T_ERROR_NOTIFICATION_SLAVES_UNEXPECTED_STATE_DESC(wCount) \
    (SIZEOF_EC_T_ERROR_NOTIFICATION_HEADER+sizeof(EC_T_WORD)/*wCount*/+sizeof(EC_T_WORD)/*wRes*/+wCount*sizeof(EC_T_SLAVES_UNEXPECTED_STATE_DESC_ENTRY))

#define SIZEOF_EC_T_ERROR_NOTIFICATION_SLAVES_ERROR_DESC(wCount) \
    (SIZEOF_EC_T_ERROR_NOTIFICATION_HEADER+sizeof(EC_T_WORD)/*wCount*/+sizeof(EC_T_WORD)/*wRes*/+wCount*sizeof(EC_T_SLAVES_ERROR_DESC_ENTRY))

typedef enum _EC_T_CNF_TYPE
{
    eCnfType_Unknown            = 0,
    eCnfType_Filename           = 1,
    eCnfType_Data               = 2,
    eCnfType_Datadiag           = 3,                /* only for diagnosis. ENI data is not stored */
    eCnfType_GenPreopENI        = 4,                /* generate mini ENI based on bus-scan result to get into preop state */
    eCnfType_GenPreopENIWithCRC = 5,                /* same as eCnfType_GenPreopENI, but also generates CRC */
    eCnfType_GenOpENI           = 6,                /* generate ENI based on bus-scan result to get into op state */
    eCnfType_None               = 7,                /* no configuration used */
    eCnfType_ConfigData         = 8,

    /* Borland C++ datatype alignment correction */
    eCnfType_BCppDummy   = 0xFFFFFFFF
} EC_T_CNF_TYPE;

typedef EC_T_VOID (*EC_T_PFMEMREQ)(EC_T_PVOID pvContext, EC_T_PBYTE* ppbyPDData);
typedef EC_T_VOID (*EC_T_PFMEMREL)(EC_T_PVOID pvContext);

/* Descriptor for EC_IOCTL_REGISTER_PDMEMORYPROVIDER */
typedef struct _EC_T_MEMPROV_DESC
{
    EC_T_PVOID      pvContext;                      /* Context pointer. This pointer is used each time one of
                                                     * the callback functions is called. 
                                                     */
    EC_T_BOOL       bUseMasterShadowInUserSyncNowaitMode;
                                                    /* This flag determines whether the master shall use an 
                                                     * internally allocated shadow memory where new input 
                                                     * process data will be stored when received from 
                                                     * the link layer. Only valid in 
                                                     * eSyncMode_USER_SYNC_NOWAIT.
                                                     */
    EC_T_PBYTE      pbyPDOutData;                   /* Pointer to the default output process data buffer */
    EC_T_DWORD      dwPDOutDataLength;              /* Length of the output process data buffer */

    EC_T_PBYTE      pbyPDInData;                    /* Pointer to the default input process data buffer */
    EC_T_DWORD      dwPDInDataLength;               /* Length of the input  process data buffer */

#ifdef INCLUDE_MEMORY_PROVIDER
    EC_T_PFMEMREQ   pfPDOutDataReadRequest;         /* This function will be called by the master within the 
                                                     * process data transfer cycle prior to read data.
                                                     */
    EC_T_PFMEMREL   pfPDOutDataReadRelease;         /* This function will be called after all data was read 
                                                     * from output process data buffer.
                                                     */

    EC_T_PFMEMREQ   pfPDInDataWriteRequest;         /* This function will be called by the master within the 
                                                     * process data transfer cycle prior to write data.
                                                     */
    EC_T_PFMEMREL   pfPDInDataWriteRelease;         /* This function will be called after all data was written
                                                     * to input process data buffer.
                                                     */
#endif
} EC_PACKED(4) EC_T_MEMPROV_DESC, *EC_PT_MEMPROV_DESC;

typedef struct _EC_T_SB_SLAVEINFO_DESC
{
    EC_T_DWORD  dwScanBusStatus;                    /* Status during last Bus Scan */
    
    EC_T_DWORD	dwVendorId;                         /* Vendor Identification */
    EC_T_DWORD	dwProductCode;                      /* Product Code */
    EC_T_DWORD	dwRevisionNumber;                   /* Revision Number */
    EC_T_DWORD	dwSerialNumber;                     /* Serial Number */
} EC_PACKED(4) EC_T_SB_SLAVEINFO_DESC, *EC_PT_SB_SLAVEINFO_DESC;

typedef enum _EC_T_eEEPENTRY
{
    eEEP_VendorId	        = ESC_SII_REG_VENDORID,
    eEEP_ProductCode        = ESC_SII_REG_PRODUCTCODE,
    eEEP_RevisionNumber     = ESC_SII_REG_REVISIONNUMBER,
    eEEP_SerialNumber       = ESC_SII_REG_SERIALNUMBER,
    eEEP_BootRcvMbx         = ESC_SII_REG_BOOT_RECV_MBX_OFFSET,
    eEEP_BootSndMbx         = ESC_SII_REG_BOOT_SEND_MBX_OFFSET,
    eEEP_StdRcvMbx          = ESC_SII_REG_STD_RECV_MBX_OFFSET,
    eEEP_StdSndMbx          = ESC_SII_REG_STD_SEND_MBX_OFFSET,
    eEEP_MbxProtocol        = ESC_SII_REG_MBX_PROTOCOL,
    eEEP_AliasAddress       = ESC_SII_REG_ALIASADDRESS,

    /* Borland C++ datatype alignment correction */
    eEEP_BCppDummy          = 0xFFFFFFFF
} EC_T_eEEPENTRY;

#define BT_CHECK_EEPENTRY_VENDORID      EC_TRUE
#define BT_CHECK_EEPENTRY_PRODUCTCODE   EC_TRUE
#define BT_CHECK_EEPENTRY_REVISIONNO    EC_TRUE
#define BT_CHECK_EEPENTRY_SERIALNO      EC_TRUE

typedef struct _EC_T_SCANBUS_PROP_DESC
{
    EC_T_eEEPENTRY  eEEPROMEntry;
    EC_T_DWORD      dwVerify;
} EC_PACKED(4) EC_T_SCANBUS_PROP_DESC, *EC_PT_SCANBUS_PROP_DESC;

typedef enum _EC_T_eSBSlaveInfoType
{
    sbsit_unknown	    = 0,
    sbsit_bustopology   = 1,
    sbsit_configuration = 2,

    /* Borland C++ datatype alignment correction */
    sbsit_BCppDummy     = 0xFFFFFFFF
} EC_T_eSBSlaveInfoType;

typedef struct _EC_T_SB_SLAVEINFO_EEP_REQ_DESC
{
    EC_T_eSBSlaveInfoType       eSbSlaveInfoType;
    EC_T_DWORD                  wAutoIncAddress;
    EC_T_eEEPENTRY              eEEPROMEntry;
} EC_T_SB_SLAVEINFO_EEP_REQ_DESC;

typedef struct _EC_T_SB_SLAVEINFO_EEP_RES_DESC
{
    EC_T_DWORD                  dwScanBusStatus;
    EC_T_eEEPENTRY              eEEPROMEntry;
    EC_T_DWORD                  dwEEPROMValue;
} EC_PACKED(4) EC_T_SB_SLAVEINFO_EEP_RES_DESC;

typedef enum _EC_T_eINFOENTRY
{
    eie_unknown         =  0,       /** nothing / invalid */
    eie_pdoffs_in       =  1,       /** get process data offset of Input data (in Bits) */
    eie_pdsize_in       =  2,       /** get process data size of Input Data (in Bits) */
    eie_pdoffs_out      =  3,       /** get process data offset of Output data (in Bits) */
    eie_pdsize_out      =  4,       /** get process data size of Output Data (in Bits) */
    eie_phys_address    =  5,       /** get slave phys Address */
    eie_portstate       =  6,       /** get port link state (SB Instance) */
    eie_dcsupport       =  7,       /** slave does support DC */
    eie_dc64support     =  8,       /** slave does support 64Bit DC */
    eie_alias_address   =  9,       /** get slave alias address */
    eie_cfgphy_address  = 10,       /** get slave configured phys address */
    eie_device_name     = 11,       /** get slave name from configuration */
    eie_ismailbox_slave = 12,       /** get whether slave support mailboxes */
    eie_pdoffs_in2      = 21,       /** get process data offset of Input data (in Bits) */
    eie_pdsize_in2      = 22,       /** get process data size of Input Data (in Bits) */
    eie_pdoffs_out2     = 23,       /** get process data offset of Output data (in Bits) */
    eie_pdsize_out2     = 24,       /** get process data size of Output Data (in Bits) */
    eie_pdoffs_in3      = 31,       /** get process data offset of Input data (in Bits) */
    eie_pdsize_in3      = 32,       /** get process data size of Input Data (in Bits) */
    eie_pdoffs_out3     = 33,       /** get process data offset of Output data (in Bits) */
    eie_pdsize_out3     = 34,       /** get process data size of Output Data (in Bits) */
    eie_pdoffs_in4      = 41,       /** get process data offset of Input data (in Bits) */
    eie_pdsize_in4      = 42,       /** get process data size of Input Data (in Bits) */
    eie_pdoffs_out4     = 43,       /** get process data offset of Output data (in Bits) */
    eie_pdsize_out4     = 44,       /** get process data size of Output Data (in Bits) */
    
    eie_mbx_outsize     = 45,       /** get out mailbox 1 size */
    eie_mbx_insize      = 46,       /** get in mailbox 1 size */
    eie_mbx_outsize2    = 47,       /** get out mailbox 2 size */
    eie_mbx_insize2     = 48,       /** get in mailbox 2 size */

    eie_isoptional      = 49,       /** is slave optional */
    eie_ispresent       = 50,       /** is slave present on bus */

    eie_esctype         = 51,       /** Type of ESC controller */
    /* Borland C++ datatype alignment correction */
    eie_BCppDummy       = 0xFFFFFFFF
} EC_T_eINFOENTRY;

typedef struct _EC_T_SB_SLAVEINFO_REQ_DESC
{
    EC_T_eINFOENTRY eInfoEntry;
    EC_T_WORD       wAutoIncAddress;
} EC_PACKED(4) EC_T_SB_SLAVEINFO_REQ_DESC;

typedef struct _EC_T_SB_SLAVEINFO_RES_DESC
{
    EC_T_eINFOENTRY eInfoEntry;
    EC_T_DWORD      dwInfoLength;
    EC_T_PBYTE      pbyInfo;
} EC_PACKED(4) EC_T_SB_SLAVEINFO_RES_DESC;

typedef struct _EC_T_DCL_ENABLE_DESC
{
    EC_T_DWORD	dwVerifySyncLatchConfiguration;
    EC_T_DWORD	dwDCLInitTimeout;
} EC_PACKED(4) EC_T_DCL_ENABLE_DESC;


/* descriptor for EC_IOCTL_GET_CYCLIC_CONFIG_INFO call */
typedef struct _EC_T_CYC_CONFIG_DESC
{
    EC_T_DWORD      dwNumCycEntries;    /* total number of cyclic entries */
    EC_T_DWORD      dwTaskId;           /* task id of selected cyclic entry */
    EC_T_DWORD      dwPriority;         /* priority of selected cyclic entry */
    EC_T_DWORD      dwCycleTime;        /* cycle time of selected cyclic entry */
} EC_PACKED(4) EC_T_CYC_CONFIG_DESC;

typedef EC_T_DWORD (*EC_PF_TIMESTAMP)(              EC_T_PVOID      pCallerData,
                                                    EC_T_DWORD*     pdwHostTimeLo   );
                                                    
/* Structure carrying instantaneous values for Master Sync */
typedef struct _EC_T_ACTUALVAL
{
    EC_T_DWORD	dwBeginUpdateCnt;               /*< out call counter incremented each time structure
                                                 *      update is started */
    EC_T_DWORD  dwEndUpdateCnt;                 /*< out call counter incremented each time structure
                                                 *      update is finished */
    EC_T_DWORD	dwBusSyncFrameSendTimeLo;       /*< out Host stamped time (result from pfTimeStamp) LSDW */
    EC_T_DWORD	dwBusTimeHi;                    /*< out Bus time (result from ARMW) MSDW */
    EC_T_DWORD	dwBusTimeLo;                    /*< out Bus time (result from ARMW) LSDW */
    EC_T_DWORD	dwBusSyncFramePostSendTimeLo;   /*< out Host stamped time (result from pfTimeStamp) LSDW */
    EC_T_DWORD	dwSyncPulseGridOffsetHi;        /*< out Sync Pulse Offset (initial grid offset) */
    EC_T_DWORD	dwSyncPulseGridOffsetLo;        /*< out Sync Pulse Offset (initial grid offset) */
    EC_T_BOOL	bDcmCtlInSync;                  /*< in  DCM controller has synchronized with DC ref.clock */
    EC_T_DWORD	dwSyncPeriodLength;             /*< out bus cycle time in nsec */
    EC_T_DWORD	dwRes;                          /*< out reserved */
    EC_T_DWORD  dwLastTimeStampResult;          /*< out Last result of call to registered callback 
                                                 *      function EC_PF_TIMESTAMP */
    EC_T_BOOL   bSlavesInSync;                  /*< out Slaves are in sync = EC_TRUE; out of sync = EC_FALSE; */
    EC_T_BOOL   bResetRequest;                  /*< out If EC_TRUE, master request DCM controller reset (e.g. if reference clock disappears) */
} EC_PACKED(4) EC_T_ACTUALVAL;

/* Controller client register parameters */
typedef struct _EC_T_REGISTER_TSPARMS
{
    EC_T_VOID*	pCallerData;                    /*< in  used by all callback functions */  
    EC_PF_TIMESTAMP	pfTimeStamp;                /*< in  timestamp callback function pointer */
    EC_T_DWORD	dwUpdateMultiplier;             /*< in  Interval multiplier */
    EC_T_DWORD	dwEnableHardRealtime;           /*< in  enable hard real time in link layer */
    EC_T_DWORD	aReserved[10];                  /*< in  reserved for future use */
} EC_PACKED(4) EC_T_REGISTER_TSPARMS;

/* Controller client register result */
typedef struct _EC_T_REGISTER_TSRESULTS
{
    EC_T_DWORD          dwHardRealtimeEnabled;  /*< out Hard real time is used */
    EC_T_ACTUALVAL*     pActualValues;          /*< out pointer to data carrying current data */
} EC_PACKED(4) EC_T_REGISTER_TSRESULTS;

typedef struct _EC_T_SLVSTATISTICS_DESC
{
    EC_T_WORD       wRxErrorCounter[ESC_PORT_COUNT];     /*< out RX Error Counters per Slave Port */
    EC_T_BYTE       byFwdRxErrorCounter[ESC_PORT_COUNT]; /*< out Forwarded RX Error Counters per Slave Port */
    EC_T_BYTE       byEcatProcUnitErrorCounter;         /*< out EtherCAT Processing unit error counter */
    EC_T_BYTE       byPDIErrorCounter;                  /*< out PDI Error counter */
    EC_T_WORD       wAlStatusCode;                      /*< out AL status code    */
    EC_T_BYTE       byLostLinkCounter[ESC_PORT_COUNT];   /*< out Lost Link Counter per Slave Port */
} EC_PACKED(4) EC_T_SLVSTATISTICS_DESC;

typedef struct _EC_T_SLAVE_PORT_DESC
{
    EC_T_WORD       wSlaveAddress;
    EC_T_WORD       wPortNumber;
} EC_PACKED(4) EC_T_SLAVE_PORT_DESC;

typedef enum _EC_T_EHOTCONNECTMODE
{
    echm_unknown        = 0x0000,
    echm_manual         = 0x0001,
    echm_automatic      = 0x0002,
    echm_fullmanual     = 0x0004,
    echm_borderclose    = 0x0010,

    echm_BCppDummy =   0xFFFFFFFF
} EC_T_EHOTCONNECTMODE;
#define echm_automan_mask (0x000f)

/* textual description of HotConnect Mode */
#define HotConnectModeText(eMode)                   \
    ((eMode)==echm_unknown?"unknown":               \
     ((eMode)==echm_manual?"manual":                \
      ((eMode)==echm_automatic?"automatic":         \
       ((eMode)==echm_fullmanual?"fullmanual":      \
        ((eMode)==echm_borderclose?"borderclose":   \
          "INVALID EC_T_EHOTCONNECTMODE")))))

/* callback pointer for RX frame */ 
typedef EC_T_VOID (*EC_T_PF_RXFRAME_CB)(EC_T_VOID*);

/* ecatGetCfgSlaveInfo */
typedef struct _EC_T_CFG_SLAVE_INFO
{
    EC_T_DWORD                  dwSlaveId;              /*< out Slave ID */
    EC_T_CHAR                   abyDeviceName[ECAT_DEVICE_NAMESIZE]; /*< out Slave name */
    EC_T_DWORD                  dwHCGroupIdx;           /*< out Index of the hot connect group, 0 for mandatory */
    EC_T_BOOL                   bIsPresent;             /*< out Slave is currently present on bus */
    EC_T_BOOL                   bIsHCGroupPresent;      /*< out Slave the hot connect group of the slave is present */

    EC_T_DWORD                  dwVendorId;             /*< out Vendor identification */
    EC_T_DWORD                  dwProductCode;          /*< out Product code */
    EC_T_DWORD                  dwRevisionNumber;       /*< out Revision number */
    EC_T_DWORD                  dwSerialNumber;         /*< out Serial number */

    EC_T_WORD                   wStationAddress;        /*< out Station address */
    EC_T_WORD                   wAutoIncAddress;        /*< out Auto increment address */

    EC_T_DWORD                  dwPdOffsIn;             /*< out Process data offset of input data (ENI: ProcessData/Recv[1]/BitStart) */
    EC_T_DWORD                  dwPdSizeIn;             /*< out Process data size of input data (ENI: ProcessData/Recv[1]/BitLength) */
    EC_T_DWORD                  dwPdOffsOut;            /*< out Process data offset of output data (ENI: ProcessData/Send[1]/BitStart) */
    EC_T_DWORD                  dwPdSizeOut;            /*< out Process data size of output data (ENI: ProcessData/Send[1]/BitLength) */
    EC_T_DWORD                  dwPdOffsIn2;            /*< out Process data offset of input data (ENI: ProcessData/Recv[2]/BitStart) */
    EC_T_DWORD                  dwPdSizeIn2;            /*< out Process data size of input data (ENI: ProcessData/Recv[2]/BitLength) */
    EC_T_DWORD                  dwPdOffsOut2;           /*< out Process data offset of output data (ENI: ProcessData/Send[2]/BitStart) */
    EC_T_DWORD                  dwPdSizeOut2;           /*< out Process data size of output data (ENI: ProcessData/Send[2]/BitLength) */
    EC_T_DWORD                  dwPdOffsIn3;            /*< out Process data offset of input data (ENI: ProcessData/Recv[3]/BitStart) */
    EC_T_DWORD                  dwPdSizeIn3;            /*< out Process data size of input data (ENI: ProcessData/Recv[3]/BitLength) */
    EC_T_DWORD                  dwPdOffsOut3;           /*< out Process data offset of output data (ENI: ProcessData/Send[3]/BitStart) */
    EC_T_DWORD                  dwPdSizeOut3;           /*< out Process data size of output data (ENI: ProcessData/Send[3]/BitLength) */
    EC_T_DWORD                  dwPdOffsIn4;            /*< out Process data offset of input data (ENI: ProcessData/Recv[4]/BitStart) */
    EC_T_DWORD                  dwPdSizeIn4;            /*< out Process data size of input data (ENI: ProcessData/Recv[4]/BitLength) */
    EC_T_DWORD                  dwPdOffsOut4;           /*< out Process data offset of output data (ENI: ProcessData/Send[4]/BitStart) */
    EC_T_DWORD                  dwPdSizeOut4;           /*< out Process data size of output data (ENI: ProcessData/Send[4]/BitLength) */

    EC_T_DWORD                  dwMbxSupportedProtocols;/*< out Supported mailbox protocols */
    EC_T_DWORD                  dwMbxOutSize;           /*< out Mailbox 1 output size (ENI: Mailbox/Send) */
    EC_T_DWORD                  dwMbxInSize;            /*< out Mailbox 1 input size (ENI: Mailbox/Recv) */
    EC_T_DWORD                  dwMbxOutSize2;          /*< out Mailbox 2 output size (ENI: Mailbox/Bootstrap/Send) */
    EC_T_DWORD                  dwMbxInSize2;           /*< out Mailbox 2 input size (ENI: Mailbox/Bootstrap/Recv) */

    EC_T_BOOL                   bDcSupport;             /*< out Slave supports DC */

    EC_T_WORD                   wNumProcessVarsInp;     /*< out Number of output process data variables*/
    EC_T_WORD                   wNumProcessVarsOutp;    /*< out Number of input process data variables */

    EC_T_WORD                   wPrevStationAddress;    /*< Station address of the previous slave */
    EC_T_WORD                   wPrevPort;              /*< Connected port of the previous slave */

    EC_T_WORD                   wIdentifyAdo;           /*< out ADO used for identification command */
    EC_T_WORD                   wIdentifyData;          /*< out Identification value to be validated */
    EC_T_BYTE                   byPortDescriptor;       /*< out Port descriptor (ESC register  0x0007) */

    EC_T_BYTE                   abyReserved[3];
    EC_T_WORD                   wWkcStateDiagOffsIn[EC_CFG_SLAVE_PD_SECTIONS]; /*< out Offset of WkcState bit in diagnosis image (ENI: ProcessData/Recv[1..4]/BitStart) */
    EC_T_WORD                   wWkcStateDiagOffsOut[EC_CFG_SLAVE_PD_SECTIONS];/*< out Offset of WkcState bit in diagnosis image (ENI: ProcessData/Send[1..4]/BitStart) */
    EC_T_DWORD                  adwReserved[3];
} EC_PACKED(4) EC_T_CFG_SLAVE_INFO;

/* ecatGetBusSlaveInfo */
typedef struct _EC_T_BUS_SLAVE_INFO
{
    EC_T_DWORD                  dwSlaveId;              /*< out Slave ID */
    EC_T_DWORD                  adwPortSlaveIds[ESC_PORT_COUNT]; /*< out Slave IDs connected to ports */
    EC_T_WORD                   wPortState;             /*< out Port state */
    EC_T_WORD                   wAutoIncAddress;        /*< out Auto increment address */
    EC_T_BOOL                   bDcSupport;             /*< out Slave support DC */
    EC_T_BOOL                   bDc64Support;           /*< out Slave support 64Bit DC */

    EC_T_DWORD                  dwVendorId;             /*< out Vendor identification   (EEPROM offset 0x0008) */
    EC_T_DWORD                  dwProductCode;          /*< out Product code            (EEPROM offset 0x000A) */
    EC_T_DWORD                  dwRevisionNumber;       /*< out Revision number         (EEPROM offset 0x000C) */
    EC_T_DWORD                  dwSerialNumber;         /*< out Serial number           (EEPROM offset 0x000E) */

    EC_T_BYTE                   byESCType;              /*< out ESC type                (ESC register  0x0000) */
    EC_T_BYTE                   byESCRevision;          /*< out ESC revision            (ESC register  0x0001) */
    EC_T_WORD                   wESCBuild;              /*< out ESC build               (ESC register  0x0002) */
    EC_T_BYTE                   byPortDescriptor;       /*< out Port descriptor         (ESC register  0x0007) */
    EC_T_BYTE                   byReserved;
    EC_T_WORD                   wFeaturesSupported;     /*< out Features supported      (ESC register  0x0008) */
    EC_T_WORD                   wStationAddress;        /*< out Station address         (ESC register  0x0010) */
    EC_T_WORD                   wAliasAddress;          /*< out Alias address           (ESC register  0x0012) */
    EC_T_WORD                   wAlStatus;              /*< out AL status               (ESC register  0x0130) */
    EC_T_WORD                   wAlStatusCode;          /*< out AL status code          (ESC register  0x0132) */
    EC_T_DWORD                  dwSystemTimeDifference; /*< out System time difference  (ESC register  0x092C) */
    EC_T_WORD                   wMbxSupportedProtocols; /*< out Support mailbox         (EEPROM offset 0x001C) */
    EC_T_WORD                   wDlStatus;              /*< out DL status               (ESC register  0x0110) */
    EC_T_WORD                   wPrevPort;              /*< out Connected port of the previous slave */
    EC_T_WORD                   wIdentifyData;          /*< out Last read identification value see EC_T_CFG_SLAVE_INFO.wIdentifyAdo */
    EC_T_BOOL                   bLineCrossed;           /*< out Line crossed detected */
    EC_T_DWORD                  dwSlaveDelay;           /*< out Time delay "behind" slave in ns */
    EC_T_DWORD                  dwPropagDelay;          /*< out Propagation delay       (ESC register  0x0928) */
    EC_T_BOOL                   bIsRefClock;            /*< out Slave is reference clock */
    EC_T_BOOL                   bIsDeviceEmulation;     /*< out Slave without Firmware  (ESC register  0x0141, enabled by EEPROM offset 0x0000.8) */
    EC_T_DWORD                  adwReserved[3];

} EC_PACKED(4) EC_T_BUS_SLAVE_INFO;

/* EC_IOCTL_SB_GET_BUS_SLAVE_PORTS_INFO */
typedef struct _EC_T_BUS_SLAVE_PORTS_INFO
{
    EC_T_BYTE   byPortDescriptor;               /* ESC:0x0007 */
    EC_T_WORD   wDlStatus;                      /* ESC:0x0110 */
    EC_T_DWORD  adwRecvTime[ESC_PORT_COUNT];    /* ESC:0x0900 - 0x090C */
    EC_T_UINT64 qwRecvTimeProcessingUnit;       /* ESC:0x0918 */
} EC_PACKED(4) EC_T_BUS_SLAVE_PORTS_INFO;
#include EC_PACKED_INCLUDESTOP/*(4)*/

#include EC_PACKED_INCLUDESTART(8)
typedef struct _EC_T_TRACE_DATA_INFO
{
    EC_T_BYTE*      pbyData;    /*< [out] Process Data Image output data buffer */
    EC_T_DWORD      dwOffset;   /*< [out] Trace Data offset in Process Data Image output data buffer (in bytes) */
    EC_T_WORD       wSize;      /*< [out] Trace Data size (in bytes) */
} EC_PACKED(8) EC_T_TRACE_DATA_INFO;
#include EC_PACKED_INCLUDESTOP/*(8)*/

#include EC_PACKED_INCLUDESTART(4)
/* See also Object 0x2002  Bus Diagnosis Object */
typedef struct _EC_T_BUS_DIAGNOSIS_INFO
{
    EC_T_DWORD dwCRC32ConfigCheckSum;
    EC_T_DWORD dwNumSlavesFound;
    EC_T_DWORD dwNumDCSlavesFound;
    EC_T_DWORD dwNumCfgSlaves;
    EC_T_DWORD dwNumMbxSlaves;

    EC_T_DWORD dwTXFrames;
    EC_T_DWORD dwRXFrames;
    EC_T_DWORD dwLostFrames;

    EC_T_DWORD dwCyclicFrames;
    EC_T_DWORD dwCyclicDatagrams;
    EC_T_DWORD dwAcyclicFrames;
    EC_T_DWORD dwAcyclicDatagrams;
    EC_T_DWORD dwClearCounters;
    EC_T_DWORD dwRes[4];
} EC_PACKED(4) EC_T_BUS_DIAGNOSIS_INFO;

/* See also Object 0x2006  Mailbox Statistics Object */
typedef struct _EC_T_STATISTIC
{
    EC_T_DWORD dwTotal;
    EC_T_DWORD dwLast;
} EC_PACKED(4) EC_T_STATISTIC;
typedef struct _EC_T_STATISTIC_TRANSFER
{
    EC_T_STATISTIC Cnt;
    EC_T_STATISTIC Bytes;
} EC_PACKED(4) EC_T_STATISTIC_TRANSFER;
typedef struct _EC_T_STATISTIC_TRANSFER_DUPLEX
{
    EC_T_STATISTIC_TRANSFER Read;
    EC_T_STATISTIC_TRANSFER Write;
} EC_PACKED(4) EC_T_STATISTIC_TRANSFER_DUPLEX;
typedef struct _EC_T_MAILBOX_STATISTICS
{
    EC_T_STATISTIC_TRANSFER_DUPLEX Aoe;
    EC_T_STATISTIC_TRANSFER_DUPLEX Coe;
    EC_T_STATISTIC_TRANSFER_DUPLEX Eoe;
    EC_T_STATISTIC_TRANSFER_DUPLEX Foe;
    EC_T_STATISTIC_TRANSFER_DUPLEX Soe;
    EC_T_STATISTIC_TRANSFER_DUPLEX Voe;
    EC_T_STATISTIC_TRANSFER_DUPLEX RawMbx;
    EC_T_STATISTIC_TRANSFER_DUPLEX aRes; /* reserved */
} EC_PACKED(4) EC_T_MAILBOX_STATISTICS;

/* ecatGetMasterInfo */
typedef struct _EC_T_MASTER_INFO
{
    EC_T_DWORD              dwMasterVersion;
    EC_T_BUS_DIAGNOSIS_INFO BusDiagnosisInfo;
    EC_T_MAILBOX_STATISTICS MailboxStatistics;
    EC_T_DWORD              adwReserved[32];
} EC_PACKED(4) EC_T_MASTER_INFO;
#include EC_PACKED_INCLUDESTOP/*(4)*/

/*-COMPILER SETTINGS---------------------------------------------------------*/
#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* INC_ECINTERFACECOMMON */

/*-END OF SOURCE FILE--------------------------------------------------------*/
