/*****************************************************************************
**
** uconn_bcm_emulator.cpp
** Implements Dummy/Test APIs for Beceem's LTE Device
**
** Author: Sriram V Iyer
** Date  : 08-JAN-2011
** (c) United Mobile Apps Pvt Ltd (2010 - 2015)

    VER		  DATE		AUTHOR		DESCRIPTION
	0.1		08/01/11     SVI		Creation
/****************************************************************************/

#include "4G_CAPI_brcm.h"
#include "brcm_4g_capi_defines.h"
#include "brcm_4g_capi_export.h"
#include "uconn_bcm_emulator.h"
#include <assert.h>
#include <iostream>

#include "uconn_bcm_DAL.h"

#define MAX_MEM_SIZE 40

using namespace std;
U_UINT32 stateInfo;
stDeviceParams		g_tDeviceParams;

stEmulatorCommandHandler g_tEmulatorCommandHandlers[] = {
	{ CAPI_CMD_LTE_EQUIP_CONFIG_GET, default_handler },
	{ CAPI_CMD_LTE_EQUIP_CONFIG_GET, default_handler },
	{ CAPI_CMD_LTE_EQUIP_CONFIG_SET, default_handler },
    { CAPI_CMD_LTE_EQUIP_INFO_GET, default_handler },
	{ CAPI_CMD_LTE_NET_SEARCH, netsearch_handler },
	{ CAPI_CMD_LTE_NET_ATTACH, netattach_handler },
	{ CAPI_CMD_LTE_NET_CFG_GET, net_cfg_get_handler },
	{ CAPI_CMD_LTE_NET_CFG_SET, net_cfg_set_handler },
	{ CAPI_CMD_LTE_NET_DETACH, netdetach_handler },
	{ CAPI_CMD_LTE_NET_STATUS_GET, netstatusget_handler },
	{ CAPI_CMD_LTE_NET_SIG_QUAL_GET, net_sig_qual_get_handler },
	{ CAPI_CMD_LTE_PS_DFN_INFO_GET, default_handler },
	{ CAPI_CMD_LTE_PS_CTX_DEFINE_PRIMARY, ps_ctx_define_primary_handler },
	{ CAPI_CMD_LTE_PS_CTX_DEFINE_SECONDARY, ps_ctx_define_secondary_handler },
	{ CAPI_CMD_LTE_PS_CTX_UNDEFINE_PRIMARY, ps_ctx_undefine_primary_handler },
	{ CAPI_CMD_LTE_PS_CTX_UNDEFINE_SECONDARY, ps_ctx_undefine_secondary_handler },
	{ CAPI_CMD_LTE_PS_CTX_CFG_GET, ps_ctx_cfg_get_handler },
	{ CAPI_CMD_LTE_PS_CTX_CFG_SET, ps_ctx_cfg_set_handler },
	{ CAPI_CMD_LTE_PS_CTX_INFO_GET, netgetinfo_handler },
	{ CAPI_CMD_LTE_PS_CTX_ACTIVATE, default_handler },
	{ CAPI_CMD_LTE_PS_CTX_DEACTIVATE, default_handler },
	{ CAPI_CMD_LTE_PS_CTX_MODIFY, default_handler },
	{ CAPI_CMD_ENCAPSULATED_LTE_AT, encapsulated_at_cmd_handler },
	{ CAPI_CMD_LTE_LOG, default_handler },
	{ CAPI_CMD_LTE_APN_TABLE_GET, prop_APN_table_get_handler },
	{ CAPI_CMD_LTE_APN_TABLE_SET, prop_APN_table_set_handler },
	{ e_API_prop_IP_IF_Status, prop_IP_IF_Status_handler },
	{ e_API_net_search_status, netsearchstatus_handler },

};

/***********************************************************************
*	opencmd -
*	Sets the Callback function for the emulation
*
*	Parameters
*	pstOpenParams				- Parameters to setup up the Emulator
*   p_fn_callback_function_addr	- Callback function pointer
***********************************************************************/

E_ERROR_DEFNS opencmd(	ST_OPEN_PARAM*	pstOpenParams,
						P_FN_CALLBACK	p_fn_callback_function_addr )
{
	// Store the callback - We can call it later from the pseudo device
	g_tDeviceParams.pCallback = p_fn_callback_function_addr;

	return ERROR_CODE_SUCCESS;
}

/***********************************************************************
*	execcmd_pkt -
*	Emulation for the Blocking call of the modem
*
*	Parameters
*	eCommandID			- Command ID
*   uiTimeout			- Time out value
*	pstAPISendPayload	- TLV of the Parameters to be sent
*	piStatus			- Status of the operation
*	pstAPIResp			- TLV of the response to the command
***********************************************************************/
#ifdef EXEC_CMD
E_ERROR_DEFNS execcmd_pkt(E_COMMAND_ID eCommandID,
						  UINT uiTimeout, 
						  P_ST_4G_API_PKT pstAPISendPayload,
						  PINT32 piStatus,
						  P_ST_4G_API_PKT *pstAPIResp)
{
	g_tEmulatorCommandHandlers[eCommandID].pfnHandler(uiTimeout, pstAPISendPayload, piStatus, pstAPIResp);


	return ERROR_CODE_SUCCESS;
}
#else
/***********************************************************************
*	postcmd_pkt -
*	Emulation for the Asynchronous call of the modem
*
*	Parameters
*	eCommandID			- Command ID
*	pstAPISendPayload	- TLV of the Parameters to be sent
***********************************************************************/

E_ERROR_DEFNS postcmd_pkt(UINT eCommandID,
                      P_ST_4G_API_PKT pstAPISendPayload)
{
    if(eCommandID == CAPI_CMD_TM_STAT_BASE){
        statisticinfo_handler(pstAPISendPayload);
    }else
    g_tEmulatorCommandHandlers[eCommandID & 0x000000ff].pfnHandler(pstAPISendPayload);

    return ERROR_CODE_SUCCESS;
}
#endif

UINT32 emuRegInfo;
/***********************************************************************
*	netattach_handler -
*	Emulation logic for the netattach command
*
*	Parameters
*   uiTimeout			- Time out value
*	pstAPISendPayload	- TLV of the Parameters to be sent
*	piStatus			- Status of the operation
*	pstAPIResp			- TLV of the response to the command
***********************************************************************/
#ifdef EXEC_CMD
E_ERROR_DEFNS netattach_handler (	UINT uiTimeout, 
									P_ST_4G_API_PKT pstAPISendPayload,
									PINT32 piStatus,
									P_ST_4G_API_PKT *pstAPIRespRcvd)
{
	cout << "NetAttach Handler Called!" << endl;

	*piStatus = 0; 

	return ERROR_CODE_SUCCESS;
}
#else
E_ERROR_DEFNS netattach_handler (P_ST_4G_API_PKT pstAPISendPayload)
{
    emuRegInfo = 1;
	cout << "NetAttach Handler Called!" << endl;



    //P_ST_CALLBACK_INFO callbackinfo = new ST_CALLBACK_INFO();
    P_ST_CALLBACK_INFO callbackinfo;
    callbackinfo = (P_ST_CALLBACK_INFO)malloc(sizeof(ST_CALLBACK_INFO) + sizeof(UINT));

	/* First call the LTE_REG_STATUS Event */
	callbackinfo->uicmd_id = CAPI_CMD_LTE_EVT_LTE_REG_STATUS;	//UCONN_EVT_PRIMARY_ACT
	callbackinfo->uinum_tlvs = 1;	
    callbackinfo->pTLVBuf[0].uiType = eTYPE_UINT;
    callbackinfo->pTLVBuf[0].uiLength = sizeof(UINT);
    //callbackinfo->pTLVBuf[0].uchaValue = (PUCHAR)&emuRegInfo;
    PUINT32 puchbuff = (PUINT32)callbackinfo->pTLVBuf[0].uchaValue;
    *puchbuff = emuRegInfo;
    g_tDeviceParams.pCallback(0, callbackinfo);

	callbackinfo->uicmd_id = CAPI_CMD_LTE_EVT_PRIMARY_ACT;	//UCONN_EVT_PRIMARY_ACT
	callbackinfo->uinum_tlvs = 9;
	stateInfo = 1;
	g_tDeviceParams.pCallback(0, callbackinfo);

	//callbackinfo->uicmd_id = SYSCTL_NOTI_DISCONNECTED;
 //   callbackinfo->uinum_tlvs = 0;
	//g_tDeviceParams.pCallback(0, callbackinfo);

	//callbackinfo->uicmd_id = SYSCTL_NOTI_DEVICE_OPEN;
 //   callbackinfo->uinum_tlvs = 0;
	//g_tDeviceParams.pCallback(0, callbackinfo);

	//delete(callbackinfo);
    free(callbackinfo);

	return ERROR_CODE_SUCCESS;
}
#endif


/***********************************************************************
*	netdetach_handler -
*	Emulation logic for the netdetach command
*
*	Parameters
*   uiTimeout			- Time out value
*	pstAPISendPayload	- TLV of the Parameters to be sent
*	piStatus			- Status of the operation
*	pstAPIResp			- TLV of the response to the command
***********************************************************************/

#ifdef EXEC_CMD
E_ERROR_DEFNS netdetach_handler (	UINT uiTimeout, 
									P_ST_4G_API_PKT pstAPISendPayload,
									PINT32 piStatus,
									P_ST_4G_API_PKT *pstAPIRespRcvd)
{
	cout<<"NetDetach Handler Called!" << endl;
	*piStatus = 0;

	return ERROR_CODE_SUCCESS;
}
#else
E_ERROR_DEFNS netdetach_handler (P_ST_4G_API_PKT pstAPISendPayload)
{
	cout<<"NetDetach Handler Called!" << endl;
	P_ST_CALLBACK_INFO callbackinfo = (P_ST_CALLBACK_INFO)malloc(sizeof(ST_CALLBACK_INFO) +
										sizeof(UINT));
	callbackinfo->uinum_tlvs = 1;
	callbackinfo->uicmd_id = CAPI_CMD_LTE_EVT_PRIMARY_DEACT;

	//TLV #1
	UINT ctx_id = 1;

	P_ST_TLV pCtxID = callbackinfo->pTLVBuf;

	pCtxID->uiType = eTYPE_UINT;
	pCtxID->uiLength = sizeof(UINT);
	FillTLVData(pCtxID, &ctx_id, sizeof(UINT));

	g_tDeviceParams.pCallback(0, callbackinfo);

	//LTE_REG_STATUS event to be sent 
	callbackinfo->uinum_tlvs = 1;
	callbackinfo->uicmd_id = CAPI_CMD_LTE_EVT_LTE_REG_STATUS;

	//TLV #2
	UINT reg_info = 0;

	P_ST_TLV pRegInfo = callbackinfo->pTLVBuf;

	pRegInfo->uiType = eTYPE_UINT;
	pRegInfo->uiLength = sizeof(UINT);
	FillTLVData(pRegInfo, &reg_info, sizeof(UINT));
    stateInfo = 0 ;
	g_tDeviceParams.pCallback(0, callbackinfo);

	free(callbackinfo);

	return ERROR_CODE_SUCCESS;
}
#endif
/***********************************************************************
*	netsearch_handler -
*	Emulation logic for the netsearch command
*
*	Parameters
*   uiTimeout			- Time out value
*	pstAPISendPayload	- TLV of the Parameters to be sent
*	piStatus			- Status of the operation
*	pstAPIResp			- TLV of the response to the command
***********************************************************************/

#ifdef EXEC_CMD
E_ERROR_DEFNS netsearch_handler (	UINT uiTimeout, 
									P_ST_4G_API_PKT pstAPISendPayload,
									PINT32 piStatus,
									P_ST_4G_API_PKT *pstAPIRespRcvd)
{

	/*--------------PLMN ID Data to be returned------------------*/
	UINT plmn_id[3] = {100, 101, 102};
	UINT rats[3] = {8, 8, 8};
	UINT membership[3] = {0, 1, 2};
	UINT num_plmn = 3;

	/*-----------------------------------------------------------*/

	cout<<"NetSearch Handler Called!" << endl;
	*piStatus = 0;
	PUCHAR puchBufferPtr = NULL;
	UINT uiLength, uiPrevLength;
	UINT	u32TLVBytesSoFar = 0;
	/* Initialize the API packet to be sent back */
	uiLength = InitializeAPIPkt(*pstAPIRespRcvd);

	/* Fill the pstAPIRespRcvd packet with the PLMN list data */
	/* Assume an array of 2 PLMN ids */
	(*pstAPIRespRcvd)->uiNumTLVs = 1 + num_plmn*3;	/* 1 PLMN count + Search results */

	/*---------PLMN Count---------*/
	P_ST_TLV pPLMNCount = CreateTLV(eValue, eTYPE_UINT);
	FillTLVData(pPLMNCount, &num_plmn, sizeof(UINT));
	uiPrevLength = uiLength;
	uiLength += pPLMNCount->uiLength;
	ALLOCATE_MEMORY((*pstAPIRespRcvd), ST_4G_API_PKT, uiLength, uiPrevLength);
	puchBufferPtr = reinterpret_cast<PUCHAR> ((*pstAPIRespRcvd)->astTLVBuffer)
					+ u32TLVBytesSoFar;
	// Now copy the contents of the TLV buffer into 4G API Packet
	COPY_MEMORY(puchBufferPtr, pPLMNCount, pPLMNCount->uiLength);
	u32TLVBytesSoFar += pPLMNCount->uiLength;
	FREE_MEMORY(pPLMNCount);

	/*---Structure TLVs for PLMN ID, RAT, Membership, free text---*/
	for(UINT i = 0; i < num_plmn; i++)
	{
		P_ST_TLV pPLMNid = CreateTLV(eValue, eTYPE_UINT);
		FillTLVData(pPLMNid, &plmn_id[i], sizeof(UINT));
		uiPrevLength = uiLength;
		uiLength += pPLMNid->uiLength;
		ALLOCATE_MEMORY((*pstAPIRespRcvd), ST_4G_API_PKT, uiLength, uiPrevLength);
		puchBufferPtr = reinterpret_cast<PUCHAR> ((*pstAPIRespRcvd)->astTLVBuffer)
						+ u32TLVBytesSoFar;
		// Now copy the contents of the TLV buffer into 4G API Packet
		COPY_MEMORY(puchBufferPtr, pPLMNid, pPLMNid->uiLength);
		u32TLVBytesSoFar += pPLMNid->uiLength;
		FREE_MEMORY(pPLMNid);

		/* Fill the TLV array for the PLMN RATs */
		P_ST_TLV pRATs = CreateTLV(eValue, eTYPE_UINT);
		FillTLVData(pRATs, &rats[i], sizeof(UINT));
		uiPrevLength = uiLength;
		uiLength += pRATs->uiLength;
		ALLOCATE_MEMORY((*pstAPIRespRcvd), ST_4G_API_PKT, uiLength, uiPrevLength);
		puchBufferPtr = reinterpret_cast<PUCHAR> ((*pstAPIRespRcvd)->astTLVBuffer)
						+ u32TLVBytesSoFar;
		// Now copy the contents of the TLV buffer into 4G API Packet
		COPY_MEMORY(puchBufferPtr, pRATs, pRATs->uiLength);
		u32TLVBytesSoFar += pRATs->uiLength;
		FREE_MEMORY(pRATs);

		/* Fill the TLV array for the PLMN Membership */
		P_ST_TLV pMembership = CreateTLV(eValue, eTYPE_UINT);
		FillTLVData(pMembership, &membership[i], sizeof(UINT));
		uiPrevLength = uiLength;
		uiLength += pMembership->uiLength;
		ALLOCATE_MEMORY((*pstAPIRespRcvd), ST_4G_API_PKT, uiLength, uiPrevLength);
		puchBufferPtr = reinterpret_cast<PUCHAR> ((*pstAPIRespRcvd)->astTLVBuffer)
						+ u32TLVBytesSoFar;
		// Now copy the contents of the TLV buffer into 4G API Packet
		COPY_MEMORY(puchBufferPtr, pMembership, pMembership->uiLength);
		u32TLVBytesSoFar += pMembership->uiLength;
		FREE_MEMORY(pMembership);
	}

	puchBufferPtr = reinterpret_cast<PUCHAR> ((*pstAPIRespRcvd)->astTLVBuffer)
					+ u32TLVBytesSoFar;
	// Update the Total Length of the 4G API Packet
	(*pstAPIRespRcvd)->uiTLVTotalLength = (puchBufferPtr - 
		reinterpret_cast<PUCHAR>(*pstAPIRespRcvd));

	return ERROR_CODE_SUCCESS;
}
#else
E_ERROR_DEFNS netsearch_handler (P_ST_4G_API_PKT pstAPISendPayload)
{
	cout<<"NetSearch Handler Called!" << endl;
	return ERROR_CODE_SUCCESS;
}

#endif
/***********************************************************************
*	default_handler -
*	Emulation logic for the default handler
*
*	Parameters
*   uiTimeout			- Time out value
*	pstAPISendPayload	- TLV of the Parameters to be sent
*	piStatus			- Status of the operation
*	pstAPIResp			- TLV of the response to the command
***********************************************************************/

#ifdef EXEC_CMD
E_ERROR_DEFNS default_handler (	UINT uiTimeout, 
								P_ST_4G_API_PKT pstAPISendPayload,
								PINT32 piStatus,
								P_ST_4G_API_PKT *pstAPIRespRcvd)
{
	cout << "Dummy Handler Called!" << endl;
	assert(false);

	return ERROR_CODE_SUCCESS;
}
#else
E_ERROR_DEFNS default_handler (P_ST_4G_API_PKT pstAPISendPayload)
{
	cout << "Dummy Handler Called!" << endl;
	//assert(false);

	return ERROR_CODE_SUCCESS;
}

#endif

/***********************************************************************
*	netsearchstatus_handler -
*	Emulation logic for the netsearch status command
*
*	Parameters
*	pRecvPkt - TLV parameters to be filled 
***********************************************************************/
E_ERROR_DEFNS netsearchstatus_handler(P_ST_4G_API_PKT pRecvPkt)
{
	/* Fill in the pRecvPkt with the netsearch status */
	
	/*--------------PLMN ID Data to be returned------------------*/
	T_PLMN plmns[4] = { {E_PLMN_ID_NUMERIC, "100"},
						{E_PLMN_ID_NUMERIC, "101"},
						{E_PLMN_ID_NUMERIC, "102"},
						{E_PLMN_ID_NUMERIC, "103"}};
	//UINT plmn_id[4] = {100, 101, 102, 103};
	UINT rats[4] = {8, 8, 8, 8};
	UINT membership[4] = {0, 1, 2, 3};
	UCHAR free_text[4][100] = {"Airtel", "Vodafone", "BSNL", "Uninor"};
	UINT num_plmn = 4;

	/*-----------------------------------------------------------*/
	cout << "Net Search Status Handler Called!" << endl;

	PUCHAR puchBufferPtr = NULL;
	UINT uiLength, uiPrevLength;
	UINT	u32TLVBytesSoFar = 0;

	/* Initialize the API packet to be sent back */
	//uiLength = InitializeAPIPkt(pRecvPkt);
	uiLength = sizeof(ST_4G_API_PKT) - sizeof(ST_TLV);

	/* Fill the pRecvPkt packet with the PLMN list data */
	/* Assume an array of 3 PLMN ids */
	pRecvPkt->uiNumTLVs = 1 + num_plmn*5;	
	/* 1 PLMN count + Search results */

	/*---------PLMN Count---------*/
	P_ST_TLV pPLMNCount = CreateTLV(&pRecvPkt, uiLength, eValue, eTYPE_UINT);
	FillTLVData(pPLMNCount, &num_plmn, sizeof(UINT));
	uiPrevLength = uiLength;
	uiLength += pPLMNCount->uiLength + 8;
	//ALLOCATE_MEMORY(pRecvPkt, ST_4G_API_PKT, uiLength, uiPrevLength);
	puchBufferPtr = reinterpret_cast<PUCHAR> (pRecvPkt->astTLVBuffer)
					+ u32TLVBytesSoFar;
	// Now copy the contents of the TLV buffer into 4G API Packet
	COPY_MEMORY(puchBufferPtr, pPLMNCount, (pPLMNCount->uiLength + 8));
	u32TLVBytesSoFar += (pPLMNCount->uiLength + 8);
	FREE_MEMORY(pPLMNCount);

	/*---Structure TLVs for PLMN ID, RAT, Membership, free text---*/
	for(UINT i = 0; i < num_plmn; i++)
	{
#if 0
		P_ST_TLV pPLMNid = CreateTLV(eValue, eTYPE_UINT);
		FillTLVData(pPLMNid, &plmn_id[i], sizeof(UINT));
		uiPrevLength = uiLength;
		uiLength += pPLMNid->uiLength;
		//ALLOCATE_MEMORY(pRecvPkt, ST_4G_API_PKT, uiLength, uiPrevLength);
		puchBufferPtr = reinterpret_cast<PUCHAR> (pRecvPkt->astTLVBuffer)
						+ u32TLVBytesSoFar;
		// Now copy the contents of the TLV buffer into 4G API Packet
		COPY_MEMORY(puchBufferPtr, pPLMNid, pPLMNid->uiLength);
		u32TLVBytesSoFar += pPLMNid->uiLength;
		FREE_MEMORY(pPLMNid);
#endif
		/* PLMN type */
		P_ST_TLV pPLMNtype = CreateTLV(&pRecvPkt, uiLength, eValue, eTYPE_UINT);
		FillTLVData(pPLMNtype, &plmns[i].plmnFormat, sizeof(UINT));
		uiPrevLength = uiLength;
		uiLength += pPLMNtype->uiLength + 8;
		//ALLOCATE_MEMORY(pRecvPkt, ST_4G_API_PKT, uiLength, uiPrevLength);
		puchBufferPtr = reinterpret_cast<PUCHAR> (pRecvPkt->astTLVBuffer)
						+ u32TLVBytesSoFar;
		// Now copy the contents of the TLV buffer into 4G API Packet
		COPY_MEMORY(puchBufferPtr, pPLMNtype, (pPLMNtype->uiLength + 8));
		u32TLVBytesSoFar += (pPLMNtype->uiLength + 8);
		FREE_MEMORY(pPLMNtype);

		/* Fill the TLV array for the PLMN ID */
		P_ST_TLV pPLMN_id = CreateTLV(&pRecvPkt, uiLength, eArray, eTYPE_CHAR, sizeof(plmns[i].plmn_id));
		FillTLVData(pPLMN_id, free_text[i], sizeof(plmns[i].plmn_id));
		uiPrevLength = uiLength;
		uiLength += pPLMN_id->uiLength + 8;
		//ALLOCATE_MEMORY(pRecvPkt, ST_4G_API_PKT, uiLength, uiPrevLength);
		puchBufferPtr = reinterpret_cast<PUCHAR> (pRecvPkt->astTLVBuffer)
						+ u32TLVBytesSoFar;
		// Now copy the contents of the TLV buffer into 4G API Packet
		COPY_MEMORY(puchBufferPtr, pPLMN_id, (pPLMN_id->uiLength + 8));
		u32TLVBytesSoFar += (pPLMN_id->uiLength + 8);
		FREE_MEMORY(pPLMN_id);

		/* Fill the TLV array for the PLMN RATs */
		P_ST_TLV pRATs = CreateTLV(&pRecvPkt, uiLength, eValue, eTYPE_UINT);
		FillTLVData(pRATs, &rats[i], sizeof(UINT));
		uiPrevLength = uiLength;
		uiLength += pRATs->uiLength + 8;
		//ALLOCATE_MEMORY(pRecvPkt, ST_4G_API_PKT, uiLength, uiPrevLength);
		puchBufferPtr = reinterpret_cast<PUCHAR> (pRecvPkt->astTLVBuffer)
						+ u32TLVBytesSoFar;
		// Now copy the contents of the TLV buffer into 4G API Packet
		COPY_MEMORY(puchBufferPtr, pRATs, (pRATs->uiLength + 8));
		u32TLVBytesSoFar += (pRATs->uiLength + 8);
		FREE_MEMORY(pRATs);

		/* Fill the TLV array for the PLMN Membership */
		P_ST_TLV pMembership = CreateTLV(&pRecvPkt, uiLength, eValue, eTYPE_UINT);
		FillTLVData(pMembership, &membership[i], sizeof(UINT));
		uiPrevLength = uiLength;
		uiLength += pMembership->uiLength + 8;
		//ALLOCATE_MEMORY(pRecvPkt, ST_4G_API_PKT, uiLength, uiPrevLength);
		puchBufferPtr = reinterpret_cast<PUCHAR> (pRecvPkt->astTLVBuffer)
						+ u32TLVBytesSoFar;
		// Now copy the contents of the TLV buffer into 4G API Packet
		COPY_MEMORY(puchBufferPtr, pMembership, (pMembership->uiLength + 8));
		u32TLVBytesSoFar += (pMembership->uiLength + 8);
		FREE_MEMORY(pMembership);

		/* Fill the TLV array for the free_text */
		P_ST_TLV pFreetext = CreateTLV(&pRecvPkt, uiLength, eArray, eTYPE_CHAR, sizeof(free_text[i]));
		FillTLVData(pFreetext, free_text[i], sizeof(free_text[i]));
		uiPrevLength = uiLength;
		uiLength += pFreetext->uiLength + 8;
		//ALLOCATE_MEMORY(pRecvPkt, ST_4G_API_PKT, uiLength, uiPrevLength);
		puchBufferPtr = reinterpret_cast<PUCHAR> (pRecvPkt->astTLVBuffer)
						+ u32TLVBytesSoFar;
		// Now copy the contents of the TLV buffer into 4G API Packet
		COPY_MEMORY(puchBufferPtr, pFreetext, (pFreetext->uiLength + 8));
		u32TLVBytesSoFar += (pFreetext->uiLength + 8);
		FREE_MEMORY(pFreetext);
	}

	puchBufferPtr = reinterpret_cast<PUCHAR> (pRecvPkt->astTLVBuffer)
					+ u32TLVBytesSoFar;
	// Update the Total Length of the 4G API Packet
	pRecvPkt->uiTLVTotalLength = (puchBufferPtr - 
		reinterpret_cast<PUCHAR>(pRecvPkt));

	return ERROR_CODE_SUCCESS;


}

/***********************************************************************
*	ps_ctx_cfg_set_handler -
*	Emulation logic for the ps_ctx_cfg_set command
*
*	Parameters
*   uiTimeout			- Time out value
*	pstAPISendPayload	- TLV of the Parameters to be sent
*	piStatus			- Status of the operation
*	pstAPIResp			- TLV of the response to the command
***********************************************************************/

#ifdef EXEC_CMD
E_ERROR_DEFNS ps_ctx_cfg_set_handler (	UINT uiTimeout, 
									P_ST_4G_API_PKT pstAPISendPayload,
									PINT32 piStatus,
									P_ST_4G_API_PKT *pstAPIRespRcvd)
{
	cout<<"PS_CTX_CFG_SET Handler Called!" << endl;
	*piStatus = 0;

	return ERROR_CODE_SUCCESS;
}
#else
E_ERROR_DEFNS ps_ctx_cfg_set_handler (P_ST_4G_API_PKT pstAPISendPayload)
{
	cout<<"PS_CTX_CFG_SET Handler Called!" << endl;

	return ERROR_CODE_SUCCESS;
}
#endif

/***********************************************************************
*	ps_ctx_cfg_get_handler -
*	Emulation logic for the ps_ctx_cfg_get command
*
*	Parameters
*   uiTimeout			- Time out value
*	pstAPISendPayload	- TLV of the Parameters to be sent
*	piStatus			- Status of the operation
*	pstAPIResp			- TLV of the response to the command
***********************************************************************/

#ifdef EXEC_CMD
E_ERROR_DEFNS ps_ctx_cfg_get_handler (	UINT uiTimeout, 
									P_ST_4G_API_PKT pstAPISendPayload,
									PINT32 piStatus,
									P_ST_4G_API_PKT *pstAPIRespRcvd)
{
	cout<<"PS_CTX_CFG_GET Handler Called!" << endl;
	*piStatus = 0;

	return ERROR_CODE_SUCCESS;
}
#else
E_ERROR_DEFNS ps_ctx_cfg_get_handler (P_ST_4G_API_PKT pstAPISendPayload)
{
	cout<<"PS_CTX_CFG_GET Handler Called!" << endl;

	return ERROR_CODE_SUCCESS;
}
#endif

/***********************************************************************
*	ps_ctx_define_primary_handler -
*	Emulation logic for the ps_ctx_define_primary command
*
*	Parameters
*   uiTimeout			- Time out value
*	pstAPISendPayload	- TLV of the Parameters to be sent
*	piStatus			- Status of the operation
*	pstAPIResp			- TLV of the response to the command
***********************************************************************/

#ifdef EXEC_CMD
E_ERROR_DEFNS ps_ctx_define_primary_handler (	UINT uiTimeout, 
									P_ST_4G_API_PKT pstAPISendPayload,
									PINT32 piStatus,
									P_ST_4G_API_PKT *pstAPIRespRcvd)
{
	cout<<"PS_CTX_DEFINE_PRIMARY Handler Called!" << endl;
	*piStatus = 0;

	return ERROR_CODE_SUCCESS;
}
#else
E_ERROR_DEFNS ps_ctx_define_primary_handler (P_ST_4G_API_PKT pstAPISendPayload)
{
	cout<<"PS_CTX_DEFINE_PRIMARY Handler Called!" << endl;

	return ERROR_CODE_SUCCESS;
}
#endif

/***********************************************************************
*	ps_ctx_define_secondary_handler -
*	Emulation logic for the ps_ctx_define_secondary command
*
*	Parameters
*   uiTimeout			- Time out value
*	pstAPISendPayload	- TLV of the Parameters to be sent
*	piStatus			- Status of the operation
*	pstAPIResp			- TLV of the response to the command
***********************************************************************/

#ifdef EXEC_CMD
E_ERROR_DEFNS ps_ctx_define_secondary_handler (	UINT uiTimeout, 
									P_ST_4G_API_PKT pstAPISendPayload,
									PINT32 piStatus,
									P_ST_4G_API_PKT *pstAPIRespRcvd)
{
	cout<<"PS_CTX_DEFINE_SECONDARY Handler Called!" << endl;
	*piStatus = 0;

	return ERROR_CODE_SUCCESS;
}
#else
E_ERROR_DEFNS ps_ctx_define_secondary_handler (P_ST_4G_API_PKT pstAPISendPayload)
{
	cout<<"PS_CTX_DEFINE_SECONDARY Handler Called!" << endl;

	return ERROR_CODE_SUCCESS;
}
#endif

/***********************************************************************
*	ps_ctx_undefine_secondary_handler -
*	Emulation logic for the ps_ctx_undefine_secondary command
*
*	Parameters
*   uiTimeout			- Time out value
*	pstAPISendPayload	- TLV of the Parameters to be sent
*	piStatus			- Status of the operation
*	pstAPIResp			- TLV of the response to the command
***********************************************************************/

#ifdef EXEC_CMD
E_ERROR_DEFNS ps_ctx_undefine_secondary_handler (	UINT uiTimeout, 
									P_ST_4G_API_PKT pstAPISendPayload,
									PINT32 piStatus,
									P_ST_4G_API_PKT *pstAPIRespRcvd)
{
	cout<<"PS_CTX_UNDEFINE_SECONDARY Handler Called!" << endl;
	*piStatus = 0;

	return ERROR_CODE_SUCCESS;
}
#else
E_ERROR_DEFNS ps_ctx_undefine_secondary_handler (P_ST_4G_API_PKT pstAPISendPayload)
{
	cout<<"PS_CTX_UNDEFINE_SECONDARY Handler Called!" << endl;

	return ERROR_CODE_SUCCESS;
}
#endif

/***********************************************************************
*	ps_ctx_undefine_primary_handler -
*	Emulation logic for the ps_ctx_undefine_secondary command
*
*	Parameters
*   uiTimeout			- Time out value
*	pstAPISendPayload	- TLV of the Parameters to be sent
*	piStatus			- Status of the operation
*	pstAPIResp			- TLV of the response to the command
***********************************************************************/

#ifdef EXEC_CMD
E_ERROR_DEFNS ps_ctx_undefine_primary_handler (	UINT uiTimeout, 
									P_ST_4G_API_PKT pstAPISendPayload,
									PINT32 piStatus,
									P_ST_4G_API_PKT *pstAPIRespRcvd)
{
	cout<<"PS_CTX_UNDEFINE_PRIMARY Handler Called!" << endl;
	*piStatus = 0;

	return ERROR_CODE_SUCCESS;
}
#else
E_ERROR_DEFNS ps_ctx_undefine_primary_handler (P_ST_4G_API_PKT pstAPISendPayload)
{
	cout<<"PS_CTX_UNDEFINE_PRIMARY Handler Called!" << endl;

	return ERROR_CODE_SUCCESS;
}
#endif

/***********************************************************************
*	net_cfg_set_handler -
*	Emulation logic for the net_cfg_set command
*
*	Parameters
*   uiTimeout			- Time out value
*	pstAPISendPayload	- TLV of the Parameters to be sent
*	piStatus			- Status of the operation
*	pstAPIResp			- TLV of the response to the command
***********************************************************************/

#ifdef EXEC_CMD
E_ERROR_DEFNS net_cfg_set_handler (	UINT uiTimeout, 
									P_ST_4G_API_PKT pstAPISendPayload,
									PINT32 piStatus,
									P_ST_4G_API_PKT *pstAPIRespRcvd)
{
	cout<<"NET_CFG_SET Handler Called!" << endl;
	*piStatus = 0;

	return ERROR_CODE_SUCCESS;
}
#else
E_ERROR_DEFNS net_cfg_set_handler (P_ST_4G_API_PKT pstAPISendPayload)
{
	cout<<"NET_CFG_SET Handler Called!" << endl;

	return ERROR_CODE_SUCCESS;
}
#endif

/***********************************************************************
*	net_cfg_get_handler -
*	Emulation logic for the net_cfg_get command
*
*	Parameters
*   uiTimeout			- Time out value
*	pstAPISendPayload	- TLV of the Parameters to be sent
*	piStatus			- Status of the operation
*	pstAPIResp			- TLV of the response to the command
***********************************************************************/

#ifdef EXEC_CMD
E_ERROR_DEFNS net_cfg_get_handler (	UINT uiTimeout, 
									P_ST_4G_API_PKT pstAPISendPayload,
									PINT32 piStatus,
									P_ST_4G_API_PKT *pstAPIRespRcvd)
{
	cout<<"NET_CFG_GET Handler Called!" << endl;
	*piStatus = 0;

	return ERROR_CODE_SUCCESS;
}
#else
E_ERROR_DEFNS net_cfg_get_handler (P_ST_4G_API_PKT pstAPISendPayload)
{
	cout<<"NET_CFG_GET Handler Called!" << endl;

	return ERROR_CODE_SUCCESS;
}
#endif

/***********************************************************************
*	prop_APN_table_set_handler -
*	Emulation logic for the prop_APN_table_set API
*
*	Parameters
*   uiTimeout			- Time out value
*	pstAPISendPayload	- TLV of the Parameters to be sent
*	piStatus			- Status of the operation
*	pstAPIResp			- TLV of the response to the command
***********************************************************************/

#ifdef EXEC_CMD
E_ERROR_DEFNS prop_APN_table_set_handler (	UINT uiTimeout, 
									P_ST_4G_API_PKT pstAPISendPayload,
									PINT32 piStatus,
									P_ST_4G_API_PKT *pstAPIRespRcvd)
{
	cout<<"prop_APN_table_set Handler Called!" << endl;
	*piStatus = 0;

	return ERROR_CODE_SUCCESS;
}
#else
E_ERROR_DEFNS prop_APN_table_set_handler (P_ST_4G_API_PKT pstAPISendPayload)
{
	cout<<"prop_APN_table_set Handler Called!" << endl;

	return ERROR_CODE_SUCCESS;
}
#endif

/***********************************************************************
*	prop_APN_table_get_handler -
*	Emulation logic for the prop_APN_table_get API
*
*	Parameters
*   uiTimeout			- Time out value
*	pstAPISendPayload	- TLV of the Parameters to be sent
*	piStatus			- Status of the operation
*	pstAPIResp			- TLV of the response to the command
***********************************************************************/

#ifdef EXEC_CMD
E_ERROR_DEFNS prop_APN_table_get_handler (	UINT uiTimeout, 
									P_ST_4G_API_PKT pstAPISendPayload,
									PINT32 piStatus,
									P_ST_4G_API_PKT *pstAPIRespRcvd)
{
	cout<<"prop_APN_table_get Handler Called!" << endl;
	*piStatus = 0;

	return ERROR_CODE_SUCCESS;
}
#else
E_ERROR_DEFNS prop_APN_table_get_handler (P_ST_4G_API_PKT pstAPISendPayload)
{
	cout<<"prop_APN_table_get Handler Called!" << endl;

	return ERROR_CODE_SUCCESS;
}
#endif

/***********************************************************************
*	prop_IP_IF_Status_handler -
*	Emulation logic for the prop_IP_IF_Status API
*
*	Parameters
*   uiTimeout			- Time out value
*	pstAPISendPayload	- TLV of the Parameters to be sent
*	piStatus			- Status of the operation
*	pstAPIResp			- TLV of the response to the command
***********************************************************************/

#ifdef EXEC_CMD
E_ERROR_DEFNS prop_IP_IF_Status_handler (	UINT uiTimeout, 
									P_ST_4G_API_PKT pstAPISendPayload,
									PINT32 piStatus,
									P_ST_4G_API_PKT *pstAPIRespRcvd)
{
	cout<<"prop_IP_IF_Status Handler Called!" << endl;
	*piStatus = 0;

	return ERROR_CODE_SUCCESS;
}
#else
E_ERROR_DEFNS prop_IP_IF_Status_handler (P_ST_4G_API_PKT pstAPISendPayload)
{
	cout<<"prop_IP_IF_Status Handler Called!" << endl;

	return ERROR_CODE_SUCCESS;
}
#endif

/***********************************************************************
*	net_sig_qual_get_handler -
*	Emulation logic for the net_sig_qual_get API
*
*	Parameters
*   uiTimeout			- Time out value
*	pstAPISendPayload	- TLV of the Parameters to be sent
*	piStatus			- Status of the operation
*	pstAPIResp			- TLV of the response to the command
***********************************************************************/

#ifdef EXEC_CMD
E_ERROR_DEFNS net_sig_qual_get_handler (	UINT uiTimeout, 
									P_ST_4G_API_PKT pstAPISendPayload,
									PINT32 piStatus,
									P_ST_4G_API_PKT *pstAPIRespRcvd)
{
	cout<<"net_sig_qual_get Handler Called!" << endl;
	*piStatus = 0;

	return ERROR_CODE_SUCCESS;
}
#else
E_ERROR_DEFNS net_sig_qual_get_handler (P_ST_4G_API_PKT pstAPISendPayload)
{
    cout<<"net_sig_qual_get Handler Called!" << endl;

    UINT u32TLVBytesSoFar = 0;
    UINT uiLength, uiAllocLen, uiData;
    UINT bufSize;
    PUCHAR puchBufferPtr = NULL;
    P_ST_4G_API_PKT pAPIPkt = (P_ST_4G_API_PKT)new char[2000];
    uiLength = InitializeAPIPkt(pAPIPkt);
    INT32 iExecStatus = 0;
    P_ST_CALLBACK_INFO callbackinfo;

    callbackinfo = (P_ST_CALLBACK_INFO)malloc(sizeof(ST_CALLBACK_INFO)+MAX_MEM_SIZE);
    callbackinfo->uicmd_id = CAPI_CMD_LTE_NET_SIG_QUAL_GET;
    callbackinfo->uinum_tlvs = 1 ;
    callbackinfo->iStatus = 0;
    callbackinfo->pTLVBuf[0].uiType = eTYPE_UINT;
    callbackinfo->pTLVBuf[0].uiLength = sizeof(U_UINT8);

    //create TLV1
    U_UINT32 Rssi = (rand()%100);
    uiData = Rssi;
    uiAllocLen = TLV_Creator(&uiData, eValue, eTYPE_UINT, sizeof(uiData),
                 uiLength, u32TLVBytesSoFar, &pAPIPkt);
    uiLength += uiAllocLen;
    u32TLVBytesSoFar += uiAllocLen;
    //create TLV2
    U_UINT32 Ber = (rand()%100);
    uiData = Ber;
    uiAllocLen = TLV_Creator(&uiData, eValue, eTYPE_UINT, sizeof(uiData),
                 uiLength, u32TLVBytesSoFar, &pAPIPkt);
    uiLength += uiAllocLen;
    u32TLVBytesSoFar += uiAllocLen;


    puchBufferPtr = reinterpret_cast<PUCHAR>(pAPIPkt->astTLVBuffer) + u32TLVBytesSoFar;
    pAPIPkt->uiTLVTotalLength = (puchBufferPtr - reinterpret_cast<PUCHAR>(pAPIPkt));
    bufSize = pAPIPkt->uiTLVTotalLength - 8;
    pAPIPkt->uiNumTLVs = 2;
    callbackinfo->uinum_tlvs = pAPIPkt->uiNumTLVs;
    callbackinfo->iStatus = 0;
    memcpy(callbackinfo->pTLVBuf,pAPIPkt->astTLVBuffer,bufSize);
    g_tDeviceParams.pCallback(pAPIPkt->uiTLVTotalLength , callbackinfo);
    free(callbackinfo);
	//delete pAPIPkt;
	//pAPIPkt = NULL;

return ERROR_CODE_SUCCESS;
}
#endif




/***********************************************************************
*	encapsulated_at_cmd_handler -
*	Emulation logic for the Encapsulated AT Command API
*
*	Parameters
*   uiTimeout			- Time out value
*	pstAPISendPayload	- TLV of the Parameters to be sent
*	piStatus			- Status of the operation
*	pstAPIResp			- TLV of the response to the command
***********************************************************************/

#ifdef EXEC_CMD
E_ERROR_DEFNS encapsulated_at_cmd_handler (	UINT uiTimeout, 
									P_ST_4G_API_PKT pstAPISendPayload,
									PINT32 piStatus,
									P_ST_4G_API_PKT *pstAPIRespRcvd)
{
	cout<<"encapsulated_at_cmd Handler Called!" << endl;
	*piStatus = 0;

	return ERROR_CODE_SUCCESS;
}
#else
E_ERROR_DEFNS encapsulated_at_cmd_handler (P_ST_4G_API_PKT pstAPISendPayload)
{
	cout<<"encapsulated_at_cmd Handler Called!" << endl;

	return ERROR_CODE_SUCCESS;
}
#endif

/***********************************************************************
*	netstatusget_handler -
*	Emulation logic for the NET STATUS GET API
*
*	Parameters
*   uiTimeout			- Time out value
*	pstAPISendPayload	- TLV of the Parameters to be sent
*	piStatus			- Status of the operation
*	pstAPIResp			- TLV of the response to the command
***********************************************************************/
E_ERROR_DEFNS netstatusget_handler (P_ST_4G_API_PKT pstAPISendPayload)
{
	cout<<"netstatusget_handler Handler Called!" << endl;

    U_UINT32 sInfo;
    U_UINT32 read_size;
    INT32 iExecStatus = 0;
    UINT u32TLVBytesSoFar = 0;
    UINT uiLength, uiAllocLen, uiData;
    PUCHAR puchBufferPtr = NULL;
    P_ST_4G_API_PKT pAPIPkt = (P_ST_4G_API_PKT)new char[2000];
    uiLength = InitializeAPIPkt(pAPIPkt);
    P_ST_CALLBACK_INFO callbackinfo;
    UINT uiPktTotalLength = pstAPISendPayload->uiTLVTotalLength;
    UINT uiTLVsLength = uiPktTotalLength - 8;

    callbackinfo = (P_ST_CALLBACK_INFO)malloc(sizeof(ST_CALLBACK_INFO)+MAX_MEM_SIZE);
    callbackinfo->uicmd_id = CAPI_CMD_LTE_NET_STATUS_GET;
    callbackinfo->uinum_tlvs = 1 ;
    callbackinfo->iStatus = 0;
    callbackinfo->pTLVBuf[0].uiType = eTYPE_UINT;
    callbackinfo->pTLVBuf[0].uiLength = sizeof(U_UINT32);
    //parsing TLV
    P_ST_TLV temp_TLV = pstAPISendPayload->astTLVBuffer;
    read_size = ParseTLVData_NetList(temp_TLV, &sInfo);

    if(stateInfo == 1)
    {
        uiData = 1;
    }
    else{
        uiData = 0;
    }
   //create TLV
    uiAllocLen = TLV_Creator(&uiData, eValue, eTYPE_UINT, sizeof(uiData),
                    uiLength, u32TLVBytesSoFar, &pAPIPkt);
    uiLength += uiAllocLen;
    u32TLVBytesSoFar += uiAllocLen;
    puchBufferPtr = reinterpret_cast<PUCHAR>(pAPIPkt->astTLVBuffer) + u32TLVBytesSoFar;
    pAPIPkt->uiTLVTotalLength = (puchBufferPtr - reinterpret_cast<PUCHAR>(pAPIPkt));
    U_UINT32 buffsize= pAPIPkt->uiTLVTotalLength;
    pAPIPkt->uiNumTLVs = 1;
    callbackinfo->uinum_tlvs = pAPIPkt->uiNumTLVs;
    callbackinfo->iStatus = 0;
    memcpy(callbackinfo->pTLVBuf,pAPIPkt->astTLVBuffer,buffsize);
    g_tDeviceParams.pCallback(buffsize , callbackinfo);
    free(callbackinfo);
	//delete pAPIPkt;
	//pAPIPkt = NULL;
return ERROR_CODE_SUCCESS;
}

/***********************************************************************
*	netgetinfo_handler -
*	Emulation logic for the getting the IP Address, Subnet Mask, GW Address, APN Name ,DYN DNS
*
*	Parameters
*   uiTimeout			- Time out value
*	pstAPISendPayload	- TLV of the Parameters to be sent
*	piStatus			- Status of the operation
*	pstAPIResp			- TLV of the response to the command
***********************************************************************/
E_ERROR_DEFNS netgetinfo_handler (P_ST_4G_API_PKT pstAPISendPayload)
{
    cout<<"netgetinfo_handler Handler Called!" << endl;

    UINT bufSize;
    U_UINT32 Info;
    U_UINT32 ctxInfo;
    U_UINT32 read_size1;
    U_UINT32 read_size2;
    INT32 iExecStatus = 0;
    UINT u32TLVBytesSoFar = 0;
    UINT uiLength, uiAllocLen, uiData;
    PUCHAR puchBufferPtr = NULL;
    P_ST_4G_API_PKT pAPIPkt = (P_ST_4G_API_PKT)new char[2000];
    uiLength = InitializeAPIPkt(pAPIPkt);
    PCHAR str_data = new CHAR[MAX_ADDRESS_SIZE];

    P_ST_CALLBACK_INFO callbackinfo;
    callbackinfo = (P_ST_CALLBACK_INFO)malloc(sizeof(ST_CALLBACK_INFO)+MAX_MEM_SIZE);
    callbackinfo->uicmd_id = CAPI_CMD_LTE_PS_CTX_INFO_GET;
    callbackinfo->uinum_tlvs = 2 ;
    callbackinfo->iStatus = 0;
    callbackinfo->pTLVBuf[0].uiType = eTYPE_UCHAR;
    callbackinfo->pTLVBuf[0].uiLength = sizeof(U_UINT8);
    UINT uiPktTotalLength = pstAPISendPayload->uiTLVTotalLength;
    UINT uiTLVsLength = uiPktTotalLength - 8;

    //parsing TLV1
    P_ST_TLV temp_TLV1 = pstAPISendPayload->astTLVBuffer;
    read_size1 = ParseTLVData_NetList(temp_TLV1, &ctxInfo);
    //parsing TLV2
    P_ST_TLV temp_TLV2 = (P_ST_TLV)((PCHAR)pstAPISendPayload->astTLVBuffer + read_size1);
    read_size2 = ParseTLVData_NetList(temp_TLV2, &Info);

    switch(Info)
    {
      case E_INFO_PDP_ADDRESS:
          {
            U_SINT8 ip[]="192.168.1.134";
           //create TLV 1
            uiData = 256;
            uiAllocLen = TLV_Creator(&uiData, eValue, eTYPE_UINT, sizeof(uiData),
                            uiLength, u32TLVBytesSoFar, &pAPIPkt);
            uiLength += uiAllocLen;
            u32TLVBytesSoFar += uiAllocLen;
            //create TLV 2
            strcpy(str_data,(char*)ip);
            uiAllocLen = TLV_Creator(str_data, eArray, eTYPE_CHAR,strlen(ip)+1,
            uiLength, u32TLVBytesSoFar, &pAPIPkt);
            uiLength += uiAllocLen;
            u32TLVBytesSoFar = u32TLVBytesSoFar+uiAllocLen;
        }
        break;
    case E_INFO_SUBNET_MASK:
        {
            U_SINT8 subMas[]="255.255.255.0";
            //create TLV 1
            uiData = 2048;
            uiAllocLen = TLV_Creator(&uiData, eValue, eTYPE_UINT, sizeof(uiData),
                            uiLength, u32TLVBytesSoFar, &pAPIPkt);
            uiLength += uiAllocLen;
            u32TLVBytesSoFar += uiAllocLen;
            //create TLV 2
            strcpy(str_data,(char*)subMas);
            uiAllocLen = TLV_Creator(str_data, eArray, eTYPE_CHAR,strlen(subMas)+1,
            uiLength, u32TLVBytesSoFar, &pAPIPkt);
            uiLength += uiAllocLen;
            u32TLVBytesSoFar = u32TLVBytesSoFar+uiAllocLen;
        }
        break;
        case E_INFO_GW_ADDRESS:
        {
            U_SINT8 gwAdd[]="umobile";
            //create TLV 1
            uiData = 4096;
            uiAllocLen = TLV_Creator(&uiData, eValue, eTYPE_UINT, sizeof(uiData),
                            uiLength, u32TLVBytesSoFar, &pAPIPkt);
            uiLength += uiAllocLen;
            u32TLVBytesSoFar += uiAllocLen;
            //create TLV 2
            strcpy(str_data,(char*)gwAdd);
            uiAllocLen = TLV_Creator(str_data, eArray, eTYPE_CHAR,strlen(gwAdd)+1,
            uiLength, u32TLVBytesSoFar, &pAPIPkt);
            uiLength += uiAllocLen;
            u32TLVBytesSoFar = u32TLVBytesSoFar+uiAllocLen;
        }
        break;
        case E_INFO_DYN_DNS:
        {
            U_SINT8 dynDns[]="connect";
            //create TLV 1
            uiData = 64;
            uiAllocLen = TLV_Creator(&uiData, eValue, eTYPE_UINT, sizeof(uiData),
                            uiLength, u32TLVBytesSoFar, &pAPIPkt);
            uiLength += uiAllocLen;
            u32TLVBytesSoFar += uiAllocLen;
            //create TLV 2
            strcpy(str_data,(char*)dynDns);
            uiAllocLen = TLV_Creator(str_data, eArray, eTYPE_CHAR,strlen(dynDns)+1,
            uiLength, u32TLVBytesSoFar, &pAPIPkt);
            uiLength += uiAllocLen;
            u32TLVBytesSoFar = u32TLVBytesSoFar+uiAllocLen;
        }
        break;
        case E_INFO_APN_NAME:
        {
            U_SINT8 apn[]="UMA";
            //create TLV 1
            uiData = 512;
            uiAllocLen = TLV_Creator(&uiData, eValue, eTYPE_UINT, sizeof(uiData),
                            uiLength, u32TLVBytesSoFar, &pAPIPkt);
            uiLength += uiAllocLen;
            u32TLVBytesSoFar += uiAllocLen;
            //create TLV 2
            strcpy(str_data,(char*)apn);
            uiAllocLen = TLV_Creator(str_data, eArray, eTYPE_CHAR,strlen(apn)+1,
            uiLength, u32TLVBytesSoFar, &pAPIPkt);
            uiLength += uiAllocLen;
            u32TLVBytesSoFar = u32TLVBytesSoFar+uiAllocLen;
        }
        break;
    default:
        break;
    }
        puchBufferPtr = reinterpret_cast<PUCHAR>(pAPIPkt->astTLVBuffer) + u32TLVBytesSoFar;
        pAPIPkt->uiTLVTotalLength = (puchBufferPtr - reinterpret_cast<PUCHAR>(pAPIPkt));
        bufSize = pAPIPkt->uiTLVTotalLength - 8;
        pAPIPkt->uiNumTLVs = 2;
        callbackinfo->uinum_tlvs = pAPIPkt->uiNumTLVs;
        callbackinfo->iStatus = 0;
        memcpy(callbackinfo->pTLVBuf,pAPIPkt->astTLVBuffer,bufSize);
        g_tDeviceParams.pCallback(pAPIPkt->uiTLVTotalLength , callbackinfo);
        free(callbackinfo);
	//delete pAPIPkt;
	//pAPIPkt = NULL;
return ERROR_CODE_SUCCESS;
}
/***********************************************************************
*	statisticinfo_handler -
*	Emulation logic for the NET STATISTICS GET API
*
*	Parameters
*   uiTimeout			- Time out value
*	pstAPISendPayload	- TLV of the Parameters to be sent
*	piStatus			- Status of the operation
*	pstAPIResp			- TLV of the response to the command
***********************************************************************/
E_ERROR_DEFNS statisticinfo_handler (P_ST_4G_API_PKT pstAPISendPayload)
{
    cout<<"statisticinfo_handler Handler Called!" << endl;

    UINT bufSize;
    U_UINT32 Info;
    U_UINT32 read_size;
    INT32 iExecStatus = 0;
    UINT u32TLVBytesSoFar = 0;
    UINT uiLength, uiAllocLen, uiData;
    PUCHAR puchBufferPtr = NULL;
    stCMStats_t CMStats;
    P_ST_4G_API_PKT pAPIPkt = (P_ST_4G_API_PKT)new char[2000];
    P_ST_CALLBACK_INFO callbackinfo;
    uiLength = InitializeAPIPkt(pAPIPkt);
    memset( &CMStats, 0, sizeof( stCMStats_t ) );

    callbackinfo = (P_ST_CALLBACK_INFO)malloc(sizeof(ST_CALLBACK_INFO)+300);
    callbackinfo->uicmd_id = CAPI_CMD_TM_STAT_BASE;
    callbackinfo->uinum_tlvs = 1 ;
    callbackinfo->iStatus = 0;
    callbackinfo->pTLVBuf[0].uiType = eTYPE_UINT;
    callbackinfo->pTLVBuf[0].uiLength = sizeof(U_UINT8);
    UINT uiPktTotalLength = pstAPISendPayload->uiTLVTotalLength;
    UINT uiTLVsLength = uiPktTotalLength - 8;
    P_ST_TLV temp_TLV = pstAPISendPayload->astTLVBuffer;
    read_size = ParseTLVData_NetList(temp_TLV, &Info);

    CMStats.sGenericStats.u32DLThruput  = (rand()%100)*1000;
    CMStats.sGenericStats.u32ULThruput  = (rand()%100)*1000;
    CMStats.sGenericStats.u32NumPacketsTx = (rand()%100);
    CMStats.sMacStats.sDLTBStats.u32NumErrTBs  = (rand()%100);
    CMStats.sMacStats.u32FrameNumber  = (rand()%100);
    CMStats.sMacStats.sDLTBStats.u32NumErrTBs = (rand()%100);
    //CMStats.sMacStats.sDLTBStats.u32PreHarqErr = (rand()%100);
    //CMStats.sMacStats.sULTBStats.u32NumErrTBs = (rand()%100);
    //CMStats.sMacStats.sULTBStats.u32PreHarqErr = (rand()%100);
    CMStats.sMacStats.u32NumPRACH = (rand()%100);
    CMStats.sMacStats.u32NumMacPdusRx = (rand()%100);
    CMStats.sMacStats.u32RAFailureCount = (rand()%100);
    CMStats.sMacStats.u32MeasReportsCount = (rand()%100);
    CMStats.sMacStats.u32SuccessfulSecurityActivationCount = (rand()%100);
    CMStats.sMacStats.u32HandoverCount = (rand()%100);
    CMStats.sMacStats.u32MeasReportsCount = (rand()%100);
    CMStats.sPHYStats.u32DLBw  = (rand()%100);
    CMStats.sPHYStats.u32ULBw = (rand()%100);
    //CMStats.sPHYStats.u32BurstErrors = (rand()%100); 
    CMStats.sPHYStats.u32PhyIdCell  = (rand()%100);

    //TLV 1
    uiData = 10;
    U_UINT32 s =sizeof(uiData);
    uiAllocLen = TLV_Creator(&uiData, eValue, eTYPE_UINT, sizeof(uiData),
                 uiLength, u32TLVBytesSoFar, &pAPIPkt);
    uiLength += uiAllocLen;
    u32TLVBytesSoFar += uiAllocLen;

    //TLV 2
     U_UINT32 structSize = sizeof(CMStats);
    uiAllocLen = TLV_Creator(&CMStats, eArray, eTYPE_CHAR, sizeof(CMStats),
                 uiLength, u32TLVBytesSoFar, &pAPIPkt);
    uiLength += uiAllocLen;
    u32TLVBytesSoFar += uiAllocLen;

    puchBufferPtr = reinterpret_cast<PUCHAR>(pAPIPkt->astTLVBuffer) + u32TLVBytesSoFar;
    pAPIPkt->uiTLVTotalLength = (puchBufferPtr - reinterpret_cast<PUCHAR>(pAPIPkt));
    bufSize = pAPIPkt->uiTLVTotalLength - 8;
    pAPIPkt->uiNumTLVs = 2;
    callbackinfo->iStatus = 0;
    callbackinfo->uinum_tlvs = pAPIPkt->uiNumTLVs;
    memcpy(callbackinfo->pTLVBuf,pAPIPkt->astTLVBuffer,bufSize);
    g_tDeviceParams.pCallback(pAPIPkt->uiTLVTotalLength , callbackinfo);
    free(callbackinfo);
	//delete pAPIPkt;
	//pAPIPkt = NULL;
    return ERROR_CODE_SUCCESS;
}