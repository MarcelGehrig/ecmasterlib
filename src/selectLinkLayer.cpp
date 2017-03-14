/*-----------------------------------------------------------------------------
 * selectLinkLayer.cpp
 * Copyright                acontis technologies GmbH, Weingarten, Germany
 * Response                 Andreas Willig
 * Description              EtherCAT master link layer selection
 *---------------------------------------------------------------------------*/

/*-INCLUDES------------------------------------------------------------------*/
#include <selectLinkLayer.h>

/*-DEFINES-------------------------------------------------------------------*/
#if (defined EC_VERSION_WINDOWS)
 #ifndef INCLUDE_WINPCAP
 #define INCLUDE_WINPCAP
 #endif
#endif /* EC_VERSION_WINDOWS */
#if (defined LINUX)
 #ifndef INCLUDE_SOCKRAW
 #define INCLUDE_SOCKRAW
 #endif
 #ifndef INCLUDE_R6040
 #define INCLUDE_R6040
 #endif
#if (defined __arm__)
 #ifndef INCLUDE_FSLFEC
 #define INCLUDE_FSLFEC
 #endif /* __arm__ */
#endif
#endif /* LINUX */
#if (defined UNDER_CE)
 #ifndef INCLUDE_NDISUIO
 #define INCLUDE_NDISUIO
 #endif
 #if (defined _ARM_)
  #ifndef INCLUDE_FSLFEC
  #define INCLUDE_FSLFEC
  #endif
 #endif
#endif /* UNDER_CE */
#if (defined VXWORKS)
 #ifndef INCLUDE_SNARF
 #define INCLUDE_SNARF
 #endif
#if (defined _VX_CPU_FAMILY) && (_VX_CPU_FAMILY==_VX_ARM)
 #ifndef INCLUDE_FSLFEC
 #define INCLUDE_FSLFEC
 #endif
 #ifndef INCLUDE_GEM
 #define INCLUDE_GEM
 #endif
#endif
#if (defined _VX_CPU) && (_VX_CPU==_VX_PPC440)
 #ifndef INCLUDE_EMAC
 #define INCLUDE_EMAC
 #endif
#endif
#endif /* VXWORKS */
#if (defined EC_VERSION_WINDOWS) || (defined LINUX) || (defined UNDER_CE) || (defined VXWORKS) || (defined RTOS_32) || (defined QNX6) || (defined UNDER_RTSS) || (defined __INTIME__) || (defined __TKERNEL) || (defined EC_VERSION_RTEMS) || (defined EC_VERSION_ECOS) || (defined __INTEGRITY)
 #ifndef INCLUDE_I8254X
 #define INCLUDE_I8254X
 #endif
 #ifndef INCLUDE_I8255X
 #define INCLUDE_I8255X
 #endif
 #ifndef INCLUDE_RTL8169
 #define INCLUDE_RTL8169
 #endif
 #ifndef INCLUDE_RTL8139
 #define INCLUDE_RTL8139
 #endif
 #ifndef INCLUDE_CCAT
 #define INCLUDE_CCAT
 #endif
 #ifndef INCLUDE_EG20T
 #define INCLUDE_EG20T
 #endif
#endif /* EC_VERSION_WINDOWS || LINUX || UNDER_CE || VXWORKS || RTOS_32 || QNX6 || UNDER_RTSS || __INTIME__ || __TKERNEL */

#if (defined QNX6) && (defined __arm__)
#ifndef INCLUDE_CPSW
#define INCLUDE_CPSW
#endif
#ifndef INCLUDE_DW3504
#define INCLUDE_DW3504
#endif
#endif /* QNX6 && __arm__ */
#if (defined EC_VERSION_ETKERNEL)
 #ifndef INCLUDE_FSLFEC
 #define INCLUDE_FSLFEC
 #endif
#endif /* EC_VERSION_ETKERNEL */

#if ((defined __TKERNEL) && (defined __arm__))
#ifndef INCLUDE_L9218I
#define INCLUDE_L9218I
#endif
#endif

#if (defined EC_VERSION_GO32)
#if !(defined INCLUDE_R6040)
#define INCLUDE_R6040
#endif
#endif

#if (defined EC_VERSION_RZGNOOS)
#define INCLUDE_SHETH
#endif

/*-TYPEDEFS------------------------------------------------------------------*/

/*-CLASS---------------------------------------------------------------------*/

/*-FUNCTION DECLARATION------------------------------------------------------*/

/*-LOCAL VARIABLES-----------------------------------------------------------*/

/*-FUNCTION-DEFINITIONS------------------------------------------------------*/
/********************************************************************************/
/** \brief  Parse for the ip-address and return an byte array (EC_T_BYTE[4])
*
* \return  EC_TRUE in case of success, EC_FALSE in case of an invalid parameter or
*          SYNTAX_ERROR (-2) .
*/

EC_T_BOOL ParseIpAddress
(
    EC_T_CHAR* ptcWord,
    EC_T_BYTE* pbyIpAddress)
{
    EC_T_CHAR* ptcTmp   = EC_NULL;
    EC_T_INT   nCnt     = 0;
    EC_T_BOOL bRetVal = EC_TRUE;

    if(EC_NULL == pbyIpAddress )
    {
        bRetVal = EC_FALSE;
        goto Exit;
    }

    /* Get IP address */
    ptcTmp = ptcWord;
    for (nCnt = 0; nCnt < 4; nCnt++)
    {
        if (ptcTmp == EC_NULL)
        {
            bRetVal = EC_FALSE;
            goto Exit;
        }
        pbyIpAddress[nCnt] = (EC_T_BYTE)OsStrtol(ptcTmp, EC_NULL, 10);
        if(nCnt < 2)
        {
            ptcTmp = OsStrtok( EC_NULL, ".");
        }
        else if(nCnt < 3)
        {
            ptcTmp = OsStrtok( EC_NULL, " ");
        }
    }

    Exit:

    return bRetVal;
}

/********************************************************************************/
/** Parse next command line argument
*
* Return: pointer to the next argument.
*/
EC_T_CHAR* GetNextWord(EC_T_CHAR **ppCmdLine, EC_T_CHAR *pStorage)
{
    EC_T_CHAR *pWord;

    EC_UNREFPARM(ppCmdLine);
    EC_UNREFPARM(pStorage);

    pWord = (EC_T_CHAR *)OsStrtok(NULL, " ");

    return pWord;
}

/***************************************************************************************************/
/**
\brief  Parses string parameter value from the command line.

\return EC_TRUE if successfully parsed, EC_FALSE on syntax errors.
*/
static EC_T_BOOL ParseString(
    EC_T_CHAR**     ptcWord,
    EC_T_CHAR**     lpCmdLine,
    EC_T_CHAR*      tcStorage)
{
    (*ptcWord) = GetNextWord(lpCmdLine, tcStorage);
    if (((*ptcWord) == EC_NULL) || (OsStrncmp( (*ptcWord), "-", 1) == 0))
    {
        return EC_FALSE;
    }

    return EC_TRUE;
}

/***************************************************************************************************/
/**
\brief  Parses EC_T_DWORD parameter value from the command line.

\return EC_TRUE if successfully parsed, EC_FALSE on syntax errors.
*/
static EC_T_BOOL ParseDword(
    EC_T_CHAR**     ptcWord,
    EC_T_CHAR**     lpCmdLine,
    EC_T_CHAR*      tcStorage,
    EC_T_DWORD*     pdwValue)
{
    if ( !ParseString(ptcWord, lpCmdLine, tcStorage) )
    {
        return EC_FALSE;
    }
    *pdwValue = (EC_T_DWORD)OsStrtol((*ptcWord), NULL, 0);

    return EC_TRUE;
}

/***************************************************************************************************/
/**
\brief  Parses EC_T_LINKMODE parameter value from the command line.

\return EC_TRUE if successfully parsed, EC_FALSE on syntax errors.
*/
EC_T_BOOL ParseLinkMode(
    EC_T_CHAR**     ptcWord,
    EC_T_CHAR**     lpCmdLine,
    EC_T_CHAR*      tcStorage,
    EC_T_LINKMODE*  peLinkMode)
{
    EC_T_DWORD dwMode = 0;
    EC_T_BOOL bRes = EC_FALSE;

    if (ParseDword(ptcWord, lpCmdLine, tcStorage, &dwMode))
    {
        if (dwMode == 0)
        {
            *peLinkMode = EcLinkMode_INTERRUPT;
            bRes = EC_TRUE;
        }
        else if (dwMode == 1)
        {
            *peLinkMode = EcLinkMode_POLLING;
            bRes = EC_TRUE;
        }
        else
        {
            OsDbgMsg("Invalid Link Layer Mode (mode == %d)\n", dwMode);
        }
    }    
    return bRes;
}

/***************************************************************************************************/
/**
\brief  Fill common link layer parameters
*/
static EC_T_VOID LinkParmsInit(EC_T_LINK_PARMS* pLinkParms, 
                               const EC_T_DWORD dwSignature, const EC_T_DWORD dwSize, const char* szDriverIdent,
                               const EC_T_DWORD dwInstance, const EC_T_LINKMODE eLinkMode, const EC_T_DWORD dwIstPriority = 0)
{
    OsMemset(pLinkParms, 0, sizeof(EC_T_LINK_PARMS));
    pLinkParms->dwSignature = dwSignature;
    pLinkParms->dwSize = dwSize;
    OsStrncpy(pLinkParms->szDriverIdent, szDriverIdent, MAX_DRIVER_IDENT_LEN - 1);
    pLinkParms->dwInstance = dwInstance;
    pLinkParms->eLinkMode = eLinkMode;
    pLinkParms->dwIstPriority = dwIstPriority;
}

#if (defined INCLUDE_CCAT)
/***************************************************************************************************/
/**
\brief  Try to create CCAT link layer parameters according to current command line parsing

\return EC_E_NOERROR     if link layer parameters was created
        EC_E_NOTFOUND    if command line was not matching
        EC_E_INVALIDPARM if syntax error
*/
static EC_T_DWORD CreateLinkParmsFromCmdLineCCAT(EC_T_CHAR** ptcWord, EC_T_CHAR** lpCmdLine, EC_T_CHAR* tcStorage, EC_T_BOOL*  pbGetNextWord,
                                                 EC_T_LINK_PARMS** ppLinkParms)
{
EC_T_DWORD dwRetVal = EC_E_ERROR;
EC_T_LINK_PARMS_CCAT* pLinkParmsAdapter = EC_NULL;

    EC_UNREFPARM(pbGetNextWord);

    /* check for matching adapter */
    if (OsStricmp((*ptcWord), "-ccat") != 0)
    {
        dwRetVal = EC_E_NOTFOUND;
        goto Exit;
    }
    /* alloc adapter specific link parms */
    pLinkParmsAdapter = (EC_T_LINK_PARMS_CCAT*)OsMalloc(sizeof(EC_T_LINK_PARMS_CCAT));
    if (EC_NULL == pLinkParmsAdapter)
    {
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }
    OsMemset(pLinkParmsAdapter, 0, sizeof(EC_T_LINK_PARMS_CCAT));
    LinkParmsInit(&pLinkParmsAdapter->linkParms, EC_LINK_PARMS_SIGNATURE_CCAT, sizeof(EC_T_LINK_PARMS_CCAT), EC_LINK_PARMS_IDENT_CCAT, 1, EcLinkMode_POLLING);

    /* parse mandatory parameters: instance, mode */
    if ( !ParseDword(ptcWord, lpCmdLine, tcStorage, &pLinkParmsAdapter->linkParms.dwInstance)
        || !ParseLinkMode(ptcWord, lpCmdLine, tcStorage, &pLinkParmsAdapter->linkParms.eLinkMode))
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* no errors */
    *ppLinkParms = &pLinkParmsAdapter->linkParms;
    dwRetVal = EC_E_NOERROR;

Exit:
    if (EC_E_NOERROR != dwRetVal)
    {
        SafeOsFree(pLinkParmsAdapter);
    }
    return dwRetVal;
}
#endif /* INCLUDE_CCAT */

#if (defined INCLUDE_EG20T)
/***************************************************************************************************/
/**
\brief  Try to create EG20T link layer parameters according to current command line parsing

\return EC_E_NOERROR     if link layer parameters was created
        EC_E_NOTFOUND    if command line was not matching
        EC_E_INVALIDPARM if syntax error
*/
static EC_T_DWORD CreateLinkParmsFromCmdLineEG20T(EC_T_CHAR** ptcWord, EC_T_CHAR** lpCmdLine, EC_T_CHAR* tcStorage, EC_T_BOOL*  pbGetNextWord,
                                                  EC_T_LINK_PARMS** ppLinkParms)
{
EC_T_DWORD dwRetVal = EC_E_ERROR;
EC_T_LINK_PARMS_EG20T* pLinkParmsAdapter = EC_NULL;

    EC_UNREFPARM(pbGetNextWord);

    /* check for matching adapter */
    if (OsStricmp((*ptcWord), "-eg20t") != 0)
    {
        dwRetVal = EC_E_NOTFOUND;
        goto Exit;
    }
    /* alloc adapter specific link parms */
    pLinkParmsAdapter = (EC_T_LINK_PARMS_EG20T*)OsMalloc(sizeof(EC_T_LINK_PARMS_EG20T));
    if (EC_NULL == pLinkParmsAdapter)
    {
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }
    OsMemset(pLinkParmsAdapter, 0, sizeof(EC_T_LINK_PARMS_CCAT));
    LinkParmsInit(&pLinkParmsAdapter->linkParms, EC_LINK_PARMS_SIGNATURE_EG20T, sizeof(EC_T_LINK_PARMS_EG20T), EC_LINK_PARMS_IDENT_EG20T, 1, EcLinkMode_POLLING);

    /* parse mandatory parameters: instance, mode */
    if ( !ParseDword(ptcWord, lpCmdLine, tcStorage, &pLinkParmsAdapter->linkParms.dwInstance)
        || !ParseLinkMode(ptcWord, lpCmdLine, tcStorage, &pLinkParmsAdapter->linkParms.eLinkMode))
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* no errors */
    *ppLinkParms = &pLinkParmsAdapter->linkParms;
    dwRetVal = EC_E_NOERROR;

Exit:
    if (EC_E_NOERROR != dwRetVal)
    {
        SafeOsFree(pLinkParmsAdapter);
    }
    return dwRetVal;
}
#endif /* INCLUDE_EG20T */

#if (defined INCLUDE_CPSW)
/***************************************************************************************************/
/**
\brief  Parses EC_T_CPSW_TYPE parameter value from the command line.

\return EC_TRUE if successfully parsed, EC_FALSE on syntax errors.
*/
EC_T_BOOL ParseCPSWType(
    EC_T_CHAR**     ptcWord,
    EC_T_CHAR**     lpCmdLine,
    EC_T_CHAR*      tcStorage,
    EC_T_CPSW_TYPE*  peType)
{
    EC_T_BOOL bRes = EC_FALSE;

    if (ParseString(ptcWord, lpCmdLine, tcStorage))
    {
        bRes = EC_TRUE;
        if (0 == OsStricmp((*ptcWord), "am33XX"))       { *peType = eCPSW_AM33XX; }
        else if (0 == OsStricmp((*ptcWord), "am437X"))  { *peType = eCPSW_AM437X; }
        else if (0 == OsStricmp((*ptcWord), "am57X"))   { *peType = eCPSW_AM57X;  }
        else if (0 == OsStricmp((*ptcWord), "am387X"))  { *peType = eCPSW_AM387X; }
        else
        {
            OsDbgMsg("Invalid CPSW type (type == %s)\n", ptcWord);
            bRes = EC_FALSE;
        }
    }
    return bRes;
}

/***************************************************************************************************/
/**
\brief  Try to create CPSW link layer parameters according to current command line parsing

\return EC_E_NOERROR     if link layer parameters was created
        EC_E_NOTFOUND    if command line was not matching
        EC_E_INVALIDPARM if syntax error
*/
static EC_T_DWORD CreateLinkParmsFromCmdLineCPSW(EC_T_CHAR** ptcWord, EC_T_CHAR** lpCmdLine, EC_T_CHAR* tcStorage, EC_T_BOOL*  pbGetNextWord,
                                                 EC_T_LINK_PARMS** ppLinkParms)
{
EC_T_DWORD dwRetVal = EC_E_ERROR;
EC_T_LINK_PARMS_CPSW* pLinkParmsAdapter = EC_NULL;
EC_T_DWORD dwPhyConnection = 0;

    /* check for matching adapter */
    if (OsStricmp((*ptcWord), "-cpsw") != 0)
    {
        dwRetVal = EC_E_NOTFOUND;
        goto Exit;
    }
    /* alloc adapter specific link parms */
    pLinkParmsAdapter = (EC_T_LINK_PARMS_CPSW*)OsMalloc(sizeof(EC_T_LINK_PARMS_CPSW));
    if (EC_NULL == pLinkParmsAdapter)
    {
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }
    OsMemset(pLinkParmsAdapter, 0, sizeof(EC_T_LINK_PARMS_CPSW));
    LinkParmsInit(&pLinkParmsAdapter->linkParms, EC_LINK_PARMS_SIGNATURE_CPSW, sizeof(EC_T_LINK_PARMS_CPSW), EC_LINK_PARMS_IDENT_CPSW, 1, EcLinkMode_POLLING);

    /* parse mandatory parameters: port (instance), mode, port priority, Master flag*/
    if ( !ParseDword(ptcWord, lpCmdLine, tcStorage, &pLinkParmsAdapter->linkParms.dwInstance)
        || !ParseLinkMode(ptcWord, lpCmdLine, tcStorage, &pLinkParmsAdapter->linkParms.eLinkMode)
        || !ParseDword(ptcWord, lpCmdLine, tcStorage, &pLinkParmsAdapter->dwPortPrio)
        || /* parse bMaster */ !ParseString(ptcWord, lpCmdLine, tcStorage) )
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    pLinkParmsAdapter->bMaster = ((*ptcWord)[0] == 'm');

    /* set default parameters */
    pLinkParmsAdapter->bNotUseDmaBuffers = EC_FALSE;
    pLinkParmsAdapter->ePhyInterface = ePHY_GMII;
    pLinkParmsAdapter->eCpswType = eCPSW_AM33XX;
    pLinkParmsAdapter->dwPhyAddr = pLinkParmsAdapter->linkParms.dwInstance - 1; // 0 -> Port1, 1 -> Port2
    
    /* parse optional parameters  */
    
    /* get reference board */
    if (!ParseString(ptcWord, lpCmdLine, tcStorage))
    {
        if (pbGetNextWord)
            *pbGetNextWord = EC_FALSE;
    }
    else
    {
        if (0 == OsStricmp((*ptcWord), "bone"))             { pLinkParmsAdapter->eCpswType = eCPSW_AM33XX; }
        else if (0 == OsStricmp((*ptcWord), "am437x-idk"))  { pLinkParmsAdapter->eCpswType = eCPSW_AM437X; pLinkParmsAdapter->bNotUseDmaBuffers = EC_TRUE; }
        else if (0 == OsStricmp((*ptcWord), "am572x-idk"))  { pLinkParmsAdapter->eCpswType = eCPSW_AM57X; pLinkParmsAdapter->bNotUseDmaBuffers = EC_TRUE; pLinkParmsAdapter->ePhyInterface = ePHY_RGMII; }
        else if (0 == OsStricmp((*ptcWord), "387X_evm"))    { pLinkParmsAdapter->eCpswType = eCPSW_AM387X; }
        else if (0 == OsStricmp((*ptcWord), "custom"))
        { 
            EC_T_DWORD dwNotUseDmaBuffers = 0;
            /* parse CpswType, PHY address, PHY connection type, use DMA */
            if (!ParseCPSWType(ptcWord, lpCmdLine, tcStorage, &pLinkParmsAdapter->eCpswType)
                || !ParseDword(ptcWord, lpCmdLine, tcStorage, &pLinkParmsAdapter->dwPhyAddr)
                || !ParseDword(ptcWord, lpCmdLine, tcStorage, &dwPhyConnection)
                || !ParseDword(ptcWord, lpCmdLine, tcStorage, &dwNotUseDmaBuffers))
            {
                OsDbgMsg("error parsing CPSW parameters for custom board\n");
                dwRetVal = EC_E_INVALIDPARM;
                goto Exit;
            }
            pLinkParmsAdapter->ePhyInterface = (dwPhyConnection == 1) ? ePHY_RGMII : ePHY_GMII;
            pLinkParmsAdapter->bNotUseDmaBuffers = (dwNotUseDmaBuffers == 0) ? EC_FALSE : EC_TRUE;
        }
        else
        {
            OsDbgMsg("Invalid RefBoard value\n");
            dwRetVal = EC_E_INVALIDPARM;
            goto Exit;
        }
    }

    /* verify parameter values */
    if (pLinkParmsAdapter->linkParms.dwInstance < 1 || pLinkParmsAdapter->linkParms.dwInstance > 2)
    {
        OsDbgMsg("Port number must be 1 or 2\n");
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    if (pLinkParmsAdapter->dwPortPrio != 0 && pLinkParmsAdapter->dwPortPrio != 1)
    {
        OsDbgMsg("Port priority must be 0 or 1\n");
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    
    pLinkParmsAdapter->bPhyRestartAutoNegotiation = EC_TRUE;

    /* no errors */
    *ppLinkParms = &pLinkParmsAdapter->linkParms;
    dwRetVal = EC_E_NOERROR;

Exit:
    if (EC_E_NOERROR != dwRetVal)
    {
        SafeOsFree(pLinkParmsAdapter);
    }
    return dwRetVal;
}
#endif /* INCLUDE_CPSW */

#if (defined INCLUDE_EMAC)
/***************************************************************************************************/
/**
\brief  Try to create EMAC link layer parameters according current command line parsing

\return EC_E_NOERROR     if link layer parameters was created
        EC_E_NOTFOUND    if command line was not matching
        EC_E_INVALIDPARM if syntax error
*/
static EC_T_DWORD CreateLinkParmsFromCmdLineEMAC(EC_T_CHAR** ptcWord, EC_T_CHAR** lpCmdLine, EC_T_CHAR* tcStorage, EC_T_BOOL*  pbGetNextWord,
                                                 EC_T_LINK_PARMS** ppLinkParms)
{
EC_T_DWORD dwRetVal = EC_E_ERROR;
EC_T_LINK_PARMS_EMAC* pLinkParmsAdapter = EC_NULL;

    EC_UNREFPARM(pbGetNextWord);

    /* check for matching adapter */
    if (OsStricmp((*ptcWord), "-emac") != 0)
    {
        dwRetVal = EC_E_NOTFOUND;
        goto Exit;
    }
    /* alloc adapter specific link parms */
    pLinkParmsAdapter = (EC_T_LINK_PARMS_EMAC*)OsMalloc(sizeof(EC_T_LINK_PARMS_EMAC));
    if (EC_NULL == pLinkParmsAdapter)
    {
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }
    OsMemset(pLinkParmsAdapter, 0, sizeof(EC_T_LINK_PARMS_EMAC));
    LinkParmsInit(&pLinkParmsAdapter->linkParms, EC_LINK_PARMS_SIGNATURE_EMAC, sizeof(EC_T_LINK_PARMS_EMAC), EC_LINK_PARMS_IDENT_EMAC, 1, EcLinkMode_POLLING);

    /* parse mandatory parameters: instance, mode */
    if ( !ParseDword(ptcWord, lpCmdLine, tcStorage, &pLinkParmsAdapter->linkParms.dwInstance)
        || !ParseLinkMode(ptcWord, lpCmdLine, tcStorage, &pLinkParmsAdapter->linkParms.eLinkMode))
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    pLinkParmsAdapter->dwRegisterBase = 0x86000000;
    pLinkParmsAdapter->dwPhyAddr      = 0;
    pLinkParmsAdapter->dwRxInterrupt  = 1; // from xparameters.h

    /* no errors */
    *ppLinkParms = &pLinkParmsAdapter->linkParms;
    dwRetVal = EC_E_NOERROR;

Exit:
    if (EC_E_NOERROR != dwRetVal)
    {
        SafeOsFree(pLinkParmsAdapter);
    }
    return dwRetVal;
}
#endif /* INCLUDE_EMAC */

#if (defined INCLUDE_ETSEC)
#if (defined _ARCH_PPC)
#define TSEC_CFG_P2020RDB /* Freescale P2020RDB reference board */
#endif
#undef TSEC_CFG_ISTMPC8548 /* Instron "MPC8548 MiniTower" board */
#undef TSEC_CFG_TWRP1025 /* Freescale TWR-P1025 board */
#undef TSEC_CFG_XJ_EPU20C /* XJ Electric Corp */
#if (defined __arm__)
#define TSEC_CFG_TWRLS1021A /* Freescale TWR-L1021A-PB board */
#endif
#if defined(VXWORKS) && ( defined(TSEC_CFG_P2020RDB) || defined(TSEC_CFG_ISTMPC8548) || defined(TSEC_CFG_XJ_EPU20C) )
   extern "C" UINT32 sysGetPeripheralBase (void); /* from sysLib.c */
#  include <hwif/intCtlr/vxbEpicIntCtlr.h>
#endif

/***************************************************************************************************/
/**
\brief  Try to create ETSEC link layer parameters according to current command line parsing

\return EC_E_NOERROR     if link layer parameters was created
        EC_E_NOTFOUND    if command line was not matching
        EC_E_INVALIDPARM if syntax error
*/
static EC_T_DWORD CreateLinkParmsFromCmdLineETSEC(EC_T_CHAR** ptcWord, EC_T_CHAR** lpCmdLine, EC_T_CHAR* tcStorage, EC_T_BOOL*  pbGetNextWord,
                                                  EC_T_LINK_PARMS** ppLinkParms)
{
EC_T_DWORD dwRetVal = EC_E_ERROR;
EC_T_LINK_PARMS_ETSEC* pLinkParmsAdapter = EC_NULL;
EC_T_DWORD dwCcsrbar = 0x0;

    EC_UNREFPARM(pbGetNextWord);

    /* check for matching adapter */
    if (OsStricmp((*ptcWord), "-fsletsec") != 0)
    {
        dwRetVal = EC_E_NOTFOUND;
        goto Exit;
    }
    /* alloc adapter specific link parms */
    pLinkParmsAdapter = (EC_T_LINK_PARMS_ETSEC*)OsMalloc(sizeof(EC_T_LINK_PARMS_ETSEC));
    if (EC_NULL == pLinkParmsAdapter)
    {
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }
    OsMemset(pLinkParmsAdapter, 0, sizeof(EC_T_LINK_PARMS_ETSEC));
    LinkParmsInit(&pLinkParmsAdapter->linkParms, EC_LINK_PARMS_SIGNATURE_ETSEC, sizeof(EC_T_LINK_PARMS_ETSEC), EC_LINK_PARMS_IDENT_ETSEC, 1, EcLinkMode_POLLING);

    /* parse mandatory parameters: instance, mode */
    if ( !ParseDword(ptcWord, lpCmdLine, tcStorage, &pLinkParmsAdapter->linkParms.dwInstance)
        || !ParseLinkMode(ptcWord, lpCmdLine, tcStorage, &pLinkParmsAdapter->linkParms.eLinkMode))
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
#ifdef TSEC_CFG_P2020RDB

    /*
     * The following configuration is for Freescale P2020RDB board
     * (CPU P2020E, VxWorks 6.8 PPC / Linux 3.0.9-PREEMPT PPC)
     */

    if (pLinkParmsAdapter->linkParms.dwInstance > 3)
    {
       OsDbgMsg("Device unit must be <= 3! There are only 3 eTSEC's on P2020RDB.\n");
       dwRetVal = EC_E_INVALIDPARM;
       goto Exit;
    }

    {
       EC_T_BYTE abyStationAddress[] = {0x00, 0x04, 0x9F, 0x01, 0x79, 0x00};
       memcpy(pLinkParmsAdapter->abyStationAddress, abyStationAddress, 6);
    }

#if VXWORKS
    dwCcsrbar = sysGetPeripheralBase();
#elif defined LINUX
    dwCcsrbar = 0xffe00000; /* from p2020si.dtsi */

    /* Get interrupt number from Nth eTSEC device in PowerPC device tree */
    pLinkParmsAdapter->dwRxInterrupt = pLinkParmsAdapter->linkParms.dwInstance - 1;
#endif

    pLinkParmsAdapter->dwPhyMdioBase = dwCcsrbar + 0x24000; /* eTSEC 1 */
    pLinkParmsAdapter->dwTbiPhyAddr = 16; /* Dummy address assigned to internal TBI PHY */
    pLinkParmsAdapter->oMiiBusMtx = EC_NULL; /* LinkOsCreateLock(eLockType_DEFAULT); */

    if (pLinkParmsAdapter->linkParms.dwInstance == 1) // eTSEC1 (4 x 1Gb switchports)
    {
       pLinkParmsAdapter->abyStationAddress[5] = 0x81;
       pLinkParmsAdapter->dwRegisterBase = dwCcsrbar + 0x24000;
       pLinkParmsAdapter->dwPhyAddr = ETSEC_FIXED_LINK; /* RGMII, No Phy, Switchport */
       pLinkParmsAdapter->dwFixedLinkVal = ETSEC_LINKFLAG_1000baseT_Full | ETSEC_LINKFLAG_LINKOK;
#if defined(VXWORKS)
       pLinkParmsAdapter->dwRxInterrupt = EPIC_TSEC1RX_INT_VEC; /* from vxbEpicIntCtlr.h */
#endif
    }
    else if (pLinkParmsAdapter->linkParms.dwInstance == 2) // eTSEC2
    {
       pLinkParmsAdapter->abyStationAddress[5] = 0x82;
       pLinkParmsAdapter->dwRegisterBase = dwCcsrbar + 0x25000;
       pLinkParmsAdapter->dwPhyAddr = 0; /* SGMII Phy on addr. 0 (from P2020RDB Ref. Manual) */
#ifdef VXWORKS
       pLinkParmsAdapter->dwRxInterrupt = EPIC_TSEC2RX_INT_VEC; /* from vxbEpicIntCtlr.h */
#endif
    }
    else if (pLinkParmsAdapter->linkParms.dwInstance == 3) // eTSEC3
    {
       pLinkParmsAdapter->abyStationAddress[5] = 0x83;
       pLinkParmsAdapter->dwRegisterBase = dwCcsrbar + 0x26000;
       pLinkParmsAdapter->dwPhyAddr = 1; /* RGMII Phy on addr. 1 (from P2020RDB Ref. Manual) */
#ifdef VXWORKS
       pLinkParmsAdapter->dwRxInterrupt = EPIC_TSEC3RX_INT_VEC; /* from vxbEpicIntCtlr.h */
#endif
    }

    pLinkParmsAdapter->dwLocalMdioBase = pLinkParmsAdapter->dwRegisterBase;

#elif defined(TSEC_CFG_TWRP1025)
    /*
     * The following configuration is for Freescale TWR-P1025 board
     * (CPU P1025, Freescale-Linuxkernel 3.0.4)
     * Hardware resource informations (phy-addr, interrupts, io-base, ...)
     * are taken from Linux-kernel's device tree for the TWR-P1025 (twr-p1025_32b.dts)
     */

    if (pLinkParmsAdapter->linkParms.dwInstance > 2) /* TWR-P1025 has 3 eTSEC's, but only eTSEC1 and eTSEC3 are routed out to RJ45 ports */
    {
       OsDbgMsg("Device unit must be <= 2! There are only 2 eTSEC's on P1025TWR.\n");
       return EC_FALSE;
    }

    {
       EC_T_BYTE abyStationAddress[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xBA, 0xBE };
       memcpy(pLinkParmsAdapter->abyStationAddress, abyStationAddress, sizeof(pLinkParmsAdapter->abyStationAddress));
    }

    dwCcsrbar = 0xffe00000;

    pLinkParmsAdapter->dwPhyMdioBase = dwCcsrbar + 0x24000; /* eTSEC 1, MDIO */
    pLinkParmsAdapter->dwTbiPhyAddr = 16; /* Dummy address assigned to internal TBI PHY */
    pLinkParmsAdapter->oMiiBusMtx = EC_NULL; /* LinkOsCreateLock(eLockType_DEFAULT); */

    /* Get interrupt number from Nth eTSEC device in PowerPC device tree */
    pLinkParmsAdapter->dwRxInterrupt = dwInstance - 1;

    if (pLinkParmsAdapter->linkParms.dwInstance == 1) // eTSEC1
    {
       pLinkParmsAdapter->dwRegisterBase = dwCcsrbar + 0xB0000; /* eTSEC1, Group 0 */
       pLinkParmsAdapter->dwLocalMdioBase = dwCcsrbar + 0x24000; /* eTSEC1, MDIO */
       pLinkParmsAdapter->dwPhyAddr = 2;
    }
    else if (pLinkParmsAdapter->linkParms.dwInstance == 2) // eTSEC3
    {
       pLinkParmsAdapter->dwRegisterBase = dwCcsrbar + 0xB2000; /* eTSEC3, Group 0 */
       pLinkParmsAdapter->dwLocalMdioBase = dwCcsrbar + 0x26000; /* eTSEC3, MDIO */
       pLinkParmsAdapter->dwPhyAddr = 1;
    }

#elif defined(TSEC_CFG_ISTMPC8548)

    /*
     * The following configuration is for the Instron "MPC8548 MiniTower" board
     * (CPU MPC8548, VxWorks 6.9 PPC)
     */

    if (dwInstance > 2) /* MPC8548 has 4 eTSEC's, but only the first 2 are routed out to RJ45 ports */
    {
       OsDbgMsg("Device unit must be 1 or 2!\n");
       dwRetVal = EC_E_INVALIDPARM;
       goto Exit;
    }

    {
       EC_T_BYTE abyStationAddress[] = { 0x00, 0x02, 0xCE, 0x90, 0x02, 0x24 };
       memcpy(pLinkParmsAdapter->abyStationAddress, abyStationAddress, sizeof(pLinkParmsAdapter->abyStationAddress));
    }

    dwCcsrbar = sysGetPeripheralBase();

    pLinkParmsAdapter->dwPhyMdioBase = dwCcsrbar + 0x24000; /* eTSEC 0 */
    pLinkParmsAdapter->dwTbiPhyAddr = 16; /* Dummy address assigned to internal TBI PHY */
    pLinkParmsAdapter->oMiiBusMtx = EC_NULL; /* LinkOsCreateLock(eLockType_DEFAULT); */

    if (dwInstance == 1) // eTSEC1 (Assigned to VxWorks, don't use!)
    {
       pLinkParmsAdapter->abyStationAddress[5] = 0x24;
       pLinkParmsAdapter->dwRegisterBase = dwCcsrbar + 0x24000;
       pLinkParmsAdapter->dwPhyAddr = 25; /* from hwconf.c */
       pLinkParmsAdapter->dwRxInterrupt = EPIC_TSEC1RX_INT_VEC; /* from vxbEpicIntCtlr.h */
    }
    else if (dwInstance == 2) // eTSEC2 (This one is used by EtherCAT)
    {
       pLinkParmsAdapter->abyStationAddress[5] = 0x25;
       pLinkParmsAdapter->dwRegisterBase = dwCcsrbar + 0x25000;
       pLinkParmsAdapter->dwPhyAddr = 26; /* from hwconf.c */
       pLinkParmsAdapter->dwRxInterrupt = EPIC_TSEC2RX_INT_VEC; /* from vxbEpicIntCtlr.h */
    }

    pLinkParmsAdapter->dwLocalMdioBase = pLinkParmsAdapter->dwRegisterBase;

#elif defined(TSEC_CFG_XJ_EPU20C)

    /*
     * The following configuration is for the Instron "XJ Electric Corp EPU20C" board
     */

    if (pLinkParmsAdapter->linkParms.dwInstance > 2) /* MPC8536 has 2 eTSEC's, but only the first 2 are routed out to RJ45 ports */
    {
       OsDbgMsg("Device unit must be 1 or 2!\n");
       dwRetVal = EC_E_INVALIDPARM;
       goto Exit;
    }

    {
       EC_T_BYTE abyStationAddress[] = { 0x00, 0x02, 0xCE, 0x90, 0x02, 0x24 };
       memcpy(pLinkParmsAdapter->abyStationAddress, abyStationAddress, sizeof(pLinkParmsAdapter->abyStationAddress));
    }

    //dwCcsrbar = CCSR_BASE;
    dwCcsrbar = sysGetPeripheralBase();

    pLinkParmsAdapter->dwPhyMdioBase = dwCcsrbar + 0x24000; /* eTSEC 0 */
    pLinkParmsAdapter->dwTbiPhyAddr = 16; /* Dummy address assigned to internal TBI PHY */
    pLinkParmsAdapter->oMiiBusMtx = EC_NULL; /* LinkOsCreateLock(eLockType_DEFAULT); */

    if (pLinkParmsAdapter->linkParms.dwInstance == 1) // eTSEC1 (Assigned to VxWorks, don't use!)
    {
       pLinkParmsAdapter->abyStationAddress[5] = 0x24;
       pLinkParmsAdapter->dwRegisterBase = dwCcsrbar + 0x24000;
       pLinkParmsAdapter->dwPhyAddr = 0; /* from hwconf.c */
       pLinkParmsAdapter->dwRxInterrupt = EPIC_TSEC1RX_INT_VEC; /* from vxbEpicIntCtlr.h */
    }
    else if (pLinkParmsAdapter->linkParms.dwInstance == 2) // eTSEC2 (This one is used by EtherCAT)
    {
       pLinkParmsAdapter->abyStationAddress[5] = 0x25;
       pLinkParmsAdapter->dwRegisterBase = dwCcsrbar + 0x26000;
       pLinkParmsAdapter->dwPhyAddr = 1; /* from hwconf.c */
       pLinkParmsAdapter->dwRxInterrupt = EPIC_TSEC2RX_INT_VEC; /* from vxbEpicIntCtlr.h */
    }

    pLinkParmsAdapter->dwLocalMdioBase = pLinkParmsAdapter->dwRegisterBase;
#elif defined(TSEC_CFG_TWRLS1021A)
    /*
     * The following configuration is for Freescale TWR-LS1021A-PB board
     * (CPU LS1021A, Freescale-Linuxkernel 3.12)
     * Hardware resource informations (phy-addr, interrupts, io-base, ...)
     * are taken from Linux-kernel's device tree for the TWR-LS1021A (ls1021a.dts and ls1021a.dtsi)
     */

    if (pLinkParmsAdapter->linkParms.dwInstance > 2) /* TWR-LS1021A-PB has 3 eTSEC's, but only two of them can be routed out to RJ45 ports */
    {
       OsDbgMsg("Device unit must be <= 2! There are only 2 eTSEC's on TWR-LS1012A-PB\n");
       dwRetVal = EC_E_INVALIDPARM;
       goto Exit;
    }

    {
       EC_T_BYTE abyStationAddress[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xBA, 0xBE };
       memcpy(pLinkParmsAdapter->abyStationAddress, abyStationAddress, sizeof(pLinkParmsAdapter->abyStationAddress));
    }

    dwCcsrbar = 0x2D00000;

    pLinkParmsAdapter->dwPhyMdioBase = dwCcsrbar + 0x24000; /* eTSEC 1, MDIO */
    pLinkParmsAdapter->dwTbiPhyAddr = 16; /* Dummy address assigned to internal TBI PHY */
    pLinkParmsAdapter->oMiiBusMtx = EC_NULL; /* LinkOsCreateLock(eLockType_DEFAULT); */

    /* Get interrupt number from Nth eTSEC device in PowerPC device tree */
    pLinkParmsAdapter->dwRxInterrupt = pLinkParmsAdapter->linkParms.dwInstance - 1;

#if (defined __arm__)
    pLinkParmsAdapter->bNotUseDmaBuffers = EC_TRUE;
#endif

    if (pLinkParmsAdapter->linkParms.dwInstance == 1) // eTSEC1
    {
        pLinkParmsAdapter->dwRegisterBase = dwCcsrbar + 0x10000; /* eTSEC1, Group 0 */
        pLinkParmsAdapter->dwLocalMdioBase = dwCcsrbar + 0x24000; /* eTSEC1, MDIO */
        pLinkParmsAdapter->dwPhyAddr = 2;
    }
    else if (pLinkParmsAdapter->linkParms.dwInstance == 2) // eTSEC2
    {
        pLinkParmsAdapter->dwRegisterBase = dwCcsrbar + 0x50000; /* eTSEC3, Group 0 */
        pLinkParmsAdapter->dwLocalMdioBase = dwCcsrbar + 0x24000; /* eTSEC3, MDIO */
        pLinkParmsAdapter->dwPhyAddr = 0;
    }
    else if (pLinkParmsAdapter->linkParms.dwInstance == 3) // eTSEC3
    {
        pLinkParmsAdapter->dwRegisterBase = dwCcsrbar + 0x90000; /* eTSEC3, Group 0 */
        pLinkParmsAdapter->dwLocalMdioBase = dwCcsrbar + 0x24000; /* eTSEC3, MDIO */
        pLinkParmsAdapter->dwPhyAddr = 1;
    }
#else
#   error "Board configuration must be selected"
#endif

    /* no errors */
    *ppLinkParms = &pLinkParmsAdapter->linkParms;
    dwRetVal = EC_E_NOERROR;

Exit:
    if (EC_E_NOERROR != dwRetVal)
    {
        SafeOsFree(pLinkParmsAdapter);
    }
    return dwRetVal;
}
#endif /* INCLUDE_ETSEC */

#if (defined INCLUDE_FSLFEC)
/***************************************************************************************************/
/**
\brief  Try to create FslFec link layer parameters according to current command line parsing

\return EC_E_NOERROR     if link layer parameters was created
        EC_E_NOTFOUND    if command line was not matching
        EC_E_INVALIDPARM if syntax error
*/
static EC_T_DWORD CreateLinkParmsFromCmdLineFslFec(EC_T_CHAR** ptcWord, EC_T_CHAR** lpCmdLine, EC_T_CHAR* tcStorage, EC_T_BOOL*  pbGetNextWord,
                                                   EC_T_LINK_PARMS** ppLinkParms)
{
EC_T_DWORD dwRetVal = EC_E_ERROR;
EC_T_LINK_PARMS_FSLFEC* pLinkParmsAdapter = EC_NULL;

    EC_UNREFPARM(pbGetNextWord);

    /* check for matching adapter */
    if (OsStricmp((*ptcWord), "-fslfec") != 0)
    {
        dwRetVal = EC_E_NOTFOUND;
        goto Exit;
    }
    /* alloc adapter specific link parms */
    pLinkParmsAdapter = (EC_T_LINK_PARMS_FSLFEC*)OsMalloc(sizeof(EC_T_LINK_PARMS_FSLFEC));
    if (EC_NULL == pLinkParmsAdapter)
    {
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }
    OsMemset(pLinkParmsAdapter, 0, sizeof(EC_T_LINK_PARMS_FSLFEC));
    LinkParmsInit(&pLinkParmsAdapter->linkParms, EC_LINK_PARMS_SIGNATURE_FSLFEC, sizeof(EC_T_LINK_PARMS_FSLFEC), EC_LINK_PARMS_IDENT_FSLFEC, 1, EcLinkMode_POLLING);

    /* parse mandatory parameters: instance, mode */
    if ( !ParseDword(ptcWord, lpCmdLine, tcStorage, &pLinkParmsAdapter->linkParms.dwInstance)
        || !ParseLinkMode(ptcWord, lpCmdLine, tcStorage, &pLinkParmsAdapter->linkParms.eLinkMode))
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    pLinkParmsAdapter->dwRxBuffers = 96;
    pLinkParmsAdapter->dwTxBuffers = 96;

    /* get reference board */
    (*ptcWord) = GetNextWord(lpCmdLine, tcStorage);
    if      (0 == OsStricmp((*ptcWord), "mars"))     { pLinkParmsAdapter->eFecType = eFEC_IMX6;  pLinkParmsAdapter->ePhyInterface = ePHY_RGMII;      }
    else if (0 == OsStricmp((*ptcWord), "sabrelite")){ pLinkParmsAdapter->eFecType = eFEC_IMX6;  pLinkParmsAdapter->ePhyInterface = ePHY_RGMII;      }
    else if (0 == OsStricmp((*ptcWord), "sabresd"))  { pLinkParmsAdapter->eFecType = eFEC_IMX6;  pLinkParmsAdapter->ePhyInterface = ePHY_RGMII;      }
    else if (0 == OsStricmp((*ptcWord), "imx28evk")) 
            if (pLinkParmsAdapter->linkParms.dwInstance == 1)
                                                     { pLinkParmsAdapter->eFecType = eFEC_IMX28; pLinkParmsAdapter->ePhyInterface = ePHY_RMII;       }
            else                                     { pLinkParmsAdapter->eFecType = eFEC_IMX28; pLinkParmsAdapter->ePhyInterface = ePHY_FIXED_LINK; }
    else if (0 == OsStricmp((*ptcWord), "topaz"))    { pLinkParmsAdapter->eFecType = eFEC_IMX25; pLinkParmsAdapter->ePhyInterface = ePHY_RMII;       }
    else if (0 == OsStricmp((*ptcWord), "custom"))
    {
        /* get FEC type */
        (*ptcWord) = GetNextWord(lpCmdLine, tcStorage);
        if      (0 == OsStricmp((*ptcWord), "imx25")) pLinkParmsAdapter->eFecType = eFEC_IMX25;
        else if (0 == OsStricmp((*ptcWord), "imx28")) pLinkParmsAdapter->eFecType = eFEC_IMX28;
        else if (0 == OsStricmp((*ptcWord), "imx53")) pLinkParmsAdapter->eFecType = eFEC_IMX53;
        else if (0 == OsStricmp((*ptcWord), "imx6"))  pLinkParmsAdapter->eFecType = eFEC_IMX6;
        else if (0 == OsStricmp((*ptcWord), "vf6"))   pLinkParmsAdapter->eFecType = eFEC_VF6;
        else
        {
            OsDbgMsg("Invalid FecType value\n");
            dwRetVal = EC_E_INVALIDPARM;
            goto Exit;
        }
        /* get PHY interface */
        (*ptcWord) = GetNextWord(lpCmdLine, tcStorage);
        if      (0 == OsStricmp((*ptcWord), "fixed")) pLinkParmsAdapter->ePhyInterface = ePHY_FIXED_LINK;
        else if (0 == OsStricmp((*ptcWord), "mii"))   pLinkParmsAdapter->ePhyInterface = ePHY_MII;
        else if (0 == OsStricmp((*ptcWord), "rmii"))  pLinkParmsAdapter->ePhyInterface = ePHY_RMII;
        else if (0 == OsStricmp((*ptcWord), "gmii"))  pLinkParmsAdapter->ePhyInterface = ePHY_GMII;
        else if (0 == OsStricmp((*ptcWord), "sgmii")) pLinkParmsAdapter->ePhyInterface = ePHY_SGMII;
        else if (0 == OsStricmp((*ptcWord), "rgmii")) pLinkParmsAdapter->ePhyInterface = ePHY_RGMII;
        else
        {
            OsDbgMsg("Invalid PhyInterface value\n");
            dwRetVal = EC_E_INVALIDPARM;
            goto Exit;
        }
    }
    else
    {
            OsDbgMsg("Invalid RefBoard value\n");
            dwRetVal = EC_E_INVALIDPARM;
            goto Exit;
    }
    /* no errors */
    *ppLinkParms = &pLinkParmsAdapter->linkParms;
    dwRetVal = EC_E_NOERROR;

Exit:
    if (EC_E_NOERROR != dwRetVal)
    {
        SafeOsFree(pLinkParmsAdapter);
    }
    return dwRetVal;
}
#endif /* INCLUDE_FSLFEC */

#if (defined INCLUDE_GEM)
/***************************************************************************************************/
/**
\brief  Try to create GEM link layer parameters according to current command line parsing

\return EC_E_NOERROR     if link layer parameters was created
        EC_E_NOTFOUND    if command line was not matching
        EC_E_INVALIDPARM if syntax error
*/
static EC_T_DWORD CreateLinkParmsFromCmdLineGEM(EC_T_CHAR** ptcWord, EC_T_CHAR** lpCmdLine, EC_T_CHAR* tcStorage, EC_T_BOOL*  pbGetNextWord,
                                                EC_T_LINK_PARMS** ppLinkParms)
{
EC_T_DWORD dwRetVal = EC_E_ERROR;
EC_T_LINK_PARMS_GEM* pLinkParmsAdapter = EC_NULL;

    EC_UNREFPARM(pbGetNextWord);

    /* check for matching adapter */
    if (OsStricmp((*ptcWord), "-gem") != 0)
    {
        dwRetVal = EC_E_NOTFOUND;
        goto Exit;
    }
    /* alloc adapter specific link parms */
    pLinkParmsAdapter = (EC_T_LINK_PARMS_GEM*)OsMalloc(sizeof(EC_T_LINK_PARMS_GEM));
    if (EC_NULL == pLinkParmsAdapter)
    {
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }
    OsMemset(pLinkParmsAdapter, 0, sizeof(EC_T_LINK_PARMS_GEM));
    LinkParmsInit(&pLinkParmsAdapter->linkParms, EC_LINK_PARMS_SIGNATURE_GEM, sizeof(EC_T_LINK_PARMS_GEM), EC_LINK_PARMS_IDENT_GEM, 1, EcLinkMode_POLLING);

    /* parse mandatory parameters */
    if ( !ParseDword(ptcWord, lpCmdLine, tcStorage, &pLinkParmsAdapter->linkParms.dwInstance)
      || !ParseLinkMode(ptcWord, lpCmdLine, tcStorage, &pLinkParmsAdapter->linkParms.eLinkMode) )
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    /* parse optional parameters or use default values */
    if ( !ParseDword(ptcWord, lpCmdLine, tcStorage, &pLinkParmsAdapter->dwPhyAddr)
      || !ParseDword(ptcWord, lpCmdLine, tcStorage, (EC_T_DWORD*)&pLinkParmsAdapter->eRxSource) )
    {
        if (pLinkParmsAdapter->linkParms.dwInstance == 2)
        {
            pLinkParmsAdapter->eRxSource = eGemRxSource_EMIO;
            pLinkParmsAdapter->dwPhyAddr = 1;
        }
        else
        {
            pLinkParmsAdapter->eRxSource = eGemRxSource_MIO;
            pLinkParmsAdapter->dwPhyAddr = 4;
        }
        if ( pbGetNextWord )
            *pbGetNextWord = EC_FALSE;
    }
    pLinkParmsAdapter->dwRxInterrupt = pLinkParmsAdapter->linkParms.dwInstance - 1;

    if (pLinkParmsAdapter->linkParms.dwInstance < 1 || pLinkParmsAdapter->linkParms.dwInstance > 2)
    {
        OsDbgMsg("Instance number must be 1 or 2\n");
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    pLinkParmsAdapter->bUseDmaBuffers = EC_FALSE;
    pLinkParmsAdapter->bNoPhyAccess = EC_FALSE;   /* Link layer should initialize PHY and read link status (connected/disconnected) */

    /* no errors */
    *ppLinkParms = &pLinkParmsAdapter->linkParms;
    dwRetVal = EC_E_NOERROR;

Exit:
    if (EC_E_NOERROR != dwRetVal)
    {
        SafeOsFree(pLinkParmsAdapter);
    }
    return dwRetVal;
}
#endif /* INCLUDE_GEM */

#if (defined INCLUDE_I8254X)
/***************************************************************************************************/
/**
\brief  Try to create I8254x link layer parameters according to current command line parsing

\return EC_E_NOERROR     if link layer parameters was created
        EC_E_NOTFOUND    if command line was not matching
        EC_E_INVALIDPARM if syntax error
*/
static EC_T_DWORD CreateLinkParmsFromCmdLineI8254x(EC_T_CHAR** ptcWord, EC_T_CHAR** lpCmdLine, EC_T_CHAR* tcStorage, EC_T_BOOL*  pbGetNextWord,
                                                   EC_T_LINK_PARMS** ppLinkParms)
{
EC_T_DWORD dwRetVal = EC_E_ERROR;
EC_T_LINK_PARMS_I8254X* pLinkParmsAdapter = EC_NULL;

    EC_UNREFPARM(pbGetNextWord);

    /* check for matching adapter */
    if (OsStricmp((*ptcWord), "-i8254x") != 0)
    {
        dwRetVal = EC_E_NOTFOUND;
        goto Exit;
    }
    /* alloc adapter specific link parms */
    pLinkParmsAdapter = (EC_T_LINK_PARMS_I8254X*)OsMalloc(sizeof(EC_T_LINK_PARMS_I8254X));
    if (EC_NULL == pLinkParmsAdapter)
    {
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }
    OsMemset(pLinkParmsAdapter, 0, sizeof(EC_T_LINK_PARMS_I8254X));
    LinkParmsInit(&pLinkParmsAdapter->linkParms, EC_LINK_PARMS_SIGNATURE_I8254X, sizeof(EC_T_LINK_PARMS_I8254X), EC_LINK_PARMS_IDENT_I8254X, 1, EcLinkMode_POLLING);

    /* parse mandatory parameters: instance, mode */
    if ( !ParseDword(ptcWord, lpCmdLine, tcStorage, &pLinkParmsAdapter->linkParms.dwInstance)
        || !ParseLinkMode(ptcWord, lpCmdLine, tcStorage, &pLinkParmsAdapter->linkParms.eLinkMode))
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* no errors */
    *ppLinkParms = &pLinkParmsAdapter->linkParms;
    dwRetVal = EC_E_NOERROR;

Exit:
    if (EC_E_NOERROR != dwRetVal)
    {
        SafeOsFree(pLinkParmsAdapter);
    }
    return dwRetVal;
}
#endif /* INCLUDE_I8254X */

#if (defined INCLUDE_I8255X)
/***************************************************************************************************/
/**
\brief  Try to create I8255X link layer parameters according to current command line parsing

\return EC_E_NOERROR     if link layer parameters was created
        EC_E_NOTFOUND    if command line was not matching
        EC_E_INVALIDPARM if syntax error
*/
static EC_T_DWORD CreateLinkParmsFromCmdLineI8255x(EC_T_CHAR** ptcWord, EC_T_CHAR** lpCmdLine, EC_T_CHAR* tcStorage, EC_T_BOOL*  pbGetNextWord,
                                                   EC_T_LINK_PARMS** ppLinkParms)
{
EC_T_DWORD dwRetVal = EC_E_ERROR;
EC_T_LINK_PARMS_I8255X* pLinkParmsAdapter = EC_NULL;

    EC_UNREFPARM(pbGetNextWord);

    /* check for matching adapter */
    if (OsStricmp((*ptcWord), "-i8255x") != 0)
    {
        dwRetVal = EC_E_NOTFOUND;
        goto Exit;
    }
    /* alloc adapter specific link parms */
    pLinkParmsAdapter = (EC_T_LINK_PARMS_I8255X*)OsMalloc(sizeof(EC_T_LINK_PARMS_I8255X));
    if (EC_NULL == pLinkParmsAdapter)
    {
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }
    OsMemset(pLinkParmsAdapter, 0, sizeof(EC_T_LINK_PARMS_I8255X));
    LinkParmsInit(&pLinkParmsAdapter->linkParms, EC_LINK_PARMS_SIGNATURE_I8255X, sizeof(EC_T_LINK_PARMS_I8255X), EC_LINK_PARMS_IDENT_I8255X, 1, EcLinkMode_POLLING);

    /* parse mandatory parameters: instance, mode */
    if ( !ParseDword(ptcWord, lpCmdLine, tcStorage, &pLinkParmsAdapter->linkParms.dwInstance)
        || !ParseLinkMode(ptcWord, lpCmdLine, tcStorage, &pLinkParmsAdapter->linkParms.eLinkMode))
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* no errors */
    *ppLinkParms = &pLinkParmsAdapter->linkParms;
    dwRetVal = EC_E_NOERROR;

Exit:
    if (EC_E_NOERROR != dwRetVal)
    {
        SafeOsFree(pLinkParmsAdapter);
    }
    return dwRetVal;
}
#endif /* INCLUDE_I8255X */

#if (defined INCLUDE_ICSS)
/***************************************************************************************************/
/**
\brief  Try to create ICSS link layer parameters according to current command line parsing

\return EC_E_NOERROR     if link layer parameters was created
EC_E_NOTFOUND    if command line was not matching
EC_E_INVALIDPARM if syntax error
*/
static EC_T_DWORD CreateLinkParmsFromCmdLineICSS(EC_T_CHAR** ptcWord, EC_T_CHAR** lpCmdLine, EC_T_CHAR* tcStorage, EC_T_BOOL*  pbGetNextWord,
    EC_T_LINK_PARMS** ppLinkParms)
{
    EC_T_DWORD dwRetVal = EC_E_ERROR;
    EC_T_LINK_PARMS_ICSS* pLinkParmsAdapter = EC_NULL;

    EC_UNREFPARM(pbGetNextWord);

    /* check for matching adapter */
    if (OsStricmp((*ptcWord), "-icss") != 0)
    {
        dwRetVal = EC_E_NOTFOUND;
        goto Exit;
    }
    /* alloc adapter specific link parms */
    pLinkParmsAdapter = (EC_T_LINK_PARMS_ICSS*)OsMalloc(sizeof(EC_T_LINK_PARMS_ICSS));
    if (EC_NULL == pLinkParmsAdapter)
    {
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }
    OsMemset(pLinkParmsAdapter, 0, sizeof(EC_T_LINK_PARMS_ICSS));
    LinkParmsInit(&pLinkParmsAdapter->linkParms, EC_LINK_PARMS_SIGNATURE_ICSS, sizeof(EC_T_LINK_PARMS_ICSS), EC_LINK_PARMS_IDENT_ICSS, 1, EcLinkMode_POLLING);

    if ( !ParseDword(ptcWord, lpCmdLine, tcStorage, &pLinkParmsAdapter->linkParms.dwInstance)
      || !ParseLinkMode(ptcWord, lpCmdLine, tcStorage, &pLinkParmsAdapter->linkParms.eLinkMode) 
      || !ParseDword(ptcWord, lpCmdLine, tcStorage, &pLinkParmsAdapter->dwPort))
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    if (pLinkParmsAdapter->linkParms.dwInstance < 1 || pLinkParmsAdapter->linkParms.dwInstance > 2)
    {
        OsDbgMsg("Instance number must be 1 or 2\n");
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    if (pLinkParmsAdapter->dwPort != 0 && pLinkParmsAdapter->dwPort != 1)
    {
        OsDbgMsg("Port number must be 0 or 1\n");
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    /* no errors */
    *ppLinkParms = &pLinkParmsAdapter->linkParms;
    dwRetVal = EC_E_NOERROR;

Exit:
    if (EC_E_NOERROR != dwRetVal)
    {
        SafeOsFree(pLinkParmsAdapter);
    }
    return dwRetVal;
}
#endif /* INCLUDE_ICSS */

#if (defined INCLUDE_L9218I)
/***************************************************************************************************/
/**
\brief  Try to create L9218I link layer parameters according to current command line parsing

\return EC_E_NOERROR     if link layer parameters was created
        EC_E_NOTFOUND    if command line was not matching
        EC_E_INVALIDPARM if syntax error
*/
static EC_T_DWORD CreateLinkParmsFromCmdLineL9218i(EC_T_CHAR** ptcWord, EC_T_CHAR** lpCmdLine, EC_T_CHAR* tcStorage, EC_T_BOOL*  pbGetNextWord,
                                                   EC_T_LINK_PARMS** ppLinkParms)
{
EC_T_DWORD dwRetVal = EC_E_ERROR;
EC_T_LINK_PARMS_L9218I* pLinkParmsAdapter = EC_NULL;

    EC_UNREFPARM(ptcWord);
    EC_UNREFPARM(lpCmdLine);
    EC_UNREFPARM(tcStorage);
    EC_UNREFPARM(pbGetNextWord);

    /* check for matching adapter */
    if (OsStricmp((*ptcWord), "-l9218i") != 0)
    {
        dwRetVal = EC_E_NOTFOUND;
        goto Exit;
    }
    /* alloc adapter specific link parms */
    pLinkParmsAdapter = (EC_T_LINK_PARMS_L9218I*)OsMalloc(sizeof(EC_T_LINK_PARMS_L9218I));
    if (EC_NULL == pLinkParmsAdapter)
    {
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }
    OsMemset(pLinkParmsAdapter, 0, sizeof(EC_T_LINK_PARMS_L9218I));
    LinkParmsInit(&pLinkParmsAdapter->linkParms, EC_LINK_PARMS_SIGNATURE_L9218I, sizeof(EC_T_LINK_PARMS_L9218I), EC_LINK_PARMS_IDENT_L9218I, 1, EcLinkMode_POLLING);

    /* get mode */
    if (!ParseLinkMode(ptcWord, lpCmdLine, tcStorage, &pLinkParmsAdapter->linkParms.eLinkMode))
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* no errors */
    *ppLinkParms = &pLinkParmsAdapter->linkParms;
    dwRetVal = EC_E_NOERROR;

Exit:
    if (EC_E_NOERROR != dwRetVal)
    {
        SafeOsFree(pLinkParmsAdapter);
    }
    return dwRetVal;
}
#endif /* INCLUDE_L9218I */

#if (defined INCLUDE_NDISUIO)
/***************************************************************************************************/
/**
\brief  Try to create NDISUIO link layer parameters according to current command line parsing

\return EC_E_NOERROR     if link layer parameters was created
        EC_E_NOTFOUND    if command line was not matching
        EC_E_INVALIDPARM if syntax error
*/
static EC_T_DWORD CreateLinkParmsFromCmdLineNDISUIO(EC_T_CHAR** ptcWord, EC_T_CHAR** lpCmdLine, EC_T_CHAR* tcStorage, EC_T_BOOL*  pbGetNextWord,
                                                    EC_T_LINK_PARMS** ppLinkParms)
{
EC_T_DWORD dwRetVal = EC_E_ERROR;
EC_T_LINK_PARMS_NDISUIO* pLinkParmsAdapter = EC_NULL;

#define NDISUIO_DEVNAME   TEXT("ECT1:")
#define NDISUIO_DRIVERKEY TEXT("Drivers\\BuiltIn\\ECAT")
HANDLE      hNdisUioDevice  = EC_NULL;
HANDLE      hNdisUioDriver  = EC_NULL;

    EC_UNREFPARM(pbGetNextWord);

    /* check for matching adapter */
    if (OsStricmp((*ptcWord), "-ndisuio") != 0)
    {
        dwRetVal = EC_E_NOTFOUND;
        goto Exit;
    }
    /* get next word */
    (*ptcWord) = GetNextWord(lpCmdLine, tcStorage);
    if (((*ptcWord) == EC_NULL) || (OsStrncmp( (*ptcWord), "-", 1) == 0))
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* alloc adapter specific link parms */
    pLinkParmsAdapter = (EC_T_LINK_PARMS_NDISUIO*)OsMalloc(sizeof(EC_T_LINK_PARMS_NDISUIO));
    if (EC_NULL == pLinkParmsAdapter)
    {
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }
    OsMemset(pLinkParmsAdapter, 0, sizeof(EC_T_LINK_PARMS_NDISUIO));
    LinkParmsInit(&pLinkParmsAdapter->linkParms, EC_LINK_PARMS_SIGNATURE_NDISUIO, sizeof(EC_T_LINK_PARMS_NDISUIO), EC_LINK_PARMS_IDENT_NDISUIO, 1, EcLinkMode_POLLING);

    /* check if NDISUIO driver started */
    OsDbgMsg("Check if ECAT driver is started...");
    hNdisUioDevice = CreateFile( NDISUIO_DEVNAME, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0);
    if ((hNdisUioDevice != EC_NULL) && (hNdisUioDevice != INVALID_HANDLE_VALUE))
    {
        OsDbgMsg("Ok!\n");
    }
    else
    {
        OsDbgMsg("Failed!\n");
        hNdisUioDevice = EC_NULL;
    }
    /* try to load driver if not already loaded */
    if (hNdisUioDevice == EC_NULL)
    {
        OsDbgMsg("Try to load ECAT driver...");
        hNdisUioDriver = ActivateDeviceEx (NDISUIO_DRIVERKEY, 0,  0, 0);
        if ((hNdisUioDriver != EC_NULL) && (hNdisUioDriver != INVALID_HANDLE_VALUE))
        {
            OsDbgMsg("Ok!\n");
        }
        else
        {
            OsDbgMsg("Failed!\n");
            hNdisUioDriver = EC_NULL;
            dwRetVal = EC_E_INVALIDSTATE;
            goto Exit;
        }
    }
    /* check if driver is available */
    if ((hNdisUioDevice == EC_NULL) && (hNdisUioDriver == EC_NULL))
    {
        OsDbgMsg("No NDISUIO ECAT driver available!!!\n" );
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    else if (hNdisUioDevice != EC_NULL)
    {
        /* close handle, it was just for the check */
        CloseHandle(hNdisUioDevice);
        hNdisUioDevice = EC_NULL;
    }
    /* NdisUio uses the network adapter name to select the appropriate network interface */
#ifdef  UNICODE
    _snwprintf((wchar_t*)pLinkParmsAdapter->szNetworkAdapterName, MAX_LEN_NDISUIO_ADAPTER_NAME, L"%S", (*ptcWord));
#else
    _snwprintf(pLinkParmsAdapter->szNetworkAdapterName, MAX_LEN_NDISUIO_ADAPTER_NAME, L"%s", (*ptcWord));
#endif
    /* no errors */
    *ppLinkParms = &pLinkParmsAdapter->linkParms;
    dwRetVal = EC_E_NOERROR;

Exit:
    if (EC_E_NOERROR != dwRetVal)
    {
        SafeOsFree(pLinkParmsAdapter);
    }
    return dwRetVal;
}
#endif /* INCLUDE_NDISUIO */

#if (defined INCLUDE_PXAMFU)
/***************************************************************************************************/
/**
\brief  Try to create PXAMFU link layer parameters according to current command line parsing

\return EC_E_NOERROR     if link layer parameters was created
        EC_E_NOTFOUND    if command line was not matching
        EC_E_INVALIDPARM if syntax error
*/
static EC_T_DWORD CreateLinkParmsFromCmdLinePXAMFU(EC_T_CHAR** ptcWord, EC_T_CHAR** lpCmdLine, EC_T_CHAR* tcStorage, EC_T_BOOL*  pbGetNextWord,
                                                   EC_T_LINK_PARMS** ppLinkParms)
{
EC_T_DWORD dwRetVal = EC_E_ERROR;
EC_T_LINK_PARMS_PXAMFU* pLinkParmsAdapter = EC_NULL;

    EC_UNREFPARM(pbGetNextWord);

    /* check for matching adapter */
    if (OsStricmp((*ptcWord), "-pxamfu") != 0)
    {
        dwRetVal = EC_E_NOTFOUND;
        goto Exit;
    }
    /* alloc adapter specific link parms */
    pLinkParmsAdapter = (EC_T_LINK_PARMS_PXAMFU*)OsMalloc(sizeof(EC_T_LINK_PARMS_PXAMFU));
    if (EC_NULL == pLinkParmsAdapter)
    {
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }
    OsMemset(pLinkParmsAdapter, 0, sizeof(EC_T_LINK_PARMS_PXAMFU));
    LinkParmsInit(&pLinkParmsAdapter->linkParms, EC_LINK_PARMS_SIGNATURE_PXAMFU, sizeof(EC_T_LINK_PARMS_PXAMFU), EC_LINK_PARMS_IDENT_PXAMFU, 1, EcLinkMode_POLLING);

    /* parse mandatory parameters: instance, mode */
    if ( !ParseDword(ptcWord, lpCmdLine, tcStorage, &pLinkParmsAdapter->linkParms.dwInstance)
        || !ParseLinkMode(ptcWord, lpCmdLine, tcStorage, &pLinkParmsAdapter->linkParms.eLinkMode))
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* no errors */
    *ppLinkParms = &pLinkParmsAdapter->linkParms;
    dwRetVal = EC_E_NOERROR;

Exit:
    if (EC_E_NOERROR != dwRetVal)
    {
        SafeOsFree(pLinkParmsAdapter);
    }
    return dwRetVal;
}
#endif /* INCLUDE_PXAMFU */

#if (defined  INCLUDE_R6040)
/***************************************************************************************************/
/**
\brief  Try to create R6040 link layer parameters according to current command line parsing

\return EC_E_NOERROR     if link layer parameters was created
        EC_E_NOTFOUND    if command line was not matching
        EC_E_INVALIDPARM if syntax error
*/
static EC_T_DWORD CreateLinkParmsFromCmdLineR6040(EC_T_CHAR** ptcWord, EC_T_CHAR** lpCmdLine, EC_T_CHAR* tcStorage, EC_T_BOOL*  pbGetNextWord,
                                                  EC_T_LINK_PARMS** ppLinkParms)
{
EC_T_DWORD dwRetVal = EC_E_ERROR;
EC_T_LINK_PARMS_R6040* pLinkParmsAdapter = EC_NULL;

    EC_UNREFPARM(pbGetNextWord);

    /* check for matching adapter */
    if (OsStricmp((*ptcWord), "-r6040") != 0)
    {
        dwRetVal = EC_E_NOTFOUND;
        goto Exit;
    }
    /* alloc adapter specific link parms */
    pLinkParmsAdapter = (EC_T_LINK_PARMS_R6040*)OsMalloc(sizeof(EC_T_LINK_PARMS_R6040));
    if (EC_NULL == pLinkParmsAdapter)
    {
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }
    OsMemset(pLinkParmsAdapter, 0, sizeof(EC_T_LINK_PARMS_R6040));
    LinkParmsInit(&pLinkParmsAdapter->linkParms, EC_LINK_PARMS_SIGNATURE_R6040, sizeof(EC_T_LINK_PARMS_R6040), EC_LINK_PARMS_IDENT_R6040, 1, EcLinkMode_POLLING);

    /* parse mandatory parameters: instance, mode */
    if ( !ParseDword(ptcWord, lpCmdLine, tcStorage, &pLinkParmsAdapter->linkParms.dwInstance)
        || !ParseLinkMode(ptcWord, lpCmdLine, tcStorage, &pLinkParmsAdapter->linkParms.eLinkMode))
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* no errors */
    *ppLinkParms = &pLinkParmsAdapter->linkParms;
    dwRetVal = EC_E_NOERROR;

Exit:
    if (EC_E_NOERROR != dwRetVal)
    {
        SafeOsFree(pLinkParmsAdapter);
    }
    return dwRetVal;
}
#endif /* INCLUDE_R6040 */

#if (defined EC_VERSION_RIN32M3)
/***************************************************************************************************/
/**
\brief  Try to create RIN32M3 link layer parameters according to current command line parsing

\return EC_E_NOERROR     if link layer parameters was created
        EC_E_NOTFOUND    if command line was not matching
        EC_E_INVALIDPARM if syntax error
*/
static EC_T_DWORD CreateLinkParmsFromCmdLineRIN32M3(EC_T_CHAR** ptcWord, EC_T_CHAR** lpCmdLine, EC_T_CHAR* tcStorage, EC_T_BOOL*  pbGetNextWord,
                                                    EC_T_LINK_PARMS** ppLinkParms)
{
EC_T_DWORD dwRetVal = EC_E_ERROR;
EC_T_LINK_PARMS_RIN32M3* pLinkParmsAdapter = EC_NULL;
const size_t nParmsSize = sizeof(EC_T_LINK_PARMS_RIN32M3);

    EC_UNREFPARM(lpCmdLine);
    EC_UNREFPARM(tcStorage);
    EC_UNREFPARM(pbGetNextWord);

    /* check for matching adapter */
    if (OsStricmp( (*ptcWord), "-rin32m3") != 0)
    {
        dwRetVal = EC_E_NOTFOUND;
        goto Exit;
    }
    /* alloc adapter specific link parms */
    pLinkParmsAdapter = (EC_T_LINK_PARMS_RIN32M3*)OsMalloc(nParmsSize);
    if (EC_NULL == pLinkParmsAdapter)
    {
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }
    OsMemset(pLinkParmsAdapter, 0, nParmsSize);
    LinkParmsInit(&pLinkParmsAdapter->linkParms, EC_LINK_PARMS_SIGNATURE_RIN32M3, nParmsSize, EC_LINK_PARMS_IDENT_RIN32M3, 1, EcLinkMode_POLLING);

    /* no errors */
    *ppLinkParms = &pLinkParmsAdapter->linkParms;
    dwRetVal = EC_E_NOERROR;

Exit:
    if (EC_E_NOERROR != dwRetVal)
    {
        SafeOsFree(pLinkParmsAdapter);
    }
    return dwRetVal;
}
#endif /* EC_VERSION_RIN32M3 */

#if (defined INCLUDE_RTL8139)
/***************************************************************************************************/
/**
\brief  Try to create RTL8139 link layer parameters according to current command line parsing

\return EC_E_NOERROR     if link layer parameters was created
        EC_E_NOTFOUND    if command line was not matching
        EC_E_INVALIDPARM if syntax error
*/
static EC_T_DWORD CreateLinkParmsFromCmdLineRTL8139(EC_T_CHAR** ptcWord, EC_T_CHAR** lpCmdLine, EC_T_CHAR* tcStorage, EC_T_BOOL*  pbGetNextWord,
                                                    EC_T_LINK_PARMS** ppLinkParms)
{
EC_T_DWORD dwRetVal = EC_E_ERROR;
EC_T_LINK_PARMS_RTL8139* pLinkParmsAdapter = EC_NULL;

    EC_UNREFPARM(pbGetNextWord);

    /* check for matching adapter */
    if (OsStricmp((*ptcWord), "-rtl8139") != 0)
    {
        dwRetVal = EC_E_NOTFOUND;
        goto Exit;
    }
    /* alloc adapter specific link parms */
    pLinkParmsAdapter = (EC_T_LINK_PARMS_RTL8139*)OsMalloc(sizeof(EC_T_LINK_PARMS_RTL8139));
    if (EC_NULL == pLinkParmsAdapter)
    {
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }
    OsMemset(pLinkParmsAdapter, 0, sizeof(EC_T_LINK_PARMS_RTL8139));
    LinkParmsInit(&pLinkParmsAdapter->linkParms, EC_LINK_PARMS_SIGNATURE_RTL8139, sizeof(EC_T_LINK_PARMS_RTL8139), EC_LINK_PARMS_IDENT_RTL8139, 1, EcLinkMode_POLLING);

    /* parse mandatory parameters: instance, mode */
    if ( !ParseDword(ptcWord, lpCmdLine, tcStorage, &pLinkParmsAdapter->linkParms.dwInstance)
        || !ParseLinkMode(ptcWord, lpCmdLine, tcStorage, &pLinkParmsAdapter->linkParms.eLinkMode))
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* no errors */
    *ppLinkParms = &pLinkParmsAdapter->linkParms;
    dwRetVal = EC_E_NOERROR;

Exit:
    if (EC_E_NOERROR != dwRetVal)
    {
        SafeOsFree(pLinkParmsAdapter);
    }
    return dwRetVal;
}
#endif /* INCLUDE_RTL8139 */

#if (defined INCLUDE_RTL8169)
/***************************************************************************************************/
/**
\brief  Try to create RTL8169 link layer parameters according to current command line parsing

\return EC_E_NOERROR     if link layer parameters was created
        EC_E_NOTFOUND    if command line was not matching
        EC_E_INVALIDPARM if syntax error
*/
static EC_T_DWORD CreateLinkParmsFromCmdLineRTL8169(EC_T_CHAR** ptcWord, EC_T_CHAR** lpCmdLine, EC_T_CHAR* tcStorage, EC_T_BOOL*  pbGetNextWord,
                                                    EC_T_LINK_PARMS** ppLinkParms)
{
EC_T_DWORD dwRetVal = EC_E_ERROR;
EC_T_LINK_PARMS_RTL8169* pLinkParmsAdapter = EC_NULL;

    EC_UNREFPARM(pbGetNextWord);

    /* check for matching adapter */
    if (OsStricmp((*ptcWord), "-rtl8169") != 0)
    {
        dwRetVal = EC_E_NOTFOUND;
        goto Exit;
    }
    /* alloc adapter specific link parms */
    pLinkParmsAdapter = (EC_T_LINK_PARMS_RTL8169*)OsMalloc(sizeof(EC_T_LINK_PARMS_RTL8169));
    if (EC_NULL == pLinkParmsAdapter)
    {
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }
    OsMemset(pLinkParmsAdapter, 0, sizeof(EC_T_LINK_PARMS_RTL8169));
    LinkParmsInit(&pLinkParmsAdapter->linkParms, EC_LINK_PARMS_SIGNATURE_RTL8169, sizeof(EC_T_LINK_PARMS_RTL8169), EC_LINK_PARMS_IDENT_RTL8169, 1, EcLinkMode_POLLING);

    if (!ParseDword(ptcWord, lpCmdLine, tcStorage, &pLinkParmsAdapter->linkParms.dwInstance)
       || !ParseLinkMode(ptcWord, lpCmdLine, tcStorage, &pLinkParmsAdapter->linkParms.eLinkMode))
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    pLinkParmsAdapter->bNotUseDmaBuffers = EC_FALSE;
#if (defined _ARM_) || (defined __arm__)
    /* for arm platform we should not use DMA memory because any unaligned access creates crash */
    pLinkParmsAdapter->bNotUseDmaBuffers = EC_TRUE;
#endif

    /* no errors */
    *ppLinkParms = &pLinkParmsAdapter->linkParms;
    dwRetVal = EC_E_NOERROR;

Exit:
    if (EC_E_NOERROR != dwRetVal)
    {
        SafeOsFree(pLinkParmsAdapter);
    }
    return dwRetVal;
}
#endif /* INCLUDE_RTL8169 */

#if (defined EC_VERSION_RZT1)
/***************************************************************************************************/
/**
\brief  Try to create RZT1 link layer parameters according to current command line parsing

\return EC_E_NOERROR     if link layer parameters was created
EC_E_NOTFOUND    if command line was not matching
EC_E_INVALIDPARM if syntax error
*/
static EC_T_DWORD CreateLinkParmsFromCmdLineRZT1(EC_T_CHAR** ptcWord, EC_T_CHAR** lpCmdLine, EC_T_CHAR* tcStorage, EC_T_BOOL*  pbGetNextWord,
    EC_T_LINK_PARMS** ppLinkParms)
{
    EC_T_DWORD dwRetVal = EC_E_ERROR;
    EC_T_LINK_PARMS_RZT1* pLinkParmsAdapter = EC_NULL;
    const size_t nParmsSize = sizeof(EC_T_LINK_PARMS_RZT1);

    EC_UNREFPARM(lpCmdLine);
    EC_UNREFPARM(tcStorage);
    EC_UNREFPARM(pbGetNextWord);

    /* check for matching adapter */
    if (OsStricmp( (*ptcWord), "-rzt1") != 0)
    {
        dwRetVal = EC_E_NOTFOUND;
        goto Exit;
    }
    /* alloc adapter specific link parms */
    pLinkParmsAdapter = (EC_T_LINK_PARMS_RZT1*)OsMalloc(nParmsSize);
    if (EC_NULL == pLinkParmsAdapter)
    {
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }
    OsMemset(pLinkParmsAdapter, 0, nParmsSize);
    LinkParmsInit(&pLinkParmsAdapter->linkParms, EC_LINK_PARMS_SIGNATURE_RZT1, nParmsSize, EC_LINK_PARMS_IDENT_RZT1, 1, EcLinkMode_POLLING);

    /* parse mandatory parameters */
    if (!ParseDword(ptcWord, lpCmdLine, tcStorage, &pLinkParmsAdapter->linkParms.dwInstance))
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    if (pLinkParmsAdapter->linkParms.dwInstance < 1 || pLinkParmsAdapter->linkParms.dwInstance > 2)
    {
        OsDbgMsg("Instance number must be 1 or 2\n");
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    /* no errors */
    *ppLinkParms = &pLinkParmsAdapter->linkParms;
    dwRetVal = EC_E_NOERROR;

Exit:
    if (EC_E_NOERROR != dwRetVal)
    {
        SafeOsFree(pLinkParmsAdapter);
    }
    return dwRetVal;
}
#endif /* EC_VERSION_RZT1 */

#if (defined INCLUDE_SNARF)
/***************************************************************************************************/
/**
\brief  Try to create Snarf link layer parameters according to current command line parsing

\return EC_E_NOERROR     if link layer parameters was created
        EC_E_NOTFOUND    if command line was not matching
        EC_E_INVALIDPARM if syntax error
*/
static EC_T_DWORD CreateLinkParmsFromCmdLineSnarf(EC_T_CHAR** ptcWord, EC_T_CHAR** lpCmdLine, EC_T_CHAR* tcStorage, EC_T_BOOL*  pbGetNextWord,
                                                  EC_T_LINK_PARMS** ppLinkParms)
{
EC_T_DWORD dwRetVal = EC_E_ERROR;
EC_T_LINK_PARMS_SNARF* pLinkParmsAdapter = EC_NULL;

    EC_UNREFPARM(pbGetNextWord);

    /* check for matching adapter */
    if (OsStricmp((*ptcWord), "-snarf") != 0)
    {
        dwRetVal = EC_E_NOTFOUND;
        goto Exit;
    }
    /* get next word */
    (*ptcWord) = GetNextWord(lpCmdLine, tcStorage);
    if (((*ptcWord) == EC_NULL) || (OsStrncmp( (*ptcWord), "-", 1) == 0))
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* alloc adapter specific link parms */
    pLinkParmsAdapter = (EC_T_LINK_PARMS_SNARF*)OsMalloc(sizeof(EC_T_LINK_PARMS_SNARF));
    if (EC_NULL == pLinkParmsAdapter)
    {
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }
    OsMemset(pLinkParmsAdapter, 0, sizeof(EC_T_LINK_PARMS_SNARF));
    LinkParmsInit(&pLinkParmsAdapter->linkParms, EC_LINK_PARMS_SIGNATURE_SNARF, sizeof(EC_T_LINK_PARMS_SNARF), EC_LINK_PARMS_IDENT_SNARF, 1, EcLinkMode_POLLING);

    /* get adapter name */
    OsSnprintf(pLinkParmsAdapter->szAdapterName, MAX_LEN_SNARF_ADAPTER_NAME, "%s", (*ptcWord));
    
    /* no errors */
    *ppLinkParms = &pLinkParmsAdapter->linkParms;
    dwRetVal = EC_E_NOERROR;

Exit:
    if (EC_E_NOERROR != dwRetVal)
    {
        SafeOsFree(pLinkParmsAdapter);
    }
    return dwRetVal;
}
#endif /* INCLUDE_SNARF */

#if (defined INCLUDE_SHETH)
/***************************************************************************************************/
/**
\brief  Try to create Super H link layer parameters according to current command line parsing

\return EC_E_NOERROR     if link layer parameters was created
        EC_E_NOTFOUND    if command line was not matching
        EC_E_INVALIDPARM if syntax error
*/
static EC_T_DWORD CreateLinkParmsFromCmdLineSHEth(EC_T_CHAR** ptcWord, EC_T_CHAR** lpCmdLine, EC_T_CHAR* tcStorage, EC_T_BOOL*  pbGetNextWord,
                                                  EC_T_LINK_PARMS** ppLinkParms)
{
EC_T_DWORD dwRetVal = EC_E_ERROR;
EC_T_LINK_PARMS_SHETH* pLinkParmsAdapter = EC_NULL;

    EC_UNREFPARM(pbGetNextWord);

    /* check for matching adapter */
    if (OsStricmp((*ptcWord), "-sheth") != 0)
    {
        dwRetVal = EC_E_NOTFOUND;
        goto Exit;
    }
    /* get next word */
    (*ptcWord) = GetNextWord(lpCmdLine, tcStorage);
    if (((*ptcWord) == EC_NULL) || (OsStrncmp( (*ptcWord), "-", 1) == 0))
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* alloc adapter specific link parms */
    pLinkParmsAdapter = (EC_T_LINK_PARMS_SHETH*)OsMalloc(sizeof(EC_T_LINK_PARMS_SHETH));
    if (EC_NULL == pLinkParmsAdapter)
    {
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }
    OsMemset(pLinkParmsAdapter, 0, sizeof(EC_T_LINK_PARMS_SHETH));
    LinkParmsInit(&pLinkParmsAdapter->linkParms, EC_LINK_PARMS_SIGNATURE_SHETH, sizeof(EC_T_LINK_PARMS_SHETH), EC_LINK_PARMS_IDENT_SHETH, 1, EcLinkMode_POLLING);

    /* parse mandatory parameters */
    if (!ParseDword(ptcWord, lpCmdLine, tcStorage, &pLinkParmsAdapter->linkParms.dwInstance))
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    if (pLinkParmsAdapter->linkParms.dwInstance < 1 || pLinkParmsAdapter->linkParms.dwInstance > 2)
    {
        OsDbgMsg("Instance number must be 1 or 2\n");
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    /* get reference board */
    (*ptcWord) = GetNextWord(lpCmdLine, tcStorage);
    if (0 == OsStricmp((*ptcWord), "a800eva"))
    {
        pLinkParmsAdapter->eType = eSHEth_R8A7740;
        pLinkParmsAdapter->dwBaseAddr = 0xE9A00000;
        pLinkParmsAdapter->byPhyAddr = 0;
    }
    else if (0 == OsStricmp((*ptcWord), "rzg1e"))
    {
        pLinkParmsAdapter->eType = eSHEth_R8A77450;
        pLinkParmsAdapter->dwBaseAddr = 0xEE700000;
        pLinkParmsAdapter->byPhyAddr = 1;
    }
    else
    {
        OsDbgMsg("Invalid RefBoard value\n");
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    {
       EC_T_BYTE abyStationAddress[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xBA, 0xBE };
       memcpy(pLinkParmsAdapter->abyStationAddress, abyStationAddress, sizeof(pLinkParmsAdapter->abyStationAddress));
    }

    pLinkParmsAdapter->bNotUseDmaBuffers = EC_TRUE;

    /* no errors */
    *ppLinkParms = &pLinkParmsAdapter->linkParms;
    dwRetVal = EC_E_NOERROR;

Exit:
    if (EC_E_NOERROR != dwRetVal)
    {
        SafeOsFree(pLinkParmsAdapter);
    }
    return dwRetVal;
}
#endif /* INCLUDE_SHETH */

#if (defined INCLUDE_SOCKRAW)
/***************************************************************************************************/
/**
\brief  Try to create SockRaw link layer parameters according to current command line parsing

\return EC_E_NOERROR     if link layer parameters was created
        EC_E_NOTFOUND    if command line was not matching
        EC_E_INVALIDPARM if syntax error
*/
static EC_T_DWORD CreateLinkParmsFromCmdLineSockRaw(EC_T_CHAR** ptcWord, EC_T_CHAR** lpCmdLine, EC_T_CHAR* tcStorage, EC_T_BOOL*  pbGetNextWord,
                                                    EC_T_LINK_PARMS** ppLinkParms)
{
EC_T_DWORD dwRetVal = EC_E_ERROR;
EC_T_LINK_PARMS_SOCKRAW* pLinkParmsAdapter = EC_NULL;

    EC_UNREFPARM(pbGetNextWord);

    /* check for matching adapter */
    if (OsStricmp((*ptcWord), "-sockraw") != 0)
    {
        dwRetVal = EC_E_NOTFOUND;
        goto Exit;
    }
    /* alloc adapter specific link parms */
    pLinkParmsAdapter = (EC_T_LINK_PARMS_SOCKRAW*)OsMalloc(sizeof(EC_T_LINK_PARMS_SOCKRAW));
    if (EC_NULL == pLinkParmsAdapter)
    {
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }
    OsMemset(pLinkParmsAdapter, 0, sizeof(EC_T_LINK_PARMS_SOCKRAW));
    LinkParmsInit(&pLinkParmsAdapter->linkParms, EC_LINK_PARMS_SIGNATURE_SOCKRAW, sizeof(EC_T_LINK_PARMS_SOCKRAW), EC_LINK_PARMS_IDENT_SOCKRAW, 1, EcLinkMode_POLLING);

    /* get adapter name */
    *ptcWord = GetNextWord(lpCmdLine, tcStorage);
    if ((*ptcWord == EC_NULL) || (OsStrncmp(*ptcWord, "-", 1) == 0))
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    if (OsStrlen((char*)*ptcWord) > MAX_LEN_SOCKRAW_ADAPTER_NAME - 1)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    OsStrncpy(pLinkParmsAdapter->szAdapterName, (char*)*ptcWord, MAX_LEN_SOCKRAW_ADAPTER_NAME - 1);

    /* Do not overwrite destination in frame with FF:FF:FF:FF:FF:FF */
    pLinkParmsAdapter->bDisableForceBroadcast = EC_TRUE;

    /* no errors */
    *ppLinkParms = &pLinkParmsAdapter->linkParms;
    dwRetVal = EC_E_NOERROR;

Exit:
    if (EC_E_NOERROR != dwRetVal)
    {
        SafeOsFree(pLinkParmsAdapter);
    }
    return dwRetVal;
}
#endif /* INCLUDE_SOCKRAW */

#if (defined INCLUDE_WINPCAP)
/***************************************************************************************************/
/**
\brief  Try to create WinPcap link layer parameters according to current command line parsing

\return EC_E_NOERROR     if link layer parameters was created
        EC_E_NOTFOUND    if command line was not matching
        EC_E_INVALIDPARM if syntax error
*/
static EC_T_DWORD CreateLinkParmsFromCmdLineWinPcap(EC_T_CHAR** ptcWord, EC_T_CHAR** lpCmdLine, EC_T_CHAR* tcStorage, EC_T_BOOL*  pbGetNextWord,
                                                    EC_T_LINK_PARMS** ppLinkParms)
{
EC_T_DWORD dwRetVal = EC_E_ERROR;
EC_T_LINK_PARMS_WINPCAP* pLinkParmsAdapter = EC_NULL;

    EC_UNREFPARM(pbGetNextWord);

    /* check for matching adapter */
    if (OsStricmp((*ptcWord), "-winpcap") != 0)
    {
        dwRetVal = EC_E_NOTFOUND;
        goto Exit;
    }
    /* alloc adapter specific link parms */
    pLinkParmsAdapter = (EC_T_LINK_PARMS_WINPCAP*)OsMalloc(sizeof(EC_T_LINK_PARMS_WINPCAP));
    if (EC_NULL == pLinkParmsAdapter)
    {
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }
    OsMemset(pLinkParmsAdapter, 0, sizeof(EC_T_LINK_PARMS_WINPCAP));
    LinkParmsInit(&pLinkParmsAdapter->linkParms, EC_LINK_PARMS_SIGNATURE_WINPCAP, sizeof(EC_T_LINK_PARMS_WINPCAP), EC_LINK_PARMS_IDENT_WINPCAP, 1, EcLinkMode_POLLING);

    /* parse the specified IP address */
    (*ptcWord) = OsStrtok(EC_NULL, ".");
    if (((*ptcWord) == EC_NULL) || (OsStrncmp( (*ptcWord), "-", 1) == 0))
    {
        OsDbgMsg("CreateLinkParmsFromCmdLineWinPcap: IP address missing!\n");
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    if (!ParseIpAddress(*ptcWord, pLinkParmsAdapter->abyIpAddress))
    {
        OsDbgMsg("CreateLinkParmsFromCmdLineWinPcap: Error parsing IP address!\n");
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* get mode */
    if (!ParseLinkMode(ptcWord, lpCmdLine, tcStorage, &pLinkParmsAdapter->linkParms.eLinkMode))
    {
        OsDbgMsg("CreateLinkParmsFromCmdLineWinPcap: Error parsing LinkMode!\n");
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* no errors */
    *ppLinkParms = &pLinkParmsAdapter->linkParms;
    dwRetVal = EC_E_NOERROR;

Exit:
    if (EC_E_NOERROR != dwRetVal)
    {
        SafeOsFree(pLinkParmsAdapter);
    }
    return dwRetVal;
}
#endif /* INCLUDE_WINPCAP */

#if (defined INCLUDE_DW3504)
/***************************************************************************************************/
/**
\brief  Try to create DW3504 link layer parameters according to current command line parsing

\return EC_E_NOERROR     if link layer parameters was created
EC_E_NOTFOUND    if command line was not matching
EC_E_INVALIDPARM if syntax error
*/
static EC_T_DWORD CreateLinkParmsFromCmdLineDW3504(EC_T_CHAR** ptcWord, EC_T_CHAR** lpCmdLine, EC_T_CHAR* tcStorage, EC_T_BOOL*  pbGetNextWord,
    EC_T_LINK_PARMS** ppLinkParms)
{
    EC_T_DWORD dwRetVal = EC_E_ERROR;
    EC_T_LINK_PARMS_DW3504* pLinkParmsAdapter = EC_NULL;

    EC_UNREFPARM(pbGetNextWord);

    /* check for matching adapter */
    if (OsStricmp((*ptcWord), "-DW3504") != 0)
    {
        dwRetVal = EC_E_NOTFOUND;
        goto Exit;
    }
    /* alloc adapter specific link parms */
    pLinkParmsAdapter = (EC_T_LINK_PARMS_DW3504*)OsMalloc(sizeof(EC_T_LINK_PARMS_DW3504));
    if (EC_NULL == pLinkParmsAdapter)
    {
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }
    OsMemset(pLinkParmsAdapter, 0, sizeof(EC_T_LINK_PARMS_DW3504));
    LinkParmsInit(&pLinkParmsAdapter->linkParms, EC_LINK_PARMS_SIGNATURE_DW3504, sizeof(EC_T_LINK_PARMS_DW3504), EC_LINK_PARMS_IDENT_DW3504, 1, EcLinkMode_POLLING);

    /* parse mandatory parameters */
    if (!ParseDword(ptcWord, lpCmdLine, tcStorage, &pLinkParmsAdapter->linkParms.dwInstance)
        || !ParseLinkMode(ptcWord, lpCmdLine, tcStorage, &pLinkParmsAdapter->linkParms.eLinkMode)
        || !ParseDword(ptcWord, lpCmdLine, tcStorage, &pLinkParmsAdapter->dwPhyAddr))
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    if (pLinkParmsAdapter->linkParms.dwInstance == 1)
        pLinkParmsAdapter->dwRegisterBasePhys = 0xFF700000;
    else if (pLinkParmsAdapter->linkParms.dwInstance == 2)
        pLinkParmsAdapter->dwRegisterBasePhys = 0xFF702000;
    else
    {
        OsDbgMsg("Instance number must be 1 or 2\n");
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    /* set default values */
    pLinkParmsAdapter->bNotUseDmaBuffers = EC_FALSE;
    pLinkParmsAdapter->ePhyInterface = ePHY_RGMII;

    /* no errors */
    *ppLinkParms = &pLinkParmsAdapter->linkParms;
    dwRetVal = EC_E_NOERROR;

Exit:
    if (EC_E_NOERROR != dwRetVal)
    {
        SafeOsFree(pLinkParmsAdapter);
    }
    return dwRetVal;
}
#endif /* INCLUDE_DW3504 */

#if (defined  INCLUDE_DUMMY)
/***************************************************************************************************/
/**
\brief  Try to create dummy link layer parameters according to current command line parsing

\return EC_E_NOERROR     if link layer parameters was created
EC_E_NOTFOUND    if command line was not matching
EC_E_INVALIDPARM if syntax error
*/
static EC_T_DWORD CreateLinkParmsFromCmdLineDummy(EC_T_CHAR** ptcWord, EC_T_CHAR** lpCmdLine, EC_T_CHAR* tcStorage, EC_T_BOOL*  pbGetNextWord,
    EC_T_LINK_PARMS** ppLinkParms)
{
    EC_T_DWORD dwRetVal = EC_E_ERROR;
    EC_T_LINK_PARMS_DUMMY* pLinkParmsAdapter = EC_NULL;

    EC_UNREFPARM(pbGetNextWord);

    /* check for matching adapter */
    if (OsStricmp((*ptcWord), "-dummy") != 0)
    {
        dwRetVal = EC_E_NOTFOUND;
        goto Exit;
    }
    /* alloc adapter specific link parms */
    pLinkParmsAdapter = (EC_T_LINK_PARMS_DUMMY*)OsMalloc(sizeof(EC_T_LINK_PARMS_DUMMY));
    if (EC_NULL == pLinkParmsAdapter)
    {
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }
    OsMemset(pLinkParmsAdapter, 0, sizeof(EC_T_LINK_PARMS_DUMMY));
    LinkParmsInit(&pLinkParmsAdapter->linkParms, EC_LINK_PARMS_SIGNATURE_DUMMY, sizeof(EC_T_LINK_PARMS_DUMMY), EC_LINK_PARMS_IDENT_DUMMY, 1, EcLinkMode_POLLING);

    /* get Instance and Mode */
    if (!ParseDword(ptcWord, lpCmdLine, tcStorage, &pLinkParmsAdapter->linkParms.dwInstance)
     || !ParseLinkMode(ptcWord, lpCmdLine, tcStorage, &pLinkParmsAdapter->linkParms.eLinkMode))
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    /* no errors */
    *ppLinkParms = &pLinkParmsAdapter->linkParms;
    dwRetVal = EC_E_NOERROR;

Exit:
    if (EC_E_NOERROR != dwRetVal)
    {
        SafeOsFree(pLinkParmsAdapter);
    }
    return dwRetVal;
}
#endif /* INCLUDE_R6040 */

/***************************************************************************************************/
/**
\brief  Select Link Layer.

This function checks whether parameter portion is a LinkLayer information and processes it
\return EC_TRUE if parameter is LinkLayer Portion, EC_FALSE otherwise.
*/
EC_T_DWORD CreateLinkParmsFromCmdLine
(   EC_T_CHAR**     ptcWord,
    EC_T_CHAR**     lpCmdLine,
    EC_T_CHAR*      tcStorage,
    EC_T_BOOL*      pbGetNextWord,  /* [out]  Shows that next parameter should be read or not */
    EC_T_LINK_PARMS** ppLinkParms
)
{
EC_T_DWORD dwRetVal = EC_E_NOTFOUND;

#if (defined INCLUDE_CCAT)
    dwRetVal = CreateLinkParmsFromCmdLineCCAT(ptcWord, lpCmdLine, tcStorage, pbGetNextWord, ppLinkParms);
    if (EC_E_NOTFOUND != dwRetVal)
    {
        goto Exit;
    }
#endif
#if (defined INCLUDE_EG20T)
    dwRetVal = CreateLinkParmsFromCmdLineEG20T(ptcWord, lpCmdLine, tcStorage, pbGetNextWord, ppLinkParms);
    if (EC_E_NOTFOUND != dwRetVal)
    {
        goto Exit;
    }
#endif
#if (defined INCLUDE_CPSW)
    dwRetVal = CreateLinkParmsFromCmdLineCPSW(ptcWord, lpCmdLine, tcStorage, pbGetNextWord, ppLinkParms);
    if (EC_E_NOTFOUND != dwRetVal)
    {
        goto Exit;
    }
#endif
#if (defined INCLUDE_EMAC)
    dwRetVal = CreateLinkParmsFromCmdLineEMAC(ptcWord, lpCmdLine, tcStorage, pbGetNextWord, ppLinkParms);
    if (EC_E_NOTFOUND != dwRetVal)
    {
        goto Exit;
    }
#endif
#if (defined INCLUDE_ETSEC)
    dwRetVal = CreateLinkParmsFromCmdLineETSEC(ptcWord, lpCmdLine, tcStorage, pbGetNextWord, ppLinkParms);
    if (EC_E_NOTFOUND != dwRetVal)
    {
        goto Exit;
    }
#endif
#if (defined INCLUDE_FSLFEC)
    dwRetVal = CreateLinkParmsFromCmdLineFslFec(ptcWord, lpCmdLine, tcStorage, pbGetNextWord, ppLinkParms);
    if (EC_E_NOTFOUND != dwRetVal)
    {
        goto Exit;
    }
#endif
#if (defined INCLUDE_GEM)
    dwRetVal = CreateLinkParmsFromCmdLineGEM(ptcWord, lpCmdLine, tcStorage, pbGetNextWord, ppLinkParms);
    if (EC_E_NOTFOUND != dwRetVal)
    {
        goto Exit;
    }
#endif
#if (defined INCLUDE_I8254X)
    dwRetVal = CreateLinkParmsFromCmdLineI8254x(ptcWord, lpCmdLine, tcStorage, pbGetNextWord, ppLinkParms);
    if (EC_E_NOTFOUND != dwRetVal)
    {
        goto Exit;
    }
#endif
#if (defined INCLUDE_I8255X)
    dwRetVal = CreateLinkParmsFromCmdLineI8255x(ptcWord, lpCmdLine, tcStorage, pbGetNextWord, ppLinkParms);
    if (EC_E_NOTFOUND != dwRetVal)
    {
        goto Exit;
    }
#endif
#if (defined INCLUDE_ICSS)
    dwRetVal = CreateLinkParmsFromCmdLineICSS(ptcWord, lpCmdLine, tcStorage, pbGetNextWord, ppLinkParms);
    if (EC_E_NOTFOUND != dwRetVal)
    {
        goto Exit;
    }
#endif
#if (defined INCLUDE_L9218I)
    dwRetVal = CreateLinkParmsFromCmdLineL9218i(ptcWord, lpCmdLine, tcStorage, pbGetNextWord, ppLinkParms);
    if (EC_E_NOTFOUND != dwRetVal)
    {
        goto Exit;
    }
#endif
#if (defined INCLUDE_NDISUIO)
    dwRetVal = CreateLinkParmsFromCmdLineNDISUIO(ptcWord, lpCmdLine, tcStorage, pbGetNextWord, ppLinkParms);
    if (EC_E_NOTFOUND != dwRetVal)
    {
        goto Exit;
    }
#endif
#if (defined INCLUDE_PXAMFU)
    dwRetVal = CreateLinkParmsFromCmdLinePXAMFU(ptcWord, lpCmdLine, tcStorage, pbGetNextWord, ppLinkParms);
    if (EC_E_NOTFOUND != dwRetVal)
    {
        goto Exit;
    }
#endif
#if (defined INCLUDE_R6040)
    dwRetVal = CreateLinkParmsFromCmdLineR6040(ptcWord, lpCmdLine, tcStorage, pbGetNextWord, ppLinkParms);
    if (EC_E_NOTFOUND != dwRetVal)
    {
        goto Exit;
    }
#endif
#if (defined EC_VERSION_RIN32M3)
    dwRetVal = CreateLinkParmsFromCmdLineRIN32M3(ptcWord, lpCmdLine, tcStorage, pbGetNextWord, ppLinkParms);
    if (EC_E_NOTFOUND != dwRetVal)
    {
        goto Exit;
    }
#endif
#if (defined INCLUDE_RTL8139)
    dwRetVal = CreateLinkParmsFromCmdLineRTL8139(ptcWord, lpCmdLine, tcStorage, pbGetNextWord, ppLinkParms);
    if (EC_E_NOTFOUND != dwRetVal)
    {
        goto Exit;
    }
#endif
#if (defined INCLUDE_RTL8169)
    dwRetVal = CreateLinkParmsFromCmdLineRTL8169(ptcWord, lpCmdLine, tcStorage, pbGetNextWord, ppLinkParms);
    if (EC_E_NOTFOUND != dwRetVal)
    {
        goto Exit;
    }
#endif
#if (defined EC_VERSION_RZT1)
    dwRetVal = CreateLinkParmsFromCmdLineRZT1(ptcWord, lpCmdLine, tcStorage, pbGetNextWord, ppLinkParms);
    if (EC_E_NOTFOUND != dwRetVal)
    {
        goto Exit;
    }
#endif
#if (defined INCLUDE_SNARF)
    dwRetVal = CreateLinkParmsFromCmdLineSnarf(ptcWord, lpCmdLine, tcStorage, pbGetNextWord, ppLinkParms);
    if (EC_E_NOTFOUND != dwRetVal)
    {
        goto Exit;
    }
#endif
#if (defined INCLUDE_SHETH)
    dwRetVal = CreateLinkParmsFromCmdLineSHEth(ptcWord, lpCmdLine, tcStorage, pbGetNextWord, ppLinkParms);
    if (EC_E_NOTFOUND != dwRetVal)
    {
        goto Exit;
    }
#endif
#if (defined INCLUDE_SOCKRAW)
    dwRetVal = CreateLinkParmsFromCmdLineSockRaw(ptcWord, lpCmdLine, tcStorage, pbGetNextWord, ppLinkParms);
    if (EC_E_NOTFOUND != dwRetVal)
    {
        goto Exit;
    }
#endif
#if (defined INCLUDE_WINPCAP)
    dwRetVal = CreateLinkParmsFromCmdLineWinPcap(ptcWord, lpCmdLine, tcStorage, pbGetNextWord, ppLinkParms);
    if (EC_E_NOTFOUND != dwRetVal)
    {
        goto Exit;
    }
#endif
#if (defined INCLUDE_DW3504)
    dwRetVal = CreateLinkParmsFromCmdLineDW3504(ptcWord, lpCmdLine, tcStorage, pbGetNextWord, ppLinkParms);
    if (EC_E_NOTFOUND != dwRetVal)
    {
        goto Exit;
    }
#endif
#if (defined INCLUDE_DUMMY)
    dwRetVal = CreateLinkParmsFromCmdLineDummy(ptcWord, lpCmdLine, tcStorage, pbGetNextWord, ppLinkParms);
    if (EC_E_NOTFOUND != dwRetVal)
    {
        goto Exit;
    }
#endif
Exit:
    return dwRetVal;
}

EC_T_VOID ShowLinkLayerSyntax1(EC_T_VOID)
{
    OsDbgMsg (""
#if (defined INCLUDE_CCAT)
    " [-ccat Instance Mode]"
#endif
#if (defined INCLUDE_CPSW)
    " [-cpsw Instance Mode PortPriority MasterFlag [RefBoard CpswType PhyAddress PhyConnectionMode NotUseDmaBuffers]]"
#endif
#if (defined INCLUDE_DW3504)
    " [-dw3504 Instance Mode PhyAddress]"
#endif
#if (defined INCLUDE_EG20T)
    " [-eg20t Instance Mode]"
#endif
#if (defined INCLUDE_EMAC)
    " [-emac Instance Mode]"
#endif
#if (defined INCLUDE_ETSEC)
    " [-fsletsec Instance Mode]"
#endif
#if (defined INCLUDE_FSLFEC)
    " [-fslfec Instance Mode RefBoard [FecType PhyInterface]]"
#endif
#if (defined INCLUDE_GEM)
    " [-gem Instance Mode [PhyAddress PhyConnectionMode] ]"
#endif
#if (defined INCLUDE_I8254X)
    " [-i8254x Instance Mode]"
#endif
#if (defined INCLUDE_I8255X)
    " [-i8255x Instance Mode]"
#endif
#if (defined INCLUDE_ICSS)
    " [-icss Instance Mode Port]"
#endif
#if (defined INCLUDE_L9218I)
    " [-l9218i Mode]"
#endif
#if (defined INCLUDE_NDISUIO)
    " [-ndisuio Adapter]"
#endif
#if (defined INCLUDE_PXAMFU)
    " [-pxamfu Instance Mode]"
#endif
#if (defined INCLUDE_R6040)
    "[-r6040] "
#endif
#if (defined EC_VERSION_RIN32M3)
    "[-rin32m3] "
#endif
#if (defined INCLUDE_RTL8139)
    " [-rtl8139 Instance Mode]"
#endif
#if (defined INCLUDE_RTL8169)
    " [-rtl8169 Instance Mode]"
#endif
#if (defined EC_VERSION_RZT1)
    "[-rzt1 Instance] "
#endif
#if (defined INCLUDE_SHETH)
    " [-sheth Instance Mode RefBoard]"
#endif
#if (defined INCLUDE_SNARF)
    " [-snarf AdapterName]"
#endif
#if (defined INCLUDE_SOCKRAW)
    " [-SockRaw device]"
#endif
#if (defined INCLUDE_WINPCAP)
    " [-winpcap IpAddress Mode]"
#endif
#if (defined INCLUDE_DW3504)
    " [-dw3504 Instance Mode PhyAddress]"
#endif
#if (defined INCLUDE_DUMMY)
    " [-dummy Instance Mode]"
#endif
    "\n");
}

EC_T_VOID ShowLinkLayerSyntax2(EC_T_VOID)
{
#if (defined INCLUDE_CCAT)
    OsDbgMsg("   -ccat             Link layer = Beckhoff CCAT\n");
    OsDbgMsg("     Instance        Device instance (1=first), ex. 1\n");
    OsDbgMsg("     Mode            Interrupt (0) or Polling (1) mode\n");
#endif
#if (defined INCLUDE_CPSW)
    OsDbgMsg("   -cpsw             Link layer = Texas Instruments Common Platform Switch (CPSW)\n");
    OsDbgMsg("     Instance        Device instance (port) 1 P1, 2 P2\n");
    OsDbgMsg("     Mode            Interrupt (0) or Polling (1)\n");
    OsDbgMsg("     PortPriority    Low priority (0) or high priority (1)\n");
    OsDbgMsg("     MasterFlag      (m) Master (Initialize Switch), (s) Slave\n");
    OsDbgMsg("     RefBoard:       custom | bone | am437x-idk | am572x-idk | 387X_evm (optional)\n");
    OsDbgMsg("      if custom       CpswType: am33XX | am437X | am57X | am387X\n");
    OsDbgMsg("      if custom       PhyAddress 0 .. 31\n");
    OsDbgMsg("      if custom       PhyConnection GMII (0) or RGMII (1)\n");
    OsDbgMsg("      if custom       NotUseDmaBuffers FALSE (0) or TRUE (1)\n");
#endif
#if (defined INCLUDE_DW3504)
    OsDbgMsg("   -dw3504           Link layer = Synopsys DesignWare 3504-0 Universal 10/100/1000 Ethernet MAC (DW3504)\n");
    OsDbgMsg("     Instance        Device instance 1 for emac0, 2 for emac1\n");
    OsDbgMsg("     Mode            Interrupt (0) or Polling (1)\n");
    OsDbgMsg("     PhyAddress      0 .. 31\n");
    OsDbgMsg("     SoCrates board: Instance=2, PhyAddress=1\n");
#endif
#if (defined INCLUDE_DUMMY)
    OsDbgMsg("   -dummy            Link layer = Dummy\n");
    OsDbgMsg("     Instance        Device instance (1=first), ex. 1\n");
    OsDbgMsg("     Mode            Interrupt (0) or Polling (1) mode\n");
#endif
#if (defined INCLUDE_EG20T)
    OsDbgMsg("   -eg20t            Link layer = EG20T Gigabit Ethernet Controller\n");
    OsDbgMsg("     Instance        Device instance (1=first), ex. 1\n");
    OsDbgMsg("     Mode            Interrupt (0) or Polling (1) mode\n");
#endif
#if (defined INCLUDE_EMAC)
    OsDbgMsg("   -emac             Link layer = Xilinx LogiCORE IP XPS EMAC\n");
    OsDbgMsg("     Instance        Device instance (must be 1)\n");
    OsDbgMsg("     Mode            Interrupt (0) or Polling (1) mode\n");
#endif
#if (defined INCLUDE_ETSEC)
    OsDbgMsg("   -fsletsec         Link layer = Freescale TSEC / eTSEC V1 / eTSEC V2 (VeTSEC)\n");
    OsDbgMsg("     Instance        Device instance (1=first), ex. 1\n");
    OsDbgMsg("     Mode            Interrupt (0) or Polling (1) mode\n");
#endif
#if (defined INCLUDE_FSLFEC)
    OsDbgMsg("   -fslfec           Link layer = Freescale FEC\n");
    OsDbgMsg("     Instance        Device instance (1=first), ex. 1\n");
    OsDbgMsg("     Mode            Interrupt (0) or Polling (1) mode\n");
    OsDbgMsg("     RefBoard: custom | mars | sabrelite | sabresd | imx28evk | topaz\n");
    OsDbgMsg("     if custom FecType: imx25 | imx28 | imx53 | imx6 | vf6\n");
    OsDbgMsg("     if custom PhyInterface: fixed | mii | rmii | gmii | sgmii | rgmii\n");
#endif
#if (defined INCLUDE_GEM)
    OsDbgMsg("   -gem             Link layer = Xilinx Zynq-7000 (GEM)\n");    
    OsDbgMsg("     Instance          Device instance 1 for GEM0, 2 for GEM1\n");
    OsDbgMsg("     Mode              Interrupt (0) or Polling (1)\n");
    OsDbgMsg("     PhyAddress        0 .. 31\n");
    OsDbgMsg("     PhyConnectionMode MIO (0) or EMIO (1)\n");
#endif
#if (defined INCLUDE_I8254X)
    OsDbgMsg("   -i8254x           Link layer = Intel 8254x\n");
    OsDbgMsg("     Instance        Device instance (1=first), ex. 1\n");
    OsDbgMsg("     Mode            Interrupt (0) or Polling (1) mode\n");
#endif
#if (defined INCLUDE_I8255X)
    OsDbgMsg("   -i8255x           Link layer = Intel 8255x\n");
    OsDbgMsg("     Instance        Device instance (1=first), ex. 1\n");
    OsDbgMsg("     Mode            Interrupt (0) or Polling (1) mode\n");
#endif
#if (defined INCLUDE_ICSS)
    OsDbgMsg("   -icss             Link layer = Texas Instruments industrial communications subsystem Ethernet Media Access Controller (ICSS)\n");
    OsDbgMsg("     Instance        Device instance PRU1 (1) or PRU2 (2)\n");
    OsDbgMsg("     Mode            Interrupt (0) or Polling (1) mode\n");
    OsDbgMsg("     Port            Port inside each PRU ETH0 (0) or ETH1 (1)\n");
#endif
#if (defined INCLUDE_L9218I)
    OsDbgMsg("   -l9218i           Link layer = SMSC LAN9218i/LAN9221\n");
    OsDbgMsg("     Mode            Interrupt (0) or Polling (1)\n");
#endif
#if (defined INCLUDE_NDISUIO)
    OsDbgMsg("   -ndisuio          Link layer = NdisUio\n");
    OsDbgMsg("     Adapter         Device name (registry), ex. PCI\\RTL81391\n");
#endif
#if (defined INCLUDE_R6040)
    OsDbgMsg("   -r6040            Link layer = R6040\n");
#endif
#if (defined EC_VERSION_RIN32M3)
    OsDbgMsg("   -rin32m3         Link layer = RIN32M3\n");
#endif
#if (defined INCLUDE_RTL8139)
    OsDbgMsg("   -rtl8139          Link layer = Realtek RTL8139\n");
    OsDbgMsg("     Instance        Device instance (1=first), ex. 1\n");
    OsDbgMsg("     Mode            Interrupt (0) or Polling (1) mode\n");
#endif
#if (defined INCLUDE_RTL8169)
    OsDbgMsg("   -rtl8169          Link layer = Realtek RTL8169 / RTL8168 / RTL8111\n");
    OsDbgMsg("     Instance        Device instance (1=first), ex. 1\n");
    OsDbgMsg("     Mode            Interrupt (0) or Polling (1) mode\n");
#endif
#if (defined EC_VERSION_RZT1)
    OsDbgMsg("   -rzt1             Link layer = RZT1\n");
    OsDbgMsg("     Instance        Device instance 1 for Port 0 or 2 for Port 1\n");
#endif
#if (defined INCLUDE_SHETH)
    OsDbgMsg("   -sheth            Link layer = Super H Etherner controller\n");
    OsDbgMsg("     Instance        Device instance (1=first), ex. 1\n");
    OsDbgMsg("     Mode            Interrupt (0) or Polling (1) mode\n");
    OsDbgMsg("     RefBoard:       a800eva | rzg1e\n");
#endif
#if (defined INCLUDE_SNARF)
    OsDbgMsg("   -snarf            Link layer = SNARF link layer device statically loaded\n");
    OsDbgMsg("    AdapterName      Adapter name, ex. fei0\n");
#endif
#if (defined INCLUDE_SOCKRAW)
    OsDbgMsg("   -SockRaw          Link layer = raw socket\n");
    OsDbgMsg("     device          network device (e.g. eth1)\n");
#endif
#if (defined INCLUDE_WINPCAP)
    OsDbgMsg("   -winpcap          Link layer = WinPcap/NPF\n");
    OsDbgMsg("     IpAddress       IP address of network adapter card, ex. 192.168.157.2\n");
    OsDbgMsg("                     NPF only: 255.255.255.x, x = network adapter number (1,2,...)\n");
    OsDbgMsg("     Mode            Interrupt (0) or Polling (1) mode\n");
#endif
}

/*-END OF SOURCE FILE--------------------------------------------------------*/
