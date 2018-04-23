#include <stdio.h>
#include "uconn_Altair_DAL.h"
#include "uconn_ATHandler.h"
#include "UAltairParser.h"
#include  "uconn_server.h"

uconnAltairDal::uconnAltairDal(UNetworkManager* p_NM):uconnAtDal(p_NM)
	{
		bufferParser = new UAltairParser(p_NM);
	}

uconnAltairDal::~uconnAltairDal()
	{
		delete bufferParser;

	}


void uconnAltairDal::processBufferParserSetEvent(string strResp,unsigned int cmdId)
{
	//printf("inside AltairDal callback\n");

	
	/* Create the Event, Post it and then pass it on to the client */
	
	//rspEvt = new UEvent(bufferParser->getEventID());
	
	switch( bufferParser->getEventID() )
	{
	case UCONN_EVT_STATS_RECEIVED:
		{
			UEvent* evt = new UEvent(UCONN_EVT_STATS_RECEIVED);
			m_pLTE_NM->getNMServer()->MsgPost((U_PUINT8)evt);
			return;

		}

	}
			
		
		
	

	uconnAtDal::processBufferParserSetEvent(strResp,cmdId);
}


DAL_STATUS	uconnAltairDal::uconnCheckConnStatus()
{
	U_STRING atcmd;
	
	/* AT+CGATT? is the command for enquiring the Connection Status */
	atcmd.clear();
	atcmd.append("AT+CGATT?");
	atcmd += CR;
	
	/* Send this atcmd to the device */
	m_pNM->mATHandler->sendATCommand(atcmd.c_str(),atcmd.size());

#if 0 
	/* AT%MEAS is the command for enquiring the Connection Status */
	atcmd.clear();
	atcmd.append("AT%MEAS=\"0\"");
	atcmd += CR;
	
	/* Send this atcmd to the device */
	m_pNM->mATHandler->sendATCommand(atcmd.c_str(),atcmd.size());

	atcmd.clear();
	atcmd.append("AT%MEAS=\"1\"");
	atcmd += CR;
	
	/* Send this atcmd to the device */
	m_pNM->mATHandler->sendATCommand(atcmd.c_str(),atcmd.size());

	atcmd.clear();
	atcmd.append("AT%MEAS=\"2\"");
	atcmd += CR;
	
	/* Send this atcmd to the device */
	m_pNM->mATHandler->sendATCommand(atcmd.c_str(),atcmd.size());
#endif
	return DAL_SUCCESS;
}

DAL_STATUS	uconnAltairDal::uconnInitInterface(string initStr)
{
	U_STRING atcmd;
	
	/* AT%STATUS= is the command to Initialize the interface */
	atcmd.clear();
	atcmd.append("AT%STATUS=");
	atcmd.append("\"");
	atcmd += initStr;
	atcmd.append("\"");
	atcmd += CR;

	/* Send this atcmd to the device */
	m_pNM->mATHandler->sendATCommand(atcmd.c_str(),atcmd.size());

	return DAL_SUCCESS;
}

DAL_STATUS	uconnAltairDal::uconnGetPINInfo()
{
	U_STRING atcmd;
	
	/* AT+COPS? is the command for enquiring the PLMN selection status */
	atcmd.clear();
	atcmd.append("AT%CPININFO");
	atcmd += CR;
	
	/* Send this atcmd to the device */
	m_pNM->mATHandler->sendATCommand(atcmd.c_str(),atcmd.size());

	return DAL_SUCCESS;
}


DAL_STATUS	uconnAltairDal::uconnReqStats(string initStr)
{
	U_STRING atcmd;
	
	/* AT%COUNT= is the command to obtain statistics */
	atcmd.clear();
	atcmd.append("AT%COUNT=");
	atcmd.append("\"");
	atcmd += initStr;
	atcmd.append("\"");
	atcmd += CR;

	/* Send this atcmd to the device */
	m_pNM->mATHandler->sendATCommand(atcmd.c_str(),atcmd.size());

	return DAL_SUCCESS;
}

U_BOOL uconnAltairDal::isDevicePresent()
{
	U_BOOL retBool = false;
	//DAL_STATUS ret = 0;
	U_SINT32 InitStatus= ERROR_DEVICE_NOT_OPEN;
	bool isCOMPortAvl = false;

	//if(gCMInfo.isEmulatorMode)
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


DAL_STATUS uconnAltairDal::uconnGetStatistics(GET_STATISTICS getStats,void* param)
{

	DAL_STATUS result = DAL_ERROR;
    
	switch(getStats) 
	{
		case STAT_ALL:

			
			result = uconnGetStatus(SIG_QUAL,(void*)&m_pLTE_NM->mNMInfo.rat);
			result = uconnGetStatus(GET_NET_STATUS,NULL); 

			
			
			result = uconnGetGenInfo();
			result = uconnSTAT_L1A();
			result = uconnSTAT_MEAS();
			break;
		case STAT_MEAS:
			{
				result = uconnSTAT_MEAS();
			}
			break;
	}

	return result;

}


DAL_STATUS uconnAltairDal::uconnSTAT_L1A()
{
	U_STRING atcmd;

	/* Form the AT command string */
	atcmd.clear();
	atcmd.append("AT%COUNT=\"L1A\"");
	atcmd += CR;

	/* Send this AT Command to the device */
	m_pNM->mATHandler->sendATCommand(atcmd.c_str(),atcmd.size());

	return DAL_SUCCESS;
}

DAL_STATUS uconnAltairDal::uconnSTAT_MEAS()
{
	U_STRING atcmd;

	/* Form the AT command string */
	atcmd.clear();
	atcmd.append("AT%MEAS=\"0\"");
	atcmd += CR;

	/* Send this AT Command to the device */
	m_pNM->mATHandler->sendATCommand(atcmd.c_str(),atcmd.size());


	/* Form the AT command string */
	atcmd.clear();
	atcmd.append("AT%MEAS=\"1\"");
	atcmd += CR;

	/* Send this AT Command to the device */
	m_pNM->mATHandler->sendATCommand(atcmd.c_str(),atcmd.size());


	/* Form the AT command string */
	atcmd.clear();
	atcmd.append("AT%MEAS=\"2\"");
	atcmd += CR;

	/* Send this AT Command to the device */
	m_pNM->mATHandler->sendATCommand(atcmd.c_str(),atcmd.size());


	return DAL_SUCCESS;
}


DAL_STATUS uconnAltairDal::uconnReset(void* param)
{	
	return DAL_SUCCESS;
}


/***********************************************************************
*	uconnModemConfig -
*	DAL API for the Modem configuration
*
*	Parameters
*	mConfig - Configuration type value
*	ctxID	- Context ID
***********************************************************************/
DAL_STATUS	uconnAltairDal::uconnATModemConfig(U_UINT32 mConfig,
						   U_UINT32 ctxId)
{
	return DAL_SUCCESS;
}



DAL_STATUS uconnAltairDal::uconnConfigNetParams(NETWORK_PARAMS networkConfig,void* param,int ctxId)
{
	DAL_STATUS result = DAL_ERROR;
	switch(networkConfig)
	{
		case PRIMARY_CTX:
		{
			U_PS_CTX_PRIMARY_PARAMS* pri_params = reinterpret_cast<U_PS_CTX_PRIMARY_PARAMS*>(param);	
			result = uconnATPsCtxDefPrimary(ctxId, pri_params);
			break;
		}
		case CONTEXT_PARAMS:
		{
			U_UINT32 data;
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

				result = uconnATPsCtxDefPrimary(ctxId, &pri_params);

				if(result) 
				{
					result = ERROR_NET_CFG_SET;
					LOG(ULOG_ERROR_LEVEL, "uconnATNetCfgSet failed \n");
					return false;
				}

			break;

		}
		

		default:
		{
			uconnAtDal::uconnConfigNetParams(networkConfig,param,ctxId);
			break;
		}
	}

	return result;	
}


DAL_STATUS uconnAltairDal::uconnConfigure(CONFIG_CODE configCode) {

	DAL_STATUS result = DAL_ERROR;
	switch(configCode) 
	{
		case UConnConfig::SET_FREQ:
			result = uconnSetFreq();
			break;

		case UConnConfig::ENABLED_STATE_ENTRY:
		{
			result = uconnATEPSRegn(CGEREP_ENABLE);
			result = uconnATNetStatusSet(CEREG_NW_REG_ENABLE);
			result = uconnGetVersionInfo();
			break;
		}

		default:
		{
			result = uconnAtDal::uconnConfigure(configCode);
			break;
		}
	}

	return result;

}


DAL_STATUS	uconnAltairDal::uconnSetFreq()
{
	if(m_pLTE_NM->mNMInfo.FreqEARCFN == -1)
	{
		//uconnAtDal::uconnPowerUp();
		return DAL_SUCCESS;
	}

	U_STRING atcmd;
	U_SINT8 temp_str[10];
	
	/* AT%EARFCN is the command for setting the DL Frequency */
	atcmd.clear();
	atcmd.append("AT%EARFCN=");

	/* Append the Frequency */
	ITOA(m_pLTE_NM->mNMInfo.FreqEARCFN, &temp_str[0], 10);
	atcmd.append(temp_str);

	atcmd += CR;
	
	/* Send this atcmd to the device */
	m_pNM->mATHandler->sendATCommand(atcmd.c_str(),atcmd.size());

	return DAL_SUCCESS;
}

DAL_STATUS uconnAltairDal::uconnGetVersionInfo()
{
	U_STRING atcmd = "ATI";
	atcmd += CR;
	m_pNM->mATHandler->sendATCommand(atcmd.c_str(),atcmd.size());

	atcmd.clear();


	atcmd = "AT%VER";
	atcmd += CR;
	m_pNM->mATHandler->sendATCommand(atcmd.c_str(),atcmd.size());

	return DAL_SUCCESS;

}

DAL_STATUS uconnAltairDal::uconnGetGenInfo()
{
	U_STRING atcmd = "AT%EARFCN?";
	atcmd += CR;
	m_pNM->mATHandler->sendATCommand(atcmd.c_str(),atcmd.size());

	atcmd.clear();

	atcmd += "AT%GETCFG=\"BAND\"";
	atcmd += CR;

	m_pNM->mATHandler->sendATCommand(atcmd.c_str(),atcmd.size());
    
	atcmd.clear();


	atcmd += "AT%PCONI=?";
	atcmd += CR;

	m_pNM->mATHandler->sendATCommand(atcmd.c_str(),atcmd.size());
    

	atcmd.clear();
	
	return DAL_SUCCESS;
}