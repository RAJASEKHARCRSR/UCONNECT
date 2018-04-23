/*****************************************************************************
**
** uconn_AT_DAL.cpp
** Implements Device Abstraction Layer for AT Commands
**
** Author: Sathyanarayanan S
** Date  : 20-JUL-2011
** (c) United Mobile Apps Pvt Ltd (2010 - 2015)

    VER		  DATE		AUTHOR		DESCRIPTION
	0.1		20/07/11     SSN		Creation
****************************************************************************/

#include "uconn_AT_DAL.h"
#include <stdio.h>
#ifdef WIN32
#include <conio.h>
#include <stdlib.h>
#endif
#include "uconn_ATHandler.h"
#include "ULog.h"
#include "UBufferParser.h"
#include "UHuawei4GParser.h"
#include  "uconn_server.h"

extern ULog uLogger;
extern UConnServer *gConnServer;

bool dummyATCommand = false;




P_AT_CALLBACK_WITH_ID callbackfnWithId_ptr = InitCallbackWithId;


void InitCallbackWithId(string strResp, 
						unsigned int cmdId,
						UNetworkManager* p_NM)
{
	//gConnServer->getDal()->callBackHandler(strResp,cmdId);
	p_NM->getNMServer()->getDal()->callBackHandler(strResp, cmdId);
}


uconnAtDal::uconnAtDal(UNetworkManager* p_NM):uconnDal()
{
	m_pNM = p_NM;
	m_pLTE_NM = NULL;
	m_pThreeG_NM = NULL;
	if(p_NM->getTechID() == LTE_TECH)
	{
		m_pLTE_NM = reinterpret_cast<ULTE_NM*>(p_NM);
	}
	else if(p_NM->getTechID() == THREE_G_TECH)
	{
		m_pThreeG_NM = reinterpret_cast<UThreeG_NM*>(p_NM);	
	}
} 


/***********************************************************************
*	uconnATInitDevice -
*	Initializes the Modem 
*
*	Parameters
*	fnCallback	- Pointer to the callback function
***********************************************************************/
DAL_STATUS uconnAtDal::uconnATInitDevice(string sComPort, 
										 UNetworkManager* pNM)
{
	/* Call to initialize the device/register the callback function */
	//m_pNM->mATHandler->openDevice(fnCallback);
	//call back function with ID is initialised here

	P_AT_CALLBACK_WITH_ID fnptr = &InitCallbackWithId;

	return m_pNM->mATHandler->initialize(sComPort, fnptr, pNM);
	
}

DAL_STATUS uconnAtDal::uconnATResetDevice()
{
	m_pNM->mATHandler->resetComPort();
	return DAL_SUCCESS;
}


void uconnAtDal::callBackHandler(string strResp,unsigned int cmdId)
{
	UEvent* rspEvt = NULL;
	bufferParser->setEventID(0);
	string atCmdResp = strResp;
	
	if ( m_pLTE_NM != NULL ) 
	{
		if(m_pLTE_NM->mNMInfo.ATCmdMode == 1)
		{
			UATCmdResponseEvent *respAT = new UATCmdResponseEvent();
			respAT->setATCmdRspString(atCmdResp);
			//SAPI_eventCB((UEvent*)respAT);
			//return;
		}
	}
	switch(cmdId)
	{
		case 1:
			bufferParser->processAtCommandResp(atCmdResp);
			break;
		case 2:
			bufferParser->processUnSolResp(atCmdResp);
			break;
		case 3:
			{
			//device open event
			//m_pLTE_NM->mCommonInfo.device_status = 1;
			//m_pLTE_NM->mCommonInfo.system_notification = NOTI_DEVICE_OPEN;

				if ( m_pLTE_NM != NULL ) 
				{
					m_pLTE_NM->mCommonInfo.device_status = 1;
					m_pLTE_NM->mNMInfo.system_notification = NOTI_DEVICE_OPEN;
					UDeviceStateChange *rspEvt = new UDeviceStateChange(NOTI_DEVICE_OPEN);
					//gConnServer->MsgPost((U_PUINT8)rspEvt);
					m_pLTE_NM->getNMServer()->MsgPost((U_PUINT8)rspEvt);
				}
				else if ( m_pThreeG_NM != NULL )
				{
					m_pThreeG_NM->mCommonInfo.device_status = 1;
					UDeviceStateChange *rspEvt = new UDeviceStateChange(NOTI_DEVICE_OPEN);
					//gConnServer->MsgPost((U_PUINT8)rspEvt);
					m_pThreeG_NM->getNMServer()->MsgPost((U_PUINT8)rspEvt);
				}
			}
			break;
	
		case 4:
			{
			//device shutdown shut down event
			//m_pLTE_NM->mCommonInfo.device_status = 0;
			//m_pLTE_NM->mCommonInfo.system_notification = NOTI_DEVICE_NOT_OPEN;
			m_pLTE_NM->mCommonInfo.device_status = 0;
			m_pLTE_NM->mNMInfo.system_notification = NOTI_DEVICE_NOT_OPEN;
			UDeviceStateChange *rspEvt = new UDeviceStateChange(NOTI_DEVICE_NOT_OPEN);
			m_pLTE_NM->getNMServer()->MsgPost((U_PUINT8)rspEvt);
			}
			break;
		default:
			break;
	}
		processBufferParserSetEvent(strResp,cmdId);
}




void uconnAtDal::processBufferParserSetEvent(string strResp,unsigned int cmdId)
{	
	string cliStr = strResp;
	
	/* Callback function */
	
	/* Create the Event, Post it and then pass it on to the client */
	switch( bufferParser->getEventID() )
	{
		case UCONN_EVT_IMS_REG_SUCCESS:
			{
				UImsRegSucess* successEvt = new UImsRegSucess(UCONN_EVT_IMS_REG_SUCCESS);
                //m_pLTE_NM->mCommonInfo.imsParams.IMS_Reg_Status = IMS_REG_SUCCESS;
				m_pLTE_NM->mCommonInfo.imsParams.IMS_Reg_Status = IMS_REG_SUCCESS;
				m_pLTE_NM->getNMServer()->MsgPost((U_PUINT8)successEvt);
			}
			break;
		case UCONN_EVT_IMS_REG_FAILURE:
			{
				UImsRegFailure* failureEvt = new UImsRegFailure(UCONN_EVT_IMS_REG_FAILURE);
                //m_pLTE_NM->mCommonInfo.imsParams.IMS_Reg_Status = IMS_REG_FAILURE;
				m_pLTE_NM->mCommonInfo.imsParams.IMS_Reg_Status = IMS_REG_FAILURE;
				m_pLTE_NM->getNMServer()->MsgPost((U_PUINT8)failureEvt);
			}
			break;
		case UCONN_EVT_IMS_DEREG:
			{
				UImsDeReg* deRegEvt = new UImsDeReg(UCONN_EVT_IMS_DEREG);
                //m_pLTE_NM->mCommonInfo.imsParams.IMS_Reg_Status = IMS_DEREG;
				m_pLTE_NM->mCommonInfo.imsParams.IMS_Reg_Status = IMS_DEREG;
				m_pLTE_NM->getNMServer()->MsgPost((U_PUINT8)deRegEvt);
			}
			break;
		case UCONN_EVT_REQ_ATTACH:
			{
				UImsReqAttachPDN* attachEvt = new UImsReqAttachPDN(UCONN_EVT_REQ_ATTACH);
                //m_pLTE_NM->mCommonInfo.imsParams.IMS_Reg_Status = REQ_ATTACH;
				m_pLTE_NM->mCommonInfo.imsParams.IMS_Reg_Status = REQ_ATTACH;
				m_pLTE_NM->getNMServer()->MsgPost((U_PUINT8)attachEvt);
			}
			break;
		case UCONN_EVT_REQ_DETACH:
			{
				UImsReqDetachPDN* detachEvt = new UImsReqDetachPDN(UCONN_EVT_REQ_DETACH);
                //m_pLTE_NM->mCommonInfo.imsParams.IMS_Reg_Status = REQ_DETACH;
				m_pLTE_NM->mCommonInfo.imsParams.IMS_Reg_Status = REQ_DETACH;
				m_pLTE_NM->getNMServer()->MsgPost((U_PUINT8)detachEvt);
			}
			break;
		case UCONN_EVT_REQ_POR:
			{
				UImsReqPowerCycle* successEvt = new UImsReqPowerCycle(UCONN_EVT_REQ_POR);
                //m_pLTE_NM->mCommonInfo.imsParams.IMS_Reg_Status = REQ_POR;
				m_pLTE_NM->mCommonInfo.imsParams.IMS_Reg_Status = REQ_POR;
				m_pLTE_NM->getNMServer()->MsgPost((U_PUINT8)successEvt);
			}
			break;
		case UCONN_EVT_REQ_IMS_LTD_MODE:
			{
				UImsRegLtdMode* ltdModeEvt = new UImsRegLtdMode(UCONN_EVT_REQ_IMS_LTD_MODE);
                //m_pLTE_NM->mCommonInfo.imsParams.IMS_Reg_Status = REQ_IMS_LTD_MODE;
				m_pLTE_NM->mCommonInfo.imsParams.IMS_Reg_Status = REQ_IMS_LTD_MODE;
				m_pLTE_NM->getNMServer()->MsgPost((U_PUINT8)ltdModeEvt);
			}
			break;
		case UCONN_EVT_LTE_REG_STATUS:
			{
				////cout << "<<<<<<<<<<<Now the Power State is:" << bufferParser->powerState << endl; 
				//ULTERegStatusEvent* regEvt = new ULTERegStatusEvent(bufferParser->powerState);
				ULTERegStatusEvent* regEvt = new ULTERegStatusEvent(m_pLTE_NM->mNMInfo.Modem_state);
                m_pLTE_NM->getNMServer()->MsgPost((U_PUINT8)regEvt);			
			}
			break;
		case UCONN_EVT_PRIMARY_ACT:
			{
				UConnectingEvent* connEvt = new UConnectingEvent(UCONN_EVT_PRIMARY_ACT);
				m_pLTE_NM->getNMServer()->MsgPost((U_PUINT8)connEvt);
				//m_pLTE_NM->mNMInfo.Modem_state = bufferParser->parsedInfo.getModem_State();
				m_pLTE_NM->mNMInfo.Modem_state = 
					bufferParser->parsedInfo.getModem_State();
				
			}
			break;
		case UCONN_EVT_NET_DETACH:
			{

				UNetdetachEvent* netDetachEvt = new UNetdetachEvent();
				m_pLTE_NM->mNMInfo.Modem_state = 
					bufferParser->parsedInfo.getModem_State();
				m_pLTE_NM->getNMServer()->MsgPost((U_PUINT8)netDetachEvt);
				
			}
			break;
		case UCONN_CMD_NET_STATUS_GET:
			{
				m_pLTE_NM->mNMInfo.Modem_state = 
					bufferParser->parsedInfo.getModem_State();
				/* Event to indicate completion of AT+CEREG? */
				UEvent *evt = new UEvent(UCONN_EVT_GET_RSP); 
				m_pLTE_NM->getNMServer()->MsgPost((U_PUINT8)evt);
				
			}
			break;
		case UCONN_CMD_NET_SIGQUAL_GET:
			{
				m_pLTE_NM->mNMInfo.stats.rssi = 
					bufferParser->parsedInfo.geRSSI();
				m_pLTE_NM->mNMInfo.stats.cinr = 
					bufferParser->parsedInfo.getCINR();
				
			}
			break;
		case UCONN_EVT_READY_FOR_ATTACH:
			{
				//m_pLTE_NM->mNMInfo.bUpdateStatsResponsePending = false;
				printf("<<<<<<<<<<< Posting READY FOR ATTACH <<<<<<<<< \n");
				UReadyforAttach* readyEvt = new UReadyforAttach(1);
				m_pLTE_NM->getNMServer()->MsgPost((U_PUINT8)readyEvt);

			}
			break;
		case UCONN_POWERUP_RSP:
			{
				UPowerupRspEvent* powerupEvt = 
					new UPowerupRspEvent(bufferParser->parsedInfo.getPowerup_state());
				//gConnServer->MsgPost((U_PUINT8)powerupEvt);
				if ( m_pLTE_NM != NULL ) 
				{
					m_pLTE_NM->getNMServer()->MsgPost((U_PUINT8)powerupEvt);
				}
				else if ( m_pThreeG_NM != NULL ) 
				{
					m_pThreeG_NM->getNMServer()->MsgPost((U_PUINT8)powerupEvt);
				}
				return;
			}
			break;
		case UCONN_CMD_LTE_VERSION_INFO_RSP:
			{
				strcpy(m_pLTE_NM->mCommonInfo.versionInfo.u8VersionString, 
				bufferParser->parsedInfo.parsed_VersionInfo.u8VersionString);
				
			}
			break;
		case UCONN_EVT_CONTRDP_RSP:
		{
			UCGCONTRDPEvent* contrdpEvt = new UCGCONTRDPEvent();
			m_pLTE_NM->getNMServer()->MsgPost((U_PUINT8)contrdpEvt);
			break;
		}
		}

	/* Pass it to the client */
	

	
	
}

DAL_STATUS	uconnAtDal::uconnSetATErrorMode()
{
	U_STRING atcmd;
	
	/* AT+CMEE=2 is the command to set AT Error mode to Verbose */
	atcmd.clear();
	atcmd.append("AT+CMEE=2");
	atcmd += CR;
	
	/* Send this atcmd to the device */
	m_pNM->mATHandler->sendATCommand(atcmd.c_str(),atcmd.size());

	return DAL_SUCCESS;
}

DAL_STATUS	uconnAtDal::uconnGetPLMNInfo()
{
	U_STRING atcmd;
	
	/* AT+COPS=3,2 is the command for retrieving the PLMN information */
	atcmd.clear();
	atcmd.append("AT+COPS=3,2");
	atcmd += CR;
	
	/* Send this atcmd to the device */
	m_pNM->mATHandler->sendATCommand(atcmd.c_str(),atcmd.size());

	return DAL_SUCCESS;
}

DAL_STATUS	uconnAtDal::uconnQueryPINStatus()
{
	U_STRING atcmd;
	
	/* AT+CPIN? is the command for getting the PIN status */
	atcmd.clear();
	atcmd.append("AT+CPIN?");
	atcmd += CR;
	
	/* Send this atcmd to the device */
	m_pNM->mATHandler->sendATCommand(atcmd.c_str(),atcmd.size());

	return DAL_SUCCESS;
}




/***********************************************************************
*	uconnATNetAttach -
*	DAL API for the Net Attach operation
*
*	Parameters
*	plmn_select_mode	- Auto/Manual selection mode
*	plmn_id				- PLMN ID to connect to
*	rat					- RAT of the network to connect to
*	primary_ctx_id		- Primary Context ID
***********************************************************************/
DAL_STATUS	uconnAtDal::uconnATNetAttach(U_PLMN_SELECT_MODE plmn_select_mode, 
							  U_PLMN plmn_id, 
							  U_RAT rat, 
							  int primary_ctx_id)
{

	U_STRING atcmd;
#if 0
	/* Set the PLMN selection mode to Auto/Manual */
	atcmd.clear();
	if(plmn_select_mode)
	{
		atcmd.append("AT+COPS=1");
	}
	else
	{
		atcmd.append("AT+COPS=0");
	}
	atcmd += CR;
	/* Send this atcmd to the device */
	

	/* Send the netattach command now */
	atcmd.clear();
	atcmd.append("AT+CGATT=1");
	atcmd += CR;
 
	/* Send this atcmd to the device */
	//Emulator_For_AT_Commands(atcmd);
	//gSerialPortUsg.WriteToSerialPort(atcmd.c_str(),atcmd.size());
#endif

	/* AT+CFUN=1 is the command for netattach */
	atcmd.clear();
	atcmd.append("AT+CFUN=1");
	atcmd += CR;
	
	/* Send this atcmd to the device */
	m_pNM->mATHandler->sendATCommand(atcmd.c_str(),atcmd.size());

	return DAL_SUCCESS;
}

/***********************************************************************
*	uconnATNetDetach -
*	DAL API for the Net Detach operation
*
*	Parameters
***********************************************************************/
DAL_STATUS	uconnAtDal::uconnATNetDetach()
{
	U_STRING atcmd;
	/* Form the AT command string */
	atcmd.clear();
	atcmd.append("AT+CGATT=0");
	atcmd += CR;

	/* Send this AT Command to the device */
	//Emulator_For_AT_Commands(atcmd);
	m_pNM->mATHandler->sendATCommand(atcmd.c_str(),atcmd.size());
	return DAL_SUCCESS;
}

/***********************************************************************
*	uconnATNetCfgSet -
*	DAL API for the Net config set operation
*
*	Parameters
*	cfg_type - The configuration type to be set
*	cfg_value - Configuration value to be set
***********************************************************************/
DAL_STATUS	uconnAtDal::uconnATNetCfgSet(U_UINT32 ctx_id,
							 U_NET_CFG cfg_type,
							 U_UINT32 cfg_value)
{
	U_STRING atcmd;
	U_SINT8 temp_str[10];
	/* Form the AT command string */
	atcmd.clear();
	atcmd.append("AT+CGDCONT=");

	ITOA(ctx_id, &temp_str[0], 10);
	atcmd.append(temp_str);
	atcmd.append(",");

	if(cfg_type == E_PDP_TYPE)
	{
		if(cfg_value == E_PDP_TYPE_IPV4)
		{
			atcmd.append("\"IP\"");
		}
		else if(cfg_value == E_PDP_TYPE_IPV6)
		{
			atcmd.append("\"IPV6\"");
		}
		else if(cfg_value == E_PDP_TYPE_IPV4V6)
		{
			//atcmd.append("\"IPV4V6Virtual\"");
			atcmd.append("\"IPv4v6\"");
		}
		else if(cfg_value == E_PDP_TYPE_IPV4)
		{
			atcmd.append("\"IP\"");
		}
	}
	atcmd += CR;

	/* Send this AT Command to the device */
	//Emulator_For_AT_Commands(atcmd);
	m_pNM->mATHandler->sendATCommand(atcmd.c_str(),atcmd.size());

	return DAL_SUCCESS;
}

/***********************************************************************
*	uconnATPsCtxDefPrimary -
*	DAL API for the Primary Context definition, done before net_attach
***********************************************************************/
DAL_STATUS uconnAtDal::uconnATPsCtxDefPrimary(U_UINT32 ctx_id,
								   U_PS_CTX_PRIMARY_PARAMS* pri_params)
{
	U_STRING atcmd;
	U_SINT8 temp_str[10];
	/* Form the AT command string */
	atcmd.clear();
	atcmd.append("AT+CGDCONT=");

	ITOA(ctx_id, &temp_str[0], 10);
	atcmd.append(temp_str);
	atcmd.append(",");

	if(pri_params->pdp_type == E_PDP_TYPE_IPV4)
	{
		atcmd.append("\"IP\"");
	}
	else if(pri_params->pdp_type == E_PDP_TYPE_IPV6)
	{
		atcmd.append("\"IPV6\"");
	}
	else if(pri_params->pdp_type == E_PDP_TYPE_IPV4V6)
	{
		//atcmd.append("\"IPV4V6Virtual\"");
		atcmd.append("\"IPv4v6\"");
	}
	else if(pri_params->pdp_type == E_PDP_TYPE_IP)
	{
		//atcmd.append("\"IPV4V6Virtual\"");
		atcmd.append("\"IP\"");
	}

	if(strlen(pri_params->apn) >= 1)
	{
		/* APN Name */
		atcmd.append(",");
		atcmd.append("\"");
		atcmd.append(pri_params->apn);
		atcmd.append("\"");
	}

	atcmd += CR;

	/* Send this AT Command to the device */
	//Emulator_For_AT_Commands(atcmd);
	m_pNM->mATHandler->sendATCommand(atcmd.c_str(),atcmd.size());

	return DAL_SUCCESS;
}


/***********************************************************************
*	uconnATNetStatusGet -
*	DAL API for obtaining the Network Registration Status
***********************************************************************/
DAL_STATUS uconnAtDal::uconnATNetStatusGet(U_UINT32 rat)
{
	U_STRING atcmd;
	/* Form the AT command string */
	atcmd.clear();
	atcmd.append("AT+CEREG?");
	atcmd += CR;

	/* Send this AT Command to the device */
	//Emulator_For_AT_Commands(atcmd);
	m_pNM->mATHandler->sendATCommand(atcmd.c_str(),atcmd.size());

	return DAL_SUCCESS;
}

/***********************************************************************
*	uconnATNetStatusSet -
*	DAL API for setting the Network Registration Status
***********************************************************************/
DAL_STATUS uconnAtDal::uconnATNetStatusSet(U_UINT32 nwReg)
{
	U_STRING atcmd;
	U_SINT8 temp_str[10];
	/* Form the AT command string */
	atcmd.clear();
	atcmd.append("AT+CEREG=");
	ITOA(nwReg, &temp_str[0], 10);
	atcmd.append(temp_str);

	atcmd += CR;

	/* Send this AT Command to the device */
	//Emulator_For_AT_Commands(atcmd);
	m_pNM->mATHandler->sendATCommand(atcmd.c_str(),atcmd.size());

	return DAL_SUCCESS;
}


/***********************************************************************
*	uconnATNetSigQualGet -
*	DAL API for getting the Network Signal Quality
***********************************************************************/
DAL_STATUS uconnAtDal::uconnATNetSigQualGet( )
{
	U_STRING atcmd;

	/* Form the AT command string */
	atcmd.clear();
	atcmd.append("AT+CSQ?");
	atcmd += CR;

	/* Send this AT Command to the device */
	//Emulator_For_AT_Commands(atcmd);
	m_pNM->mATHandler->sendATCommand(atcmd.c_str(),atcmd.size());

	return DAL_SUCCESS;
}


/***********************************************************************
*	uconnATPsCtxInfoGet -
*	DAL API for getting the IP Stack information from the LTE Stack
***********************************************************************/
DAL_STATUS uconnAtDal::uconnATPsCtxInfoGet(U_UINT32 ctx_id,
							   U_PS_CTX_INFO_TYPE info_type)
{
	U_STRING atcmd;
	U_SINT8 temp_str[10];
	atcmd.clear();

	if(info_type == E_INFO_PDP_ADDRESS)
	{
		/* Form the AT command string */
		atcmd.append("AT+CGPADDR=");

		/* Append the CTX_INFO Type Command */
		ITOA(ctx_id, &temp_str[0], 10);
		atcmd.append(temp_str);
	}
	else
	{
		/* Form the AT command string */
		atcmd.append("AT+CGCONTRDP=");

		/* Append the CTX_INFO Type Command */
		ITOA(ctx_id, &temp_str[0], 10);
		atcmd.append(temp_str);
	}

	atcmd += CR;

	/* Send this AT Command to the device */
	//Emulator_For_AT_Commands(atcmd);
	m_pNM->mATHandler->sendATCommand(atcmd.c_str(),atcmd.size());

	return DAL_SUCCESS;
}


/***********************************************************************
*	uconnATNetSearch -
*	DAL API for Searching available networks
***********************************************************************/
DAL_STATUS uconnAtDal::uconnATNetSearch()
{
	U_STRING atcmd;

	/* Set the Search mode to Manual */
	atcmd.clear();
	atcmd.append("AT+COPS=1");
	atcmd += CR;
	/* Send this AT Command to the device */

	/* Now perform the Net Search operation */
	atcmd.clear();
	atcmd.append("AT+COPS?");
	atcmd += CR;

	/* Send this AT Command to the device */
	//Emulator_For_AT_Commands(atcmd);
	//m_pNM->mATHandler->sendATCommand(atcmd.c_str(),atcmd.size());

	return DAL_SUCCESS;
}

/***********************************************************************
*	uconnATPsCtxDefSecondary -
*	DAL API for Defining the Secondary context
***********************************************************************/
DAL_STATUS uconnAtDal::uconnATPsCtxDefSecondary(U_UINT32 ctx_id,
									 U_UINT32 pri_ctx_id,
									 U_PS_CTX_SECONDARY_PARAMS* sec_params)
{
	U_STRING atcmd;
	U_SINT8 temp_str[10];
	/* Form the AT command string */
	atcmd.clear();
	atcmd.append("AT+CGDSCONT=");

	/* Context ID */
	ITOA(ctx_id, &temp_str[0], 10);
	atcmd.append(temp_str);
	atcmd.append(",");

	/* Primary Context ID */
	ITOA(pri_ctx_id, &temp_str[0], 10);
	atcmd.append(temp_str);

	atcmd += CR;

	/* Send this AT Command to the device */
	//Emulator_For_AT_Commands(atcmd);
	//m_pNM->mATHandler->sendATCommand(atcmd.c_str(),atcmd.size());

	return DAL_SUCCESS;
}

/***********************************************************************
*	uconnATPsCtxActivate -
*	DAL API to activate an EPS Bearer context
***********************************************************************/
DAL_STATUS uconnAtDal::uconnATPsCtxActivate(U_UINT32 ctx_id)
{
	U_STRING atcmd;
	U_SINT8 temp_str[10];

	/* Enable the Bearer context */
	atcmd.clear();
	atcmd.append("AT+CGACT=1");
	atcmd.append(",");
	
	/* Context ID */
	ITOA(ctx_id, &temp_str[0], 10);
	atcmd.append(temp_str);

	atcmd += CR;

	/* Send this AT Command to the device */
	//Emulator_For_AT_Commands(atcmd);
	//m_pNM->mATHandler->sendATCommand(atcmd.c_str(),atcmd.size());

	return DAL_SUCCESS;
}

/***********************************************************************
*	uconnATPsCtxDeactivate -
*	DAL API to activate an EPS Bearer context
***********************************************************************/
DAL_STATUS uconnAtDal::uconnATPsCtxDeactivate(U_UINT32 ctx_id)
{
	U_STRING atcmd;
	U_SINT8 temp_str[10];

	/* Enable the Bearer context */
	atcmd.clear();
	atcmd.append("AT+CGACT=0");
	atcmd.append(",");
	
	/* Context ID */
	ITOA(ctx_id, &temp_str[0], 10);
	atcmd.append(temp_str);

	atcmd += CR;

	/* Send this AT Command to the device */
	//Emulator_For_AT_Commands(atcmd);
	//m_pNM->mATHandler->sendATCommand(atcmd.c_str(),atcmd.size());

	return DAL_SUCCESS;
}

/***********************************************************************
*	uconnATNetSearchStatus -
*	DAL API to Obtain the Netsearch results
***********************************************************************/
DAL_STATUS uconnAtDal::uconnATNetSearchStatus()
{
	U_STRING atcmd;
	/* Now perform the Net Search operation */
	atcmd.clear();
	atcmd.append("AT+COPS?");
	atcmd += CR;

	/* Send this AT Command to the device */
	//Emulator_For_AT_Commands(atcmd);
	//m_pNM->mATHandler->sendATCommand(atcmd.c_str(),atcmd.size());

	return DAL_SUCCESS;
}

/***********************************************************************
*	uconnGetVersionInfo -
*	DAL API to Obtain the Binary Version information
***********************************************************************/
DAL_STATUS uconnAtDal::uconnGetVersionInfo()
{
	return DAL_SUCCESS;
}

/***********************************************************************
*	uconnPowerUp -
*	DAL API to Obtain the Binary Version information
***********************************************************************/
DAL_STATUS uconnAtDal::uconnPowerUp()
{
	U_STRING atcmd;
	printf("POWER UP DAL called \n");

	/* Now perform the Power Up operation */
	atcmd.clear();
	atcmd.append("AT+CFUN=4");
	atcmd += CR;

	/* Call the UBufferSerialPort function for Sending commands to the device here */
	//bool ret = writeATCommandToSerialPort(atcmd.c_str(), atcmd.size());
	bool ret = m_pNM->mATHandler->sendATCommand(atcmd.c_str(),atcmd.size());

	if(ret == false)
	{
        //printf("AT command Not written to serial Port\n");
	}
    //printf("AT command is written to serial Port\n");

#if 0
	//getch();

	/* Call the second AT command here */
	atcmd.clear();
	atcmd.append("AT+CGDCONT=1,\"IPv4v6\"");
	atcmd += CR;	

	/* Call the UBufferSerialPort function for Sending commands to the device here */
	//ret = writeATCommandToSerialPort(atcmd.c_str(), atcmd.size());
	ret =m_pNM->mATHandler->sendATCommand(atcmd.c_str(),atcmd.size());

	if(ret == false)
	{
		//printf("AT command Not written to serial Port\n");
	}

	//printf("AT command is written to serial Port\n");
	//getch();

	/* Call the Third AT Command here */
	atcmd.clear();
	atcmd.append("AT+CFUN=1");
	atcmd += CR;	

	/* Call the UBufferSerialPort function for Sending commands to the device here */
	//ret = writeATCommandToSerialPort(atcmd.c_str(), atcmd.size());
	ret =m_pNM->mATHandler->sendATCommand(atcmd.c_str(),atcmd.size());

	if(ret == false)
	{
		//printf("AT command Not written to serial Port\n");
	}
	//printf("AT command is written to serial Port\n");
	//getch();

	//getch();
#endif

	return DAL_SUCCESS;

}

/***********************************************************************
*	uconnATEPSRegn -
*	DAL API for the EPS Registration Events
*
*	Parameters
*	RegInfo	- Registration Information
***********************************************************************/
DAL_STATUS	uconnAtDal::uconnATEPSRegn(U_UINT32 RegInfo)
{
	U_STRING atcmd;

	/* AT+CGEREP is the command for EPS Regn events */
	atcmd.clear();
	atcmd.append("AT+CGEREP=2,1");

	/* Registration information required */
#if 0
	ITOA(RegInfo, &temp_str[0], 10);
	atcmd.append(temp_str);
#endif 
	atcmd += CR;
	
	/* Send this atcmd to the device */
	m_pNM->mATHandler->sendATCommand(atcmd.c_str(),atcmd.size());

	return DAL_SUCCESS;
}






/***********************************************************************
*	uconnATNetAttach_DefaultEPS -
*	DAL API for the Net Attach operation (Default EPS bearer)
*
*	Parameters
*	plmn_select_mode	- Auto/Manual selection mode
*	plmn_id				- PLMN ID to connect to
*	rat					- RAT of the network to connect to
*	primary_ctx_id		- Primary Context ID
***********************************************************************/
DAL_STATUS	uconnAtDal::uconnATNetAttach_DefaultEPS(U_PLMN_SELECT_MODE plmn_select_mode, 
							  U_PLMN plmn_id, 
							  U_RAT rat, 
							  int primary_ctx_id)
{
	U_STRING atcmd;

	/* AT+CGACT=1 is the command for netattach */
	atcmd.clear();
	atcmd.append("AT+CGATT=1");
	//atcmd.append(",");

	/* Configuration type */
	//_itoa(primary_ctx_id, &temp_str[0], 10);
	//atcmd.append(temp_str);

	atcmd += CR;
	
	/* Send this atcmd to the device */
	m_pNM->mATHandler->sendATCommand(atcmd.c_str(),atcmd.size());

	return DAL_SUCCESS;
}



//abstraction implementation functions

using namespace UConnConfig;
using namespace UConnSetStatus;
using namespace UConnGetStatus;
using namespace UConnGetStatistics;
using namespace UConnConfigNetParams;
using namespace UConnNetAttach;

DAL_STATUS uconnAtDal::uconnConfigure(CONFIG_CODE configCode) {

	DAL_STATUS result = DAL_ERROR;
	switch(configCode) 
	{
		
		case UConnConfig::VERSION_INFO:
			result = uconnGetVersionInfo();
			break;

		case UConnConfig::POWER_STATUS_QUERY:
			result = uconnPowerStatusQuery();
			break;
		case UConnConfig::SET_CONTEXT:
			{
				
				
				/* Set the Network configuration */
				U_UINT32 data;
				U_UINT32 ctxID = uconn_GetCtxID();
				U_PS_CTX_PRIMARY_PARAMS pri_params;
				if(m_pLTE_NM->mNMInfo.pdp_type == PDP_IPV4)
				{
					/* Set the PDP type that the CM is looking for */
					data = E_PDP_TYPE_IPV4;	
				}
				else if(m_pLTE_NM->mNMInfo.pdp_type == PDP_IPV6)
				{
					/* Set the PDP type that the CM is looking for */
					data = E_PDP_TYPE_IPV6;	
				}
				else if(m_pLTE_NM->mNMInfo.pdp_type == PDP_IPV4V6)
				{
					/* Set the PDP type that the CM is looking for */
					data = E_PDP_TYPE_IPV4V6;	
				}
				else if(m_pLTE_NM->mNMInfo.pdp_type == PDP_IP)
				{
					/* Set the PDP type that the CM is looking for */
					data = E_PDP_TYPE_IP;
					pri_params.pdp_type = E_PDP_TYPE_IP;
				}

				
				pri_params.u32Bitfields = 0x17;
				//pri_params.pdp_type = E_PDP_TYPE_IP;
				if(m_pLTE_NM->mCommonInfo.imsParams.Test_Mode == TEST_MODE_ENABLE)
				{
					strcpy((U_SINT8*)pri_params.apn, m_pLTE_NM->mNMInfo.VzAPN);
				}
				else
				{
					strcpy((U_SINT8*)pri_params.apn, m_pLTE_NM->mNMInfo.VzAPNIMS);
				}
				strcpy((U_SINT8*)pri_params.pdp_addr, "192.168.0.1");
				pri_params.data_comp = E_DATA_COMP_V42;
				pri_params.hdr_comp = E_HDR_COMP_ROHC;

				result = uconnATPsCtxDefPrimary(ctxID, &pri_params);

				if(result) 
				{
					result = ERROR_NET_CFG_SET;
					LOG(ULOG_ERROR_LEVEL, "uconnATNetCfgSet failed \n");
					break;
				}					
			}
			break;
		
	}

	return result;

}

DAL_STATUS uconnAtDal::uconnSetStatus(SET_STATUS setState,void* params,int ctxId)
{
	DAL_STATUS result = DAL_ERROR;

	switch(setState) 
	{
		case SET_NET_STATUS:
			result = uconnATNetStatusSet(CEREG_NW_REG_ENABLE);
			break;
		case POWER_DOWN:
			result = uconnATPowerDown();
			break;
	}

	return result;	
}

DAL_STATUS uconnSendATCommandMode(U_STRING atcmd, UNetworkManager *pNM)
{
#if 1
	gCMInfo.ATCmdMode = 1;
	pNM->mATHandler->sendATCommand(atcmd.c_str(),atcmd.size());
	return DAL_SUCCESS;
#endif
}



DAL_STATUS	uconnAtDal::uconnPowerStatusQuery()
{

	U_STRING atcmd;
	
	/* AT+MCNFG is the command for EPS Regn events */
	atcmd.clear();
	atcmd.append("AT+CFUN?");

	atcmd += CR;
	
	/* Send this atcmd to the device */
	m_pNM->mATHandler->sendATCommand(atcmd.c_str(),atcmd.size());

	return DAL_SUCCESS;
}


DAL_STATUS uconnAtDal::uconnGetStatus(GET_STATUS getState,void* param)
{
		DAL_STATUS result = DAL_ERROR;

	switch(getState) 
	{
		case GET_NET_STATUS:
			result = uconnATNetStatusGet(E_RAT_EUTRAN);
			break;

		case SIG_QUAL:
			{
				result = uconnATNetSigQualGet();
			}
			break;

		case IP_STACK:
			result = uconnATPsCtxInfoGet(1, E_INFO_PDP_ADDRESS);
			result = uconnATPsCtxInfoGet(1, E_INFO_DYN_DNS);
			result = uconnATPsCtxInfoGet(1, E_INFO_GW_ADDRESS);
			result = uconnATPsCtxInfoGet(1, E_INFO_SUBNET_MASK);
			result = uconnATPsCtxInfoGet(1, E_INFO_APN_NAME);
			break;

		case GET_APN_NAME:
			result = uconnATPsCtxInfoGet(1, E_INFO_APN_NAME);
			break;

		case AT_QUERY_CONTEXT:
			result = uconnATQueryContext();
			break;

	}

	return result;



}


DAL_STATUS uconnAtDal::uconnConfigNetParams(NETWORK_PARAMS networkConfig,void* param,int ctxId)
{

  DAL_STATUS result = DAL_ERROR; 	

  switch(networkConfig)
  {
  case PDP_CONFIG:
	  {
		U_UINT32 data;
		U_UINT32 ctxID = 1;
		/* Set the Network configuration */
		
		if(m_pLTE_NM->mNMInfo.pdp_type == PDP_IPV4)
		{
			/* Set the PDP type that the CM is looking for */
			data = E_PDP_TYPE_IPV4;	
		}
		else if(m_pLTE_NM->mNMInfo.pdp_type == PDP_IPV6)
		{
			/* Set the PDP type that the CM is looking for */
			data = E_PDP_TYPE_IPV6;	
		}
		else if(m_pLTE_NM->mNMInfo.pdp_type == PDP_IPV4V6)
		{
			/* Set the PDP type that the CM is looking for */
			data = E_PDP_TYPE_IPV4V6;	
		}

		 result = uconnATNetCfgSet(ctxID, E_PDP_TYPE, data);
		 break;
	  }

  case CTX_ID:
	  {
			uconnATPsCtxActivate(ctxId);
			break;
	  }


  }

		 return result;	


}

DAL_STATUS uconnAtDal::uconnNetSearch()
{

		//U_SINT32* searchTimer = (U_SINT32*)param;

		uconnATNetSearch();

		
		// blank implementation for now.. currently working in auto mode

		return DAL_SUCCESS;

		

}


DAL_STATUS uconnAtDal::uconnNetAttach(ATTACH_MODE attachMode,void* param,int ctxId)
{
	DAL_STATUS result = DAL_ERROR;
	
	U_PLMN plmn = new char[20];

	switch(attachMode) 
	{
		

		case CONNECT_MANUAL:
			
			result = uconnATModemConfig(MCNFG_PDN_CONN_REQUEST,ctxId);
			result = uconnATNetAttach(m_pLTE_NM->mCommonInfo.cfg_params.Config_mode, plmn, E_RAT_EUTRAN, ctxId);
			break;

		case CONNECT_DEFAULT:
			
			result = uconnATNetAttach_DefaultEPS(m_pLTE_NM->mCommonInfo.cfg_params.Config_mode,plmn, E_RAT_EUTRAN,ctxId);
			break;


	}
	delete plmn;

	return result;

}

U_BOOL uconnAtDal::isDevicePresent()
{
	U_BOOL retBool = false;
	//DAL_STATUS ret = 0;
	U_SINT32 InitStatus= ERROR_DEVICE_NOT_OPEN;
	bool isCOMPortAvl = false;

	if(m_pLTE_NM->mCommonInfo.isEmulatorMode)
	{
		isCOMPortAvl = true;
		return 1;
	}

	populateDeviceInfo(m_pLTE_NM->mCommonInfo.dalMode, 
		m_pLTE_NM->mNMInfo.VidPidCount, m_pLTE_NM->mNMInfo.VidPidList, 
		m_pLTE_NM->mNMInfo.APNId, m_pLTE_NM->mNMInfo.Adapter_desc, 
		m_pLTE_NM->mCommonInfo.com_port, m_pLTE_NM->mNMInfo.Adapter_name, 
		m_pLTE_NM->mNMInfo.HW_Addr, m_pLTE_NM->mNMInfo.Adapter_index);
	ComPortFinder(m_pLTE_NM->mCommonInfo.com_port, 
		m_pLTE_NM->mCommonInfo.hwIdCOMPort);

	if(strstr(m_pLTE_NM->mCommonInfo.com_port.c_str(),"COM"))
	{
		LOG(ULOG_DEBUG_LEVEL,"Port no is:%s",m_pLTE_NM->mCommonInfo.com_port.c_str());
		retBool = true;
	}
	else
	{
		retBool = false;
	}


	if(retBool == true)
	{
		m_pLTE_NM->mCommonInfo.device_status = 1;
	}
	else
	{
		m_pLTE_NM->mCommonInfo.device_status = 0;
		m_pLTE_NM->mCommonInfo.com_port.clear();
	}

	return retBool;
}


DAL_STATUS	uconnAtDal::uconnInit(UNetworkManager* pNM)
{
	DAL_STATUS result = uconnATInitDevice(pNM->mCommonInfo.com_port, pNM);

	return result;

}


DAL_STATUS uconnAtDal::uconnNetDetach() 
{
		DAL_STATUS result = uconnATNetDetach();
		return result;

}



DAL_STATUS uconnAtDal::uconnDeInit()
{
	return DAL_SUCCESS;

}


DAL_STATUS uconnAtDal::uconnNetSearchStatus()
{	
	uconnATNetSearchStatus();
	return DAL_SUCCESS;

}

DAL_STATUS uconnAtDal::uconnReset(void* param)
{	
	return DAL_SUCCESS;
}

/***********************************************************************
*	uconnATPowerDown -
*	DAL API for the Power down operation
*
*	Parameters
***********************************************************************/
DAL_STATUS uconnAtDal::uconnATPowerDown()
{
	U_STRING atcmd;
	/* Form the AT command string */
	atcmd.clear();
	atcmd.append("AT+CFUN=0");
	atcmd += CR;

	/* Send this AT Command to the device */
	m_pNM->mATHandler->sendATCommand(atcmd.c_str(),atcmd.size());
	return DAL_SUCCESS;
}

/***********************************************************************
*	uconnATQueryContext -
*	DAL API to query the Contexts used
*
*	Parameters
***********************************************************************/
DAL_STATUS	uconnAtDal::uconnATQueryContext()
{
	U_STRING atcmd;
	/* Form the AT command string */
	atcmd.clear();
	atcmd.append("AT+CGACT?");
	atcmd += CR;

	/* Send this AT Command to the device */
	m_pNM->mATHandler->sendATCommand(atcmd.c_str(),atcmd.size());
	return DAL_SUCCESS;
}


DAL_STATUS uconnAtDal::uconnGetStatistics(GET_STATISTICS stats,void* param)
{

	return DAL_SUCCESS;

}

DAL_STATUS	uconnAtDal::uconnATModemConfig(U_UINT32 mConfig,
						   U_UINT32 ctxId)
{
	return DAL_SUCCESS;
}