#ifndef ETHERCAT_LOGGING_H
#define ETHERCAT_LOGGING_H

/*-----------------------------------------------------------------------------
 * Logging.h
 * Copyright                acontis technologies GmbH, Weingarten, Germany
 * Response                 Stefan Zintgraf
 * Description              EtherCAT Master application logging header
 *---------------------------------------------------------------------------*/

#ifndef __LOGGING_H__
#define __LOGGING_H__   1

/*-INCLUDES------------------------------------------------------------------*/
#if (defined __MET__)
#include "fio.h"
#else
#include "stdio.h"
#endif

#ifndef INC_ECOS
#include "EcOs.h"
#endif
#ifndef INC_ECTIMER
#include "EcTimer.h"
#endif

/*-MACROS--------------------------------------------------------------------*/


/*-DEFINES-------------------------------------------------------------------*/

/* log thread priority (very low) */
#if defined WIN32 && !defined UNDER_CE && !defined RTOS_32
 #define LOG_ROLLOVER                ((EC_T_WORD)0)
#elif (defined __RCX__)
 #define LOG_ROLLOVER                ((EC_T_WORD)0)
#elif (defined RTOS_32)
 #define LOG_ROLLOVER                ((EC_T_WORD)3000)
#else
 #define LOG_ROLLOVER                ((EC_T_WORD)10000)
#endif

#define MAX_PATH_LEN                 256

/*-GLOBAL VARIABLES-----------------------------------------------------------*/

extern EC_T_BOOL bLogFileEnb;

/*-TYPEDEFS------------------------------------------------------------------*/

typedef struct _LOG_MSG_DESC
{
    EC_T_BOOL  bValid;                /* entry is valid */
    EC_T_CHAR* szMsgBuffer;           /* buffers */
    EC_T_DWORD dwLen;                 /* message size */
    EC_T_DWORD dwMsgTimestamp;        /* timestamp values */
    EC_T_DWORD dwMsgThreadId;         /* threadId values */
    EC_T_BOOL  bMsgCrLf;              /* CR/LF do/don't */
    EC_T_BOOL  bOsDbgMsg;             /* OsDbgMsg values */
} LOG_MSG_DESC;



typedef struct _MSG_BUFFER_DESC
{
    struct _MSG_BUFFER_DESC* pNextMsgBuf;           /* link to next message buffer */
    LOG_MSG_DESC*   paMsg;              /* array of messages */
    EC_T_DWORD  dwMsgSize;              /* message size */
    EC_T_DWORD  dwNumMsgs;              /* number of messages */
    EC_T_DWORD  dwNextEmptyMsgIndex;    /* index of next empty message buffer */
    EC_T_DWORD  dwNextPrintMsgIndex;    /* index of next message buffer to print */
    EC_T_CHAR   szMsgLogFileName[MAX_PATH_LEN]; /* message log file name */
    EC_T_CHAR   szMsgLogFileExt[4];             /* message log file extension */
    FILE*       pfMsgFile;              /* file pointer for message log file */
    EC_T_BOOL   bPrintTimestamp;        /* EC_TRUE if a timestamp shall be printed in the log file */
    EC_T_BOOL   bPrintConsole;          /* EC_TRUE if the message shall be printed on the console */
    EC_T_BOOL   bIsInitialized;         /* EC_TRUE if message buffer is initialized */
    EC_T_WORD   wLogFileIndex;          /* Index of current log file */
    EC_T_WORD   wEntryCounter;          /* Entries to detect roll over */
    EC_T_WORD   wEntryCounterLimit;     /* Entries before roll over */
    EC_T_WORD   wRes;
	/* logging into memory buffer */
    EC_T_CHAR   szLogName[MAX_PATH_LEN];/* name of the logging buffer */
    EC_T_BYTE*  pbyLogMemory;           /* if != EC_NULL then log into memory instead of file */
    EC_T_BYTE*  pbyNextLogMsg;          /* pointer to next logging message */
    EC_T_DWORD  dwLogMemorySize;        /* size of logging memory */
    EC_T_BOOL   bLogBufferFull;         /* EC_TRUE if log buffer is full */
    /* skip identical messages */
    EC_T_BOOL	bSkipDuplicateMessages; /* if set to EC_TRUE, then multiple identical messages will not be printed out */
    EC_T_DWORD	dwNumDuplicates;        /* if 0, the new message is not duplicated */
    EC_T_CHAR*  pszLastMsg;             /* pointer to last message (points into message buffer) */
    EC_T_BOOL   bNewLine;               /* EC_TRUE if last message printed with CrLf */
} MSG_BUFFER_DESC;



/*-FORWARD DECLARATIONS------------------------------------------------------*/


/*-CLASS---------------------------------------------------------------------*/

class CAtEmLogging
{
    
public:
                CAtEmLogging(                   EC_T_VOID                                           );

    EC_T_BOOL   OsDbgMsgHook(                   const
                                                EC_T_CHAR*              szFormat, 
                                                EC_T_VALIST             vaArgs                      );
    EC_T_DWORD  LogMsg(                         const
                                                EC_T_CHAR*              szFormat,...                );
    EC_T_DWORD  LogMsgAdd(                      const
                                                EC_T_CHAR*              szFormat,...                );
    EC_T_DWORD  LogError(                       const
                                                EC_T_CHAR*              szFormat,...                );
    EC_T_DWORD  LogErrorAdd(                    const
                                                EC_T_CHAR*              szFormat,...                );
    EC_T_DWORD  LogDcm(                         const
                                                EC_T_CHAR*              szFormat,...                );
    EC_T_DWORD  LogDcmAdd(                    const
                                                EC_T_CHAR*              szFormat,...                );
    EC_T_VOID   InitLogging(                    EC_T_DWORD              dwMasterID, 
                                                EC_T_WORD               wRollOver,
                                                EC_T_DWORD              dwPrio,
                                                EC_T_DWORD              dwCpuIndex,
                                                EC_T_CHAR*              szFilenamePrefix = EC_NULL, 
                                                EC_T_DWORD              dwStackSize = 0x4000        );
    EC_T_VOID   SetLogMsgBuf(                   EC_T_BYTE*              pbyLogMem,
                                                EC_T_DWORD              dwSize                      );
    EC_T_VOID   SetLogErrBuf(                   EC_T_BYTE*              pbyLogMem,
                                                EC_T_DWORD              dwSize                      );
    EC_T_VOID   SetLogDcmBuf(                   EC_T_BYTE*              pbyLogMem,
                                                EC_T_DWORD              dwSize                      );
    EC_T_VOID   DeinitLogging(                  EC_T_VOID                                           );
    EC_T_BOOL   SetLogThreadAffinity(           EC_T_DWORD              dwCpuIndex                  );
    EC_T_BOOL   OsDbgMsgHookEnable(             EC_T_BOOL               bEnable                     );

    static 
    EC_T_BOOL   OsDbgMsgHookWrapper(            const
                                                EC_T_CHAR*              szFormat, 
                                                EC_T_VALIST             vaArgs                      );
    EC_T_VOID   tAtEmLog(                       EC_T_VOID*              pvParms                     );
    EC_T_VOID   ProcessAllMsgs(                 EC_T_VOID                                           );

    struct _MSG_BUFFER_DESC*  AddLogBuffer(     EC_T_DWORD              dwMasterID,
                                                EC_T_WORD               wRollOver,
                                                EC_T_DWORD				dwBufferSize,
                                                EC_T_BOOL               bSkipDuplicates,
                                                EC_T_CHAR*              szLogName,
                                                EC_T_CHAR*              szLogFilename,
                                                EC_T_CHAR*              szLogFileExt,
                                                EC_T_BOOL               bPrintConsole,
                                                EC_T_BOOL               bPrintTimestamp             );

    static
    EC_T_VOID   SetMsgBuf(                      MSG_BUFFER_DESC*        pMsgBufferDesc,
                                                EC_T_BYTE*              pbyLogMem,
                                                EC_T_DWORD              dwSize                      );

    EC_T_DWORD  InsertNewMsg(                   MSG_BUFFER_DESC*        pMsgBufferDesc, 
                                                const
                                                EC_T_CHAR*              szFormat, 
                                                EC_T_VALIST             vaArgs,
                                                EC_T_BOOL               bDoCrLf=EC_TRUE,
                                                EC_T_BOOL               bOsDbgMsg=EC_FALSE          );

    EC_T_DWORD  SetLogDir(                      EC_T_CHAR*              szLogDir                    );


private:
    static
    EC_T_BOOL   InitMsgBuffer(                  MSG_BUFFER_DESC*        pMsgBufferDesc,
                                                EC_T_DWORD              dwMsgSize,
                                                EC_T_DWORD              dwNumMsgs,
                                                EC_T_BOOL               bSkipDuplicates,
                                                EC_T_BOOL               bPrintConsole,
                                                EC_T_BOOL               bPrintTimestamp,
                                                EC_T_CHAR*              szMsgLogFileName,
                                                EC_T_CHAR*              szMsgLogFileExt,
                                                EC_T_WORD               wRollOver,
                                                EC_T_CHAR*              szLogName                   );
  
    EC_T_VOID   DeinitMsgBuffer(                MSG_BUFFER_DESC*        pMsgBufferDesc              );
    EC_T_VOID   ProcessMsgs(                    MSG_BUFFER_DESC*        pMsgBufferDesc              );
    
    static
    EC_T_VOID   SelectNextLogMemBuffer(         MSG_BUFFER_DESC*        pMsgBufferDesc              );

    static 
    EC_T_VOID   tAtEmLogWrapper(                EC_T_VOID*              pvParms                     );
    
    EC_T_PVOID              m_pvLogThreadObj;
    EC_T_BOOL               m_bLogTaskRunning;
    EC_T_BOOL               m_bShutdownLogTask;

    MSG_BUFFER_DESC*        m_pFirstMsgBufferDesc;          /* pointer to first message buffer */
    MSG_BUFFER_DESC*        m_pLastMsgBufferDesc;           /* link to last message buffer */
    MSG_BUFFER_DESC*        m_pAllMsgBufferDesc;            /* buffer for all messages */
    MSG_BUFFER_DESC*        m_pErrorMsgBufferDesc;          /* buffer for application error messages */
    MSG_BUFFER_DESC*        m_pDcmMsgBufferDesc;            /* DCM buffer */
    EC_T_CHAR*              m_pchTempbuffer;
    EC_T_VOID*              m_poInsertMsgLock;              /* lock object for inserting new messages */
    EC_T_VOID*              m_poProcessMsgLock;             /* lock object for processing messages */
    EC_T_BOOL               m_bDbgMsgHookEnable;

    CEcTimer                m_oMsgTimeout;
    CEcTimer                m_oSettlingTimeout;
    EC_T_DWORD              m_dwNumMsgsSinceMsrmt;
    EC_T_BOOL               m_bSettling;

    EC_T_CHAR               m_pchLogDir[MAX_PATH_LEN];      /* directory for all EtherCAT logging files */
};

#endif /*__LOGGING_H__*/

/*-END OF SOURCE FILE--------------------------------------------------------*/
#endif // ETHERCAT_LOGGING_H