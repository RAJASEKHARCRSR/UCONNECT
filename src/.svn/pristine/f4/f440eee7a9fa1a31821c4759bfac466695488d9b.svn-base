#ifndef _BECM_4G_CAPI_EXPORTS_H
#define _BECM_4G_CAPI_EXPORTS_H
#ifdef WIN32
#include <windows.h>
#include <stdio.h>
#endif

#include "4G_CAPI_brcm.h"

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

    E_ERROR_DEFNS DLL_EXPORT execcmd(IN UINT uiCommandID, IN UINT uiTimeout,
            OUT PINT32 piStatus, OUT P_ST_4G_API_PKT *pstAPIRespRcvd,IN UINT uiNumOfInputTLVs, ...);

	E_ERROR_DEFNS DLL_EXPORT  execcmd_pkt(IN UINT uiCommandID,IN UINT uiTimeout, IN  P_ST_4G_API_PKT pstAPISendPayload,OUT PINT32 piStatus,OUT P_ST_4G_API_PKT *pstAPIRespRcvd);
    
    E_ERROR_DEFNS DLL_EXPORT postcmd(IN UINT uiCommandID, IN UINT uiNumOfInputTLVs, ...);

    E_ERROR_DEFNS DLL_EXPORT postcmd_pkt(IN UINT uiCommandID,IN  P_ST_4G_API_PKT pstAPISendPayload);
  
    VOID DLL_EXPORT free_rcv_pkt(IN P_ST_4G_API_PKT pPktPtr);

#ifdef __cplusplus
}
#endif
#endif
