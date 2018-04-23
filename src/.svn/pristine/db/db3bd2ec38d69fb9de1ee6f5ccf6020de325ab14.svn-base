/*****************************************************************************
**  uconn_StateMachine.cpp
**  This file describes the methods for the UConnect Server State
**  Machine
**
** Author: Sathyanarayanan S
** Date  : 30-DEC-2010
** (c) United Mobile Apps Pvt Ltd (2010 - 2015)

    VER		  DATE		AUTHOR		DESCRIPTION
	0.1		30/12/10     SS			Creation
****************************************************************************/

#include <string>
#include "UTypes.h"
#include "uconn_OSAL.h"
#include "uconn_StateMachine.h"
#include "uconn_IdleState.h"
#include "uconn_ServerAPI.h"
#include "uconn_IdleEvent.h"
#include "uconn_server.h"

#include "uconn_Broadcom_DAL.h"
#include "uconn_Altair_DAL.h"


#ifdef __linux__
#define stricmp strcasecmp
#include <signal.h>
#endif
/***********************************************************************
*	TimerProc -
*	Timer Callback function
*
*	Parameters
*	hwnd				- Window pointer - NULL
*	msg					- Message ID
*	eventID				- Timer Event ID
*	time				- Time at which event was received
***********************************************************************/
extern U_MUTEX	timerCBLock;

void 
#ifdef WIN32
CALLBACK
#endif
TimerCallBack(U_PVOID timerHead, U_BOOL val)
{
	UTimeOutEvent *evt = new UTimeOutEvent((U_TIMERID)(timerHead));
	 ////cout<< "The Timer head is "<<timerHead<<endl;
	if(gConnServer != NULL)
	{
		gConnServer->MsgPost((U_PUINT8) evt);
	}
			
}

/***********************************************************************
*	TimerCallback_NM -
*	Timer Callback function with NM UConnServer instance
*
*	Parameters
*	hwnd				- Window pointer - NULL
*	msg					- Message ID
*	eventID				- Timer Event ID
*	time				- Time at which event was received
***********************************************************************/
void 
#ifdef WIN32
CALLBACK
#endif
TimerCallBack_NM(U_PVOID timerHead, U_BOOL val)
{
	Timer_struct_s* timer_s = (Timer_struct_s*)timerHead;
	UTimeOutEvent *evt = new UTimeOutEvent((U_TIMERID)(timer_s->mTimer));
	 ////cout<< "The Timer head is "<<timerHead<<endl;
	if(timer_s->mConnServer != NULL)
	{
		timer_s->mConnServer->MsgPost((U_PUINT8) evt);
	}			
}


#if 0
/***********************************************************************
*	InitCallback -
*	Callback function
*
*	Parameters
*	length				- Length of the data received
*	P_ST_CALLBACK_INFO	- Call back information
***********************************************************************/
void InitCallback(string strResp)
{
	UEvent* rspEvt = NULL;
	/* Callback function */
	uLogger.log(ULOG_INFO_LEVEL, "AT DAL Callback function - %s\n", strResp.c_str());

	/* Call the Parser here to determine the Response/Event */
	UBufferParser *ptr = new UBufferParser();
	ptr->startParsingBuffer(strResp);

	/* Create the Event, Post it and then pass it on to the client */
	rspEvt = new UEvent(ptr->getEventID());
	if(rspEvt->getCmdId() == UCONN_EVT_LTE_REG_STATUS)
	{
		rspEvt->setSize(sizeof(UEvent));
		gConnServer->MsgPost((U_PUINT8)rspEvt);
	}
	else if(rspEvt->getCmdId() == UCONN_EVT_PRIMARY_ACT)
	{
		UConnectingEvent* connEvt = new UConnectingEvent(UCONN_EVT_PRIMARY_ACT);
		gConnServer->MsgPost((U_PUINT8)connEvt);
		this->mLTE_NM->mNMInfo.Modem_state = ptr->parsedInfo.getModem_State();
		delete(rspEvt);
	}
	else if(rspEvt->getCmdId() == UCONN_EVT_NET_DETACH)
	{
		/*rspEvt->setSize(sizeof(UEvent));
		gConnServer->MsgPost((U_PUINT8)rspEvt);*/
		UIdleEvent* idleEvt = new UIdleEvent(UCONN_EVT_NET_DETACH, 0);
		gConnServer->MsgPost((U_PUINT8)idleEvt);
		this->mLTE_NM->mNMInfo.Modem_state = ptr->parsedInfo.getModem_State();
		delete(rspEvt);
	}
	else if(rspEvt->getCmdId() == UCONN_CMD_NET_STATUS_GET)
	{
		this->mLTE_NM->mNMInfo.Modem_state = ptr->parsedInfo.getModem_State();
		delete(rspEvt);
	}
	else if(rspEvt->getCmdId() == UCONN_CMD_NET_SIGQUAL_GET)
	{
		this->mLTE_NM->mNMInfo.stats.rssi = ptr->parsedInfo.geRSSI();
		this->mLTE_NM->mNMInfo.stats.ber = ptr->parsedInfo.getCINR();
		delete(rspEvt);
	}
	else if(rspEvt->getCmdId() == UCONN_EVT_LTE_MODEM_STATISTIC)
	{
		if(ptr->parsedInfo.stat.uiGenericStatsLength)
		{
			memcpy(&this->mLTE_NM->mNMInfo.modemStats.sGenericStats, 
				&ptr->parsedInfo.stat.sGenericStats, sizeof(UParsed_CMGenericStats_t));
		}
		else if(ptr->parsedInfo.stat.uiMacstatsLength)
		{
			memcpy(&this->mLTE_NM->mNMInfo.modemStats.sMacStats, 
				&ptr->parsedInfo.stat.sMacStats, sizeof(UParsed_CMMACStats_t));
		}	
		else if(ptr->parsedInfo.stat.uiPHYStatsLength)
		{
			memcpy(&this->mLTE_NM->mNMInfo.modemStats.sPHYStats, 
				&ptr->parsedInfo.stat.sPHYStats, sizeof(UParsed_CMPHYStats_t));
		}			

		delete(rspEvt);
	}
	else
	{
		delete(rspEvt);
		//rspEvt->setSize(sizeof(UEvent));
		/* Post to the State Machine */
	//	gConnServer->MsgPost((U_PUINT8)rspEvt);
	}
	
	/* Pass it to the client */
	

	delete ptr;
}

#endif




#if 0
void InitCallback(U_UINT32 length, P_ST_CALLBACK_INFO callbackbuf)
{
	try {
		UEvent *evt = NULL;

		//printf("Length of TLV Packet - %d\n", length);
		//printf("CMD ID - %x and No. of TLVs - %d\n", callbackbuf->uicmd_id, callbackbuf->uinum_tlvs);
		/* Handle the events from the modem here */
		switch(callbackbuf->uicmd_id)
		{
		case CAPI_CMD_LTE_EVT_RSSI_2G3G:
			break;
		case EVT_RSSI_4G:
			break;
		case CAPI_CMD_LTE_EVT_CS_REG_STATUS:
			break;
		case CAPI_CMD_LTE_EVT_UMTS_REG_STATUS:
			break;
		case CAPI_CMD_LTE_EVT_LTE_REG_STATUS:
		case 0x11000104:
			/* Receive a T_REG_INFO TLV, parse it and work on the SM */
			{
				U_SINT32 numTLVs = callbackbuf->uinum_tlvs;
				U_UINT32 read_size = 0;
				U_UINT32 regInfo;

				LOG(ULOG_INFO_LEVEL, "LTE REG STATUS EVENT received \n");

				if(numTLVs == 1)	/* Only one TLV */
				{
					LOG(ULOG_INFO_LEVEL, "1 TLV found in the Event received \n");
					LOG(ULOG_DEBUG_LEVEL, "TLV Packet length = %d\n", length);
					P_ST_TLV ptemp_TLV = 
						(P_ST_TLV)((U_PCHAR)callbackbuf->pTLVBuf + read_size);
					read_size = ParseTLVData_NetList(ptemp_TLV, 
									&regInfo);
					numTLVs--;
				}
				
				LOG(ULOG_DEBUG_LEVEL, "Registration Information = %d\n", regInfo);
				ULTERegStatusEvent* regEvt = new ULTERegStatusEvent( regInfo);
			//	gConnServer->MsgPost((U_PUINT8) idleEvent);

				/* Call to the Client Callback function */
				evt = (UEvent *) regEvt;// new UEvent(UCONN_EVT_LTE_REG_STATUS);
			}
			break;
		case CAPI_CMD_LTE_EVT_ROAMING_IND:
			/* Receive a T_ROAM_INFO, parse it and work on the SM */
			{
				U_SINT32 numTLVs = callbackbuf->uinum_tlvs;
				U_UINT32 read_size = 0;
				U_UINT32 rat;
				U_UINT32 roam_status;
		
				if(numTLVs == 2) /* Only 2 TLVs */
				{
					P_ST_TLV ptemp_TLV = 
						(P_ST_TLV)((U_PCHAR)callbackbuf->pTLVBuf + read_size);
					read_size = ParseTLVData_NetList(ptemp_TLV, 
									&rat);
					ptemp_TLV = 
						(P_ST_TLV)((U_PCHAR)callbackbuf->pTLVBuf + read_size);
					read_size = ParseTLVData_NetList(ptemp_TLV, 
									&roam_status);
					/* May need to free the pTLVBuf received */
				}
				URoamingInd* rmgEvt = new URoamingInd( roam_status);
				
				//gConnServer->MsgPost((U_PUINT8) idleEvent);

				/* Call to the Client Callback function */
				evt = (UEvent *) rmgEvt;//new UEvent(UCONN_EVT_ROAMING_IND);
			}
			break;
		case CAPI_CMD_LTE_EVT_PRIMARY_ACT:
		case 0x11000106:
			/* This event signals the activation of a Primary PDP Context */
			/* The PDP Address is available with this event */
			{
				//LOG(ULOG_ERROR_LEVEL, "EVT PRIMARY ACT EVENT received \n");
				//LOG(ULOG_ERROR_LEVEL, 
				//	"No. of TLVs = %d \n TLV packet length = %d \n", 
				//	callbackbuf->uinum_tlvs, length);
				U_SINT32 numTLVs = callbackbuf->uinum_tlvs;
				U_UINT32 read_size = 0;
				//U_UINT32 itest = 32;
				UConnectingEvent* connEvt = 
					new UConnectingEvent(UCONN_EVT_PRIMARY_ACT);

				if(numTLVs == 1)
				{
					P_ST_TLV ptemp_TLV = 
						(P_ST_TLV)((U_PCHAR)callbackbuf->pTLVBuf + read_size);
					read_size = ParseTLVData_NetList(ptemp_TLV, 
						&connEvt->pri_evt.ctx_id);
					LOG(ULOG_ERROR_LEVEL, "Context ID received = %d, read_size %d\n",
						connEvt->pri_evt.ctx_id, read_size);
				}

				if(numTLVs == 9)
				{
					//connEvt->pri_evt.ctx_id = 1;
	/*
					connEvt->pri_evt.apn_type = E_PDP_TYPE_IPV6;
					connEvt->pri_evt.apn_id = 100;
					strcpy(connEvt->pri_evt.apn_name, "Live!");
					strcpy(connEvt->pri_evt.pdp_addr, "fe80::abcd:ef12:3456:7890");
					connEvt->pri_evt.bearer_id = 23;
					strcpy(connEvt->pri_evt.subnet_mask, "255.255.255.0");
					strcpy(connEvt->pri_evt.gw_addr, "192.168.1.1");
					strcpy(connEvt->pri_evt.dyndns_server, "fe80::0123:4567:89ab:cdef");
	*/

				
	/*
					P_ST_TLV ptemp_TLV = 
						(P_ST_TLV)((U_PCHAR)callbackbuf->pTLVBuf + read_size);
					read_size = ParseTLVData_NetList(ptemp_TLV, 
						&connEvt->pri_evt.context_id);

					ptemp_TLV = 
						(P_ST_TLV)((U_PCHAR)callbackbuf->pTLVBuf + read_size);
					read_size = ParseTLVData_NetList(ptemp_TLV, 
						&connEvt->pri_evt.apn_type);

					ptemp_TLV = 
						(P_ST_TLV)((U_PCHAR)callbackbuf->pTLVBuf + read_size);
					read_size = ParseTLVData_NetList(ptemp_TLV, 
						&connEvt->pri_evt.apn_id);

					ptemp_TLV = 
						(P_ST_TLV)((U_PCHAR)callbackbuf->pTLVBuf + read_size);
					read_size = ParseTLVData_NetList(ptemp_TLV, 
						connEvt->pri_evt.apn_name);

					ptemp_TLV = 
						(P_ST_TLV)((U_PCHAR)callbackbuf->pTLVBuf + read_size);
					read_size = ParseTLVData_NetList(ptemp_TLV, 
						connEvt->pri_evt.pdp_addr);

					ptemp_TLV = 
						(P_ST_TLV)((U_PCHAR)callbackbuf->pTLVBuf + read_size);
					read_size = ParseTLVData_NetList(ptemp_TLV, 
						&connEvt->pri_evt.bearer_id);

					ptemp_TLV = 
						(P_ST_TLV)((U_PCHAR)callbackbuf->pTLVBuf + read_size);
					read_size = ParseTLVData_NetList(ptemp_TLV, 
						connEvt->pri_evt.subnet_mask);

					ptemp_TLV = 
						(P_ST_TLV)((U_PCHAR)callbackbuf->pTLVBuf + read_size);
					read_size = ParseTLVData_NetList(ptemp_TLV, 
						connEvt->pri_evt.gw_addr);

					ptemp_TLV = 
						(P_ST_TLV)((U_PCHAR)callbackbuf->pTLVBuf + read_size);
					read_size = ParseTLVData_NetList(ptemp_TLV, 
						connEvt->pri_evt.dyndns_server);
	*/
				}
				this->mLTE_NM->mNMInfo.primary_ctx.status = PRIMARY_ACTIVATED;
				
				//gConnServer->MsgPost((U_PUINT8)connEvt);
				
				/* Call to the Client Callback function */
				evt = (UEvent *) connEvt;//new UEvent(UCONN_EVT_PRIMARY_ACT);
			}
			break;
		case CAPI_CMD_LTE_EVT_SECONDARY_ACT:
			/* This event signals the activation of a Secondary PDP Context */
			{
				U_SINT32 numTLVs = callbackbuf->uinum_tlvs;
				U_UINT32 read_size = 0;
				U_UINT32 ctxid;

				if(numTLVs == 1)
				{
					P_ST_TLV ptemp_TLV = 
						(P_ST_TLV)((U_PCHAR)callbackbuf->pTLVBuf + read_size);
					read_size = ParseTLVData_NetList(ptemp_TLV, 
									&ctxid);
				}
				this->mLTE_NM->mNMInfo.secondary_ctx.status = SECONDARY_ACTIVATED;

				USecondaryAct* secEvt = new USecondaryAct(ctxid);
				//gConnServer->MsgPost((U_PUINT8) idleEvent);

				/* Call to the Client Callback function */
				evt = (UEvent *) secEvt;//new UEvent(UCONN_EVT_SECONDARY_ACT);
			}
			break;
		case CAPI_CMD_LTE_EVT_PRIMARY_DEACT:
			/* This event signals the deactivation of a Primary PDP Context */
			{
				U_SINT32 numTLVs = callbackbuf->uinum_tlvs;
				U_UINT32 read_size = 0;
				U_UINT32 ctxid;

				if(numTLVs == 1)
				{
					P_ST_TLV ptemp_TLV = 
						(P_ST_TLV)((U_PCHAR)callbackbuf->pTLVBuf + read_size);
					read_size = ParseTLVData_NetList(ptemp_TLV, 
									&ctxid);
				}
				this->mLTE_NM->mNMInfo.primary_ctx.status = PRIMARY_DEACTIVATED;
				UPrimaryDeAct* deactEvt = new UPrimaryDeAct(ctxid);
				
				//gConnServer->MsgPost((U_PUINT8) idleEvent);

				/* Call to the Client Callback function */
				evt = (UEvent *) deactEvt;// new UEvent(UCONN_EVT_PRIMARY_DEACT);
			}
			break;
		case CAPI_CMD_LTE_EVT_DETACH:
		case 0x1100010b:
			{
				LOG(ULOG_INFO_LEVEL, "LTE EVT DETACH EVENT received \n");
				LOG(ULOG_DEBUG_LEVEL, 
					"No. of TLVs = %d \n TLV packet length = %d \n", 
					callbackbuf->uinum_tlvs, length);
				UNetdetachEvent* netDetachEvt = new UNetdetachEvent();
				//gConnServer->MsgPost((U_PUINT8) idleEvent);

				/* Call back to the State Machine */
				evt = (UEvent *) netDetachEvt;//evt = new UEvent(UCONN_EVT_NET_DETACH);
			}
			break;
		case CAPI_CMD_LTE_EVT_SECONDARY_DEACT:
			/* This event signals the deactivation of a Secondary PDP Context */
			{
				U_SINT32 numTLVs = callbackbuf->uinum_tlvs;
				U_UINT32 read_size = 0;
				U_UINT32 ctxid;

				if(numTLVs == 1)
				{
					P_ST_TLV ptemp_TLV = 
						(P_ST_TLV)((U_PCHAR)callbackbuf->pTLVBuf + read_size);
					read_size = ParseTLVData_NetList(ptemp_TLV, 
									&ctxid);
				}
				this->mLTE_NM->mNMInfo.secondary_ctx.status = SECONDARY_DEACTIVATED;
				USecondaryDeAct* secDeactEvt = new USecondaryDeAct(ctxid);
				//gConnServer->MsgPost((U_PUINT8) idleEvent);

				/* Call to the Client Callback function */
				evt = (UEvent *) secDeactEvt;//evt = new UEvent(UCONN_EVT_SECONDARY_DEACT);
			}
			break;
		case SYSCTL_NOTI_DEVICE_OPEN:
			LOG(ULOG_INFO_LEVEL, "DEVICE OPEN EVENT received \n");
			this->mLTE_NM->mCommonInfo.device_status = 1;
			this->mLTE_NM->mNMInfo.system_notification = NOTI_DEVICE_OPEN;
			//evt  = new USystemEvent(NOTI_DEVICE_OPEN);
			evt = new UDeviceStateChange(NOTI_DEVICE_OPEN);
			break;
		case SYSCTL_NOTI_DEVICE_CONNECTED:
			LOG(ULOG_INFO_LEVEL, "DEVICE CONNECTED EVENT received \n");
			this->mLTE_NM->mCommonInfo.device_status = 1;
			this->mLTE_NM->mNMInfo.system_notification = NOTI_DEVICE_OPEN;
			//evt  = new USystemEvent(NOTI_DEVICE_CONNECTED);	
			evt = new UDeviceStateChange(NOTI_DEVICE_OPEN);
			break;
		case SYSCTL_NOTI_DEVICE_ERROR:
			LOG(ULOG_INFO_LEVEL, "DEVICE ERROR EVENT received \n");
			this->mLTE_NM->mCommonInfo.device_status = 0;
			this->mLTE_NM->mNMInfo.system_notification = NOTI_DEVICE_DISCONNECTED;
			//evt  = new USystemEvent(NOTI_DEVICE_ERROR);
			evt = new UDeviceStateChange(NOTI_DEVICE_NOT_OPEN);
			break;
		case SYSCTL_NOTI_DISCONNECTED:
			LOG(ULOG_INFO_LEVEL, "DISCONNECTED EVENT received \n");
			this->mLTE_NM->mCommonInfo.device_status = 0;
			this->mLTE_NM->mNMInfo.system_notification = NOTI_DEVICE_DISCONNECTED;
			//evt  = new USystemEvent(NOTI_DISCONNECTED);	
			evt = new UDeviceStateChange(NOTI_DEVICE_NOT_OPEN);
			break;		
		case SYSCTL_NOTI_DEVICE_DISCONNECTED:
			LOG(ULOG_INFO_LEVEL, "DEVICE DISCONNECTED EVENT received \n");
			this->mLTE_NM->mCommonInfo.device_status = 0;
			this->mLTE_NM->mNMInfo.system_notification = NOTI_DEVICE_DISCONNECTED;
			//evt  = new USystemEvent(NOTI_DEVICE_DISCONNECTED);	
			evt = new UDeviceStateChange(NOTI_DEVICE_NOT_OPEN);
			break;		
		case SYSCTL_NOTI_DEVICE_NOT_OPEN:
			LOG(ULOG_INFO_LEVEL, "DEVICE NOT OPEN EVENT received \n");
			this->mLTE_NM->mCommonInfo.device_status = 0;
			this->mLTE_NM->mNMInfo.system_notification = NOTI_DEVICE_DISCONNECTED;
			//evt  = new USystemEvent(NOTI_DEVICE_NOT_OPEN);	
			evt = new UDeviceStateChange(NOTI_DEVICE_NOT_OPEN);
			break;		
		case CAPI_CMD_TM_STAT_BASE:
		{
			LOG(ULOG_INFO_LEVEL, "CAPI Statistics command Response ...\n");
			LOG(ULOG_DEBUG_LEVEL, 
				"No. of TLVs = %d \n TLV packet length = %d \n", 
				callbackbuf->uinum_tlvs, length);
			U_SINT32 numTLVs = callbackbuf->uinum_tlvs;
			U_UINT32 read_size = 0;
			U_UINT32 statsid = 0;

			if(numTLVs == 2)
			{
				//TLV 1
				P_ST_TLV ptemp_TLV = 
					(P_ST_TLV)((U_PCHAR)callbackbuf->pTLVBuf + read_size);
				read_size = ParseTLVData_NetList(ptemp_TLV, &statsid);
				LOG(ULOG_DEBUG_LEVEL, "Statistics ID received = %d\n",
					statsid);
				
				//TLV 2
				ptemp_TLV = 
					(P_ST_TLV)((U_PCHAR)callbackbuf->pTLVBuf + read_size);
				read_size = ParseTLVData_NetList(ptemp_TLV, 
					&this->mLTE_NM->mNMInfo.modemStats);
				LOG(ULOG_DEBUG_LEVEL, "Statistics Structure received \n");
			}
		}
			//evt = new UEvent(0);	/* May need to be changed later */
			break;
		case CAPI_CMD_LTE_PS_CTX_INFO_GET:
		{
			U_UINT32 ctxInfo;
			U_SINT8 infoRequested[40];
			LOG(ULOG_INFO_LEVEL, "PS_CTX_INFO_GET command Response ...\n");
			LOG(ULOG_DEBUG_LEVEL, 
				"No. of TLVs = %d \n TLV packet length = %d \n", 
				callbackbuf->uinum_tlvs, length);
			U_SINT32 numTLVs = callbackbuf->uinum_tlvs;
			U_UINT32 read_size = 0;

			//TLV 1
			P_ST_TLV ptemp_TLV = 
				(P_ST_TLV)((U_PCHAR)callbackbuf->pTLVBuf + read_size);
			read_size = ParseTLVData_NetList(ptemp_TLV, &ctxInfo);
			LOG(ULOG_DEBUG_LEVEL, "TLV #1 received = %d\n",
				ctxInfo);

			//TLV 2
			ptemp_TLV = 
				(P_ST_TLV)((U_PCHAR)callbackbuf->pTLVBuf + read_size);
			read_size = ParseTLVData_NetList(ptemp_TLV, infoRequested);
			infoRequested[ptemp_TLV->uiLength] = '\0';
			LOG(ULOG_DEBUG_LEVEL, "TLV #2 received = %s, TLV #2 Length = %d\n",
				infoRequested, ptemp_TLV->uiLength);

			LOG(ULOG_DEBUG_LEVEL, "Length of INFO Received - %d\n", 
				strlen(infoRequested));

			switch(ctxInfo)
			{
			case 256:
				LOG(ULOG_DEBUG_LEVEL, "PDP ADDRESS = %s\n", infoRequested);
				strcpy(this->mLTE_NM->mNMInfo.ip_address, infoRequested);
				break;
			case 4096:
				LOG(ULOG_DEBUG_LEVEL, "GW ADDRESS = %s\n", infoRequested);
				strcpy(this->mLTE_NM->mNMInfo.gw_address, infoRequested);
				break;
			case 64:
				LOG(ULOG_DEBUG_LEVEL, "DYN DNS SERVER = %s\n", infoRequested);
				strcpy(this->mLTE_NM->mNMInfo.dyn_dns, infoRequested);
				break;
			case 512:
				LOG(ULOG_DEBUG_LEVEL, "APN NAME = %s\n", infoRequested);
				strcpy(this->mLTE_NM->mNMInfo.apn_name, infoRequested);
				break;
			case 2048:
				LOG(ULOG_DEBUG_LEVEL, "SUBNET MASK = %s\n", infoRequested);
				strcpy(this->mLTE_NM->mNMInfo.subnet_mask, infoRequested);
				break;
			default:
				break;
			}

		}
		//	evt = new UEvent(0);	/* May need to be changed later */
			break;

		case CAPI_CMD_LTE_PS_CTX_ACTIVATE:
		{
			U_UINT32 ctxInfo;
			LOG(ULOG_INFO_LEVEL, "PS_CTX_ACTIVATE command Response ...\n");
			LOG(ULOG_DEBUG_LEVEL, 
				"No. of TLVs = %d \n TLV packet length = %d \n", 
				callbackbuf->uinum_tlvs, length);
			U_SINT32 numTLVs = callbackbuf->uinum_tlvs;
			U_UINT32 read_size = 0;

				//TLV 1
				P_ST_TLV ptemp_TLV = 
					(P_ST_TLV)((U_PCHAR)callbackbuf->pTLVBuf + read_size);
				read_size = ParseTLVData_NetList(ptemp_TLV, &ctxInfo);
				LOG(ULOG_DEBUG_LEVEL, "TLV #1 received = %d\n",
					ctxInfo);
		}
			//evt = new UEvent(0);	/* May need to be changed later */
			break;
		case CAPI_CMD_LTE_NET_STATUS_GET:
			{
				U_UINT32 read_size = 0;
				U_UINT32 netStatus;
				LOG(ULOG_DEBUG_LEVEL, "CAPI_CMD_LTE_NET_STATUS_GET Response ...\n");
				LOG(ULOG_DEBUG_LEVEL, 
					"No. of TLVs = %d \n TLV packet length = %d \n", 
					callbackbuf->uinum_tlvs, length);	

				//TLV 1
				P_ST_TLV ptemp_TLV = 
					(P_ST_TLV)((U_PCHAR)callbackbuf->pTLVBuf + read_size);
				read_size = ParseTLVData_NetList(ptemp_TLV, &netStatus);
				LOG(ULOG_DEBUG_LEVEL, "TLV #1 received = %d\n",
					netStatus);
				this->mLTE_NM->mNMInfo.Modem_state = netStatus;
			}
			evt = new UEvent(UCONN_EVT_GET_RSP);	/* May need to be changed later */
			break;
		case CAPI_CMD_LTE_NET_SIG_QUAL_GET:
			{
				U_UINT32 read_size = 0;
				U_UINT32 sigQual;

				LOG(ULOG_DEBUG_LEVEL, "CAPI_CMD_LTE_NET_SIG_QUAL_GET Response ...\n");
				LOG(ULOG_DEBUG_LEVEL, 
					"No. of TLVs = %d \n TLV packet length = %d \n", 
					callbackbuf->uinum_tlvs, length);	

				//TLV 1
				P_ST_TLV ptemp_TLV = 
					(P_ST_TLV)((U_PCHAR)callbackbuf->pTLVBuf + read_size);
				read_size = ParseTLVData_NetList(ptemp_TLV, &sigQual);
				LOG(ULOG_DEBUG_LEVEL, "TLV #1 received RSSI = %d\n",
					sigQual);
				this->mLTE_NM->mNMInfo.stats.rssi = sigQual;

				//TLV 2
				ptemp_TLV = 
					(P_ST_TLV)((U_PCHAR)callbackbuf->pTLVBuf + read_size);
				read_size = ParseTLVData_NetList(ptemp_TLV, &sigQual);
				LOG(ULOG_DEBUG_LEVEL, "TLV #2 received BER = %d\n",
					sigQual);
				this->mLTE_NM->mNMInfo.stats.ber = sigQual;
			}
			//evt = new UEvent(0);	/* May need to be changed later */
			break;
		case CAPI_CMD_LTE_APN_TABLE_SET:
			{
				LOG(ULOG_DEBUG_LEVEL, "CAPI_CMD_LTE_APN_TABLE_SET Response ...\n");
				this->mLTE_NM->mNMInfo.ipConfigUP = 1;
			}
		//	evt = new UEvent(0);	/* May need to be changed later */
			break;
		case SYSCTL_CMD_GET_VERSION_INFO:
			{
				LOG(ULOG_DEBUG_LEVEL, "SYSCTL_CMD_GET_VERSION_INFO Response ...\n");
				LOG(ULOG_DEBUG_LEVEL, 
					"No. of TLVs = %d \n TLV packet length = %d \n", 
					callbackbuf->uinum_tlvs, length);
				U_SINT32 numTLVs = callbackbuf->uinum_tlvs;
				U_UINT32 read_size = 0;

				if(numTLVs == 1)
				{
					//TLV 1
					P_ST_TLV ptemp_TLV = 
						(P_ST_TLV)((U_PCHAR)callbackbuf->pTLVBuf + read_size);
					read_size = ParseTLVData_NetList(ptemp_TLV, &this->mLTE_NM->mNMInfo.versionInfo);
					LOG(ULOG_DEBUG_LEVEL, 
						"VERSION Information received size = %d\n", 
						ptemp_TLV->uiLength);
				}
				//evt = new UEvent(0);
			}
			break;

		default:
			LOG(ULOG_ERROR_LEVEL, "Unknown EVENT \n");
			LOG(ULOG_ERROR_LEVEL, "EVENT ID = %x\n", callbackbuf->uicmd_id);		
		//	evt = new UEvent(0);	/* May need to be changed later */
		}
		
		if(evt != NULL){
			/* Client Callback function */
			/*if(evt->getCmdId() == UCONN_EVT_SYSTEM_EVENT)
			{
				evt->setSize(sizeof(USystemEvent));
			}*/
			
			gConnServer->MsgPost((U_PUINT8)evt);
		}

		//if(((U_SINT32)evt->execStatus >= 0) && 
		//	 ((U_SINT32)evt->getCmdId() < UCONN_LAST_CMD_ID) &&
		//	 ((U_SINT32)evt->getSize() > 0) && ((U_SINT32)evt->getCmdId() > 0))
		//{
		//	SAPI_eventCB(evt);
		//}
		
	}		
	catch(bad_alloc& ba) {
		printf("Out of Memory!\n");
	}
	catch(exception& e) {
		printf("Unknown Exception received\n");
	}
}
#endif /* if 0 */

/***********************************************************************
*	UCMStateMachine::UCMStateMachine() -
*	CM State Machine constructor.  All States are malloced here
***********************************************************************/

UCMStateMachine::UCMStateMachine():UStateMachine()
{
#if 0
	initParams.eOperatingMode = eBcmLocalMode;
        #if (!defined(QTPROJECT) && defined(WIN32))
        ::strcpy_s(initParams.szIpAddress,128, "127.0.0.1");
        #else
        strcpy(initParams.szIpAddress,"127.0.0.1");
        #endif
	initParams.u32Port = 228877;
#endif 

	/* Parent States initialization */
	this->enabledState = new UCMEnabledState((UStateMachine*)this);
	this->disabledState = new UCMDisabledState((UCMStateMachine*)this);

	/* States derived from Enabled State */
	this->idleState = new UCMIdleState((UStateMachine*)this);
	this->searchingState = new UCMSearchingState((UStateMachine*)this);
	this->connectingState = new UCMConnectingState((UStateMachine*)this);
	this->connectedState = new UCMConnectedState((UStateMachine*)this);
	this->disconnectingState = new UCMDisconnectingState((UCMStateMachine*)this);

	/* States derived from Connecting State */
    this->wait_for_regstatusAct = new UCMWaitRegStatusAct((UStateMachine*)this);
	this->wait_for_primaryAct = new UCMWaitPrimaryAct((UStateMachine*)this);
	//this->primaryState = new UCMDefPrimaryState((UStateMachine*)this);
	//this->secondaryState = new UCMDefSecondaryState((UStateMachine*)this);
	//this->netattachState = new UCMNetAttachState((UStateMachine*)this);
    this->setState(this->disabledState);

	this->apn_table = NULL;
    

	/* Power up set to false initially */
	this->bPowerUpDone = false;

	this->updateTimer = NULL;
	this->searchTimer = NULL;
	this->deviceStateCheckTimer = NULL;
	this->stateTimer = NULL;
	this->mLTE_NM = NULL;

	this->reset();
}

#if 0
/***********************************************************************
*	UCMStateMachine::UCMStateMachine(UStateMachine *pSM) -
*	CM State Machine constructor.  All States are malloced here
***********************************************************************/

UCMStateMachine::UCMStateMachine(UStateMachine *pSM):UStateMachine(pSM)
{
	initParams.eOperatingMode = eBcmLocalMode;
        #ifndef QTPROJECT
        ::strcpy_s(initParams.szIpAddress,128, "127.0.0.1");
        #else
        strcpy(initParams.szIpAddress,"127.0.0.1");
        #endif
	initParams.u32Port = 228877;

	/* Parent States initialization */
	this->enabledState = new UCMEnabledState((UStateMachine*)this);
	this->disabledState = new UCMDisabledState((UCMStateMachine*)this);

	/* States derived from Enabled State */
	this->idleState = new UCMIdleState((UStateMachine*)this);
	this->searchingState = new UCMSearchingState((UStateMachine*)this);
	this->connectingState = new UCMConnectingState((UStateMachine*)this);
	this->connectedState = new UCMConnectedState((UStateMachine*)this);
	this->disconnectingState = new UCMDisconnectingState((UCMStateMachine*)this);

	/* States derived from Connecting State */
	this->primaryState = new UCMDefPrimaryState((UStateMachine*)this);
	this->secondaryState = new UCMDefSecondaryState((UStateMachine*)this);
	this->netattachState = new UCMNetAttachState((UStateMachine*)this);

	/* Timer initialization */
	this->updateTimer = NULL;
	this->stateTimer = NULL;

	/* Statistics object initialization */
	stats = new CM_Statistics_s();

	/* APN Table Request Initialization */
	apn_table = new T_APN_TABLE();
}
#endif

void UCMStateMachine::resetCMInfo()
{
    // Reset all CM Info variables here
	memset(&this->mLTE_NM->mNMInfo.modemStats, 0, sizeof(Modem_stats_s));
	this->mLTE_NM->mNMInfo.m_nBroadcomStoredStats.u32DLBw = 6;
	this->mLTE_NM->mNMInfo.m_nBroadcomStoredStats.u32ULBw = 6;
	this->mLTE_NM->mNMInfo.stats.rssi = 121;
	this->mLTE_NM->mNMInfo.stats.cinr = 0;
	strcpy(this->mLTE_NM->mNMInfo.HW_Addr, "");
	strcpy(this->mLTE_NM->mNMInfo.apn_name, "");
	strcpy(this->mLTE_NM->mNMInfo.Adapter_index, "");
	strcpy(this->mLTE_NM->mNMInfo.Adapter_name, "");
	strcpy(this->mLTE_NM->mNMInfo.ip_address, "");
	strcpy(this->mLTE_NM->mNMInfo.ipv6_address, "");
	this->mLTE_NM->mNMInfo.ATCmdMode = 0;
	strcpy(this->mLTE_NM->mCommonInfo.versionInfo.u8VersionString, "");
	this->RS_attempts = 0;
	this->IPv6_Throttle_count = 0;
	this->PDN_Reject_Throttle_count = 0;
	this->isThrottleTimerRunning = false;
	this->mLTE_NM->mCommonInfo.imsParams.IMS_Reg_Status = 0;
}
void UCMStateMachine::reset()
{
	static U_UINT32 timer_calls = 0;
	//printf("RESET CALLED = %d\n", ++timer_calls);

	if(!this->mLTE_NM)
		return;
	//initParams.u32Port = 228877;
	/* Timer initialization */
	if(this->updateTimer != NULL)
	{
		UKillTimer(this->updateTimer);
		this->updateTimer = NULL;
	}

    this->bSyncedWithDevice = false;

    this->mLTE_NM->mNMInfo.Modem_state = 0;

	if(apn_table)
	{
		delete apn_table;
		apn_table = new U_APN_TABLE();
	}
	else
	{
		/* APN Table Request Initialization */
		apn_table = new U_APN_TABLE();
	}
	
	resetCMInfo();
	this->mLTE_NM->mNMInfo.lte_reg_status = 0;
	this->context_id = 1;

	/* Power up set to false initially */
	this->bPowerUpDone = false;
	this->mLTE_NM->mNMInfo.bUpdateStatsResponsePending = false;
	if(this->mLTE_NM->mCommonInfo.imsParams.Test_Mode == TEST_MODE_ENABLE)
	{
		this->isIMSRegnDone = true;
	}
	else
	{
		this->isIMSRegnDone = false;
	}

	this->updateTimer = NULL;
	this->stateTimer = NULL;
	this->searchTimer = NULL;
	this->deviceStateCheckTimer = NULL;

}


/***********************************************************************
*	UCMStateMachine::UCMStartUp() -
*	CM State Machine startup routine
***********************************************************************/
void UCMStateMachine::UCMStartUp(UNetworkManager* pNM){

	
	U_SINT32 InitStatus = ERROR_DEVICE_NOT_OPEN;
	bool isCOMPortAvl = false;

	LOG(ULOG_INFO_LEVEL, "About to create DAL_Object \n");

	if ( pNM->getTechID() == LTE_TECH ) 
	{
		mLTE_NM = reinterpret_cast<ULTE_NM*>(pNM);
		mThreeG_NM = NULL ;
	}
	else if (pNM->getTechID() == THREE_G_TECH ) 
	{
		mThreeG_NM = reinterpret_cast<UThreeG_NM*>(pNM);
		mLTE_NM = NULL ;
	}
#if 0
	if(this->mLTE_NM->mCommonInfo.dalMode == 1)
	{
		dal = reinterpret_cast<uconnDal*>(new uconnBroadcomDal());
	}
	else if(this->mLTE_NM->mCommonInfo.dalMode == 2)
	{
		dal = reinterpret_cast<uconnDal*>(new uconnAltairDal());
	}
#endif /* if 0 */
}

/***********************************************************************
*	UCMStateMachine::setCommandId -
*	Set the Command ID
*
*	Parameters
*	cId		- Command ID to be set
***********************************************************************/

void UCMStateMachine::setCommandId(U_UINT32 cId)
{
	commandId = cId;
}

/***********************************************************************
*	UCMStateMachine::getCommandId() -
*	Retrieve the Command ID in the State Machine
***********************************************************************/

U_UINT32 UCMStateMachine::getCommandId()
{
	return commandId;
}

/***********************************************************************
*	UCMStateMachine::throttle_counter_update -
*	Updates the Throttle counter and the delay
***********************************************************************/
void UCMStateMachine::throttle_counter_update()
{
	U_UINT32 throttle_counter = max(IPv6_Throttle_count, 
		PDN_Reject_Throttle_count);
	U_UINT32 throttle_delay = 0;

	/* Set the Throttle Delay based on the Throttle counter */
	switch(throttle_counter)
	{
		case 0:
		case 1:
		case 2:
			throttle_delay = 0;
			break;
		case 3:
			throttle_delay = ((1 * 60) + max((rand() % 60), 15)) * 1000;
			break;
		case 4:
			throttle_delay = 2 * 60 * 1000;
			break;
		case 5:
			throttle_delay = 8 * 60 * 1000;
			break;
		case 6:
		default:
			throttle_delay = 15 * 60 * 1000;
			break;
	}

	/* Start the throttle timer now and disallow Connect till it expires */
	if((throttle_delay) && (throttleTimer == NULL))
	{
		throttleTimer = USetTimer(throttle_delay, 0, (U_CALLBACK)TimerCallBack);
		isThrottleTimerRunning = true;
	}
}

/***********************************************************************
*	UCMStateMachine::~UCMStateMachine() -
*	CM State Machine destructor.  All States are freed here
***********************************************************************/

UCMStateMachine::~UCMStateMachine()
{
	delete idleState;
	delete searchingState;
	delete connectingState;
	delete connectedState;
	delete disconnectingState;
	//delete primaryState;
	//delete secondaryState;
	//delete netattachState;
	delete enabledState;
	delete disabledState;
	delete wait_for_primaryAct;
	delete wait_for_regstatusAct;

	/* Statistics object deletion */
	delete stats;

	/* APN Table Entry Request Deletion */
	delete apn_table;
}
