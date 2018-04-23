/*****************************************************************************
**
** uconn_DisabledState.cpp
** This file describes the methods for the UConnect Disabled State
** Machine
**
** Author: Sathyanarayanan S
** Date  : 10-FEB-11
** (c) United Mobile Apps Pvt Ltd (2010 - 2015)

    VER		  DATE		AUTHOR		DESCRIPTION
	0.1		10/02/11     SS			Creation
	1.0		20/04/12	MSH			Added code for 3G NM
****************************************************************************/

#include "uconn_DisabledState.h"
#include "uconn_StateMachine.h"
#include "ULog.h"
#include "uconn_OSAL.h"

//bool emulatorMode = true;

/***********************************************************************
*	UCMDisabledState::entry() -
*	Disabled State Entry point
***********************************************************************/

void UCMDisabledState::entry()
{

	UCMStateMachine* ucmsm_p = (UCMStateMachine*)this->getStateMachine();
	UServerStateEvent *disableStateEvt_p = new UServerStateEvent(DISABLED_STATE);
	//SAPI_eventCB((UEvent*)disableStateEvt_p);
	if ( ucmsm_p->mLTE_NM != NULL )
	{
		ucmsm_p->mLTE_NM->postEventToBearerManager((UEvent*)disableStateEvt_p);
		
		ENMStateForBM oldState = ucmsm_p->mLTE_NM->state;
		
		//don't post event to BM on creation of LTE NM object where the state is already disabled
		if(oldState != NM_DISABLED_STATE)
		{
			ucmsm_p->mLTE_NM->state = NM_DISABLED_STATE;
			ucmsm_p->mLTE_NM->updateNMStateAndPost(LTE_TECH,ucmsm_p->mLTE_NM->state);
		}
		//::Sleep(1000);

		
    
		LOG(ULOG_INFO_LEVEL, "Disabled State Entry \n");
		ucmsm_p->mLTE_NM->mNMInfo.state = DISABLED_STATE;
		
		/* Post event to the GUI to decide if settings need to be shown */
		UNMShowSettings* lteSettings = new UNMShowSettings(false, 
			ucmsm_p->mLTE_NM->getTechID());
		ucmsm_p->mLTE_NM->postEventToBearerManager((UEvent*)lteSettings);
	}
	else if ( ucmsm_p->mThreeG_NM != NULL )
	{
		
		ucmsm_p->mThreeG_NM->ChangeStateAndInformBM(NM_DISABLED_STATE);

		/* Post event to the GUI to decide if settings need to be shown */
		UNMShowSettings* threeGSettings = new UNMShowSettings(false, 
			ucmsm_p->mThreeG_NM->getTechID());
		ucmsm_p->mThreeG_NM->postEventToBearerManager((UEvent*)threeGSettings);
	}

#if 0
	if(ucmsm_p->getDal()->isDevicePresent())
	{
		DAL_STATUS InitStatus = ucmsm_p->getDal()->uconnInit();
		if(InitStatus == UCM_SUCCESS)
		{
			/* OpenCmd success may be enough to start the CM */
			ucmsm_p->mLTE_NM->mCommonInfo.device_status = 1;
			LOG(ULOG_DEBUG_LEVEL,"Dal Init called sucessfuly\n");
		}
		else
		{
			ucmsm_p->mLTE_NM->mCommonInfo.device_status = 0;
			ucmsm_p->mLTE_NM->mCommonInfo.com_port.clear();
			LOG(ULOG_ERROR_LEVEL,"Init Device failed.. exiting...\n");
		}
	}
	else
	{
		ucmsm_p->mLTE_NM->mCommonInfo.device_status = 0;
		ucmsm_p->mLTE_NM->mCommonInfo.com_port.clear();
		// Start a periodic timer for 1 sec interval to poll for the device present
		
		ucmsm_p->deviceStateCheckTimer = USetTimer(1000,1,(U_CALLBACK)TimerCallBack);
	}		
#endif
#if 0
		if(ucmsm_p->mLTE_NM->mCommonInfo.com_port.length() > 3)
		{
			DAL_STATUS InitStatus = ucmsm_p->getDal()->uconnInit();
			if(InitStatus == UCM_SUCCESS)
			{
				/* OpenCmd success may be enough to start the CM */
				ucmsm_p->mLTE_NM->mCommonInfo.device_status = 1;
				LOG(ULOG_DEBUG_LEVEL,"Dal Init called sucessfuly\n");
			}
			else
			{
				ucmsm_p->deviceStateCheckTimer = USetTimer(1000,1,(U_CALLBACK)TimerCallBack);
			}
		}
		else
			ucmsm_p->deviceStateCheckTimer = USetTimer(1000,1,(U_CALLBACK)TimerCallBack);
#endif
}

/***********************************************************************
*	UCMDisabledState::handle -
*	Disabled State handler
*
*	Parameters
*	evt	- Pointer to the Event type
***********************************************************************/

U_UINT32 UCMDisabledState::handle(UEvent* evt)
{
    UCMStateMachine* ucmsm_p = (UCMStateMachine*)this->getStateMachine();
	LOG(ULOG_ERROR_LEVEL, "Disabled State Handle - %d\n",evt->getCmdId());

	U_UINT32 cmdID = evt->getCmdId();
	
	switch(cmdID)
	{
		case  UCONN_DEVICE_STATE_CHANGE:
		{
			UDeviceStateChange * devt = ( UDeviceStateChange *)evt;
			if ( ucmsm_p->mLTE_NM != NULL )
			{
				if(ucmsm_p->mLTE_NM->mCommonInfo.cfg_params.Enable_4G)
				{
					if(devt->getDeviceState()== NOTI_DEVICE_OPEN)
					{
						//if(ucmsm_p->mLTE_NM->mCommonInfo.com_port.length() > 3)
						{
							/* COMPortFinder to be called here */
							ucmsm_p->reset();
							ucmsm_p->getDal()->isDevicePresent();
							DAL_STATUS InitStatus = -1;
							while(InitStatus != UCM_SUCCESS)
							{
								InitStatus = 
									ucmsm_p->getDal()->uconnInit(ucmsm_p->mLTE_NM);
							}
							if(InitStatus == UCM_SUCCESS)
							{
								//::Sleep(2000);
								//ucmsm_p->mLTE_NM->mNMInfo.isSerialPortOpened = true;
								/* OpenCmd success may be enough to start the CM */

								ucmsm_p->mLTE_NM->mCommonInfo.device_status = true;
								LOG(ULOG_DEBUG_LEVEL,"Dal Init called sucessfuly\n");
								ucmsm_p->transit(ucmsm_p->enabledState);
							}
						}
					}
				}
			}
			else if ( ucmsm_p->mThreeG_NM  != NULL )
			{
				if(devt->getDeviceState()== NOTI_DEVICE_OPEN)
				{
					LOG(ULOG_INFO_LEVEL, "NOTI_DEVICE_OPEN recd in disabled state for 3G device. \n");
					ucmsm_p->reset();
					if ( ucmsm_p->mThreeG_NM->EnableThreeGNM() == true)
					{
						if ( ucmsm_p->mThreeG_NM->ThreeGNMInfo.EnableCommandRecd == true)
						{
							ucmsm_p->transit(ucmsm_p->enabledState);
						}
					}
				}
				else if(devt->getDeviceState()== NOTI_DEVICE_NOT_OPEN)
				{
					LOG(ULOG_INFO_LEVEL, "NOTI_DEVICE_NOT_OPEN recd in disabled state for 3G device. \n");
					ucmsm_p->mThreeG_NM->mCommonInfo.device_status = 0;
					/* Msh: As of now do nothing, cauz on moving from Disable to Enable state 
					 * the init will be taken care of. 
					 */
				}
			}
		}
		break;
		case UCONN_BM_NM_ENABLE:
		{
			if (evt->getTechId() == LTE_TECH ) 
			{
				ucmsm_p->mLTE_NM->mCommonInfo.cfg_params.Enable_4G = true;
				ucmsm_p->reset();
				if(ucmsm_p->getDal()->isDevicePresent())
				{
					/* COMPortFinder to be called here */
					DAL_STATUS InitStatus = 
						ucmsm_p->getDal()->uconnInit(ucmsm_p->mLTE_NM);
					if(InitStatus == UCM_SUCCESS)
					{
						//::Sleep(2000);
						//ucmsm_p->mLTE_NM->mNMInfo.isSerialPortOpened = true;
						/* OpenCmd success may be enough to start the CM */
						ucmsm_p->mLTE_NM->mCommonInfo.device_status = true;
						LOG(ULOG_DEBUG_LEVEL,"Dal Init called sucessfuly\n");
						ucmsm_p->transit(ucmsm_p->enabledState);
					}
				}
			}
			else if ( evt->getTechId() == THREE_G_TECH ) 
			{
				LOG(ULOG_INFO_LEVEL, "UCONN_BM_NM_ENABLE recd in disabled state for 3G device. \n");
				/* Msh: If the device is already present and working, checked in function
				 * EnableThreeGNM. Then exit into enabled state. 
				 * The var EnableCommandRecd keeps a track of when the Enable 
				 * was recd from GUI. So it is set here.
				 */
				ucmsm_p->mThreeG_NM->ThreeGNMInfo.EnableCommandRecd = true;
				ucmsm_p->reset();
				if ( ucmsm_p->mThreeG_NM->EnableThreeGNM() == true)
				{
					ucmsm_p->transit(ucmsm_p->enabledState);
				}
			}
		}
		break;
		case  UCONN_CMD_LTE_4G_ONOFF:
		{
			UConfigEnable4GEvent *cfg4Gevt = (UConfigEnable4GEvent *) evt;
			if(cfg4Gevt->cfgenable4g == ON_4G){
				if((ucmsm_p->mLTE_NM->mNMInfo.system_notification == NOTI_DEVICE_OPEN)||
					(ucmsm_p->mLTE_NM->mNMInfo.system_notification == NOTI_DEVICE_CONNECTED)){
						ucmsm_p->transit(ucmsm_p->enabledState);
				}
			}

		}
		break;
		case UCONN_EVT_TIMEOUT:
			{
				UTimeOutEvent* timeoutEvt = (UTimeOutEvent*)evt;
				if(timeoutEvt->getTimerId() == ucmsm_p->deviceStateCheckTimer)
				{
#if 0
					if(ucmsm_p->mLTE_NM->mCommonInfo.com_port.length() > 3)
					{
						DAL_STATUS InitStatus = ucmsm_p->getDal()->uconnInit();
						if(InitStatus == UCM_SUCCESS)
						{
							/* OpenCmd success may be enough to start the CM */
							ucmsm_p->mLTE_NM->mCommonInfo.device_status = 1;
							LOG(ULOG_DEBUG_LEVEL,"Dal Init called sucessfuly\n");
							ucmsm_p->transit(ucmsm_p->enabledState);
						}
					}
#endif
#if 0
					// Check if device is present, If Yes then Call the uConnInit
					if(ucmsm_p->getDal()->isDevicePresent())
					{
						DAL_STATUS InitStatus = ucmsm_p->getDal()->uconnInit();
						if(InitStatus == UCM_SUCCESS)
						{
							/* OpenCmd success may be enough to start the CM */
							ucmsm_p->mLTE_NM->mCommonInfo.device_status = 1;
							LOG(ULOG_DEBUG_LEVEL,"Dal Init called sucessfuly\n");
							if(ucmsm_p->deviceStateCheckTimer)
							{
								UKillTimer(ucmsm_p->deviceStateCheckTimer);
								ucmsm_p->deviceStateCheckTimer = NULL;
							}
						}
						else
						{
							ucmsm_p->mLTE_NM->mCommonInfo.device_status = 0;
							ucmsm_p->mLTE_NM->mCommonInfo.com_port.clear();
							LOG(ULOG_ERROR_LEVEL,"Init Device failed.. exiting...\n");
						}
					}
					else
					{
						ucmsm_p->mLTE_NM->mCommonInfo.device_status = 0;
						ucmsm_p->mLTE_NM->mCommonInfo.com_port.clear();
					}
#endif

				}
			}
			break;

		default:
		{
			LOG(ULOG_ERROR_LEVEL, "Disabled State - command not executed \n");
		}
	}

	return 0;
}

/***********************************************************************
*	UCMDisabledState::exit() -
*	Disabled State Exit point
***********************************************************************/

void UCMDisabledState::exit()
{
	LOG(ULOG_ERROR_LEVEL, "Disabled State Exit \n");
	/* Kill the Update Timer on exit */
	UCMStateMachine* ucmsm_p = (UCMStateMachine*)this->getStateMachine();
	if(ucmsm_p->deviceStateCheckTimer)
	{
		UKillTimer(ucmsm_p->deviceStateCheckTimer);
		ucmsm_p->deviceStateCheckTimer = NULL;
	}
}
