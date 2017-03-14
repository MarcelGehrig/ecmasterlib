/*-----------------------------------------------------------------------------
 * EcCommon.h               
 * Copyright                acontis technologies GmbH, Weingarten, Germany
 * Response                 Stefan Zintgraf
 * Description              Common header shared by all AT-EM layers.
 *---------------------------------------------------------------------------*/

#ifndef INC_ECCOMMON
#define INC_ECCOMMON

/*-INCLUDES------------------------------------------------------------------*/
#ifndef INC_ECTYPE
  #include "EcType.h"
#endif
#ifndef INC_ECERROR
  #include "EcError.h"
#endif
#ifndef INC_ECOS
  #include "EcOs.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

/*-DEFINES-------------------------------------------------------------------*/

/* debug tracing */
#define     EC_TRACE_OS                     0x00000001  /* ecat os layer */
#define     EC_TRACE_CONFIG                 0x00000002  /* ecat config layer */
#define     EC_TRACE_LINK                   0x00000004  /* ecat link layer */
#define     EC_TRACE_CORE                   0x00000008  /* ecat master core layer, general */
#define     EC_TRACE_CORE_QUECMD_SEQ        0x00000010  /* ecat core layer, queued command sequence */
#define     EC_TRACE_CORE_SND_CYCFRAMES     0x00000020  /* ecat core layer, sent cyclic frames */
#define     EC_TRACE_CORE_SND_QUEFRAMES     0x00000040  /* ecat core layer, sent queued frames */
#define     EC_TRACE_CORE_RCV_CYCFRAMES     0x00000080  /* ecat core layer, received cyclic frames */
#define     EC_TRACE_CORE_RCV_QUEFRAMES     0x00000100  /* ecat core layer, received queued frames */
#define     EC_TRACE_CORE_SEQUENCE          0x00000200  /* ecat core layer, general sequence for timing */
#define     EC_TRACE_INTERFACE              0x00000400  /* interface layer */
#define     EC_TRACE_INITCMDS               0x00000800  /* trace ecat init commands */
#define     EC_TRACE_CYCCMD_HASH            0x00001000  /* ecat master core layer, hashing of cyclic commands */
#define     EC_TRACE_CREATE_FRAMES          0x00002000  /* ecat master core layer, creation of frames */

#define     EC_TRACE_CORE_ALL_FRAMES (EC_TRACE_CORE_SND_CYCFRAMES|EC_TRACE_CORE_SND_QUEFRAMES|EC_TRACE_CORE_RCV_CYCFRAMES|EC_TRACE_CORE_RCV_QUEFRAMES)

/* memory usage tracing */
#define     EC_MEMTRACE_STRING          0x00000001  /* memory tracing CString class (config layer) */
#define     EC_MEMTRACE_CONFIG          0x00000002  /* ecat config layer, general */
#define     EC_MEMTRACE_CONFIG_STR      0x00000004  /* ecat config layer, string objects */
#define     EC_MEMTRACE_CONFIG_NODE     0x00000008  /* ecat config layer, xml nodes */
#define     EC_MEMTRACE_CONFIG_DATA     0x00000010  /* ecat config layer, xml data */
#define     EC_MEMTRACE_CONFIG_CMDDESC  0x00000020  
#define     EC_MEMTRACE_XMLPARSER       0x00000040  /* ecat xml parser */
#define     EC_MEMTRACE_CORE_FACTORY    0x00000080  /* ecat core layer, device factory */
#define     EC_MEMTRACE_CORE_DEVICE     0x00000100  /* ecat core layer, device */
#define     EC_MEMTRACE_CORE_MASTER     0x00000200  /* ecat core layer, master */
#define     EC_MEMTRACE_CORE_SLAVE      0x00000400  /* ecat core layer, slaves */
#define     EC_MEMTRACE_CORE_SWITCH     0x00000800  /* ecat core layer, switch */
#define     EC_MEMTRACE_CORE_MBX        0x00001000  /* ecat core layer, mailbox */
#define     EC_MEMTRACE_CORE_FRAMES     0x00002000  /* ecat core layer, sent frames */
#define     EC_MEMTRACE_RAS             0x00004000  /* RAS layer */
#define     EC_MEMTRACE_INTERFACE       0x00008000  /* ecat interface layer */
#define     EC_MEMTRACE_APP             0x00010000  /* application */

/* 0x40-0x5F Manufacturer Specific Complex Data Type */

#define DEFTYPE_ARRAY_OF_INT      ((EC_T_WORD)0x0260)       /* ARRAY [0..n] OF INT  16*(n+1) */
#define DEFTYPE_ARRAY_OF_SINT     ((EC_T_WORD)0x0261)       /* ARRAY [0..n] OF SINT  8*(n+1) */
#define DEFTYPE_ARRAY_OF_DINT     ((EC_T_WORD)0x0262)       /* ARRAY [0..n] OF DINT  32*(n+1) */
#define DEFTYPE_ARRAY_OF_UDINT    ((EC_T_WORD)0x0263)       /* ARRAY [0..n] OF UDINT 32*(n+1) */

#define DEFTYPE_ERROR_SETTING     ((EC_T_WORD)0x0281)
#define DEFTYPE_HISTORY           ((EC_T_WORD)0x0282)       /* Object 0x10F3 */
#define DEFTYPE_DIAGNOSIS_OBJECT  ((EC_T_WORD)0x0282)       /* Object 0x10F3 */

#define DEFTYPE_EXTERNAL_SYNC_STATUS    ((EC_T_WORD)0x0283)       /* Object 0x10F4 */
#define DEFTYPE_EXTERNAL_SYNC_SETTINGS  ((EC_T_WORD)0x0284)       /* Object 0x10F5 */
#define DEFTYPE_FSOEFRAME               ((EC_T_WORD)0x0285)
#define DEFTYPE_FSOECOMMPAR             ((EC_T_WORD)0x0286)


/*-FORWARD DECLARATIONS------------------------------------------------------*/
/* these functions are only available in the DEBUG version */
ATECAT_API EC_T_DWORD ecatGetTraceMask(EC_T_VOID);
ATECAT_API EC_T_DWORD ecatGetMemTraceMask(EC_T_VOID);
ATECAT_API EC_T_VOID  ecatSetTraceMask(EC_T_DWORD dwTraceMask );
ATECAT_API EC_T_VOID  ecatSetMemTraceMask(EC_T_DWORD dwMemTraceMask );


#ifdef __cplusplus
} /* extern "C"*/
#endif

#endif /* INC_ECCOMMON */


/*-END OF SOURCE FILE--------------------------------------------------------*/
