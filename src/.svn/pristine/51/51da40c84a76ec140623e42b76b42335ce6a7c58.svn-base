#include <stdio.h>
#include "uconn_Broadcom_DAL.h"
#include "uconn_ATHandler.h"
#include "UBroadcomParser.h"
#include  "uconn_server.h"

uconnBroadcomDal::uconnBroadcomDal(UNetworkManager* p_NM):uconnAtDal(p_NM)
	{
		bufferParser = new UBroadcomParser(p_NM);
	}

uconnBroadcomDal::~uconnBroadcomDal()
	{
		delete bufferParser;
	}

/***********************************************************************
*	uconnATpropIPIFStatus -
*	DAL API for Setting UP/DOWN the IP Status
***********************************************************************/
DAL_STATUS uconnBroadcomDal::uconnATpropIPIFStatus(U_UINT32 ctx_id,
								U_IP_IF_STATUS IF_Status)
{
	U_STRING atcmd;
	U_SINT8 temp_str[10];

	/* Form the AT command string */
	atcmd.append("AT*IPIFSTATUS=");

	/* Append the CTX ID Command */
	ITOA(ctx_id, &temp_str[0], 10);
	atcmd.append(temp_str);
	atcmd.append(",");

	/* Append the IF Status Command */
	ITOA(IF_Status, &temp_str[0], 10);
	atcmd.append(temp_str);

	atcmd += CR;

	/* Send this AT Command to the device */
	//Emulator_For_AT_Commands(atcmd);
	m_pNM->mATHandler->sendATCommand(atcmd.c_str(),atcmd.size());

	return DAL_SUCCESS;
}

/***********************************************************************
*	uconnIMSConfig -
*	DAL API for the IMS configuration
*
*	Parameters
*	cfgType - Configuration type 
*	cfgValue - Config value
***********************************************************************/
DAL_STATUS	uconnBroadcomDal::uconnIMSConfig(U_UINT32 cfgType,
						   U_UINT32 cfgValue)
{

	U_STRING atcmd;
	U_SINT8 temp_str[10];
	
	/* AT+MCNFG is the command for EPS Regn events */
	atcmd.clear();
	atcmd.append("AT*MIMSCFG=");

	/* Configuration type */
	ITOA(cfgType, &temp_str[0], 10);
	atcmd.append(temp_str);
	atcmd.append(",");

	/* Context ID */
	ITOA(cfgValue, &temp_str[0], 10);
	atcmd.append(temp_str);

	atcmd += CR;
	
	/* Send this atcmd to the device */
	m_pNM->mATHandler->sendATCommand(atcmd.c_str(),atcmd.size());

	return DAL_SUCCESS;
}


/***********************************************************************
*	uconnIMSResponse -
*	DAL API for the IMS Response configuration
*
*	Parameters
*	cfgType - Configuration type 
*	cfgValue - Config value
***********************************************************************/
DAL_STATUS	uconnBroadcomDal::uconnIMSResponse(U_UINT32 rspMode)
{

	U_STRING atcmd;
	U_SINT8 temp_str[10];
	
	/* AT+MCNFG is the command for EPS Regn events */
	atcmd.clear();
	atcmd.append("AT*MIMSRSP=");

	/* Configuration type */
	ITOA(rspMode, &temp_str[0], 10);
	atcmd.append(temp_str);
	atcmd += CR;
	
	/* Send this atcmd to the device */
	m_pNM->mATHandler->sendATCommand(atcmd.c_str(),atcmd.size());

	return DAL_SUCCESS;
}
	



void uconnBroadcomDal::processBufferParserSetEvent(string strResp,unsigned int cmdId)
{
	//printf("inside broadcomDalcallback\n");

	
	/* Create the Event, Post it and then pass it on to the client */
	
	//rspEvt = new UEvent(bufferParser->getEventID());
	
	switch( bufferParser->getEventID() )
	{
		case UCONN_EVT_LTE_MODEM_STATISTIC:
			{
				/*
				if(bufferParser->parsedInfo.stat.uiGenericStatsLength)
				{
					memcpy(&m_pLTE_NM->mNMInfo.modemStats.sGenericStats, 
						&bufferParser->parsedInfo.stat.sGenericStats, sizeof(UParsed_CMGenericStats_t));
				}
				else if(bufferParser->parsedInfo.stat.uiMacstatsLength)
				{
					memcpy(&m_pLTE_NM->mNMInfo.modemStats.sMacStats, 
						&bufferParser->parsedInfo.stat.sMacStats, sizeof(UParsed_CMMACStats_t));
				}	
				else if(bufferParser->parsedInfo.stat.uiPHYStatsLength)
				{
					memcpy(&m_pLTE_NM->mNMInfo.modemStats.sPHYStats, 
						&bufferParser->parsedInfo.stat.sPHYStats, sizeof(UParsed_CMPHYStats_t));
				}
				*/
				UEvent *evt = new UEvent(UCONN_EVT_STATS_RECEIVED);
				m_pLTE_NM->getNMServer()->MsgPost((U_PUINT8)evt);
			
			}
			 
			
			break;
		
		

		}
	

	uconnAtDal::processBufferParserSetEvent(strResp,cmdId);
}



DAL_STATUS uconnBroadcomDal::uconnGetStatistics(GET_STATISTICS getStats,void* param)
{

	DAL_STATUS result = DAL_ERROR;
    
	CM_Info_s* modemDetails = reinterpret_cast<CM_Info_s*>(param);

	
	


	switch(getStats) 
	{
		case STAT_GENERAL:
			result = uconnATStatsUpdate(E_STAT_GENERAL);
			break;
		case STAT_MAC:
			result = uconnATStatsUpdate(E_STAT_MAC);
			break;
		case STAT_PHY:
			result = uconnATStatsUpdate(E_STAT_PHY);
			break;
		case STAT_ALL:
			result = uconnATStatsUpdate(E_STAT_GENERAL);
			result = uconnATStatsUpdate(E_STAT_MAC);
			result = uconnATStatsUpdate(E_STAT_PHY);
			break;

	}

	return result;

}

DAL_STATUS uconnBroadcomDal::uconnSetStatus(SET_STATUS setState,void* params,int ctxId)
{
DAL_STATUS result = DAL_ERROR;

	switch(setState) 
	{
		
		case ATP_CTX_SET: 
			{
			U_PS_CTX_CFG ctx_config;
		
			result = uconnATPsCtxCfgSet(10, ctxId, E_CFG_PCO_MS, &ctx_config);
			}
			break;
		case IP_IF_STATUS_UP:
			result = uconnATpropIPIFStatus(ctxId, E_IP_IF_STATUS_UP);
			break;
		case IP_IF_STATUS_DOWN:
			result = uconnATpropIPIFStatus(ctxId, E_IP_IF_STATUS_DOWN);
			break;
		default:
			result= uconnAtDal::uconnSetStatus(setState,params);
			break;
	}

	return result;



}




DAL_STATUS uconnBroadcomDal::uconnGetVersionInfo()
{
	U_STRING atcmd;

	/* ATI2 is the command for Version Information */
	atcmd.clear();
	atcmd.append("ATI2");
	atcmd += CR;

	/* Send this atcmd to the device */
	m_pNM->mATHandler->sendATCommand(atcmd.c_str(),atcmd.size());

	return DAL_SUCCESS;
}


U_BOOL uconnBroadcomDal::isDevicePresent()
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

	retBool = ComPortFinder(m_pLTE_NM->mCommonInfo.com_port,
		m_pLTE_NM->mCommonInfo.hwIdCOMPort);

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


/***********************************************************************
*	uconnModemConfig -
*	DAL API for the Modem configuration
*
*	Parameters
*	mConfig - Configuration type value
*	ctxID	- Context ID
***********************************************************************/
DAL_STATUS	uconnBroadcomDal::uconnATModemConfig(U_UINT32 mConfig,
						   U_UINT32 ctxId)
{

	U_STRING atcmd;
	U_SINT8 temp_str[10];
	
	/* AT+MCNFG is the command for EPS Regn events */
	atcmd.clear();
	atcmd.append("AT*MCNFG=");

	/* Configuration type */
	ITOA(mConfig, &temp_str[0], 10);
	atcmd.append(temp_str);
	atcmd.append(",");

	/* Context ID */
	ITOA(ctxId, &temp_str[0], 10);
	atcmd.append(temp_str);

	atcmd += CR;
	
	/* Send this atcmd to the device */
	m_pNM->mATHandler->sendATCommand(atcmd.c_str(),atcmd.size());

	return DAL_SUCCESS;
}


DAL_STATUS uconnBroadcomDal::uconnReset(void* param)
{	
	U_APN_TABLE* apn_table = reinterpret_cast<U_APN_TABLE*>(param);
	uconnATpropAPNTableDelete(apn_table);
	return DAL_SUCCESS;

}

DAL_STATUS uconnBroadcomDal::uconnConfigure(CONFIG_CODE configCode) 
{
	DAL_STATUS result;
	switch(configCode)
	{
	case UConnConfig::INIT_CONFIG:
		{
			result = uconnPowerUp();
			result = uconnATEPSRegn(CGEREP_ENABLE);
			result = uconnATNetStatusSet(CEREG_NW_REG_ENABLE);
			result = uconnGetVersionInfo();
			break;		
		}
			case UConnConfig::SET_CONTEXT:
			{
			U_UINT32 ctxID = uconn_GetCtxID();


			ctxID = 1;
			/* Set the Network configuration */
			U_UINT32 data;
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

			U_PS_CTX_PRIMARY_PARAMS pri_params;
			pri_params.u32Bitfields = 0x17;
			pri_params.pdp_type = E_PDP_TYPE_IPV4V6;
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
			//result = uconnConfigNetParams(PRIMARY_CTX,(void*)&pri_params);

			if(result) 
			{
				result = ERROR_NET_CFG_SET;
				LOG(ULOG_ERROR_LEVEL, "uconnATNetCfgSet failed \n");
				break;
			}

			/* For the Auto mode settings */
#if 0
			U_PS_CTX_PRIMARY_PARAMS pri_params;
			pri_params.u32Bitfields = 0x17;
			pri_params.pdp_type = E_PDP_TYPE_IPV4;
			strcpy((U_SINT8*)pri_params.apn, "TestApn");
			strcpy((U_SINT8*)pri_params.pdp_addr, "192.168.0.1");
			pri_params.data_comp = E_DATA_COMP_V42;
			pri_params.hdr_comp = E_HDR_COMP_ROHC;

			ctxID = 1;			/* ONLY FOR INTEGRATION TESTING */
			result = uconnATPsCtxDefPrimary(ctxID, &pri_params);
			if(result)
			{
				result = ERROR_PRIMARY_INIT;
				LOG(ULOG_ERROR_LEVEL, "uconnATPsCtxDefPrimary failed \n");
				return;
			}
#endif 

			/* PCO Configuration API */
			result = uconnSetStatus(ATP_CTX_SET,NULL);
			if(result)
			{
				result = ERROR_CTX_CFG_SET;
				LOG(ULOG_ERROR_LEVEL, "uconnATPsCtxCfgSet failed \n");
				break;
			}

			/* QOS configuration API */

			//result = uconnATPsCtxCfgSet(10, ctxID, E_CFG_QOS_4G, &ctx_config);


			}
			break;

		case UConnConfig::IMS_CONFIG:
			result = uconnIMSConfig(IMSCFG_SMS_OVER_IMS, m_pLTE_NM->mCommonInfo.imsParams.Sms_Over_Ims);
            result = uconnIMSConfig(IMSCFG_T1_TIMER, m_pLTE_NM->mCommonInfo.imsParams.T1_timer);
            result = uconnIMSConfig(IMSCFG_T2_TIMER, m_pLTE_NM->mCommonInfo.imsParams.T2_timer);
            result = uconnIMSConfig(IMSCFG_TF_TIMER, m_pLTE_NM->mCommonInfo.imsParams.TF_timer);
            //result = uconnIMSConfig(IMSCFG_SMS_FORMAT, m_pLTE_NM->mCommonInfo.imsParams.Sms_format);
            result = uconnIMSConfig(IMSCFG_IMS_REGN, IMS_REGN_REGISTER);
            result = uconnIMSConfig(IMSCFG_TEST_MODE, m_pLTE_NM->mCommonInfo.imsParams.Test_Mode);
			break;

		case UConnConfig::IMS_CONFIG_TEST:
			result = uconnIMSConfig(IMSCFG_TEST_MODE, TEST_MODE_ENABLE);
			break;

		case UConnConfig::IMS_CONFIG_DETACH:
			result = uconnIMSConfig(IMSCFG_IMS_REGN, IMS_REGN_DEREGISTER);
			break;

		case UConnConfig::IMS_RESPONSE:
			result = uconnIMSResponse(IMSEV_ENABLE);
			break;
		default:
			result = uconnAtDal::uconnConfigure(configCode);

	}
	return result;

}



/***********************************************************************
*	uconnATStatsUpdate -
*	DAL API for getting the Statistics from the LTE Stack
***********************************************************************/
DAL_STATUS uconnBroadcomDal::uconnATStatsUpdate(U_UINT32 statCmd)
{
	U_STRING atcmd;
	U_SINT8 temp_str[10];

	/* Form the AT command string */
	atcmd.append("AT*STAT=");

	/* Append the Stat Command */
	ITOA(statCmd, &temp_str[0], 10);
	atcmd.append(temp_str);

	atcmd += CR;

	/* Send this AT Command to the device */
	//Emulator_For_AT_Commands(atcmd);
	m_pNM->mATHandler->sendATCommand(atcmd.c_str(),atcmd.size());

	return DAL_SUCCESS;
}


/***********************************************************************
*	uconnATPsCtxCfgSet -
*	DAL API for configuring the PCO parameters
***********************************************************************/

DAL_STATUS uconnBroadcomDal::uconnATPsCtxCfgSet(U_UINT32 bit_fields,
							   U_UINT32 ctx_id,
							   U_PS_CTX_CFG_TYPE cfg_type,
							   U_PS_CTX_CFG *ctx_cfg)
{
	U_STRING atcmd;
	U_SINT8 temp_str[100];

	atcmd.clear();
	if(cfg_type == E_CFG_PCO_MS)
	{
		/* Form the AT command string */
		atcmd.append("AT*PCO=");

		ITOA(ctx_id, &temp_str[0], 10);
		atcmd.append(temp_str);

		/* Append the PCO Data for this command */

		atcmd += CR;
	}
	else if(cfg_type == E_CFG_QOS_4G)
	{
		/* Form the AT command string */
		atcmd.append("AT*CGEQOS=");

		ITOA(ctx_id, &temp_str[0], 10);
		atcmd.append(temp_str);
		atcmd.append(",");	
		
		/* Append the QOS Data for this command */
		/* QCI */
		ITOA(ctx_cfg->ps_ctx_qos_4g.qci, &temp_str[0], 10);
		atcmd.append(temp_str);
		atcmd.append(",");	

		/* DL_GBR */
		ITOA(ctx_cfg->ps_ctx_qos_4g.dl_gbr, &temp_str[0], 10);
		atcmd.append(temp_str);
		atcmd.append(",");	

		/* UL_GBR */
		ITOA(ctx_cfg->ps_ctx_qos_4g.ul_gbr, &temp_str[0], 10);
		atcmd.append(temp_str);
		atcmd.append(",");	

		/* DL_MBR */
		ITOA(ctx_cfg->ps_ctx_qos_4g.dl_mbr, &temp_str[0], 10);
		atcmd.append(temp_str);
		atcmd.append(",");	

		/* UL_MBR */
		ITOA(ctx_cfg->ps_ctx_qos_4g.ul_mbr, &temp_str[0], 10);
		atcmd.append(temp_str);

		atcmd += CR;
	}
	else if(cfg_type == E_CFG_TFT)
	{
		/* Form the AT command string */
		atcmd.append("AT*CGTFT=");

		ITOA(ctx_id, &temp_str[0], 10);
		atcmd.append(temp_str);

		/* Append the TFT Data for this command */

		atcmd += CR;
	}

	/* Send this AT Command to the device */
	//Emulator_For_AT_Commands(atcmd);
	m_pNM->mATHandler->sendATCommand(atcmd.c_str(),atcmd.size());

	return DAL_SUCCESS;
}




/***********************************************************************
*	uconnATpropAPNTableSet -
*	DAL API for setting the APN Table on the LTE Stack
***********************************************************************/
DAL_STATUS uconnBroadcomDal::uconnATpropAPNTableSet(U_APN_TABLE* apn_table)
{
	U_STRING atcmd;
	U_SINT8 temp_str[10];
	/* Form the AT command string */
	atcmd.append("AT*APN=");

	for(U_UINT32 i = 0; i < apn_table->PLMN_count; i++)
	{
		/* APN ID */
		ITOA(apn_table->plmn_search_entry[i].APN_id, &temp_str[0], 10);
		atcmd.append(temp_str);
		atcmd.append(",");

		/* Context ID */
		ITOA(apn_table->plmn_search_entry[i].cid, &temp_str[0], 10);
		atcmd.append(temp_str);
		atcmd.append(",");

		/* PDP Type */
		if(apn_table->plmn_search_entry[i].apn_type == E_PDP_TYPE_IPV4)
		{
			atcmd.append("\"IP\"");
		}
		else if(apn_table->plmn_search_entry[i].apn_type == E_PDP_TYPE_IPV6)
		{
			atcmd.append("\"IPV6\"");
		}
		else if(apn_table->plmn_search_entry[i].apn_type == E_PDP_TYPE_IPV4V6)
		{
			atcmd.append("\"IPv4v6\"");
		}
		atcmd.append(",");		

		/* APN Name */
		atcmd.append("\"");
		atcmd.append(apn_table->plmn_search_entry[i].apn);
		atcmd.append("\"");
		atcmd.append(",");

		/* MAC ADDRESS */
		atcmd.append("\"");
		atcmd.append(apn_table->plmn_search_entry[i].network_interface);
		atcmd.append("\"");
		atcmd.append(",");

		/* USB Endpoint */
		atcmd.append("\"");
		atcmd.append(apn_table->plmn_search_entry[i].usb_endpoint);
		atcmd.append("\"");
		atcmd.append(",");

		/* PDP Address */
		atcmd.append("\"");
		atcmd.append(apn_table->plmn_search_entry[i].source_addr);
		atcmd.append("\"");
		/* Inactivity Timer */
		if(m_pLTE_NM->mNMInfo.APN_Inactivity_timer > 0)//changes for apn
		{
			atcmd.append(",");
			ITOA(apn_table->plmn_search_entry[i].inactivity_timer, &temp_str[0], 10);
			atcmd.append(temp_str);
		}
	}
	atcmd += CR;

	/* Send this AT Command to the device */
	//Emulator_For_AT_Commands(atcmd);
	m_pNM->mATHandler->sendATCommand(atcmd.c_str(),atcmd.size());

	return DAL_SUCCESS;
}

/***********************************************************************
*	uconnATpropAPNTableGet -
*	DAL API for getting the APN Table on the LTE Stack
***********************************************************************/
DAL_STATUS uconnBroadcomDal::uconnATpropAPNTableGet()
{
	U_STRING atcmd;

	/* Form the AT command string */
	atcmd.append("AT*APN?");
	atcmd += CR;

	/* Send this AT Command to the device */
	//Emulator_For_AT_Commands(atcmd);
	//m_pNM->mATHandler->sendATCommand(atcmd.c_str(),atcmd.size());

	return DAL_SUCCESS;
}





/***********************************************************************
*	uconnIMSNWSupport -
*	DAL API for the IMS Network configuration
***********************************************************************/
DAL_STATUS	uconnBroadcomDal::uconnIMSNWSupport()
{

	U_STRING atcmd;
	
	/* AT+MCNFG is the command for EPS Regn events */
	atcmd.clear();
	atcmd.append("AT*MIMSNWSPRT");

	atcmd += CR;
	
	/* Send this atcmd to the device */
	m_pNM->mATHandler->sendATCommand(atcmd.c_str(),atcmd.size());

	return DAL_SUCCESS;
}

/***********************************************************************
*	uconnATpropAPNTableDelete -
*	DAL API for deleting an APN Table entry on the LTE Stack
***********************************************************************/
DAL_STATUS uconnBroadcomDal::uconnATpropAPNTableDelete(U_APN_TABLE* apn_table)
{
	U_STRING atcmd;
	U_SINT8 temp_str[10];
	/* Form the AT command string */
	atcmd.append("AT*APN=");

	for(U_UINT32 i = 0; i < apn_table->PLMN_count; i++)
	{
		/* APN ID */
		ITOA(apn_table->plmn_search_entry[i].APN_id, &temp_str[0], 10);
		atcmd.append(temp_str);
		atcmd.append(",");

		/* Context ID */
		ITOA(apn_table->plmn_search_entry[i].cid, &temp_str[0], 10);
		atcmd.append(temp_str);
		atcmd.append(",");

		/* PDP Type */
		if(apn_table->plmn_search_entry[i].apn_type == E_PDP_TYPE_IPV4)
		{
			atcmd.append("\"IP\"");
		}
		else if(apn_table->plmn_search_entry[i].apn_type == E_PDP_TYPE_IPV6)
		{
			atcmd.append("\"IPV6\"");
		}
		else if(apn_table->plmn_search_entry[i].apn_type == E_PDP_TYPE_IPV4V6)
		{
			atcmd.append("\"IPV4V6Virtual\"");
		}
		atcmd.append(",");		

		/* MAC ADDRESS */
		atcmd.append("\"");
		atcmd.append(apn_table->plmn_search_entry[i].network_interface);
		atcmd.append("\"");
		atcmd.append(",");

		/* USB Endpoint */
		atcmd.append("\"");
		atcmd.append(apn_table->plmn_search_entry[i].usb_endpoint);
		atcmd.append("\"");
		atcmd.append(",");

		/* PDP Address */
		atcmd.append("\"");
		atcmd.append(apn_table->plmn_search_entry[i].source_addr);
		atcmd.append("\"");
	}
	atcmd += CR;

	/* Send this AT Command to the device */
	//Emulator_For_AT_Commands(atcmd);
	m_pNM->mATHandler->sendATCommand(atcmd.c_str(),atcmd.size());

	return DAL_SUCCESS;
}





#if 1
DAL_STATUS uconnBroadcomDal::uconnConfigNetParams(NETWORK_PARAMS networkConfig,void* param,int ctxId)
{

  DAL_STATUS result = DAL_ERROR; 	

  switch(networkConfig)
  {
  case PCO_CONFIG:
	  {
		  
		  U_PS_CTX_CFG ctx_config;
	      result = uconnATPsCtxCfgSet(10,ctxId,E_CFG_PCO_MS, &ctx_config);
		  break;	
	  }
  case APN_CONFIG:
	  {
			UCMStateMachine* ucmsm_p = reinterpret_cast<UCMStateMachine*>(param);

			result = uconnATpropAPNTableSet(ucmsm_p->apn_table);
			break;

	  }
  case PRIMARY_CTX:
	  {
			U_PS_CTX_PRIMARY_PARAMS* pri_params = reinterpret_cast<U_PS_CTX_PRIMARY_PARAMS*>(param);	
			result = uconnATPsCtxDefPrimary(ctxId, pri_params);
			break;

	  }
  case CONTEXT_PARAMS:
	  {
	
	/* Set the Network configuration */
	U_UINT32 data;
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
	else
			{
				data = E_PDP_TYPE_IPV4V6;
					
			}
	
	U_PS_CTX_PRIMARY_PARAMS pri_params;
	pri_params.u32Bitfields = 0x17;
	pri_params.pdp_type = data;
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
		U_PS_CTX_CFG ctx_config;
		result = uconnATPsCtxCfgSet(10,ctxId,E_CFG_PCO_MS, &ctx_config);
		break;
	}
  default:
	  {
			result = uconnAtDal::uconnConfigNetParams(networkConfig,param,ctxId);
			break;
	  }
  }

		 return result;	

}

#endif



