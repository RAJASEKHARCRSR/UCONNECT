/*****************************************************************************
**
** uconn_ConnectedState.cpp
** 	This file describes the methods for the UConnect Connected
**	State
**
** Author: Sathyanarayanan S
** Date  : 04-JAN-2011
** (c) United Mobile Apps Pvt Ltd (2010 - 2015)

    VER		  DATE		AUTHOR		DESCRIPTION
	0.1		04/01/11     SS			Creation
****************************************************************************/

#include "uconn_StateMachine.h"
#include "uconn_ConnectedState.h"
#include "uconn_ServerAPI.h"
#include "uconn_ConnectedEvent.h"
#include "uconn_AT_DAL.h"
//#include "uconn_bcm_DAL.h"

/***********************************************************************
*	UCMConnectedState::entry() -
*	Connected State Entry point
***********************************************************************/

void UCMConnectedState::entry()
{
	//UServerStateEvent *connectedStateEvt_p = new UServerStateEvent(CONNECTED_STATE);
    UCMStateMachine* ucmsm_p = (UCMStateMachine*)this->getStateMachine();
	
	
	strcpy(ucmsm_p->mLTE_NM->mNMInfo.HW_Addr, "");
	populateDeviceInfo(ucmsm_p->mLTE_NM->mCommonInfo.dalMode, 
	ucmsm_p->mLTE_NM->mNMInfo.VidPidCount, ucmsm_p->mLTE_NM->mNMInfo.VidPidList, 
	ucmsm_p->mLTE_NM->mNMInfo.APNId, ucmsm_p->mLTE_NM->mNMInfo.Adapter_desc, 
	ucmsm_p->mLTE_NM->mCommonInfo.com_port, ucmsm_p->mLTE_NM->mNMInfo.Adapter_name, 
	ucmsm_p->mLTE_NM->mNMInfo.HW_Addr, ucmsm_p->mLTE_NM->mNMInfo.Adapter_index);

	ucmsm_p->getDal()->uconnConfigure(UConnConfig::VERSION_INFO);
	
	/* PDN Rejection counter to be set to Zero here */
	ucmsm_p->PDN_Reject_Throttle_count = 0;

	if(ucmsm_p->stateTimer)
	{
		/* Kill the State Timeout Timer before going to the next State */
		UKillTimer(ucmsm_p->stateTimer);
		ucmsm_p->stateTimer = NULL;
	}
	
	U_UINT32 result = UCM_SUCCESS;

	LOG(ULOG_ERROR_LEVEL, "CONNECTED State entry\n");
	ucmsm_p->mLTE_NM->mNMInfo.state = CONNECTED_STATE;

	/* Call the DAL APIs to retrieve the IP Stack Information */
	/*result = uconnATPsCtxInfoGet(1, E_INFO_PDP_ADDRESS);
	result = uconnATPsCtxInfoGet(1, E_INFO_DYN_DNS);
	result = uconnATPsCtxInfoGet(1, E_INFO_GW_ADDRESS);
	result = uconnATPsCtxInfoGet(1, E_INFO_SUBNET_MASK);*/
	result = ucmsm_p->getDal()->uconnGetStatus(GET_APN_NAME,NULL);

}

/***********************************************************************
*	UCMConnectedState::handle -
*	Connected State handler
*
*	Parameters
*	evt	- Pointer to the Event type
***********************************************************************/

U_UINT32 UCMConnectedState::handle(UEvent *evt)
{
	U_UINT8 iResult = UCM_SUCCESS;
	//U_UINT32 iResult;
	UCMStateMachine* ucmsm_p = (UCMStateMachine*)this->getStateMachine();
	/* We enter here only when a Disconnect command is received */
	LOG(ULOG_ERROR_LEVEL, "CONNECTED State Handle  %d\n",evt->getCmdId());
	switch(evt->getCmdId())
	{
		case UCONN_CMD_LTE_NET_DETACH:
		{
            if(ucmsm_p->mLTE_NM->mCommonInfo.imsParams.Test_Mode == TEST_MODE_ENABLE)
            {
			    ucmsm_p->transit(ucmsm_p->disconnectingState);
				if(ucmsm_p->mLTE_NM->mNMInfo.isPowerdown)
				{
			       	// asper at_command
					iResult = ucmsm_p->getDal()->uconnSetStatus(UConnSetStatus::POWER_DOWN,NULL);
			    
				}
				else
				{
					iResult = ucmsm_p->getDal()->uconnNetDetach();
					
				}

			    //ucmsm_p->getState()->handle(evt);
            }
            else /* If Test Mode is Disabled */
            {
				iResult = ucmsm_p->getDal()->uconnConfigure(IMS_CONFIG_DETACH);
            }
		}
		break;
        case UCONN_EVT_IMS_DEREG:
        {
            LOG(ULOG_ERROR_LEVEL, "IMS Deregistration Succeeded\n");
		    ucmsm_p->transit(ucmsm_p->disconnectingState);
		    /* DAL for the net_detach to be called here */
		    iResult = ucmsm_p->getDal()->uconnNetDetach();
        }
        break;
        case UCONN_EVT_REQ_DETACH:
        {
            LOG(ULOG_ERROR_LEVEL, "REQUEST DETACH IMS Event received\n");
		    ucmsm_p->transit(ucmsm_p->disconnectingState);
		    /* DAL for the net_detach to be called here */
		     iResult = ucmsm_p->getDal()->uconnNetDetach();
        }
        break;
		case UCONN_EVT_CONTRDP_RSP:
		{
			U_UINT32 result = UCM_SUCCESS;
			U_SINT8* intface = ucmsm_p->intface;
			U_BOOL isIPv6 = false;

			if(ucmsm_p->ipv6_support)
				isIPv6 = isIpV6supported();    /* Check for IPv6 on the adapter */

			/* IP Configuration to be done in this handle */
			LOG(ULOG_INFO_LEVEL, "CONNECTED State Handle: IP Configuration...\n");

			/* Call the IPv4/v6 configuration APIs */
			/* strcpy(intface, "Local Area Connection 6"); */
			if(IsWinXP())
			{
				//strcpy(intface, gCMInfo.interface_name);
				strcpy(intface, ucmsm_p->mLTE_NM->mNMInfo.Adapter_name);	/* Name for Win XP */
			}
			else
			{
				strcpy(intface, ucmsm_p->mLTE_NM->mNMInfo.Adapter_index); /* Index for Win Vista/7 */
			}


			LOG(ULOG_INFO_LEVEL, "AT+CGCONTRDP Response received \n");

			LOG(ULOG_DEBUG_LEVEL, "IP Address Received = %s\n", ucmsm_p->mLTE_NM->mNMInfo.ip_address);
			LOG(ULOG_DEBUG_LEVEL, "GW Address Received = %s\n", ucmsm_p->mLTE_NM->mNMInfo.gw_address);
			LOG(ULOG_DEBUG_LEVEL, "Subnet Mask Received = %s\n", ucmsm_p->mLTE_NM->mNMInfo.subnet_mask);
			LOG(ULOG_DEBUG_LEVEL, "DYN DNS Received = %s\n", ucmsm_p->mLTE_NM->mNMInfo.dyn_dns);
			LOG(ULOG_DEBUG_LEVEL, "APN Name Received = %s\n", ucmsm_p->mLTE_NM->mNMInfo.apn_name);

			LOG(ULOG_DEBUG_LEVEL, "Length of IP Address = %d\n", 
				strlen(ucmsm_p->mLTE_NM->mNMInfo.ip_address));
			LOG(ULOG_DEBUG_LEVEL, "Length of GW Address = %d\n", 
				strlen(ucmsm_p->mLTE_NM->mNMInfo.gw_address));
			LOG(ULOG_DEBUG_LEVEL, "Length of Subnet Mask = %d\n", 
				strlen(ucmsm_p->mLTE_NM->mNMInfo.subnet_mask));
			LOG(ULOG_DEBUG_LEVEL, "Length of DYN DNS = %d\n", 
				strlen(ucmsm_p->mLTE_NM->mNMInfo.dyn_dns));
			LOG(ULOG_DEBUG_LEVEL, "Length of APN Name = %d\n", 
				strlen(ucmsm_p->mLTE_NM->mNMInfo.apn_name));

			/*strcpy(ucmsm_p->ip_addr, ucmsm_p->mLTE_NM->mNMInfo.ip_address);
			strcpy(ucmsm_p->dyndns_server, gCMInfo.dyn_dns);
			strcpy(ucmsm_p->gw_addr, gCMInfo.gw_address);*/

			strcpy(ucmsm_p->ip_addr, ucmsm_p->mLTE_NM->mNMInfo.ip_address);
			strcpy(ucmsm_p->dyndns_server, ucmsm_p->mLTE_NM->mNMInfo.dyn_dns);
			strcpy(ucmsm_p->gw_addr, ucmsm_p->mLTE_NM->mNMInfo.gw_address);
			strcpy(ucmsm_p->subnet_mask, ucmsm_p->mLTE_NM->mNMInfo.subnet_mask);

			/* At this point, we need to call the PROP_APN_TABLE_SET */
			/* TESTING SETUP with the Anite DT */
			ucmsm_p->apn_table->PLMN_count = 1;
			ucmsm_p->apn_table->plmn_search_entry[0].u32Bitfields = 255;
			ucmsm_p->apn_table->plmn_search_entry[0].apn_type = ucmsm_p->mLTE_NM->mNMInfo.pdp_type; //E_PDP_TYPE_IPV4;
			ucmsm_p->apn_table->plmn_search_entry[0].cid = ucmsm_p->context_id; 
			ucmsm_p->apn_table->plmn_search_entry[0].APN_id = ucmsm_p->mLTE_NM->mNMInfo.APNId;
			strcpy((char*)ucmsm_p->apn_table->plmn_search_entry[0].apn, 
				ucmsm_p->mLTE_NM->mNMInfo.apn_name);
			strcpy((char*)ucmsm_p->apn_table->plmn_search_entry[0].network_interface, 
				ucmsm_p->mLTE_NM->mNMInfo.HW_Addr);
			strcpy((char*)ucmsm_p->apn_table->plmn_search_entry[0].source_addr, 
				ucmsm_p->mLTE_NM->mNMInfo.ip_address);
			strcpy((char*)ucmsm_p->apn_table->plmn_search_entry[0].usb_endpoint, "");

			/* APN name not being sent properly, so at*apn would fail, forcing it to apn1 */
			strcpy((char*)ucmsm_p->apn_table->plmn_search_entry[0].apn, 
			"apn1");
			ucmsm_p->apn_table->plmn_search_entry[0].inactivity_timer = ucmsm_p->mLTE_NM->mNMInfo.APN_Inactivity_timer;

			//if(strlen(gCMInfo.Adapter_name))
			{
				/* Prints for the APN Table Set params */
				printf("APN Type = %d\n", ucmsm_p->apn_table->plmn_search_entry[0].apn_type);
				printf("Context ID = %d\n", ucmsm_p->apn_table->plmn_search_entry[0].cid);
				printf("APN ID = %d\n", ucmsm_p->apn_table->plmn_search_entry[0].APN_id);
				printf("APN Name = %s\n", ucmsm_p->apn_table->plmn_search_entry[0].apn);
				printf("MAC Address = %s\n", ucmsm_p->apn_table->plmn_search_entry[0].network_interface);
				printf("IP address = %s\n", ucmsm_p->apn_table->plmn_search_entry[0].source_addr);
				printf("USB Endpoint = %s\n", ucmsm_p->apn_table->plmn_search_entry[0].usb_endpoint);
				printf("Inactivity Timer = %d\n", ucmsm_p->apn_table->plmn_search_entry[0].inactivity_timer);
				//printf("PROP APN TABLE SET CALLED \n");

				if(((ucmsm_p->mLTE_NM->mCommonInfo.imsParams.IMS_Attach_Only == 0) &&
					(ucmsm_p->mLTE_NM->mCommonInfo.imsParams.Test_Mode == TEST_MODE_DISABLE)) ||
					(ucmsm_p->mLTE_NM->mCommonInfo.imsParams.Test_Mode == TEST_MODE_ENABLE))
				{
					result = ucmsm_p->getDal()->uconnConfigNetParams(APN_CONFIG,(void*)ucmsm_p);
				}
			}

			if(result)
			{
				result = ERROR_APN_TABLE_SET;
				//return;
			}
			else
			{
				/* Call Prop_IP_IF_Status and the Set the status UP */
				//if(gCMInfo.ipConfigUP)
				{
					if(((ucmsm_p->mLTE_NM->mCommonInfo.imsParams.IMS_Attach_Only == 0) &&
					(ucmsm_p->mLTE_NM->mCommonInfo.imsParams.Test_Mode == TEST_MODE_DISABLE)) ||
					(ucmsm_p->mLTE_NM->mCommonInfo.imsParams.Test_Mode == TEST_MODE_ENABLE))
					{
						result = ucmsm_p->getDal()->uconnSetStatus(IP_IF_STATUS_UP,NULL,ucmsm_p->context_id);//asper at_command null is con text_id
						if(result)
						{
							result = ERROR_IP_IF_STATUS;
							//return;
						}
					}
				}
			}

			/* Check for valid IPv4 address in the IPv4 Ctx config, 
			if not valid, detach */
			if(ucmsm_p->mLTE_NM->mNMInfo.pdp_type == E_PDP_TYPE_IPV4)
			{
				if(false == isValidIPv4Address(ucmsm_p->ip_addr))
				{
					if(ucmsm_p->mLTE_NM->mCommonInfo.imsParams.Test_Mode == TEST_MODE_ENABLE)
					{
						ucmsm_p->transit(ucmsm_p->disconnectingState);
						if(ucmsm_p->mLTE_NM->mNMInfo.isPowerdown)
						{
							/* DAL for the power down to be called here */
							iResult = ucmsm_p->getDal()->uconnSetStatus(UConnSetStatus::POWER_DOWN,NULL);
							//asper at_command
						}
						else
						{
							/* DAL for the net_detach to be called here */
							iResult = ucmsm_p->getDal()->uconnNetDetach();
							//asper at_command
						}
					}
				}
			}

			U_SINT8 linklocal_addr[140];
			/* If IPv6 convert to hex and enable IPv6 on the Interface */
			if((ucmsm_p->mLTE_NM->mNMInfo.pdp_type == E_PDP_TYPE_IPV6) ||
				(ucmsm_p->mLTE_NM->mNMInfo.pdp_type == E_PDP_TYPE_IPV4V6))
			{
				/* May need to use ucmsm_p->mLTE_NM->mNMInfo.ipv6_address here */
				NAStoIPv6AddrConverter(ucmsm_p->mLTE_NM->mNMInfo.ipv6_address, linklocal_addr);
				strcpy(ucmsm_p->mLTE_NM->mNMInfo.ipv6_address, linklocal_addr);
			}

			/* Set the DNS Address */
			if(ucmsm_p->mLTE_NM->mNMInfo.pdp_type == E_PDP_TYPE_IPV6)
			{
				strcpy(ucmsm_p->mLTE_NM->mNMInfo.dyn_dnsv6, ucmsm_p->mLTE_NM->mNMInfo.dyn_dns);
			}

		#ifndef VERIZON
			if((!strlen(ucmsm_p->ip_addr)) && (!strlen(ucmsm_p->mLTE_NM->mNMInfo.ipv6_address)))
				enableDHCP(intface, isIPv6);
			else
			{
				if((ucmsm_p->mLTE_NM->mNMInfo.pdp_type == E_PDP_TYPE_IPV4) ||
					(ucmsm_p->mLTE_NM->mNMInfo.pdp_type == E_PDP_TYPE_IPV4V6))
				{
					//enableDHCP(intface, false);
					if(strlen(ucmsm_p->ip_addr))
					{
						addIPaddress(intface, ucmsm_p->ip_addr, 
							ucmsm_p->mLTE_NM->mNMInfo.gw_address, false,ucmsm_p->mLTE_NM->mNMInfo.dyn_dns);
					}
				}
				
				if((ucmsm_p->mLTE_NM->mNMInfo.pdp_type == E_PDP_TYPE_IPV6) ||
					(ucmsm_p->mLTE_NM->mNMInfo.pdp_type == E_PDP_TYPE_IPV4V6))
				{
					if(strlen(ucmsm_p->mLTE_NM->mNMInfo.ipv6_address))
					{
						addIPaddress(intface, ucmsm_p->mLTE_NM->mNMInfo.ipv6_address, 
							ucmsm_p->mLTE_NM->mNMInfo.dyn_dnsv6, isIPv6,ucmsm_p->mLTE_NM->mNMInfo.gwv6_address);
					}
				}
			}
		#else
			/* Add the IP address */
			if(!strlen(ucmsm_p->ip_addr))
				addIPaddress(intface, ucmsm_p->ip_addr, isIPv6);
			else
				ucmsm_p->transit(ucmsm_p->idleState);
		#endif

			if((ucmsm_p->mLTE_NM->mNMInfo.pdp_type == E_PDP_TYPE_IPV4) ||
				(ucmsm_p->mLTE_NM->mNMInfo.pdp_type == E_PDP_TYPE_IPV4V6))
			{
				setDnsAddresses(intface, ucmsm_p->mLTE_NM->mNMInfo.dyn_dns, ucmsm_p->mLTE_NM->mNMInfo.sec_dns, false);
			}
			if((ucmsm_p->mLTE_NM->mNMInfo.pdp_type == E_PDP_TYPE_IPV6) ||
				(ucmsm_p->mLTE_NM->mNMInfo.pdp_type == E_PDP_TYPE_IPV4V6))
			{
				setDnsAddresses(intface, ucmsm_p->mLTE_NM->mNMInfo.dyn_dnsv6, ucmsm_p->mLTE_NM->mNMInfo.secv6_dns, isIPv6);
			}

			/* For IPv6 Router Discovery needs to be Disabled then Enabled */
			if((ucmsm_p->mLTE_NM->mNMInfo.pdp_type == E_PDP_TYPE_IPV6) ||
				(ucmsm_p->mLTE_NM->mNMInfo.pdp_type == E_PDP_TYPE_IPV4V6))
			{
				if(strlen(ucmsm_p->mLTE_NM->mNMInfo.ipv6_address))
				{
					disableRouterDiscovery(intface);
				}
			}

			/* Disable Neighbour Solicitation */
			disableNS();

			/* For IPv6 bring the Interface down first */
			if((ucmsm_p->mLTE_NM->mNMInfo.pdp_type == E_PDP_TYPE_IPV6) ||
				(ucmsm_p->mLTE_NM->mNMInfo.pdp_type == E_PDP_TYPE_IPV4V6))
			{
				setIfStatus(intface, "down");
			}

			/* Bring up the host's interface */
			setIfStatus(intface, "up");

			/* For IPv6 bring the Interface down first */
			if((ucmsm_p->mLTE_NM->mNMInfo.pdp_type == E_PDP_TYPE_IPV6) || 
				(ucmsm_p->mLTE_NM->mNMInfo.pdp_type == E_PDP_TYPE_IPV4V6))
			{
				if(strlen(ucmsm_p->mLTE_NM->mNMInfo.ipv6_address))
				{
					enableRouterDiscovery(intface);
				}
			}

			if((ucmsm_p->mLTE_NM->mNMInfo.pdp_type == E_PDP_TYPE_IPV6) || 
				(ucmsm_p->mLTE_NM->mNMInfo.pdp_type == E_PDP_TYPE_IPV4V6))
			{
				/* Start the RSRA Timer to check if RS/RA is complete */
				if((ucmsm_p->RSRATimer == NULL) && 
					(ucmsm_p->mLTE_NM->mCommonInfo.imsParams.Test_Mode == TEST_MODE_ENABLE))
				{
					ucmsm_p->RSRATimer = USetTimer(RSRA_POLLING_INTERVAL, 0,(U_CALLBACK)TimerCallBack);
				}
			}
			ucmsm_p->mLTE_NM->state = NM_CONNECTED_STATE;
			ucmsm_p->mLTE_NM->updateNMStateAndPost(LTE_TECH,ucmsm_p->mLTE_NM->state);


		#if 0
			while(1)
			{
				uconnATNetStatusGet(E_RAT_EUTRAN);
				if(ucmsm_p->mLTE_NM->mNMInfo.Modem_state != 0)
				{
					/* Display CONNECTED after this point */
					SAPI_eventCB((UEvent*)connectedStateEvt_p);
					ucmsm_p->mLTE_NM->mNMInfo.state = CONNECTED_STATE;
					break;
				}
			}
		#endif /* if 0 */			
		}
		break;
		case UCONN_EVT_TIMEOUT://asper at_command
		{
			UTimeOutEvent* timeoutevt = (UTimeOutEvent*)evt;
			if(ucmsm_p->RSRATimer == timeoutevt->timerId)
			{
				LOG(ULOG_DEBUG_LEVEL, "RSRA Polling Timer \n");
				/* We would need to check if RS/RA is complete here */
				U_UINT32 u32RSRACompStatus = 0;
				u32RSRACompStatus = isRSRAComplete(ucmsm_p->mLTE_NM->mNMInfo.Adapter_desc);
				if((u32RSRACompStatus == ROUTER_ADV_FAILURE) &&
					(ucmsm_p->RS_attempts < RSRA_MAX_TRIES))
				{
					/* The timer needs to fire again in this case */
					ucmsm_p->RSRATimer = USetTimer(RSRA_POLLING_INTERVAL, 0,(U_CALLBACK)TimerCallBack);
					ucmsm_p->RS_attempts++;		/* Increment the attempts */
				}
				else if(u32RSRACompStatus == ROUTER_ADV_SUCCESS)
				{
					/* APN Table Entry and IP IF Status UP to de done here */
					LOG(ULOG_DEBUG_LEVEL, "ROUTER ADVERTISEMENT SUCCESSFUL \n");
					ucmsm_p->RS_attempts = 0;
					ucmsm_p->IPv6_Throttle_count = 0;
				}
				else	/* RA has failed for sure */
				{
					/* If IPv4 address has been received, then continue */
					if(strlen(ucmsm_p->mLTE_NM->mNMInfo.ip_address))	
					{
						/* AT*APN and AT*IPIFSTATUS here */

					}
					else /* If only an IPv6 address has been received */
					{
						/* Detach command to be sent and throttle counter to be
						incremented */
						ucmsm_p->transit(ucmsm_p->disconnectingState);
						if(ucmsm_p->mLTE_NM->mNMInfo.isPowerdown)
						{
							/* DAL for the power down to be called here */
							iResult = ucmsm_p->getDal()->uconnSetStatus(UConnSetStatus::POWER_DOWN,NULL);
							
						}
						else
						{
							/* DAL for the net_detach to be called here */
							iResult = ucmsm_p->getDal()->uconnNetDetach();
						}
						ucmsm_p->IPv6_Throttle_count++;
						ucmsm_p->throttle_counter_update();
					}
				}
			}
			else
			{
				return UCMEnabledState::handle(evt);
			}
		}
		break;		
		case UCONN_EVT_NET_DETACH:
		{
			/* Needed to handle unsolicited Detach events from the PS */
			ucmsm_p->transit(ucmsm_p->disconnectingState);
			ucmsm_p->getState()->handle(evt);
			ucmsm_p->getDal()->uconnConfigure(UConnConfig::POWER_UP);
		}
		break;
		case UCONN_EVT_LTE_REG_STATUS:
		{
			ULTERegStatusEvent *regEvt = (ULTERegStatusEvent *) evt;
			if(regEvt->getRegStatus() == 0)
			{
				if(ucmsm_p->mLTE_NM->mCommonInfo.imsParams.Test_Mode == TEST_MODE_ENABLE)
				{
					ucmsm_p->transit(ucmsm_p->disconnectingState);
					if(ucmsm_p->mLTE_NM->mNMInfo.isPowerdown)
					{
						/* DAL for the power down to be called here */
						iResult = ucmsm_p->getDal()->uconnSetStatus(UConnSetStatus::POWER_DOWN,NULL);
					}
					else
					{
						/* DAL for the net_detach to be called here */
						iResult = ucmsm_p->getDal()->uconnSetStatus(UConnSetStatus::POWER_DOWN,NULL);
					}
					//ucmsm_p->getState()->handle(evt);
				}
				else /* If Test Mode is Disabled */
				{
					iResult = ucmsm_p->getDal()->uconnConfigure(UConnConfig::IMS_CONFIG_DETACH);
				}
			}
		}
		break;
		default:
			return UCMEnabledState::handle(evt);
			break;
	}
	return iResult;
}

/***********************************************************************
*	UCMConnectedState::exit() -
*	Connected State Exit point
***********************************************************************/

void UCMConnectedState::exit()
{
	LOG(ULOG_ERROR_LEVEL, "CONNECTED State Exit \n");
}