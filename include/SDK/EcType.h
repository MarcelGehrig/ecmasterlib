/*-----------------------------------------------------------------------------
 * EcType.h
 * Copyright                acontis technologies GmbH, Weingarten, Germany
 * Response                 Stefan Zintgraf
 * Description              EtherCAT Master type definitions
 *---------------------------------------------------------------------------*/

#ifndef INC_ECTYPE
#define INC_ECTYPE

/*-TYPEDEFS------------------------------------------------------------------*/
#define EC_T_VOID void
typedef void*           EC_T_PVOID;

typedef int             EC_T_BOOL;

typedef char            EC_T_CHAR;
typedef unsigned short  EC_T_WCHAR;

typedef unsigned char   EC_T_BYTE, *EC_T_PBYTE;
typedef unsigned short  EC_T_WORD;
typedef unsigned int    EC_T_DWORD;

typedef signed char	    EC_T_SBYTE;
typedef signed short    EC_T_SWORD;
typedef signed int      EC_T_SDWORD;

typedef int             EC_T_INT;
typedef unsigned int    EC_T_UINT;

typedef short           EC_T_SHORT;
typedef unsigned short  EC_T_USHORT;

typedef double			EC_T_REAL;

typedef void*			EC_T_ADDRESS;

typedef void*			EC_T_HANDLE;

/* type of lock */
typedef enum
{
    eLockType_DEFAULT= 1,                           /*< Default mutex           */
    eLockType_SPIN,                                 /*< only jobs --> spin lock */
    eLockType_INTERFACE,                            /*< interface and jobs      */

    /* Borland C++ datatype alignment correction */
    eLockType_BCppDummy   = 0xFFFFFFFF
} EC_T_OS_LOCK_TYPE;                                       


/*/////////////////////////////////////////////////////////////////////////////////////////
//
// Standard Types
*/
#define	DEFTYPE_NULL              ((EC_T_WORD)0x0000)       
#define	DEFTYPE_BOOLEAN           ((EC_T_WORD)0x0001)       /* bit size: 1 */
#define	DEFTYPE_INTEGER8          ((EC_T_WORD)0x0002)       /* bit size: 8 */
#define	DEFTYPE_INTEGER16         ((EC_T_WORD)0x0003)       /* bit size: 16 */
#define	DEFTYPE_INTEGER32         ((EC_T_WORD)0x0004)       /* bit size: 32 */
#define	DEFTYPE_UNSIGNED8         ((EC_T_WORD)0x0005)       /* bit size: 8 */
#define	DEFTYPE_UNSIGNED16        ((EC_T_WORD)0x0006)       /* bit size: 16 */
#define	DEFTYPE_UNSIGNED32        ((EC_T_WORD)0x0007)       /* bit size: 32 */
#define	DEFTYPE_REAL32            ((EC_T_WORD)0x0008)       /* bit size: 32 */
#define	DEFTYPE_VISIBLESTRING     ((EC_T_WORD)0x0009)       /* bit size: 8*n */
#define	DEFTYPE_OCTETSTRING       ((EC_T_WORD)0x000A)       /* bit size: 8*(n+1) */
#define	DEFTYPE_UNICODESTRING     ((EC_T_WORD)0x000B)       /* bit size: 16*(n+1) */
#define	DEFTYPE_TIMEOFDAY         ((EC_T_WORD)0x000C)
#define	DEFTYPE_TIMEDIFFERENCE    ((EC_T_WORD)0x000D)
#define	DEFTYPE_DOMAIN            ((EC_T_WORD)0x000F)
#define	DEFTYPE_INTEGER24         ((EC_T_WORD)0x0010)       /* bit size: 24 */
#define	DEFTYPE_REAL64            ((EC_T_WORD)0x0011)       /* bit size: 64 */
#define	DEFTYPE_INTEGER40         ((EC_T_WORD)0x0012)       /* bit size: 40 */
#define	DEFTYPE_INTEGER48         ((EC_T_WORD)0x0013)       /* bit size: 48 */
#define	DEFTYPE_INTEGER56         ((EC_T_WORD)0x0014)       /* bit size: 56 */
#define	DEFTYPE_INTEGER64         ((EC_T_WORD)0x0015)       /* bit size: 64 */
#define	DEFTYPE_UNSIGNED24        ((EC_T_WORD)0x0016)       /* bit size: 24 */
#define	DEFTYPE_UNSIGNED40        ((EC_T_WORD)0x0018)       /* bit size: 40 */
#define	DEFTYPE_UNSIGNED48        ((EC_T_WORD)0x0019)       /* bit size: 48 */
#define	DEFTYPE_UNSIGNED56        ((EC_T_WORD)0x001A)       /* bit size: 56 */
#define	DEFTYPE_UNSIGNED64        ((EC_T_WORD)0x001B)       /* bit size: 64 */

#define	DEFTYPE_GUID              ((EC_T_WORD)0x001D)       /* bit size: 128 */
#define	DEFTYPE_BYTE              ((EC_T_WORD)0x001E)       /* bit size: 8 */
#define	DEFTYPE_WORD              ((EC_T_WORD)0x001F)       /* bit size: 16 */
#define	DEFTYPE_DWORD             ((EC_T_WORD)0x0020)       /* bit size: 32 */
#define	DEFTYPE_PDOMAPPING        ((EC_T_WORD)0x0021)

#define	DEFTYPE_IDENTITY          ((EC_T_WORD)0x0023)

#define	DEFTYPE_COMMAND           ((EC_T_WORD)0x0025)
#define	DEFTYPE_PDOCOMPAR         ((EC_T_WORD)0x0027)
#define	DEFTYPE_ENUM              ((EC_T_WORD)0x0028)
#define	DEFTYPE_SMPAR             ((EC_T_WORD)0x0029)
#define	DEFTYPE_RECORD            ((EC_T_WORD)0x002A)
#define	DEFTYPE_BACKUP_PARAMETER  ((EC_T_WORD)0x002B)
#define	DEFTYPE_MODULAR_DEVICE_PROFILE  ((EC_T_WORD)0x002C)
#define	DEFTYPE_BITARR8           ((EC_T_WORD)0x002D)       /* bit size: 8 */
#define	DEFTYPE_BITARR16          ((EC_T_WORD)0x002E)       /* bit size: 16 */
#define	DEFTYPE_BITARR32          ((EC_T_WORD)0x002F)       /* bit size: 32 */
#define	DEFTYPE_BIT1              ((EC_T_WORD)0x0030)       /* bit size: 1 */
#define	DEFTYPE_BIT2              ((EC_T_WORD)0x0031)       /* bit size: 1 */
#define	DEFTYPE_BIT3              ((EC_T_WORD)0x0032)       /* bit size: 1 */
#define	DEFTYPE_BIT4              ((EC_T_WORD)0x0033)       /* bit size: 1 */
#define	DEFTYPE_BIT5              ((EC_T_WORD)0x0034)       /* bit size: 1 */
#define	DEFTYPE_BIT6              ((EC_T_WORD)0x0035)       /* bit size: 1 */
#define	DEFTYPE_BIT7              ((EC_T_WORD)0x0036)       /* bit size: 1 */
#define	DEFTYPE_BIT8              ((EC_T_WORD)0x0037)       /* bit size: 1 */


/*-MACROS--------------------------------------------------------------------*/
#define EC_FALSE            0
#define EC_TRUE             1
#define EC_NULL             0

#define EC_NOWAIT           ((EC_T_DWORD)0x00000000)
#define EC_WAITINFINITE     ((EC_T_DWORD)0xFFFFFFFF)


/*-MASTER FEATURES-----------------------------------------------------------*/

/* object dictionary */
#define EC_MASTER_DEVICE_NAME       "EC-Master"         /* Index: 0x1008 */
#define EC_MASTER_HW_VERSION        "V xx.xx.xx.xx"     /* Index: 0x1009 */
#define EC_MASTER_SW_VERSION        "V xx.xx.xx.xx"     /* Index: 0x100A */
#define EC_MASTER_VENDOR_ID         0x00004154          /* Index: 0x1018 Subindex 1 */
#define EC_MASTER_PRODUCT_CODE      0x6d657461          /* Index: 0x1018 Subindex 2 */

/* bit mask for supported features (used for license check) */
#define EC_FEATURES_MASTER_MASK     0x00000001
#define EC_FEATURES_RED_MASK        0x00000002

/* Class B */
#ifdef  EXCLUDE_EC_MASTER_CLASS_B
#define EXCLUDE_EC_MASTER_CLASS_A           /* Class A needs Class B */
#define EXCLUDE_EOE_SUPPORT                 /* Ethernet over EtherCAT */
#define EXCLUDE_SOE_SUPPORT                 /* ServoDrive over EtherCAT */
#define EXCLUDE_FRAME_LOGGING               /* Callback interface to log frames */
#define EXCLUDE_FRAME_LOSS_SIMULATION
#define EXCLUDE_MEMORY_PROVIDER
#define EXCLUDE_SLAVE_STATISTICS            /* Cyclic reading of slave error registers */
#define EXCLUDE_VARREAD                     /* Read PD variable tags from XML */
#endif

/* Class A */
#ifdef  EXCLUDE_EC_MASTER_CLASS_A
#define EXCLUDE_AOE_SUPPORT                 /* ADS over EtherCAT */ 
#define EXCLUDE_DC_SUPPORT                  /* Distributed Clocks */
#define EXCLUDE_FOE_SUPPORT                 /* File access over EtherCAT */
#define EXCLUDE_MASTER_OBD                  /* Master Object Dictionary */
#define EXCLUDE_MULTIPLE_CYC_ENTRIES        /* Multiple cyclic entries */
#define EXCLUDE_RED_DEVICE                  /* Cable Redundancy */
#define EXCLUDE_VOE_SUPPORT                 /* Vendor specific access over EtherCAT */ 
#endif

/* Base functionality */
#ifdef  EXCLUDE_AOE_SUPPORT
#define EXCLUDE_ADS_ADAPTER                 /* ADS adapter needs AoE */
#else
#define INCLUDE_AOE_SUPPORT                 /* ADS over EtherCAT */ 
#endif
#ifndef EXCLUDE_DC_SUPPORT
#define INCLUDE_DC_SUPPORT                  /* Distributed Clocks */
#endif
#ifndef EXCLUDE_DC_ADD_ACYC_DISTRIBUTION
#define INCLUDE_DC_ADD_ACYC_DISTRIBUTION    /* Additional system time distribution in acyc frame */
#endif
#ifndef EXCLUDE_EOE_SUPPORT
#define INCLUDE_EOE_SUPPORT                 /* Ethernet over EtherCAT */
#endif
#ifndef EXCLUDE_FOE_SUPPORT
#define INCLUDE_FOE_SUPPORT                 /* File access over EtherCAT */
#endif
#ifndef EXCLUDE_FRAME_LOGGING
#define INCLUDE_FRAME_LOGGING               /* Callback interface to log frames */
#endif
#ifndef EXCLUDE_FRAME_LOSS_SIMULATION
#define FRAME_LOSS_SIMULATION               /* Frame loss simulation */
#endif
#ifndef EXCLUDE_GEN_OP_ENI
#define INCLUDE_GEN_OP_ENI                  /* Generation of ENI file to reach OP state */
#endif
#ifdef  EXCLUDE_LOG_MESSAGES
#define EXCLUDE_TEXT
#else
#define INCLUDE_LOG_MESSAGES                /* Log messages */
#endif
#ifndef EXCLUDE_TEXT
#define INCLUDE_TEXT                        /* Text returned by e.g. EcErrorText() */
#endif
#ifndef EXCLUDE_MASTER_OBD
#define INCLUDE_MASTER_OBD                  /* Master Object Dictionary */
#endif
#ifndef EXCLUDE_MEMORY_PROVIDER
#define INCLUDE_MEMORY_PROVIDER             /* Memory provider */
#endif
#ifndef EXCLUDE_MULTIPLE_CYC_ENTRIES
#define INCLUDE_MULTIPLE_CYC_ENTRIES        /* Multiple cyclic entries */
#endif
#ifndef EXCLUDE_PORT_OPERATION
#define INCLUDE_PORT_OPERATION              /* Port operation (close/open ports blocknode) */
#endif
#ifndef EXCLUDE_RAWMBX_SUPPORT
#define INCLUDE_RAWMBX_SUPPORT              /* Raw mailbox transfer (e.g. used by mailbox gateway) */
#endif
#ifndef EXCLUDE_SOE_SUPPORT
#define INCLUDE_SOE_SUPPORT                 /* ServoDrive over EtherCAT */
#endif
#ifndef EXCLUDE_SLAVE_STATISTICS
#define INCLUDE_SLAVE_STATISTICS            /* Cyclic reading of slave error registers */
#endif
#ifndef EXCLUDE_TRACE_DATA
#define INCLUDE_TRACE_DATA                  /* Trace data */
#ifndef EXCLUDE_TRACE_DATA_VARINFO
#define INCLUDE_TRACE_DATA_VARINFO          /* Trace data variable info */
#endif
#endif
#ifndef EXCLUDE_VARREAD
#define INCLUDE_VARREAD                     /* Read PD variable tags from XML */
#endif
#ifndef EXCLUDE_VOE_SUPPORT
#define INCLUDE_VOE_SUPPORT                 /* Vendor specific access over EtherCAT */ 
#endif
#ifndef EXCLUDE_WKCSTATE
#define INCLUDE_WKCSTATE                    /* WkcState bits in diagnosis image */ 
#endif
#ifndef EXCLUDE_XPAT
#define INCLUDE_XPAT                        /* ENI file parser */
#endif

#if 0
#define VLAN_FRAME_SUPPORT                  /* VLAN frame support. default: not included. */
#define INCLUDE_COE_PDO_SUPPORT             /* currently not supported */
#define INCLUDE_EC_INTERNAL_TSC_MEASUREMENT
#define INCLUDE_RAS_SPOCSUPPORT
#endif

#ifndef EXCLUDE_FORCE_PROCESSDATA
#define INCLUDE_FORCE_PROCESSDATA           /* Force/Release process data */ 
#endif

#ifndef EXCLUDE_LINE_CROSSED_DETECTION
#define INCLUDE_LINE_CROSSED_DETECTION
#endif

#ifndef EXCLUDE_OEM
#define INCLUDE_OEM
#endif

#ifndef EXCLUDE_JUNCTION_REDUNDANCY
#define INCLUDE_JUNCTION_REDUNDANCY         /* Junction Redundancy */
#endif

/* feature packs */
#ifndef EXCLUDE_RED_DEVICE
#define INCLUDE_RED_DEVICE                  /* Cable Redundancy */
#endif
#ifndef EXCLUDE_ADS_ADAPTER
#define INCLUDE_ADS_ADAPTER
#endif
#ifndef EXCLUDE_EOE_ENDPOINT
#define INCLUDE_EOE_ENDPOINT                /* Ethernet over EtherCAT end point */
#endif
#ifndef EXCLUDE_HOTCONNECT
#define INCLUDE_HOTCONNECT
#endif
#ifndef EXCLUDE_PASS_THROUGH_SERVER
#define INCLUDE_PASS_THROUGH_SERVER
#endif
#ifndef EXCLUDE_DC_SUPPORT
#ifndef EXCLUDE_DCX                         /* DCX external synchronization */
#define INCLUDE_DCX
#endif
#endif /* !EXCLUDE_DC_SUPPORT */
#ifndef EXCLUDE_RESCUE_SCAN                 /* Rescue Scan */
#define INCLUDE_RESCUE_SCAN
#endif
#ifndef EXCLUDE_CONFIG_EXTEND  
#define INCLUDE_CONFIG_EXTEND               /* Extend configuration to be able to set unexpected bus slaves to PREOP */
#endif
#ifndef EXCLUDE_MAILBOX_STATISTICS
#define INCLUDE_MAILBOX_STATISTICS          /* Collect statistics of mailbox transfers */
#endif

#endif /* INC_ECTYPE */

/*-END OF SOURCE FILE--------------------------------------------------------*/
