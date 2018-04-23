/*****************************************************************************
**
** uconn_EnabledState.cpp
** This file describes the methods for the UConnect Idle State
** Machine
**
** Author: Sathyanarayanan S
** Date  : 04-JAN-11
** (c) United Mobile Apps Pvt Ltd (2010 - 2015)

    VER		  DATE		AUTHOR		DESCRIPTION
	0.1		04/01/11     SS			Creation
	1.0		20/04/12	MSH			Added code for 3G NM
****************************************************************************/

#include "uconn_StateMachine.h"
#include "uconn_EnabledState.h"
#include "uconn_server.h"
#include "three_g_dal.h"
#include "ULog.h"
#include "UCommonUtils.h"

/***********************************************************************
*	UEnabledState::entry() -
*	Idle State Entry point
***********************************************************************/

void UCMEnabledState::entry()
{
	U_UINT32 result = UCM_SUCCESS;
	UCMStateMachine* ucmsm_p = (UCMStateMachine*)this->getStateMachine();

	UServerStateEvent *enableStateEvt_p = new UServerStateEvent(ENABLED_STATE);

	LOG(ULOG_ERROR_LEVEL, "ENABLED State Entry \n");

	ucmsm_p->getDal()->uconnConfigure(VERSION_INFO);//To populate the version string in nminfo

	if ( ucmsm_p->mLTE_NM != NULL )
	{
		ucmsm_p->mLTE_NM->mNMInfo.context_id = 1;
		ucmsm_p->mLTE_NM->postEventToBearerManager((UEvent*)enableStateEvt_p);
		ucmsm_p->mLTE_NM->state = NM_NOT_READY_STATE;
		ucmsm_p->mLTE_NM->updateNMStateAndPost(LTE_TECH,ucmsm_p->mLTE_NM->state);

		/* Read the Configuration here - to be implemented later */

		ucmsm_p->mLTE_NM->mNMInfo.state = ENABLED_STATE;
		/* ucmsm_p->updateTimer = USetTimer_NM(1000, 1, 
			ucmsm_p->mLTE_NM->getNMServer(), (U_CALLBACK)TimerCallBack_NM); */
		ucmsm_p->getDal()->uconnConfigure(POWER_STATUS_QUERY);
		ucmsm_p->getDal()->uconnConfigure(UConnConfig::ENABLED_STATE_ENTRY);
	}
	else if ( ucmsm_p->mThreeG_NM != NULL ) 
	{
		ucmsm_p->mThreeG_NM->ChangeStateAndInformBM(NM_NOT_READY_STATE);
	}
	ucmsm_p->stateTimer = NULL;
	//result = ucmsm_p->getDal()->uconnConfigure(UConnConfig::SET_FREQ_POWER_UP);
	ucmsm_p->RSRATimer = NULL;
	ucmsm_p->throttleTimer = NULL;
	ucmsm_p->isRASDialTimerRunning = false;
	ucmsm_p->isUSSDTimerRunning = false;

	ucmsm_p->RASDialTimer = NULL;
	ucmsm_p->ThreeGStatsTimer = NULL;
	ucmsm_p->USSDTimer = NULL;

	if ( ucmsm_p->mThreeG_NM != NULL ) 
	{
		ucmsm_p->ThreeGStatsTimer = USetTimer_NM(5000, 0,
			ucmsm_p->mThreeG_NM->mNMServer, (U_CALLBACK)TimerCallBack_NM);
	}
	
	//result = ucmsm_p->getDal()->uconnConfigure(UConnConfig::ENABLED_STATE_ENTRY); 

	/* First up, enquire if Power up has already been done */
	//result = ucmsm_p->getDal()->uconnConfigure(POWER_STATUS_QUERY);//AT+CFUN?

}
/***********************************************************************
*	UEnabledState::handle -
*	Idle State handler
*
*	Parameters
*	evt	- Pointer to the Event type
***********************************************************************/

U_UINT32 UCMEnabledState::handle(UEvent* evt)
{
	U_UINT32 cmdID = evt->getCmdId();
	UCMStateMachine* ucmsm_p = (UCMStateMachine*)this->getStateMachine();
	U_UINT32 result = UCM_SUCCESS;

	LOG(ULOG_ERROR_LEVEL, "ENABLED State Handle -%d\n",evt->getCmdId());
	//printf("ENABLED State Handle -%d\n",evt->getCmdId());
	switch(cmdID)
	{
#if 0
	case UCONN_CMD_LTE_4G_ONOFF:
		/* 4G Off to be handled in the Enabled State */
		UConfigEnable4GEvent *cfg4Gevt = (UConfigEnable4GEvent *) evt;
			if(cfg4Gevt->cfgenable4g == OFF_4G){

				ucmsm_p->setState(ucmsm_p->enabledState);
				ucmsm_p->transit(ucmsm_p->disabledState);
			}
		break;

	case UCONN_EVT_ROAMING_IND:
		{
			UIdleEvent* idleEvt = (UIdleEvent*)evt;
			ucmsm_p->mLTE_NM->mNMInfo.roaming_status = idleEvt->roam_status;
		}
		break;
#endif
	case UCONN_CMD_LTE_NET_DETACH:
		{
			ucmsm_p->transit(ucmsm_p->idleState);
		}
		break;
#if 0
	case UCONN_EVT_LTE_REG_STATUS:
        {
			ULTERegStatusEvent * regEvt = (ULTERegStatusEvent *) evt;
			
			/* Registration accepted, wait for the EVT_PRIMARY_ACT */
			LOG(ULOG_INFO_LEVEL, "EVT_LTE_REG_STATUS received \n");

			//ucmsm_p->mLTE_NM->mNMInfo.lte_reg_status = regEvt->getRegStatus();
			
			//ucmsm_p->transit(ucmsm_p->wait_for_primaryAct);
			
			/* The SM starts at the idleState */
            ucmsm_p->setState(ucmsm_p->idleState);
            ucmsm_p->getState()->entry();		//Enter the IDLE State
        }
		break;
#endif

	case UCONN_DEVICE_STATE_CHANGE:
        {
#if 1
			UDeviceStateChange * devt = ( UDeviceStateChange *)evt;
			if (ucmsm_p->mLTE_NM != NULL )
			{
				if(devt->getDeviceState()== NOTI_DEVICE_NOT_OPEN)
				{
					ucmsm_p->reset();
					ucmsm_p->getDal()->uconnATResetDevice();
					ucmsm_p->transit(ucmsm_p->disabledState);
				}
#if 0
				if(devt->getDeviceState()== NOTI_DEVICE_OPEN)
				{
					DAL_STATUS InitStatus = 
						ucmsm_p->getDal()->uconnInit(ucmsm_p->mLTE_NM);
					if(InitStatus == UCM_SUCCESS)
					{
						//::Sleep(2000);
						//ucmsm_p->mLTE_NM->mNMInfo.isSerialPortOpened = true;
						/* OpenCmd success may be enough to start the CM */
						ucmsm_p->mLTE_NM->mCommonInfo.device_status = true;
						LOG(ULOG_DEBUG_LEVEL,"Dal Init called sucessfuly\n");
						//ucmsm_p->transit(ucmsm_p->enabledState);

						result = ucmsm_p->getDal()->uconnConfigure(UConnConfig::ENABLED_STATE_ENTRY); 

						/* First up, enquire if Power up has already been done */
						result = ucmsm_p->getDal()->uconnConfigure(POWER_STATUS_QUERY);//AT+CFUN?
						/* Post event to the GUI to decide if settings need to be shown */
						UNMShowSettings* lteSettings = new UNMShowSettings(true, 
							ucmsm_p->mLTE_NM->getTechID());
						ucmsm_p->mLTE_NM->postEventToBearerManager((UEvent*)lteSettings);
					}					
				}
				else if(devt->getDeviceState()== NOTI_DEVICE_NOT_OPEN)
				{
					ucmsm_p->reset();
					ucmsm_p->getDal()->uconnATResetDevice();
					ucmsm_p->transit(ucmsm_p->disabledState);
				}
#endif /* if 0 */
			}
			else if ( ucmsm_p->mThreeG_NM != NULL )
			{
				if(devt->getDeviceState()== NOTI_DEVICE_OPEN)
				{
					// Event shud not come in enabled state.
					LOG(ULOG_INFO_LEVEL, "NOTI_DEVICE_OPEN recd in enabled state for 3G device.. Error!!! \n");
				}
				else if ( devt->getDeviceState()== NOTI_DEVICE_NOT_OPEN)
				{
					/* Post event to the GUI to decide if settings need to be shown */
//					UNMShowSettings* threeGSettings = new UNMShowSettings(false, 
//						ucmsm_p->mThreeG_NM->getTechID());
//					ucmsm_p->mThreeG_NM->postEventToBearerManager((UEvent*)threeGSettings);
					LOG(ULOG_INFO_LEVEL, "NOTI_DEVICE_NOT_OPEN recd in enabled state for 3G device. \n");

					/* Msh: Reset and Disable 3G NM  since there is a device plug out. 
					 */
					ucmsm_p->reset();
					ucmsm_p->mThreeG_NM->DisableThreeGNM();
					ucmsm_p->transit(ucmsm_p->disabledState);
				}
			}
#endif
        }
        break;
	case UCONN_BM_NM_DISABLE:
		{
			if (evt->getTechId() == LTE_TECH ) 
			{
				ucmsm_p->mLTE_NM->mCommonInfo.cfg_params.Enable_4G = false;
				ucmsm_p->getDal()->uconnNetDetach();
				ucmsm_p->reset();
				ucmsm_p->getDal()->uconnATResetDevice();
				ucmsm_p->transit(ucmsm_p->disabledState);
			}
			else if ( evt->getTechId() == THREE_G_TECH ) 
			{
				LOG(ULOG_INFO_LEVEL, "UCONN_BM_NM_DISABLE recd in enabled state for 3G device. \n");
				/* Msh: reset and Disable3G NM and exit to Disabled state. 
				 * The var EnableCommandRecd keeps a track of when the Enable 
				 * was recd from GUI. So it is reset here.
				 */
				ucmsm_p->reset();
				ucmsm_p->mThreeG_NM->ThreeGNMInfo.EnableCommandRecd = false;
				ucmsm_p->mThreeG_NM->DisableThreeGNM();
				ucmsm_p->transit(ucmsm_p->disabledState);
			}
		}
		break;
	case UCONN_BM_NM_CONNECT:
		{
			if (evt->getTechId() == LTE_TECH ) 
			{
			}
			else if ( evt->getTechId() == THREE_G_TECH ) 
			{
				U_DWORD ReturnFromDialRas;
				LOG(ULOG_INFO_LEVEL, "UCONN_BM_NM_CONNECT recd in enabled state for 3G device. \n");
				if ( (ucmsm_p->mThreeG_NM->ThreeGRasConnection.GetRasisDialing() == false) &&
					 (ucmsm_p->mThreeG_NM->ThreeGRasConnection.GetRasisConnected() == false))
				{
					if ( ucmsm_p->isRASDialTimerRunning == false )
					{
						
						ReturnFromDialRas = ucmsm_p->mThreeG_NM->ThreeGRasConnection.DialRas( 
									ucmsm_p->mThreeG_NM->mCommonInfo.rasUserName, 
									ucmsm_p->mThreeG_NM->mCommonInfo.rasPassword);
						if ( ReturnFromDialRas == RAS_SUCCESS) 
						{
							if ( ucmsm_p->mThreeG_NM->ThreeGRasConnection.GetRasisConnected() == true)
							{
								ucmsm_p->mThreeG_NM->ChangeStateAndInformBM(NM_CONNECTED_STATE);
							}

							ucmsm_p->RASDialTimer = USetTimer_NM(7000, 0,
									ucmsm_p->mThreeG_NM->mNMServer, (U_CALLBACK)TimerCallBack_NM);
							ucmsm_p->isRASDialTimerRunning = true;
						}
						else if ( ReturnFromDialRas == ERROR_REMOTE_DISCONNECTION ||
								  ReturnFromDialRas == ERROR_NO_CONNECTION ||
								  ReturnFromDialRas == ERROR_NO_ANSWER ||
								  ReturnFromDialRas == ERROR_LINE_BUSY ||
								  ReturnFromDialRas == ERROR_VOICE_ANSWER ||
								  ReturnFromDialRas == ERROR_PPP_TIMEOUT ||
								  ReturnFromDialRas == ERROR_PPP_NO_PROTOCOLS_CONFIGURED ||
								  ReturnFromDialRas == ERROR_PPP_NO_RESPONSE ||
								  ReturnFromDialRas == ERROR_PPP_CP_REJECTED ||
								  ReturnFromDialRas == ERROR_PPP_LCP_TERMINATED ||
								  ReturnFromDialRas == ERROR_PPP_REQUIRED_ADDRESS_REJECTED ||
								  ReturnFromDialRas == ERROR_PPP_NCP_TERMINATED ||
								  ReturnFromDialRas == ERROR_CONNECTION_REJECT ||
								  ReturnFromDialRas == ERROR_CONGESTION ||
								  ReturnFromDialRas == ERROR_TEMPFAILURE ||
								  ReturnFromDialRas == ERROR_CONNECTING_DEVICE_NOT_FOUND ||
								  ReturnFromDialRas == ERROR_IDLE_TIMEOUT ||
								  ReturnFromDialRas == ERROR_LINK_FAILURE ||
								  ReturnFromDialRas == ERROR_HIBERNATION ||
								  ReturnFromDialRas == ERROR_SYSTEM_SUSPENDED )
						{
							ucmsm_p->mThreeG_NM->HandleRasError(ReturnFromDialRas);
							ucmsm_p->RASDialTimer = USetTimer_NM(7000, 0,
							ucmsm_p->mThreeG_NM->mNMServer, (U_CALLBACK)TimerCallBack_NM);
							ucmsm_p->isRASDialTimerRunning = true;

							/* Msh: If DialRas returns an error then do nothing but start the RAS timer. 
							 * Sometimes RAS takes some time to connect and is not instant most of the times. 
							 * On Expiry of RAS timer the connection status is checked and if the connection
							 * is yet not established, proper action is taken.
							 */						
						}
						else
						{
							ucmsm_p->mThreeG_NM->HandleRasError(ReturnFromDialRas);
							// Do Nothing. Timer not started here since we want the user to take some action. 
						}

					}
				}
			}
		}
		break;
	case UCONN_BM_NM_DISCONNECT:
		{
			if (evt->getTechId() == LTE_TECH ) 
			{
			}
			else if ( evt->getTechId() == THREE_G_TECH ) 
			{
				LOG(ULOG_INFO_LEVEL, "UCONN_BM_NM_DISCONNECT recd in enabled state for 3G device. \n");
				/* Msh: Hangup from the RAS connection, if it is connected. 
				 * The function DisconnectThreeGNM does all inits of vars as well as
				 * hangup if required. 
				 * Depending upon the Modem state the 3G NM moves into ready or not ready state. 
				 */
				ucmsm_p->mThreeG_NM->DisconnectThreeGNM();

				if ( ( ucmsm_p->mThreeG_NM->ThreeGNMInfo.reg_status == 1 ) ||
				     ( ucmsm_p->mThreeG_NM->ThreeGNMInfo.reg_status == 5 ) )
				{	
					ucmsm_p->mThreeG_NM->ChangeStateAndInformBM(NM_READY_STATE);
				}
				else 
				{
					ucmsm_p->mThreeG_NM->ChangeStateAndInformBM(NM_NOT_READY_STATE);
					UThreeGDal* dal_3g = (UThreeGDal*)ucmsm_p->getDal();
					dal_3g->ATCommandRegisterNetworkStatus();
				}
			}
		}
		break;
	case UCONN_BM_NM_SEARCH:
		{
			if ( evt->getTechId() == THREE_G_TECH ) 
			{
				LOG(ULOG_INFO_LEVEL, "UCONN_BM_NM_SEARCH recd in enabled state for 3G device. \n");
				UThreeGDal* dal_3g = (UThreeGDal*)ucmsm_p->getDal();
				dal_3g->ATCommandInitNetworkSearch();
			}
		}
		break;

#if 1
    case UCONN_EVT_LTE_REG_STATUS:
        {
			ULTERegStatusEvent * regEvt = (ULTERegStatusEvent *) evt;
            //LOG(ULOG_INFO_LEVEL, "Hurray \n");
            if(ucmsm_p->bSyncedWithDevice == false){
				if(regEvt->getRegStatus() == 1)	//1 = Connected, 0 = Disconnected
                {
                    /* Move to the CONNECTED State */
                    ucmsm_p->setState(ucmsm_p->connectedState);
                    ucmsm_p->getState()->entry();
                }
                else
                {
                    rat = ucmsm_p->mLTE_NM->mNMInfo.rat;
                    /* The SM starts at the idleState */
                    ucmsm_p->setState(ucmsm_p->idleState);
                    ucmsm_p->getState()->entry();		//Enter the IDLE State
				}
				ucmsm_p->bSyncedWithDevice = true;
			}
        }
        break;
#endif
	case UCONN_EVT_TIMEOUT:
		{
			if ( ucmsm_p->mLTE_NM != NULL ) 
			{
				UTimeOutEvent* timeoutEvt = (UTimeOutEvent*)evt;
				if(timeoutEvt->getTimerId() == ucmsm_p->updateTimer)
				{
					LOG(ULOG_INFO_LEVEL, "Update Timer Called \n");
					/* Call the DAL APIs to retrieve parameter information */
					/*Not required for AT Command
					result = uconnATNetSigQualGet(rat);
					result = ucmsm_p->getDal()->uconnGetStatus(GET_NET_STATUS);
					result = uconnATStatsUpdate(E_STAT_GENERAL);
					result = uconnATStatsUpdate(E_STAT_MAC);
					result = uconnATStatsUpdate(E_STAT_PHY);*/
					
					if(result)
					{
						LOG(ULOG_ERROR_LEVEL, "NetSigQualGet failed \n");
					}
					ucmsm_p->updateTimer = USetTimer(1000, 0,(U_CALLBACK)TimerCallBack);
#if 0
					/* Update the persistence file with the CM Info structure */
	                #ifndef QTPROJECT
		                fopen_s(&config.persistfile_p, config.persistfilename_p, "wb");
			        #else
				        config.persistfile_p = fopen(config.persistfilename_p, "wb");
					#endif
					fwrite(&ucmsm_p->mLTE_NM->mNMInfo, sizeof(CM_Info_s), 1, config.persistfile_p);
					fclose(config.persistfile_p);
#endif /* if 0 */
				}
				/* Check now the State timeout Timer */
				else if(ucmsm_p->stateTimer == timeoutEvt->getTimerId())
				{
					/* Go back to the IDLE State in this case */
					LOG(ULOG_INFO_LEVEL, "STATE TIMEOUT \n");
					UKillTimer(ucmsm_p->stateTimer);
					ucmsm_p->resetCMInfo();
					ucmsm_p->stateTimer = NULL;
					ucmsm_p->transit(ucmsm_p->idleState);
				}
				else if(ucmsm_p->throttleTimer == timeoutEvt->getTimerId())
				{
					/* Throttle Timer has expired */
					ucmsm_p->isThrottleTimerRunning = false;
					UKillTimer(ucmsm_p->throttleTimer);
					ucmsm_p->throttleTimer = NULL;
				}
			}
			else if ( ucmsm_p->mThreeG_NM != NULL ) 
			{
				UTimeOutEvent* timeoutEvt = (UTimeOutEvent*)evt;
				if( ucmsm_p->RASDialTimer == timeoutEvt->getTimerId() )
				{
					LOG(ULOG_INFO_LEVEL, "RAS Timer Expiry. \n");
					if ( ucmsm_p->isRASDialTimerRunning ==true )
					{
						ucmsm_p->mThreeG_NM->HandleRasTimer();
					}
				}
				else if ( ucmsm_p->ThreeGStatsTimer == timeoutEvt->getTimerId() )
				{
					LOG(ULOG_INFO_LEVEL, "3G Stats Timer Expiry. \n");
					ucmsm_p->mThreeG_NM->Handle3GStatsTimerExpiry();
				}
				else if (ucmsm_p->USSDTimer == timeoutEvt->getTimerId() )
				{
					LOG(ULOG_INFO_LEVEL, "USSD Timer Expiry. \n");
					if ( ucmsm_p->isUSSDTimerRunning ==true )
					{
						ucmsm_p->mThreeG_NM->HandleUSSDTimer();
					}
				}
			}
		}
		break;
	case UCONN_EVT_SYSTEM_EVENT:
		{
			USystemEvent* sysEvt = (USystemEvent*)evt;
			U_UINT32 notifyEvtID = sysEvt->notifyEvent;
			ucmsm_p->mLTE_NM->mNMInfo.system_notification = notifyEvtID;
			LOG(ULOG_INFO_LEVEL, "SYSTEM EVENT from MODEM = %d\n", 
				notifyEvtID);
			if(notifyEvtID == NOTI_DEVICE_ERROR ||
				notifyEvtID == NOTI_DEVICE_DISCONNECTED ||
				notifyEvtID == NOTI_DEVICE_NOT_OPEN ||
				notifyEvtID == NOTI_DISCONNECTED)
			{
				/* On a System ERROR, transit to ENABLED state */
				memset(&ucmsm_p->mLTE_NM->mNMInfo.modemStats,0,sizeof(Modem_stats_s));		// needs to be reviewed
				ucmsm_p->transit(ucmsm_p->enabledState);
			}

		}
		break;
	case UCONN_EVT_READY_FOR_ATTACH://asper at_command
		{
            ucmsm_p->setState(ucmsm_p->idleState);
            ucmsm_p->getState()->entry();		//Enter the IDLE State
		}
		break;
    /* IMS Events Handled here */
    case UCONN_EVT_REQ_ATTACH:
        {
            /* Move to IDLE state and when the user issues a Connect perform the
            IMS regn and PDN netattach */
            ucmsm_p->setState(ucmsm_p->idleState);
			ucmsm_p->getState()->handle(evt);//asper at_command
            /* Call the IMS Registration Related AT commands */

		//	result = ucmsm_p->getDal()->uconnConfigure(UConnConfig::IMS_CONFIG);//asper at_command
          

          //  result = ucmsm_p->getDal()->uconnConfigure(UConnConfig::IMS_RESPONSE);//asper at_command
        }
        break;
    case UCONN_EVT_REQ_POR:
        {
            ucmsm_p->setState(ucmsm_p->idleState);
            UImsReqPowerCycle *porEvt_p = new UImsReqPowerCycle(0);
            //SAPI_eventCB((UEvent*)porEvt_p);
        }
        break;
    case UCONN_EVT_REQ_IMS_LTD_MODE:
        {
            UImsRegLtdMode *reqLtdModeEvt_p = new UImsRegLtdMode(0);
            //SAPI_eventCB((UEvent*)reqLtdModeEvt_p);
        }
        break;

	case UCONN_EVT_STATS_RECEIVED:
		{
			ucmsm_p->mLTE_NM->mNMInfo.bUpdateStatsResponsePending = false;
			
			UModemStatisticEvent* modemStats = new UModemStatisticEvent(ucmsm_p->mLTE_NM->mNMInfo.m_nL1AStats);
			ucmsm_p->mLTE_NM->postEventToBearerManager((UEvent*)(modemStats));
			
			UMEASStatisticEvent* measStats = new UMEASStatisticEvent(ucmsm_p->mLTE_NM->mNMInfo.m_nMeasStats);
			ucmsm_p->mLTE_NM->postEventToBearerManager((UEvent*)(measStats));
			
			UBearerGetParamRsp* rssiParamRsp = new UBearerGetParamRsp(LTE_TECH,RSSI,ucmsm_p->mLTE_NM->mNMInfo.m_nMeasStats.rssiVal);
			ucmsm_p->mLTE_NM->postEventToBearerManager((UEvent*)(rssiParamRsp));

			UBearerGetParamRsp* cinrParamRsp = new UBearerGetParamRsp(LTE_TECH,CINR,ucmsm_p->mLTE_NM->mNMInfo.m_nMeasStats.cinrVal);
			ucmsm_p->mLTE_NM->postEventToBearerManager((UEvent*)(cinrParamRsp));


		}
		break;
	case UCONN_CMD_UPDATE_STATS:
		{
			LOG(ULOG_INFO_LEVEL, "Update Statistics Called \n");

			// bUpdateStatsResponsePending
			/* Call the DAL APIs to retrieve parameter information */
			if(false == ucmsm_p->mLTE_NM->mNMInfo.bUpdateStatsResponsePending)
			{
				result = ucmsm_p->getDal()->uconnGetStatistics(STAT_ALL,NULL);
				ucmsm_p->mLTE_NM->mNMInfo.bUpdateStatsResponsePending = true;
			}
			else
			{
				//cout << "ALARM    Droping stat requests since responses are waiting" << endl;
			}
		}
		break;
    case UCONN_TEST_MODE_TOGGLE:
        {
			LOG(ULOG_INFO_LEVEL, "TEST MODE Toggled \n");
            ucmsm_p->reset();
            ucmsm_p->setState(ucmsm_p->enabledState);
            ucmsm_p->getState()->entry();
        }
        break;
	case UCONN_POWERUP_RSP:
		{
			LOG(ULOG_INFO_LEVEL, "Power up response \n");
			UPowerupRspEvent *powEvt = (UPowerupRspEvent *)evt;
			U_UINT32 currPowerState = powEvt->powerupState;
			
			ucmsm_p->bPowerUpDone = (currPowerState!=0);
			
			if ( ucmsm_p->mLTE_NM != NULL )
			{
				if(currPowerState != 1)
				{
					result = ucmsm_p->getDal()->uconnConfigure(SET_FREQ);//AT+EARFCN = 12345 // frequency set
					result = ucmsm_p->getDal()->uconnConfigure(INIT_CONFIG);//AT+CEREG=1 and AT+CGEREP=2,1
				
					if(ucmsm_p->mLTE_NM->mCommonInfo.imsParams.Test_Mode == TEST_MODE_ENABLE) 
					{
						result = ucmsm_p->getDal()->uconnConfigure(IMS_CONFIG_TEST);
				
					}
					/* Check for the Modem State before entering IDLE State */
					result = ucmsm_p->getDal()->uconnGetStatus(GET_NET_STATUS,NULL);
				}
				else
				{
					/* No power up needed, just query the Registration status */
					/* Check for the Modem State before entering IDLE State */
					result = ucmsm_p->getDal()->uconnGetStatus(GET_NET_STATUS,NULL);
				 
				}
			}
			else if ( ucmsm_p->mThreeG_NM != NULL ) 
			{
				LOG(ULOG_INFO_LEVEL, "UCONN_POWERUP_RSP recd in enabled state for 3G device. \n");
				/* Msh: Power up for the 3G device is successful, so now send the AT commands
				 * to register to the network and set the automatic mode. 
				 */
				UThreeGDal* dal_3g = (UThreeGDal*) ucmsm_p->mThreeG_NM->mNMServer->getDal();
				dal_3g->ATCommandRegisterNetwork();
				dal_3g->ATCommandRegisterNetworkStatus();
				dal_3g->ATCommandGetSignalStrength();
				dal_3g->ATCommandSetAutomaticNetworkMode();
				dal_3g->ATCommandGetModemInfo();
				/* To define the Primary context */
				U_PS_CTX_PRIMARY_PARAMS pri_params;
				pri_params.u32Bitfields = 1;
				pri_params.pdp_type = E_PDP_TYPE_IPV4;
				strcpy(pri_params.apn, ucmsm_p->mThreeG_NM->mCommonInfo.APN_name);
				strcpy((U_SINT8*)pri_params.pdp_addr, "0.0.0.0");
				pri_params.data_comp = E_DATA_COMP_V42;
				pri_params.hdr_comp = E_HDR_COMP_ROHC;
				dal_3g->uconnConfigNetParams(CONTEXT_PARAMS, (U_VOID *)&pri_params, 1);
//				dal_3g->ATCommandSetNetworkInfo();
//				dal_3g->ATCommandInitNetworkSearch();
			}
		}
		break;
	case UCONN_CMD_LTE_VERSION_INFO_REQ:
		{
			/* Get the Version info from the Modem */
			ucmsm_p->getDal()->uconnConfigure(VERSION_INFO);
		}
		break;
		
		/* IMS Detach events need to be handled here */
    case UCONN_EVT_IMS_DEREG:
    {
        LOG(ULOG_ERROR_LEVEL, "IMS Deregistration Succeeded\n");
	    ucmsm_p->transit(ucmsm_p->disconnectingState);
	    /* DAL for the net_detach to be called here */
		if(ucmsm_p->mLTE_NM->mNMInfo.isPowerdown)
		{
			/* DAL for the power down to be called here */
			result = ucmsm_p->getDal()->uconnSetStatus(UConnSetStatus::POWER_DOWN,NULL);
		}
		else
		{
			/* DAL for the net_detach to be called here */
			result = ucmsm_p->getDal()->uconnNetDetach();
		}
    }
    break;
    case UCONN_EVT_REQ_DETACH:
    {
        LOG(ULOG_ERROR_LEVEL, "REQUEST DETACH IMS Event received\n");
	    ucmsm_p->transit(ucmsm_p->disconnectingState);
	    /* DAL for the net_detach to be called here */
		if(ucmsm_p->mLTE_NM->mNMInfo.isPowerdown)
		{
			/* DAL for the power down to be called here */
			result = ucmsm_p->getDal()->uconnSetStatus(UConnSetStatus::POWER_DOWN,NULL);
		}
		else
		{
			/* DAL for the net_detach to be called here */
			result = ucmsm_p->getDal()->uconnNetDetach();
		}
    }
    break;
	case UCONN_EVT_PDN_REJECT:
	{
		/* Increment the PDN Reject Throttle count */
		ucmsm_p->PDN_Reject_Throttle_count++;
		LOG(ULOG_INFO_LEVEL, "PDN Connection Rejected - %d\n", 
			ucmsm_p->PDN_Reject_Throttle_count);
		ucmsm_p->throttle_counter_update();
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
	case UCONN_EVT_PARAM_UPDATE:
		{
			UNMParamUpdate *ParamUpdateEvt = (UNMParamUpdate*) evt ;
			if ( ucmsm_p->mThreeG_NM != NULL ) 
			{
				if (ParamUpdateEvt->NM_Param == UCONN_EVT_NM_RSSI_UPDATE ) 
				{
					LOG(ULOG_INFO_LEVEL, "RSSI value is recd in enabled state for 3G device. \n");
					ucmsm_p->mThreeG_NM->Update3GParams( UCONN_EVT_NM_RSSI_UPDATE, ParamUpdateEvt->value, 0);
				}
				else if (ParamUpdateEvt->NM_Param == UCONN_RSP_NM_RSSI_CINR_UPDATE )
				{
					LOG(ULOG_INFO_LEVEL, "RSSI and CINR values isrecd in enabled state for 3G device. \n");
					ucmsm_p->mThreeG_NM->Update3GParams( UCONN_RSP_NM_RSSI_CINR_UPDATE, 
						ParamUpdateEvt->value, ParamUpdateEvt->value2);
				}
				else if (ParamUpdateEvt->NM_Param == UCONN_EVT_NM_MODEM_STATE_UPDATE )
				{
					if ( ucmsm_p->mThreeG_NM->ThreeGNMInfo.reg_status  != ParamUpdateEvt->value )
					{
						LOG(ULOG_INFO_LEVEL, "Network registration has changed in enabled state for 3G device. \n");
						ucmsm_p->mThreeG_NM->Update3GParams( UCONN_EVT_NM_MODEM_STATE_UPDATE, ParamUpdateEvt->value, 0 );	
						UThreeGDal* dal_3g = (UThreeGDal*) ucmsm_p->mThreeG_NM->mNMServer->getDal();
						dal_3g->uconnNetSearchStatus();
					}
					
				}
				else if (ParamUpdateEvt->NM_Param == UCONN_EVT_NM_MODEM_STATE_NETWORK_ACCESS_UPDATE)
				{
					if ( ucmsm_p->mThreeG_NM->ThreeGNMInfo.reg_status  != ParamUpdateEvt->value )
					{
						LOG(ULOG_INFO_LEVEL, "Network registration has changed in enabled state for 3G device. \n");
						ucmsm_p->mThreeG_NM->Update3GParams( UCONN_EVT_NM_MODEM_STATE_NETWORK_ACCESS_UPDATE, 
							ParamUpdateEvt->value, ParamUpdateEvt->value2 );	
						UThreeGDal* dal_3g = (UThreeGDal*) ucmsm_p->mThreeG_NM->mNMServer->getDal();
						dal_3g->uconnNetSearchStatus();
					}
				}
//				else if (ParamUpdateEvt->NM_Param == UCONN_EVT_NM_NETWORK_UPDATE)
//				{
//					ucmsm_p->mThreeG_NM->ThreeGNMInfo.network_name  = ParamUpdateEvt->value ;
//					ucmsm_p->mThreeG_NM->EncodeSearchNetworkResponse();
//				}
				else if (ParamUpdateEvt->NM_Param == UCONN_EVT_NM_MANUFACTURE_UPDATE )
				{
//					ucmsm_p->mThreeG_NM->ThreeGNMInfo.manufacturer = 
//						\ dal_3g->bufferParser->parsedInfo. ();
				}

				/* Msh: In case of no ongoing connection, if there is a change in Network status 
				 * and corresponding state of 3G NM, inform the BM of the same. 
				 */
				ucmsm_p->mThreeG_NM->Update3GNMState();
			}
		}
		break;
	case UCONN_RSP_NM_NETWORK_SEARCH:
		{
			UNMSearchNetworksRsp *SearchNetworkRsp = (UNMSearchNetworksRsp*) evt ;
			int TmpSize = sizeof(SearchNetworkRsp->threeGNetworkName);
			strncpy( ucmsm_p->mThreeG_NM->ThreeGNMInfo.network_name, SearchNetworkRsp->threeGNetworkName, TmpSize);
			ucmsm_p->mThreeG_NM->ThreeGNMInfo.network_name[TmpSize] = '\0';
			string encodeStr = ucmsm_p->mThreeG_NM->EncodeSearchNetworkResponse();
			UNMNetworkNameUpdate* ThreeG_NetworkNameUpdate = new UNMNetworkNameUpdate(encodeStr);
			ucmsm_p->mThreeG_NM->postEventToBearerManager((UEvent*)(ThreeG_NetworkNameUpdate));

		}
		break;
	case UCONN_RSP_NM_VERSION_INFO:
		{
			UNMVersionInfo *versioninfo = (UNMVersionInfo*) evt ;
			int TmpSize = sizeof(versioninfo->VersionInfo);
			strncpy(ucmsm_p->mThreeG_NM->mCommonInfo.versionInfo.u8VersionString, 
				   versioninfo->VersionInfo, TmpSize);
		}
		break;
	case UCONN_EVT_USSD:
		{
			UNMUSSDRsp *USSDResponse = (UNMUSSDRsp*) evt ;
			int TmpSize = strlen(USSDResponse->USSDStr);
			char* in_ussd = reinterpret_cast<char*>(USSDResponse->USSDStr);
			unsigned int ret_len;
			
			if ( ucmsm_p->mThreeG_NM->ThreeGNMInfo.UssdDecodingReqd == false )
			{
				UNMUSSDToGUI* USSDStr = new UNMUSSDToGUI(U_STRING(reinterpret_cast<const char*>(in_ussd)));
				ucmsm_p->mThreeG_NM->postEventToBearerManager((UEvent*)(USSDStr));
			}
			else
			{
		
				const unsigned char* decoded_ussd = ussd_decode(in_ussd, &ret_len, (TmpSize+1));
				if ( decoded_ussd != NULL )
				{
					/* Format a message and send it to GUI */
					//ucmsm_p->mThreeG_NM->EncodeUSSDString((unsigned char*)(decoded_ussd));
				
					UNMUSSDToGUI* USSDStr = new UNMUSSDToGUI(U_STRING(reinterpret_cast<const char*>(decoded_ussd)));
					ucmsm_p->mThreeG_NM->postEventToBearerManager((UEvent*)(USSDStr));
					free((char*)decoded_ussd);
				}
			}

			if ( ucmsm_p->isUSSDTimerRunning ==  true )
			{
				ucmsm_p->isUSSDTimerRunning = false;
				UKillTimer(ucmsm_p->USSDTimer);
				ucmsm_p->USSDTimer = NULL;
			}
		}
		break;
	case UCONN_EVT_SEND_USSD:
		{
			unsigned char* in_ussd = (unsigned char *)((UNMSendUSSD*)evt)->USSDStr; 
			int size_of_ussd = strlen((const char *)in_ussd);
			unsigned int ret_len;
			if ( ucmsm_p->mThreeG_NM->ThreeGNMInfo.UssdDecodingReqd == false )
			{
				UThreeGDal* dal_3g = (UThreeGDal*) ucmsm_p->mThreeG_NM->mNMServer->getDal();
				dal_3g->ATCommandUSSDStr(U_STRING(reinterpret_cast<const char*>(in_ussd)));
			}
			else
			{
				const unsigned char* encode_ussd = ussd_encode(in_ussd, &ret_len, (size_of_ussd));
				if ( encode_ussd != NULL)
				{
					UThreeGDal* dal_3g = (UThreeGDal*) ucmsm_p->mThreeG_NM->mNMServer->getDal();
					dal_3g->ATCommandUSSDStr(U_STRING(reinterpret_cast<const char*>(encode_ussd)));
					free((char*)encode_ussd);
				}
			}
			ucmsm_p->USSDTimer = USetTimer_NM(10000, 0,
									ucmsm_p->mThreeG_NM->mNMServer, (U_CALLBACK)TimerCallBack_NM);
			ucmsm_p->isUSSDTimerRunning = true;
		}
		break;
	default:
		LOG(ULOG_INFO_LEVEL, "Command not Valid \n");
		break;
	}
	return 0;
}

/***********************************************************************
*	UEnabledState::exit() -
*	Idle State Exit point
***********************************************************************/

void UCMEnabledState::exit()
{
	UCMStateMachine* ucmsm_p = (UCMStateMachine*)this->getStateMachine();
	LOG(ULOG_ERROR_LEVEL, "ENABLED State Exit \n");
	
	/* Delete all Enabled State mem allocations */

	/* Kill the Update Timer on exit */
	if(ucmsm_p->updateTimer)
	{
		UKillTimer(ucmsm_p->updateTimer);
		ucmsm_p->updateTimer = NULL;
	}

	/* Kill the Search Timer too, on exit */
	if(ucmsm_p->searchTimer)
	{
		UKillTimer(ucmsm_p->searchTimer);
		ucmsm_p->searchTimer = NULL;
	}

	if(ucmsm_p->RSRATimer)
	{
		UKillTimer(ucmsm_p->RSRATimer);
		ucmsm_p->RSRATimer = NULL;
	}

	if(ucmsm_p->throttleTimer)
	{
		UKillTimer(ucmsm_p->throttleTimer);
		ucmsm_p->throttleTimer = NULL;
	}

	if ( ucmsm_p->RASDialTimer)
	{
		UKillTimer(ucmsm_p->RASDialTimer);
		ucmsm_p->RASDialTimer = NULL;
		ucmsm_p->isRASDialTimerRunning = false;
	}

	if ( ucmsm_p->USSDTimer)
	{
		UKillTimer(ucmsm_p->USSDTimer);
		ucmsm_p->USSDTimer = NULL;
		ucmsm_p->isUSSDTimerRunning = false;
	}

	if ( ucmsm_p->ThreeGStatsTimer )
	{
		UKillTimer(ucmsm_p->ThreeGStatsTimer);
		ucmsm_p->ThreeGStatsTimer = NULL;
	}
}
