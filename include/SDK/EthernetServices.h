/*-----------------------------------------------------------------------------
 * EthernetServices.h       
 * Copyright                acontis technologies GmbH, Weingarten, Germany
 * Response                 Stefan Zintgraf
 * Description              
 *---------------------------------------------------------------------------*/

#ifndef INC_ETHERNETSERVICES
#define INC_ETHERNETSERVICES

/*-INCLUDES------------------------------------------------------------------*/

/*-DEFINES/MACROS------------------------------------------------------------*/
#if (!defined __cplusplus) && (!defined EC_NO_BITFIELDS)
#define EC_NO_BITFIELDS
#endif

#define ETHERNET_FRAME_TYPE_IP                  0x0800
#define ETHERNET_FRAME_TYPE_ARP1                0x0806
#define ETHERNET_FRAME_TYPE_ARP2                0x0807
#define ETHERNET_FRAME_TYPE_RARP                0x8035
#define ETHERNET_FRAME_TYPE_VLAN                0x8100
#define ETHERNET_FRAME_TYPE_SNMP                0x814C
#define ETHERNET_FRAME_TYPE_LOOP                0x9000
#define ETHERNET_FRAME_TYPE_BKHF                0x88A4
#define ETHERNET_FRAME_TYPE_PROFINET            0x8892

#define ETHERNET_MIN_FRAME_LEN                  60      /* frame length without CRC */
#define ETHERNET_MAX_FRAME_LEN                  1514    /* frame length without CRC */
#define ETHERNET_MAX_VLAN_FRAME_LEN             1518    /* frame length without CRC */

#define ETHERNET_MAX_FRAMEBUF_LEN               1536    /* = 0x600 used for memory allocation */

/*-TYPEDEFS/ENUMS------------------------------------------------------------*/
#include EC_PACKED_INCLUDESTART(1)
typedef struct TETHERNET_ADDRESS
{
    EC_T_BYTE b[6];

} EC_PACKED(1) ETHERNET_ADDRESS, *PETHERNET_ADDRESS;
#define ETHERNET_ADDRESS_LEN    (6)

#ifdef __cplusplus
    /* for calculating the hashvalue */
    EC_INLINESTART EC_T_INT operator% (const ETHERNET_ADDRESS& lhs, EC_T_INT hashSize)
    {
    	EC_T_VOID* pvData = (EC_T_VOID*)lhs.b;
        return (EC_GETDWORD((EC_T_DWORD*)pvData) % hashSize);
    } EC_INLINESTOP

    EC_INLINESTART EC_T_BOOL operator== (const TETHERNET_ADDRESS& lhs,  const TETHERNET_ADDRESS &addr)
    {
        return (0 == OsMemcmp(lhs.b, addr.b, ETHERNET_ADDRESS_LEN));
    } EC_INLINESTOP
    EC_INLINESTART EC_T_BOOL operator!= (const TETHERNET_ADDRESS& lhs, const TETHERNET_ADDRESS &addr)
    {
        return (0 != OsMemcmp(lhs.b, addr.b, ETHERNET_ADDRESS_LEN));
    } EC_INLINESTOP
#endif  /* __cplusplus */
    
static const   ETHERNET_ADDRESS        BroadcastEthernetAddress        ={{0xFF,0xFF,0xFF,0xFF,0xFF,0xFF}};
static const   ETHERNET_ADDRESS        FirstMulticastEthernetAddress   ={{0x01,0x00,0x5E,0x00,0x00,0x00}};
static const   ETHERNET_ADDRESS        NullEthernetAddress             ={{0x00,0x00,0x00,0x00,0x00,0x00}};


/*---------------------------------------------------------------------------*/
typedef struct TETHERNET_FRAME 
{
    ETHERNET_ADDRESS    Destination;   /* 0 */
    ETHERNET_ADDRESS    Source;        /* 6 */
    EC_T_WORD           __FrameType;   /*12 */  /* value is big endian. See also EC_ETHFRM_GET_FRAMETYPE. */
} EC_PACKED(1) ETHERNET_FRAME, *PETHERNET_FRAME;
#define ETHERNET_FRAMETYPE_LEN  sizeof(EC_T_WORD)
#define ETHERNET_FRAME_LEN      (2*ETHERNET_ADDRESS_LEN+ETHERNET_FRAMETYPE_LEN)

#ifdef VLAN_FRAME_SUPPORT
#ifdef EC_BIG_ENDIAN
#define EC_ETHFRM_IS_VLAN(p)                ((EC_GETWORD((((EC_T_BYTE*)(p))+12)) == ETHERNET_FRAME_TYPE_VLAN) ? EC_TRUE : EC_FALSE)
#define EC_ETHFRM_GET_FRAMETYPE(p)          EC_GETWORD((((EC_T_BYTE*)(p))+(EC_ETHFRM_IS_VLAN(p)?16:12)))
#define EC_ETHFRM_SET_FRAMETYPE(p, wVal)    EC_SETWORD((((EC_T_BYTE*)(p))+12), (wVal))
#else
#define EC_ETHFRM_IS_VLAN(p)                (((EC_WORDSWAP(EC_GETWORD((((EC_T_BYTE*)(p))+12)))) == ETHERNET_FRAME_TYPE_VLAN) ? EC_TRUE : EC_FALSE)
#define EC_ETHFRM_GET_FRAMETYPE(p)          EC_WORDSWAP(EC_GETWORD((((EC_T_BYTE*)(p))+(EC_ETHFRM_IS_VLAN(p)?16:12))))
#define EC_ETHFRM_SET_FRAMETYPE(p, wVal)    EC_SETWORD((((EC_T_BYTE*)(p))+12), EC_WORDSWAP(wVal))
#endif
#else
#ifdef EC_BIG_ENDIAN
#define EC_ETHFRM_GET_FRAMETYPE(p)          EC_GETWORD((((EC_T_BYTE*)(p))+12))
#define EC_ETHFRM_SET_FRAMETYPE(p, wVal)    EC_SETWORD((((EC_T_BYTE*)(p))+12), (wVal))
#else
#define EC_ETHFRM_GET_FRAMETYPE(p)          EC_WORDSWAP(EC_GETWORD((((EC_T_BYTE*)(p))+12)))
#define EC_ETHFRM_SET_FRAMETYPE(p, wVal)    EC_SETWORD((((EC_T_BYTE*)(p))+12), EC_WORDSWAP((wVal)))
#endif
#endif

#define EC_ETHFRM_SET_RETRYINDEX(p, byVal)  (*(((EC_T_BYTE*)(p))+(6+1))) = (EC_LOBYTE((byVal)))
#define EC_ETHFRM_GET_RETRYINDEX(p)         EC_LOBYTE((*(((EC_T_BYTE*)(p))+(6+1))))

#define EC_ETHFRM_ADD_RETRYINDEX(p, byVal)  EC_ETHFRM_SET_RETRYINDEX((p), EC_LOBYTE((EC_ETHFRM_GET_RETRYINDEX(p)+(byVal))))
#define EC_ETHFRM_SET_RETRYMACIDX(p, abyMac, byVal ) EC_ETHFRM_SET_RETRYINDEX((p), EC_LOBYTE(((abyMac)[1]+(byVal))))
            

/*---------------------------------------------------------------------------*/
typedef struct TETYPE_VLAN_HEADER
{
    EC_T_WORD   __VLanType;             /* 0   */
#if (!defined EC_NO_BITFIELDS)
  #ifdef EC_BIG_ENDIAN
    EC_T_WORD   __VLanIdL       : 8;    /* 3   */
    EC_T_WORD   __Priority      : 3;    /* 2.5 */
    EC_T_WORD   Reserved        : 1;    /* 2.4 */        
    EC_T_WORD   __VLanIdH       : 4;    /* 2.0 */        
  #else
    EC_T_WORD   __VLanIdH       : 4;    /* 2.0 */        
    EC_T_WORD   Reserved        : 1;    /* 2.4 */        
    EC_T_WORD   __Priority      : 3;    /* 2.5 */
    EC_T_WORD   __VLanIdL       : 8;    /* 3   */
  #endif
#else
    EC_T_WORD   wVLanIdLPriorityVLanIdH;
#endif /* EC_NO_BITFIELDS */
} EC_PACKED(1) ETYPE_VLAN_HEADER, *PETYPE_VLAN_HEADER;
#define ETYPE_VLAN_HEADER_LEN   sizeof(ETYPE_VLAN_HEADER)

#define EC_RESET_VLANHDR(p)                 OsMemset(p,0,sizeof(ETYPE_VLAN_HEADER))
#define EC_AL_SET_VLANHDR_IDH(p, wVal)      ((p)->__VLanIdH)  = ((wVal)&0xf)
#define EC_AL_SET_VLANHDR_IDL(p, wVal)      ((p)->__VLanIdL)  = ((wVal)&0xff)
#define EC_SET_VLANHDR_Prio(p, byVal)       ((p)->__Priority) = (byVal)
#ifdef EC_BIG_ENDIAN
#define EC_SET_VLANHDR_Type(p, wVal)        ((p)->__VLanType) = (wVal)
#else
#define EC_SET_VLANHDR_Type(p, wVal)        ((p)->__VLanType) = EC_WORDSWAP((wVal))
#endif
#define EC_SET_VLANHDR_ID(p, wVal)          EC_AL_SET_VLANHDR_IDH((p), (((wVal)>>8)&0xff)); EC_AL_SET_VLANHDR_IDL((p), (((wVal)>>0)&0xf))

/*---------------------------------------------------------------------------*/
typedef struct TETHERNET_VLAN_FRAME 
{
    ETHERNET_ADDRESS    Destination;
    ETHERNET_ADDRESS    Source;
    ETYPE_VLAN_HEADER   VLan;
    EC_T_WORD           __FrameType;              /* in host-order */
} EC_PACKED(1) ETHERNET_VLAN_FRAME, *PETHERNET_VLAN_FRAME;
#define ETHERNET_VLAN_FRAME_LEN sizeof(ETHERNET_VLAN_FRAME)

#ifdef VLAN_FRAME_SUPPORT
  #define FRAMETYPE_PTR(p)            (EC_ETHFRM_IS_VLAN((p)) ? & ((EC_T_WORD*)(p))[8] : &((EC_T_WORD*)(p))[6])
#else
  #define FRAMETYPE_PTR(p)            (&((EC_T_WORD*)(p))[6])
#endif


/*---------------------------------------------------------------------------*/
#define ETYPE_88A4_TYPE_ECAT        1           /* ECAT header follows */
#define ETYPE_88A4_TYPE_ADS         2           /* ADS header follows */
#define ETYPE_88A4_TYPE_IO          3           /* IO process image follows directly */
#define ETYPE_88A4_TYPE_NV          4           /* Network Variables */
#define ETYPE_88A4_TYPE_MAILBOX     5           /* MAILBOX Header follows */

#define ETYPE_88A4_HEADER_LEN   sizeof(EC_T_WORD)


typedef struct TETYPE_88A4_HEADER
{
    union _t_u88a4Hdr
    {
        EC_T_WORD __w88a4Hdr;
#if (!defined EC_NO_BITFIELDS)
        struct _t_sw88a4Hdr
        {
  #ifdef EC_BIG_ENDIAN
            EC_T_WORD   __E88A4HdrType      : 4;    /* 1.4 */
            EC_T_WORD   Reserved            : 1;    /* 1.3 */
            EC_T_WORD   __E88A4FrameLength  : 11;   /* 0   */
  #else
            EC_T_WORD   __E88A4FrameLength  : 11;   /* 0   */
            EC_T_WORD   Reserved            : 1;    /* 1.3 */
            EC_T_WORD   __E88A4HdrType      : 4;    /* 1.4 */
  #endif
        } EC_PACKED(1) sw88a4Hdr;
#else
        EC_T_WORD   wE88A4HdrTypeE88A4FrameLength;
#endif /* EC_NO_BITFIELDS */
    } EC_PACKED(1) u88a4Hdr;
} EC_PACKED(1) ETYPE_88A4_HEADER, *PETYPE_88A4_HEADER;

#ifdef EC_NO_BITFIELDS

#define EC_88A4HDR_RESET(p)                         ((*((EC_T_WORD*)(p))) = 0)

#define EC_88A4HDR_GET_E88A4FRAMELEN(p)             EC_GET_FRM_WORD_BITFIELD(0,11,(*((EC_T_WORD*)(p))))
#define EC_88A4HDR_SET_E88A4FRAMELEN(p, wVal)       EC_SET_FRM_WORD_BITFIELD((*((EC_T_WORD*)(p))),(wVal),0,11)

#define EC_88A4HDR_GET_E88A4HDRTYPE(p)              EC_GET_FRM_WORD_BITFIELD(12,4,(*((EC_T_WORD*)(p))))
#define EC_88A4HDR_SET_E88A4HDRTYPE(p, wVal)        EC_SET_FRM_WORD_BITFIELD((*((EC_T_WORD*)(p))),(wVal),12,4)

#define EC_AL_88A4HDR_RESET                         EC_88A4HDR_RESET            
                                                                            
#define EC_AL_88A4HDR_GET_E88A4FRAMELEN(p)          EC_88A4HDR_GET_E88A4FRAMELEN((p))      
#define EC_AL_88A4HDR_SET_E88A4FRAMELEN(p, wVal)    EC_88A4HDR_SET_E88A4FRAMELEN((p), (wVal))
                                                                                         
#define EC_AL_88A4HDR_GET_E88A4HDRTYPE(p)           EC_88A4HDR_GET_E88A4HDRTYPE((p))       
#define EC_AL_88A4HDR_SET_E88A4HDRTYPE(p, wVal)     EC_88A4HDR_SET_E88A4HDRTYPE((p), (wVal)) 

#else /* EC_NO_BITFIELDS */

#define EC_AL_88A4HDR_RESET(p)                      ((p)->u88a4Hdr.__w88a4Hdr) = 0

#define EC_AL_88A4HDR_GET_E88A4FRAMELEN(p)          EC_NTOHS(((p)->u88a4Hdr.sw88a4Hdr.__E88A4FrameLength))
#define EC_AL_88A4HDR_SET_E88A4FRAMELEN(p, wVal)    ((p)->u88a4Hdr.sw88a4Hdr.__E88A4FrameLength) = (wVal)

#define EC_AL_88A4HDR_GET_E88A4HDRTYPE(p)           ((p)->u88a4Hdr.sw88a4Hdr.__E88A4HdrType)
#define EC_AL_88A4HDR_SET_E88A4HDRTYPE(p, wVal)     ((p)->u88a4Hdr.sw88a4Hdr.__E88A4HdrType) = (wVal)

#if (defined WITHALIGNMENT)

#define EC_88A4HDR_RESET(p)                         EC_SETWORD((p), 0)
#define EC_88A4HDR_GET_E88A4HDRTYPE(p)              ((EC_GETWORD((p))>>12)&0xf)
#define EC_88A4HDR_SET_E88A4HDRTYPE(p, wVal)        EC_SETWORD((p), (EC_GETWORD((p))&(~(0xf<<12))) | ((((wVal)&0xf)<<12)))

#define EC_88A4HDR_GET_E88A4FRAMELEN(p)             (EC_GETWORD((p))&0x7ff)
#define EC_88A4HDR_SET_E88A4FRAMELEN(p, wVal)       EC_SETWORD((p), (((wVal)&0x7ff) | (ETYPE_88A4_TYPE_ECAT<<12)))

#else  /* WITHALIGNMENT */

#define EC_88A4HDR_RESET(p)                         EC_AL_88A4HDR_RESET((p))

#define EC_88A4HDR_GET_E88A4FRAMELEN(p)             EC_AL_88A4HDR_GET_E88A4FRAMELEN((p))
#define EC_88A4HDR_SET_E88A4FRAMELEN(p, wVal)       EC_AL_88A4HDR_SET_E88A4FRAMELEN((p), (wVal))

#define EC_88A4HDR_GET_E88A4HDRTYPE(p)              EC_AL_88A4HDR_GET_E88A4HDRTYPE((p))
#define EC_88A4HDR_SET_E88A4HDRTYPE(p, wVal)        EC_AL_88A4HDR_SET_E88A4HDRTYPE((p), (wVal))

#endif /* WITHALIGNMENT */
#endif /* EC_NO_BITFIELDS */


/*---------------------------------------------------------------------------*/
typedef struct TETHERNET_88A4_FRAME
{
    ETHERNET_FRAME      EthernetFrame;
    ETYPE_88A4_HEADER   E88A4Header;
} EC_PACKED(1) ETHERNET_88A4_FRAME, *PETHERNET_88A4_FRAME;
#define ETHERNET_88A4_FRAME_LEN sizeof(ETHERNET_88A4_FRAME)

#define SIZEOF_88A4_FRAME(p)    (ETHERNET_88A4_FRAME_LEN+EC_88A4HDR_GET_E88A4FRAMELEN((p)))
#define ENDOF_88A4_FRAME(p)     ((PETHERNET_88A4_FRAME)&(((EC_T_BYTE*)(p))[SIZEOF_88A4_FRAME((p))]))


/*---------------------------------------------------------------------------*/

#define EC_CMDHDR_OFFS_CMDIDX       ((EC_T_BYTE)0)
#define EC_CMDHDR_OFFS_CMDIDX_CMD   ((EC_T_BYTE)0)
#define EC_CMDHDR_OFFS_CMDIDX_IDX   ((EC_T_BYTE)1)

#define EC_CMDHDR_OFFS_ADDR         ((EC_T_BYTE)2)
#define EC_CMDHDR_OFFS_ADDR_ADP     ((EC_T_BYTE)2)
#define EC_CMDHDR_OFFS_ADDR_ADO     ((EC_T_BYTE)4)

#define EC_CMDHDR_OFFS_LEN          ((EC_T_BYTE)6)
#define EC_CMDHDR_OFFS_LEN_LEN      ((EC_T_BYTE)6)
#define EC_CMDHDR_OFFS_LEN_NEXT_BYTE ((EC_T_BYTE)7)
#define EC_CMDHDR_OFFS_IRQ          ((EC_T_BYTE)8)
                    

/* General EtherCAT telegram header */
typedef struct TETYPE_EC_CMD_HEADER
{
    union _t_uCmdIdx
    {
        EC_T_WORD __wCmdIdx;        /* 0 */
        struct _t_swCmdIdx
        {
            EC_T_BYTE   byCmd;      /* 0 */
            EC_T_BYTE   byIdx;      /* 1 */
        } EC_PACKED(1) swCmdIdx;
    } EC_PACKED(1) uCmdIdx;
    union _t_uAddr
    {
        struct _t_sladdr
        {
            EC_T_WORD   __adp;      /* 2 */
            EC_T_WORD   __ado;      /* 4 */
        } EC_PACKED(1) sladdr;
        EC_T_DWORD __laddr;         /* 2 */
    } EC_PACKED(1) uAddr;
    union _t_uLen
    {
#if (!defined EC_NO_BITFIELDS)
        struct _t_slength
        {
  #ifdef EC_BIG_ENDIAN
            EC_T_WORD   __bNext: 1; /* 6.15 */
            EC_T_WORD   res    : 4; /* 6.12 */
            EC_T_WORD   __len  : 11;/* 6.0  */
  #else
            EC_T_WORD   __len  : 11;/* 6.0  */
            EC_T_WORD   res    : 4; /* 6.12 */
            EC_T_WORD   __bNext: 1; /* 6.15 */
  #endif
        } EC_PACKED(1) slength;
#else
        EC_T_WORD   wNextLen;
#endif /* EC_NO_BITFIELDS */
        EC_T_WORD __length;         /* 6 */
    } EC_PACKED(1) uLen;
    EC_T_WORD __wIrq;               /* 8 */
} EC_PACKED(1) ETYPE_EC_CMD_HEADER, *PETYPE_EC_CMD_HEADER;
#define ETYPE_EC_CMD_HEADER_LEN     sizeof(ETYPE_EC_CMD_HEADER)
#define ETYPE_EC_WKC_LEN            sizeof(EC_T_WORD)
#define ETYPE_EC_OVERHEAD           (ETYPE_EC_CMD_HEADER_LEN+ETYPE_EC_WKC_LEN)

/* access macros */
/* Offset 0 */
#ifdef EC_BIG_ENDIAN
#define EC_AL_ICMDHDR_GET_CMDIDX(p)             EC_WORDSWAP((p)->uCmdIdx.__wCmdIdx)
#else
#define EC_AL_ICMDHDR_GET_CMDIDX(p)             ((p)->uCmdIdx.__wCmdIdx)
#endif
#define EC_AL_ICMDHDR_GET_CMDIDX_CMD(p)         ((p)->uCmdIdx.swCmdIdx.byCmd)
#define EC_AL_ICMDHDR_GET_CMDIDX_IDX(p)         ((p)->uCmdIdx.swCmdIdx.byIdx)
                                                
/* Offset 2 */
#ifdef EC_BIG_ENDIAN
#define EC_AL_ICMDHDR_GET_ADDR(p)               EC_DWORDSWAP((p)->uAddr.__laddr)
#define EC_AL_ICMDHDR_GET_ADDR_ADO(p)           EC_WORDSWAP((p)->uAddr.sladdr.__ado)
#define EC_AL_ICMDHDR_GET_ADDR_ADP(p)           EC_WORDSWAP((p)->uAddr.sladdr.__adp)

#define EC_AL_ICMDHDR_SET_ADDR(p, dwVal)        ((p)->uAddr.__laddr) = EC_DWORDSWAP((dwVal))
#define EC_AL_ICMDHDR_SET_ADDR_ADO(p, wVal)     ((p)->uAddr.sladdr.__ado) = EC_WORDSWAP((wVal))
#define EC_AL_ICMDHDR_SET_ADDR_ADP(p, wVal)     ((p)->uAddr.sladdr.__adp) = EC_WORDSWAP((wVal))
#else
#define EC_AL_ICMDHDR_GET_ADDR(p)               EC_GETDWORD(&((p)->uAddr.__laddr))
#define EC_AL_ICMDHDR_GET_ADDR_ADO(p)           ((p)->uAddr.sladdr.__ado)
#define EC_AL_ICMDHDR_GET_ADDR_ADP(p)           ((p)->uAddr.sladdr.__adp)

#define EC_AL_ICMDHDR_SET_ADDR(p, dwVal)        EC_SETDWORD(&((p)->uAddr.__laddr), dwVal)
#define EC_AL_ICMDHDR_SET_ADDR_ADO(p, wVal)     ((p)->uAddr.sladdr.__ado) = (wVal)
#define EC_AL_ICMDHDR_SET_ADDR_ADP(p, wVal)     ((p)->uAddr.sladdr.__adp) = (wVal)
#endif
       
/* Offset 6 */
#ifdef EC_BIG_ENDIAN
#define EC_AL_ICMDHDR_GET_LEN(p)                EC_WORDSWAP((p)->uLen.__length)
#else
#define EC_AL_ICMDHDR_GET_LEN(p)                ((p)->uLen.__length)
#endif
#if (!defined EC_NO_BITFIELDS)
#define EC_AL_ICMDHDR_GET_LEN_LEN(p)            ((p)->uLen.slength.__len)
#define EC_AL_CMDHDRLEN_GET_NEXT(puLen)         ((puLen)->slength.__bNext)
#define EC_AL_CMDHDRLEN_GET_LEN(puLen)          ((puLen)->slength.__len)
#define EC_AL_CMDHDRLEN_SET_LEN(puLen, wVal)    ((puLen)->slength.__len) = (wVal)
#endif

#ifdef EC_BIG_ENDIAN
#define EC_AL_CMDHDRLEN_SET_LEN_AND_NEXT(puLen, wVal, bNext) \
                                                ((puLen)->__length) = EC_WORDSWAP((EC_T_WORD)(((wVal)&0x7ff) | (((bNext)&1)<<15)))
#else
#define EC_AL_CMDHDRLEN_SET_LEN_AND_NEXT(puLen, wVal, bNext) \
                                                ((puLen)->__length) = ((EC_T_WORD)(((wVal)&0x7ff) | (((bNext)&1)<<15)))
#endif

/* Offset 8 */
#ifdef EC_BIG_ENDIAN
#define EC_AL_ICMDHDR_GET_IRQ(p)                EC_WORDSWAP((p)->__wIrq)
#define EC_AL_ICMDHDR_SET_IRQ(p, wVal)          {(p)->__wIrq = EC_WORDSWAP((wVal));}
#else
#define EC_AL_ICMDHDR_GET_IRQ(p)                ((p)->__wIrq)
#define EC_AL_ICMDHDR_SET_IRQ(p, wVal)          {(p)->__wIrq = (wVal);}
#endif

                                                
#if (defined EC_NO_BITFIELDS)
typedef EC_T_WORD TETYPE_EC_CMD_HEADER_LENGTH;

#define EC_CMDHDRLEN_GET_NEXT(puLen)        EC_GET_FRM_WORD_BITFIELD(15,1,(puLen)->__length)
#define EC_CMDHDRLEN_SET_NEXT(puLen,bNext)  EC_SET_FRM_WORD_BITFIELD((puLen)->__length,(bNext),15,1)

#define EC_CMDHDRLEN_GET_LEN(puLen)         EC_GET_FRM_WORD_BITFIELD(0,11,(puLen)->__length)
#define EC_CMDHDRLEN_SET_LEN(puLen, wVal)   EC_SET_FRM_WORD_BITFIELD((puLen)->__length,wVal,0,11)
#endif /* #ifdef EC_NO_BITFIELDS */

#if (defined EC_BIG_ENDIAN)

#define EC_CMDHDRLEN_SET_LEN_AND_NEXT(puLen, wVal, bNext)   \
    EC_CMDHDRLEN_SET_LEN((puLen), (wVal))                   \
    EC_CMDHDRLEN_SET_NEXT((puLen),(bNext))
#define EC_AL_ICMDHDR_GET_LEN_LEN               EC_ICMDHDR_GET_LEN_LEN
#define EC_AL_CMDHDRLEN_GET_NEXT(puLen)         EC_CMDHDRLEN_GET_NEXT((puLen))
#define EC_AL_CMDHDRLEN_GET_LEN(puLen)          EC_CMDHDRLEN_GET_LEN((puLen))
#define EC_AL_CMDHDRLEN_SET_LEN(puLen, wVal)    EC_CMDHDRLEN_SET_LEN((puLen),(wVal))

static EC_INLINESTART EC_T_WORD EC_ICMDHDR_GET_CMDIDX(PETYPE_EC_CMD_HEADER p)
{
    return EC_GET_FRM_WORD(
        (((EC_T_BYTE*)p) + EC_CMDHDR_OFFS_CMDIDX)
        );
} EC_INLINESTOP
#define EC_ICMDHDR_GET_CMDIDX_CMD(p)        EC_AL_ICMDHDR_GET_CMDIDX_CMD((p))
#define EC_ICMDHDR_GET_CMDIDX_IDX(p)        EC_AL_ICMDHDR_GET_CMDIDX_IDX((p))

static EC_INLINESTART EC_T_DWORD EC_ICMDHDR_GET_ADDR(PETYPE_EC_CMD_HEADER p)
{
    return EC_GET_FRM_DWORD(
        (((EC_T_BYTE*)p) + EC_CMDHDR_OFFS_ADDR)
        );
} EC_INLINESTOP
static EC_INLINESTART EC_T_WORD EC_ICMDHDR_GET_ADDR_ADO(PETYPE_EC_CMD_HEADER p)
{
    return EC_GET_FRM_WORD(
        (((EC_T_BYTE*)p) + EC_CMDHDR_OFFS_ADDR_ADO)
        );
} EC_INLINESTOP
static EC_INLINESTART EC_T_WORD EC_ICMDHDR_GET_ADDR_ADP(PETYPE_EC_CMD_HEADER p)
{
    return EC_GET_FRM_WORD(
        (((EC_T_BYTE*)p) + EC_CMDHDR_OFFS_ADDR_ADP)
        );
} EC_INLINESTOP
static EC_INLINESTART EC_T_WORD EC_ICMDHDR_GET_LEN(PETYPE_EC_CMD_HEADER p)
{
    return EC_GET_FRM_WORD(
        (((EC_T_BYTE*)p) + EC_CMDHDR_OFFS_LEN)
        );
} EC_INLINESTOP
static EC_INLINESTART EC_T_WORD EC_ICMDHDR_GET_LEN_LEN(PETYPE_EC_CMD_HEADER p)
{
    return (EC_T_WORD)(
        EC_GET_FRM_WORD(
        (((EC_T_BYTE*)p) + EC_CMDHDR_OFFS_LEN_LEN)
        )&((1<<11)-1));
} EC_INLINESTOP
static EC_INLINESTART EC_T_WORD EC_ICMDHDR_GET_IRQ(PETYPE_EC_CMD_HEADER p)
{
    return EC_GET_FRM_WORD(
        (((EC_T_BYTE*)p) + EC_CMDHDR_OFFS_IRQ)
        );
} EC_INLINESTOP
static EC_INLINESTART EC_T_VOID EC_ICMDHDR_SET_IRQ(PETYPE_EC_CMD_HEADER p, EC_T_WORD wVal)
{
    EC_SET_FRM_WORD(
        (((EC_T_BYTE*)p) + EC_CMDHDR_OFFS_IRQ),
        wVal
        );
} EC_INLINESTOP
static EC_INLINESTART EC_T_VOID EC_ICMDHDR_SET_ADDR(PETYPE_EC_CMD_HEADER p, EC_T_DWORD dwVal)
{
    EC_SET_FRM_DWORD(
        (((EC_T_BYTE*)p) + EC_CMDHDR_OFFS_ADDR),
        dwVal
        );
} EC_INLINESTOP
static EC_INLINESTART EC_T_VOID EC_ICMDHDR_SET_ADDR_ADO(PETYPE_EC_CMD_HEADER p, EC_T_WORD wVal)
{
    EC_SET_FRM_WORD(
        (((EC_T_BYTE*)p) + EC_CMDHDR_OFFS_ADDR_ADO), 
        wVal
        );
} EC_INLINESTOP
static EC_INLINESTART EC_T_VOID EC_ICMDHDR_SET_ADDR_ADP(PETYPE_EC_CMD_HEADER p, EC_T_WORD wVal)
{
    EC_SET_FRM_WORD(
        (((EC_T_BYTE*)p) + EC_CMDHDR_OFFS_ADDR_ADP), 
        wVal
        );
} EC_INLINESTOP

#elif (defined WITHALIGNMENT) /* !EC_BIG_ENDIAN */
  
/* Offset 0 */
#define EC_ICMDHDR_GET_CMDIDX(p)            (EC_GETWORD((p)))
#define EC_ICMDHDR_GET_CMDIDX_CMD(p)        (((EC_T_BYTE*)(p))[0])
#define EC_ICMDHDR_GET_CMDIDX_IDX(p)        (((EC_T_BYTE*)(p))[1])

/* Offset 2 */
#define EC_ICMDHDR_GET_ADDR(p)              EC_GETDWORD( (((EC_T_BYTE*)(p))+ EC_CMDHDR_OFFS_ADDR) )
#define EC_ICMDHDR_GET_ADDR_ADO(p)          EC_GETWORD( (((EC_T_BYTE*)(p))+ EC_CMDHDR_OFFS_ADDR_ADO) )
#define EC_ICMDHDR_GET_ADDR_ADP(p)          EC_GETWORD( (((EC_T_BYTE*)(p))+ EC_CMDHDR_OFFS_ADDR_ADP) )

#define EC_ICMDHDR_SET_ADDR(p, dwVal)       EC_SETDWORD( (((EC_T_BYTE*)(p))+ EC_CMDHDR_OFFS_ADDR), (dwVal))
#define EC_ICMDHDR_SET_ADDR_ADO(p, wVal)    EC_SETWORD( (((EC_T_BYTE*)(p))+ EC_CMDHDR_OFFS_ADDR_ADO), (wVal))
#define EC_ICMDHDR_SET_ADDR_ADP(p, wVal)    EC_SETWORD( (((EC_T_BYTE*)(p))+ EC_CMDHDR_OFFS_ADDR_ADP), (wVal))
                                            
/* Offset 6 */
#define EC_ICMDHDR_GET_LEN(p)               ((EC_T_WORD)(EC_GETWORD( (((EC_T_BYTE*)(p)) + EC_CMDHDR_OFFS_LEN_LEN))))
#define EC_ICMDHDR_GET_LEN_LEN(p)           ((EC_T_WORD)(EC_GETWORD( (((EC_T_BYTE*)(p))) + EC_CMDHDR_OFFS_LEN_LEN )&0x7ff))

#define EC_CMDHDRLEN_GET_NEXT(puLen)        ((EC_T_WORD)((EC_GETWORD((((EC_T_BYTE*)(puLen))))>>15)&0x1))

#define EC_CMDHDRLEN_GET_LEN(puLen)         ((EC_T_WORD)((EC_GETWORD((((EC_T_BYTE*)(puLen)))))&0x7ff))

#define EC_CMDHDRLEN_SET_LEN_AND_NEXT(puLen, wVal, bNext)   EC_SETWORD((puLen), ((EC_T_WORD)(((wVal)&0x7ff) | (((bNext)&1)<<15))) )


/* Offset 8 */
#define EC_ICMDHDR_GET_IRQ(p)               EC_GETWORD((((EC_T_BYTE*)(p)) + EC_CMDHDR_OFFS_IRQ))
#define EC_ICMDHDR_SET_IRQ(p, wVal)         EC_SETWORD((((EC_T_BYTE*)(p)) + EC_CMDHDR_OFFS_IRQ), (wVal))

#else /* !EC_BIG_ENDIAN && !WITHALIGMENT */

/* Offset 0 */
#define EC_ICMDHDR_GET_CMDIDX(p)            EC_AL_ICMDHDR_GET_CMDIDX((p))
#define EC_ICMDHDR_GET_CMDIDX_CMD(p)        EC_AL_ICMDHDR_GET_CMDIDX_CMD((p))
#define EC_ICMDHDR_GET_CMDIDX_IDX(p)        EC_AL_ICMDHDR_GET_CMDIDX_IDX((p))
   
/* Offset 2 */
#define EC_ICMDHDR_GET_ADDR(p)              EC_AL_ICMDHDR_GET_ADDR((p))
#define EC_ICMDHDR_GET_ADDR_ADO(p)          EC_AL_ICMDHDR_GET_ADDR_ADO((p))
#define EC_ICMDHDR_GET_ADDR_ADP(p)          EC_AL_ICMDHDR_GET_ADDR_ADP((p))

#define EC_ICMDHDR_SET_ADDR(p, dwVal)       EC_AL_ICMDHDR_SET_ADDR((p), (dwVal))
#define EC_ICMDHDR_SET_ADDR_ADO(p, wVal)    EC_AL_ICMDHDR_SET_ADDR_ADO((p), (wVal))
#define EC_ICMDHDR_SET_ADDR_ADP(p, wVal)    EC_AL_ICMDHDR_SET_ADDR_ADP((p), (wVal))
                                         
/* Offset 6 */
#define EC_ICMDHDR_GET_LEN(p)               EC_AL_ICMDHDR_GET_LEN((p))
#define EC_ICMDHDR_GET_LEN_LEN(p)           EC_AL_ICMDHDR_GET_LEN_LEN((p))

#ifndef EC_CMDHDRLEN_GET_NEXT
#define EC_CMDHDRLEN_GET_NEXT(puLen)        EC_AL_CMDHDRLEN_GET_NEXT((puLen))
#endif

#ifndef EC_CMDHDRLEN_GET_LEN
#define EC_CMDHDRLEN_GET_LEN(puLen)         EC_AL_CMDHDRLEN_GET_LEN((puLen))
#endif
#ifndef EC_CMDHDRLEN_SET_LEN
#define EC_CMDHDRLEN_SET_LEN(puLen, wVal)   EC_AL_CMDHDRLEN_SET_LEN((puLen), (wVal))
#endif

#define EC_CMDHDRLEN_SET_LEN_AND_NEXT(puLen, wVal, bNext)   EC_AL_CMDHDRLEN_SET_LEN_AND_NEXT((puLen), (wVal), (bNext))

/* Offset 8 */
#define EC_ICMDHDR_GET_IRQ(p)               EC_AL_ICMDHDR_GET_IRQ((p))
#define EC_ICMDHDR_SET_IRQ(p, wVal)         EC_AL_ICMDHDR_SET_IRQ((p), (wVal))

#endif /* !EC_BIG_ENDIAN && !WITHALIGMENT */



static EC_INLINESTART EC_T_VOID EC_ICMDHDR_SET_LEN(PETYPE_EC_CMD_HEADER p, EC_T_WORD wVal)
{
    EC_SET_FRM_WORD(
        (((EC_T_BYTE*)p) + EC_CMDHDR_OFFS_LEN), 
        wVal
              );
} EC_INLINESTOP

static EC_INLINESTART EC_T_VOID EC_ICMDHDR_SET_LEN_LEN(PETYPE_EC_CMD_HEADER p, EC_T_WORD wVal)
{
EC_T_WORD wOldVal;
EC_T_WORD wMask;
    wOldVal = EC_ICMDHDR_GET_LEN((p));          /* get old value (as the len is shared with other bits) */
    wMask = (EC_T_WORD)(~((1<<11)-1));
    wOldVal &= wMask;                           /* eliminate old len value (only relevant bits) */
    wVal = (EC_T_WORD)(wVal & ((1<<11)-1));     /* mask irrelevant bits */
    wVal = (EC_T_WORD)(wOldVal | wVal);         /* determine new length together with other bits untouched */
    EC_ICMDHDR_SET_LEN((p),wVal);               /* store it */
} EC_INLINESTOP
static EC_INLINESTART EC_T_VOID EC_ICMDHDR_SET_LEN_NEXT(PETYPE_EC_CMD_HEADER p, EC_T_BOOL bVal)
{
    if (bVal)
    {
        (((EC_T_BYTE*)p) + EC_CMDHDR_OFFS_LEN_NEXT_BYTE)[0] |=((EC_T_BYTE)0x80);
    }
    else
    {
        (((EC_T_BYTE*)p) + EC_CMDHDR_OFFS_LEN_NEXT_BYTE)[0] &=((EC_T_BYTE)~0x80);
    }
} EC_INLINESTOP

#if (defined EC_AL_ICMDHDR_GET_LEN_LEN)
static EC_INLINESTART EC_T_WORD ETYPE_EC_CMD_GETLEN(PETYPE_EC_CMD_HEADER p)
{
    return (EC_T_WORD)(ETYPE_EC_OVERHEAD + EC_ICMDHDR_GET_LEN_LEN(p));
} EC_INLINESTOP
static EC_INLINESTART EC_T_VOID ETYPE_EC_CMD_SETWKC(PETYPE_EC_CMD_HEADER p, EC_T_WORD wVal)
{
EC_T_BYTE* pbyCur = EC_NULL;

    pbyCur = &(((EC_T_BYTE*)p)[ETYPE_EC_CMD_HEADER_LEN + EC_ICMDHDR_GET_LEN_LEN(p)]);

    EC_SET_FRM_WORD(pbyCur, wVal);
} EC_INLINESTOP
static EC_INLINESTART EC_T_WORD ETYPE_EC_CMD_GETWKC(PETYPE_EC_CMD_HEADER p)
{
EC_T_BYTE* pbyCur = EC_NULL;

    /*pbyCur = &(((EC_T_BYTE*)p)[ETYPE_EC_CMD_HEADER_LEN + p->uLen.slength.__len]);*/
    pbyCur = &(((EC_T_BYTE*)p)[ETYPE_EC_CMD_HEADER_LEN + EC_ICMDHDR_GET_LEN_LEN( p )]);

    return EC_GET_FRM_WORD(pbyCur);
} EC_INLINESTOP
static EC_INLINESTART EC_T_WORD ETYPE_EC_CMD_GETWKCOFF(PETYPE_EC_CMD_HEADER p)
{
    return (EC_T_WORD)(ETYPE_EC_CMD_HEADER_LEN + EC_ICMDHDR_GET_LEN_LEN(p));
} EC_INLINESTOP
static EC_INLINESTART PETYPE_EC_CMD_HEADER NEXT_EcCmdHeader(PETYPE_EC_CMD_HEADER p)
{
PETYPE_EC_CMD_HEADER pRetVal = 0;

    pRetVal = (PETYPE_EC_CMD_HEADER)(&((EC_T_BYTE*)p)[EC_ICMDHDR_GET_LEN_LEN(p) + ETYPE_EC_OVERHEAD]);

    return pRetVal;
} EC_INLINESTOP
#endif /* EC_AL_ICMDHDR_GET_LEN_LEN */

/* FMMU configuration command */
typedef struct _ETYPE_EC_T_FMMU_CFG_CMD
{
    EC_T_DWORD  dwLogStartAddr;     /* 0x00 */
    EC_T_WORD   wLength;            /* 0x04 */
    EC_T_BYTE   byStartBit;         /* 0x06 */
    EC_T_BYTE   byStopBit;          /* 0x07 */
    EC_T_WORD   wPhysStart;         /* 0x08 */
    EC_T_BYTE   byPhysStartBit;     /* 0x0A */
    EC_T_BYTE   byType;             /* 0x0B */
    EC_T_BYTE   byActivate;         /* 0x0C */
    EC_T_BYTE   byReserved;         /* 0x0D */
    EC_T_WORD   wReserved;          /* 0x0E */
} EC_PACKED(1) ETYPE_EC_T_FMMU_CFG_CMD, *PETYPE_EC_T_FMMU_CFG_CMD;

static EC_INLINESTART EC_T_DWORD EC_ECFMMUCFGCMD_GETLOGSTARTADDR(PETYPE_EC_T_FMMU_CFG_CMD p)
{
    return EC_GET_FRM_DWORD(&(p->dwLogStartAddr));
} EC_INLINESTOP

static EC_INLINESTART EC_T_WORD EC_ECFMMUCFGCMD_GETLENGTH(PETYPE_EC_T_FMMU_CFG_CMD p)
{
    return EC_GET_FRM_WORD(&(p->wLength));
} EC_INLINESTOP

#define FMMU_CFG_CMD_TYPE_READ  ((EC_T_BYTE)0x01)
#define FMMU_CFG_CMD_TYPE_WRITE ((EC_T_BYTE)0x02)

#define ETYPE_EC_T_FMMU_CFG_CMD_SIZE sizeof(ETYPE_EC_T_FMMU_CFG_CMD)

#define MAX_EC_CMD_PER_FRAME  124   /* ((ETHERNET_MAX_FRAME_LEN - 0x10)/ETYPE_EC_OVERHEAD) */

#define MAX_EC_DATA_LEN (ETHERNET_MAX_FRAME_LEN - ETHERNET_88A4_FRAME_LEN - ETYPE_EC_OVERHEAD)

/*---------------------------------------------------------------------------*/
typedef struct TETHERNET_88A4_HEADER
{
    ETYPE_88A4_HEADER E88A4Header;
    struct _t_sETECAT_88a4Hdr
    {   /* ETYPE_88A4_TYPE_ECAT */
        ETYPE_EC_CMD_HEADER FirstEcCmdHeader;
    } EC_PACKED(1)
    sETECAT;
} EC_PACKED(1) ETHERNET_88A4_HEADER, *PETHERNET_88A4_HEADER;

/*---------------------------------------------------------------------------*/
typedef struct TETHERNET_88A4_MAX_FRAME
{
    ETHERNET_FRAME    EthernetFrame;
    ETYPE_88A4_HEADER E88A4Header;
    struct _t_sETECAT_maxFrame
    {   /* ETYPE_88A4_TYPE_ECAT */
        ETYPE_EC_CMD_HEADER FirstEcCmdHeader;
    } EC_PACKED(1)
    sETECAT;
    EC_T_BYTE   abyData[MAX_EC_DATA_LEN];
    EC_T_WORD   __wWkc;
} EC_PACKED(1) ETHERNET_88A4_MAX_FRAME, *PETHERNET_88A4_MAX_FRAME;

typedef struct TETHERNET_88A4_MAX_VLAN_FRAME
{
    ETHERNET_VLAN_FRAME    EthernetFrame;
    ETYPE_88A4_HEADER E88A4Header;
    struct _t_sETECAT_maxVlanFrame
    {   /* ETYPE_88A4_TYPE_ECAT */
        ETYPE_EC_CMD_HEADER FirstEcCmdHeader;
    } EC_PACKED(1)
        sETECAT;
    EC_T_BYTE   abyData[MAX_EC_DATA_LEN-3];
    EC_T_WORD   __wWkc;
} EC_PACKED(1) ETHERNET_88A4_MAX_VLAN_FRAME, *PETHERNET_88A4_MAX_VLAN_FRAME;

/*---------------------------------------------------------------------------*/

typedef struct TEcCmdDesc
{
    ETYPE_EC_CMD_HEADER     EcCmdHeader;
    EC_T_WORD               reserved1;
    EC_T_BOOL               bChkCntRecv;

    EC_T_WORD               reserved2;
    EC_T_WORD               cmdSize;
    EC_T_WORD               imageOffs[2]; /* in bytes */
    EC_T_WORD               imageSize[2]; /* in bytes */
#define EC_ECCMDDESC_PARMCOPYINPUTS         0x0001
#define EC_ECCMDDESC_PARMCOPYOUTPUTS        0x0002
#define EC_ECCMDDESC_PARMLOGMBOXSTATE       0x0004
    EC_T_WORD               cpParm;
    EC_T_WORD               cntRecv;
    EC_T_BYTE               byConfOpStatesMask;         /* configuration operational state (../config/cyclic/frame/cmd/State) */
    EC_T_BYTE               reserved3[3];
} EC_PACKED(1) EcCmdDesc, *PEcCmdDesc;

#define ECCMDDESC_LEN    sizeof(EcCmdDesc)


/*---------------------------------------------------------------------------*/
/* Copy information for Slave-to-Slave communication                         */
/*---------------------------------------------------------------------------*/
/* The master has to copy valid input data of this command from the source 
   offest (bit offs in the complete process image) to a destination offset. */
typedef struct
{
    EC_T_WORD  wSrcBitOffs; 
    EC_T_WORD  wDstBitOffs;
    EC_T_WORD  wBitSize;
    EC_T_WORD  wTaskId;
} EC_PACKED(1) EC_T_CYC_COPY_INFO;

/*---------------------------------------------------------------------------*/
/* master properties */
/*---------------------------------------------------------------------------*/
#define EC_MASTER_PROP_MAX_NAME_LENGTH      80
#define EC_MASTER_PROP_MAX_VALUE_LENGTH     80

typedef struct _EC_T_MASTER_PROP_DESC
{
    EC_T_CHAR       szNameString[EC_MASTER_PROP_MAX_NAME_LENGTH];     /* name  */
    EC_T_CHAR       szValueString[EC_MASTER_PROP_MAX_VALUE_LENGTH];   /* value */
    EC_T_DWORD      dwRes1;
    EC_T_DWORD      dwRes2;
} EC_PACKED(1) EC_T_MASTER_PROP_DESC;


/*---------------------------------------------------------------------------*/
#define ECAT_INITCMD_I_P            0x0001  
#define ECAT_INITCMD_P_S            0x0002
#define ECAT_INITCMD_P_I            0x0004
#define ECAT_INITCMD_S_P            0x0008  
#define ECAT_INITCMD_S_O            0x0010
#define ECAT_INITCMD_S_I            0x0020  
#define ECAT_INITCMD_O_S            0x0040
#define ECAT_INITCMD_O_P            0x0080  
#define ECAT_INITCMD_O_I            0x0100  
#define ECAT_INITCMD_I_B            0x0200
#define ECAT_INITCMD_B_I            0x0400
#define ECAT_INITCMD_I_I            0x0800
#define ECAT_INITCMD_P_P            0x1000
/*
 * The following definitions are combinations of the ECAT_INITCMD_... above.
 * E.g. ECAT_INITCMD_IP_PS == ECAT_INITCMD_I_P | ECAT_INITCMD_P_S
 */
#define ECAT_INITCMD_IP_PS          0x0003
#define ECAT_INITCMD_BACKTO_P       0x0088
#define ECAT_INITCMD_SI_OI          0x0120
#define ECAT_INITCMD_OSP_I          0x0124
#define ECAT_INITCMD_OSP_I__I_P     0x0125
#define ECAT_INITCMD_SP_SI_OP_OI    0x01A8
#define ECAT_INITCMD_IP_SP_SI_OP_OI 0x01A9
#define ECAT_INITCMD_I_PB           0x0201
#define ECAT_INITCMD_OSP_I__I_BP    0x0325
#define ECAT_INITCMD_BI_IP          0x0401
#define ECAT_INITCMD_BACKTO_I       0x0524
#define ECAT_INITCMD_IP_PI_BI_SI_OI 0x0525
#define ECAT_INITCMD_BEFORE         0x8000

#define ECAT_INITCMD_ADR_ERROR      0x7FFE
#define ECAT_INITCMD_FAILURE        0x7FFF

#define EC_ECINITCMDDESC_OFFS_TRANSITION        ((EC_T_BYTE)ETYPE_EC_CMD_HEADER_LEN)
#define EC_ECINITCMDDESC_OFFS_CNT               ((EC_T_BYTE)ETYPE_EC_CMD_HEADER_LEN + 2)
#define EC_ECINITCMDDESC_OFFS_CMTLEN            ((EC_T_BYTE)ETYPE_EC_CMD_HEADER_LEN + 4)
#define EC_ECINITCMDDESC_OFFS_INITCMDTIMEOUT    ((EC_T_BYTE)ETYPE_EC_CMD_HEADER_LEN + 7)
#define EC_ECINITCMDDESC_OFFS_RETRIES           ((EC_T_BYTE)ETYPE_EC_CMD_HEADER_LEN + 9)

typedef struct TEcInitCmdDesc
{
    ETYPE_EC_CMD_HEADER         EcCmdHeader;                /* 0 */
    EC_T_WORD                   __transition;               /* ETYPE_EC_CMD_HEADER_LEN + 0 = 10 */

    EC_T_WORD                   __cnt;                      /* ETYPE_EC_CMD_HEADER_LEN + 2 */
    EC_T_WORD                   __cmtLen;                   /* ETYPE_EC_CMD_HEADER_LEN + 4 */
                                                            /* (excl. \0) */
    EC_T_BYTE                   byFlags;                    /* ETYPE_EC_CMD_HEADER_LEN + 6:  */
/*
    EC_T_BYTE                   __newCycle          : 1;    / * ETYPE_EC_CMD_HEADER_LEN + 6.0 * /
    
    EC_T_BYTE                   __newFrame          : 1;    / * ETYPE_EC_CMD_HEADER_LEN + 6.1 * /
    EC_T_BYTE                   __validate          : 1;    / * ETYPE_EC_CMD_HEADER_LEN + 6.2 * /
    
    EC_T_BYTE                   __validateMask      : 1;    / * ETYPE_EC_CMD_HEADER_LEN + 6.3 * /
    EC_T_BYTE                   __masterInitCmd     : 1;    / * ETYPE_EC_CMD_HEADER_LEN + 6.4 * /
    EC_T_BYTE                   reserved            : 3;    / * ETYPE_EC_CMD_HEADER_LEN + 6.5 * /
    */

    EC_T_WORD                   __wInitCmdTimeout;          /* ETYPE_EC_CMD_HEADER_LEN + 7 */
                                                            /* in ms */
    EC_T_WORD                   __retries;                  /* ETYPE_EC_CMD_HEADER_LEN + 9 */
    EC_T_BYTE                   reserved2;                  /* ETYPE_EC_CMD_HEADER_LEN + 11 */
} EC_PACKED(1) EcInitCmdDesc, *PEcInitCmdDesc;

static EC_INLINESTART EC_T_WORD EC_ECINITCMDDESC_GET_TRANSITION(PEcInitCmdDesc p)
{
    return EC_GETWORD(
        (((EC_T_PBYTE)p) + EC_ECINITCMDDESC_OFFS_TRANSITION)
                     );
} EC_INLINESTOP

static EC_INLINESTART EC_T_WORD EC_ECINITCMDDESC_GET_CNT(PEcInitCmdDesc p)
{
    return EC_GETWORD(
        (((EC_T_PBYTE)p) + EC_ECINITCMDDESC_OFFS_CNT)
                     );
} EC_INLINESTOP

static EC_INLINESTART EC_T_WORD EC_ECINITCMDDESC_GET_CMTLEN(PEcInitCmdDesc p)
{
    return EC_GETWORD(
        (((EC_T_PBYTE)p) + EC_ECINITCMDDESC_OFFS_CMTLEN)
                     );
} EC_INLINESTOP

static EC_INLINESTART EC_T_BOOL EC_ECINITCMDDESC_GET_VALIDATE(PEcInitCmdDesc p)
{
    return (0 != (p->byFlags & (1<<2)));
} EC_INLINESTOP

static EC_INLINESTART EC_T_BOOL EC_ECINITCMDDESC_GET_VALIDATEMASK(PEcInitCmdDesc p)
{
    return (0 != (p->byFlags & (1<<3)));
} EC_INLINESTOP
static EC_INLINESTART EC_T_BOOL EC_ECINITCMDDESC_GET_MASTERINITCMD(PEcInitCmdDesc p)
{
    return (0 != (p->byFlags & (1 << 4)));
} EC_INLINESTOP

static EC_INLINESTART EC_T_WORD EC_ECINITCMDDESC_GET_INITCMDTIMEOUT(PEcInitCmdDesc p)
{
    return EC_GETWORD((((EC_T_PBYTE)p) + EC_ECINITCMDDESC_OFFS_INITCMDTIMEOUT));
} EC_INLINESTOP

static EC_INLINESTART EC_T_WORD EC_ECINITCMDDESC_GET_RETRIES(PEcInitCmdDesc p)
{
    return EC_GETWORD((((EC_T_PBYTE)p) + EC_ECINITCMDDESC_OFFS_RETRIES));
} EC_INLINESTOP

static EC_INLINESTART EC_T_VOID EC_ECINITCMDDESC_SET_INITCMDTIMEOUT(PEcInitCmdDesc p, EC_T_WORD wVal)
{
    EC_SETWORD((((EC_T_PBYTE)p) + EC_ECINITCMDDESC_OFFS_INITCMDTIMEOUT), wVal);
} EC_INLINESTOP

static EC_INLINESTART EC_T_VOID EC_ECINITCMDDESC_SET_CMTLEN(PEcInitCmdDesc p, EC_T_WORD wVal)
{
    EC_SETWORD((((EC_T_PBYTE)p) + EC_ECINITCMDDESC_OFFS_CMTLEN), wVal);
} EC_INLINESTOP

static EC_INLINESTART EC_T_VOID EC_ECINITCMDDESC_SET_TRANSITION(PEcInitCmdDesc p, EC_T_WORD wVal)
{
    EC_SETWORD((((EC_T_PBYTE)p) + EC_ECINITCMDDESC_OFFS_TRANSITION), wVal);
} EC_INLINESTOP

static EC_INLINESTART EC_T_VOID EC_ECINITCMDDESC_SET_RETRIES(PEcInitCmdDesc p, EC_T_WORD wVal)
{
    EC_SETWORD((((EC_T_PBYTE)p) + EC_ECINITCMDDESC_OFFS_RETRIES), wVal);
} EC_INLINESTOP

/* static EC_INLINESTART EC_T_VOID EC_ECINITCMDDESC_SET_NEWCYCLE(PEcInitCmdDesc p, EC_T_WORD wVal)
{
    if (wVal)
    {
        p->byFlags |= (1<<0);
    }
    else
    {
        
        p->byFlags &= ~(1<<0);
    }
} EC_INLINESTOP */

/* static EC_INLINESTART EC_T_VOID EC_ECINITCMDDESC_SET_NEWFRAME(PEcInitCmdDesc p, EC_T_WORD wVal)
{
    if (wVal)
    {
        p->byFlags |= (1<<1);
    }
    else
    {
        
        p->byFlags &= ~(1<<1);
    }
} EC_INLINESTOP */

static EC_INLINESTART EC_T_VOID EC_ECINITCMDDESC_SET_VALIDATE(PEcInitCmdDesc p, EC_T_WORD wVal)
{
    if (wVal)
    {
        p->byFlags |= (1<<2);
    }
    else
    {
        p->byFlags &= (EC_T_BYTE)(~(1<<2));
    }
} EC_INLINESTOP

static EC_INLINESTART EC_T_VOID EC_ECINITCMDDESC_SET_VALIDATEMASK(PEcInitCmdDesc p, EC_T_WORD wVal)
{
    if (wVal)
    {
        p->byFlags |= (1<<3);
    }
    else
    {
        p->byFlags &= (EC_T_BYTE)(~(1<<3));
    }
} EC_INLINESTOP

static EC_INLINESTART EC_T_VOID EC_ECINITCMDDESC_SET_MASTERINITCMD(PEcInitCmdDesc p, EC_T_BOOL bVal)
{
    if (bVal)
    {
        p->byFlags |= (1 << 4);
    }
    else
    {
        p->byFlags &= (EC_T_BYTE)(~(1 << 4));
    }
} EC_INLINESTOP

static EC_INLINESTART EC_T_VOID EC_ECINITCMDDESC_SET_CNT(PEcInitCmdDesc p, EC_T_WORD wVal)
{
    EC_SETWORD((((EC_T_PBYTE)p) + EC_ECINITCMDDESC_OFFS_CNT), wVal);
} EC_INLINESTOP

#define SIZEOF_EcInitCmdDesc(p) (sizeof(EcInitCmdDesc) + EC_ICMDHDR_GET_LEN_LEN(&(((PEcInitCmdDesc)(p))->EcCmdHeader)) + \
    (EC_ECINITCMDDESC_GET_VALIDATE(((PEcInitCmdDesc)(p)))     ? EC_ICMDHDR_GET_LEN_LEN(&(((PEcInitCmdDesc)(p))->EcCmdHeader)) : 0) + \
    (EC_ECINITCMDDESC_GET_VALIDATEMASK(((PEcInitCmdDesc)(p))) ? EC_ICMDHDR_GET_LEN_LEN(&(((PEcInitCmdDesc)(p))->EcCmdHeader)) : 0) + \
    (EC_ECINITCMDDESC_GET_CMTLEN(((PEcInitCmdDesc)(p)))       ? EC_ECINITCMDDESC_GET_CMTLEN(((PEcInitCmdDesc)(p))) + 1 : 0))
#define NEXT_EcInitCmdDesc(p)   (PEcInitCmdDesc)&(((EC_T_BYTE*)(p))[(sizeof(EcInitCmdDesc) + EC_ICMDHDR_GET_LEN_LEN(&(((PEcInitCmdDesc)(p))->EcCmdHeader)) + \
    (EC_ECINITCMDDESC_GET_VALIDATE(((PEcInitCmdDesc)(p))) ? EC_ICMDHDR_GET_LEN_LEN(&(((PEcInitCmdDesc)(p))->EcCmdHeader)) : 0) + \
    (EC_ECINITCMDDESC_GET_VALIDATEMASK(((PEcInitCmdDesc)(p))) ? EC_ICMDHDR_GET_LEN_LEN(&(((PEcInitCmdDesc)(p))->EcCmdHeader)) : 0) + \
    (EC_ECINITCMDDESC_GET_CMTLEN(((PEcInitCmdDesc)(p)))       ? EC_ECINITCMDDESC_GET_CMTLEN(((PEcInitCmdDesc)(p))) + 1 : 0))])
#define EcInitCmdDescData(p)    (&(((EC_T_BYTE*)(p))[sizeof(EcInitCmdDesc)]))
#define EcInitCmdDescVData(p)   (&(((EC_T_BYTE*)(p))[sizeof(EcInitCmdDesc) + EC_ICMDHDR_GET_LEN_LEN(&(((PEcInitCmdDesc)(p))->EcCmdHeader))]))
#define EcInitCmdDescVMData(p)  (&(((EC_T_BYTE*)(p))[sizeof(EcInitCmdDesc) + 2*EC_ICMDHDR_GET_LEN_LEN(&(((PEcInitCmdDesc)(p))->EcCmdHeader))]))
#define EcInitCmdDescComment(p) ((EC_T_CHAR*)&(((EC_T_BYTE*)(p))[sizeof(EcInitCmdDesc) + EC_ICMDHDR_GET_LEN_LEN(&(((PEcInitCmdDesc)(p))->EcCmdHeader)) + \
    (EC_ECINITCMDDESC_GET_VALIDATE(((PEcInitCmdDesc)(p))) ? EC_ICMDHDR_GET_LEN_LEN(&(((PEcInitCmdDesc)(p))->EcCmdHeader)) : 0) + \
    (EC_ECINITCMDDESC_GET_VALIDATEMASK(((PEcInitCmdDesc)(p))) ? EC_ICMDHDR_GET_LEN_LEN(&(((PEcInitCmdDesc)(p))->EcCmdHeader)) : 0)]))

/*---------------------------------------------------------------------------*/
#define ECCANOPENCMDDESC_TYPE_AUTO          0
#define ECCANOPENCMDDESC_TYPE_DD            1
#define ECCANOPENCMDDESC_TYPE_USER          2

/*---------------------------------------------------------------------------*/
/* Mailbox */
extern EC_T_CHAR* g_cStrMbxTypeText[];

#define ETHERCAT_MBOX_TYPE_ERROR        ((EC_T_BYTE)0)      /* mailbox error response */
#define ETHERCAT_MBOX_TYPE_ADS          ((EC_T_BYTE)1)      /* AMS/ADS header follows */
#define ETHERCAT_MBOX_TYPE_ETHERNET     ((EC_T_BYTE)2)      /* ETHERCAT_ETHERNET_HEADER follows */
#define ETHERCAT_MBOX_TYPE_CANOPEN      ((EC_T_BYTE)3)      /* ETHERCAT_CANOPEN_HEADER follows */
#define ETHERCAT_MBOX_TYPE_FILEACCESS   ((EC_T_BYTE)4)      /* EC_FOE_HDR follows */
#define ETHERCAT_MBOX_TYPE_SOE          ((EC_T_BYTE)5)      /* EC_SOE_HDR follows */
#define ETHERCAT_MBOX_TYPE_VOE          ((EC_T_BYTE)15)     /* ETHERCAT_VOE_HEADER follows */

#define EC_ECMBOXHDR_OFFS_LENGTH        ((EC_T_BYTE)0)
#define EC_ECMBOXHDR_OFFS_ADDRESS       ((EC_T_BYTE)2)
#define EC_ECMBOXHDR_OFFS_CHANPRIO      ((EC_T_BYTE)4)
#define EC_ECMBOXHDR_OFFS_TYPCRT        ((EC_T_BYTE)5)

#define ETHERCAT_MBOX_HEADER_LEN    sizeof(ETHERCAT_MBOX_HEADER)
#define MBXHDR_CLR(pMbx) OsDbgAssert(sizeof(*pMbx)==sizeof(ETHERCAT_MBOX_HEADER)); OsMemset(pMbx,0,sizeof(ETHERCAT_MBOX_HEADER))

typedef struct TETHERCAT_MBOX_HEADER
{
    EC_T_WORD   wLength;               /* 0 */     /* following bytes */
    EC_T_WORD   wAddress;              /* 2 */     /* S->M: phys addr of destination; M->S: phys addr of source; 0 = master */
    EC_T_BYTE   byChnPri;              /* 4 */     /* channel, Priority */
    EC_T_BYTE   byTypCntRsvd;          /* 5 */     /* wMbxType, Counter, Rsvd */
} EC_PACKED(1) ETHERCAT_MBOX_HEADER, *PETHERCAT_MBOX_HEADER;

#define EC_ECMBOXHDR_SET_MBXTYPE(p, byVal)  {(p)->byTypCntRsvd = (EC_T_BYTE)(((p)->byTypCntRsvd & 0xF0) | ((byVal & 0x0F)));}
#define EC_ECMBOXHDR_GET_MBXTYPE(p)         ((EC_T_BYTE)((p)->byTypCntRsvd & 0x0F))

#define EC_ECMBOXHDR_SET_COUNTER(p, byVal)  {(p)->byTypCntRsvd = (EC_T_BYTE)(((p)->byTypCntRsvd & 0x8F) | ((byVal & 0x07) << 4));}
#define EC_ECMBOXHDR_GET_COUNTER(p)         ((EC_T_BYTE)(((p)->byTypCntRsvd >> 4) & 0x07))

static EC_INLINESTART EC_T_VOID EC_ECMBOXHDR_SET_LENGTH(ETHERCAT_MBOX_HEADER* p, EC_T_WORD wVal)
{
    EC_SET_FRM_WORD(((EC_T_PBYTE)(&(p->wLength))), wVal);
} EC_INLINESTOP
static EC_INLINESTART EC_T_WORD EC_ECMBOXHDR_GET_LENGTH(ETHERCAT_MBOX_HEADER* p)
{
    return EC_GET_FRM_WORD(((EC_T_PBYTE)(&(p->wLength))));
} EC_INLINESTOP

static EC_INLINESTART EC_T_VOID EC_ECMBOXHDR_SET_ADDRESS(ETHERCAT_MBOX_HEADER* p, EC_T_WORD wVal)
{
    EC_SET_FRM_WORD(((EC_T_PBYTE)(&(p->wAddress))), wVal);
} EC_INLINESTOP
static EC_INLINESTART EC_T_WORD EC_ECMBOXHDR_GET_ADDRESS(ETHERCAT_MBOX_HEADER* p)
{
    return EC_GET_FRM_WORD(((EC_T_PBYTE)(&(p->wAddress))));
} EC_INLINESTOP

typedef struct TETHERCAT_MBOX_CMD
{
    ETYPE_EC_CMD_HEADER     EcCmdHeader;
    ETHERCAT_MBOX_HEADER    MBoxHeader;
} EC_PACKED(1) ETHERCAT_MBOX_CMD, *PETHERCAT_MBOX_CMD;
#define ETHERCAT_MBOX_CMD_LEN   sizeof(ETHERCAT_MBOX_CMD)
          

typedef struct TETHERCAT_EOE_TIMESTAMP
{
    EC_T_DWORD  TimeStamp;  /* 32 bit time stamp */
} ETHERCAT_EOE_TIMESTAMP, *PETHERCAT_EOE_TIMESTAMP;
#define	ETHERCAT_EOE_TIMESTAMP_LEN	sizeof(ETHERCAT_EOE_TIMESTAMP)

/*---------------------------------------------------------------------------*/
/* EoE (Ethernet over EtherCAT) */
#define ETHERCAT_ETHERNET_FRAME_TYPE_EOE_FRAG_REQ   0   /* EoE fragment request */
#define ETHERCAT_ETHERNET_FRAME_TYPE_EOE_REQ        1   /* EoE request */
#define ETHERCAT_ETHERNET_FRAME_TYPE_EOE_RSP        3   /* EoE response */
#define ETHERCAT_ETHERNET_FRAME_TYPE_EOE_SETIP_REQ  2   /* EoE set IP parameter request */
#define ETHERCAT_ETHERNET_FRAME_TYPE_EOE_SETIP_RSP  3   /* EoE set IP parameter response */
#define ETHERCAT_ETHERNET_FRAME_TYPE_EOE_SETAF_REQ  4   /* EoE set adress filter request */
#define ETHERCAT_ETHERNET_FRAME_TYPE_EOE_SETAF_RSP  5   /* EoE set adress filter response */

#define ETHERNET_FRAGMENT_GRANULAR      32              /* length of each fragment (except the last fragment) must be dividable by 32 */
#define ETHERNET_FRAGMENT_MASK          0xFFFFFFE0
#define ETHERNET_FRAGMENT_BUFFER(n)     ((n+31)/32)
#define ETHERNET_FRAMENUMBER_MASK       0x0000000F
#define ETHERNET_MAX_FRAGMENTS          48
#define ETHERNET_MAX_FRAGMENTBUFFER (ETHERNET_MAX_FRAGMENTS*ETHERNET_FRAGMENT_GRANULAR) /* 1536 */

typedef struct TETHERCAT_ETHERNET_HEADER
{
    EC_T_BYTE   byTypePort;                         /*  0.0 */  /* frame type */
                                                    /*  0.4 */  /* port number */

    EC_T_BYTE   byBits;                             /*  1.0 */  /* EC_TRUE if last fragment */
                                                    /*  1.1 */  /* EC_TRUE if time stamp will be appended after the EoE data in the last fragment */
                                                    /*  1.2 */  /* EC_TRUE if time stamp value of the send time is requested */
    EC_T_WORD   wNumSizeNum;                        /*  2.0 */  /* fragment number */
                                                    /*  2.6 */  /* (complete size of the Ethernet frame + 17)/32 */
                                                    /*  2.6 */  /* byte offset multiplied by 32 (if Fragment != 0);  */
                                                                /* buffer size multiplied by 32 (if Fragment == 0) */
                                                    /*  2.12*/  /* number of the Ethernet frame */
} EC_PACKED(1) ETHERCAT_ETHERNET_HEADER, *PETHERCAT_ETHERNET_HEADER;
#define ETHERCAT_ETHERNET_HEADER_LEN    sizeof(ETHERCAT_ETHERNET_HEADER)
#define EOEHDR_CLR(pEoe) OsDbgAssert(sizeof(*pEoe)==sizeof(ETHERCAT_ETHERNET_HEADER)); OsMemset(pEoe,0,sizeof(ETHERCAT_ETHERNET_HEADER))

/* wTypePortBits */
#define EC_ECETHHDR_GET_FRAMETYPE(p)                ((p)->byTypePort & 0xF)
#define EC_ECETHHDR_SET_FRAMETYPE(p, byNewVal)      {(p)->byTypePort = (EC_T_BYTE)((((p)->byTypePort) & 0xF0) | (byNewVal & 0xF));}

#define EC_ECETHHDR_GET_PORT(p)                     ((p)->byTypePort>>4 & 0x0F)

#define EC_ECETHHDR_GET_LASTFRAGMENT(p)             ((p)->byBits>>0 & 0x01)
#define EC_ECETHHDR_SET_LASTFRAGMENT(p, byNewVal)   {(*p).byBits = (EC_T_BYTE)((((*p).byBits)& 0xFE) | (byNewVal & 1));}

#define EC_ECETHHDR_GET_TIMEAPPENDED(p)             ((p)->byBits>>1 & 0x01)
/* #define EC_ECETHHDR_SET_TIMEAPPENDED(p, byNewVal)   ((p)->byBits = (((p)->byBits)& 0XFD) | ((byNewVal & 1)<<1) */

#define EC_ECETHHDR_GET_TIMEREQUEST(p)              ((p)->byBits>>2 & 0x01)
/* #define EC_ECETHHDR_SET_TIMEREQUEST(p, byNewVal)    ((p)->byBits = (((p)->byBits)& 0XFB) | ((byNewVal & 1)<<2) */

/* wNumSizeNum */
#define EC_ECETHHDR_GET_FRAGMENTNUMBER(p)           EC_GET_FRM_WORD_BITFIELD(0,6, (*p).wNumSizeNum)
#define EC_ECETHHDR_SET_FRAGMENTNUMBER(p, wNewVal)  EC_SET_FRM_WORD_BITFIELD((*p).wNumSizeNum,(wNewVal),0,6)

#define EC_ECETHHDR_GET_COMPLETEFRAMESIZE(p)        EC_GET_FRM_WORD_BITFIELD(6,6, p->wNumSizeNum)
#define EC_ECETHHDR_SET_COMPLETEFRAMESIZE(p, wNewVal)  EC_SET_FRM_WORD_BITFIELD((*p).wNumSizeNum,(wNewVal),6,6)

#define EC_ECETHHDR_GET_OFFSETBUFFER(p)             EC_GET_FRM_WORD_BITFIELD(6,6, p->wNumSizeNum)
#define EC_ECETHHDR_SET_OFFSETBUFFER(p, wNewVal)    EC_SET_FRM_WORD_BITFIELD((*p).wNumSizeNum, (wNewVal),6,6)

#define EC_ECETHHDR_GET_FRAMENUMBER(p)              EC_GET_FRM_WORD_BITFIELD(12,4, (*p).wNumSizeNum)
#define EC_ECETHHDR_SET_FRAMENUMBER(p, wNewVal)     EC_SET_FRM_WORD_BITFIELD((*p).wNumSizeNum, (wNewVal),12,4)


/*---------------------------------------------------------------------------*/
/* CoE (CANopen over EtherCAT) */
#define ETHERCAT_CANOPEN_TYPE_EMERGENCY 1
#define ETHERCAT_CANOPEN_TYPE_SDOREQ    2
#define ETHERCAT_CANOPEN_TYPE_SDORES    3
#define ETHERCAT_CANOPEN_TYPE_TXPDO     4
#define ETHERCAT_CANOPEN_TYPE_RXPDO     5
#define ETHERCAT_CANOPEN_TYPE_TXPDO_RTR 6       /* Remote transmission request of TXPDO (master requested) */
#define ETHERCAT_CANOPEN_TYPE_RXPDO_RTR 7       /* Remote transmission request of RXPDO (slave requested) */
#define ETHERCAT_CANOPEN_TYPE_SDOINFO   8

#ifdef EC_NO_BITFIELDS

typedef EC_T_WORD ETHERCAT_CANOPEN_HEADER, *PETHERCAT_CANOPEN_HEADER;
#define ETHERCAT_CANOPEN_HEADER_LEN sizeof(ETHERCAT_CANOPEN_HEADER)
#define COEHDR_CLR(pCan) OsDbgAssert(sizeof(*pCan)==ETHERCAT_CANOPEN_HEADER_LEN); (*(pCan) = 0)

#define EC_ECCOEHDR_GET_COENUMBER(p)            EC_GET_FRM_WORD_BITFIELD(0,9,*((EC_T_WORD*)p))
#define EC_ECCOEHDR_SET_COENUMBER(p, wVal)      EC_SET_FRM_WORD_BITFIELD(*((EC_T_WORD*)p),wVal,0,9)

#define EC_ECCOEHDR_GET_COETYPE(p)              EC_GET_FRM_WORD_BITFIELD(12,4,*((EC_T_WORD*)p))
#define EC_ECCOEHDR_SET_COETYPE(p, wVal)        EC_SET_FRM_WORD_BITFIELD(*((EC_T_WORD*)p),wVal,12,4)

#else /* #ifdef EC_NO_BITFIELDS */

typedef struct TETHERCAT_CANOPEN_HEADER
{
    union _t_uCoeHdr
    {
        EC_T_WORD __wCoeHdr:16;
        struct _t_swCoeHdr
        {
#ifdef EC_BIG_ENDIAN
            EC_T_WORD   __wCoeType   : 4;   /* CANopen type */
            EC_T_WORD   Reserved     : 3;   /* = 0 */
            EC_T_WORD   __wCoeNumber : 9;   /* e.g. PDO number */
#else
            EC_T_WORD   __wCoeNumber : 9;   /* e.g. PDO number */
            EC_T_WORD   Reserved     : 3;   /* = 0 */
            EC_T_WORD   __wCoeType   : 4;   /* CANopen type (ETHERCAT_CANOPEN_TYPE_...) */
#endif
        } EC_PACKED(1) swCoeHdr;
    } EC_PACKED(1) uCoeHdr;
           
} EC_PACKED(1) ETHERCAT_CANOPEN_HEADER, *PETHERCAT_CANOPEN_HEADER;
#define ETHERCAT_CANOPEN_HEADER_LEN sizeof(ETHERCAT_CANOPEN_HEADER)
#define COEHDR_CLR(pCan) OsDbgAssert(sizeof(*pCan)==ETHERCAT_CANOPEN_HEADER_LEN); ((pCan)->uCoeHdr.__wCoeHdr = 0)

#define EC_ECCOEHDR_SET_COENUMBER(p, wVal)      ((p)->uCoeHdr.swCoeHdr.__wCoeNumber = (wVal))  
#define EC_ECCOEHDR_GET_COETYPE(p)              ((p)->uCoeHdr.swCoeHdr.__wCoeType)
#define EC_ECCOEHDR_SET_COETYPE(p, wVal)        ((p)->uCoeHdr.swCoeHdr.__wCoeType = (wVal))

#endif /* #else EC_NO_BITFIELDS */


#define EC_ECSDOHDR_OFFS_INDEX      ((EC_T_BYTE)1)
#define EC_ECSDOHDR_OFFS_SUBINDEX   ((EC_T_BYTE)3)
#define EC_ECSDOHDR_OFFS_SDODATA    ((EC_T_BYTE)4)

typedef struct TEC_SDO_HDR
{
    union _t_uHdr_sdoheader
    {
/* EC_NO_BITFIELDS is always set for EC_BIG_ENDIAN, but the SET/GET macros for the Bit Field definitions below do not exist or are not used */
#if (!defined EC_NO_BITFIELDS) || (defined EC_BIG_ENDIAN)
        struct _t_sIdq
        {   /* Initiate Download Request */
#ifdef EC_BIG_ENDIAN
            EC_T_BYTE   Ccs         : 3;    /* = 1 */
            EC_T_BYTE   Complete    : 1;
            EC_T_BYTE   Size        : 2;
            EC_T_BYTE   Expedited   : 1;
            EC_T_BYTE   SizeInd     : 1;
#else
            EC_T_BYTE   SizeInd     : 1;
            EC_T_BYTE   Expedited   : 1;
            EC_T_BYTE   Size        : 2;
            EC_T_BYTE   Complete    : 1;
            EC_T_BYTE   Ccs         : 3;    /* = 1 */
#endif /* !EC_BIG_ENDIAN */
        } EC_PACKED(1) Idq;
        struct _t_sIds
        {   /* Initiate Download Response */
#ifdef EC_BIG_ENDIAN
            EC_T_BYTE   Scs         : 3;    /* = 3 */
            EC_T_BYTE   Reserved    : 5;
#else
            EC_T_BYTE   Reserved    : 5;
            EC_T_BYTE   Scs         : 3;    /* = 3 */
#endif /* !EC_BIG_ENDIAN */
        } EC_PACKED(1) Ids;
        struct _t_sDsq
        {   /* Download Segment Request */
#ifdef EC_BIG_ENDIAN
            EC_T_BYTE   Ccs         : 3;    /* = 0 */
            EC_T_BYTE   Toggle      : 1;
            EC_T_BYTE   Size        : 3;
            EC_T_BYTE   LastSeg     : 1;
#else
            EC_T_BYTE   LastSeg     : 1;
            EC_T_BYTE   Size        : 3;
            EC_T_BYTE   Toggle      : 1;
            EC_T_BYTE   Ccs         : 3;    /* = 0 */
#endif /* !EC_BIG_ENDIAN */
        } EC_PACKED(1) Dsq;
        struct _t_sDss
        {   /* Download Segment Response */
#ifdef EC_BIG_ENDIAN
            EC_T_BYTE   Scs         : 3;    /* = 1 */
            EC_T_BYTE   Toggle      : 1;
            EC_T_BYTE   Reserved    : 4;
#else
            EC_T_BYTE   Reserved    : 4;
            EC_T_BYTE   Toggle      : 1;
            EC_T_BYTE   Scs         : 3;    /* = 1 */
#endif /* !EC_BIG_ENDIAN */
        } EC_PACKED(1) Dss;
        struct _t_sIuq
        {   /* Initiate Upload Request */
#ifdef EC_BIG_ENDIAN
            EC_T_BYTE   Ccs         : 3;    /* = 2 */
            EC_T_BYTE   Complete    : 1;
            EC_T_BYTE   Reserved    : 4;
#else
            EC_T_BYTE   Reserved    : 4;
            EC_T_BYTE   Complete    : 1;
            EC_T_BYTE   Ccs         : 3;    /* = 2 */
#endif /* !EC_BIG_ENDIAN */
        } EC_PACKED(1) Iuq;
        struct _t_sIus
        {   /* Initiate Upload Response */
#ifdef EC_BIG_ENDIAN
            EC_T_BYTE   Scs         : 3;    /* = 2 */
            EC_T_BYTE   Reserved    : 1;
            EC_T_BYTE   Size        : 2;
            EC_T_BYTE   Expedited   : 1;
            EC_T_BYTE   SizeInd     : 1;
#else
            EC_T_BYTE   SizeInd     : 1;
            EC_T_BYTE   Expedited   : 1;
            EC_T_BYTE   Size        : 2;
            EC_T_BYTE   Reserved    : 1;
            EC_T_BYTE   Scs         : 3;    /* = 2 */
#endif /* !EC_BIG_ENDIAN */
        } EC_PACKED(1) Ius;
        struct _t_sUsq
        {   /* Upload Segment Request */
#ifdef EC_BIG_ENDIAN
            EC_T_BYTE   Ccs         : 3;    /* = 3 */
            EC_T_BYTE   Toggle      : 1;
            EC_T_BYTE   Reserved    : 4;
#else
            EC_T_BYTE   Reserved    : 4;
            EC_T_BYTE   Toggle      : 1;
            EC_T_BYTE   Ccs         : 3;    /* = 3 */
#endif /* !EC_BIG_ENDIAN */
        } EC_PACKED(1) Usq;
        struct _t_sUss
        {   /* Upload Segment Response */
#ifdef EC_BIG_ENDIAN
            EC_T_BYTE   Scs         : 3;    /* = 0 */
            EC_T_BYTE   Toggle      : 1;
            EC_T_BYTE   Size        : 3;
            EC_T_BYTE   LastSeg     : 1;
#else
            EC_T_BYTE   LastSeg     : 1;
            EC_T_BYTE   Size        : 3;
            EC_T_BYTE   Toggle      : 1;
            EC_T_BYTE   Scs         : 3;    /* = 0 */
#endif /* !EC_BIG_ENDIAN */
        } EC_PACKED(1) Uss;
        struct _t_sCS
        {   /* Abort Transfer */
#ifdef EC_BIG_ENDIAN
            EC_T_BYTE   Ccs         : 3;    /* = 4 */
            EC_T_BYTE   Reserved    : 5;
#else
            EC_T_BYTE   Reserved    : 5;
            EC_T_BYTE   Ccs         : 3;    /* = 4 */
#endif /* !EC_BIG_ENDIAN */
        } EC_PACKED(1) Abt;
#endif /* EC_NO_BITFIELDS && !EC_BIG_ENDIAN */
        EC_T_BYTE byCS;                                
    } EC_PACKED(1) uHdr;                                   /* 0 */
    EC_T_WORD   __Index;                                /* 1 */
    EC_T_BYTE   SubIndex;                               /* 3 */
    EC_T_DWORD  __dwSdoData;                            /* 4 */
} EC_PACKED(1) EC_SDO_HDR, *PEC_SDO_HDR;

#define SDO_HDR_INDEX_OFFSET            (sizeof(EC_T_BYTE))                                     /* CS */
#define SDO_HDR_SUB_INDEX_OFFSET        (sizeof(EC_T_BYTE)+sizeof(EC_T_WORD))                   /* CS + Index */
#define SDO_HDR_DATA_OFFSET             (sizeof(EC_T_BYTE)+sizeof(EC_T_WORD)+sizeof(EC_T_BYTE)) /* CS + Index + SubIndex */
#define EC_SDO_HDR_LEN         sizeof(EC_SDO_HDR)
#define ETHERCAT_MIN_SDO_MBOX_LEN       (ETHERCAT_MBOX_HEADER_LEN + ETHERCAT_CANOPEN_HEADER_LEN + EC_SDO_HDR_LEN)
#define SDOHDR_CLR(pSdo) OsDbgAssert(sizeof(*pSdo)==EC_SDO_HDR_LEN); OsMemset(pSdo,0,EC_SDO_HDR_LEN)

static EC_INLINESTART EC_T_WORD EC_ECSDOHDR_GET_INDEX(PEC_SDO_HDR p)
{
    return EC_GET_FRM_WORD((((EC_T_PBYTE)p) + EC_ECSDOHDR_OFFS_INDEX));
} EC_INLINESTOP
static EC_INLINESTART EC_T_DWORD EC_ECSDOHDR_GET_SDODATA(PEC_SDO_HDR p)
{
    return EC_GET_FRM_DWORD((((EC_T_PBYTE)p) + EC_ECSDOHDR_OFFS_SDODATA));
} EC_INLINESTOP
static EC_INLINESTART EC_T_VOID EC_ECSDOHDR_SET_INDEX(PEC_SDO_HDR p, EC_T_WORD wVal)
{
    EC_SET_FRM_WORD((((EC_T_PBYTE)p) + EC_ECSDOHDR_OFFS_INDEX), wVal);
} EC_INLINESTOP
static EC_INLINESTART EC_T_VOID EC_ECSDOHDR_SET_SDODATA(PEC_SDO_HDR p, EC_T_DWORD dwVal)
{
    EC_SET_FRM_DWORD((((EC_T_PBYTE)p) + EC_ECSDOHDR_OFFS_SDODATA), dwVal);
} EC_INLINESTOP

static EC_INLINESTART EC_T_PBYTE EC_ECSDOHDR_SDODATA(PEC_SDO_HDR p)
{
    return (EC_T_PBYTE)(((EC_T_PBYTE)p) + EC_ECSDOHDR_OFFS_SDODATA);
} EC_INLINESTOP

/* command specifier - request */
#define SDO_CCS_DOWNLOAD_SEGMENT                        0
#define SDO_CCS_INITIATE_DOWNLOAD                       1
#define SDO_CCS_INITIATE_UPLOAD                         2
#define SDO_CCS_UPLOAD_SEGMENT                          3
#define SDO_CCS_ABORT_TRANSFER                          4

/* command specifier - response */                                                       
#define SDO_SCS_UPLOAD_SEGMENT                          0
#define SDO_SCS_DOWNLOAD_SEGMENT                        1
#define SDO_SCS_INITIATE_UPLOAD                         2
#define SDO_SCS_INITIATE_DOWNLOAD                       3
#define SDO_SCS_ABORT_TRANSFER                          4           /* Not defined as valid SDO Res in ETG.1000.6
                                                                       but implemented on some slaves not based on ET9300 (SSC) */
                                                        
#define SDO_DOWNLOAD_SEGMENT_MAX_DATA                   7

/*****************************************************************************
 * CoE Abort Codes are defined in ETG.1000.6
 * Additional codes are defined in ETG.1020, V1.2.0, Table 21: CoE Abort Codes (extension)
 * See also EC_E_SDO_ABORTCODE_..., EC_SZTXT_E_SDO_ABORTCODE_..., SdoAbortToErrorCode, EC_SDO_ABORTCODE_
 *****************************************************************************/
#define SDO_ABORTCODE_TOGGLE                            0x05030000  /* Toggle bit not alternated */
#define SDO_ABORTCODE_TIMEOUT                           0x05040000  /* SDO protocol timed out */
#define SDO_ABORTCODE_CCS_SCS                           0x05040001  /* Client/server command specifier not valid or unknown */
#define SDO_ABORTCODE_BLK_SIZE                          0x05040002  /* Invalid block size (block mode only) */
#define SDO_ABORTCODE_SEQNO                             0x05040003  /* Invalid sequence number (block mode only) */
#define SDO_ABORTCODE_CRC                               0x05040004  /* CRC error (block mode only) */
#define SDO_ABORTCODE_MEMORY                            0x05040005  /* Out of memory */
#define SDO_ABORTCODE_ACCESS                            0x06010000  /* Unsupported access to an object */
#define SDO_ABORTCODE_WRITEONLY                         0x06010001  /* Attempt to read a write only object */
#define SDO_ABORTCODE_READONLY                          0x06010002  /* Attempt to write a read only object */
#define SDO_ABORTCODE_SI_NOT_WRITTEN                    0x06010003  /* Subindex cannot be written, SI0 must be 0 for write access */
#define SDO_ABORTCODE_CA_TYPE_MISM                      0x06010004  /* Complete access not supported for objects of variable length such as ENUM object types */
#define SDO_ABORTCODE_OBJ_TOO_BIG                       0x06010005  /* Object length exceeds mailbox size */
#define SDO_ABORTCODE_PDO_MAPPED                        0x06010006  /* Object mapped to RxPDO, SDO Download blocked */
#define SDO_ABORTCODE_INDEX                             0x06020000  /* Object does not exist in the object dictionary */
#define SDO_ABORTCODE_PDO_MAP                           0x06040041  /* Object cannot be mapped to the PDO */
#define SDO_ABORTCODE_PDO_LEN                           0x06040042  /* The number and length of the objects to be mapped would exceed PDO length */
#define SDO_ABORTCODE_P_INCOMP                          0x06040043  /* General parameter incompatibility reason */
#define SDO_ABORTCODE_I_INCOMP                          0x06040047  /* General internal incompatibility in the device */
#define SDO_ABORTCODE_HARDWARE                          0x06060000  /* Access failed due to an hardware error */
#define SDO_ABORTCODE_DATA_SIZE                         0x06070010  /* Data type does not match, length of service parameter does not match */
#define SDO_ABORTCODE_DATA_SIZE1                        0x06070012  /* Data type does not match, length of service parameter too high */
#define SDO_ABORTCODE_DATA_SIZE2                        0x06070013  /* Data type does not match, length of service parameter too low */
#define SDO_ABORTCODE_OFFSET                            0x06090011  /* Sub-index does not exist */
#define SDO_ABORTCODE_DATA_RANGE                        0x06090030  /* Value range of parameter exceeded (only for write access) */
#define SDO_ABORTCODE_DATA_RANGE1                       0x06090031  /* Value of parameter written too high */
#define SDO_ABORTCODE_DATA_RANGE2                       0x06090032  /* Value of parameter written too low */
#define SDO_ABORTCODE_MODULE_ID_LIST_NOT_MATCH          0x06090033  /* Detected Module Ident List (0xF030) and Configured Module Ident list (0xF050) does not match */
#define SDO_ABORTCODE_MINMAX                            0x06090036  /* Maximum value is less than minimum value */
#define SDO_ABORTCODE_GENERAL                           0x08000000  /* general error*/
#define SDO_ABORTCODE_TRANSFER                          0x08000020  /* Data cannot be transferred or stored to the application */
#define SDO_ABORTCODE_TRANSFER1                         0x08000021  /* Data cannot be transferred or stored to the application because of local control */
#define SDO_ABORTCODE_TRANSFER2                         0x08000022  /* Data cannot be transferred or stored to the application because of the present device state */
#define SDO_ABORTCODE_DICTIONARY                        0x08000023  /* Object dictionary dynamic generation fails or no object dictionary is present (e.g. object dictionary is generated from file and generation fails because of an file error) */

#define SDO_SUB_IDX_SUBINDEX_CNT                        0

#define SDO_IDX_DEVICE_TYPE                             0x1000
#define SDO_IDX_ERROR                                   0x1001
#define SDO_IDX_MANUFACTURER_NAME                       0x1008
#define SDO_IDX_MANUFACTURER_HW_VER                     0x1009
#define SDO_IDX_MANUFACTURER_SW_VER                     0x100A
#define SDO_IDX_IDENTITY_OBJECT                         0x1018
#define SDO_SIDX_IDENTITY_OBJECT_VENDORID               1
#define SDO_SIDX_IDENTITY_OBJECT_PCODE                  2
#define SDO_SIDX_IDENTITY_OBJECT_REVNO                  3
#define SDO_SIDX_IDENTITY_OBJECT_SERNO                  4

                                                        
#define SDO_IDX_ETHERCAT_ADDR                           0x1100

#define SDO_IDX_RXPDO1_PARA                             0x1400
#define SDO_SIDX_RXPDXX_PARA_EXCLUDE                    1

#define SDO_IDX_RXPDO2_PARA                             0x1401
/* ... */
#define SDO_IDX_RXPDO512_PARA                           0x15FF

#define SDO_IDX_TXPDO1_PARA                             0x1800
#define SDO_IDX_TXPDO2_PARA                             0x1801
/* ... */
#define SDO_IDX_TXPDO512_PARA                           0x19FF

#define SDO_IDX_RXPDO1_MAPPING                          0x1600
#define SDO_IDX_RXPDO2_MAPPING                          0x1601
/* ... */
#define SDO_IDX_RXPDO512_MAPPING                        0x17FF
                                                        
#define SDO_IDX_TXPDO1_MAPPING                          0x1A00
#define SDO_IDX_TXPDO2_MAPPING                          0x1A01
/* ... */
#define SDO_IDX_TXPDO512_MAPPING                        0x1BFF
                                                        
#define SDO_IDX_SYNCMAN_TYPE                            0x1C00
#define SDO_IDX_SYNCMAN0_PDOASSIGN                      0x1C10
#define SDO_IDX_SYNCMAN1_PDOASSIGN                      0x1C11
#define SDO_IDX_SYNCMAN2_PDOASSIGN                      0x1C12
#define SDO_IDX_SYNCMAN3_PDOASSIGN                      0x1C13

/* ... */
#define SDO_IDX_SYNCMAN31_PDOASSIGN                     0x1C2F

#define SDO_IDX_ECAT_MEMORY_0000_00FF                   0x1D00
#define SDO_IDX_ECAT_MEMORY_0100_01FF                   0x1D01
/* ... */
#define SDO_IDX_ECAT_MEMORY_FF00_FFFF                   0x1DFF

/* CoE SDO Information */
#define ECAT_COE_INFO_OPCODE_LIST_Q                     1
#define ECAT_COE_INFO_OPCODE_LIST_S                     2
#define ECAT_COE_INFO_OPCODE_OBJ_Q                      3
#define ECAT_COE_INFO_OPCODE_OBJ_S                      4
#define ECAT_COE_INFO_OPCODE_ENTRY_Q                    5
#define ECAT_COE_INFO_OPCODE_ENTRY_S                    6
#define ECAT_COE_INFO_OPCODE_ERROR_S                    7

#define ECAT_COE_INFO_LIST_TYPE_LENGTH                  0
#define ECAT_COE_INFO_LIST_TYPE_ALL                     1
#define ECAT_COE_INFO_LIST_TYPE_RXPDOMAP                2
#define ECAT_COE_INFO_LIST_TYPE_TXPDOMAP                3
#define ECAT_COE_INFO_LIST_TYPE_BACKUP                  4
                                                        
#define ECAT_COE_INFO_OBJCODE_NULL                      0
#define ECAT_COE_INFO_OBJCODE_DOMAIN                    2
#define ECAT_COE_INFO_OBJCODE_DEFTYPE                   5
#define ECAT_COE_INFO_OBJCODE_DEFSTRUCT                 6
#define ECAT_COE_INFO_OBJCODE_VAR                       7
#define ECAT_COE_INFO_OBJCODE_ARRAY                     8
#define ECAT_COE_INFO_OBJCODE_RECORD                    9
                                                        
#define ECAT_COE_INFO_OBJCAT_OPTIONAL                   0
#define ECAT_COE_INFO_OBJCAT_MANDATORY                  1
                                                        
#define ECAT_COE_INFO_OBJACCESS_RO                      0x07
#define ECAT_COE_INFO_OBJACCESS_RW                      0x3f

typedef struct TETHERCAT_SDO_INFO_LIST
{
    EC_T_WORD   __ListType;                   /* == SDO_INFO_LIST_TYPE_XXX */
    struct _t_sRes_sdoinfolist
    {
        EC_T_WORD   __Index[1];       
    } EC_PACKED(1) Res;
} EC_PACKED(1) ETHERCAT_SDO_INFO_LIST, *PETHERCAT_SDO_INFO_LIST;

static EC_INLINESTART EC_T_WORD EC_SDOINFOLIST_GET_LISTTYPE(PETHERCAT_SDO_INFO_LIST p)
{
    return EC_GET_FRM_WORD((((EC_T_PBYTE)p)));
} EC_INLINESTOP
static EC_INLINESTART EC_T_VOID EC_SDOINFOLIST_SET_LISTTYPE(PETHERCAT_SDO_INFO_LIST p, EC_T_WORD wVal)
{
    EC_SET_FRM_WORD((((EC_T_PBYTE)p)), wVal);
} EC_INLINESTOP

#define EC_SDOINFOOBJ_OFFS_INDEX        ((EC_T_BYTE)0)
#define EC_SDOINFOOBJ_OFFS_DATATYPE     ((EC_T_BYTE)2)

typedef struct TETHERCAT_SDO_INFO_OBJ
{
    EC_T_WORD   __Index;
    struct _t_sRes_sdoinfoobj
    {
        EC_T_WORD   __DataType;               /* refer to data type index */
        EC_T_BYTE   MaxSubIndex;            /* max subIndex */
#if (!defined EC_NO_BITFIELDS)
  #ifdef EC_BIG_ENDIAN
        EC_T_BYTE   Reserved        : 3;    /* == 0 */
        EC_T_BYTE   ObjCategory     : 1;    /* 0=optional, 1=mandatory */
        EC_T_BYTE   ObjCode         : 4;    /* defined in DS 301 (Table 37) */
  #else
        EC_T_BYTE   ObjCode         : 4;    /* defined in DS 301 (Table 37) */
        EC_T_BYTE   ObjCategory     : 1;    /* 0=optional, 1=mandatory */
        EC_T_BYTE   Reserved        : 3;    /* == 0 */
  #endif
#else
        EC_T_BYTE   byObjCodeCategory;
#endif /* EC_NO_BITFIELDS */
        EC_T_CHAR   Name[1];                /* rest of mailbox data */
    } EC_PACKED(1) Res;
} EC_PACKED(1) ETHERCAT_SDO_INFO_OBJ, *PETHERCAT_SDO_INFO_OBJ;

static EC_INLINESTART EC_T_WORD EC_SDOINFOOBJ_GET_INDEX(PETHERCAT_SDO_INFO_OBJ p)
{
    return EC_GET_FRM_WORD((((EC_T_PBYTE)p) + EC_SDOINFOOBJ_OFFS_INDEX));
} EC_INLINESTOP
static EC_INLINESTART EC_T_WORD EC_SDOINFOOBJ_GET_DATATYPE(PETHERCAT_SDO_INFO_OBJ p)
{
    return EC_GET_FRM_WORD((((EC_T_PBYTE)p) + EC_SDOINFOOBJ_OFFS_DATATYPE));
} EC_INLINESTOP
static EC_INLINESTART EC_T_VOID EC_SDOINFOOBJ_SET_INDEX(PETHERCAT_SDO_INFO_OBJ p, EC_T_WORD wVal)
{
    EC_SET_FRM_WORD((((EC_T_PBYTE)p) + EC_SDOINFOOBJ_OFFS_INDEX), wVal);
} EC_INLINESTOP
static EC_INLINESTART EC_T_VOID EC_SDOINFOOBJ_SET_DATATYPE(PETHERCAT_SDO_INFO_OBJ p, EC_T_WORD wVal)
{
    EC_SET_FRM_WORD((((EC_T_PBYTE)p) + EC_SDOINFOOBJ_OFFS_DATATYPE), wVal);
} EC_INLINESTOP

#define EC_ECSDOINFOENTRY_OFFS_INDEX        ((EC_T_BYTE)0)
#define EC_ECSDOINFOENTRY_OFFS_DATATYPE     ((EC_T_BYTE)4)
#define EC_ECSDOINFOENTRY_OFFS_BITLEN       ((EC_T_BYTE)6)
#define EC_ECSDOINFOENTRY_OFFS_BITPARM      ((EC_T_BYTE)8)

typedef struct TETHERCAT_SDO_INFO_ENTRY
{
    EC_T_WORD       __Index;
    EC_T_BYTE       SubIdx; 
    EC_T_BYTE       ValueInfo;              /* bit0 = ObjAccess, bit1 = ObjCategory, bit2 = PdoMapping, bit3 = UnitType */
                                            /* bit4 = DefaultValue, bit5 = MinValue, bit6 = MaxValue */
    struct _t_sRes_sdoinfoentry
    {
        EC_T_WORD   __DataType;             /* refer to data type index */
        EC_T_WORD   __BitLen;       
#if (!defined EC_NO_BITFIELDS)
  #ifdef EC_BIG_ENDIAN
        EC_T_BYTE   __TxPdoMapping  : 1;    /* */
        EC_T_BYTE   __RxPdoMapping  : 1;    /* */
        EC_T_BYTE   __ObjAccess     : 6;    /* bit0 = read; bit1 = write; bit2 = const. bit3 = PRE-OP bit4 = SAFE-OP bit5 = OP */
  #else
        EC_T_BYTE   __ObjAccess     : 6;    /* bit0 = read; bit1 = write; bit2 = const. bit3 = PRE-OP bit4 = SAFE-OP bit5 = OP */
        EC_T_BYTE   __RxPdoMapping  : 1;    /* */
        EC_T_BYTE   __TxPdoMapping  : 1;    /* */        
  #endif
#else
        EC_T_BYTE   byObjAccessRxPdoMappingTxPdoMapping;
#endif /* EC_NO_BITFIELDS */
        EC_T_BYTE   Reserved;    			/* for future use */
    } EC_PACKED(1) Res;
} EC_PACKED(1) ETHERCAT_SDO_INFO_ENTRY, *PETHERCAT_SDO_INFO_ENTRY;

static EC_INLINESTART EC_T_WORD EC_SDOINFOENTRY_GET_INDEX(PETHERCAT_SDO_INFO_ENTRY p)
{
    return EC_GET_FRM_WORD((((EC_T_PBYTE)p) + EC_ECSDOINFOENTRY_OFFS_INDEX));
} EC_INLINESTOP
static EC_INLINESTART EC_T_WORD EC_SDOINFOENTRY_GET_DATATYPE(PETHERCAT_SDO_INFO_ENTRY p)
{
    return EC_GET_FRM_WORD((((EC_T_PBYTE)p) + EC_ECSDOINFOENTRY_OFFS_DATATYPE));
} EC_INLINESTOP
static EC_INLINESTART EC_T_WORD EC_SDOINFOENTRY_GET_BITLEN(PETHERCAT_SDO_INFO_ENTRY p)
{
    return EC_GET_FRM_WORD((((EC_T_PBYTE)p) + EC_ECSDOINFOENTRY_OFFS_BITLEN));
} EC_INLINESTOP
static EC_INLINESTART EC_T_WORD EC_SDOINFOENTRY_GET_OBJACCESS(PETHERCAT_SDO_INFO_ENTRY p)
{
    return (EC_T_WORD)((EC_GET_FRM_WORD((((EC_T_PBYTE)p) + EC_ECSDOINFOENTRY_OFFS_BITLEN))>>0) & ((1<<6) - 1));
} EC_INLINESTOP
static EC_INLINESTART EC_T_WORD EC_SDOINFOENTRY_GET_RXPDOMAPPING(PETHERCAT_SDO_INFO_ENTRY p)
{
    return (EC_T_WORD)((EC_GET_FRM_WORD((((EC_T_PBYTE)p) + EC_ECSDOINFOENTRY_OFFS_BITLEN)) >> 6) & 1);
} EC_INLINESTOP
static EC_INLINESTART EC_T_WORD EC_SDOINFOENTRY_GET_TXPDOMAPPING(PETHERCAT_SDO_INFO_ENTRY p)
{
    return (EC_T_WORD)((EC_GET_FRM_WORD((((EC_T_PBYTE)p) + EC_ECSDOINFOENTRY_OFFS_BITLEN)) >> 7) & 1);
} EC_INLINESTOP

static EC_INLINESTART EC_T_VOID EC_SDOINFOENTRY_SET_INDEX(PETHERCAT_SDO_INFO_ENTRY p, EC_T_WORD wVal)
{
    EC_SET_FRM_WORD((((EC_T_PBYTE)p) + EC_ECSDOINFOENTRY_OFFS_INDEX), wVal);
} EC_INLINESTOP
       
typedef struct TETHERCAT_SDO_INFO_ERROR
{
    EC_T_DWORD      __ErrorCode;
    EC_T_CHAR       ErrorText[1];           /* rest of mailbox data */
} EC_PACKED(1) ETHERCAT_SDO_INFO_ERROR, *PETHERCAT_SDO_INFO_ERROR;

static EC_INLINESTART EC_T_DWORD EC_SDOINFOERROR_GET_ERRORCODE(PETHERCAT_SDO_INFO_ERROR p)
{
    return EC_GET_FRM_DWORD(((EC_T_PBYTE)p));
} EC_INLINESTOP
static EC_INLINESTART EC_T_VOID EC_SDOINFOERROR_SET_ERRORCODE(PETHERCAT_SDO_INFO_ERROR p, EC_T_DWORD dwVal)
{
    EC_SET_FRM_DWORD(((EC_T_PBYTE)p), dwVal);
} EC_INLINESTOP

typedef struct TETHERCAT_SDO_INFO_HEADER
{
/* EC_NO_BITFIELDS is always set for EC_BIG_ENDIAN, but the SET/GET macros for the Bit Field definitions below do not exist or are not used */
#if (!defined EC_NO_BITFIELDS) || (defined EC_BIG_ENDIAN)
  #ifdef EC_BIG_ENDIAN
    EC_T_BYTE       InComplete      : 1;    /* */
    EC_T_BYTE       OpCode          : 7;    /* == SDO_INFO_TYPE_XXX */  /* 0 */
  #else
    EC_T_BYTE       OpCode          : 7;    /* == SDO_INFO_TYPE_XXX */  /* 0 */
    EC_T_BYTE       InComplete      : 1;    /* */
  #endif
#else
    EC_T_BYTE       byOpCodeInComplete;
#endif /* EC_NO_BITFIELDS && !EC_BIG_ENDIAN */

    EC_T_BYTE       Reserved;               /* == 0 */                  /* 1 */

    EC_T_WORD       __FragmentsLeft;        /* */                       /* 2 */

    union _t_uInfo
    {
        ETHERCAT_SDO_INFO_LIST  List;
        ETHERCAT_SDO_INFO_OBJ   Obj;
        ETHERCAT_SDO_INFO_ENTRY Entry;
        ETHERCAT_SDO_INFO_ERROR Error;
        EC_T_BYTE               Data[1];
    } EC_PACKED(1) uInfo;                                                  /* 4 */
} EC_PACKED(1) ETHERCAT_SDO_INFO_HEADER, *PETHERCAT_SDO_INFO_HEADER;
#define SDOINFOHDR_CLR(pSdoInfo) OsDbgAssert(sizeof(*pSdoInfo)==sizeof(ETHERCAT_SDO_INFO_HEADER)); OsMemset(pSdoInfo,0,sizeof(ETHERCAT_SDO_INFO_HEADER))

static EC_INLINESTART EC_T_WORD EC_SDOINFOHDR_GET_FRAGMENTSLEFT(PETHERCAT_SDO_INFO_HEADER p)
{
    return EC_GET_FRM_WORD((((EC_T_PBYTE)p)+2));
} EC_INLINESTOP
static EC_INLINESTART EC_T_VOID EC_SDOINFOHDR_SET_FRAGMENTSLEFT(PETHERCAT_SDO_INFO_HEADER p, EC_T_WORD wVal)
{
    EC_SET_FRM_WORD((((EC_T_PBYTE)p)+2), wVal);
} EC_INLINESTOP

#define ETHERCAT_SDO_INFO_LISTREQ_LEN   EC_OFFSETOF(ETHERCAT_SDO_INFO_HEADER, uInfo.List.Res)
#define ETHERCAT_SDO_INFO_OBJREQ_LEN    EC_OFFSETOF(ETHERCAT_SDO_INFO_HEADER, uInfo.Obj.Res)
#define ETHERCAT_SDO_INFO_ENTRYREQ_LEN  EC_OFFSETOF(ETHERCAT_SDO_INFO_HEADER, uInfo.Entry.Res)

typedef struct TETHERCAT_EMERGENCY_HEADER
{
    EC_T_WORD   __ErrorCode;
    EC_T_BYTE   ErrorRegister;
    EC_T_BYTE   Data[5];
} EC_PACKED(1) ETHERCAT_EMERGENCY_HEADER, *PETHERCAT_EMERGENCY_HEADER;

static EC_INLINESTART EC_T_WORD EC_EMERGHDR_GET_ERRORCODE(PETHERCAT_EMERGENCY_HEADER p)
{
    return EC_GET_FRM_WORD(((EC_T_PBYTE)p));
} EC_INLINESTOP
static EC_INLINESTART EC_T_VOID EC_EMERGHDR_SET_ERRORCODE(PETHERCAT_EMERGENCY_HEADER p, EC_T_WORD wVal)
{
    EC_SET_FRM_WORD(((EC_T_PBYTE)p), wVal);
} EC_INLINESTOP

/*---------------------------------------------------------------------------*/
/* FoE (File Access over EtherCAT) */
#define ECAT_FOE_OPCODE_RRQ                 1
#define ECAT_FOE_OPCODE_WRQ                 2
#define ECAT_FOE_OPCODE_DATA                3
#define ECAT_FOE_OPCODE_ACK                 4
#define ECAT_FOE_OPCODE_ERR                 5
#define ECAT_FOE_OPCODE_BUSY                6

/* See ETG1000.6 Table 92 - Error codes of FoE */
#define ECAT_FOE_ERRCODE_NOTDEFINED         0x8000 /**< \brief Not defined */
#define ECAT_FOE_ERRCODE_NOTFOUND           0x8001 /**< \brief File not found */
#define ECAT_FOE_ERRCODE_ACCESS             0x8002 /**< \brief No file access */
#define ECAT_FOE_ERRCODE_DISKFULL           0x8003 /**< \brief Disk is full */
#define ECAT_FOE_ERRCODE_ILLEGAL            0x8004 /**< \brief Illegal access */
#define ECAT_FOE_ERRCODE_PACKENO            0x8005 /**< \brief Invalid packed number */
#define ECAT_FOE_ERRCODE_EXISTS             0x8006 /**< \brief File not exists */
#define ECAT_FOE_ERRCODE_NOUSER             0x8007 /**< \brief No User */
#define ECAT_FOE_ERRCODE_BOOTSTRAPONLY      0x8008 /**< \brief Only in Bootstrap state */
#define ECAT_FOE_ERRCODE_NOTINBOOTSTRAP     0x8009 /**< \brief Downloaded file name is not valid in Bootstrap state */
#define ECAT_FOE_ERRCODE_NORIGHTS           0x800A /**< \brief No access rights */
#define ECAT_FOE_ERRCODE_PROGERROR          0x800B /**< \brief Program error */
#define ECAT_FOE_ERRCODE_INVALID_CHECKSUM   0x800C /**< \brief Wrong checksum */
#define ECAT_FOE_ERRCODE_INVALID_FIRMWARE   0x800D /**< \brief Firmware does not fit for Hardware */
                                         /* 0x800E  **<  reserved */
#define ECAT_FOE_ERRCODE_NO_FILE            0x800F /**< \brief No file to read */
#define ECAT_FOE_ERRCODE_FILE_HEAD_MISSING  0x8010 /**< \brief File header does not exist */
#define ECAT_FOE_ERRCODE_FLASH_PROBLEM      0x8011 /**< \brief Flash problem */
#define ECAT_FOE_ERRCODE_FILE_INCOMPATIBLE  0x8012 /**< \brief File incompatible */

#define EC_ECFOEHDR_OFFS_OPCODE             ((EC_T_BYTE)0)
#define EC_ECFOEHDR_OFFS_UHDR               ((EC_T_BYTE)2)
#define EC_ECFOEHDR_OFFS_ENTIRE             ((EC_T_BYTE)4)
#define EC_ECFOEHDR_OFFS_BUSY_TEXT          ((EC_T_BYTE)8)

typedef struct TEC_FOE_HDR
{
    EC_T_BYTE       OpCode;         /* 0 EC_ECFOEHDR_OFFS_OPCODE: = 1 (RRQ), = 2 (WRQ), = 3 (DATA), = 4 (ACK), = 5 (ERR), = 6 (BUSY), see ECAT_FOE_OPCODE_... */
    EC_T_BYTE       Reserved1;      /* 1 */     /* = 0 */
    union _t_uHdr_foeheader
    {
        EC_T_DWORD      __Password; /* 2 EC_ECFOEHDR_OFFS_UHDR: (ECAT_FOE_OPCODE_RRQ, ECAT_FOE_OPCODE_WRQ)       = 0 if unknown */
        EC_T_DWORD      __PacketNo; /* 2 EC_ECFOEHDR_OFFS_UHDR: (ECAT_FOE_OPCODE_DATA, ECAT_FOE_OPCODE_ACK) */
        EC_T_DWORD      __ErrorCode;/* 2 EC_ECFOEHDR_OFFS_UHDR: (ECAT_FOE_OPCODE_ERR) */
        struct _t_sStatus
        {
            EC_T_WORD   __Done;     /* 2 EC_ECFOEHDR_OFFS_UHDR: 0...max busy progress (ECAT_FOE_OPCODE_BUSY) */
            EC_T_WORD   __Entire;   /* 4 EC_ECFOEHDR_OFFS_ENTIRE: max busy progress (ECAT_FOE_OPCODE_BUSY) */
        } EC_PACKED(1) sStatus;
    } EC_PACKED(1) uHdr;
/*  union */
/*  { */
/*      EC_T_CHAR       Name[]      (ECAT_FOE_OPCODE_RRQ, ECAT_FOE_OPCODE_WRQ)  rest of mailbox data */
/*      EC_T_BYTE       Data[]      (ECAT_FOE_OPCODE_DATA)                      rest of mailbox data */
/*      EC_T_CHAR       ErrorText[] (ECAT_FOE_OPCODE_ERR)                       rest of mailbox data */
/*      EC_T_CHAR       BusyText[]  (ECAT_FOE_OPCODE_BUSY)                      rest of mailbox data */
/*  }; */
} EC_PACKED(1) EC_FOE_HDR, *PEC_FOE_HDR;
#define EC_FOE_HDR_LEN sizeof(EC_FOE_HDR)
#define FOEHDR_CLR(pFoe) OsDbgAssert(sizeof(*pFoe)==(EC_FOE_HDR_LEN)); OsMemset(pFoe,0,(EC_FOE_HDR_LEN))

static EC_INLINESTART EC_T_BYTE EC_ECFOEHDR_GET_OPCODE(PEC_FOE_HDR p)
{
    return p->OpCode;
} EC_INLINESTOP
static EC_INLINESTART EC_T_DWORD EC_ECFOEHDR_GET_PASSWORD(PEC_FOE_HDR p)
{
    return EC_GET_FRM_DWORD((((EC_T_PBYTE)p) + EC_ECFOEHDR_OFFS_UHDR));
} EC_INLINESTOP
static EC_INLINESTART EC_T_DWORD EC_ECFOEHDR_GET_PACKETNO(PEC_FOE_HDR p)
{
    return EC_GET_FRM_DWORD((((EC_T_PBYTE)p) + EC_ECFOEHDR_OFFS_UHDR));
} EC_INLINESTOP
static EC_INLINESTART EC_T_DWORD EC_ECFOEHDR_GET_ERRORCODE(PEC_FOE_HDR p)
{
    return EC_GET_FRM_DWORD((((EC_T_PBYTE)p) + EC_ECFOEHDR_OFFS_UHDR));
} EC_INLINESTOP
static EC_INLINESTART EC_T_WORD EC_ECFOEHDR_GET_STATUSDONE(PEC_FOE_HDR p)
{
    return EC_GET_FRM_WORD((((EC_T_PBYTE)p) + EC_ECFOEHDR_OFFS_UHDR));
} EC_INLINESTOP
static EC_INLINESTART EC_T_WORD EC_ECFOEHDR_GET_STATUSENTIRE(PEC_FOE_HDR p)
{
    return EC_GET_FRM_WORD((((EC_T_PBYTE)p) + EC_ECFOEHDR_OFFS_ENTIRE));
} EC_INLINESTOP
static EC_INLINESTART EC_T_CHAR* EC_ECFOEHDR_GET_BUSYCOMMENT(PEC_FOE_HDR p)
{
    return ((EC_T_CHAR*)p) + EC_ECFOEHDR_OFFS_BUSY_TEXT;
} EC_INLINESTOP
static EC_INLINESTART EC_T_VOID EC_ECFOEHDR_SET_OPCODE(PEC_FOE_HDR p, EC_T_BYTE byVal)
{
    p->OpCode = byVal;
} EC_INLINESTOP
static EC_INLINESTART EC_T_VOID EC_ECFOEHDR_SET_PASSWORD(PEC_FOE_HDR p, EC_T_DWORD dwVal)
{
    EC_SET_FRM_DWORD((((EC_T_PBYTE)p) + EC_ECFOEHDR_OFFS_UHDR),dwVal);
} EC_INLINESTOP
static EC_INLINESTART EC_T_VOID EC_ECFOEHDR_SET_PACKETNO(PEC_FOE_HDR p, EC_T_DWORD dwVal)
{
    EC_SET_FRM_DWORD((((EC_T_PBYTE)p) + EC_ECFOEHDR_OFFS_UHDR),dwVal);
} EC_INLINESTOP
static EC_INLINESTART EC_T_VOID EC_ECFOEHDR_SET_ERRORCODE(PEC_FOE_HDR p, EC_T_DWORD dwVal)
{
    EC_SET_FRM_DWORD((((EC_T_PBYTE)p) + EC_ECFOEHDR_OFFS_UHDR),dwVal);
} EC_INLINESTOP

#define EC_ECFOEBUSYINFO_OFFS_DONE      ((EC_T_BYTE)0)
#define EC_ECFOEBUSYINFO_OFFS_ENTIRE    ((EC_T_BYTE)2)

typedef struct TETHERCAT_FOE_BUSY_INFO
{
    EC_T_WORD   __Done;         /* 0 */
    EC_T_WORD   __Entire;       /* 2 */
    EC_T_CHAR   Comment[32];    /* 4 */
} EC_PACKED(1) ETHERCAT_FOE_BUSY_INFO, *PETHERCAT_FOE_BUSY_INFO;
#define ETHERCAT_FOE_BUSY_INFO_LEN      sizeof(ETHERCAT_FOE_BUSY_INFO)

static EC_INLINESTART EC_T_WORD EC_ECFOEBUSYINFO_GET_STATUSDONE(PETHERCAT_FOE_BUSY_INFO p)
{
    return EC_GET_FRM_WORD((((EC_T_PBYTE)p) + EC_ECFOEBUSYINFO_OFFS_DONE));
} EC_INLINESTOP
static EC_INLINESTART EC_T_WORD EC_ECFOEBUSYINFO_GET_STATUSENTIRE(PETHERCAT_FOE_BUSY_INFO p)
{
    return EC_GET_FRM_WORD((((EC_T_PBYTE)p) + EC_ECFOEBUSYINFO_OFFS_ENTIRE));
} EC_INLINESTOP
static EC_INLINESTART EC_T_VOID EC_ECFOEBUSYINFO_SET_STATUSDONE(PETHERCAT_FOE_BUSY_INFO p, EC_T_WORD wVal)
{
    EC_SET_FRM_WORD((((EC_T_PBYTE)p) + EC_ECFOEBUSYINFO_OFFS_DONE), wVal);
} EC_INLINESTOP
static EC_INLINESTART EC_T_VOID EC_ECFOEBUSYINFO_SET_STATUSENTIRE(PETHERCAT_FOE_BUSY_INFO p, EC_T_WORD wVal)
{
    EC_SET_FRM_WORD((((EC_T_PBYTE)p) + EC_ECFOEBUSYINFO_OFFS_ENTIRE), wVal);
} EC_INLINESTOP


/*---------------------------------------------------------------------------*/
/* SoE (Servo Profile over EtherCAT) */

#ifdef INCLUDE_SOE_SUPPORT

#define ECAT_SOE_OPCODE_RRQ         1   /*Read request*/
#define ECAT_SOE_OPCODE_RRS         2   /*Read response*/
#define ECAT_SOE_OPCODE_WRQ         3   /*Write request*/
#define ECAT_SOE_OPCODE_WRS         4   /*Write response*/
#define ECAT_SOE_OPCODE_NOTIFY      5   /*Notification*/
#define ECAT_SOE_OPCODE_SLV_INFO    6   /*Slave Info*/
#define ECAT_SOE_OPCODE_RESERVED    7

#define ECAT_SOE_ERRCODE_INVALID_ACCESS     0x0009 /* "Invalid access to element 0" */ 
#define ECAT_SOE_ERRCODE_NOT_EXIST          0x1001 /* "Does not exist" */ 
#define ECAT_SOE_ERRCODE_INVL_ACC_ELEM1     0x1009 /* "Invalid access to element 1" */ 
#define ECAT_SOE_ERRCODE_NAME_NOT_EXIST     0x2001 /* "Name does not exist" */ 
#define ECAT_SOE_ERRCODE_NAME_UNDERSIZE     0x2002 /* "Name undersize in transmission" */ 
#define ECAT_SOE_ERRCODE_NAME_OVERSIZE      0x2003 /* "Name oversize in transmission" */ 
#define ECAT_SOE_ERRCODE_NAME_UNCHANGE      0x2004 /* "Name unchangeable" */ 
#define ECAT_SOE_ERRCODE_NAME_WR_PROT       0x2005 /* "Name currently write-protected" */ 
#define ECAT_SOE_ERRCODE_UNDERS_TRANS       0x3002 /* "Attribute undersize in transmission" */ 
#define ECAT_SOE_ERRCODE_OVERS_TRANS        0x3003 /* "Attribute oversize in transmission" */ 
#define ECAT_SOE_ERRCODE_ATTR_UNCHANGE      0x3004 /* "Attribute unchangeable" */ 
#define ECAT_SOE_ERRCODE_ATTR_WR_PROT       0x3005 /* "Attribute currently write-protected" */ 
#define ECAT_SOE_ERRCODE_UNIT_NOT_EXIST     0x4001 /* "Unit does not exist" */ 
#define ECAT_SOE_ERRCODE_UNIT_UNDERSIZE     0x4002 /* "Unit undersize in transmission" */ 
#define ECAT_SOE_ERRCODE_UNIT_OVERSIZE      0x4003 /* "Unit oversize in transmission" */ 
#define ECAT_SOE_ERRCODE_UNIT_UNCHANGE      0x4004 /* "Unit unchangeable" */ 
#define ECAT_SOE_ERRCODE_UNIT_WR_PROT       0x4005 /* "Unit currently write-protected" */ 
#define ECAT_SOE_ERRCODE_MIN_NOT_EXIST      0x5001 /* "Minimum input value does not exist" */ 
#define ECAT_SOE_ERRCODE_MIN_UNDERSIZE      0x5002 /* "Minimum input value undersize in transmission" */ 
#define ECAT_SOE_ERRCODE_MIN_OVERSIZE       0x5003 /* "Minimum input value oversize in transmission" */ 
#define ECAT_SOE_ERRCODE_MIN_UNCHANGE       0x5004 /* "Minimum input value unchangeable" */ 
#define ECAT_SOE_ERRCODE_MIN_WR_PROT        0x5005 /* "Minimum input value currently write-protected" */ 
#define ECAT_SOE_ERRCODE_MAX_NOT_EXIST      0x6001 /* "Maximum input value does not exist" */ 
#define ECAT_SOE_ERRCODE_MAX_UNDERSIZE      0x6002 /* "Maximum input value undersize in transmission" */ 
#define ECAT_SOE_ERRCODE_MAX_OVERSIZE       0x6003 /* "Maximum input value oversize in transmission" */ 
#define ECAT_SOE_ERRCODE_MAX_UNCHANGE       0x6004 /* "Maximum input value unchangeable" */ 
#define ECAT_SOE_ERRCODE_MAX_WR_PROT        0x6005 /* "Maximum input value currently write-protected" */ 
#define ECAT_SOE_ERRCODE_DATA_NOT_EXIST     0x7001 /* "Data item does not exist" */ 
#define ECAT_SOE_ERRCODE_DATA_UNDERSIZE     0x7002 /* "Data item undersize in transmission" */ 
#define ECAT_SOE_ERRCODE_DATA_OVERSIZE      0x7003 /* "Data item oversize in transmission" */ 
#define ECAT_SOE_ERRCODE_DATA_UNCHANGE      0x7004 /* "Data item unchangeable" */ 
#define ECAT_SOE_ERRCODE_DATA_WR_PROT       0x7005 /* "Data item currently write-protected" */ 
#define ECAT_SOE_ERRCODE_DATA_MIN_LIMIT     0x7006 /* "Data item less than minimum input value limit" */
#define ECAT_SOE_ERRCODE_DATA_MAX_LIMIT     0x7007 /* "Data item exceeds maximum input value limit" */
#define ECAT_SOE_ERRCODE_DATA_INCOR         0x7008 /* "Data item is incorrect" */ 
#define ECAT_SOE_ERRCODE_PASWD_PROT         0x7009 /* "Data item is protected by password" */ 
#define ECAT_SOE_ERRCODE_TEMP_UNCHANGE      0x700A /* "Data item temporary unchangeable (in AT or MDT)" */ 
#define ECAT_SOE_ERRCODE_INVL_INDIRECT      0x700B /* "Invalid indirect" */ 
#define ECAT_SOE_ERRCODE_TEMP_UNCHANGE1     0x700C /* "Data item temporary unchangeable (parameter or opmode...)" */ 
#define ECAT_SOE_ERRCODE_ALREADY_ACTIVE     0x7010 /* "Command already active" */ 
#define ECAT_SOE_ERRCODE_NOT_INTERRUPT      0x7011 /* "Command not interruptable" */ 
#define ECAT_SOE_ERRCODE_CMD_NOT_AVAIL      0x7012 /* "Command not available (in this phase)" */ 
#define ECAT_SOE_ERRCODE_CMD_NOT_AVAIL1     0x7013 /* "Command not available (invalid parameter...)" */ 
#define ECAT_SOE_ERRCODE_NO_DATA            0x7014 /* "No data state"*/
#define ECAT_SOE_ERRCODE_NO_DEFAULT_VALUE   0x8001 /* "No default value */
#define ECAT_SOE_ERRCODE_DEFAULT_LONG       0x8002 /* "Default value transmission too long" */
#define ECAT_SOE_ERRCODE_DEFAULT_WP         0x8004 /* "Default value cannot be changed, read only." */
#define ECAT_SOE_ERRCODE_INVL_DRIVE_NO      0x800A /* "Invalid drive number" */
#define ECAT_SOE_ERRCODE_GENERAL_ERROR      0x800B /* "General error" */
#define ECAT_SOE_ERRCODE_NO_ELEM_ADR        0x800C /* "No element addressed" */




#define EC_ECSOEHDR_OFFS_IDN      ((EC_T_BYTE)2)
#define EC_ECSOEHDR_OFFS_FRAGLEFT ((EC_T_BYTE)2)
typedef struct TEC_SOE_HDR
{
/* EC_NO_BITFIELDS is always set for EC_BIG_ENDIAN, but the SET/GET macros for the Bit Field definitions below do not exist or are not used */
#if (!defined EC_NO_BITFIELDS) || (defined EC_BIG_ENDIAN)
  #ifdef EC_BIG_ENDIAN
    EC_T_BYTE       DriveNo     : 3;
    EC_T_BYTE       Error       : 1;
    EC_T_BYTE       Incomplete  : 1;
    EC_T_BYTE       OpCode      : 3;
  #else
    EC_T_BYTE       OpCode      : 3;
    EC_T_BYTE       Incomplete  : 1;
    EC_T_BYTE       Error       : 1;
    EC_T_BYTE       DriveNo     : 3;
  #endif
#else
    EC_T_BYTE       byOpCodeIncompleteErrorDriveNo;
#endif /* EC_NO_BITFIELDS && !EC_BIG_ENDIAN */
    EC_T_BYTE       byElements;
    union _t_uIdnFragLeft
    {
        EC_T_WORD   wIdn;
        EC_T_WORD   wFragmentsLeft;
    } EC_PACKED(1) uIdnFragLeft;
} EC_PACKED(1) EC_SOE_HDR, *PEC_SOE_HDR;


typedef struct TETHERCAT_SOE_EMERGENCY_HEADER
{
    EC_T_BYTE   Data[64];
} EC_PACKED(1) ETHERCAT_SOE_EMERGENCY_HEADER, *PETHERCAT_SOE_EMERGENCY_HEADER;

typedef struct TETHERCAT_SOE_NOTIFICATION_HEADER
{
    EC_T_WORD   DataStatus;
} EC_PACKED(1) ETHERCAT_SOE_NOTIFICATION_HEADER, *PETHERCAT_SOE_NOTIFICATION_HEADER;

/* definition of bitmask for above element flags */
#define SOE_BM_ELEMENTFLAG_DATATSTATE   0x01
#define SOE_BM_ELEMENTFLAG_NAME         0x02
#define SOE_BM_ELEMENTFLAG_ATTRIBUTE    0x04
#define SOE_BM_ELEMENTFLAG_UNIT         0x08
#define SOE_BM_ELEMENTFLAG_MIN          0x10
#define SOE_BM_ELEMENTFLAG_MAX          0x20
#define SOE_BM_ELEMENTFLAG_VALUE        0x40
#define SOE_BM_ELEMENTFLAG_DEFAULT      0x80

#define SOE_HDR_IDN_OFFSET              (sizeof(EC_T_BYTE)+sizeof(EC_T_BYTE))                   /* OpCode/... + DataState/... */
#define SOE_HDR_DATA_OFFSET             (sizeof(EC_T_BYTE)+sizeof(EC_T_BYTE)+sizeof(EC_T_WORD)) /* OpCode/... + DataState/... + IDN/FragLeft */
#define EC_SOE_HDR_LEN         sizeof(EC_SOE_HDR)
#define ETHERCAT_MIN_SOE_MBOX_LEN       (ETHERCAT_MBOX_HEADER_LEN + EC_SOE_HDR_LEN)
#define SOEHDR_CLR(pSoe) OsDbgAssert(sizeof(*pSoe)==EC_SOE_HDR_LEN); OsMemset(pSoe,0,EC_SOE_HDR_LEN)

static EC_INLINESTART EC_T_WORD EC_ECSOEHDR_GET_IDN(PEC_SOE_HDR p)
{
    return EC_GET_FRM_WORD((((EC_T_PBYTE)p) + EC_ECSOEHDR_OFFS_IDN));
} EC_INLINESTOP
static EC_INLINESTART EC_T_WORD EC_ECSOEHDR_GET_FRAGLEFT(PEC_SOE_HDR p)
{
    return EC_GET_FRM_WORD((((EC_T_PBYTE)p) + EC_ECSOEHDR_OFFS_FRAGLEFT));
} EC_INLINESTOP
static EC_INLINESTART EC_T_DWORD EC_ECSOEHDR_GET_ERRORCODE(PEC_SOE_HDR p)
{
    return EC_GET_FRM_WORD((((EC_T_PBYTE)p)+SOE_HDR_DATA_OFFSET));
} EC_INLINESTOP

static EC_INLINESTART EC_T_VOID EC_ECSOEHDR_SET_IDN(PEC_SOE_HDR p, EC_T_WORD wVal)
{
    EC_SET_FRM_WORD((((EC_T_PBYTE)p) + EC_ECSOEHDR_OFFS_IDN), wVal);
} EC_INLINESTOP
static EC_INLINESTART EC_T_VOID EC_ECSOEHDR_SET_FRAGLEFT(PEC_SOE_HDR p, EC_T_WORD wVal)
{
    EC_SET_FRM_WORD((((EC_T_PBYTE)p) + EC_ECSOEHDR_OFFS_FRAGLEFT), wVal);
} EC_INLINESTOP
#endif /* #ifdef INCLUDE_SOE_SUPPORT */

#ifdef INCLUDE_AOE_SUPPORT
/*---------------------------------------------------------------------------*/
/* AoE (ADS over EtherCAT) */
#define ECAT_AOEHDR_COMMANDID_READ             0x002
#define ECAT_AOEHDR_COMMANDID_WRITE            0x003
#define ECAT_AOEHDR_COMMANDID_WRITECONTROL     0x005
#define ECAT_AOEHDR_COMMANDID_READWRITE        0x009
#define ECAT_AOEHDR_COMMANDID_FRAGMENTATION    0x902

/*---------------------------------------------------------------------------*/
/* Global error codes */
#define ADSERR_NOERROR                          0 	/*No error*/
#define ADSERR_DEVICE_INTERNAL                  1 	/*Internal error */
/*#define ADSERR_DEVICE                         2 	// No Rtime */
/*#define ADSERR_DEVICE                         3 	// Allocation locked memory error */
/*#define ADSERR_DEVICE                         4 	// Insert mailbox error */
/*#define ADSERR_DEVICE                         5 	// Wrong receive HMSG */
#define ADSERR_DEVICE_TARGET_PORT_NOT_FOUND     6   /* target port not found */
#define ADSERR_DEVICE_TARGET_MACHINE_NOT_FOUND  7 	/*	target machine not found */
#define ADSERR_DEVICE_UNKNOWN_CMD_ID            8 	/*	Unknown command ID */
/* #define ADSERR_DEVICE                        9 	// Bad task ID */
/* #define ADSERR_DEVICE                        10 	// No IO */
/* #define ADSERR_DEVICE                        11 	// Unknown AMS command */
/* #define ADSERR_DEVICE                        12 	// Win 32 error */
#define ADSERR_DEVICE_PORT_NOT_CONNECTED        13 	/* Port not connected */
#define ADSERR_DEVICE_INVALID_AMS_LENGTH        14 	/* Invalid AMS length */
#define ADSERR_DEVICE_INVALID_AMS_ID            15 	/* Invalid AMS Net ID */
/* #define ADSERR_DEVICE                        16 	// Low Installation level */
/* #define ADSERR_DEVICE                        17 	// No debug available */
#define ADSERR_DEVICE_PORT_DISABLED             18 	/* Port disabled */
#define ADSERR_DEVICE_PORT_CONNECTED            19 	/* Port already connected */
/* #define ADSERR_DEVICE                        20 	// AMS Sync Win32 error */
/* #define ADSERR_DEVICE                        21 	// AMS Sync Timeout */
/* #define ADSERR_DEVICE                        22 	// AMS Sync AMS error */
/* #define ADSERR_DEVICE                        23 	// AMS Sync no index map */
#define ADSERR_DEVICE_INVALID_AMS_PORT          24 	/* Invalid AMS port */
#define ADSERR_DEVICE_NO_MEMORY                 25 	/* No memory */
/* #define ADSERR_DEVICE                        26 	// TCP send error */
/* #define ADSERR_DEVICE                        27 	// Host unreachable */
#define ADSERR_DEVICE_INVALIDAMSFRAGMENT        28 	/* invalid ams fragment */


/* General ADS error codes */
#define	ERR_ADSERRS					     0x0700
#define ADSERR_DEVICE_ERROR				(0x00 + ERR_ADSERRS) /* Error class < device error > */
#define ADSERR_DEVICE_SRVNOTSUPP		(0x01 + ERR_ADSERRS) /* Service is not supported by server */
#define ADSERR_DEVICE_INVALIDGRP 		(0x02 + ERR_ADSERRS) /* invalid indexGroup */
#define ADSERR_DEVICE_INVALIDOFFSET		(0x03 + ERR_ADSERRS) /* invalid indexOffset */
#define ADSERR_DEVICE_INVALIDACCESS		(0x04 + ERR_ADSERRS) /* reading/writing not permitted */
#define ADSERR_DEVICE_INVALIDSIZE		(0x05 + ERR_ADSERRS) /* parameter size not correct */
#define ADSERR_DEVICE_INVALIDDATA		(0x06 + ERR_ADSERRS) /* invalid parameter value(s) */
#define ADSERR_DEVICE_NOTREADY			(0x07 + ERR_ADSERRS) /* device is not in a ready state */
#define ADSERR_DEVICE_BUSY				(0x08 + ERR_ADSERRS) /* device is busy */
#define ADSERR_DEVICE_INVALIDCONTEXT	(0x09 + ERR_ADSERRS) /* invalid context (must be InWindows) */
#define ADSERR_DEVICE_NOMEMORY			(0x0A + ERR_ADSERRS) /* out of memory */
#define ADSERR_DEVICE_INVALIDPARM		(0x0B + ERR_ADSERRS) /* invalid parameter value(s) */
#define ADSERR_DEVICE_NOTFOUND			(0x0C + ERR_ADSERRS) /* not found (files, ...) */
#define ADSERR_DEVICE_SYNTAX			(0x0D + ERR_ADSERRS) /* syntax error in comand or file */
#define ADSERR_DEVICE_INCOMPATIBLE		(0x0E  +  ERR_ADSERRS) /* objects do not match */
#define ADSERR_DEVICE_EXISTS			(0x0F + ERR_ADSERRS) /* object already exists */
#define ADSERR_DEVICE_SYMBOLNOTFOUND	(0x10 + ERR_ADSERRS) /* symbol not found */
#define ADSERR_DEVICE_SYMBOLVERSIONINVALID	(0x11 + ERR_ADSERRS) /* symbol version invalid */
#define ADSERR_DEVICE_INVALIDSTATE		(0x12 + ERR_ADSERRS) /* server is in invalid state */
#define ADSERR_DEVICE_TRANSMODENOTSUPP	(0x13 + ERR_ADSERRS) /* AdsTransMode not supported */
#define ADSERR_DEVICE_NOTIFYHNDINVALID	(0x14 + ERR_ADSERRS) /* Notification handle is invalid */
#define ADSERR_DEVICE_CLIENTUNKNOWN		(0x15 + ERR_ADSERRS) /* Notification client not registered	 */
#define ADSERR_DEVICE_NOMOREHDLS		(0x16 + ERR_ADSERRS) /* no more notification handles	 */
#define ADSERR_DEVICE_INVALIDWATCHSIZE	(0x17 + ERR_ADSERRS) /* size for watch to big	 */
#define ADSERR_DEVICE_NOTINIT			(0x18 + ERR_ADSERRS) /* device not initialized	 */
#define ADSERR_DEVICE_TIMEOUT			(0x19 + ERR_ADSERRS) /* device has a timeout */
#define ADSERR_DEVICE_NOINTERFACE		(0x1A + ERR_ADSERRS) /* query interface failed */
#define ADSERR_DEVICE_INVALIDINTERFACE	(0x1B + ERR_ADSERRS) /* wrong interface required */
#define ADSERR_DEVICE_INVALIDCLSID		(0x1C + ERR_ADSERRS) /* class ID is invalid */
#define ADSERR_DEVICE_INVALIDOBJID		(0x1D + ERR_ADSERRS) /* object ID is invalid */
#define ADSERR_DEVICE_PENDING			(0x1E  +  ERR_ADSERRS) /* request is pending */
#define ADSERR_DEVICE_ABORTED			(0x1F + ERR_ADSERRS) /* request is aborted */
#define ADSERR_DEVICE_WARNING			(0x20 + ERR_ADSERRS) /* signal warning */
#define ADSERR_DEVICE_INVALIDARRAYIDX	(0x21 + ERR_ADSERRS) /* invalid array index */
#define ADSERR_DEVICE_SYMBOLNOTACTIVE	(0x22 + ERR_ADSERRS) /* symbol not active -> release handle and try again */
#define ADSERR_DEVICE_ACCESSDENIED		(0x23 + ERR_ADSERRS) /* access denied */
#define ADSERR_CLIENT_ERROR				(0x40 + ERR_ADSERRS) /* Error class < client error > */
#define ADSERR_CLIENT_INVALIDPARM		(0x41 + ERR_ADSERRS) /* invalid parameter at service call */
#define ADSERR_CLIENT_LISTEMPTY			(0x42 + ERR_ADSERRS) /* polling list	is empty */
#define ADSERR_CLIENT_VARUSED			(0x43 + ERR_ADSERRS) /* var connection already in use */
#define ADSERR_CLIENT_DUPLINVOKEID		(0x44 + ERR_ADSERRS) /* invoke id in use */
#define ADSERR_CLIENT_SYNCTIMEOUT		(0x45 + ERR_ADSERRS) /* timeout elapsed */
#define ADSERR_CLIENT_W32ERROR			(0x46 + ERR_ADSERRS) /* error in win32 subsystem */
#define ADSERR_CLIENT_TIMEOUTINVALID	(0x47 + ERR_ADSERRS) /* ? */
#define ADSERR_CLIENT_PORTNOTOPEN		(0x48 + ERR_ADSERRS) /* ads dll */
#define ADSERR_CLIENT_NOAMSADDR			(0x49 + ERR_ADSERRS) /* ads dll  */
#define ADSERR_CLIENT_SYNCINTERNAL		(0x50 + ERR_ADSERRS) /* internal error in ads sync  */
#define ADSERR_CLIENT_ADDHASH			(0x51 + ERR_ADSERRS) /* hash table overflow  */
#define ADSERR_CLIENT_REMOVEHASH		(0x52 + ERR_ADSERRS) /* key not found in hash table  */
#define ADSERR_CLIENT_NOMORESYM			(0x53 + ERR_ADSERRS) /* no more symbols in cache  */
#define ADSERR_CLIENT_SYNCRESINVALID	(0x54 + ERR_ADSERRS) /* invalid response received  */
#define ADSERR_CLIENT_SYNCPORTLOCKED	(0x55 + ERR_ADSERRS) /* sync port is locked  */


typedef struct _EC_AOE_HDR
{ 
	EC_T_BYTE       oTargetNetId[6];    /*  0: destination NetID */
    EC_T_WORD       wTargetPort;        /*  6: destination port */
	EC_T_BYTE       oSenderNetId[6];    /*  8: source NetID */
    EC_T_WORD	    wSenderPort;	    /* 14: source port */
    EC_T_WORD       wCmdId;			    /* 16: command id  */
	EC_T_WORD       wStateFlags;        /* 18: state flags */
	EC_T_DWORD	    dwDataSize;         /* 20: count bytes for actual command (excl. AmsHead) */
    EC_T_DWORD      dwErrorCode;        /* 24: error code (only for responses) */
    EC_T_DWORD      dwAoeInvokeId;      /* 28: will be copied unchanged from request to response */
} EC_PACKED(1) EC_AOE_HDR, *PEC_AOE_HDR;
#define EC_AOE_HDR_OFFS_TARGET_NET_ID  ((EC_T_BYTE)0)
#define EC_AOE_HDR_OFFS_TARGET_PORT    ((EC_T_BYTE)6)
#define EC_AOE_HDR_OFFS_SENDER_NET_ID  ((EC_T_BYTE)8)
#define EC_AOE_HDR_OFFS_SENDER_PORT    ((EC_T_BYTE)14)
#define EC_AOE_HDR_OFFS_CMD_ID         ((EC_T_BYTE)16)
#define EC_AOE_HDR_OFFS_STATE_FLAGS    ((EC_T_BYTE)18)
#define EC_AOE_HDR_OFFS_DATA_SIZE      ((EC_T_BYTE)20)
#define EC_AOE_HDR_OFFS_ERROR_CODE     ((EC_T_BYTE)24)
#define EC_AOE_HDR_OFFS_INVOKE_ID      ((EC_T_BYTE)28)

static EC_INLINESTART EC_T_WORD EC_AOE_HDR_GET_TARGET_PORT(EC_AOE_HDR* p)
{
    return EC_GET_FRM_WORD((((EC_T_PBYTE)p) + EC_AOE_HDR_OFFS_TARGET_PORT));
} EC_INLINESTOP
static EC_INLINESTART EC_T_WORD EC_AOE_HDR_GET_SENDER_PORT(EC_AOE_HDR* p)
{
    return EC_GET_FRM_WORD((((EC_T_PBYTE)p) + EC_AOE_HDR_OFFS_SENDER_PORT));
} EC_INLINESTOP
static EC_INLINESTART EC_T_WORD EC_AOE_HDR_GET_CMD_ID(EC_AOE_HDR* p)
{
    return EC_GET_FRM_WORD((((EC_T_PBYTE)p) + EC_AOE_HDR_OFFS_CMD_ID));
} EC_INLINESTOP
static EC_INLINESTART EC_T_WORD EC_AOE_HDR_GET_STATE_FLAGS(EC_AOE_HDR* p)
{
    return EC_GET_FRM_WORD((((EC_T_PBYTE)p) + EC_AOE_HDR_OFFS_STATE_FLAGS));
} EC_INLINESTOP
static EC_INLINESTART EC_T_DWORD EC_AOE_HDR_GET_DATA_SIZE(EC_AOE_HDR* p)
{
    return EC_GET_FRM_DWORD((((EC_T_PBYTE)p) + EC_AOE_HDR_OFFS_DATA_SIZE));
} EC_INLINESTOP
static EC_INLINESTART EC_T_DWORD EC_AOE_HDR_GET_ERROR_CODE(EC_AOE_HDR* p)
{
    return EC_GET_FRM_DWORD((((EC_T_PBYTE)p) + EC_AOE_HDR_OFFS_ERROR_CODE));
} EC_INLINESTOP
static EC_INLINESTART EC_T_DWORD EC_AOE_HDR_GET_INVOKE_ID(EC_AOE_HDR* p)
{
    return EC_GET_FRM_DWORD((((EC_T_PBYTE)p) + EC_AOE_HDR_OFFS_INVOKE_ID));
} EC_INLINESTOP

static EC_INLINESTART EC_T_VOID EC_AOE_HDR_SET_TARGET_PORT(EC_AOE_HDR* p, EC_T_WORD wVal)
{
    EC_SET_FRM_WORD((((EC_T_PBYTE)p) + EC_AOE_HDR_OFFS_TARGET_PORT), wVal);
} EC_INLINESTOP
static EC_INLINESTART EC_T_VOID EC_AOE_HDR_SET_SENDER_PORT(EC_AOE_HDR* p, EC_T_WORD wVal)
{
    EC_SET_FRM_WORD((((EC_T_PBYTE)p) + EC_AOE_HDR_OFFS_SENDER_PORT), wVal);
} EC_INLINESTOP
static EC_INLINESTART EC_T_VOID EC_AOE_HDR_SET_CMD_ID(EC_AOE_HDR* p, EC_T_WORD wVal)
{
    EC_SET_FRM_WORD((((EC_T_PBYTE)p) + EC_AOE_HDR_OFFS_CMD_ID), wVal);
} EC_INLINESTOP
static EC_INLINESTART EC_T_VOID EC_AOE_HDR_SET_STATE_FLAGS(EC_AOE_HDR* p, EC_T_WORD wVal)
{
    EC_SET_FRM_WORD((((EC_T_PBYTE)p) + EC_AOE_HDR_OFFS_STATE_FLAGS), wVal);
} EC_INLINESTOP
static EC_INLINESTART EC_T_VOID EC_AOE_HDR_SET_DATA_SIZE(EC_AOE_HDR* p, EC_T_DWORD dwVal)
{
    EC_SET_FRM_DWORD((((EC_T_PBYTE)p) + EC_AOE_HDR_OFFS_DATA_SIZE), dwVal);
} EC_INLINESTOP
static EC_INLINESTART EC_T_VOID EC_AOE_HDR_SET_ERROR_CODE(EC_AOE_HDR* p, EC_T_DWORD dwVal)
{
    EC_SET_FRM_DWORD((((EC_T_PBYTE)p) + EC_AOE_HDR_OFFS_ERROR_CODE), dwVal);
} EC_INLINESTOP
static EC_INLINESTART EC_T_VOID EC_AOE_HDR_SET_INVOKE_ID(EC_AOE_HDR* p, EC_T_DWORD dwVal)
{
    EC_SET_FRM_DWORD((((EC_T_PBYTE)p) + EC_AOE_HDR_OFFS_INVOKE_ID), dwVal);
} EC_INLINESTOP

/* Command ID 2 (Read request) */
typedef struct _ETHERCAT_AOE_READ_REQ_HEADER
{ 
    EC_T_DWORD dwIndexGroup;
    EC_T_DWORD dwIndexOffset;
    EC_T_DWORD dwReadLength;
} EC_PACKED(1) ETHERCAT_AOE_READ_REQ_HEADER, *PETHERCAT_AOE_READ_REQ_HEADER;

/* Command ID 2 (Read response) */
typedef struct _ETHERCAT_AOE_READ_RES_HEADER
{ 
    EC_T_DWORD dwResult;
    EC_T_DWORD dwReadLength;
    /* EC_T_BYTE  dwReadData[1]; */
} EC_PACKED(1) ETHERCAT_AOE_READ_RES_HEADER, *PETHERCAT_AOE_READ_RES_HEADER;


/* Command ID 3 (Write request) */
typedef struct _ETHERCAT_AOE_WRITE_REQ_HEADER
{ 
    EC_T_DWORD dwIndexGroup;            /* 0 */
    EC_T_DWORD dwIndexOffset;           /* 4 */
    EC_T_DWORD dwWriteLength;           /* 8 */
    /* EC_T_BYTE  dwWriteData[1]; */    /* 12 */
} EC_PACKED(1) ETHERCAT_AOE_WRITE_REQ_HEADER, *PETHERCAT_AOE_WRITE_REQ_HEADER;
#define ETHERCAT_AOE_WRITE_REQ_HEADER_OFFS_INDEX_GROUP  ((EC_T_BYTE)0)
#define ETHERCAT_AOE_WRITE_REQ_HEADER_OFFS_INDEX_OFFSET ((EC_T_BYTE)4)
#define ETHERCAT_AOE_WRITE_REQ_HEADER_OFFS_WRITE_LENGTH ((EC_T_BYTE)8)
#define ETHERCAT_AOE_WRITE_REQ_HEADER_OFFS_WRITE_DATA   ((EC_T_BYTE)12)
static EC_INLINESTART EC_T_DWORD ETHERCAT_AOE_WRITE_REQ_HEADER_GET_INDEX_GROUP(PETHERCAT_AOE_WRITE_REQ_HEADER p)
{
    return EC_GET_FRM_DWORD((((EC_T_PBYTE)p) + ETHERCAT_AOE_WRITE_REQ_HEADER_OFFS_INDEX_GROUP));
} EC_INLINESTOP
static EC_INLINESTART EC_T_DWORD ETHERCAT_AOE_WRITE_REQ_HEADER_GET_INDEX_OFFSET(PETHERCAT_AOE_WRITE_REQ_HEADER p)
{
    return EC_GET_FRM_DWORD((((EC_T_PBYTE)p) + ETHERCAT_AOE_WRITE_REQ_HEADER_OFFS_INDEX_OFFSET));
} EC_INLINESTOP
static EC_INLINESTART EC_T_DWORD ETHERCAT_AOE_WRITE_REQ_HEADER_GET_WRITE_LENGTH(PETHERCAT_AOE_WRITE_REQ_HEADER p)
{
    return EC_GET_FRM_DWORD((((EC_T_PBYTE)p) + ETHERCAT_AOE_WRITE_REQ_HEADER_OFFS_WRITE_LENGTH));
} EC_INLINESTOP

static EC_INLINESTART EC_T_VOID ETHERCAT_AOE_WRITE_REQ_HEADER_SET_INDEX_GROUP(PETHERCAT_AOE_WRITE_REQ_HEADER p, EC_T_DWORD dwVal)
{
    EC_SET_FRM_DWORD((((EC_T_PBYTE)p) + ETHERCAT_AOE_WRITE_REQ_HEADER_OFFS_INDEX_GROUP), dwVal);
} EC_INLINESTOP
static EC_INLINESTART EC_T_VOID ETHERCAT_AOE_WRITE_REQ_HEADER_SET_INDEX_OFFSET(PETHERCAT_AOE_WRITE_REQ_HEADER p, EC_T_DWORD dwVal)
{
    EC_SET_FRM_DWORD((((EC_T_PBYTE)p) + ETHERCAT_AOE_WRITE_REQ_HEADER_OFFS_INDEX_OFFSET), dwVal);
} EC_INLINESTOP
static EC_INLINESTART EC_T_VOID ETHERCAT_AOE_WRITE_REQ_HEADER_SET_WRITE_LENGTH(PETHERCAT_AOE_WRITE_REQ_HEADER p, EC_T_DWORD dwVal)
{
    EC_SET_FRM_DWORD((((EC_T_PBYTE)p) + ETHERCAT_AOE_WRITE_REQ_HEADER_OFFS_WRITE_LENGTH), dwVal);
} EC_INLINESTOP

/* Command ID 3 (Write response) */
typedef struct _ETHERCAT_AOE_WRITE_RES_HEADER
{ 
    EC_T_DWORD dwResult;
} EC_PACKED(1) ETHERCAT_AOE_WRITE_RES_HEADER, *PETHERCAT_AOE_WRITE_RES_HEADER;

/* Command ID 5 (Write Control request) */
typedef struct _ETHERCAT_AOE_WRITECONTROL_REQ_HEADER
{ 
    EC_T_WORD  wAoEState;
    EC_T_WORD  wDeviceState;
    EC_T_DWORD dwWriteLength;
    /* EC_T_BYTE  dwWriteData[1]; */
} EC_PACKED(1) ETHERCAT_AOE_WRITECONTROL_REQ_HEADER, *PETHERCAT_AOE_WRITECONTROL_REQ_HEADER;

/* Command ID 5 (Write Control response) */
typedef struct _ETHERCAT_AOE_WRITECONTROL_RES_HEADER
{ 
    EC_T_DWORD dwResult;
} EC_PACKED(1) ETHERCAT_AOE_WRITECONTROL_RES_HEADER, *PETHERCAT_AOE_WRITECONTROL_RES_HEADER;

/* Command ID 9 (ReadWrite request) */
typedef struct _ETHERCAT_AOE_READWRITE_REQ_HEADER
{ 
    EC_T_DWORD dwIndexGroup;
    EC_T_DWORD dwIndexOffset;
    EC_T_DWORD dwReadLength;
    EC_T_DWORD dwWriteLength;
    /* EC_T_BYTE  dwWriteData[1]; */
} EC_PACKED(1) ETHERCAT_AOE_READWRITE_REQ_HEADER, *PETHERCAT_AOE_READWRITE_REQ_HEADER;

/* Command ID 9 (ReadWrite response) */
/* Response it equal to ETHERCAT_AOE_READ_RES_HEADER -> do not define a own one */

typedef struct _EC_AOE_CMD_HDR
{
    union _t_uCmd
    {
        ETHERCAT_AOE_READ_REQ_HEADER oReadReqHdr;
        ETHERCAT_AOE_WRITE_REQ_HEADER oWriteReqHdr;
        ETHERCAT_AOE_WRITECONTROL_REQ_HEADER oWriteControlReqHdr;
        ETHERCAT_AOE_READWRITE_REQ_HEADER oReadWriteReqHdr;
    } EC_PACKED(1) uCmd;
    EC_T_DWORD dwUsedLength;
} EC_PACKED(1) EC_AOE_CMD_HDR, *PEC_AOE_CMD_HDR;


#define EC_ECAOEHDR_STATEFLAG_REQ  0x0004
#define EC_ECAOEHDR_STATEFLAG_RES  0x0005

#define EC_AOE_HDR_LEN                           sizeof(EC_AOE_HDR)
#define ETHERCAT_AOE_READ_REQ_HEADER_LEN         sizeof(ETHERCAT_AOE_READ_REQ_HEADER)
#define ETHERCAT_AOE_WRITE_REQ_HEADER_LEN        sizeof(ETHERCAT_AOE_WRITE_REQ_HEADER)
#define ETHERCAT_AOE_READWRITE_REQ_HEADER_LEN    sizeof(ETHERCAT_AOE_READWRITE_REQ_HEADER)
#define ETHERCAT_AOE_WRITECONTROL_REQ_HEADER_LEN sizeof(ETHERCAT_AOE_WRITECONTROL_REQ_HEADER)

#define ETHERCAT_MIN_AOE_MBOX_LEN       (ETHERCAT_MBOX_HEADER_LEN + EC_AOE_HDR_LEN)
#define AOEHDR_CLR(pAoe) OsDbgAssert(sizeof(*pAoe)==(EC_AOE_HDR_LEN)); OsMemset(pAoe,0,(EC_AOE_HDR_LEN))

#endif /* INCLUDE_AOE_SUPPORT */

/*---------------------------------------------------------------------------*/
#define EC_ECMBXCMDDESC_OFFS_TRANSITION         ((EC_T_BYTE)0)
#define EC_ECMBXCMDDESC_OFFS_PROTOCOL           ((EC_T_BYTE)2)
#define EC_ECMBXCMDDESC_OFFS_DATALEN            ((EC_T_BYTE)4)
#define EC_ECMBXCMDDESC_OFFS_CMTLEN             ((EC_T_BYTE)8)
#define EC_ECMBXCMDDESC_OFFS_MBXCMDTIMEOUT      ((EC_T_BYTE)10)
#define EC_ECMBXCMDDESC_OFFS_RETRIES            ((EC_T_BYTE)12)
#define EC_ECMBXCMDDESC_OFFS_IGNOREFAILURE      ((EC_T_BYTE)14)
#define EC_ECMBXCMDDESC_OFFS_FIXED              ((EC_T_BYTE)15)
#define EC_ECMBXCMDDESC_OFFS_HANDLE             ((EC_T_BYTE)16)
#define EC_ECMBXCMDDESC_OFFS_FOENAMESIZE        ((EC_T_BYTE)32)
#define EC_ECMBXCMDDESC_OFFS_FOEPASSWORD        ((EC_T_BYTE)36)

typedef struct TEcMailboxCmdDesc
{
    EC_T_WORD                   __transition;       /*  0 */
    EC_T_WORD                   __protocol;         /*  2 */   
    EC_T_DWORD                  __dataLen;          /*  4 */
    EC_T_WORD                   __cmtLen;           /*  8 */    /* (excl. \0) */
    EC_T_WORD                   __wMbxCmdTimeout;   /* 10 */    /* in ms */
    EC_T_WORD                   __retries;          /* 12 */
    EC_T_BYTE                   __ignorefailure;    /* 14 */
    EC_T_BYTE                   __fixed;            /* 15 */
    EC_T_DWORD                  __handle;           /* 16 */
    EC_T_DWORD                  reserved2[3];       /* 20 */
    union _t_uMbxHdr                                
    {                                               
        struct _t_sCoe                              
        {                                           
            EC_SDO_HDR EcSdoHeader;                 /* 32 */
            EC_T_BYTE           data[1];            /* 32 + EC_SDO_HDR_LEN */    
                                                                /* data[dataLen-sizeof(sdo)]; */
        } EC_PACKED(1) coe;
        struct _t_sEoe
        {
            EC_T_BYTE           data[1];            /* 32 + ETHERCAT_EOE_HEADER_LEN */    
                                                                /* data[dataLen-sizeof(sdo)]; */
        } EC_PACKED(1) eoe;                            
#if (defined INCLUDE_FOE_SUPPORT)
        struct _t_sFoe                              
        {                                           
            EC_FOE_HDR EcFoeHeader;                 /* 32 */
            EC_T_CHAR           name[1];            /* 40 */    /* no \0 */
        /*  EC_T_BYTE           data[];     */
        } EC_PACKED(1) foe;
#endif
#ifdef INCLUDE_SOE_SUPPORT
        struct _t_sSoe
        {                                           
            EC_SOE_HDR EcSoeHeader;                 /* 32 */
            EC_T_BYTE           data[1];            /* 32 + EC_SDO_HDR_LEN */    
                                                                /* data[dataLen-sizeof(sdo)]; */
        } EC_PACKED(1) soe;
#endif
#ifdef INCLUDE_AOE_SUPPORT
        struct _t_sAoe
        {                                           
            EC_AOE_HDR          EcAoeHeader;        /* 32 */
            EC_T_BYTE           data[1];            /* 32 + ETHERCAT_ADO_HEADER_LEN */    
                                                                /* data[dataLen-sizeof(sdo)]; */
        } EC_PACKED(1) aoe;
#endif
        EC_T_BYTE               data[1];            /* 32 */    /* data[dataLen]; */
    } EC_PACKED(1) uMbxHdr;
/*  EC_T_CHAR   cmt[cmtLen+1]; */
} EC_PACKED(1) EcMailboxCmdDesc, *PEcMailboxCmdDesc;

static EC_INLINESTART EC_T_WORD EC_ECMBXCMDDESC_GET_TRANSITION(PEcMailboxCmdDesc p)
{
    return EC_GETWORD((((EC_T_PBYTE)p) + EC_ECMBXCMDDESC_OFFS_TRANSITION));
} EC_INLINESTOP
static EC_INLINESTART EC_T_WORD EC_ECMBXCMDDESC_GET_PROTOCOL(PEcMailboxCmdDesc p)
{
    return EC_GETWORD((((EC_T_PBYTE)p) + EC_ECMBXCMDDESC_OFFS_PROTOCOL));
} EC_INLINESTOP
static EC_INLINESTART EC_T_DWORD EC_ECMBXCMDDESC_GET_DATALEN(PEcMailboxCmdDesc p)
{
    return EC_GETDWORD((((EC_T_PBYTE)p) + EC_ECMBXCMDDESC_OFFS_DATALEN));
} EC_INLINESTOP
static EC_INLINESTART EC_T_WORD EC_ECMBXCMDDESC_GET_CMTLEN(PEcMailboxCmdDesc p)
{
    return EC_GETWORD((((EC_T_PBYTE)p) + EC_ECMBXCMDDESC_OFFS_CMTLEN));
} EC_INLINESTOP
static EC_INLINESTART EC_T_WORD EC_ECMBXCMDDESC_GET_MBXTIMEOUT(PEcMailboxCmdDesc p)
{
    return EC_GETWORD((((EC_T_PBYTE)p) + EC_ECMBXCMDDESC_OFFS_MBXCMDTIMEOUT));
} EC_INLINESTOP
static EC_INLINESTART EC_T_WORD EC_ECMBXCMDDESC_GET_RETRIES(PEcMailboxCmdDesc p)
{
    return EC_GETWORD((((EC_T_PBYTE)p) + EC_ECMBXCMDDESC_OFFS_RETRIES));
} EC_INLINESTOP
static EC_INLINESTART EC_T_BYTE EC_ECMBXCMDDESC_GET_IGNOREFAILURE(PEcMailboxCmdDesc p)
{
    return *(((EC_T_PBYTE)p) + EC_ECMBXCMDDESC_OFFS_IGNOREFAILURE);
} EC_INLINESTOP
static EC_INLINESTART EC_T_BYTE EC_ECMBXCMDDESC_GET_FIXED(PEcMailboxCmdDesc p)
{
    return *(((EC_T_PBYTE)p) + EC_ECMBXCMDDESC_OFFS_FIXED);
} EC_INLINESTOP
static EC_INLINESTART EC_T_DWORD EC_ECMBXCMDDESC_GET_HANDLE(PEcMailboxCmdDesc p)
{
    return EC_GETDWORD((((EC_T_PBYTE)p) + EC_ECMBXCMDDESC_OFFS_HANDLE));
} EC_INLINESTOP
static EC_INLINESTART EC_T_WORD EC_ECMBXCMDDESC_GET_FOENAMESIZE(PEcMailboxCmdDesc p)
{
    return EC_GETWORD((((EC_T_PBYTE)p) + EC_ECMBXCMDDESC_OFFS_FOENAMESIZE));
} EC_INLINESTOP
static EC_INLINESTART EC_T_DWORD EC_ECMBXCMDDESC_GET_FOEPASSWORD(PEcMailboxCmdDesc p)
{
    return EC_GETDWORD((((EC_T_PBYTE)p) + EC_ECMBXCMDDESC_OFFS_FOEPASSWORD));
} EC_INLINESTOP
static EC_INLINESTART EC_T_VOID EC_ECMBXCMDDESC_SET_IGNOREFAILURE(PEcMailboxCmdDesc p, EC_T_BOOL bVal)
{
    *(((EC_T_PBYTE)p) + EC_ECMBXCMDDESC_OFFS_IGNOREFAILURE) = (EC_T_BYTE)(bVal?1:0);
} EC_INLINESTOP
static EC_INLINESTART EC_T_VOID EC_ECMBXCMDDESC_SET_FIXED(PEcMailboxCmdDesc p, EC_T_BOOL bVal)
{
    *(((EC_T_PBYTE)p) + EC_ECMBXCMDDESC_OFFS_FIXED) = (EC_T_BYTE)(bVal?1:0);
} EC_INLINESTOP
static EC_INLINESTART EC_T_VOID EC_ECMBXCMDDESC_SET_HANDLE(PEcMailboxCmdDesc p, EC_T_DWORD dwVal)
{
    EC_SETDWORD((((EC_T_PBYTE)p) + EC_ECMBXCMDDESC_OFFS_HANDLE), dwVal);
} EC_INLINESTOP
static EC_INLINESTART EC_T_VOID EC_ECMBXCMDDESC_SET_DATALEN(PEcMailboxCmdDesc p, EC_T_DWORD dwVal)
{
    EC_SETDWORD((((EC_T_PBYTE)p) + EC_ECMBXCMDDESC_OFFS_DATALEN), dwVal);
} EC_INLINESTOP
static EC_INLINESTART EC_T_VOID EC_ECMBXCMDDESC_SET_CMTLEN(PEcMailboxCmdDesc p, EC_T_WORD wVal)
{
    EC_SETWORD((((EC_T_PBYTE)p) + EC_ECMBXCMDDESC_OFFS_CMTLEN), wVal);
} EC_INLINESTOP
static EC_INLINESTART EC_T_VOID EC_ECMBXCMDDESC_SET_PROTOCOL(PEcMailboxCmdDesc p, EC_T_WORD wVal)
{
    EC_SETWORD((((EC_T_PBYTE)p) + EC_ECMBXCMDDESC_OFFS_PROTOCOL), wVal);
} EC_INLINESTOP
static EC_INLINESTART EC_T_VOID EC_ECMBXCMDDESC_SET_TRANSITION(PEcMailboxCmdDesc p, EC_T_WORD wVal)
{
    EC_SETWORD((((EC_T_PBYTE)p) + EC_ECMBXCMDDESC_OFFS_TRANSITION), wVal);
} EC_INLINESTOP
static EC_INLINESTART EC_T_VOID EC_ECMBXCMDDESC_SET_MBXTIMEOUT(PEcMailboxCmdDesc p, EC_T_WORD wVal)
{
    EC_SETWORD((((EC_T_PBYTE)p) + EC_ECMBXCMDDESC_OFFS_MBXCMDTIMEOUT), wVal);
} EC_INLINESTOP
#define SIZEOF_EcMailboxCmdDesc(p)  (EC_OFFSETOF(EcMailboxCmdDesc, uMbxHdr.data) + EC_ECMBXCMDDESC_GET_DATALEN(((PEcMailboxCmdDesc)(p))) + EC_ECMBXCMDDESC_GET_CMTLEN(((PEcMailboxCmdDesc)(p))) + 1)
#define EcMailboxCmdDescComment(p)  (EC_T_CHAR*)&(((EC_T_BYTE*)(p))[EC_OFFSETOF(EcMailboxCmdDesc, uMbxHdr.data) + EC_ECMBXCMDDESC_GET_DATALEN(((PEcMailboxCmdDesc)(p)))])

typedef struct _EC_T_EEP_REGS
{
    EC_T_WORD   wCtrlStatus;                /* 0x0502 */
    EC_T_DWORD  dwEEPAddress;               /* 0x0504 */
    EC_T_BYTE   abyEEPData[4];              /* 0x0508 */
} EC_PACKED(1) EC_T_SB_EEP_REGS, *EC_PT_SB_EEP_REGS;

/* Represents an IP address */
typedef union _EC_T_IPADDR
{
    EC_T_DWORD  dwAddr;
    struct _t_sAddr
    {
        EC_T_BYTE by[4];
    } EC_PACKED(1) sAddr; 
} EC_PACKED(1) EC_T_IPADDR, *EC_PT_IPADDR;

/** \struct Structure of an IP header */
typedef struct TEC_IP_HEADER
{
#if (!defined EC_NO_BITFIELDS) || (defined EC_BIG_ENDIAN)
    #ifdef EC_BIG_ENDIAN 
        EC_T_BYTE   byVersion       : 4;    /**< version */
        EC_T_BYTE   byHeaderLength  : 4;    /**< header length */
    #else 
        EC_T_BYTE   byHeaderLength  : 4;    /**< header length */
        EC_T_BYTE   byVersion       : 4;    /**< version */
    #endif
#endif
    EC_T_BYTE   byTos;                  /**< type of service */
    EC_T_WORD   wTotalLength;           /**< total length */
    EC_T_WORD   wId;                    /**< identification */
    EC_T_WORD   wFragmentOffset;        /**< fragment offset field */
    EC_T_BYTE   byTtl;                  /**< time to live */
    EC_T_BYTE   byProtocol;             /**< protocol */
    EC_T_WORD   wCheckSum;              /**< checksum */
    EC_T_IPADDR dwSrcAddr;              /**< source address */
    EC_T_IPADDR dwDstAddr;              /**< destination address */
} EC_PACKED(1) EC_IP_HEADER, *PEC_IP_HEADER;

#define EC_PROTOCOL_UDP (17)

/** \struct Structure of an UDP header */
typedef struct TEC_UDP_HEADER
{
    EC_T_WORD   wSrcPort;
    EC_T_WORD   wDstPort;
    EC_T_WORD   wLength;
    EC_T_WORD   wCheckSum;
} EC_PACKED(1) EC_UDP_HEADER, *PEC_UDP_HEADER;

#include EC_PACKED_INCLUDESTOP

/*-HELPER FUNCTIONS-----------------------------------------------------------*/
extern char* GetStateChangeNameShort(EC_T_WORD transition);

#endif /* INC_ETHERNETSERVICES */

/*-END OF SOURCE FILE--------------------------------------------------------*/
