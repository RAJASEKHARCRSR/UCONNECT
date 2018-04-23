#ifndef _BRCM_4G_CAPI_EXPORTS_H
#define _BRCM_4G_CAPI_EXPORTS_H
#ifdef WIN32
#include <windows.h>
#include <stdio.h>
#endif

#include "4G_CAPI_brcm.h"
#include "brcm_4g_capi_defines.h"

typedef BRCM_CAPI_VOID(*P_FN_CALLBACK) (BRCM_CAPI_UINT uiLength, P_ST_CALLBACK_INFO pCallbackbuf);

#ifdef WIN32
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT
void __attribute__ ((constructor)) CAPI_init(void);
void __attribute__ ((destructor)) CAPI_exit(void);
#endif
#ifdef __cplusplus
extern "C" {
#endif
    E_ERROR_DEFNS DLL_EXPORT opencmd(IN ST_OPEN_PARAM * pstOpenParams,
                IN P_FN_CALLBACK p_fn_callback_function_addr);


    E_ERROR_DEFNS DLL_EXPORT closecmd();

    E_ERROR_DEFNS DLL_EXPORT execcmd(IN BRCM_CAPI_UINT uiCommandID, IN BRCM_CAPI_UINT uiTimeout,
            OUT BRCM_CAPI_PINT32 piStatus, OUT P_ST_4G_API_PKT *pstAPIRespRcvd,IN BRCM_CAPI_UINT uiNumOfInputTLVs, ...);

	E_ERROR_DEFNS DLL_EXPORT  execcmd_pkt(IN BRCM_CAPI_UINT uiCommandID,IN BRCM_CAPI_UINT uiTimeout, IN  P_ST_4G_API_PKT pstAPISendPayload,OUT BRCM_CAPI_PINT32 piStatus,OUT P_ST_4G_API_PKT *pstAPIRespRcvd);
    
    E_ERROR_DEFNS DLL_EXPORT postcmd(IN BRCM_CAPI_UINT uiCommandID, IN BRCM_CAPI_UINT uiNumOfInputTLVs, ...);

    E_ERROR_DEFNS DLL_EXPORT postcmd_pkt(IN BRCM_CAPI_UINT uiCommandID,IN  P_ST_4G_API_PKT pstAPISendPayload);
  
    BRCM_CAPI_VOID DLL_EXPORT free_rcv_pkt(IN P_ST_4G_API_PKT pPktPtr);

#ifdef __cplusplus
}
#endif
#endif
