/*-----------------------------------------------------------------------------
 * EcLink.h
 * Copyright                acontis technologies GmbH, Weingarten, Germany
 * Response                 Stefan Zintgraf
 * Description              EtherCAT Master link layer interface
 *---------------------------------------------------------------------------*/

#ifndef INC_ECLINK
#define INC_ECLINK

/*-INCLUDES------------------------------------------------------------------*/
#ifndef INC_ECTYPE
#include <EcType.h>
#endif
#ifndef INC_ECERROR
#include <EcError.h>
#endif
#ifndef INC_ECOS
#include <EcOs.h>
#endif

#ifndef ATEMLL_API
#define ATEMLL_API
#endif

/*-COMPILER SETTINGS---------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

/*-DEFINES-------------------------------------------------------------------*/
#define LINK_LAYER_DRV_DESC_PATTERN         (EC_T_DWORD)0x11aaddaa
#define LINK_LAYER_DRV_DESC_VERSION         (EC_T_DWORD)0x00020200      /* version 2.2 */

#define EC_LINK_PARMS_SIGNATURE_PATTERN     (EC_T_DWORD)0xBA100000      /* Mask 0xfff00000 */
#define EC_LINK_PARMS_SIGNATURE_VERSION     (EC_T_DWORD)0x00020000      /* Version 2, mask 0x000f0000 */
#define EC_LINK_PARMS_SIGNATURE (EC_T_DWORD)(EC_LINK_PARMS_SIGNATURE_PATTERN|EC_LINK_PARMS_SIGNATURE_VERSION)

#define MAX_DRIVER_IDENT_LEN                40              /* maximum length for a driver ident string */

#define EC_LINKUNIT_PCILOCATION             0x01000000      /* if the MSB of dwUnit is set to this value, the 3LSB mean Bus-Dev-Func */

#define EC_LINKNOTIFY_GENERIC               0x00000000
#define EC_LINKNOTIFY_LINKSTATECHGD         (EC_LINKNOTIFY_GENERIC | 0x1)   /* not used within EC-Master */
#define EC_LINKNOTIFY_TXMEMORYCHGD          (EC_LINKNOTIFY_GENERIC | 0x2)   /* not used within EC-Master */

#define EC_LINKIOCTL_GENERIC                0x00000000
#define EC_LINKIOCTL_INTERRUPTENABLE        (EC_LINKIOCTL_GENERIC  | 0x1)   /* not used within EC-Master */
#define EC_LINKIOCTL_PROMISCOUSMODE         (EC_LINKIOCTL_GENERIC  | 0x2)   /* not used within EC-Master */
#define EC_LINKIOCTL_SETMULTICASTADDR       (EC_LINKIOCTL_GENERIC  | 0x3)   /* not used within EC-Master */
#define EC_LINKIOCTL_CLRMULTICASTADDR       (EC_LINKIOCTL_GENERIC  | 0x4)   /* not used within EC-Master */
#define EC_LINKIOCTL_SETLINKMODE            (EC_LINKIOCTL_GENERIC  | 0x5)   /* not used within EC-Master */
#define EC_LINKIOCTL_GETLINKMODE            (EC_LINKIOCTL_GENERIC  | 0x6)   /* not used within EC-Master */
#define EC_LINKIOCTL_RESTART                (EC_LINKIOCTL_GENERIC  | 0x7)   /* not used within EC-Master */
#define EC_LINKIOCTL_SET_LINKMODE           (EC_LINKIOCTL_GENERIC  | 0x8)   /* not used within EC-Master: set mode to polling or interrupt */
#define EC_LINKIOCTL_SET_ALLOC_SENDFRAME    (EC_LINKIOCTL_GENERIC  | 0x9)   /* not used within EC-Master: for debug purposes: enable/disable EcLinkAllocSendFrame() support */
#define EC_LINKIOCTL_FORCELINKMODE          (EC_LINKIOCTL_GENERIC  | 0xA)   /* not used within EC-Master: Bit 0=Autoneg.;1=FullDuplex;16-31=LinkSpeed 1=10,2=100,3=1000MBit */
#define EC_LINKIOCTL_GETINFOLIST            (EC_LINKIOCTL_GENERIC  | 0xB)   /* not used within EC-Master: query EC_T_LINK_INFOLIST */

#define EC_LINKENABLED_OFF 0
#define EC_LINKENABLED_ON 1
#define EC_LINKENABLED_ONLY_SEND 2
#define EC_LINKENABLED_ONLY_RECEIVE 3
#define EC_LINKIOCTL_SET_LINKENABLED        (EC_LINKIOCTL_GENERIC  | 0xC)   /* enables or disables the link */

/* The following IO-Controls are mandatory for LinkLayer in interrupt mode */
#define EC_LINKIOCTL_REGISTER_FRAME_CLBK    (EC_LINKIOCTL_GENERIC  | 0x10)  /* registers the receive frame callback */
#define EC_LINKIOCTL_UNREGISTER_FRAME_CLBK  (EC_LINKIOCTL_GENERIC  | 0x11)  /* unregisters the receive frame callback */
#define EC_LINKIOCTL_INTSTATUS_READ         (EC_LINKIOCTL_GENERIC  | 0x12)  /* not used within EC-Master: read card's interrupt status register */
#define EC_LINKIOCTL_INTSTATUS_WRITE        (EC_LINKIOCTL_GENERIC  | 0x13)  /* not used within EC-Master: write card's interrupt status register */

#define EC_LINKIOCTL_FEATURECONTROL         (EC_LINKIOCTL_GENERIC  | 0x14)  /* not used within EC-Master: Bit 0=JumboFrame, Bit 1=ResetOndisconnect */

#define EC_LINKIOCTL_UPDATE_LINKSTATUS      (EC_LINKIOCTL_GENERIC  | 0x15)  /* Update link status (eTSEC only) */
#define EC_LINKIOCTL_GET_ETHERNET_ADDRESS   (EC_LINKIOCTL_GENERIC  | 0x23)  /* calls EcLinkGetEthernetAddress */
#define EC_LINKIOCTL_IS_REPEAT_CNT_SUPPORTED (EC_LINKIOCTL_GENERIC | 0x24)  /* return EC_E_NOERROR if supported, EC_E_NOTSUPPORTED otherwise */

/* LinkLayer timer support */
#define  EC_LINKIOCTL_SETTIME               (EC_LINKIOCTL_GENERIC  | 0x30)
#define  EC_LINKIOCTL_GETTIME               (EC_LINKIOCTL_GENERIC  | 0x31)
#define  EC_LINKIOCTL_WAITFORTIME           (EC_LINKIOCTL_GENERIC  | 0x32)

/* Frame type differentiation */
#define  EC_LINKIOCTL_PROCESSCYCLICFRAMES   (EC_LINKIOCTL_GENERIC  | 0x33)
#define  EC_LINKIOCTL_PROCESSACYCLICFRAMES  (EC_LINKIOCTL_GENERIC  | 0x34)
#define  EC_LINKIOCTL_SENDCYCLICFRAMES      (EC_LINKIOCTL_GENERIC  | 0x35) /* prepares link layer for sending cyclic frames */
#define  EC_LINKIOCTL_SENDACYCLICFRAMES     (EC_LINKIOCTL_GENERIC  | 0x36) /* prepares link layer for sending acyclic frames */
#define  EC_LINKIOCTL_IS_FRAMETYPE_REQUIRED (EC_LINKIOCTL_GENERIC  | 0x37) /* EC_TRUE: Master stack calls EC_LINKIOCTL_SENDCYCLICFRAMES or EC_LINKIOCTL_SENDACYCLICFRAMES once before LinkSendFrame() */

/* Frame flushing (frames are queue and sent after flushing) */
#define  EC_LINKIOCTL_FLUSHFRAMES             (EC_LINKIOCTL_GENERIC | 0x38) /* flush cyclic / acyclic frames queued at link layer (see also EC_LINKIOCTL_SENDCYCLICFRAMES, EC_LINKIOCTL_SENDACYCLICFRAMES) */
#define  EC_LINKIOCTL_IS_FLUSHFRAMES_REQUIRED (EC_LINKIOCTL_GENERIC | 0x39) /* EC_TRUE: Master stack calls EC_LINKIOCTL_FLUSHFRAMES after all cyclic frames were sent and after all acyclic frames were sent */

#define  EC_LINKIOCTL_SET_LINKSTATUS         (EC_LINKIOCTL_GENERIC | 0x3A) /* changes link status if this IO is supported. EC_T_LINKSTATUS used as parameter*/
#define  EC_LINKIOCTL_SET_FORCEDISCONNECTION (EC_LINKIOCTL_GENERIC | 0x3B)

#define  EC_LINKIOCTL_GET_SPEED              (EC_LINKIOCTL_GENERIC | 0x3C)

/*-TYPEDEFS------------------------------------------------------------------*/
typedef enum _EC_T_LINKMODE
{        
    EcLinkMode_UNDEFINED = 0,
    EcLinkMode_INTERRUPT = 1,
    EcLinkMode_POLLING   = 2,
    EcLinkMode_RANDOM    = 3,      /* for debug purposes only! TODO: remove */

    /* Borland C++ datatype alignment correction */
    EcLinkMode_BCppDummy   = 0xFFFFFFFF
} EC_T_LINKMODE;

#include EC_PACKED_INCLUDESTART(4)
typedef struct _EC_T_LINKLAYER_MODE_DESC
{
    EC_T_LINKMODE	eLinkMode;                  /*< out Linkmode of LL */
    EC_T_LINKMODE	eLinkModeRed;               /*< out Linkmode of RedLL */
} EC_PACKED(4) EC_T_LINKLAYER_MODE_DESC;
#include EC_PACKED_INCLUDESTOP

typedef enum _EC_T_LINKSTATUS
{
    eLinkStatus_UNDEFINED = 0,
    eLinkStatus_OK,
    eLinkStatus_DISCONNECTED,
    eLinkStatus_HALFDUPLEX,

    /* Borland C++ datatype alignment correction */
    eLinkStatus_BCppDummy   = 0xFFFFFFFF
} EC_T_LINKSTATUS;

typedef enum _EC_T_ALLOCSENDFRAME_MODE
{
    EcLinkAllocSendFrameMode_UNDEFINED = 0,
    EcLinkAllocSendFrameMode_ENABLED,
    EcLinkAllocSendFrameMode_DISABLED,

    /* Borland C++ datatype alignment correction */
    EcLinkAllocSendFrameMode_BCppDummy   = 0xFFFFFFFF
} EC_T_ALLOCSENDFRAMEMODE;

/* PHY Interface */
typedef enum _EC_T_PHYINTERFACE
{
    ePHY_FIXED_LINK = 1 << 0,  /* No PHY access at all */
    ePHY_MII = 1 << 1,  /* 10 / 100 MBit */
    ePHY_RMII = 1 << 2,  /* Reduced MII, 10 / 100 MBit */
    ePHY_GMII = 1 << 3,  /* Gigabit MII, 10, 100, 1000 MBit */  
    ePHY_SGMII = 1 << 4,  /* Serial (SERDES) Gigabit MII, 10, 100, 1000 MBit */
    ePHY_RGMII = 1 << 5  /* Reduced Gigabit MII, 10, 100, 1000 MBit */
} EC_T_PHYINTERFACE;

struct _EC_T_LINK_NOTIFYPARMS;
struct _EC_T_LINK_FRAMEDESC;

typedef EC_T_DWORD (*EC_T_LINK_GETTIMESTAMP)(EC_T_PVOID pCallerData, EC_T_DWORD* pdwHostTimeLo);
typedef EC_T_DWORD (*EC_T_RECEIVEFRAMECALLBACK)(EC_T_VOID* pvContext, struct _EC_T_LINK_FRAMEDESC* pLinkFrameDesc, EC_T_BOOL* pbFrameProcessed);
typedef EC_T_DWORD (*EC_T_LINK_NOTIFY)(EC_T_DWORD dwCode, struct _EC_T_LINK_NOTIFYPARMS* pParms);

/* EtherCAT notify parameters */
#include EC_PACKED_INCLUDESTART(4)
typedef struct _EC_T_LINK_NOTIFYPARMS
{
    EC_T_VOID*      pvContext;			/*< in  context */
    EC_T_BYTE*      pbyInBuf;            /*< in  input data buffer */
    EC_T_DWORD      dwInBufSize;        /*< in  size of input data buffer in byte */
    EC_T_BYTE*      pbyOutBuf;          /*< out output data buffer */
    EC_T_DWORD      dwOutBufSize;       /*< in  size of output data buffer in byte */
    EC_T_DWORD*     pdwNumOutData;      /*< out number of output data bytes stored in output data buffer */
} EC_PACKED(8) EC_T_LINK_NOTIFYPARMS;
#include EC_PACKED_INCLUDESTOP

/* EtherCAT link frame descriptor */
#include EC_PACKED_INCLUDESTART(8)
typedef struct _EC_T_LINK_FRAMEDESC
{
    EC_T_VOID*          pvContext;          /*< link layer context */
    EC_T_BYTE*          pbyFrame;           /*< frame data buffer */
    EC_T_DWORD          dwSize;             /*< size of the frame buffer */
    EC_T_BOOL           bBuffersFollow;     /*< if EC_TRUE try to queue next frame in link layer,
                                                if EC_FALSE fill up DMA descriptors to force immediate send */

    EC_T_DWORD*         pdwTimeStampLo;     /*< data store to store timestamp result to */
    EC_T_DWORD*         pdwTimeStampPostLo; /*< data store to store timestamp result to */
    EC_T_PVOID          pvTimeStampCtxt;    /*< context for pfnTimeStamp */
    EC_T_LINK_GETTIMESTAMP pfnTimeStamp;    /*< function if not EC_NULL called to do timestamping */
    EC_T_DWORD*         pdwLastTSResult;    /*< result code store of last time stamp call */

    EC_T_WORD           wTimestampOffset;   /*< Place in the frame where the timestamp has to be placed */
    EC_T_WORD           wTimestampSize;     /*< Size in byte of the timestamp */
    EC_T_UINT64         qwTimestamp;        /*< Send or receive time point */

    EC_T_DWORD          dwRepeatCnt;        /*< Repeat count 0 or 1 send once, otherwise repeat the frame */
} EC_PACKED(8) EC_T_LINK_FRAMEDESC;
#include EC_PACKED_INCLUDESTOP

/* Structure to register the Link Layer receive callback */
#include EC_PACKED_INCLUDESTART(8)
typedef struct _EC_T_LINK_FRM_RECV_CLBK
{
    EC_T_RECEIVEFRAMECALLBACK   pfFrameReceiveCallback;
    EC_T_VOID*                  pvDevice;
} EC_PACKED(8) EC_T_LINK_FRM_RECV_CLBK;
#include EC_PACKED_INCLUDESTOP

#include EC_PACKED_INCLUDESTART(8)
typedef struct _EC_T_LINK_IOCTLPARMS
{
    EC_T_PBYTE      pbyInBuf;           /*< in  input data buffer */
    EC_T_DWORD      dwInBufSize;        /*< in  size of input data buffer in byte */
    EC_T_PBYTE      pbyOutBuf;          /*< out output data buffer */
    EC_T_DWORD      dwOutBufSize;       /*< in  size of output data buffer in byte */
    EC_T_DWORD*     pdwNumOutData;      /*< out number of output data bytes stored in output data buffer */
} EC_PACKED(8) EC_T_LINK_IOCTLPARMS;
#include EC_PACKED_INCLUDESTOP

#include EC_PACKED_INCLUDESTART(8)
typedef struct _EC_T_LINK_PARMS
{
    EC_T_DWORD      dwSignature;                            /* see EC_LINK_PARMS_SIGNATURE_... */
    EC_T_DWORD      dwSize;
    EC_T_CHAR       szDriverIdent[MAX_DRIVER_IDENT_LEN];    /* driver identification string (zero terminated) */
    EC_T_DWORD      dwInstance;
    EC_T_LINKMODE   eLinkMode;                              /* operation mode: polling or interrupt */
    EC_T_DWORD      dwIstPriority;
} EC_PACKED(8) EC_T_LINK_PARMS;
#include EC_PACKED_INCLUDESTOP

typedef EC_T_DWORD (*PF_EcLinkOpen)(EC_T_VOID* pvLinkParms, EC_T_RECEIVEFRAMECALLBACK pfReceiveFrameCallback, EC_T_LINK_NOTIFY pfLinkNotifyCallback, EC_T_VOID* pvContext, EC_T_VOID** ppvInstance);
typedef EC_T_DWORD (*PF_EcLinkClose)(EC_T_VOID* pvInstance);
typedef EC_T_DWORD (*PF_EcLinkSendFrame)(EC_T_VOID* pvInstance, EC_T_LINK_FRAMEDESC* pLinkFrameDesc);
typedef EC_T_DWORD (*PF_EcLinkSendAndFreeFrame)(EC_T_VOID* pvInstance, EC_T_LINK_FRAMEDESC* pLinkFrameDesc);
typedef EC_T_DWORD (*PF_EcLinkRecvFrame)(EC_T_VOID* pvInstance, EC_T_LINK_FRAMEDESC* pLinkFrameDesc);
typedef EC_T_DWORD (*PF_EcLinkAllocSendFrame)(EC_T_VOID* pvInstance, EC_T_LINK_FRAMEDESC* pLinkFrameDesc, EC_T_DWORD dwSize);
typedef EC_T_VOID  (*PF_EcLinkFreeSendFrame )(EC_T_VOID* pvInstance, EC_T_LINK_FRAMEDESC* pLinkFrameDesc);
typedef EC_T_VOID  (*PF_EcLinkFreeRecvFrame )(EC_T_VOID* pvInstance, EC_T_LINK_FRAMEDESC* pLinkFrameDesc);
typedef EC_T_DWORD      (*PF_EcLinkGetEthernetAddress)(EC_T_VOID* pvInstance, EC_T_BYTE* pbyEthernetAddress);
typedef EC_T_LINKSTATUS (*PF_EcLinkGetStatus)(EC_T_VOID* pvInstance);
typedef EC_T_DWORD      (*PF_EcLinkGetSpeed)(EC_T_VOID* pvInstance);
typedef EC_T_LINKMODE   (*PF_EcLinkGetMode)(EC_T_VOID* pvInstance);
typedef EC_T_DWORD      (*PF_EcLinkIoctl)(EC_T_VOID* pvInstance, EC_T_DWORD dwCode, EC_T_LINK_IOCTLPARMS* pParms );

#include EC_PACKED_INCLUDESTART(8)
typedef struct _EC_T_LINK_DRV_DESC
{
    EC_T_DWORD                  dwValidationPattern;
    EC_T_DWORD                  dwInterfaceVersion;
    EC_PF_OSDBGMSGHK            pfOsDbgMsgHook;
    PF_EcLinkOpen               pfEcLinkOpen;
    PF_EcLinkClose              pfEcLinkClose;

    /* Standard send functions */
    PF_EcLinkSendFrame          pfEcLinkSendFrame;
    PF_EcLinkSendAndFreeFrame   pfEcLinkSendAndFreeFrame;

    /* Timestamping time optimized send functions */
    PF_EcLinkSendFrame          pfEcLinkRes1;
    PF_EcLinkSendAndFreeFrame   pfEcLinkRes2;

    PF_EcLinkRecvFrame          pfEcLinkRecvFrame;
    PF_EcLinkAllocSendFrame     pfEcLinkAllocSendFrame;
    PF_EcLinkFreeSendFrame      pfEcLinkFreeSendFrame ;
    PF_EcLinkFreeRecvFrame      pfEcLinkFreeRecvFrame ;
    PF_EcLinkGetEthernetAddress pfEcLinkGetEthernetAddress;
    PF_EcLinkGetStatus          pfEcLinkGetStatus;
    PF_EcLinkGetSpeed           pfEcLinkGetSpeed;
    PF_EcLinkGetMode            pfEcLinkGetMode;
    PF_EcLinkIoctl              pfEcLinkIoctl;

    EC_T_VOID*                  pvLinkInstance;
} EC_PACKED(8) EC_T_LINK_DRV_DESC;
#include EC_PACKED_INCLUDESTOP

/* PF_emllRegister is obsolete, use EC_PF_LLREGISTER instead. */
typedef EC_T_DWORD (*PF_DOINT_HDL)(EC_T_PVOID pvLinkParms);


#define EC_LINK_INFO_DESCRIPTION_SIZE_MAX   63
typedef enum _EC_T_LINK_INFO_DATATYPE
{
    eLinkInfoDataType_bool = 0,
    eLinkInfoDataType_byte,
    eLinkInfoDataType_word,
    eLinkInfoDataType_dword,
    eLinkInfoDataType_qword,

    /* Borland C++ datatype alignment correction */
    eLinkInfoDataType_BCppDummy   = 0xFFFFFFFF
} EC_T_LINK_INFO_DATATYPE;

#ifndef  __TMS470__
#include EC_PACKED_INCLUDESTART(8)
typedef struct _EC_T_LINK_INFO
{
    EC_T_CHAR               szDescription[EC_LINK_INFO_DESCRIPTION_SIZE_MAX];  /*< Description text of the value */
    EC_T_LINK_INFO_DATATYPE eDataType;          /*< Datatype */
    union _EC_T_LINK_INFO_DATA
    {
        EC_T_BOOL           bData;              /*< Data as boolean */
        EC_T_BYTE           byData;             /*< Data as byte */
        EC_T_WORD           wData;              /*< Data as word */
        EC_T_DWORD          dwData;             /*< Data as dword */
        EC_T_UINT64         qwData;             /*< Data as qword */
    } EC_PACKED(8) u;
} EC_PACKED(8) EC_T_LINK_INFO;
#include EC_PACKED_INCLUDESTOP

#include EC_PACKED_INCLUDESTART(8)
typedef struct _EC_T_LINK_INFOLIST
{
    EC_T_DWORD          dwInfoCntMax;       /*< Total number of info elements available */
    EC_T_DWORD          dwInfoCntUsed;      /*< Number of info elements used */
    EC_T_LINK_INFO      aInfo[1];
} EC_PACKED(8) EC_T_LINK_INFOLIST;
#include EC_PACKED_INCLUDESTOP
#endif /* __TMS470__ */

/*****************************************************************************/
/** \brief  Variables to identify the EOE link layer driver/instance which shall
*           be opened with the ``EoELinkOpen()`` call.
*
*           Attention: Parameters not used for identification of the link layer
*                      (or master stack  instance) must be cleared to 0.
*/
#include EC_PACKED_INCLUDESTART(8)
typedef struct _EC_T_LINK_OPENPARMS_EOE
{
    /*! \var    dwEmInstanceID
        \brief  Instance ID (or instance number), identical to the instance ID
                of the EtherCAT master which is the master of the network we want
                to open.
	*/
    EC_T_DWORD dwEmInstanceID;

    /*! \var    pvUplinkInstance
        \brief  Pointer to the ``CEcEoEUplink`` instance/object (if available).
	*/
    EC_T_PVOID pvUplinkInstance;

    /*! \var    dwEthAddress
        \brief  Ethernet address of the driver/adapter we want to open.
	*/
    EC_T_BYTE abyEthAddress[6];

    /*! \var    abyIpAdress
        \brief  IP address of the driver/adapter we want to open.
	*/
    EC_T_BYTE abyIpAdress[4];

    /*! \var    szEoEDrvIdent
        \brief  Name of the driver/adapter we want to open (e.g. "eth0").
	*/
    EC_T_CHAR szEoEDrvIdent[MAX_DRIVER_IDENT_LEN];
} EC_PACKED(8) EC_T_LINK_OPENPARMS_EOE;
#include EC_PACKED_INCLUDESTOP

/*****************************************************************************/
/* Hardware specific link layer parameters                                   */
/*****************************************************************************/

/* WinPcap Windows */
/* =============== */
#define EC_LINK_PARMS_SIGNATURE_WINPCAP_PATTERN (EC_T_DWORD)0x0000CA00
#define EC_LINK_PARMS_SIGNATURE_WINPCAP_VERSION (EC_T_DWORD)0x00000001
#define EC_LINK_PARMS_SIGNATURE_WINPCAP (EC_T_DWORD)(EC_LINK_PARMS_SIGNATURE|EC_LINK_PARMS_SIGNATURE_WINPCAP_PATTERN|EC_LINK_PARMS_SIGNATURE_WINPCAP_VERSION)
#define EC_LINK_PARMS_IDENT_WINPCAP "Pcap"

#define MAX_LEN_WINPCAP_ADAPTER_ID  ((EC_T_DWORD)39)
#include EC_PACKED_INCLUDESTART(8)
typedef struct _EC_T_LINK_PARMS_WINPCAP
{
    /** 
     * \brief Link Layer abstraction. 
     * set linkParms.dwSignature = EC_LINK_PARMS_SIGNATURE_WINPCAP
     *
     * Must be first, see casts in usage.
     */
    EC_T_LINK_PARMS linkParms;

    EC_T_BYTE                   abyIpAddress[4];
    EC_T_CHAR                   szAdapterId[MAX_LEN_WINPCAP_ADAPTER_ID]; /* {XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX} */
} EC_PACKED(8) EC_T_LINK_PARMS_WINPCAP;
#include EC_PACKED_INCLUDESTOP

/* SNARF VxWorks */
/* ============= */
#define EC_LINK_PARMS_SIGNATURE_SNARF_PATTERN (EC_T_DWORD)0x0000CA10
#define EC_LINK_PARMS_SIGNATURE_SNARF_VERSION (EC_T_DWORD)0x00000001
#define EC_LINK_PARMS_SIGNATURE_SNARF (EC_T_DWORD)(EC_LINK_PARMS_SIGNATURE|EC_LINK_PARMS_SIGNATURE_SNARF_PATTERN|EC_LINK_PARMS_SIGNATURE_SNARF_VERSION)
#define EC_LINK_PARMS_IDENT_SNARF "Snarf"

#define MAX_LEN_SNARF_ADAPTER_NAME 64

#include EC_PACKED_INCLUDESTART(8)
typedef struct _EC_T_LINK_PARMS_SNARF
{
    /** 
     * \brief Link Layer abstraction. 
     * set linkParms.dwSignature = EC_LINK_PARMS_SIGNATURE_SNARF
     *
     * Must be first, see casts in usage.
     */
    EC_T_LINK_PARMS linkParms;

    EC_T_CHAR szAdapterName[MAX_LEN_SNARF_ADAPTER_NAME];

} EC_PACKED(8) EC_T_LINK_PARMS_SNARF;
#include EC_PACKED_INCLUDESTOP

/* Intel Pro 100 family */
/* ==================== */
#define EC_LINK_PARMS_SIGNATURE_I8255X_PATTERN (EC_T_DWORD)0x0000CA20
#define EC_LINK_PARMS_SIGNATURE_I8255X_VERSION (EC_T_DWORD)0x00000001
#define EC_LINK_PARMS_SIGNATURE_I8255X (EC_T_DWORD)(EC_LINK_PARMS_SIGNATURE|EC_LINK_PARMS_SIGNATURE_I8255X_PATTERN|EC_LINK_PARMS_SIGNATURE_I8255X_VERSION)
#define EC_LINK_PARMS_IDENT_I8255X "I8255x"

#include EC_PACKED_INCLUDESTART(8)
typedef struct _EC_T_LINK_PARMS_I8255X
{
    /** 
     * \brief Link Layer abstraction. 
     * set linkParms.dwSignature = EC_LINK_PARMS_SIGNATURE_I8255X
     *
     * Must be first, see casts in usage.
     */
    EC_T_LINK_PARMS linkParms;

} EC_PACKED(8) EC_T_LINK_PARMS_I8255X;
#include EC_PACKED_INCLUDESTOP

/* Intel Pro 1000 family */
/* ==================== */
#define EC_LINK_PARMS_SIGNATURE_I8254X_PATTERN (EC_T_DWORD)0x0000CA30
#define EC_LINK_PARMS_SIGNATURE_I8254X_VERSION (EC_T_DWORD)0x00000001
#define EC_LINK_PARMS_SIGNATURE_I8254X (EC_T_DWORD)(EC_LINK_PARMS_SIGNATURE|EC_LINK_PARMS_SIGNATURE_I8254X_PATTERN|EC_LINK_PARMS_SIGNATURE_I8254X_VERSION)
#define EC_LINK_PARMS_IDENT_I8254X "I8254x"

#include EC_PACKED_INCLUDESTART(8)
typedef struct _EC_T_LINK_PARMS_I8254X
{
    /**
     * \brief Link Layer abstraction.
     * set linkParms.dwSignature = EC_LINK_PARMS_SIGNATURE_I8254X
     *
     * Must be first, see casts in usage.
     */
    EC_T_LINK_PARMS linkParms;

    EC_T_WORD                   wRxBufferCnt;           /* RX buffer count, 0: default to 96 */
    EC_T_WORD                   wRxBufferSize;          /* RX buffer size (single Ethernet frame).
														   \sa EC_T_LINK_I8254X_BUFFERSIZE.
														   0: buffer optimized for standard Ethernet frame. */
    EC_T_WORD                   wTxBufferCnt;           /* TX buffer count, 0: default to 96 */
    EC_T_WORD                   wTxBufferSize;          /* TX buffer size (single Ethernet frame).
														   \sa EC_T_LINK_I8254X_BUFFERSIZE.
														   0: buffer optimized for standard Ethernet frame. */
    EC_T_BOOL                   bDisableLocks;          /* Locks in LL Disabled */
} EC_PACKED(8) EC_T_LINK_PARMS_I8254X;
#include EC_PACKED_INCLUDESTOP

/* Realtek RTL8139      */
/* ==================== */
#define EC_LINK_PARMS_SIGNATURE_RTL8139_PATTERN (EC_T_DWORD)0x0000CA40
#define EC_LINK_PARMS_SIGNATURE_RTL8139_VERSION (EC_T_DWORD)0x00000001
#define EC_LINK_PARMS_SIGNATURE_RTL8139 (EC_T_DWORD)(EC_LINK_PARMS_SIGNATURE|EC_LINK_PARMS_SIGNATURE_RTL8139_PATTERN|EC_LINK_PARMS_SIGNATURE_RTL8139_VERSION)
#define EC_LINK_PARMS_IDENT_RTL8139 "RTL8139"

#include EC_PACKED_INCLUDESTART(8)
typedef struct _EC_T_LINK_PARMS_RTL8139
{
/** 
     * \brief Link Layer abstraction. 
     * set linkParms.dwSignature = EC_LINK_PARMS_SIGNATURE_RTL8139
     *
     * Must be first, see casts in usage.
     */
    EC_T_LINK_PARMS linkParms;
    EC_T_DWORD                  dwRxBuffers;            /* [in]     amount of Receive  Buffers */
    EC_T_DWORD                  dwTxBuffers;            /* [in]     amount of Transmit Buffers (Frames) */
} EC_PACKED(8) EC_T_LINK_PARMS_RTL8139;
#include EC_PACKED_INCLUDESTOP

/* Realtek RTL8169      */
/* ==================== */
#define EC_LINK_PARMS_SIGNATURE_RTL8169_PATTERN (EC_T_DWORD)0x0000CA50
#define EC_LINK_PARMS_SIGNATURE_RTL8169_VERSION (EC_T_DWORD)0x00000002
#define EC_LINK_PARMS_SIGNATURE_RTL8169 (EC_T_DWORD)(EC_LINK_PARMS_SIGNATURE|EC_LINK_PARMS_SIGNATURE_RTL8169_PATTERN|EC_LINK_PARMS_SIGNATURE_RTL8169_VERSION)
#define EC_LINK_PARMS_IDENT_RTL8169 "RTL8169"

#include EC_PACKED_INCLUDESTART(8)
typedef struct _EC_T_LINK_PARMS_RTL8169
{
    /** 
     * \brief Link Layer abstraction. 
     * set linkParms.dwSignature = EC_LINK_PARMS_SIGNATURE_RTL8169
     *
     * Must be first, see casts in usage.
     */
    EC_T_LINK_PARMS linkParms;

    EC_T_BOOL       bNotUseDmaBuffers;         /* [in]  Use buffers from DMA (EC_FALSE) or from heap for receive. AllocSend is not supported, when EC_TRUE. */


} EC_PACKED(8) EC_T_LINK_PARMS_RTL8169;
#include EC_PACKED_INCLUDESTOP

/* FreeScale FEC */
/* ============= */
#define EC_LINK_PARMS_SIGNATURE_FSLFEC_PATTERN (EC_T_DWORD)0x0000CA60
#define EC_LINK_PARMS_SIGNATURE_FSLFEC_VERSION1 (EC_T_DWORD)0x00000001
#define EC_LINK_PARMS_SIGNATURE_FSLFEC_VERSION2 (EC_T_DWORD)0x00000002
#define EC_LINK_PARMS_SIGNATURE_FSLFEC_V1 (EC_T_DWORD)(EC_LINK_PARMS_SIGNATURE|EC_LINK_PARMS_SIGNATURE_FSLFEC_PATTERN|EC_LINK_PARMS_SIGNATURE_FSLFEC_VERSION1)
#define EC_LINK_PARMS_SIGNATURE_FSLFEC_V2 (EC_T_DWORD)(EC_LINK_PARMS_SIGNATURE|EC_LINK_PARMS_SIGNATURE_FSLFEC_PATTERN|EC_LINK_PARMS_SIGNATURE_FSLFEC_VERSION2)
#define EC_LINK_PARMS_SIGNATURE_FSLFEC EC_LINK_PARMS_SIGNATURE_FSLFEC_V2
#define EC_LINK_PARMS_IDENT_FSLFEC "FslFec"

typedef enum _EC_T_FEC_TYPE
{
    eFEC_IMX25 = 0, /* MAC on Freescale i.MX25 (ARM9; ARMv5) */
    eFEC_IMX28 = 1, /* MAC on Freescale i.MX28 (ARM9; ARMv5) */
    eFEC_IMX53 = 2, /* MAC on Freescale i.MX53 (ARM Cortex-A8; ARMv7-a) */
    eFEC_IMX6  = 3, /* MAC on Freescale i.MX6  (ARM Cortex-A9 Single/Dual/Quad; ARMv7-a) */
    eFEC_VF6   = 4, /* MAC on Freescale VYBRID VF6xx (ARM Cortex-A5 + Cortex-M4) */
} EC_T_FEC_TYPE;

#include EC_PACKED_INCLUDESTART(8)
typedef struct _EC_T_LINK_PARMS_FSLFEC1
{
    /** 
     * \brief Link Layer abstraction. 
     * set linkParms.dwSignature = EC_LINK_PARMS_SIGNATURE_FSLFEC
     *
     * Must be first, see casts in usage.
     */
    EC_T_LINK_PARMS linkParms;

    EC_T_DWORD                  dwRxBuffers;            /* [in]     Low word : Number of receive buffers */
    EC_T_DWORD                  dwTxBuffers;            /* [in]     Low word : Number of transmit buffers */

    EC_T_FEC_TYPE               eFecType;
    EC_T_PHYINTERFACE           ePhyInterface;
    EC_T_BYTE                   abyStationAddress[6];   /* [in]  MAC station address */

} EC_PACKED(8) EC_T_LINK_PARMS_FSLFEC1;
#include EC_PACKED_INCLUDESTOP

#include EC_PACKED_INCLUDESTART(8)
typedef struct _EC_T_LINK_PARMS_FSLFEC2
{
    /**
    * \brief Link Layer abstraction.
    * set linkParms.dwSignature = EC_LINK_PARMS_SIGNATURE_FSLFEC
    *
    * Must be first, see casts in usage.
    */
    EC_T_LINK_PARMS linkParms;

    EC_T_DWORD                  dwRxBuffers;            /* [in]     Low word : Number of receive buffers */
    EC_T_DWORD                  dwTxBuffers;            /* [in]     Low word : Number of transmit buffers */

    EC_T_FEC_TYPE               eFecType;
    EC_T_PHYINTERFACE           ePhyInterface;
    EC_T_BYTE                   abyStationAddress[6];   /* [in]  MAC station address */

    /* New parameters in version 2 */
    EC_T_BOOL                   bUseDmaBuffers;         /* [in]  Use buffers from DMA (EC_TRUE) or from heap for receive and AllocSend not supported (EC_FALSE) */

} EC_PACKED(8) EC_T_LINK_PARMS_FSLFEC2;
#include EC_PACKED_INCLUDESTOP

typedef struct _EC_T_LINK_PARMS_FSLFEC2 EC_T_LINK_PARMS_FSLFEC;


/* FreeScale TSEC/eTSEC */
/* ==================== */
#define EC_LINK_PARMS_SIGNATURE_ETSEC_PATTERN (EC_T_DWORD)0x0000CA70
#define EC_LINK_PARMS_SIGNATURE_ETSEC_VERSION (EC_T_DWORD)0x00000002
#define EC_LINK_PARMS_SIGNATURE_ETSEC (EC_T_DWORD)(EC_LINK_PARMS_SIGNATURE|EC_LINK_PARMS_SIGNATURE_ETSEC_PATTERN|EC_LINK_PARMS_SIGNATURE_ETSEC_VERSION)
#define EC_LINK_PARMS_IDENT_ETSEC "ETSEC"

#include EC_PACKED_INCLUDESTART(8)
typedef struct _EC_T_LINK_PARMS_ETSEC
{
    /** 
     * \brief Link Layer abstraction. 
     * set linkParms.dwSignature = EC_LINK_PARMS_SIGNATURE_ETSEC
     *
     * Must be first, see casts in usage.
     */
    EC_T_LINK_PARMS linkParms;

    EC_T_DWORD                  dwRegisterBase;         /* [in] Physical base address of register block (4k) */
    EC_T_DWORD                  dwLocalMdioBase;        /* [in] Physical base address of local MDIO register block (4k). 
                                                                For the eTSEC V1 or TSEC this is the same as dwRegisterBase, for
                                                                the eTSEC V2 it's not. */
    EC_T_DWORD                  dwPhyMdioBase;          /* [in] Physical base address of MDIO register block (4k). 
                                                                This is the MDIO base of the (e)TSEC where the PHY (MII bus)
                                                                is physically connected to (MII interface is shared by (e)TSEC's). */
    
#define ETSEC_FIXED_LINK  0xFFFFFFFF
    
    EC_T_DWORD                  dwPhyAddr;              /* [in] PHY address on MII bus. ETSEC_FIXED_LINK if fixed link configuration */
    EC_T_DWORD                  dwTbiPhyAddr;           /* [in] Address of internal TBI phy. Any address from [0..31] can be used here,
                                                                but the address shouldn't collide with any external PHY connected to
                                                                the external MII bus */

#define ETSEC_LINKFLAG_LINKOK                (1 << 0)
#define ETSEC_LINKFLAG_1000baseT_Full        (1 << 1)
#define ETSEC_LINKFLAG_1000baseT_Half        (1 << 2)
#define ETSEC_LINKFLAG_100baseT_Full         (1 << 3)
#define ETSEC_LINKFLAG_100baseT_Half         (1 << 4)
#define ETSEC_LINKFLAG_10baseT_Full          (1 << 5)
#define ETSEC_LINKFLAG_10baseT_Half          (1 << 6)
    
    EC_T_DWORD                  dwFixedLinkVal;         /* [in]  Only evaluated if dwPhyAddr == FIXED_LINK. Set to one of the
                                                                 ETSEC_LINKFLAG_* macros. I.e. ETSEC_LINKFLAG_1000baseT_Full */
    EC_T_BYTE                   abyStationAddress[6];   /* [in]  MAC station address */
    EC_T_BYTE                   byRes[2];
    EC_T_VOID*                  oMiiBusMtx;             /* [in]  This mutex protect the access to the (shared) MII bus. Set to 0 if mutex shouldn't be used.
                                                                 The MII bus is shared between eTSEC instances. So this mutex should be created once and 
                                                                 assigned here for all Linklayer instances */
    
    /* Interrupt handling */
    EC_T_DWORD                  dwRxInterrupt;          /* [in]  Receive interrupt number (IRQ) */

    EC_T_BOOL                   bNotUseDmaBuffers;      /* [in]  Use buffers from DMA (EC_FALSE) or from heap for receive. AllocSend is not supported, when EC_TRUE. */
} EC_PACKED(8) EC_T_LINK_PARMS_ETSEC;
#include EC_PACKED_INCLUDESTOP

/* NDISUIO Windows CE   */
/* ==================== */
#define EC_LINK_PARMS_SIGNATURE_NDISUIO_PATTERN (EC_T_DWORD)0x0000CA80
#define EC_LINK_PARMS_SIGNATURE_NDISUIO_VERSION (EC_T_DWORD)0x00000001
#define EC_LINK_PARMS_SIGNATURE_NDISUIO (EC_T_DWORD)(EC_LINK_PARMS_SIGNATURE|EC_LINK_PARMS_SIGNATURE_NDISUIO_PATTERN|EC_LINK_PARMS_SIGNATURE_NDISUIO_VERSION)
#define EC_LINK_PARMS_IDENT_NDISUIO "NdisUio"

#define ECAT_NDISUIO_DEVNAME    TEXT("ECT1:")
#define MAX_LEN_NDISUIO_ADAPTER_NAME ((EC_T_DWORD)64)

#include EC_PACKED_INCLUDESTART(8)
typedef struct _EC_T_LINK_PARMS_NDISUIO
{
    /** 
     * \brief Link Layer abstraction. 
     * set linkParms.dwSignature = EC_LINK_PARMS_SIGNATURE_NDISUIO
     *
     * Must be first, see casts in usage.
     */
    EC_T_LINK_PARMS linkParms;

    EC_T_WCHAR      szNetworkAdapterName[MAX_LEN_NDISUIO_ADAPTER_NAME];

} EC_PACKED(8) EC_T_LINK_PARMS_NDISUIO;
#include EC_PACKED_INCLUDESTOP

/* Davicom DM9000       */
/* ==================== */
#define EC_LINK_PARMS_SIGNATURE_DM9000_PATTERN (EC_T_DWORD)0x0000CA90
#define EC_LINK_PARMS_SIGNATURE_DM9000_VERSION (EC_T_DWORD)0x00000001
#define EC_LINK_PARMS_SIGNATURE_DM9000 (EC_T_DWORD)(EC_LINK_PARMS_SIGNATURE|EC_LINK_PARMS_SIGNATURE_DM9000_PATTERN|EC_LINK_PARMS_SIGNATURE_DM9000_VERSION)
#define EC_LINK_PARMS_IDENT_DM9000 "DM9000"

#include EC_PACKED_INCLUDESTART(8)
typedef struct _EC_T_LINK_OPENPARMS_DM9000
{
    /** 
     * \brief Link Layer abstraction. 
     * set linkParms.dwSignature = EC_LINK_PARMS_SIGNATURE_DM9000
     *
     * Must be first, see casts in usage.
     */
    EC_T_LINK_PARMS linkParms;

    EC_T_DWORD      dwBaseAddress;
    EC_T_DWORD      dwBaseIO;
    EC_T_DWORD      dwIrqNum;
    EC_T_DWORD      dwSysIntr;
    EC_T_DWORD      dwFrameBufferLen;

} EC_PACKED(8) EC_T_LINK_PARMS_DM9000;
#include EC_PACKED_INCLUDESTOP

/* Marvell PXA MFU */
/* =============== */
#define EC_LINK_PARMS_SIGNATURE_PXAMFU_PATTERN (EC_T_DWORD)0x0000CAA0
#define EC_LINK_PARMS_SIGNATURE_PXAMFU_VERSION (EC_T_DWORD)0x00000001
#define EC_LINK_PARMS_SIGNATURE_PXAMFU (EC_T_DWORD)(EC_LINK_PARMS_SIGNATURE|EC_LINK_PARMS_SIGNATURE_PXAMFU_PATTERN|EC_LINK_PARMS_SIGNATURE_PXAMFU_VERSION)
#define EC_LINK_PARMS_IDENT_PXAMFU "PXAMFU"

#include EC_PACKED_INCLUDESTART(8)
typedef struct _EC_T_LINK_PARMS_PXAMFU
{
    /** 
     * \brief Link Layer abstraction. 
     * set linkParms.dwSignature = EC_LINK_PARMS_SIGNATURE_PXAMFU
     *
     * Must be first, see casts in usage.
     */
    EC_T_LINK_PARMS linkParms;
    
} EC_PACKED(8) EC_T_LINK_PARMS_PXAMFU;
#include EC_PACKED_INCLUDESTOP

/* Beckhoff CCAT */
/* =============== */
#define EC_LINK_PARMS_SIGNATURE_CCAT_PATTERN (EC_T_DWORD)0x0000CAB0
#define EC_LINK_PARMS_SIGNATURE_CCAT_VERSION (EC_T_DWORD)0x00000001
#define EC_LINK_PARMS_SIGNATURE_CCAT (EC_T_DWORD)(EC_LINK_PARMS_SIGNATURE|EC_LINK_PARMS_SIGNATURE_CCAT_PATTERN|EC_LINK_PARMS_SIGNATURE_CCAT_VERSION)
#define EC_LINK_PARMS_IDENT_CCAT "CCAT"

#include EC_PACKED_INCLUDESTART(8)
typedef struct _EC_T_LINK_PARMS_CCAT
{
    /** 
     * \brief Link Layer abstraction. 
     * set linkParms.dwSignature = EC_LINK_PARMS_SIGNATURE_CCAT
     *
     * Must be first, see casts in usage.
     */
    EC_T_LINK_PARMS linkParms;

} EC_PACKED(8) EC_T_LINK_PARMS_CCAT;
#include EC_PACKED_INCLUDESTOP

/* Texas Instruments CPSW */
/* ====================== */
#define EC_LINK_PARMS_SIGNATURE_CPSW_PATTERN (EC_T_DWORD)0x0000CAC0
#define EC_LINK_PARMS_SIGNATURE_CPSW_VERSION (EC_T_DWORD)0x00000001
#define EC_LINK_PARMS_SIGNATURE_CPSW (EC_T_DWORD)(EC_LINK_PARMS_SIGNATURE|EC_LINK_PARMS_SIGNATURE_CPSW_PATTERN|EC_LINK_PARMS_SIGNATURE_CPSW_VERSION)
#define EC_LINK_PARMS_IDENT_CPSW "CPSW"

/* Needed for IOCTL */
#define CPSW_LINKFLAG_LINKOK                (1 << 0)
#define CPSW_LINKFLAG_1000baseT_Full        (1 << 1)
#define CPSW_LINKFLAG_1000baseT_Half        (1 << 2)
#define CPSW_LINKFLAG_100baseT_Full         (1 << 3)
#define CPSW_LINKFLAG_100baseT_Half         (1 << 4)
#define CPSW_LINKFLAG_10baseT_Full          (1 << 5)
#define CPSW_LINKFLAG_10baseT_Half          (1 << 6)

typedef enum _EC_T_CPSW_TYPE
{
    eCPSW_AM33XX  = 0, /* AM33xx (e.g. Beaglebone) */
    eCPSW_AM387X  = 1, /* DM814x/AM387x (e.g. Mistral/TI 814X/387X BASE EVM) */
    eCPSW_AM437X  = 2, /* AM437x */
    eCPSW_AM57X  = 3,  /* AM57x */
} EC_T_CPSW_TYPE;


typedef struct _EC_T_LINK_PARMS_CPSW
{
    /** 
     * \brief Link Layer abstraction. 
     * set linkParms.dwSignature = EC_LINK_PARMS_SIGNATURE_CPSW
     *
     * Must be first, see casts in usage.
     */
    EC_T_LINK_PARMS linkParms;

    EC_T_CPSW_TYPE              eCpswType;              /* [in]  CPSW chip type */
    EC_T_DWORD                  dwPhyAddr;              /* [in]  PHY address (0 .. 31) on MII bus */
    EC_T_DWORD                  dwPortPrio;             /* [in]  0 (lowest), 1 (highest) */
    EC_T_BOOL                   bMaster;                /* [in]  TRUE := Initialize MAC */
    EC_T_BOOL                   bPhyRestartAutoNegotiation;
    EC_T_PHYINTERFACE           ePhyInterface;          /* [in]  PHY connection type (only GMII, RGMII are supported) */

    /* Interrupt handling */
    EC_T_DWORD                  dwRxInterrupt;          /* [in]  Receive interrupt number (IRQ) */

    EC_T_BOOL                   bNotUseDmaBuffers;      /* [in]  Use buffers from DMA (EC_FALSE) or from heap for receive. AllocSend is not supported, when EC_TRUE. */
} EC_T_LINK_PARMS_CPSW;


/* Xilinx GEM */
/* ====================== */
#define EC_LINK_PARMS_SIGNATURE_GEM_PATTERN (EC_T_DWORD)0x0000CAD0
#define EC_LINK_PARMS_SIGNATURE_GEM_VERSION (EC_T_DWORD)0x00000002
#define EC_LINK_PARMS_SIGNATURE_GEM         (EC_T_DWORD)(EC_LINK_PARMS_SIGNATURE|EC_LINK_PARMS_SIGNATURE_GEM_PATTERN|EC_LINK_PARMS_SIGNATURE_GEM_VERSION)
#define EC_LINK_PARMS_IDENT_GEM "GEM"

/* Needed for IOCTL */
#define GEM_LINKFLAG_LINKOK                (1 << 0)
#define GEM_LINKFLAG_1000baseT_Full        (1 << 1)
#define GEM_LINKFLAG_100baseT_Full         (1 << 3)

#define GEM_SLAVECNT 2

/* Select the source of the Rx clock, control and data signals */
typedef enum _EC_T_GEM_RXSOURCE
{
    eGemRxSource_MIO = 0,
    eGemRxSource_EMIO
} EC_T_GEM_RXSOURCE;

typedef struct _EC_T_LINK_PARMS_GEM
{
    /**
    * \brief Link Layer abstraction.
    * set linkParms.dwSignature = EC_LINK_PARMS_SIGNATURE_GEM
    *
    * Must be first, see casts in usage.
    */
    EC_T_LINK_PARMS linkParms;

    EC_T_GEM_RXSOURCE           eRxSource;              /* [in]  Select the source of the Rx clock, control and data signals */
    EC_T_DWORD                  dwPhyAddr;              /* [in]  PHY address (0 .. 31) on MII bus */

    /* Interrupt handling */
    EC_T_DWORD                  dwRxInterrupt;          /* [in]  Receive interrupt number (IRQ) */

    /* New parameters in version 2 */
    EC_T_BOOL                   bUseDmaBuffers;         /* [in]  Use buffers from DMA (EC_TRUE) or from heap for receive. AllocSend is not supported, when EC_FALSE. */
    EC_T_BOOL                   bNoPhyAccess;           /* [in]  When EC_FALSE, Link layer should initialize PHY and read link status (connected/disconnected)
                                                                 When EC_TRUE, than client is responsible of PHY initialization and clock initialization */

} EC_T_LINK_PARMS_GEM;

/* SMSC LAN9218i */
/* ============= */
#define EC_LINK_PARMS_SIGNATURE_L9218I_PATTERN (EC_T_DWORD)0x0000CAE0
#define EC_LINK_PARMS_SIGNATURE_L9218I_VERSION (EC_T_DWORD)0x00000001
#define EC_LINK_PARMS_SIGNATURE_L9218I (EC_T_DWORD)(EC_LINK_PARMS_SIGNATURE|EC_LINK_PARMS_SIGNATURE_L9218I_PATTERN|EC_LINK_PARMS_SIGNATURE_L9218I_VERSION)
#define EC_LINK_PARMS_IDENT_L9218I "L9218i"

typedef struct _EC_T_LINK_PARMS_L9218I
{
    /** 
     * \brief Link Layer abstraction. 
     * set linkParms.dwSignature = EC_LINK_PARMS_SIGNATURE_L9218I
     *
     * Must be first, see casts in usage.
     */
    EC_T_LINK_PARMS linkParms;

    EC_T_DWORD                  dwTxBuffers;            /* [in]     amount of Transmit Buffers (Frames) */
} EC_T_LINK_PARMS_L9218I;


/* Xilinx LogiCORE IP XPS Ethernet Lite Media Access Controller */
/* ============================================================ */
#define EC_LINK_PARMS_SIGNATURE_EMAC_PATTERN (EC_T_DWORD)0x0000CAF0
#define EC_LINK_PARMS_SIGNATURE_EMAC_VERSION (EC_T_DWORD)0x00000001
#define EC_LINK_PARMS_SIGNATURE_EMAC (EC_T_DWORD)(EC_LINK_PARMS_SIGNATURE|EC_LINK_PARMS_SIGNATURE_EMAC_PATTERN|EC_LINK_PARMS_SIGNATURE_EMAC_VERSION)
#define EC_LINK_PARMS_IDENT_EMAC "EMAC"

#include EC_PACKED_INCLUDESTART(8)
typedef struct _EC_T_LINK_PARMS_EMAC
{
    /** 
     * \brief Link Layer abstraction. 
     * set linkParms.dwSignature = EC_LINK_PARMS_SIGNATURE_EMAC
     *
     * Must be first, see casts in usage.
     */
    EC_T_LINK_PARMS linkParms;

    EC_T_DWORD                  dwRegisterBase;         /* [in] Physical base address of register block (4k) */
    EC_T_DWORD                  dwPhyAddr;              /* [in] PHY address on MII bus */
    EC_T_DWORD                  dwRxInterrupt;          /* [in]  Receive interrupt number (IRQ) */

} EC_PACKED(8) EC_T_LINK_PARMS_EMAC;
#include EC_PACKED_INCLUDESTOP

/* Intel EG20T MAC */
/* =============== */
#define EC_LINK_PARMS_SIGNATURE_EG20T_PATTERN (EC_T_DWORD)0x0000CB00
#define EC_LINK_PARMS_SIGNATURE_EG20T_VERSION (EC_T_DWORD)0x00000001
#define EC_LINK_PARMS_SIGNATURE_EG20T (EC_T_DWORD)(EC_LINK_PARMS_SIGNATURE|EC_LINK_PARMS_SIGNATURE_EG20T_PATTERN|EC_LINK_PARMS_SIGNATURE_EG20T_VERSION)
#define EC_LINK_PARMS_IDENT_EG20T "EG20T"

/* Needed for IOCTL */
#define EG20T_LINKFLAG_LINKOK                (1 << 0)
#define EG20T_LINKFLAG_1000baseT_Full        (1 << 1)
#define EG20T_LINKFLAG_1000baseT_Half        (1 << 2)
#define EG20T_LINKFLAG_100baseT_Full         (1 << 3)
#define EG20T_LINKFLAG_100baseT_Half         (1 << 4)
#define EG20T_LINKFLAG_10baseT_Full          (1 << 5)
#define EG20T_LINKFLAG_10baseT_Half          (1 << 6)

#include EC_PACKED_INCLUDESTART(8)
typedef struct _EC_T_LINK_PARMS_EG20T
{
    /** 
     * \brief Link Layer abstraction. 
     * set linkParms.dwSignature = EC_LINK_PARMS_SIGNATURE_EG20T
     *
     * Must be first, see casts in usage.
     */
    EC_T_LINK_PARMS linkParms;

} EC_PACKED(8) EC_T_LINK_PARMS_EG20T;
#include EC_PACKED_INCLUDESTOP

/*****************************************************************************/
/* PMX1000-R6040                                                             */
/*****************************************************************************/
#define EC_LINK_PARMS_SIGNATURE_R6040_PATTERN (EC_T_DWORD)0x0000CB10
#define EC_LINK_PARMS_SIGNATURE_R6040_VERSION (EC_T_DWORD)0x00000001
#define EC_LINK_PARMS_SIGNATURE_R6040 (EC_T_DWORD)(EC_LINK_PARMS_SIGNATURE|EC_LINK_PARMS_SIGNATURE_R6040_PATTERN|EC_LINK_PARMS_SIGNATURE_R6040_VERSION)
#define EC_LINK_PARMS_IDENT_R6040 "R6040"

#include EC_PACKED_INCLUDESTART(8)
typedef struct _EC_T_LINK_PARMS_R6040
{
    /** 
     * \brief Link Layer abstraction. 
     * set linkParms.dwSignature = EC_LINK_PARMS_SIGNATURE_R6040
     *
     * Must be first, see casts in usage.
     */
    EC_T_LINK_PARMS linkParms;

} EC_PACKED(8) EC_T_LINK_PARMS_R6040;
#include EC_PACKED_INCLUDESTOP

/* SockRaw Linux */
/* ============= */
#define EC_LINK_PARMS_SIGNATURE_SOCKRAW_PATTERN (EC_T_DWORD)0x0000CC10
#define EC_LINK_PARMS_SIGNATURE_SOCKRAW_VERSION (EC_T_DWORD)0x00000001
#define EC_LINK_PARMS_SIGNATURE_SOCKRAW (EC_T_DWORD)(EC_LINK_PARMS_SIGNATURE|EC_LINK_PARMS_SIGNATURE_SOCKRAW_PATTERN|EC_LINK_PARMS_SIGNATURE_SOCKRAW_VERSION)
#define EC_LINK_PARMS_IDENT_SOCKRAW "SockRaw"

#define MAX_LEN_SOCKRAW_ADAPTER_NAME 64

#include EC_PACKED_INCLUDESTART(8)
typedef struct _EC_T_LINK_PARMS_SOCKRAW
{
    /** 
     * \brief Link Layer abstraction. 
     * set linkParms.dwSignature = EC_LINK_PARMS_SIGNATURE_SOCKRAW
     *
     * Must be first, see casts in usage.
     */
    EC_T_LINK_PARMS linkParms;

    EC_T_CHAR       szAdapterName[MAX_LEN_SOCKRAW_ADAPTER_NAME];

    EC_T_BOOL       bDisableForceBroadcast;
} EC_PACKED(8) EC_T_LINK_PARMS_SOCKRAW;
#include EC_PACKED_INCLUDESTOP

/*****************************************************************************/
/* NIOS                                                                      */
/*****************************************************************************/
#define EC_LINK_PARMS_SIGNATURE_NIOS_PATTERN (EC_T_DWORD)0x0000CB20
#define EC_LINK_PARMS_SIGNATURE_NIOS_VERSION (EC_T_DWORD)0x00000001
#define EC_LINK_PARMS_SIGNATURE_NIOS (EC_T_DWORD)(EC_LINK_PARMS_SIGNATURE|EC_LINK_PARMS_SIGNATURE_NIOS_PATTERN|EC_LINK_PARMS_SIGNATURE_NIOS_VERSION)
#define EC_LINK_PARMS_IDENT_NIOS "NIOS"

#include EC_PACKED_INCLUDESTART(8)
typedef struct _EC_T_LINK_PARMS_NIOS
{
    /** 
     * \brief Link Layer abstraction. 
     * set linkParms.dwSignature = EC_LINK_PARMS_SIGNATURE_NIOS
     *
     * Must be first, see casts in usage.
     */
    EC_T_LINK_PARMS linkParms;

} EC_PACKED(8) EC_T_LINK_PARMS_NIOS;
#include EC_PACKED_INCLUDESTOP

/*****************************************************************************/
/* RIN32M3                                                                      */
/*****************************************************************************/
#define EC_LINK_PARMS_SIGNATURE_RIN32M3_PATTERN (EC_T_DWORD)0x0000CD10
#define EC_LINK_PARMS_SIGNATURE_RIN32M3_VERSION (EC_T_DWORD)0x00000001
#define EC_LINK_PARMS_SIGNATURE_RIN32M3 (EC_T_DWORD)(EC_LINK_PARMS_SIGNATURE|EC_LINK_PARMS_SIGNATURE_RIN32M3_PATTERN|EC_LINK_PARMS_SIGNATURE_RIN32M3_VERSION)
#define EC_LINK_PARMS_IDENT_RIN32M3 "RIN32M3"

#include EC_PACKED_INCLUDESTART(8)
typedef struct _EC_T_LINK_PARMS_RIN32M3
{
    /**
     * \brief Link Layer abstraction.
     * set linkParms.dwSignature = EC_LINK_PARMS_SIGNATURE_RIN32M3
     *
     * Must be first, see casts in usage.
     */
    EC_T_LINK_PARMS linkParms;

} EC_PACKED(8) EC_T_LINK_PARMS_RIN32M3;
#include EC_PACKED_INCLUDESTOP

/*****************************************************************************/
/* DW3504                                                                      */
/*****************************************************************************/
#define EC_LINK_PARMS_SIGNATURE_DW3504_PATTERN (EC_T_DWORD)0x0000CE10
#define EC_LINK_PARMS_SIGNATURE_DW3504_VERSION1 (EC_T_DWORD)0x00000001
#define EC_LINK_PARMS_SIGNATURE_DW3504_VERSION2 (EC_T_DWORD)0x00000002
#define EC_LINK_PARMS_SIGNATURE_DW3504_V1 (EC_T_DWORD)(EC_LINK_PARMS_SIGNATURE|EC_LINK_PARMS_SIGNATURE_DW3504_PATTERN|EC_LINK_PARMS_SIGNATURE_DW3504_VERSION1)
#define EC_LINK_PARMS_SIGNATURE_DW3504_V2 (EC_T_DWORD)(EC_LINK_PARMS_SIGNATURE|EC_LINK_PARMS_SIGNATURE_DW3504_PATTERN|EC_LINK_PARMS_SIGNATURE_DW3504_VERSION2)
#define EC_LINK_PARMS_SIGNATURE_DW3504 EC_LINK_PARMS_SIGNATURE_DW3504_V2
#define EC_LINK_PARMS_IDENT_DW3504 "DW3504"

#include EC_PACKED_INCLUDESTART(8)
typedef struct _EC_T_LINK_PARMS_DW3504_1
{
    /**
    * \brief Link Layer abstraction.
    * set linkParms.dwSignature = EC_LINK_PARMS_SIGNATURE_DW3504
    *
    * Must be first, see casts in usage.
    */
    EC_T_LINK_PARMS linkParms;

    EC_T_DWORD                  dwPhyAddr;              /* [in]  PHY address (0 .. 31) on MII bus */
    EC_T_DWORD                  dwRegisterBasePhys;     /* [in] Physical base address of register block (8k) */

} EC_PACKED(8) EC_T_LINK_PARMS_DW3504_1;
#include EC_PACKED_INCLUDESTOP

#include EC_PACKED_INCLUDESTART(8)
typedef struct _EC_T_LINK_PARMS_DW3504_2
{
    /**
    * \brief Link Layer abstraction.
    * set linkParms.dwSignature = EC_LINK_PARMS_SIGNATURE_DW3504
    *
    * Must be first, see casts in usage.
    */
    EC_T_LINK_PARMS linkParms;

    EC_T_DWORD                  dwPhyAddr;              /* [in]  PHY address (0 .. 31) on MII bus */
    EC_T_DWORD                  dwRegisterBasePhys;     /* [in] Physical base address of register block (8k) */

    /* New parameters in version 2 */
    EC_T_PHYINTERFACE           ePhyInterface;          /* [in]  PHY connection type. Use one of enum values */
    EC_T_BOOL                   bNotUseDmaBuffers;      /* [in]  Use buffers from DMA (EC_FALSE) or from heap for receive. AllocSend is not supported, when EC_TRUE. */

} EC_PACKED(8) EC_T_LINK_PARMS_DW3504_2;
#include EC_PACKED_INCLUDESTOP

typedef struct _EC_T_LINK_PARMS_DW3504_2 EC_T_LINK_PARMS_DW3504;

/*****************************************************************************/
/* RZT1                                                                      */
/*****************************************************************************/
#define EC_LINK_PARMS_SIGNATURE_RZT1_PATTERN (EC_T_DWORD)0x0000CD10
#define EC_LINK_PARMS_SIGNATURE_RZT1_VERSION (EC_T_DWORD)0x00000001
#define EC_LINK_PARMS_SIGNATURE_RZT1 (EC_T_DWORD)(EC_LINK_PARMS_SIGNATURE|EC_LINK_PARMS_SIGNATURE_RZT1_PATTERN|EC_LINK_PARMS_SIGNATURE_RZT1_VERSION)
#define EC_LINK_PARMS_IDENT_RZT1 "RZT1"

#include EC_PACKED_INCLUDESTART(8)
typedef struct _EC_T_LINK_PARMS_RZT1
{
    /**
    * \brief Link Layer abstraction.
    * set linkParms.dwSignature = EC_LINK_PARMS_SIGNATURE_RZT1
    *
    * Must be first, see casts in usage.
    */
    EC_T_LINK_PARMS linkParms;

} EC_PACKED(8) EC_T_LINK_PARMS_RZT1;
#include EC_PACKED_INCLUDESTOP

/*****************************************************************************/
/* ICSS - Texas Instruments industrial communications subsystem Ethernet Media Access Controller (ICSS) */
/*****************************************************************************/
#define EC_LINK_PARMS_SIGNATURE_ICSS_PATTERN (EC_T_DWORD)0x0000CB30
#define EC_LINK_PARMS_SIGNATURE_ICSS_VERSION (EC_T_DWORD)0x00000001
#define EC_LINK_PARMS_SIGNATURE_ICSS (EC_T_DWORD)(EC_LINK_PARMS_SIGNATURE|EC_LINK_PARMS_SIGNATURE_ICSS_PATTERN|EC_LINK_PARMS_SIGNATURE_ICSS_VERSION)
#define EC_LINK_PARMS_IDENT_ICSS "ICSS"

/* Needed for IOCTL */
#define ICSS_LINKFLAG_LINKOK                (1 << 0)
#define ICSS_LINKFLAG_1000baseT_Full        (1 << 1)
#define ICSS_LINKFLAG_1000baseT_Half        (1 << 2)
#define ICSS_LINKFLAG_100baseT_Full         (1 << 3)
#define ICSS_LINKFLAG_100baseT_Half         (1 << 4)
#define ICSS_LINKFLAG_10baseT_Full          (1 << 5)
#define ICSS_LINKFLAG_10baseT_Half          (1 << 6)

typedef struct _EC_T_LINK_PLATFORMDATA_ICSS
{
    EC_T_DWORD dwPrussMiiMdioRegs;   /*! MDIO Base register */
    EC_T_DWORD dwDataRam0BaseAddr;   /*! DataRam 0 Base Address */
    EC_T_DWORD dwDataRam1BaseAddr;   /*! DataRam 0 Base Address */
    EC_T_DWORD dwL3OcmcBaseAddr;     /*! L3 OCMC Base Address */
    EC_T_DWORD dwSharedDataRamBaseAddrs;    /*! PRUSS Shared RAM Base Address */
    EC_T_DWORD dwPrussIntcRegs;      /*! Pruss INTC Register Base Address */
    EC_T_DWORD dwPrussPru0CtrlRegs;  /*! PRU0 Control register Base Address */
    EC_T_DWORD dwPrussPru1CtrlRegs;  /*! PRU1 Control register Base Address */
    EC_T_DWORD dwPrussIepRegs;       /*! PRUSS IEP register Base Address */
    EC_T_DWORD dwPrussCfgRegs;       /*! Pruss CFG register Base Address */
    EC_T_DWORD dwPrussMiiRtCfgRegsBaseAddr;  /*! MII RT Config register Base Address */

} EC_T_LINK_PLATFORMDATA_ICSS;

#include EC_PACKED_INCLUDESTART(8)
typedef struct _EC_T_LINK_PARMS_ICSS
{
    /**
    * \brief Link Layer abstraction.
    * set linkParms.dwSignature = EC_LINK_PARMS_SIGNATURE_ICSS
    *
    * Must be first, see casts in usage.
    */
    EC_T_LINK_PARMS linkParms;

    EC_T_LINK_PLATFORMDATA_ICSS oPlatCfg;
    EC_T_DWORD                  dwPort;                 /* [in]  Port number inside each PRU: ETH0 (0) or ETH1 (1)*/

} EC_PACKED(8) EC_T_LINK_PARMS_ICSS;
#include EC_PACKED_INCLUDESTOP

/* SH Eth */
#define EC_LINK_PARMS_SIGNATURE_SHETH_PATTERN (EC_T_DWORD)0x0000CF10
#define EC_LINK_PARMS_SIGNATURE_SHETH_VERSION (EC_T_DWORD)0x00000001
#define EC_LINK_PARMS_SIGNATURE_SHETH (EC_T_DWORD)(EC_LINK_PARMS_SIGNATURE|EC_LINK_PARMS_SIGNATURE_SHETH_PATTERN|EC_LINK_PARMS_SIGNATURE_SHETH_VERSION)
#define EC_LINK_PARMS_IDENT_SHETH "SHEth"

/* Super H Ethernet controller type */
typedef enum
{
    eSHEth_R8A777X,
    eSHEth_R8A779X,
    eSHEth_SH7724,
    eSHEth_SH7757,
    eSHEth_SH7757_GIGA,
    eSHEth_SH7734,
    eSHEth_SH7763,
    eSHEth_R8A7740,        /* R Mobile */
    eSHEth_R7S72100,
    eSHEth_SH7619,
    eSHEth_SH771X,         /* old stuff */
    eSHEth_R8A77400,
    eSHEth_R8A77435,
    eSHEth_R8A77430,       /* RZ/G1E */
    eSHEth_R8A77450,       /* RZ/G1E */

    /* Borland C++ datatype alignment correction */
    eSHEth_BCppDummy   = 0xFFFFFFFF
} EC_T_SHETH_TYPE;

#include EC_PACKED_INCLUDESTART(8)
typedef struct _EC_T_LINK_PARMS_SHETH
{
    /**
     * \brief Link Layer abstraction.
     * set linkParms.dwSignature = EC_LINK_PARMS_SIGNATURE_SHETH
     *
     * Must be first, see casts in usage.
     */
    EC_T_LINK_PARMS             linkParms;

    /* controller type identification string (zero terminated) */
    EC_T_SHETH_TYPE             eType;
    EC_T_BYTE                   abyStationAddress[6];
    EC_T_DWORD                  dwBaseAddr;             /* Base address */
    EC_T_BYTE                   byPhyAddr;

    /* [in]  Use buffers from DMA (EC_FALSE) or from heap for receive. AllocSend is not supported, when EC_TRUE. */
    EC_T_BOOL                   bNotUseDmaBuffers;
} EC_PACKED(8) EC_T_LINK_PARMS_SHETH;
#include EC_PACKED_INCLUDESTOP

/*-FUNCTIONS DECLARATION-----------------------------------------------------*/
ATEMLL_API EC_T_DWORD emllRegisterI8255x
    (EC_T_LINK_DRV_DESC*  pLinkDrvDesc        /* [in,out] link layer driver descriptor */
    ,EC_T_DWORD         dwLinkDrvDescSize);   /* [in]     size in bytes of link layer driver descriptor */

ATEMLL_API EC_T_DWORD emllRegisterI8254x
    (EC_T_LINK_DRV_DESC*  pLinkDrvDesc        /* [in,out] link layer driver descriptor */
    ,EC_T_DWORD         dwLinkDrvDescSize);   /* [in]     size in bytes of link layer driver descriptor */

ATEMLL_API EC_T_DWORD emllRegisterFslFec
(EC_T_LINK_DRV_DESC*  pLinkDrvDesc        /* [in,out] link layer driver descriptor */
 ,EC_T_DWORD         dwLinkDrvDescSize);   /* [in]     size in bytes of link layer driver descriptor */

ATEMLL_API EC_T_DWORD emllRegisterRTL8139
    (EC_T_LINK_DRV_DESC*  pLinkDrvDesc        /* [in,out] link layer driver descriptor */
    ,EC_T_DWORD         dwLinkDrvDescSize);   /* [in]     size in bytes of link layer driver descriptor */
    
ATEMLL_API EC_T_DWORD emllRegisterRTL8169
    (EC_T_LINK_DRV_DESC*  pLinkDrvDesc        /* [in,out] link layer driver descriptor */
    ,EC_T_DWORD         dwLinkDrvDescSize);   /* [in]     size in bytes of link layer driver descriptor */

ATEMLL_API EC_T_DWORD emllRegisterCCAT
    (EC_T_LINK_DRV_DESC*  pLinkDrvDesc        /* [in,out] link layer driver descriptor */
    ,EC_T_DWORD         dwLinkDrvDescSize);   /* [in]     size in bytes of link layer driver descriptor */

ATEMLL_API EC_T_DWORD emllRegisterNdisUio
    (EC_T_LINK_DRV_DESC*  pLinkDrvDesc        /* [in,out] link layer driver descriptor */
    ,EC_T_DWORD         dwLinkDrvDescSize);   /* [in]     size in bytes of link layer driver descriptor */

ATEMLL_API EC_T_DWORD emllRegisterR6040
    (EC_T_LINK_DRV_DESC*  pLinkDrvDesc        /* [in,out] link layer driver descriptor */
    ,EC_T_DWORD         dwLinkDrvDescSize);   /* [in]     size in bytes of link layer driver descriptor */

/*-COMPILER SETTINGS---------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif /* INC_ECLINK */

/*-END OF SOURCE FILE--------------------------------------------------------*/
