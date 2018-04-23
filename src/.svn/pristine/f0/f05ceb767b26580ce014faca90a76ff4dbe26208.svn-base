/*****************************************************************************
**
** Three_g_dal.cpp
** Implements Device Abstraction Layer for AT Commands for the 3G devices.
**
** Author: Mukta Sharma
** Date  : 20-APR-2012
** (c) United Mobile Apps Pvt Ltd (2010 - 2015)

    VER		  DATE		AUTHOR		DESCRIPTION
	0.1		20/04/12     MSH		Creation
****************************************************************************/
#include "Three_g_dal.h"
#include "ThreeGParser.h"
#include "uconn_ATHandler.h"
#include "uconn_server.h"

UThreeGDal::UThreeGDal(UNetworkManager* pThreeGNM) : uconnAtDal(pThreeGNM)
{
	bufferParser = new UThreeGParser(pThreeGNM);
}

UThreeGDal::~UThreeGDal()
{
	delete bufferParser;
	bufferParser = NULL;
}



U_BOOL UThreeGDal::isDevicePresent()
{
	return DAL_SUCCESS;
}

DAL_STATUS UThreeGDal::uconnGetVersionInfo()
{
	return DAL_SUCCESS;
}

DAL_STATUS UThreeGDal::uconnATModemConfig(U_UINT32 mConfig, U_UINT32 ctxId)
{
	return DAL_SUCCESS;
}

DAL_STATUS UThreeGDal::uconnConfigure(CONFIG_CODE configCode)
{
	return DAL_SUCCESS;
}

DAL_STATUS UThreeGDal::uconnConfigNetParams(NETWORK_PARAMS networkConfig,
											void* param,int ctxId)
{
	switch(networkConfig)
	{
	case CONTEXT_PARAMS:
		{
			/* To define the Primary context */
			U_PS_CTX_PRIMARY_PARAMS *priCtx = (U_PS_CTX_PRIMARY_PARAMS *)param;
			uconnATPsCtxDefPrimary(ctxId, priCtx);
		}
	}

	return DAL_SUCCESS;
}

DAL_STATUS UThreeGDal::uconnSetStatus(SET_STATUS,void*,int ctxId)
{
	return DAL_SUCCESS;
}

DAL_STATUS UThreeGDal::uconnGetStatistics(GET_STATISTICS,void*)
{
	return DAL_SUCCESS;
}

DAL_STATUS UThreeGDal::uconnReset(void* param)
{
	return DAL_SUCCESS;
}


void UThreeGDal::callBackHandler(string strResp,unsigned int cmdId)
{
	uconnAtDal::callBackHandler(strResp, cmdId);
}



/***********************************************************************
*	processBufferParserSetEvent -
*	function which sends a Event to the SM of 3G depending upon the cmdID set
*	The Event is formulated depending upon the cmdID and the corresponsding 
*	variables which are set when the AT command response are parsed by buffer 
*	parser. 
*
*	Parameters - 
*	None
***********************************************************************/
void UThreeGDal::processBufferParserSetEvent(string strResp,unsigned int cmdId)
{
	switch( bufferParser->getEventID() )
	{
	/* Handle all the events here, eventid is set in processAtCommandResp. 
	 * Then post a message to State Machine and handle in corresponsding 
	 * state handler functions.  
	 */
	case UCONN_EVT_NM_RSSI_UPDATE:
		{
			UNMParamUpdate *evt = new UNMParamUpdate(UCONN_EVT_NM_RSSI_UPDATE, (bufferParser->parsedInfo.geRSSI()),0);
			m_pThreeG_NM->getNMServer()->MsgPost((U_PUINT8)evt);
		}
		break;
	case UCONN_RSP_NM_RSSI_CINR_UPDATE:
		{
			UNMParamUpdate *evt = new UNMParamUpdate(UCONN_RSP_NM_RSSI_CINR_UPDATE, (bufferParser->parsedInfo.geRSSI()),(bufferParser->parsedInfo.getCINR()));
			m_pThreeG_NM->getNMServer()->MsgPost((U_PUINT8)evt);
		}
		break;
	case UCONN_EVT_NM_MODEM_STATE_UPDATE:
		{
			UNMParamUpdate *evt = new UNMParamUpdate(UCONN_EVT_NM_MODEM_STATE_UPDATE, (bufferParser->parsedInfo.getModem_State()),0);
			m_pThreeG_NM->getNMServer()->MsgPost((U_PUINT8)evt);
		}
		break;

	case UCONN_EVT_NM_MODEM_STATE_NETWORK_ACCESS_UPDATE:
		{
			UNMParamUpdate *evt = new UNMParamUpdate(UCONN_EVT_NM_MODEM_STATE_NETWORK_ACCESS_UPDATE, 
				(bufferParser->parsedInfo.getModem_State()),(bufferParser->parsedInfo.getNetworkAccessType()));
			m_pThreeG_NM->getNMServer()->MsgPost((U_PUINT8)evt);
		}
		break;

	case UCONN_EVT_NM_MANUFACTURE_UPDATE:
		{
			UNMVersionInfo *evt = new UNMVersionInfo(bufferParser->parsedInfo.getVersionInfo());
				m_pThreeG_NM->getNMServer()->MsgPost((U_PUINT8)evt);
		}
		break; 
		
	case UCONN_EVT_NM_NETWORK_UPDATE:
		{
			UNMSearchNetworksRsp *evt = new UNMSearchNetworksRsp((bufferParser->parsedInfo.getNetworkName()));
			m_pThreeG_NM->getNMServer()->MsgPost((U_PUINT8)evt);

		}
		break;
	case UCONN_EVT_NM_USSD_RSP:
		{
			UNMUSSDRsp *evt = new UNMUSSDRsp((bufferParser->parsedInfo.getUSSDRsp()));
			m_pThreeG_NM->getNMServer()->MsgPost((U_PUINT8)evt);

		}
		break;
	}

	uconnAtDal::processBufferParserSetEvent(strResp,cmdId);
}

/***********************************************************************
*	ATCommandEchoOff -
*	function which sends an AT command to set Echo off for the modem. 
*
*	Parameters - 
*	None
***********************************************************************/
DAL_STATUS UThreeGDal::ATCommandEchoOff()
{
	U_STRING atcmd;
	
	/* AT Command to Set Echo Off */
	atcmd.clear();
	atcmd.append("ATE0");
	atcmd += CR;

	/* Send this atcmd to the device */
	m_pNM->mATHandler->sendATCommand(atcmd.c_str(),atcmd.size());

	return DAL_SUCCESS;
}

/***********************************************************************
*	ATCommandSetAutomaticNetworkMode -
*	function which sends an AT Command to set the Network mode to 
*	automatic with Long Alphanumeric Format.
*
*	Parameters - 
*	None
***********************************************************************/
DAL_STATUS UThreeGDal::ATCommandSetAutomaticNetworkMode()
{
	U_STRING atcmd;
	
	/* AT Command to set the Network mode to automatic with Long Alphanumeric Format. */
	atcmd.clear();
	atcmd.append("AT+COPS=0,0");
	atcmd += CR;

	/* Send this atcmd to the device */
	m_pNM->mATHandler->sendATCommand(atcmd.c_str(),atcmd.size());

	return DAL_SUCCESS;
}

/***********************************************************************
*	ATCommandGetModemInfo -
*	function which sends an AT command to get the Modem Info. 
*
*	Parameters - 
*	None
***********************************************************************/
DAL_STATUS UThreeGDal::ATCommandGetModemInfo()
{
	U_STRING atcmd;
	
	/* AT*/
	atcmd.clear();
	atcmd.append("ATI");
	atcmd += CR;

	/* Send this atcmd to the device */
	m_pNM->mATHandler->sendATCommand(atcmd.c_str(),atcmd.size());

	return DAL_SUCCESS;
}


/***********************************************************************
*	ATCommandSetFunctionality -
*	function which sends an AT command to set the modem functionality. 
*
*	Parameters - 
*	None
***********************************************************************/
DAL_STATUS UThreeGDal::ATCommandSetFunctionality()
{
	U_STRING atcmd;
	
	/* AT*/
	atcmd.clear();
	atcmd.append("AT+CFUN=1");
	atcmd += CR;

	/* Send this atcmd to the device */
	m_pNM->mATHandler->sendATCommand(atcmd.c_str(),atcmd.size());

	return DAL_SUCCESS;
}


/***********************************************************************
*	ATCommandGetSignalStrength -
*	function which sends an AT command to get signa strength. 
*
*	Parameters - 
*	None
***********************************************************************/
DAL_STATUS UThreeGDal::ATCommandGetSignalStrength()
{
	U_STRING atcmd;
	
	/* AT COmmand to Get signal strength */
	atcmd.clear();
	atcmd.append("AT+CSQ");
	atcmd += CR;

	/* Send this atcmd to the device */
	m_pNM->mATHandler->sendATCommand(atcmd.c_str(),atcmd.size());

	return DAL_SUCCESS;
}

/***********************************************************************
*	ATCommandRegisterNetwork -
*	function which sends an AT command to set the Network status. 
*
*	Parameters - 
*	None
***********************************************************************/
DAL_STATUS UThreeGDal::ATCommandRegisterNetwork()
{
	U_STRING atcmd;
	
	/* AT*/
	atcmd.clear();
	atcmd.append("AT+CGREG=2");
	atcmd += CR;

	/* Send this atcmd to the device */
	m_pNM->mATHandler->sendATCommand(atcmd.c_str(),atcmd.size());

	return DAL_SUCCESS;
}

/***********************************************************************
*	ATCommandRegisterNetworkStatus -
*	function sends an AT command to get the status of register to network. 
*
*	Parameters - 
*	None
***********************************************************************/
DAL_STATUS UThreeGDal::ATCommandRegisterNetworkStatus()
{
	U_STRING atcmd;
	
	/* AT*/
	atcmd.clear();
	atcmd.append("AT+CGREG?");
	atcmd += CR;

	/* Send this atcmd to the device */
	m_pNM->mATHandler->sendATCommand(atcmd.c_str(),atcmd.size());

	return DAL_SUCCESS;
}

/***********************************************************************
*	ATCommandInitNetworkSearch -
*	function for Initiating Network Search to get the list of PLMN's.
*
*	Parameters - 
*	None
***********************************************************************/
DAL_STATUS UThreeGDal::ATCommandInitNetworkSearch()
{
	U_STRING atcmd;
	
	/* AT*/
	atcmd.clear();
	atcmd.append("AT+COPS=?");
	atcmd += CR;

	/* Send this atcmd to the device */
	m_pNM->mATHandler->sendATCommand(atcmd.c_str(),atcmd.size());

	return DAL_SUCCESS;
}

/***********************************************************************
*	ATCommandUSSDStr -
*	function for Sending the USSD String tot he network. 
*
*	Parameters - 
*	Inout is the USSD string to be sent to the network. 
***********************************************************************/
DAL_STATUS UThreeGDal::ATCommandUSSDStr(U_STRING ussd_str)
{
	U_STRING atcmd;
	
	/* AT*/
	atcmd.clear();
//	atcmd.append("AT+CUSD=1,\"2A594C3602\",15");
	atcmd.append("AT+CUSD=1,\"");
	atcmd.append(ussd_str);
	atcmd.append("\",15");
	atcmd += CR;

	/* Send this atcmd to the device */
	m_pNM->mATHandler->sendATCommand(atcmd.c_str(),atcmd.size());

	return DAL_SUCCESS;
}

/***********************************************************************
*	ATCommandGetHardwareInfo -
*	function which sends an AT command to get the hardware 
*   information of the Modem. Specific to Huawei modem. 
*
*	Parameters - 
*	None
***********************************************************************/
DAL_STATUS UThreeGDal::ATCommandGetHardwareInfo()
{
	U_STRING atcmd;
	
	/* AT*/
	atcmd.clear();
	atcmd.append("AT^HWVER");
	atcmd += CR;

	/* Send this atcmd to the device */
	m_pNM->mATHandler->sendATCommand(atcmd.c_str(),atcmd.size());

	return DAL_SUCCESS;
}

/***********************************************************************
*	ATCommandGetSystemConfigInfo -
*	function which sends an AT command to get the system configuration 
*   information of the Modem. Specific to Huawei modem. 
*
*	Parameters - 
*	None
***********************************************************************/
DAL_STATUS UThreeGDal::ATCommandGetSystemConfigInfo()
{
	U_STRING atcmd;
	
	/* AT*/
	atcmd.clear();
	atcmd.append("AT^SYSCFG");
	atcmd += CR;

	/* Send this atcmd to the device */
	m_pNM->mATHandler->sendATCommand(atcmd.c_str(),atcmd.size());

	return DAL_SUCCESS;
}
