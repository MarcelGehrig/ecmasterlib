#ifndef ETHERCAT_ECATNOTIFICATION_H
#define ETHERCAT_ECATNOTIFICATION_H

/*-----------------------------------------------------------------------------
 * ecatNotification.h
 * Copyright                acontis technologies GmbH, Weingarten, Germany
 * Response                 Paul Bussmann
 * Description              EC-Master generic notification handler
 *---------------------------------------------------------------------------*/

#ifndef INC_ECATNOTIFICATION
#define INC_ECATNOTIFICATION 1

/*-INCLUDES------------------------------------------------------------------*/
#include <AtEthercat.h>
// #include <EcType.h>

/*-DEFINES-------------------------------------------------------------------*/
#if !(defined EC_DEMO_TINY)
#define JOB_QUEUE_LENGTH     20
#else
#define JOB_QUEUE_LENGTH     1
#endif /* !(defined EC_DEMO_TINY) */

/*-TYPEDEFS------------------------------------------------------------------*/
typedef EC_T_BOOL (*PF_PROCESS_NOTIFICATION_HOOK)(EC_T_PVOID pInstance, struct _T_SLAVEJOBS* pSlaveJob);

typedef struct _EC_T_RAWCMD_DATA_DESC
{
    EC_T_DWORD	dwInvokeId;
    EC_T_DWORD	dwResult;
    EC_T_DWORD	dwWkc;
    EC_T_DWORD	dwCmdIdx;
    EC_T_DWORD	dwAddr;
    EC_T_DWORD	dwLength;
    EC_T_BYTE	abyData[MAX_EC_DATA_LEN];
} EC_T_RAWCMD_DATA_DESC;
                                                  

typedef union _T_JobData
{
    EC_T_DWORD                      StatusJob;
    EC_T_SB_STATUS_NTFY_DESC        ScanBusJob;
    EC_T_RAWCMD_DATA_DESC           RawCmdJob;
    EC_T_DC_SYNC_NTFY_DESC          DcSlaveSyncJob;
    EC_T_SB_MISMATCH_DESC           SBSlaveMismatchJob;
    EC_T_SLAVE_ERROR_INFO_DESC      SlaveErrorInfo;
    EC_T_MBXTFER                    MbxTferJob;
} T_JobData;

typedef struct _T_SLAVEJOBS
{
    EC_T_DWORD      dwCode;                     /* notification code */
    T_JobData       JobData;
} T_SLAVEJOBS, *PT_SLAVEJOBS;

typedef struct _T_SLAVEJOBQUEUE
{
    T_SLAVEJOBS     Jobs[JOB_QUEUE_LENGTH];     /* single job is at least size of largest union element,
                                                 * which in case (default) is 1500 byte (MAX_EC_DATA_LEN)
                                                 */
    PT_SLAVEJOBS    pWrite;
    PT_SLAVEJOBS    pRead;
} T_SLAVEJOBQUEUE, *PT_SLAVEJOBQUEUE;

/*-CLASS---------------------------------------------------------------------*/
class CAtEmLogging;

class CEmNotification
{
    
public:
                CEmNotification(            EC_T_DWORD                      dwMasterInstance, 
                                            CAtEmLogging*                   pcLogging,
                                            EC_T_BOOL                       bRasClient = EC_FALSE);
                                                                                                        
    EC_T_BOOL   ProcessNotificationJobs(    EC_T_VOID                                                   );
                                                                                                        
    EC_T_DWORD  emRasNotify(                EC_T_DWORD                      dwCode,                     
                                            EC_T_NOTIFYPARMS*               pParms                      );

    static 
    EC_T_DWORD  NotifyWrapper(              EC_T_DWORD                      dwCode,
                                            EC_T_NOTIFYPARMS*               pParms                      );

    EC_T_DWORD  ecatNotify(                 EC_T_DWORD                      dwCode,                     
                                            EC_T_NOTIFYPARMS*               pParms);
                                                                                                        
                                                                                                        
    EC_T_VOID   ResetErrorCounters(         EC_T_VOID                                                   );
    EC_T_VOID   SetProcessNotificationHook( EC_T_PVOID                      pInstance,
                                            PF_PROCESS_NOTIFICATION_HOOK    pfProcessNotificationHook   );
    EC_T_VOID   Verbose(                    EC_T_INT                        nVal                        )
                    { m_nVerbosePrinting = nVal; }
    EC_T_INT    Verbose(                    EC_T_VOID                                                   )
                    { return m_nVerbosePrinting; }

    EC_T_BOOL   AllDevsOperational(         EC_T_VOID                                                   )
                    { return m_bAllDevsOperational; }
    
    EC_T_VOID   MasterInstanceId(                   EC_T_DWORD              dwVal                       )
                    { m_dwMasterInstance = dwVal; }
    EC_T_DWORD  MasterInstanceId(                   EC_T_VOID                                           )
                    { return m_dwMasterInstance; }
    EC_T_VOID   SetClientID(                        EC_T_DWORD              dwClientID                  )
                    { m_dwClientID = dwClientID; }
    EC_T_DWORD  GetClientID(                        EC_T_VOID                                           )
                    { return m_dwClientID; }
    

    EC_T_BOOL   RasServerDisconnect(                EC_T_VOID                                           )
                    { return m_bRasServerDisconnect; }
    EC_T_VOID   RasServerDisconnect(                EC_T_BOOL               bVal                        )
                    { m_bRasServerDisconnect = bVal; }

    EC_T_BOOL   IsBlockIssued(                      EC_T_VOID                                           )
                    { return m_bDidIssueBlock; }
    EC_T_VOID   ResetBlockIssued(                   EC_T_VOID                                           )
                    { m_bDidIssueBlock = EC_FALSE; }
    EC_T_VOID   SetBlockIssued(                     EC_T_VOID                                           )
                    { m_bDidIssueBlock = EC_TRUE; }

private:

    EC_T_DWORD                      m_dwMasterInstance;
    CAtEmLogging*                   m_pcLogging;
                                    
    EC_T_DWORD                      m_adwErrorCounter[EC_NUM_ERROR_NOTIFICATIONS + 1]; /* error counter */

    T_SLAVEJOBQUEUE                 m_oSlaveJobQueue;
    
    EC_T_INT                        m_nVerbosePrinting;
    EC_T_BOOL                       m_bAllDevsOperational;

    EC_T_DWORD                      m_dwClientID;                           /* ID of registered client */
   
    EC_T_BOOL                       m_bRasServerDisconnect;                 /* TRUE: RAS Server notified Disconnection, all Objects (MBX, ClientReg ...) have to be
                                                                             * re-created
                                                                             */
    EC_T_BOOL                       m_bRasClient;                           /* Flag: We are Remote API client */

    EC_T_BOOL                       m_bDidIssueBlock;                       /* Node(s) is (are) blocked */

    struct
    {
        EC_T_DWORD                  dwSlaveId;
        EC_T_VOID*                  hFile;
        EC_T_DWORD                  dwDataLen;
    } m_SegmentedFoeDownload;

    PT_SLAVEJOBS IncrementJobPtr(   PT_SLAVEJOBS pPtr                                                           );

    EC_T_BOOL   EnqueueJob(         PT_SLAVEJOBS pJob, EC_T_DWORD dwCode, EC_T_VOID* pSrc, EC_T_DWORD dwSize    );
    EC_T_BOOL   DequeueJob(         PT_SLAVEJOBS pJob                                                           );
};

#endif /* INC_ECATNOTIFICATION */

/*-END OF SOURCE FILE--------------------------------------------------------*/
#endif // ETHERCAT_ECATNOTIFICATION_H