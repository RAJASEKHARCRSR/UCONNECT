/*****************************************************************************
**
**  ucon_SAPIwrapper.cpp
**
**	This file implements the wrapper for calling the Server API's. If sockets
**	are used, then the calls are transplated to socket calls. The calls can
**	be blocking or no-blocking, based on the implementation of Server API's,
**	and this files implements no logic for the same.
**	The functions in this file will be called by the CLI as well as the 
**	GUI clients**
**
**	Author: Vasuki Setlur
**	Date  : 05-JAN-2011
**  (c) United Mobile Apps Pvt Ltd - (2010 - 2015)

	VER		  DATE		AUTHOR		DESCRIPTION
	0.1		05/01/11     VS			Creation
*****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <stdarg.h>
#include "UTypes.h"
#include "uconn_OSAL.h"
#include "uconn_SAPIwrapper.h"
#ifndef UCONN_CLIENT_USE_SOCKETS
#include "uconn_ServerAPI.h"
#endif /*UCONN_CLIENT_USE_SOCKETS*/
#include "ULog.h"
#include <map>
#ifdef QTPROJECT
#include <QDebug>
#endif  /* QTPROJECT */
#include <iostream>

static int statsRequestCount = 100;


typedef struct clientParams_s {
#ifdef UCONN_CLIENT_USE_SOCKETS
U_SOCKET gSocketHandle;
UEventQueue *gEvtQ;
U_UINT32 gExpectedTokenId;
#endif
U_VOID (*gEventHandler)(UEvent *event_p, U_SINT8 *data_p, U_VOID *param_p);
U_VOID *eventHdlrParam_p; /* passed by the client to use when calling event handler */
} clientParams_t;

static map<U_STRING,U_UINT32> setGetStringTable;
#define ADD_STRING_MAPPING(str,val)  setGetStringTable.insert ( pair<string,int>(str,val) );


static clientParams_t gCliParams;

#ifdef UCONN_CLIENT_USE_SOCKETS
static U_THREAD_RETURN_TYPE uconn_SAPIwrapProcessServerMessage (U_THREAD_ARG arg_p);
#endif

/* following are the handlers for the event and log callbacks */

/***********************************************************************
*	Returns a unique integer to be used as tokens
***********************************************************************/
static U_UINT32 uconn_GetMyToken ()
{
	static U_UINT32 tokenId = 0;

	/* token ID 0 will be reserved for events */
	((tokenId == 0xfff)? tokenId = 1: tokenId += 1);

	return tokenId;
}

/***********************************************************************
*	Initialises SAPI wrapper.
***********************************************************************/
uconnSAPIwrapErr_t uconn_SAPIwrapInit (U_VOID (*eventHandler)(UEvent *, U_SINT8*, U_VOID *),
									   U_VOID *evtHdlrParam_t)
{

	memset(&gCliParams, 0, sizeof(gCliParams));

#ifdef UCONN_CLIENT_USE_SOCKETS
	/* initialise the socket */
	if(init_client() != 0)
		return UCONN_SAPI_WRAP_FAILURE;
	if(!(gCliParams.gEvtQ = new UEventQueue(1)))
		return UCONN_SAPI_WRAP_FAILURE;
	gCliParams.gSocketHandle = INVALID_SOCKET;
#else
	SAPI_UC_Init(uconn_SAPIwrapEventCallback); //Asper at_command branch
#endif

	gCliParams.gEventHandler = eventHandler;
	gCliParams.eventHdlrParam_p = evtHdlrParam_t;
    setGetStringTable.clear();
    ADD_STRING_MAPPING("datarate",DATA_RATE);
    ADD_STRING_MAPPING("ul_datarate",UL_DATA);
    ADD_STRING_MAPPING("dl_datarate",DL_DATA);
    ADD_STRING_MAPPING("currentstate",CM_STATE);
    ADD_STRING_MAPPING("rssi_val",RSSI_VAL);
    ADD_STRING_MAPPING("cinr_val",CINR_VAL);
    ADD_STRING_MAPPING("roaminginfo",ROAMING_INFO);
    ADD_STRING_MAPPING("ipv4addr",IPv4_ADDR);
    ADD_STRING_MAPPING("ipv4submask",IPv4_SUBNET_MASK);
    ADD_STRING_MAPPING("ipv4gwaddr",IPv4_GW_ADDRESS);
	ADD_STRING_MAPPING("ipv6addr",IPv6_ADDR);
    ADD_STRING_MAPPING("ipv6submask",IPv6_SUBNET_MASK);
    ADD_STRING_MAPPING("ipv6gwaddr",IPv6_GW_ADDRESS);
	ADD_STRING_MAPPING("ipv4dns",IPv4_DNS);
	ADD_STRING_MAPPING("ipv6dns",IPv6_DNS);
	ADD_STRING_MAPPING("activationstatus",ACTIVATION_STATUS);
	ADD_STRING_MAPPING("modemstate", MODEM_STATE);
	ADD_STRING_MAPPING("system_noti", SYSTEM_NOTIFICATION);
    ADD_STRING_MAPPING("enable_4g",CFG_ENABLE_4G);
    ADD_STRING_MAPPING("configmode",CFG_CONFIG_MODE);
    ADD_STRING_MAPPING("attachrat",CFG_ATTACH_RAT);
    ADD_STRING_MAPPING("disablerate",CFG_DISABLE_RAT);
    ADD_STRING_MAPPING("logginglevel",CFG_LOGGING_LEVEL);
    ADD_STRING_MAPPING("enablelog",CFG_ENABLE_LOG);
    ADD_STRING_MAPPING("apn",APN_NAME);
	ADD_STRING_MAPPING("atmode",CFG_ATCOMMAND_MODE);

	return UCONN_SAPI_WRAP_SUCCESS;
}

/***********************************************************************
*	If the response from the server to a CLI command is a string, then
*	the string is created here in SAPI wrapper. This string is 
*	passed to the response agnostoc cliMain.c where it is just printed 
*	on the console.
***********************************************************************/
string printToString(const U_SINT8 * format,...)
{
	va_list argLst;
	U_SINT8 buffer[1000];

	va_start(argLst, format);
    vsnprintf(buffer,1000,format, argLst);
	va_end(argLst);

	string result(buffer);
	return result;
}


/***********************************************************************
*	Overloaded function SAPIExecute, either takes the commands from the
*	CLI or GUI, and passes it to the server. A thread listens to the
*	response from the server, and passes it on to the CLI/GUI. This 
*	thread also processes the Events and Logs from the client
***********************************************************************/
uconnSAPIstrResp_s* uconn_SAPIExecute (U_SINT32 cmdId,
									   U_SINT8 *cmdComponents[], 
									   U_SINT32 numberOfArgs)
{
    (void)numberOfArgs;
	UEvent *cmd = NULL;
	UEvent *rspCmd = NULL;
	uconnSAPIstrResp_s* sapiRsp	 = new uconnSAPIstrResp_s();
    sapiRsp->status = UCONN_SAPI_WRAP_FAILURE;

	switch(cmdId)
	{
		case UCONN_EVT_AT_CMD_REQUEST:
		{
			string s1 = *(cmdComponents);
			UATCmdRequestEvent *atCmdReqEvt = new UATCmdRequestEvent();
			atCmdReqEvt->setATReqCmdString(s1);
			cmd = reinterpret_cast<UEvent *>(atCmdReqEvt);
			break;
		}
		case UCONN_CMD_LTE_MOD_STATISTIC_REQ:
		{
			cmd = new UModemStatisticRequest();
			break;
		}
		case UCONN_CMD_LTE_NET_SEARCH:
		{
			if(isnumeric(cmdComponents[1]))
			{
				cmd = new UNetsearch((U_UINT32)atoi(cmdComponents[1]));
			}
			else if(!_stricmp("status", cmdComponents[1]))
			{
				cmd = new UNetsearch();
			}
			else
			{
				sapiRsp = new uconnSAPIstrResp_s();
				sapiRsp->status = UCONN_SAPI_WRAP_PARAMETER_ERROR;
				return sapiRsp;
			}
			break;
		}
		case UCONN_CMD_LTE_NET_ATTACH:
		{
			cmd = new UNetattach((U_UINT32)atoi(cmdComponents[1]));
			break;
		}
		case UCONN_CMD_LTE_NET_DETACH:
		{
			cmd = new UNetdetach();
			break;
		}
		case UCONN_CMD_SERVER_LOG_ONOFF:
		{
			if(!_stricmp(cmdComponents[1], "on"))
			{
				/* turn it on */
				cmd = new ULogOnOffEvent(1);
			}
			else
			{
				/* turn it off */
				cmd = new ULogOnOffEvent(0);
			}
			break;
		}
		case UCONN_CMD_SERVER_LOG_LEVEL:
		{
			if(!_stricmp(cmdComponents[2], "info"))
				cmd = new ULogLevelEvent(ULOG_INFO_LEVEL);
			else if (!_stricmp(cmdComponents[2], "debug"))
				cmd = new ULogLevelEvent(ULOG_DEBUG_LEVEL);
			else
				cmd = new ULogLevelEvent(ULOG_ERROR_LEVEL);

			break;
		}
		case UCONN_CMD_SERVER_LOG_GETSTATUS:
		{
			cmd = new UGetLogStatusEvent();
			break;
		}

		case UCONN_SAPI_WRAP_SETCFGPARAM:
            {
                U_UINT32 paramId = 0;
                if(isnumeric(cmdComponents[1]))
                {
                   paramId = atoi(cmdComponents[1]);
                }
                else
                {
                    map<U_STRING,U_UINT32>::iterator it;
                    it = setGetStringTable.find(cmdComponents[1]);
                    if(it != setGetStringTable.end())
                    {
                        paramId = it->second;
                    }
                    else
                    {
                            sapiRsp->status = UCONN_SAPI_WRAP_PARAMETER_ERROR;
                            sapiRsp->strResp = printToString("Invalid Parameter to Get");
                            return sapiRsp;
                     }
                }

                // Check if this parameter is Read Only
                if(IsReadOnly(paramId))
                {
                    sapiRsp->status = UCONN_SAPI_WRAP_PARAMETER_ERROR;
                    sapiRsp->strResp = printToString("Parameter is Read Only");
                    return sapiRsp;

                }
                // Do Range Check here
                //We mighty have to remove the range check to add MIB support
                //::TODO:: 

                if(MASK_ROBIT(paramId) >= LAST_PARAM_TYPE)
                {
                    sapiRsp->status = UCONN_SAPI_WRAP_PARAMETER_ERROR;
                    sapiRsp->strResp = printToString("Invalid Parameter to Set");
                    return sapiRsp;

                }

                U_UINT32 value = atoi(cmdComponents[2]);
                cmd = new USetCfgParam(paramId,value);
            }
            break;
		case UCONN_SAPI_WRAP_GETSTATS:
            {
                U_UINT32 paramId = 0;
                if(isnumeric(cmdComponents[1]))
                {
                   paramId = atoi(cmdComponents[1]);
                }
                else
                {
                    map<U_STRING,U_UINT32>::iterator it;
                    it = setGetStringTable.find(cmdComponents[1]);
                    if(it != setGetStringTable.end())
                    {
                        paramId = it->second;
                    }
                    else
                    {
                            sapiRsp->status = UCONN_SAPI_WRAP_PARAMETER_ERROR;
                            sapiRsp->strResp = printToString("Invalid Parameter to Get");
                            return sapiRsp;
                     }
                }
                // Do Range Check here

               if(MASK_ROBIT(paramId) >= LAST_PARAM_TYPE)
               {
                    sapiRsp->status = UCONN_SAPI_WRAP_PARAMETER_ERROR;
                    sapiRsp->strResp = printToString("Invalid Parameter to Get");
                    return sapiRsp;

                }
                cmd = new UGetCfgParam(paramId);
            }
			break;

		case UCONN_SAPI_WRAP_GET_FLT_REQ:
            {
                U_UINT32 paramId = 0;
                if(isnumeric(cmdComponents[1]))
                {
                   paramId = atoi(cmdComponents[1]);
                }
                else
                {
                    map<U_STRING,U_UINT32>::iterator it;
                    it = setGetStringTable.find(cmdComponents[1]);
                    if(it != setGetStringTable.end())
                    {
                        paramId = it->second;
                    }
                    else
                    {
                            sapiRsp->status = UCONN_SAPI_WRAP_PARAMETER_ERROR;
                            sapiRsp->strResp = printToString("Invalid Parameter to Get");
                            return sapiRsp;
                     }
                }
                // Do Range Check here

               if(MASK_ROBIT(paramId) >= LAST_PARAM_TYPE)
               {
                    sapiRsp->status = UCONN_SAPI_WRAP_PARAMETER_ERROR;
                    sapiRsp->strResp = printToString("Invalid Parameter to Get");
                    return sapiRsp;

                }
                cmd = new UGetfltCfgParam(paramId);
            }
			break;

		case UCONN_CMD_LTE_BRCM_STATISTIC_REQ:
            {
               cmd = new UBrcmStatisticRequest();
			   break;
			}
		
		case UCONN_SAPI_WRAP_GET_STRPARAM_REQ:
            {
                U_UINT32 paramId = 0;
                if(isnumeric(cmdComponents[1]))
                {
                   paramId = atoi(cmdComponents[1]);
                }
                else
                {
                    map<U_STRING,U_UINT32>::iterator it;
                    it = setGetStringTable.find(cmdComponents[1]);
                    if(it != setGetStringTable.end())
                    {
                        paramId = it->second;
                    }
                    else
                    {
                            sapiRsp->status = UCONN_SAPI_WRAP_PARAMETER_ERROR;
                            sapiRsp->strResp = printToString("Invalid Parameter to Get");
                            return sapiRsp;
                     }
                }

			
                // Do Range Check here

               if(MASK_ROBIT(paramId) >= LAST_PARAM_TYPE)
               {
                    sapiRsp->status = UCONN_SAPI_WRAP_PARAMETER_ERROR;
                    sapiRsp->strResp = printToString("Invalid Parameter to Get");
                    return sapiRsp;

                }
                cmd = new UGetStrCfgParam(paramId);
            }
            break;
	case UCONN_CMD_LTE_VERSION_INFO_REQ:
		{
			cmd = new UVersionInfoRequest();
			break;
		}

	case UCONN_CMD_BEARER_ENABLED:
		{
			cmd = new UBearerEnabled(atoi(cmdComponents[1]));
			break;
		}
	case UCONN_CMD_BEARER_DISABLED:
		{
			cmd = new UBearerDisabled(atoi(cmdComponents[1]));
			break;
		}
	case UCONN_CMD_BEARER_CONNECT:
		{
			cmd = new UBearerConnect(atoi(cmdComponents[1]));
			break;
		}
	case UCONN_CMD_BEARER_DISCONNECT:
		{
			cmd = new UBearerDisconnect(atoi(cmdComponents[1]));
			break;
		}
	case UCONN_CMD_BEARER_SET_PARAM:
		{
			string param = cmdComponents[3];
			Set_NM_Param paramId = (Set_NM_Param)(atoi(cmdComponents[2]));
			cmd = new UBearerStrParam(atoi(cmdComponents[1]),paramId,param);
			break;
		}
	/*case UCONN_CMD_BEARER_VPN_LIST_CLIENTS:
		{
			cmd = new UVPNListClients();
			break;
		}
	case UCONN_CMD_BEARER_VPN_LIST_PROFILES:
		{
			cmd = new UVPNListProfiles();
			break;
		}
	
	case UCONN_CMD_BEARER_VPN_START:
		{
			cmd = new UVPNStart();
			break;
		}
	case UCONN_CMD_BEARER_VPN_STOP:
		{
			cmd = new UVPNStop();
			break;
		}   */


		
		default:
		{
			sapiRsp->status = UCONN_SAPI_WRAP_FAILURE;
			return sapiRsp;
		}
	}

	if(cmd != NULL)
	{
		rspCmd =  uconn_SAPIExecute(cmd);
		delete cmd;
	}

	if(rspCmd != NULL)
	{
		switch(rspCmd->getCmdId())
		{
			case UCONN_SAPI_WRAP_NETSEARCH_RSP:
				{
					sapiRsp->status = UCONN_SAPI_WRAP_SUCCESS;
					sapiRsp->strResp = "";
					switch(rspCmd->getExecStatus())
					{
						case SRCH_COMPLETED:
						{
							sapiRsp->strResp += printToString("\t%10s%10s%10s%15s%20s\n", 
								"PLMN IDX", "PLMN ID", "RAT", "MEMBERSHIP", "DESCRIPTION");
							NetSearch_Result_s *searchStatus_p = &((UNetSearchRsp *)rspCmd)->nwSearchRsp;
							for(U_UINT32 i = 0; i < ((NetSearch_Result_s*)searchStatus_p)->num_plmns; i++)
							{
								/* Display the PLMN List information */
								sapiRsp->strResp += printToString("\t%10d%10d%10d%15d%20s\n", 
									i,
									((NetSearch_Result_s*)searchStatus_p)->nw_list[i].PLMN_id,
									((NetSearch_Result_s*)searchStatus_p)->nw_list[i].rat, 
									((NetSearch_Result_s*)searchStatus_p)->nw_list[i].list_membership,
								((NetSearch_Result_s*)searchStatus_p)->nw_list[i].free_text);
							}
						}
						break;

						case SRCH_INPROGRESS:
						{
							sapiRsp->strResp += printToString("\tSearch in Progress\n");
						}
						break;

						case SRCH_FAILED:
						{
							sapiRsp->strResp += printToString("\tSearch Failed\n");
						}
						break;

						case SRCH_NOTDONE:
						{
							sapiRsp->strResp += printToString("\tSearch Not Initiated\n");
						}
						break;
					}
					break;
				}
			case UCONN_SAPI_WRAP_GENERIC_RESP:
				{
                    sapiRsp->status = (uconnSAPIwrapErr_t)rspCmd->execStatus;
				}
				break;
			case UCONN_SAPI_WRAP_GET_RSP:
				{
					UGetCfgRsp *getRsp = (UGetCfgRsp*)rspCmd;
					if(getRsp->cfgParam == (U_UINT32)atoi(cmdComponents[1]))
					{
					//	printf("Got the Value of requested Stat = %d\n", 
					//		getRsp->cfgVal);
					}
					sapiRsp->status = UCONN_SAPI_WRAP_SUCCESS;
                   
                    map<U_STRING,U_UINT32>::iterator it;
                   U_STRING Rsp="";
                    for( it = setGetStringTable.begin() ; it != setGetStringTable.end() ; it++)
                   {
                       if(getRsp->cfgParam == it->second)
                       {
                           Rsp = it->first ;
                           break;
                       }
                   }
					/*if (getRsp->cfgParam == RSSI_VAL)
					{
                                                 sapiRsp->strResp = printToString(" Param %d  %s  Value = %lf\n",
							getRsp->cfgParam,Rsp.c_str(),getRsp->cfgVal);
					}
					else if(getRsp->cfgParam == CINR_VAL)
					{
						//U_DOUBLE Cinr = signedToFloat(getRsp->cfgVal,1,15,16);
                                                sapiRsp->strResp = printToString(" Param %d  %s  Value = %lf\n",
							getRsp->cfgParam,Rsp.c_str(),getRsp->cfgVal);
						
					}
					else
					{*/
						sapiRsp->strResp = printToString(" Param %d  %s  Value = %d\n",
							getRsp->cfgParam,Rsp.c_str(),getRsp->cfgVal);
					//}
					
				}
				break;

			case UCONN_SAPI_WRAP_GET_FLT_RSP:
				{
					UGetfltCfgRsp *getRsp = (UGetfltCfgRsp*)rspCmd;
					if(getRsp->cfgParam == (U_UINT32)atoi(cmdComponents[1]))
					{
					//	printf("Got the Value of requested Stat = %d\n", 
					//		getRsp->cfgVal);
					}
					sapiRsp->status = UCONN_SAPI_WRAP_SUCCESS;
                   
                    map<U_STRING,U_UINT32>::iterator it;
                   U_STRING Rsp="";
                    for( it = setGetStringTable.begin() ; it != setGetStringTable.end() ; it++)
                   {
                       if(getRsp->cfgParam == it->second)
                       {
                           Rsp = it->first ;
                           break;
                       }
                   }
					if (getRsp->cfgParam == RSSI_VAL)
					{
                                                 sapiRsp->strResp = printToString(" Param rssi value %d  %s  Value = %lf\n",
							getRsp->cfgParam,Rsp.c_str(),getRsp->cfgVal);
					}
					else if(getRsp->cfgParam == CINR_VAL)
					{
						//U_DOUBLE Cinr = signedToFloat(getRsp->cfgVal,1,15,16);
                                                sapiRsp->strResp = printToString(" Param cinr value %d  %s  Value = %lf\n",
							getRsp->cfgParam,Rsp.c_str(),getRsp->cfgVal);
						
					}
					else
					{
						sapiRsp->strResp = printToString(" Param %d  %s  Value = %d\n",
							getRsp->cfgParam,Rsp.c_str(),getRsp->cfgVal);
					}
					
				}
				break;
        case UCONN_SAPI_WRAP_GET_STRPARAM_RSP:
            {
                UGetStrCfgRsp *getRsp = (UGetStrCfgRsp*)rspCmd;
                if(getRsp->cfgParam == (U_UINT32)atoi(cmdComponents[1]))
                {
                //	printf("Got the Value of requested Stat = %d\n",
                //		getRsp->cfgVal);
                }
                sapiRsp->status = UCONN_SAPI_WRAP_SUCCESS;

                map<U_STRING,U_UINT32>::iterator it;
               U_STRING Rsp="";
                for( it = setGetStringTable.begin() ; it != setGetStringTable.end() ; it++)
               {
                   if(getRsp->cfgParam == it->second)
                   {
                       Rsp = it->first ;
                       break;
                   }
               }
                sapiRsp->strResp = printToString
                    (" Param %d  %s  Value = %s\n",
                    getRsp->cfgParam,Rsp.c_str(),getRsp->cfgString);
            }
            break;

			case UCONN_CMD_SERVER_LOG_STATUS_RESP:
				{
					UGetLogStatusRespEvent *statusRsp = (UGetLogStatusRespEvent*)rspCmd;
					sapiRsp->strResp = printToString("\tLog Status:%6s\n",
						(statusRsp->cfglogOnOff? "On": "Off"));
					if(statusRsp->cfglogLevel == ULOG_DEBUG_LEVEL)
						sapiRsp->strResp += printToString("\tLog Level:%6s\n", "Debug");
					else if(statusRsp->cfglogLevel == ULOG_INFO_LEVEL)
						sapiRsp->strResp += printToString("\tLog Level:%6s\n", "Info");
					else if(statusRsp->cfglogLevel == ULOG_ERROR_LEVEL)
						sapiRsp->strResp += printToString("\tLog Level:%6s\n", "Error");
					sapiRsp->status = UCONN_SAPI_WRAP_SUCCESS;
				}
				break;
			case UCONN_EVT_LTE_MODEM_STATISTIC:
				{
                                   
                    //UModemStatisticEvent *statRsp = (UModemStatisticEvent*)rspCmd;
 #if 0
					sapiRsp->strResp += printToString("Modem Statistics data\n\nGeneric Statistics\nu32DLThruput:%d\nu32ModemState:%d\nu32NumOfConnects:%d\nu32NumOfResync:%d\nu32NumPacketsRx:%d\nu32NumPacketsTx:%d\nu32ULThruput:%d\n", 
						statRsp->modemStatistic.sGenericStats.u32DLThruput,
						statRsp->modemStatistic.sGenericStats.u32ModemState,
						statRsp->modemStatistic.sGenericStats.u32NumOfConnects,
						statRsp->modemStatistic.sGenericStats.u32NumOfResync,
						statRsp->modemStatistic.sGenericStats.u32NumPacketsRx,
						statRsp->modemStatistic.sGenericStats.u32NumPacketsTx,
						statRsp->modemStatistic.sGenericStats.u32ULThruput);
						
						sapiRsp->strResp += printToString("\nMac Statistics\nsDLTBStats.u32NumErrTBs:%d\nsDLTBStats.u32NumTBs:%d\nsDLTBStats.u32PostHarqErr:%d\nsDLTBStats.u32PreHarqErr:%d\nsULTBStats.u32NumErrTBs:%d\nsULTBStats.u32NumTBs:%d\nsULTBStats.u32PostHarqErr:%d\nsULTBStats.u32PreHarqErr:%d\nu32CellReselectionCount:%d\nu32CRNTI:%d\nu32FailedSecurityActivationCount:%d\nu32FrameNumber:%d\nu32HandoverCount:%d\nu32IntegrityFailureCount:%d\nu32MeasReportsCount:%d\nu32NumDRBs:%d\nu32NumMacPdusRx:%d\nu32NumMacPdusTx:%d\nu32NumPRACH:%d\nu32NumPUCCHTransmissions:%d\nu32NumSRs:%d\nu32OutOfCoverageCount:%d\nu32PLMNId:%d\nu32RAFailureCount:%d\nu32RASuccessCount:%d\nu32SuccessfulSecurityActivationCount:%d\n",
						statRsp->modemStatistic.sMacStats.sDLTBStats.u32NumErrTBs,
						statRsp->modemStatistic.sMacStats.sDLTBStats.u32NumTBs,
                                                statRsp->modemStatistic.sMacStats.sDLTBStats.u32PostHarqPercentageErrRate,
                                                statRsp->modemStatistic.sMacStats.sDLTBStats.u32PreHarqPercentageErrRate,
                                                statRsp->modemStatistic.sMacStats.sULTBStats.u32NumReTxTBs,
						statRsp->modemStatistic.sMacStats.sULTBStats.u32NumTBs,
                                                //statRsp->modemStatistic.sMacStats.sULTBStats.u32PostHarqErr,
                                                //statRsp->modemStatistic.sMacStats.sULTBStats.u32PreHarqErr,
						statRsp->modemStatistic.sMacStats.u32CellReselectionCount,
						statRsp->modemStatistic.sMacStats.u32CRNTI,
						statRsp->modemStatistic.sMacStats.u32FailedSecurityActivationCount,
						statRsp->modemStatistic.sMacStats.u32FrameNumber,
						statRsp->modemStatistic.sMacStats.u32HandoverCount,
						statRsp->modemStatistic.sMacStats.u32IntegrityFailureCount,
						statRsp->modemStatistic.sMacStats.u32MeasReportsCount,
						statRsp->modemStatistic.sMacStats.u32NumDRBs,
						statRsp->modemStatistic.sMacStats.u32NumMacPdusRx,
						statRsp->modemStatistic.sMacStats.u32NumMacPdusTx,
						statRsp->modemStatistic.sMacStats.u32NumPRACH,
						statRsp->modemStatistic.sMacStats.u32NumPUCCHTransmissions,
						statRsp->modemStatistic.sMacStats.u32NumSRs,
						statRsp->modemStatistic.sMacStats.u32OutOfCoverageCount,
						statRsp->modemStatistic.sMacStats.u32PLMNId,
						statRsp->modemStatistic.sMacStats.u32RAFailureCount,
						statRsp->modemStatistic.sMacStats.u32RASuccessCount,
						statRsp->modemStatistic.sMacStats.u32SuccessfulSecurityActivationCount);
						
						sapiRsp->strResp += printToString("\nPHY Statistics\ns32RSRP:%d\ns32RSRQ:%d\ns32RssiAnt0:%d\ns32RssiAnt1:%d\ns32UlTxPower:%d\nu32BandType:%d\nu32BurstErrors:%d\nu32DLBw:%d\nu32DLEARFCN:%d\nu32NumDCIErrors:%d\nu32NumDCIs:%d\nu32NumPorts:%d\nu32PhyIdCell:%d\nu32ULBw:%d\nu32ULEARFCN\n",
						statRsp->modemStatistic.sPHYStats.s32RSRP,
						statRsp->modemStatistic.sPHYStats.s32RSRQ,
						statRsp->modemStatistic.sPHYStats.s32RssiAnt0,
						statRsp->modemStatistic.sPHYStats.s32RssiAnt1,
						statRsp->modemStatistic.sPHYStats.s32UlTxPower,
						statRsp->modemStatistic.sPHYStats.u32BandType,
                                                //statRsp->modemStatistic.sPHYStats.u32BurstErrors,
						statRsp->modemStatistic.sPHYStats.u32DLBw,
						statRsp->modemStatistic.sPHYStats.u32DLEARFCN,
                                                statRsp->modemStatistic.sPHYStats.u32TxMode,
                                                statRsp->modemStatistic.sPHYStats.s32DeviceCinr,
						statRsp->modemStatistic.sPHYStats.u32NumDCIs,
						statRsp->modemStatistic.sPHYStats.u32NumPorts,
						statRsp->modemStatistic.sPHYStats.u32PhyIdCell,
						statRsp->modemStatistic.sPHYStats.u32ULBw,
						statRsp->modemStatistic.sPHYStats.u32ULEARFCN
						);
						sapiRsp->status = UCONN_SAPI_WRAP_SUCCESS;
                                                #endif
				}
				break;
			case UCONN_EVT_LTE_BRCM_STATISTIC_RSP:
				{
					UBrcmStatisticEvent *statRsp = (UBrcmStatisticEvent*)rspCmd;
					//printf("%s",statRsp->broadcomStat.genStats);
					//printf("%s",statRsp->broadcomStat.macStats);
					//printf("%s",statRsp->broadcomStat.phyStats);
					sapiRsp->strResp += printToString("%s",statRsp->broadcomStat.genStats);
					sapiRsp->strResp += printToString("%s",statRsp->broadcomStat.macStats);
					sapiRsp->strResp += printToString("%s",statRsp->broadcomStat.phyStats);
					//int x=0;
					//sapiRsp->strResp += printToString("%d",x);
					sapiRsp->status = UCONN_SAPI_WRAP_SUCCESS;
					
				}
				break;
			case UCONN_CMD_LTE_VERSION_INFO_RSP:
				{
                                    UVersionInfoResponse *verRsp = (UVersionInfoResponse*)rspCmd;
                                    sapiRsp->status = UCONN_SAPI_WRAP_SUCCESS;
#if 0
                                    sapiRsp->strResp +=
                                            printToString("VERSION INFORMATION RESPONSE \n");
                                    sapiRsp->strResp += printToString("RFIC Version - %08x\n",
                                            verRsp->verInfo.u32RFICVer);
                                    sapiRsp->strResp += printToString("CHIP Version - %08x\n",
                                            verRsp->verInfo.u32ChipVer);
                                    sapiRsp->strResp += printToString("Flash Map Version - %d\n",
                                            verRsp->verInfo.u32FlasMapVer);
                                    sapiRsp->strResp += printToString("DSD Version - %d\n",
                                            verRsp->verInfo.u32DSDVer);
                                    sapiRsp->strResp += printToString("CAPI Version - %d.%d.%d\n",
                                            verRsp->verInfo.u32CAPIVer[0],
                                            verRsp->verInfo.u32CAPIVer[1],
                                            verRsp->verInfo.u32CAPIVer[2]);
                                    sapiRsp->strResp += printToString("RSA Version - %d.%d.%d\n",
                                            verRsp->verInfo.u32RSAVer[0],
                                            verRsp->verInfo.u32RSAVer[1],
                                            verRsp->verInfo.u32RSAVer[2]);
                                    sapiRsp->strResp += printToString("System Control CPU0 Version - %d.%d.%d\n",
                                            verRsp->verInfo.u32SySCntrlCPU0Ver[0],
                                            verRsp->verInfo.u32SySCntrlCPU0Ver[1],
                                            verRsp->verInfo.u32SySCntrlCPU0Ver[2]);
                                    sapiRsp->strResp += printToString("Firmware Version - %d.%d.%d\n",
                                            verRsp->verInfo.u32FWVer[0],
                                            verRsp->verInfo.u32FWVer[1],
                                            verRsp->verInfo.u32FWVer[2]);
                                    sapiRsp->strResp += printToString("DSD Phy Version - %d.%d.%d\n",
                                            verRsp->verInfo.u32DSPPhyVer[0],
                                            verRsp->verInfo.u32DSPPhyVer[1],
                                            verRsp->verInfo.u32DSPPhyVer[2]);
                                    sapiRsp->strResp += printToString("UBoot Version - %d.%d.%d\n",
                                            verRsp->verInfo.u32UbootVer[0],
                                            verRsp->verInfo.u32UbootVer[1],
                                            verRsp->verInfo.u32UbootVer[2]);
#endif /* if 0 */
                                    sapiRsp->strResp += printToString(verRsp->verInfo.u8VersionString);
                            }
                            break;
			default:
				{
					sapiRsp->status = UCONN_SAPI_WRAP_FAILURE;
				}
				break;
			}
		}
		if(rspCmd) 
			free(rspCmd);

		return sapiRsp;
}



#ifdef WIN32
U_VOID CALLBACK SAPIWrapperTimerCallBack(PVOID* lpParam , BOOLEAN )
#else
U_VOID SAPIWrapperTimerCallBack(U_PVOID* lpParam, U_BOOL TimerOrWaitFired)
#endif
{
    UTimeOutEvent *evt = new UTimeOutEvent((U_TIMERID)(*lpParam));
    #if 1
    gCliParams.gEvtQ->MsgQPost((U_PUINT8) evt);
    #endif
}

UEvent *uconn_SAPIExecute (UEvent *cmd)
{
		UEvent *rspCmd = NULL;

#ifdef UCONN_CLIENT_USE_SOCKETS
		U_SINT8 *netBuf_p = NULL;
		U_UINT32 tokenId = 0;

		if(!(netBuf_p = (U_SINT8*)calloc(1, cmd->getSize())))
		{
			rspCmd = new UGenResponse();
			rspCmd->setExecStatus(UCONN_SAPI_WRAP_FAILURE);
			return rspCmd;
		}

		tokenId = uconn_GetMyToken();
		cmd->setTokenId(tokenId);

		/* send this request to the server
		it is assumed here that the socket 
		connection has been established in handle_p */
		memcpy(netBuf_p, cmd, cmd->getSize());
		if(send_data(gCliParams.gSocketHandle, netBuf_p, cmd->getSize()) == -1)
		{
			rspCmd = new UGenResponse();
			rspCmd->setExecStatus(UCONN_SAPI_WRAP_FAILURE);
			return rspCmd;
		}
		free(netBuf_p);


         //#if defined(WIN32)
         U_TIMERID tid = USetTimer(3000, 0,(U_CALLBACK)SAPIWrapperTimerCallBack);
         //#endif

         #ifdef QTPROJECT 
                 //qDebug() << "Timer Started********************************* " << tid << " Token " << tokenId;
		 #endif

		/* Here we continuously read the socket to get the return value of the 
		function called */
//#if defined(WIN32)
		if(gCliParams.gEvtQ->MsgQGet((U_UINT8**)&rspCmd) != -1)
		{
            if(rspCmd->getCmdId() == UCONN_EVT_TIMEOUT){
				#ifdef QTPROJECT
                //QDebug() << "Msg Post Timed Out******************************* " << tid;
				#endif
                delete rspCmd;
                rspCmd = new UGenResponse();
                rspCmd->setExecStatus(UCONN_SAPI_WRAP_FAILURE);
            }
            else{
               //#ifdef WIN32
                UKillTimer(tid);
               //#endif
            }
			return rspCmd;
		}
//#endif /* Win32 */
#else /* UCONN_CLIENT_USE_SOCKETS */
            #if 0
		rspCmd = SAPI_UC_Execute(cmd);
            #endif
#endif /* UCONN_CLIENT_USE_SOCKETS */

		return rspCmd;
}

#ifdef UCONN_CLIENT_USE_SOCKETS
/***********************************************************************
*	Connect to the server. This function will open a socket connection
*	to the server. The socfd, will be used as the handle to communicate
*	all subsequent commands 
***********************************************************************/
uconnSAPIwrapErr_t uconn_SAPIwrapConnectToSever ( U_SINT8 *serverIp_p,
	U_UINT16 port)
{
		U_SINT8 tmpBuf[32];
		U_THREAD threadHandle;
    int handle_pthread;
    /* we dont want multiple calls to this function */
		if(gCliParams.gSocketHandle != INVALID_SOCKET)
			return UCONN_SAPI_WRAP_SUCCESS;

		/* Connect to the server. If Sockets are used, then handle_p
		points to the socket descriptor */
                #ifdef WIN32
                ITOA(port, tmpBuf, 10);
                #elif defined(__linux__) || defined(__MACH__)
                   sprintf(tmpBuf, "%d", port);
                #endif

		if((gCliParams.gSocketHandle = connect_to_server(serverIp_p, tmpBuf))
			== INVALID_SOCKET)
		{
			goto err;
		}
                printf("connection established\n");
		/* start the thread that listens for server messages, that
		could be command responses, events or logs */
                #ifdef WIN32
		if(!(threadHandle = 
                        spawn_thread(NULL,10000, (LPTHREAD_START_ROUTINE)uconn_SAPIwrapProcessServerMessage,
			(U_THREAD_ARG)NULL, 0, NULL)))
		{
			/* exit here - no way to run normally */
			printf("%s:%s - Thread creation failed\n", __FILE__, __FUNCTION__);
			uconn_SAPIwrapDisconnectFrmSever();
			goto err;
		}
		#elif defined(__linux__) || defined(__MACH__)
                handle_pthread  = pthread_create(&threadHandle,
                                             NULL,
	         			     uconn_SAPIwrapProcessServerMessage,
                                             (void*)NULL);
                printf("PThread creared : %d\n", handle_pthread);
                if(handle_pthread != 0)
                {
                    printf("Thread creation failed with error code : %d\n", handle_pthread);
                    goto err;
                }
		#endif

		return UCONN_SAPI_WRAP_SUCCESS;
err:
		return UCONN_SAPI_WRAP_FAILURE;
}

/***********************************************************************
*	Disconnect from the server
***********************************************************************/
U_VOID uconn_SAPIwrapDisconnectFrmSever ()
{
	disconnect_fromServer(gCliParams.gSocketHandle);
	gCliParams.gSocketHandle = INVALID_SOCKET;
	/* the thread that is listening to server messages should kill itself
	as the socket is closed. DO NOT ALLOW SERVER CONNECT UNTIL THE THREAD
	EXITS*/
}

//#define UCONN_SOCK_RD_BUF_LEN 2048
#define UCONN_SOCK_RD_BUF_LEN 4096
/***********************************************************************
*	The function (spawned as a thread) that listens for server events
*	If sockets are not used, this thread will not be spawned
***********************************************************************/
static U_THREAD_RETURN_TYPE uconn_SAPIwrapProcessServerMessage (U_THREAD_ARG )
{
    //printf("Entering CLIENT SIDE THREAD\n");
        U_SINT8 *rdBuf_p = NULL;
        U_SINT8 *tmpBuf_p = NULL;
        U_SINT32 tmpLen = 0, readLen = 0;
        UEvent *rspCmd = NULL;

        if(!(rdBuf_p = (U_SINT8*)calloc(UCONN_SOCK_RD_BUF_LEN, sizeof(U_SINT8))))
               #ifdef WIN32
               return -1;
               #elif defined(__linux__) || defined(__MACH__)
               pthread_exit(NULL);
               #endif
        tmpBuf_p = rdBuf_p;
        while(rcv_data(gCliParams.gSocketHandle, tmpBuf_p, &tmpLen,
               UCONN_SOCK_RD_BUF_LEN) == 0)
        {
#ifdef QTPROJECT
            //QDebug()<<"Received Packet with length" << tmpLen;
#endif /* QTPROJECT */
            readLen += tmpLen;
            if(readLen >= 16) /* get the header */
            {
                rspCmd = (UEvent *) rdBuf_p;





#ifdef QTPROJECT
                //QDebug() << " Cmd Received size" << rspCmd->getSize() << "  Token " << rspCmd->getTokenId();
#endif /* QTPROJECT */
                if(readLen == rspCmd->getSize())
                {
                    if(rspCmd->getTokenId())
                    {
                        /* this is the response to the command passed
                        this is handled by posting the response on the
                        message Q*/
                        if(gCliParams.gEvtQ->MsgQPost((U_UINT8*)rspCmd) == -1)
                        {
                            /* should never happen */
                            printf("posting to MesQ failed %s:%d\n", __FILE__, __LINE__);
#ifdef QTPROJECT
                            //QDebug()<<"Posting Msgq failed";
#endif /* QTPROJECT */
				#ifdef WIN32
                                return -1;
                               #elif defined(__linux__) || defined(__MACH__)
                                pthread_exit(NULL);
                                #endif
                        }
                     }
                     else
                     {
                        /* this is an asynchronous event/ log event */
                        uconn_SAPIwrapEventCallback(rspCmd);
#ifdef QTPROJECT
                        //QDebug() << "EVT Callback returned";
#endif /* QTPROJECT */
                     }
                     /* memory will be freed by the consumer, so allocate a new buffer */
                     if(!(rdBuf_p = (U_SINT8*)calloc(UCONN_SOCK_RD_BUF_LEN, sizeof(U_SINT8))))
                        #ifdef WIN32
                        return -1;
                        #elif defined(__linux__) || defined(__MACH__)
                        pthread_exit(NULL);
                        #endif
                     tmpBuf_p = rdBuf_p;
                     readLen = 0;
                     tmpLen = 0;
                     continue;
                 }
                 else if((signed int)rspCmd->getSize() > readLen)
                 {
                    /* This case shouldn't Ideally occur */
#ifdef QTPROJECT
                    //qDebug() << "Response cmd is Larger than ReadLen"
                    //        << "ReadLen : " << readLen
                    //        << "rspCmd->getSize() : " << rspCmd->getSize()
                    //        << endl;
#endif /* QTPROJECT */
                    free(rdBuf_p);
                    if(!(rdBuf_p = (U_SINT8*)calloc(UCONN_SOCK_RD_BUF_LEN, sizeof(U_SINT8))))
                        #ifdef WIN32
                        return -1;
                        #elif defined(__linux__) || defined(__MACH__)
                        pthread_exit(NULL);
                        #endif
                    tmpBuf_p = rdBuf_p;
                    readLen = 0;
                    tmpLen = 0;
                    continue;
                 }
                 else if(rspCmd->getSize() >=16)
                 {
                    U_SINT32 remaining = readLen;
                    U_UINT32 packet_size = 0;
                    U_SINT8* movRdBuf_p = rdBuf_p;
                    /* Here we read the first Packet and then check the recvd data
                    for another packet(s) */
                    while(remaining >= 16)
                    {
                        U_SINT8 *remBuf_p = NULL;
                        packet_size = rspCmd->getSize();
#ifdef QTPROJECT
                        //QDebug() << "uconn_SAPIwrapProcessServerMessage: Size mismatch "
                        //    << "readLen :" << readLen << "rspCmd->getSize()"
                         //   << rspCmd->getSize() << " Token" <<rspCmd->getTokenId();
#endif /* QTPROJECT */
                        if(!(remBuf_p = (U_SINT8*)calloc(packet_size, sizeof(U_SINT8))))
			    #ifdef WIN32
                            return -1;
                            #elif defined(__linux__) || defined(__MACH__)
                            pthread_exit(NULL);
                            #endif
                        memcpy(remBuf_p, movRdBuf_p, packet_size);
                        UEvent* remRspCmd = (UEvent *)remBuf_p;
                        movRdBuf_p += packet_size;
                        rspCmd = (UEvent*) movRdBuf_p;
                        if(remRspCmd->getTokenId())
                        {
                            /* this is the response to the command passed
                            this is handled by posting the response on the
                            message Q*/
                            if(gCliParams.gEvtQ->MsgQPost((U_UINT8*)remRspCmd) == -1)
                            {
                                /* should never happen */
                                printf("posting to MesQ failed %s:%d\n", __FILE__, __LINE__);
				#ifdef WIN32
                                return -1;
				#elif defined(__linux__) || defined(__MACH__)
				pthread_exit(NULL);
				#endif
                            }
                        }
                        else
                        {
                            /* this is an asynchronous event/ log event */
                            uconn_SAPIwrapEventCallback(remRspCmd);
                        }
                        remaining -= packet_size;
                        if((signed int)rspCmd->getSize() > remaining)
                        {

#ifdef QTPROJECT
                            qCritical()<<" ALARM ****************** HALF PACKET!!!!!!!!!!"
                                    <<rspCmd->getSize() << "REMAINING : "<< remaining;
#endif /* QTPROJECT */
                            if(!(tmpBuf_p = (U_SINT8*)calloc(UCONN_SOCK_RD_BUF_LEN+remaining, sizeof(U_SINT8))))
                                #ifdef WIN32
                                return -1;
                                #elif defined(__linux__) || defined(__MACH__)
                                pthread_exit(NULL);
                                #endif
                            memcpy(tmpBuf_p, movRdBuf_p, remaining);

                            free(rdBuf_p);
                            rdBuf_p = tmpBuf_p;
                            tmpBuf_p += remaining;
                            readLen = remaining;
                            tmpLen = 0;
                            break;

                        }
                    }
                    if(remaining < 16){
                        free(rdBuf_p);
                        if(!(rdBuf_p = (U_SINT8*)calloc(UCONN_SOCK_RD_BUF_LEN, sizeof(U_SINT8))))
                            #ifdef WIN32
                            return -1;
                            #elif defined(__linux__) || defined(__MACH__)
                            pthread_exit(NULL);
                            #endif
                        tmpBuf_p = rdBuf_p;
                        readLen = 0;
                        tmpLen = 0;
                    }
                }
                else
                {
#ifdef QTPROJECT
                    qCritical()<<" ALARM ****************** ZERO PACKET!!!!!!!!!!"
                            <<rspCmd->getSize();
#endif /* QTPROJECT */

                    free(rdBuf_p);
                    if(!(rdBuf_p = (U_SINT8*)calloc(UCONN_SOCK_RD_BUF_LEN, sizeof(U_SINT8))))
			#ifdef WIN32
                        return -1;
                        #elif defined(__linux__) || defined(__MACH__)
			pthread_exit(NULL);
                        #endif
                    tmpBuf_p = rdBuf_p;
                    readLen = 0;
                    tmpLen = 0;
                }
            }
            tmpBuf_p += tmpLen;
            tmpLen = 0;
        }
        /* the socket has been closed here. so exit the thread gracefully */
        gCliParams.gSocketHandle = INVALID_SOCKET;
        /* pass the disconnect data to the CLI/GUI */
        UServerDisconnect *disconnectEvent_p = new UServerDisconnect();
        if(disconnectEvent_p)
        {
               uconn_SAPIwrapEventCallback((UEvent*)disconnectEvent_p);
        }
	printf(" Exit Client side thread\n");
        return(0);
}

#endif /* UCONN_CLIENT_USE_SOCKETS */


/***********************************************************************
*	Handle the events from the server. If sockets are not used, then this
*	function will be directly called from the SAPI Layer of the server
*
*	Parameters
*	event_p - Event object
***********************************************************************/
U_VOID uconn_SAPIwrapEventCallback (UEvent *event_p)
{
	/* this function will be called only for the asynchronous
	notifications from the server. thses could be events or logs.
	all command responses will be taken care earlier than this 
	function is called. some events will be accompanied with data. 
	this data will be displayed on the cli screen for now */

	/* if log event then just call the function registerd to handle
	logs, else call the function registered to handle events. both
	CLI and GUI use this function. if sockets are used, then this 
	function is in a thread context, else it will be in the server
	context */

	gCliParams.gEventHandler(event_p , NULL, gCliParams.eventHdlrParam_p);
	free(event_p);
}

