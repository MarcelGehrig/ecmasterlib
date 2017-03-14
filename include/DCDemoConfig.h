#ifndef ETHERCAT_DCDEMOCONFIG_H
#define ETHERCAT_DCDEMOCONFIG_H
/*-----------------------------------------------------------------------------
 * DCDemoConfig.h
 * Copyright                acontis technologies GmbH, Weingarten, Germany
 * Response                 Stefan Zintgraf
 * Description              EtherCAT Master demo configuration header
 *---------------------------------------------------------------------------*/

/*-INCLUDES------------------------------------------------------------------*/


/*-DEFINES-------------------------------------------------------------------*/

/*********************/
/* thread priorities */
/*********************/
#if defined WIN32 && !defined UNDER_CE && !defined RTOS_32 && !defined UNDER_RTSS
 /* we need to set all threads to the highest possible priority to avoid errors! */
 #define TIMER_THREAD_PRIO           ((EC_T_DWORD)THREAD_PRIORITY_TIME_CRITICAL) /* EtherCAT master trigger thread priority */
 #define JOBS_THREAD_PRIO            ((EC_T_DWORD)THREAD_PRIORITY_TIME_CRITICAL) /* EtherCAT master job thread priority */
 #define RECV_THREAD_PRIO            ((EC_T_DWORD)THREAD_PRIORITY_TIME_CRITICAL) /* EtherCAT master interrupt service thread priority */
 #define LOG_THREAD_PRIO             ((EC_T_DWORD)THREAD_PRIORITY_LOWEST)  /* EtherCAT message logging thread priority */
 #define MAIN_THREAD_PRIO            ((EC_T_DWORD)THREAD_PRIORITY_NORMAL)
#elif (defined UNDER_RTSS)
 #define TIMER_THREAD_PRIO           (RT_PRIORITY_MAX-0) /* EtherCAT master trigger thread priority */
 #define JOBS_THREAD_PRIO            (RT_PRIORITY_MAX-1) /* EtherCAT master job thread priority */
 #define RECV_THREAD_PRIO            (RT_PRIORITY_MAX-2) /* EtherCAT master interrupt service thread priority */
 #define LOG_THREAD_PRIO             (RT_PRIORITY_MIN)   /* EtherCAT message logging thread priority */
 #define LOG_ROLLOVER                ((EC_T_WORD)0)      /* EtherCAT message logging rollover counter limit */
 #define MAIN_THREAD_PRIO            (RT_PRIORITY_MIN)
 #define JOBS_THREAD_STACKSIZE       0x8000
 #define LOG_THREAD_STACKSIZE        0x8000
#elif (defined RTOS_32)
 #define MAIN_THREAD_PRIO_OFFSET     10
 #define TIMER_THREAD_PRIO           ((EC_T_DWORD)64)    /* EtherCAT master trigger thread priority */
 #define JOBS_THREAD_PRIO            ((EC_T_DWORD)63)    /* EtherCAT master job thread priority */
 #define RECV_THREAD_PRIO            ((EC_T_DWORD)62)    /* EtherCAT master interrupt service thread priority */
 #define LOG_THREAD_PRIO             ((EC_T_DWORD)1 )    /* EtherCAT message logging thread priority */
 #define MAIN_THREAD_PRIO            ((EC_T_DWORD)3+MAIN_THREAD_PRIO_OFFSET)
 #define JOBS_THREAD_STACKSIZE       0x8000
 #define LOG_THREAD_STACKSIZE        0x8000
#elif (defined QNX6)
 #define TIMER_THREAD_PRIO           ((EC_T_DWORD)100)   /* EtherCAT master trigger thread priority */
 #define JOBS_THREAD_PRIO            ((EC_T_DWORD)99)    /* EtherCAT master job thread priority */
 #define RECV_THREAD_PRIO            ((EC_T_DWORD)98)    /* EtherCAT master interrupt service thread priority */
 #define LOG_THREAD_PRIO             ((EC_T_DWORD)40)    /* EtherCAT message logging thread priority */
 #define MAIN_THREAD_PRIO            ((EC_T_DWORD)30)
 #define REMOTE_RECV_THREAD_PRIO     ((EC_T_DWORD)50)    /* slightly higher than logger */
#elif (defined __RCX__)
 #define REALTIME_PRIORITY_OFFSET    TSK_PRIO_2
 #define MAIN_THREAD_PRIO_OFFSET     TSK_PRIO_50
 #define TIMER_THREAD_PRIO           (0+REALTIME_PRIORITY_OFFSET)   /* EtherCAT master trigger thread priority */
 #define JOBS_THREAD_PRIO            (1+REALTIME_PRIORITY_OFFSET)   /* EtherCAT master job thread priority */
 #define RECV_THREAD_PRIO            (2+REALTIME_PRIORITY_OFFSET)   /* EtherCAT master interrupt service thread priority */
 #define LOG_THREAD_PRIO             (5+MAIN_THREAD_PRIO_OFFSET)    /* EtherCAT message logging thread priority */
 #define LOG_ROLLOVER                ((EC_T_WORD)0)                 /* EtherCAT message logging rollover counter limit */
 #define MAIN_THREAD_PRIO            (MAIN_THREAD_PRIO_OFFSET)
#elif (defined RTAI)
 #define TIMER_THREAD_PRIO           ((EC_T_DWORD)99)   /* EtherCAT master timer task (tEcTimingTask) */
 #define JOBS_THREAD_PRIO            ((EC_T_DWORD)5)    /* EtherCAT master job thread priority (tEcJobTask) */
 #define RECV_THREAD_PRIO            ((EC_T_DWORD)99)   /* EtherCAT master packet receive thread priority (tLOsaL_IST) */
 #define LOG_THREAD_PRIO             ((EC_T_DWORD)99)   /* EtherCAT message logging thread priority (tAtEmLog) */
#elif (defined LINUX)
 #define TIMER_THREAD_PRIO           ((EC_T_DWORD)99)   /* EtherCAT master timer task (tEcTimingTask) */
 #define JOBS_THREAD_PRIO            ((EC_T_DWORD)98)   /* EtherCAT master job thread priority (tEcJobTask) */
 #define RECV_THREAD_PRIO            ((EC_T_DWORD)97)   /* EtherCAT master packet receive thread priority (tLOsaL_IST) */
 #define LOG_THREAD_PRIO             ((EC_T_DWORD)29)   /* EtherCAT message logging thread priority (tAtEmLog) */
 #define MAIN_THREAD_PRIO            ((EC_T_DWORD)39)   /* Main thread */
#elif (defined __INTEGRITY)
 #define TIMER_THREAD_PRIO           ((EC_T_DWORD)200)   /* EtherCAT master timer task (tEcTimingTask) */
 #define JOBS_THREAD_PRIO            ((EC_T_DWORD)199)   /* EtherCAT master job thread priority */
 #define RECV_THREAD_PRIO            ((EC_T_DWORD)198)   /* EtherCAT master interrupt service thread priority */
 #define LOG_THREAD_PRIO             ((EC_T_DWORD)40)    /* EtherCAT message logging thread priority */
 #define MAIN_THREAD_PRIO            ((EC_T_DWORD)40)
#elif (defined __INTIME__)
 /*Priority 0 to 127 Used by the OS for servicing external interrupts. */
 #define TIMER_THREAD_PRIO           ((EC_T_DWORD)128)   /* EtherCAT master timer task (tEcTimingTask) */
 #define JOBS_THREAD_PRIO            ((EC_T_DWORD)129)   /* EtherCAT master job thread priority */
 #define RECV_THREAD_PRIO            ((EC_T_DWORD)0)     /* Is not used for INtime. The receive thread priority is defined by the Interrupt Level  */
 #define LOG_THREAD_PRIO             ((EC_T_DWORD)200)   /* EtherCAT message logging thread priority */
 #define MAIN_THREAD_PRIO            ((EC_T_DWORD)130)
#elif (defined __TKERNEL)
 #define TIMER_THREAD_PRIO           ((EC_T_DWORD)1)     /* EtherCAT master timer task (tEcTimingTask) */
 #define JOBS_THREAD_PRIO            ((EC_T_DWORD)2)     /* EtherCAT master job thread priority */
 #define RECV_THREAD_PRIO            ((EC_T_DWORD)3)     /* EtherCAT master interrupt service thread priority */
 #define LOG_THREAD_PRIO             ((EC_T_DWORD)4)     /* EtherCAT message logging thread priority */
 #define MAIN_THREAD_PRIO            ((EC_T_DWORD)10)
#elif (defined EC_VERSION_SYSBIOS)
 #define TIMER_THREAD_PRIO           ((EC_T_DWORD)12)   /* EtherCAT master timer task (tEcTimingTask) */
 #define JOBS_THREAD_PRIO            ((EC_T_DWORD)12)   /* EtherCAT master job thread priority (tEcJobTask) */
 #define RECV_THREAD_PRIO            ((EC_T_DWORD)12)   /* EtherCAT master packet receive thread priority (tLOsaL_IST) */
 #define LOG_THREAD_PRIO             ((EC_T_DWORD)1)    /* EtherCAT message logging thread priority (tAtEmLog) */
 #define MAIN_THREAD_PRIO            ((EC_T_DWORD)2)    /* Main thread */
 #if defined SOC_AM572x
  #if defined LINKLAYER_ICSS
   #define DEMO_PARAMETERS            "-auxclk 2000 -v 2 -t 10000 -perf " \
                                      "-icss "                    \
                                      "2 "    /* Instance */      \
                                      "1 "    /* mode */          \
                                      "0"     /* Eth port */
  #else
   #define DEMO_PARAMETERS            "-auxclk 2000 -v 2 -t 10000 -perf " \
                                      "-cpsw "                    \
                                      "2 "    /* port */          \
                                      "1 "    /* mode */          \
                                      "1 "    /* priority */      \
                                      "m "    /* master flag */   \
                                      "am572x-idk " /* reference board */
  #endif
  #else
   #define DEMO_PARAMETERS            "-auxclk 2000 -v 2 -t 10000 -perf " \
                                      "-cpsw "                    \
                                      "1 "    /* port */          \
                                      "1 "    /* mode */          \
                                      "1 "    /* priority */      \
                                      "m "    /* master flag */   \
                                      "custom am33XX " /* custom board for AM33xx*/   \
                                      "1 "    /* PHY address */   \
                                      "1 "    /* PHY connection mode: RGMII */ \
                                      "0 "    /* Not use DMA buffers */
  #endif /* SOC_AM571x */

#elif (defined EC_VERSION_RIN32M3)
 #define TIMER_THREAD_PRIO           ((EC_T_DWORD)3)   /* EtherCAT master timer task (tEcTimingTask) */
 #define JOBS_THREAD_PRIO            ((EC_T_DWORD)3)   /* EtherCAT master job thread priority (tEcJobTask) */
 #define RECV_THREAD_PRIO            ((EC_T_DWORD)3)   /* EtherCAT master packet receive thread priority (tLOsaL_IST) */
 #define LOG_THREAD_PRIO             ((EC_T_DWORD)14)    /* EtherCAT message logging thread priority (tAtEmLog) */
 #define DEMO_PARAMETERS             "-auxclk 1000 -t 0 -v 2 -perf -rin32m3"
#elif (defined EC_VERSION_XILINX_STANDALONE)
 #define TIMER_THREAD_PRIO           ((EC_T_DWORD)3)   /* EtherCAT master timer task (tEcTimingTask) */
 #define JOBS_THREAD_PRIO            ((EC_T_DWORD)3)   /* EtherCAT master job thread priority (tEcJobTask) */
 #define RECV_THREAD_PRIO            ((EC_T_DWORD)3)   /* EtherCAT master packet receive thread priority (tLOsaL_IST) */
 #define LOG_THREAD_PRIO             ((EC_T_DWORD)14)    /* EtherCAT message logging thread priority (tAtEmLog) */
 #define DEMO_PARAMETERS             "-auxclk 1000 -v 2 -t 10000 -perf -rin32m3"
#elif (defined EC_VERSION_ETKERNEL)
 #define TIMER_THREAD_PRIO           ((EC_T_DWORD)1)     /* EtherCAT master timer task (tEcTimingTask) */
 #define JOBS_THREAD_PRIO            ((EC_T_DWORD)2)     /* EtherCAT master job thread priority */
 #define RECV_THREAD_PRIO            ((EC_T_DWORD)3)     /* EtherCAT master interrupt service thread priority */
 #define LOG_THREAD_PRIO             ((EC_T_DWORD)4)     /* EtherCAT message logging thread priority */
 #define MAIN_THREAD_PRIO            ((EC_T_DWORD)10)
 #define DEMO_PARAMETERS             "-b 2000 -fslfec 1 1 sabresd -v 2 -t 10000 -perf"
#elif (defined EC_VERSION_RZT1)
 #define TIMER_THREAD_PRIO           ((EC_T_DWORD)1)     /* EtherCAT master timer task (tEcTimingTask) */
 #define JOBS_THREAD_PRIO            ((EC_T_DWORD)2)     /* EtherCAT master job thread priority */
 #define RECV_THREAD_PRIO            ((EC_T_DWORD)3)     /* EtherCAT master interrupt service thread priority */
 #define LOG_THREAD_PRIO             ((EC_T_DWORD)4)     /* EtherCAT message logging thread priority */
 #define MAIN_THREAD_PRIO            ((EC_T_DWORD)10)
 #define DEMO_PARAMETERS             "-rzt1 1 -v 2 -t 20000 -perf -auxclk 1000"
 #define LOG_THREAD_STACKSIZE		 0x2000
#elif (defined EC_VERSION_RZGNOOS)
 #define TIMER_THREAD_PRIO           ((EC_T_DWORD)1)     /* EtherCAT master timer task (tEcTimingTask) */
 #define JOBS_THREAD_PRIO            ((EC_T_DWORD)2)     /* EtherCAT master job thread priority */
 #define RECV_THREAD_PRIO            ((EC_T_DWORD)3)     /* EtherCAT master interrupt service thread priority */
 #define LOG_THREAD_PRIO             ((EC_T_DWORD)4)     /* EtherCAT message logging thread priority */
 #define MAIN_THREAD_PRIO            ((EC_T_DWORD)10)
 #define DEMO_PARAMETERS             "-sheth 1 1 rzg1e -v 3 -t 20000 -perf -auxclk 1000"
 #define LOG_THREAD_STACKSIZE        0x2000
#elif (defined EC_VERSION_ECOS)
 #define TIMER_THREAD_PRIO           ((EC_T_DWORD)1)     /* EtherCAT master timer task (tEcTimingTask) */
 #define JOBS_THREAD_PRIO            ((EC_T_DWORD)2)     /* EtherCAT master job thread priority */
 #define RECV_THREAD_PRIO            ((EC_T_DWORD)3)     /* EtherCAT master interrupt service thread priority */
 #define LOG_THREAD_PRIO             ((EC_T_DWORD)4)     /* EtherCAT message logging thread priority */
#elif (defined EC_VERSION_JSLWARE)
 #define TIMER_THREAD_PRIO           ((EC_T_DWORD)12)   /* EtherCAT master timer task (tEcTimingTask) */
 #define JOBS_THREAD_PRIO            ((EC_T_DWORD)12)   /* EtherCAT master job thread priority (tEcJobTask) */
 #define RECV_THREAD_PRIO            ((EC_T_DWORD)12)   /* EtherCAT master packet receive thread priority (tLOsaL_IST) */
 #define LOG_THREAD_PRIO             ((EC_T_DWORD)1)    /* EtherCAT message logging thread priority (tAtEmLog) */
 #define MAIN_THREAD_PRIO            ((EC_T_DWORD)2)    /* Main thread */
   #define DEMO_PARAMETERS            "-auxclk 500 -v 2 -t 10000 -perf " \
                                      "-cpsw "                    \
                                      "1 "    /* port */          \
                                      "1 "    /* mode */          \
                                      "1 "    /* priority */      \
                                      "m "    /* master flag */   \
                                      "custom am33XX " /* custom board for AM33xx*/   \
                                      "1 "    /* PHY address */   \
                                      "1 "    /* PHY connection mode: RGMII */ \
                                      "0 "    /* Not use DMA buffers */
#else

 #ifdef VXWORKS_NORMAL_PRIO
  #define REALTIME_PRIORITY_OFFSET    60
 #else
  #define REALTIME_PRIORITY_OFFSET    2
 #endif

 #define TIMER_THREAD_PRIO           (0+REALTIME_PRIORITY_OFFSET)    /* EtherCAT master timer task (tEcTimingTask) */
 #define JOBS_THREAD_PRIO            (1+REALTIME_PRIORITY_OFFSET)    /* EtherCAT master cyclic packet send thread priority */
 #define RECV_THREAD_PRIO            (2+REALTIME_PRIORITY_OFFSET)    /* EtherCAT master packet receive thread priority */
 #define MBX_THREAD_PRIO             (3+REALTIME_PRIORITY_OFFSET)    /* mailbox demo thread priority */

 #define LOG_THREAD_PRIO             (200)                           /* EtherCAT message logging thread priority */
 #define MAIN_THREAD_PRIO            (4+REALTIME_PRIORITY_OFFSET)
 #define REMOTE_RECV_THREAD_PRIO     0

#endif
#ifndef TIMER_THREAD_STACKSIZE
#define TIMER_THREAD_STACKSIZE       0x1000
#endif
#ifndef JOBS_THREAD_STACKSIZE
#define JOBS_THREAD_STACKSIZE        0x4000
#endif
#ifndef LOG_THREAD_STACKSIZE
#define LOG_THREAD_STACKSIZE         0x4000
#endif

/******************/
/* timer settings */
/******************/
#define CYCLE_TIME          1           /* 1 msec */
#define BASE_PERIOD         1000        /* 1000 usec */

/***********************************************/
/* static EtherCAT master configuration values */
/***********************************************/
#define MASTER_CFG_ECAT_CMD_MAX_RETRIES        5    /* maximum retries to send pending ethercat command frames */

#define MASTER_CFG_EOE_TIMEOUT              1000    /* timeout sending EoE frames */
#define MASTER_CFG_FOE_BUSY_TIMEOUT          250    /* timeout FoE busy */

#if !(defined EC_DEMO_TINY)
#define MASTER_CFG_MAX_QUEUED_ETH_FRAMES      32    /* max number of queued frames, 127 = the absolute maximum number */
#define MASTER_CFG_MAX_SENT_QUFRM_PER_CYC      3    /* max number of queued frames sent with eUsrJob_SendAcycFrames within one cycle */
#define MASTER_CFG_MAX_SLAVECMD_PER_FRAME     32    /* max number of ecat telegrams per frame (0=maximum possible) */
#define MASTER_CFG_ECAT_MAX_BUS_SLAVES       256    /* max number of pre-allocated bus slave objects */
#else /* EC_DEMO_TINY */
#define MASTER_CFG_MAX_QUEUED_ETH_FRAMES      12    /* max number of queued frames, 127 = the absolute maximum number */
#define MASTER_CFG_MAX_SENT_QUFRM_PER_CYC      1    /* max number of queued frames sent with eUsrJob_SendAcycFrames within one cycle */
#define MASTER_CFG_MAX_SLAVECMD_PER_FRAME      3    /* max number of ecat telegrams per frame (0=maximum possible) */
#define MASTER_CFG_ECAT_MAX_BUS_SLAVES         8    /* max number of pre-allocated bus slave objects */
#endif /* EC_DEMO_TINY */

/* maximum time to get in-sync */
#define DCM_SETVAL_TIMEOUT_MSEC         12000

/*-END OF SOURCE FILE--------------------------------------------------------*/
#endif // ETHERCAT_DCDEMOCONFIG_H