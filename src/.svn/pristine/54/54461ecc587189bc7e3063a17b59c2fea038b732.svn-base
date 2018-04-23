/*****************************************************************************
**
** uconn_bcm_emulator.h
** Implements Dummy/Test APIs for Beceem's LTE Device
**
** Author: Sriram V Iyer
** Date  : 08-JAN-2011
** (c) United Mobile Apps Pvt Ltd (2010 - 2015)

    VER		  DATE		AUTHOR		DESCRIPTION
	0.1		08/01/11     SVI		Creation
****************************************************************************/

#ifndef UCONN_BCM_EMULATOR_H_
#define UCONN_BCM_EMULATOR_H_

// Currently we need to implement the calls that are currently used in the 
// project from becm_4g_capi_export.h

#include "4G_CAPI_brcm.h"
#include "becm_4g_capi_export.h"
#include "brcm_4g_capi_defines.h"

#include <uconn_bcm_DAL.h>

#ifdef EXEC_CMD
typedef E_ERROR_DEFNS (*EMULATOR_CMD_HANDLER)(UINT uiTimeout, 
						  P_ST_4G_API_PKT pstAPISendPayload,
						  PINT32 piStatus,
						  P_ST_4G_API_PKT *pstAPIRespRcvd);
#else
typedef E_ERROR_DEFNS (*EMULATOR_CMD_HANDLER)(
						P_ST_4G_API_PKT pstAPISendPayload);
#endif

struct stEmulatorCommandHandler {
	UINT					eCmdId;
	EMULATOR_CMD_HANDLER	pfnHandler;
};

typedef struct _stDeviceParams {
	P_FN_CALLBACK	pCallback;
} stDeviceParams;

ST_TLV CreateTLV_emulation( eTLVType eTType, 
						   E_TYPE_DEFNS eDType, 
						   int iNumElements = 1);

#ifdef EXEC_CMD
E_ERROR_DEFNS default_handler (	UINT uiTimeout, 
								P_ST_4G_API_PKT pstAPISendPayload,
								PINT32 piStatus,
								P_ST_4G_API_PKT *pstAPIRespRcvd);


E_ERROR_DEFNS netattach_handler (	UINT uiTimeout, 
									P_ST_4G_API_PKT pstAPISendPayload,
									PINT32 piStatus,
									P_ST_4G_API_PKT *pstAPIRespRcvd);

E_ERROR_DEFNS netdetach_handler (	UINT uiTimeout, 
									P_ST_4G_API_PKT pstAPISendPayload,
									PINT32 piStatus,
									P_ST_4G_API_PKT *pstAPIRespRcvd);

E_ERROR_DEFNS netsearch_handler (	UINT uiTimeout, 
									P_ST_4G_API_PKT pstAPISendPayload,
									PINT32 piStatus,
									P_ST_4G_API_PKT *pstAPIRespRcvd);

E_ERROR_DEFNS ps_ctx_cfg_set_handler (	UINT uiTimeout, 
									P_ST_4G_API_PKT pstAPISendPayload,
									PINT32 piStatus,
									P_ST_4G_API_PKT *pstAPIRespRcvd);

E_ERROR_DEFNS ps_ctx_cfg_get_handler (	UINT uiTimeout, 
									P_ST_4G_API_PKT pstAPISendPayload,
									PINT32 piStatus,
									P_ST_4G_API_PKT *pstAPIRespRcvd);

E_ERROR_DEFNS ps_ctx_define_primary_handler (	UINT uiTimeout, 
									P_ST_4G_API_PKT pstAPISendPayload,
									PINT32 piStatus,
									P_ST_4G_API_PKT *pstAPIRespRcvd);

E_ERROR_DEFNS ps_ctx_define_secondary_handler (	UINT uiTimeout, 
									P_ST_4G_API_PKT pstAPISendPayload,
									PINT32 piStatus,
									P_ST_4G_API_PKT *pstAPIRespRcvd);

E_ERROR_DEFNS ps_ctx_undefine_primary_handler (	UINT uiTimeout, 
									P_ST_4G_API_PKT pstAPISendPayload,
									PINT32 piStatus,
									P_ST_4G_API_PKT *pstAPIRespRcvd);

E_ERROR_DEFNS ps_ctx_undefine_secondary_handler (	UINT uiTimeout, 
									P_ST_4G_API_PKT pstAPISendPayload,
									PINT32 piStatus,
									P_ST_4G_API_PKT *pstAPIRespRcvd);

E_ERROR_DEFNS net_cfg_set_handler (	UINT uiTimeout, 
									P_ST_4G_API_PKT pstAPISendPayload,
									PINT32 piStatus,
									P_ST_4G_API_PKT *pstAPIRespRcvd);

E_ERROR_DEFNS net_cfg_get_handler (	UINT uiTimeout, 
									P_ST_4G_API_PKT pstAPISendPayload,
									PINT32 piStatus,
									P_ST_4G_API_PKT *pstAPIRespRcvd);

E_ERROR_DEFNS prop_APN_table_set_handler (	UINT uiTimeout, 
									P_ST_4G_API_PKT pstAPISendPayload,
									PINT32 piStatus,
									P_ST_4G_API_PKT *pstAPIRespRcvd);

E_ERROR_DEFNS prop_APN_table_get_handler (	UINT uiTimeout, 
									P_ST_4G_API_PKT pstAPISendPayload,
									PINT32 piStatus,
									P_ST_4G_API_PKT *pstAPIRespRcvd);

E_ERROR_DEFNS prop_IP_IF_Status_handler (	UINT uiTimeout, 
									P_ST_4G_API_PKT pstAPISendPayload,
									PINT32 piStatus,
									P_ST_4G_API_PKT *pstAPIRespRcvd);

#else
E_ERROR_DEFNS default_handler	(P_ST_4G_API_PKT pstAPISendPayload);

E_ERROR_DEFNS netattach_handler (P_ST_4G_API_PKT pstAPISendPayload);

E_ERROR_DEFNS netdetach_handler (P_ST_4G_API_PKT pstAPISendPayload);

E_ERROR_DEFNS netsearch_handler (P_ST_4G_API_PKT pstAPISendPayload);

E_ERROR_DEFNS netsearchstatus_handler(P_ST_4G_API_PKT pRecvPkt);

E_ERROR_DEFNS ps_ctx_cfg_set_handler (P_ST_4G_API_PKT pstAPISendPayload);

E_ERROR_DEFNS ps_ctx_cfg_get_handler (P_ST_4G_API_PKT pstAPISendPayload);

E_ERROR_DEFNS ps_ctx_define_primary_handler (P_ST_4G_API_PKT pstAPISendPayload);

E_ERROR_DEFNS ps_ctx_define_secondary_handler (P_ST_4G_API_PKT pstAPISendPayload);

E_ERROR_DEFNS ps_ctx_undefine_primary_handler (P_ST_4G_API_PKT pstAPISendPayload);

E_ERROR_DEFNS ps_ctx_undefine_secondary_handler (P_ST_4G_API_PKT pstAPISendPayload);

E_ERROR_DEFNS net_cfg_set_handler (P_ST_4G_API_PKT pstAPISendPayload);

E_ERROR_DEFNS net_cfg_get_handler (P_ST_4G_API_PKT pstAPISendPayload);

E_ERROR_DEFNS prop_APN_table_set_handler (P_ST_4G_API_PKT pstAPISendPayload);

E_ERROR_DEFNS prop_APN_table_get_handler (P_ST_4G_API_PKT pstAPISendPayload);

E_ERROR_DEFNS prop_IP_IF_Status_handler (P_ST_4G_API_PKT pstAPISendPayload);

E_ERROR_DEFNS net_sig_qual_get_handler (P_ST_4G_API_PKT pstAPISendPayload);

E_ERROR_DEFNS encapsulated_at_cmd_handler (P_ST_4G_API_PKT pstAPISendPayload);

E_ERROR_DEFNS netstatusget_handler (P_ST_4G_API_PKT pstAPISendPayload);

E_ERROR_DEFNS netgetinfo_handler (P_ST_4G_API_PKT pstAPISendPayload);

E_ERROR_DEFNS statisticinfo_handler (P_ST_4G_API_PKT pstAPISendPayload);

#endif

#endif
