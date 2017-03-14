/*-----------------------------------------------------------------------------
 * AtEmRasError.h
 * Copyright                acontis technologies GmbH, Weingarten, Germany
 * Response                 Paul Bussmann
 * Description              RAS error structures
 *---------------------------------------------------------------------------*/

#ifndef INC_ATEMRASERROR
#define INC_ATEMRASERROR 1

/*-INCLUDES------------------------------------------------------------------*/
#include <EcOs.h>
#include <EcError.h>

/*-COMPILER SETTINGS---------------------------------------------------------*/
#ifdef __cplusplus
extern "C"
{
#endif

/*-TYPEDEFS------------------------------------------------------------------*/
#include EC_PACKED_INCLUDESTART(1)
typedef struct _ATEMRAS_T_CONNOTIFYDESC
{
    EC_T_DWORD      dwCause;    /**< [in]   Connection change type */
    EC_T_DWORD      dwCookie;   /**< [in]   Cookie of connection */
} EC_PACKED(1) ATEMRAS_T_CONNOTIFYDESC, *ATEMRAS_PT_CONNOTIFYDESC;

typedef struct _ATEMRAS_T_REGNOTIFYDESC
{
    EC_T_DWORD      dwCookie;       /**< [in]   Cookie of connection */
    EC_T_DWORD      dwResult;       /**< [in]   Result of call */
    EC_T_DWORD      dwInstanceId;   /**< [in]   ID master instance */
    EC_T_DWORD      dwClientId;     /**< [in]   ID of un- / registered client */
} EC_PACKED(1) ATEMRAS_T_REGNOTIFYDESC, *ATEMRAS_PT_REGNOTIFYDESC;

typedef struct _ATEMRAS_T_MARSHALERRORDESC
{
    EC_T_DWORD      dwCookie;       /**< [in]   Cookie of faulting connection */
    EC_T_DWORD      dwCause;        /**< [in]   Cause of fault */
    EC_T_DWORD      dwLenStatCmd;   /**< [in]   Faulty Protocol Header */
    EC_T_DWORD      dwCommandCode;  /**< [in]   Faulting command code */
} EC_PACKED(1) ATEMRAS_T_MARSHALERRORDESC, *ATEMRAS_PT_MARSHALERRORDESC;

typedef struct _ATEMRAS_T_NONOTIFYMEMORYDESC
{
    EC_T_DWORD      dwCookie;       /**< [in]   Cookie of faulting connection */
    EC_T_DWORD      dwCode;         /**< [in]   Fault causing notification code */
} EC_PACKED(1) ATEMRAS_T_NONOTIFYMEMORYDESC, *ATEMRAS_PT_NONOTIFYMEMORYDESC;
#include EC_PACKED_INCLUDESTOP

/*-FUNCTION DECLARATION------------------------------------------------------*/
ATECAT_API const EC_T_CHAR*  emRasErrorText(EC_T_DWORD dwError);
ATECAT_API const EC_T_CHAR*  emRasEventText(EC_T_DWORD dwEvent);

/*-COMPILER SETTINGS---------------------------------------------------------*/
#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* ATEMRASERROR_H */

/*-END OF SOURCE FILE--------------------------------------------------------*/
