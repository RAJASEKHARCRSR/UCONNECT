#include "UAltairParser.h"
#include "uconn_Event.h"
#include "ULog.h"

UAltairParser::UAltairParser(UNetworkManager* p_NM):UBufferParser(p_NM)
{
	
}


void UAltairParser::processUnSolResp(string &unSolicitedRespStr)
{
	setEventID(0);
	//cout << "The unsolicited events received:" << unSolicitedRespStr << endl;
	LOG(ULOG_DEBUG_LEVEL, "The unsolicited events received: %s", 
		unSolicitedRespStr.c_str()); 
	//printf("################# The EVT string received = %s #############\n", unSolicitedRespStr.c_str());
	if(strstr(unSolicitedRespStr.c_str(),"+CEREG:"))
	{
		vector<string> vStr;
		string tempStr = "";
		////cout << "Unsolicited Events:" << unSolicitedRespStr << endl;
		/* Check for +CEREG: and set the relevant Event */
		if(strstr(unSolicitedRespStr.c_str(),"+CEREG:"))
		{
			tokeniseInformation(unSolicitedRespStr,vStr,':');
			int size = vStr.size();
			if(size == 2)
			{
				if(strstr(vStr[1].c_str(),","))
				{
					tempStr.clear();
					tempStr = vStr[1];
					vStr.clear();
					tokeniseInformation(tempStr,vStr,',');
					int size = vStr.size();
					if(size >= 2)
					{
						tempStr.clear();
						tempStr = vStr[1];
						replaceAll(tempStr,"\"","");
						replaceAll(tempStr," ","");
						powerState = atoi(tempStr.c_str());
						m_pLTE_NM->mNMInfo.Modem_state = powerState;
						setEventID(UCONN_EVT_LTE_REG_STATUS);
					}
				}
				else
				{
					tempStr.clear();
					tempStr = vStr[1];
					replaceAll(tempStr,"\"","");
					replaceAll(tempStr," ","");
					powerState = atoi(tempStr.c_str());
					m_pLTE_NM->mNMInfo.Modem_state = powerState;
					setEventID(UCONN_EVT_LTE_REG_STATUS);
				}
			}
		}

		unSolicitedRespStr = "";
		return;
	}
 	else if(strstr(unSolicitedRespStr.c_str(),"NW detach"))
	{
		////cout << "Unsolicited Events:" << unSolicitedRespStr << endl;
		unSolicitedRespStr = "";
		return;
	}
	else if(strstr(unSolicitedRespStr.c_str(),"ME detach"))
	{
		////cout << "Unsolicited Events:" << unSolicitedRespStr << endl;
		unSolicitedRespStr = "";
		return;
	}
	else if(strstr(unSolicitedRespStr.c_str(),"ME PDN ACT"))
	{
		////cout << "Unsolicited Events:" << unSolicitedRespStr << endl;
		unSolicitedRespStr = "";
		parsedInfo.setModem_State(1);

		/* Perform the tasks on this response */
		setEventID(UCONN_EVT_PRIMARY_ACT);
		return;
	}
	else if(strstr(unSolicitedRespStr.c_str(),"NW ACT"))
	{
		////cout << "Unsolicited Events:" << unSolicitedRespStr << endl;
		unSolicitedRespStr = "";
		return;
	}
	else if(strstr(unSolicitedRespStr.c_str(),"NW DEACT"))
	{
		////cout << "Unsolicited Events:" << unSolicitedRespStr << endl;
		parsedInfo.setModem_State(0);
		setEventID(UCONN_EVT_NET_DETACH);
		unSolicitedRespStr = "";
		return;
	}
	else if(strstr(unSolicitedRespStr.c_str(),"ME DEACT"))
	{
		////cout << "Unsolicited Events:" << unSolicitedRespStr << endl;
		unSolicitedRespStr = "";
		parsedInfo.setModem_State(0);
		/* Perform the tasks on this response */
		setEventID(UCONN_EVT_NET_DETACH);
		return;
	}
	else if(strstr(unSolicitedRespStr.c_str(),"NW PDN DEACT"))
	{
		////cout << "Unsolicited Events:" << unSolicitedRespStr << endl;
		parsedInfo.setModem_State(0);
		setEventID(UCONN_EVT_NET_DETACH);
		unSolicitedRespStr = "";
		return;
	}
	else if(strstr(unSolicitedRespStr.c_str(),"ME PDN DEACT"))
	{
		////cout << "Unsolicited Events:" << unSolicitedRespStr << endl;
		parsedInfo.setModem_State(0);
		setEventID(UCONN_EVT_NET_DETACH);
		unSolicitedRespStr = "";
		return;
	}
	else if(strstr(unSolicitedRespStr.c_str(),"NW MODIFY"))
	{
		////cout << "Unsolicited Events:" << unSolicitedRespStr << endl;
		unSolicitedRespStr = "";
		return;
	}
	else if(strstr(unSolicitedRespStr.c_str(),"PDN ACT"))
	{
		unSolicitedRespStr = "";
		parsedInfo.setModem_State(1);

		//Perform the tasks on this response 
		setEventID(UCONN_EVT_PRIMARY_ACT);
		return;
	}
	else if(strstr(unSolicitedRespStr.c_str()," DEACT"))
	{
		//cout << "Unsolicited Events:" << unSolicitedRespStr << endl;
		parsedInfo.setModem_State(0);
		setEventID(UCONN_EVT_NET_DETACH);
		unSolicitedRespStr = "";
		return;
	} 
	else
	{
		UBufferParser::processUnSolResp(unSolicitedRespStr);
	}

	unSolicitedRespStr = "";
	return;
}




void UAltairParser::processAtCommandResp(string &atCmdRespStr)
{
	vector<string> vStr;
	string tmpStr = "";
	setEventID(0);
	//cout << "The Response for AT command received is:" << atCmdRespStr << endl;

	if(strstr(atCmdRespStr.c_str(),"RSRP:") || strstr(atCmdRespStr.c_str(),"RSRQ:") || strstr(atCmdRespStr.c_str(),"SINR:"))
	{
		////cout << "The Response for AT command received is:" << atCmdRespStr << endl;
		////cout << "The Response for AT command received is:such as RSRP or RSRQ or SINR"<< endl;
		tokeniseInformation(atCmdRespStr,vStr,',');
		//setEventID(UCONN_EVT_LTE_MODEM_L1A_STATISTIC);

		parse_Meas(vStr);

		if(strstr(atCmdRespStr.c_str(),"SINR:"))
		{
			setEventID(UCONN_EVT_STATS_RECEIVED);
		}
		
		
		

		
		vStr.clear();
	}
	else if(strstr(atCmdRespStr.c_str(),"L1A Stats:"))
	{
		////cout << "The Response for AT command received is:" << atCmdRespStr << endl;
		////cout << "The Response for AT command received is:L1A stats"<< endl;
		tokeniseInformation(atCmdRespStr,vStr,'\n');
		parse_L1A_STATs(vStr);
		
		//setEventID(UCONN_EVT_LTE_MODEM_L1A_STATISTIC);
		
		vStr.clear();
	}
	else if(strstr(atCmdRespStr.c_str(),"NAS Stats:"))
	{
		////cout << "The Response for AT command received is:" << atCmdRespStr << endl;
		tokeniseInformation(atCmdRespStr,vStr,'\n');
		setEventID(UCONN_EVT_LTE_MODEM_NAS_STATISTIC);
		
		//parse_NAS_STATs(vStr);
		vStr.clear();
	}
	else if(strstr(atCmdRespStr.c_str(),"RRC Stats:"))
	{
		////cout << "The Response for AT command received is:" << atCmdRespStr << endl;
		tokeniseInformation(atCmdRespStr,vStr,'\n');
		setEventID(UCONN_EVT_LTE_MODEM_RRC_STATISTIC);

		//parse_RRC_STATs(vStr);
		vStr.clear();
	}
	else if(strstr(atCmdRespStr.c_str(),"Timers Stats:"))
	{
		////cout << "The Response for AT command received is:" << atCmdRespStr << endl;
		tokeniseInformation(atCmdRespStr,vStr,'\n');
		setEventID(UCONN_EVT_LTE_MODEM_TIMERS_STATISTIC);

		//parse_Timer_STATs(vStr);
		vStr.clear();
	}
	
	
	else if(strstr(atCmdRespStr.c_str(),"+CSQ:"))	
	{
		////cout << "The Response for AT command received is:" << atCmdRespStr << endl;
		tokeniseInformation(atCmdRespStr,vStr,':');

		tmpStr.clear();
		tmpStr = vStr[1];
		vStr.clear();

		/*tokenise parameters here*/
		tokeniseInformation(tmpStr,vStr);

		if(vStr.size() <= 2)
		{
			parsedInfo.setRSSI(atoi(vStr[0].c_str()));
			m_pLTE_NM->mNMInfo.stats.rssi = atoi(vStr[0].c_str());
			m_pLTE_NM->mNMInfo.m_nAltairStoredStats.RSSI = atoi(vStr[0].c_str());
			//MInfo.stats.rssi = atoi(vStr[0].c_str());
			////cout << "RSSIValue is:" << parsedInfo.geRSSI() << endl;

			parsedInfo.setCINR(atoi(vStr[1].c_str()));
			m_pLTE_NM->mNMInfo.stats.cinr = atoi(vStr[1].c_str());
			m_pLTE_NM->mNMInfo.m_nAltairStoredStats.CINR= atoi(vStr[1].c_str());
			//MInfo.stats.cinr = atoi(vStr[1].c_str());
			////cout << "CINRValue is:" << parsedInfo.getCINR() << endl;

			setEventID(UCONN_CMD_NET_SIGQUAL_GET);
		}
		vStr.clear();
	}
	
	
	
	else if(strstr(atCmdRespStr.c_str(),"Manufacturer: "))
	{
		/*Parsing of ATI response here*/
        ////cout << "The Response for AT command received is:" << atCmdRespStr << endl;
		this->versionString = atCmdRespStr;
		
		vStr.clear();
	}

	else if(strstr(atCmdRespStr.c_str(),"SB Revision:"))
	{
		/*Parsing of ATI response here*/
        ////cout << "The Response for AT command received is:" << atCmdRespStr << endl;
		this->versionString += atCmdRespStr;
		replaceAll(this->versionString,"OK","");
		strncpy(parsedInfo.parsed_VersionInfo.u8VersionString,this->versionString.c_str(),this->versionString.size());
		parsedInfo.parsed_VersionInfo.u8VersionString[this->versionString.size()]='\0';
		this->versionString.clear();
		
		setEventID(UCONN_CMD_LTE_VERSION_INFO_RSP);	
		
		vStr.clear();
	}
	else if(strstr(atCmdRespStr.c_str(),"%EARFCN:"))
	{

		replaceAll(atCmdRespStr,"%EARFCN:","");
		replaceAll(atCmdRespStr,"OK","");
		replaceAll(atCmdRespStr,"\n","");

		StatsEncodeInfo statsEncodeInfo;
		
		statsEncodeInfo.statName = "DL_EARFCN";
		statsEncodeInfo.statValueStr = atCmdRespStr;
		statsEncodeInfo.encodedStats.level = 0;
		statsEncodeInfo.encodedStats.rangeFlag=0;
		statsEncodeInfo.encodedStats.valuePresent=1;
		statsEncodeInfo.encodedStats.reserved=0;
		

		string encodeStr = encodeStats(statsEncodeInfo);

		strncpy(m_pLTE_NM->mNMInfo.m_nMeasStats.dlEarfcn,encodeStr.c_str(),encodeStr.size());
		m_pLTE_NM->mNMInfo.m_nMeasStats.dlEarfcn[encodeStr.size()] = '\0';
		//cout <<"dl Earfcn is "<<m_pLTE_NM->mNMInfo.m_nMeasStats.dlEarfcn<<endl;

		vStr.clear();
	}
	
	else if(strstr(atCmdRespStr.c_str(),"%BANDCAP"))  // at command response for AT%BANDCAP=?
	{
		StatsEncodeInfo_s statsEncodeInfo;
		statsEncodeInfo.encodedStats.level=0;
		statsEncodeInfo.encodedStats.rangeFlag=0;
		statsEncodeInfo.encodedStats.reserved=0;
		statsEncodeInfo.encodedStats.valuePresent=1;
	
		statsEncodeInfo.statName = "BANDCAP";
		replaceAll(atCmdRespStr,"%BANDCAP:  ","");
		replaceAll(atCmdRespStr,"OK","");
		replaceAll(atCmdRespStr,"\n","");
		statsEncodeInfo.statValueStr=atCmdRespStr;
		string encodeStr = encodeStats(statsEncodeInfo);
		
		strncpy(m_pLTE_NM->mNMInfo.m_nMeasStats.bandcap,encodeStr.c_str(),encodeStr.size());
		m_pLTE_NM->mNMInfo.m_nMeasStats.bandcap[encodeStr.size()] = '\0';
		
    }
	else if(strstr(atCmdRespStr.c_str(),"Bands:"))  // at command response for AT%GETCFG="BAND"
	{
		StatsEncodeInfo_s statsEncodeInfo;
		statsEncodeInfo.encodedStats.level=0;
		statsEncodeInfo.encodedStats.rangeFlag=0;
		statsEncodeInfo.encodedStats.reserved=0;
		statsEncodeInfo.encodedStats.valuePresent=1;
	
		statsEncodeInfo.statName = "Bands";
		replaceAll(atCmdRespStr,"Bands: ","");
		replaceAll(atCmdRespStr,"OK","");
		replaceAll(atCmdRespStr,"\n","");
		statsEncodeInfo.statValueStr=atCmdRespStr;
		string encodeStr = encodeStats(statsEncodeInfo);
		
		strncpy(m_pLTE_NM->mNMInfo.m_nMeasStats.bands,encodeStr.c_str(),encodeStr.size());
		m_pLTE_NM->mNMInfo.m_nMeasStats.bands[encodeStr.size()] = '\0';
		
    }
	else if(strstr(atCmdRespStr.c_str(),"Bandwidth:"))  // at command response for AT%PCONI=?
	{
		StatsEncodeInfo_s statsEncodeInfo;
		statsEncodeInfo.encodedStats.level=0;
		statsEncodeInfo.encodedStats.rangeFlag=0;
		statsEncodeInfo.encodedStats.reserved=0;
		statsEncodeInfo.encodedStats.valuePresent=1;

		tokeniseInformation(atCmdRespStr,vStr,'\n');

		
		statsEncodeInfo.statName = "cell ID";
		statsEncodeInfo.statValueStr="N/A";
		


		string encodeStr = encodeStats(statsEncodeInfo);

		strncpy(m_pLTE_NM->mNMInfo.m_nMeasStats.cellID,encodeStr.c_str(),encodeStr.size());
		m_pLTE_NM->mNMInfo.m_nMeasStats.cellID[encodeStr.size()] = '\0';

		//cout <<"cell id info is"<<m_pLTE_NM->mNMInfo.m_nMeasStats.cellID<<endl;

		statsEncodeInfo.statName.clear();
		statsEncodeInfo.statValueStr.clear();

		encodeStr.clear();
		int vstrSize = vStr.size(); 
		for(int i=0;i<vstrSize;i++)
		{
			vector<string> statVector;
			tokeniseInformation(vStr[i],statVector,':');

			if(strstr(vStr[i].c_str(),"Bandwidth"))
			{
				statsEncodeInfo.statName = statVector[0];
				statsEncodeInfo.statValueStr = statVector[1];
				string encodeStr = encodeStats(statsEncodeInfo);
				strncpy(m_pLTE_NM->mNMInfo.m_nMeasStats.bandwidth,encodeStr.c_str(),encodeStr.size());
				m_pLTE_NM->mNMInfo.m_nMeasStats.bandwidth[encodeStr.size()] = '\0';

			}
			else if(strstr(vStr[i].c_str(),"cell ID"))
			{
				statsEncodeInfo.statName = statVector[0];
				statsEncodeInfo.statValueStr = statVector[1];
				string encodeStr = encodeStats(statsEncodeInfo);
				strncpy(m_pLTE_NM->mNMInfo.m_nMeasStats.cellID,encodeStr.c_str(),encodeStr.size());
				m_pLTE_NM->mNMInfo.m_nMeasStats.cellID[encodeStr.size()] = '\0';

			}
			

		}

	
        		
    }

	
	else
	{
		UBufferParser::processAtCommandResp(atCmdRespStr);
	}
}


void UAltairParser::parse_L1A_STATs(vector<std::string>& vStr)
{

	string encodeStr="";
	encodeStr.clear();
	

	StatsEncodeInfo_s statsEncodeInfo;

	statsEncodeInfo.encodedStats.level = 0;
	statsEncodeInfo.encodedStats.rangeFlag = 0;
	statsEncodeInfo.encodedStats.valuePresent = 0;
	statsEncodeInfo.encodedStats.reserved = 0;
	
	string tmpStr = "";
	string valStr = "";
	string stringToRemove = "() \"\r\n";
	string genStr = "";
	for(unsigned int i=0;i<(vStr.size()-1);++i)
	{
		iPos = 0;
		tmpStr.clear();
		valStr.clear();
		if( ((iPos = vStr[i].find(":",0)) != string::npos))
		{
			tmpStr.clear();
			tmpStr = vStr[i].substr(0,iPos);
			valStr = vStr[i].substr(iPos+1);
			replaceAll(valStr,"\n","");
			replaceAll(valStr," ","");
			replaceAll(valStr,"\r","");

			statsEncodeInfo.statName = tmpStr;
			statsEncodeInfo.statValueStr = valStr;
			
			if(strstr(tmpStr.c_str(),"L1A Stats"))
			{
				statsEncodeInfo.encodedStats.level = 0;
				statsEncodeInfo.encodedStats.valuePresent = 0;
				encodeStr += encodeStats(statsEncodeInfo);
			}

			else if(strstr(tmpStr.c_str(),"Received TBs per Redundancy Version") || 
				    strstr(tmpStr.c_str(),"Redundancy Version distribution"))
			{
				statsEncodeInfo.encodedStats.level = 1;
				statsEncodeInfo.encodedStats.valuePresent = 0;
				encodeStr += encodeStats(statsEncodeInfo);
				
				vector<string> vTmpStr;
				tokeniseInformation(valStr,vTmpStr,',');

				statsEncodeInfo.encodedStats.level = 2;
				statsEncodeInfo.encodedStats.valuePresent = 1;

				for(unsigned int i = 0 ; i < vTmpStr.size(); ++i)
				{
					string par1;
					string par2;
					if( ((iPos = vTmpStr[i].find("=",0)) != string::npos))
					{
						par1 = vTmpStr[i].substr(0,iPos);
						par2 = vTmpStr[i].substr(iPos+1);

						statsEncodeInfo.statName = par1;
						statsEncodeInfo.statValueStr = par2;


					
						if(strstr(par1.c_str(),"RV"))
						{
							encodeStr += encodeStats(statsEncodeInfo);
						}
					
					}

				}
					vTmpStr.clear();
			}


			else if(strstr(tmpStr.c_str(),"RX info per MCS(Tbs,Bytes,Crc)") || 
					strstr(tmpStr.c_str(),"RX info") || 
					strstr(tmpStr.c_str(),"TX info per MCS(Bytes,Grants,Acks, Nacks, Discards)") || 
					strstr(tmpStr.c_str(),"TX info"))
			{
				statsEncodeInfo.encodedStats.level = 1;
				statsEncodeInfo.encodedStats.valuePresent = 0;
				encodeStr += encodeStats(statsEncodeInfo);
			}
			else if(strstr(tmpStr.c_str(),"MCS "))
			{
				statsEncodeInfo.encodedStats.level = 2;
				statsEncodeInfo.encodedStats.valuePresent = 1;
				encodeStr += encodeStats(statsEncodeInfo);
			}
			else if(strlen(statsEncodeInfo.statName.c_str())>2)
			{
			
				statsEncodeInfo.encodedStats.level = 1;
				statsEncodeInfo.encodedStats.valuePresent = 1;
			 	encodeStr += encodeStats(statsEncodeInfo);
			}

			else if(strstr(vStr[i].c_str(),"") || strlen(vStr[i].c_str()) < 2)
			{

			}
			else if(strstr(vStr[i].c_str(),"OK") )
			{

			}
		}
	}
	vStr.clear();

	

	strncpy(m_pLTE_NM->mNMInfo.m_nL1AStats.L1AencodStr,encodeStr.c_str(),encodeStr.size());
	m_pLTE_NM->mNMInfo.m_nL1AStats.L1AencodStr[encodeStr.size()]='\0';
}

#if 0
void UAltairParser::parse_NAS_STATs(vector<std::string>& vStr)
{
	string tmpStr = "";
	string valStr = "";
	for(int i=3;i<vStr.size();++i)
	{
		iPos = 0;
		tmpStr.clear();
		valStr.clear();
		if( ((iPos = vStr[i].find(":",0)) != string::npos))
		{
			tmpStr.clear();
			tmpStr = vStr[i].substr(0,iPos);
			valStr = vStr[i].substr(iPos+1);
			if(strstr(tmpStr.c_str(),"attachAccept"))
			{
				m_pLTE_NM->mNMInfo.altairStats.NASStats.RxattachAccept = atoi(valStr.c_str());
				////cout << "attachAccept:" << m_pLTE_NM->mNMInfo.altairStats.NASStats.RxattachAccept << endl;
			}
			else if(strstr(tmpStr.c_str(),"attachReject"))
			{
				m_pLTE_NM->mNMInfo.altairStats.NASStats.RxattachReject = atoi(valStr.c_str());
				////cout << "attachReject:" << m_pLTE_NM->mNMInfo.altairStats.NASStats.RxattachReject << endl;
			}
			else if(strstr(tmpStr.c_str(),"authenticationRequest"))
			{
				m_pLTE_NM->mNMInfo.altairStats.NASStats.RxauthenticationRequest = atoi(valStr.c_str());
				////cout << "authenticationRequest:" << m_pLTE_NM->mNMInfo.altairStats.NASStats.RxauthenticationRequest << endl;
			}
			else if(strstr(tmpStr.c_str(),"authenticationReject"))
			{
				m_pLTE_NM->mNMInfo.altairStats.NASStats.RxauthenticationReject = atoi(valStr.c_str());
				////cout << "authenticationReject:" << m_pLTE_NM->mNMInfo.altairStats.NASStats.RxauthenticationReject << endl;
			}
			else if(strstr(tmpStr.c_str(),"identityRequest"))
			{
				m_pLTE_NM->mNMInfo.altairStats.NASStats.RxidentityRequest = atoi(valStr.c_str());
				////cout << "identityRequest:" << m_pLTE_NM->mNMInfo.altairStats.NASStats.RxidentityRequest << endl;
			}
			else if(strstr(tmpStr.c_str(),"gutiReallocationCommand"))
			{
				m_pLTE_NM->mNMInfo.altairStats.NASStats.RxgutiReallocationCommand = atoi(valStr.c_str());
				////cout << "gutiReallocationCommand:" << m_pLTE_NM->mNMInfo.altairStats.NASStats.RxgutiReallocationCommand << endl;
			}
			else if(strstr(tmpStr.c_str(),"securityModeCommand"))
			{
				m_pLTE_NM->mNMInfo.altairStats.NASStats.RxsecurityModeCommand = atoi(valStr.c_str());
				////cout << "securityModeCommand:" << m_pLTE_NM->mNMInfo.altairStats.NASStats.RxsecurityModeCommand << endl;
			}
			else if(strstr(tmpStr.c_str(),"detachRequest"))
			{
				m_pLTE_NM->mNMInfo.altairStats.NASStats.RxdetachRequest = atoi(valStr.c_str());
				////cout << "detachRequest:" << m_pLTE_NM->mNMInfo.altairStats.NASStats.RxdetachRequest << endl;
			}
			else if(strstr(tmpStr.c_str(),"detachAccept"))
			{
				m_pLTE_NM->mNMInfo.altairStats.NASStats.RxdetachAccept = atoi(valStr.c_str());
				////cout << "detachAccept:" << m_pLTE_NM->mNMInfo.altairStats.NASStats.RxdetachAccept << endl;
			}
			else if(strstr(tmpStr.c_str(),"serviceReject"))
			{
				m_pLTE_NM->mNMInfo.altairStats.NASStats.RxserviceReject = atoi(valStr.c_str());
				////cout << "serviceReject:" << m_pLTE_NM->mNMInfo.altairStats.NASStats.RxserviceReject << endl;
			}
			else if(strstr(tmpStr.c_str(),"taUpdateAccept"))
			{
				m_pLTE_NM->mNMInfo.altairStats.NASStats.RxtaUpdateAccept = atoi(valStr.c_str());
				////cout << "taUpdateAccept:" << m_pLTE_NM->mNMInfo.altairStats.NASStats.RxtaUpdateAccept << endl;
			}
			else if(strstr(tmpStr.c_str(),"taUpdateReject"))
			{
				m_pLTE_NM->mNMInfo.altairStats.NASStats.RxtaUpdateReject = atoi(valStr.c_str());
				////cout << "taUpdateReject:" << m_pLTE_NM->mNMInfo.altairStats.NASStats.RxtaUpdateReject << endl;
			}
			else if(strstr(tmpStr.c_str(),"emmInformation"))
			{
				m_pLTE_NM->mNMInfo.altairStats.NASStats.RxemmInformation = atoi(valStr.c_str());
				////cout << "emmInformation:" << m_pLTE_NM->mNMInfo.altairStats.NASStats.RxemmInformation << endl;
			}
			else if(strstr(tmpStr.c_str(),"emmStatus"))
			{
				m_pLTE_NM->mNMInfo.altairStats.NASStats.RxemmStatus = atoi(valStr.c_str());
				////cout << "emmStatus:" << m_pLTE_NM->mNMInfo.altairStats.NASStats.RxemmStatus << endl;
			}
			else if(strstr(tmpStr.c_str(),"activateDefaultEpsBearRequest"))
			{
				m_pLTE_NM->mNMInfo.altairStats.NASStats.RxactivateDefaultEpsBearRequest = atoi(valStr.c_str());
				////cout << "activateDefaultEpsBearRequest:" << m_pLTE_NM->mNMInfo.altairStats.NASStats.RxactivateDefaultEpsBearRequest << endl;
			}
			else if(strstr(tmpStr.c_str(),"activateDedicEpsBearRequest"))
			{
				m_pLTE_NM->mNMInfo.altairStats.NASStats.RxactivateDedicEpsBearRequest = atoi(valStr.c_str());
				////cout << "activateDedicEpsBearRequest:" << m_pLTE_NM->mNMInfo.altairStats.NASStats.RxactivateDedicEpsBearRequest << endl;
			}
			else if(strstr(tmpStr.c_str(),"deactivateEpsBearAccept"))
			{
				m_pLTE_NM->mNMInfo.altairStats.NASStats.RxdeactivateEpsBearAccept = atoi(valStr.c_str());
				////cout << "deactivateEpsBearAccept:" << m_pLTE_NM->mNMInfo.altairStats.NASStats.RxdeactivateEpsBearAccept << endl;
			}
			else if(strstr(tmpStr.c_str(),"modifyEpsBearRequest"))
			{
				m_pLTE_NM->mNMInfo.altairStats.NASStats.RxmodifyEpsBearRequest = atoi(valStr.c_str());
				////cout << "modifyEpsBearRequest:" << m_pLTE_NM->mNMInfo.altairStats.NASStats.RxmodifyEpsBearRequest << endl;
			}
			else if(strstr(tmpStr.c_str(),"esmStatus"))
			{
				m_pLTE_NM->mNMInfo.altairStats.NASStats.RxesmStatus = atoi(valStr.c_str());
				////cout << "esmStatus:" << m_pLTE_NM->mNMInfo.altairStats.NASStats.RxesmStatus << endl;
			}
			else if(strstr(tmpStr.c_str(),"esmInforamtionRequest"))
			{
				m_pLTE_NM->mNMInfo.altairStats.NASStats.RxesmInforamtionRequest = atoi(valStr.c_str());
				////cout << "esmInforamtionRequest:" << m_pLTE_NM->mNMInfo.altairStats.NASStats.RxesmInforamtionRequest << endl;
			}
			else if(strstr(tmpStr.c_str(),"pdnDisconnectReject"))
			{
				m_pLTE_NM->mNMInfo.altairStats.NASStats.RxpdnDisconnectReject = atoi(valStr.c_str());
				////cout << "pdnDisconnectReject:" << m_pLTE_NM->mNMInfo.altairStats.NASStats.RxpdnDisconnectReject << endl;
			}
			else if(strstr(tmpStr.c_str(),"pdnConnectReject"))
			{
				m_pLTE_NM->mNMInfo.altairStats.NASStats.RxpdnConnectReject = atoi(valStr.c_str());
				////cout << "pdnConnectReject:" << m_pLTE_NM->mNMInfo.altairStats.NASStats.RxpdnConnectReject << endl;
			}
			else if(strstr(tmpStr.c_str(),"modifyEpsBearReject"))
			{
				m_pLTE_NM->mNMInfo.altairStats.NASStats.RxmodifyEpsBearReject = atoi(valStr.c_str());
				////cout << "modifyEpsBearReject:" << m_pLTE_NM->mNMInfo.altairStats.NASStats.RxmodifyEpsBearReject << endl;
			}
			else if(strstr(tmpStr.c_str(),"attachComplete"))
			{
				m_pLTE_NM->mNMInfo.altairStats.NASStats.TxattachComplete = atoi(valStr.c_str());
				////cout << "attachComplete:" << m_pLTE_NM->mNMInfo.altairStats.NASStats.TxattachComplete << endl;
			}
			else if(strstr(tmpStr.c_str(),"attachRequest"))
			{
				m_pLTE_NM->mNMInfo.altairStats.NASStats.TxattachRequest = atoi(valStr.c_str());
				////cout << "attachRequest:" << m_pLTE_NM->mNMInfo.altairStats.NASStats.TxattachRequest << endl;
			}
			else if(strstr(tmpStr.c_str(),"authenticationFailure"))
			{
				m_pLTE_NM->mNMInfo.altairStats.NASStats.Txauthentication_Failure = atoi(valStr.c_str());
				////cout << "authenticationFailure:" << m_pLTE_NM->mNMInfo.altairStats.NASStats.Txauthentication_Failure << endl;
			}
			else if(strstr(tmpStr.c_str(),"authenticationResponse"))
			{
				m_pLTE_NM->mNMInfo.altairStats.NASStats.Txauthentication_Response = atoi(valStr.c_str());
				////cout << "authenticationResponse:" << m_pLTE_NM->mNMInfo.altairStats.NASStats.Txauthentication_Response << endl;
			}
			else if(strstr(tmpStr.c_str(),"detachRequest"))
			{
				m_pLTE_NM->mNMInfo.altairStats.NASStats.TxdetachRequest = atoi(valStr.c_str());
				////cout << "detachRequest:" << m_pLTE_NM->mNMInfo.altairStats.NASStats.TxdetachRequest << endl;
			}
			else if(strstr(tmpStr.c_str(),"detachAccept"))
			{
				m_pLTE_NM->mNMInfo.altairStats.NASStats.TxdetachAccept = atoi(valStr.c_str());
				////cout << "detachAccept:" << m_pLTE_NM->mNMInfo.altairStats.NASStats.TxdetachAccept << endl;
			}
			else if(strstr(tmpStr.c_str(),"emmStatus"))
			{
				m_pLTE_NM->mNMInfo.altairStats.NASStats.TxemmStatus = atoi(valStr.c_str());
				////cout << "emmStatus:" << m_pLTE_NM->mNMInfo.altairStats.NASStats.TxemmStatus << endl;
			}
			else if(strstr(tmpStr.c_str(),"gutiReallocationComplete"))
			{
				m_pLTE_NM->mNMInfo.altairStats.NASStats.TxgutiReallocationComplete = atoi(valStr.c_str());
				////cout << "gutiReallocationComplete:" << m_pLTE_NM->mNMInfo.altairStats.NASStats.TxgutiReallocationComplete << endl;
			}
			else if(strstr(tmpStr.c_str(),"identityResponse"))
			{
				m_pLTE_NM->mNMInfo.altairStats.NASStats.TxidentityResponse = atoi(valStr.c_str());
				////cout << "identityResponse:" << m_pLTE_NM->mNMInfo.altairStats.NASStats.TxidentityResponse << endl;
			}
			else if(strstr(tmpStr.c_str(),"securityModeReject"))
			{
				m_pLTE_NM->mNMInfo.altairStats.NASStats.TxsecurityModeReject = atoi(valStr.c_str());
				////cout << "securityModeReject:" << m_pLTE_NM->mNMInfo.altairStats.NASStats.TxsecurityModeReject << endl;
			}
			else if(strstr(tmpStr.c_str(),"serviceRequest"))
			{
				m_pLTE_NM->mNMInfo.altairStats.NASStats.TxserviceRequest = atoi(valStr.c_str());
				////cout << "serviceRequest:" << m_pLTE_NM->mNMInfo.altairStats.NASStats.TxserviceRequest << endl;
			}
			else if(strstr(tmpStr.c_str(),"taUpdateComplete"))
			{
				m_pLTE_NM->mNMInfo.altairStats.NASStats.TxtaUpdateComplete = atoi(valStr.c_str());
				////cout << "taUpdateComplete:" << m_pLTE_NM->mNMInfo.altairStats.NASStats.TxtaUpdateComplete << endl;
			}
			else if(strstr(tmpStr.c_str(),"taUpdateRequest"))
			{
				m_pLTE_NM->mNMInfo.altairStats.NASStats.TxtaUpdateRequest = atoi(valStr.c_str());
				////cout << "taUpdateRequest:" << m_pLTE_NM->mNMInfo.altairStats.NASStats.TxtaUpdateRequest << endl;
			}
			else if(strstr(tmpStr.c_str(),"ulNasTransport"))
			{
				m_pLTE_NM->mNMInfo.altairStats.NASStats.TxulNasTransport = atoi(valStr.c_str());
				////cout << "ulNasTransport:" << m_pLTE_NM->mNMInfo.altairStats.NASStats.TxulNasTransport << endl;
			}
			else if(strstr(tmpStr.c_str(),"activateDefaultEpsBearAccept"))
			{
				m_pLTE_NM->mNMInfo.altairStats.NASStats.TxactivateDefaultEpsBearAccept = atoi(valStr.c_str());
				////cout << "activateDefaultEpsBearAccept:" << m_pLTE_NM->mNMInfo.altairStats.NASStats.TxactivateDefaultEpsBearAccept << endl;
			}
			else if(strstr(tmpStr.c_str(),"activateDefaultEpsBearReject"))
			{
				m_pLTE_NM->mNMInfo.altairStats.NASStats.TxactivateDefaultEpsBearReject = atoi(valStr.c_str());
				////cout << "activateDefaultEpsBearReject:" << m_pLTE_NM->mNMInfo.altairStats.NASStats.TxactivateDefaultEpsBearReject << endl;
			}
			else if(strstr(tmpStr.c_str(),"activateDedicEpsBearAccept"))
			{
				m_pLTE_NM->mNMInfo.altairStats.NASStats.TxactivateDedicEpsBearAccept = atoi(valStr.c_str());
				////cout << "activateDedicEpsBearAccept:" << m_pLTE_NM->mNMInfo.altairStats.NASStats.TxactivateDedicEpsBearAccept << endl;
			}
			else if(strstr(tmpStr.c_str(),"activateDedicEpsBearReject"))
			{
				m_pLTE_NM->mNMInfo.altairStats.NASStats.TxactivateDedicEpsBearReject = atoi(valStr.c_str());
				////cout << "activateDedicEpsBearReject:" << m_pLTE_NM->mNMInfo.altairStats.NASStats.TxactivateDedicEpsBearReject << endl;
			}
			else if(strstr(tmpStr.c_str(),"deactivateEpsBearAccept"))
			{
				m_pLTE_NM->mNMInfo.altairStats.NASStats.TxdeactivateEpsBearAccept = atoi(valStr.c_str());
				////cout << "deactivateEpsBearAccept:" << m_pLTE_NM->mNMInfo.altairStats.NASStats.TxdeactivateEpsBearAccept << endl;
			}
			else if(strstr(tmpStr.c_str(),"bearerResourceModificationRequest"))
			{
				m_pLTE_NM->mNMInfo.altairStats.NASStats.TxbearerResourceModificationRequest = atoi(valStr.c_str());
				////cout << "bearerResourceModificationRequest:" << m_pLTE_NM->mNMInfo.altairStats.NASStats.TxbearerResourceModificationRequest << endl;
			}
			else if(strstr(tmpStr.c_str(),"esmInformationResponse"))
			{
				m_pLTE_NM->mNMInfo.altairStats.NASStats.TxesmInformationResponse = atoi(valStr.c_str());
				////cout << "esmInformationResponse:" << m_pLTE_NM->mNMInfo.altairStats.NASStats.TxesmInformationResponse << endl;
			}
			else if(strstr(tmpStr.c_str(),"ESM_Status"))
			{
				m_pLTE_NM->mNMInfo.altairStats.NASStats.TxESM_Status = atoi(valStr.c_str());
				////cout << "ESM_Status:" << m_pLTE_NM->mNMInfo.altairStats.NASStats.TxESM_Status << endl;
			}
			else if(strstr(tmpStr.c_str(),"modifyEpsBearerAccept"))
			{
				m_pLTE_NM->mNMInfo.altairStats.NASStats.TxmodifyEpsBearerAccept = atoi(valStr.c_str());
				////cout << "modifyEpsBearerAccept:" << m_pLTE_NM->mNMInfo.altairStats.NASStats.TxmodifyEpsBearerAccept << endl;
			}

			else if(strstr(tmpStr.c_str(),"modifyEpsBearerReject"))
			{
				m_pLTE_NM->mNMInfo.altairStats.NASStats.TxmodifyEpsBearerReject = atoi(valStr.c_str());
				////cout << "modifyEpsBearerReject:" << m_pLTE_NM->mNMInfo.altairStats.NASStats.TxmodifyEpsBearerReject << endl;
			}
			else if(strstr(tmpStr.c_str(),"pdnConnectRequest"))
			{
				m_pLTE_NM->mNMInfo.altairStats.NASStats.TxpdnConnectRequest = atoi(valStr.c_str());
				////cout << "pdnConnectRequest:" << m_pLTE_NM->mNMInfo.altairStats.NASStats.TxpdnConnectRequest << endl;
			}
			else if(strstr(tmpStr.c_str(),"pdnDisconnectRequest"))
			{
				m_pLTE_NM->mNMInfo.altairStats.NASStats.TxpdnDisconnectRequest = atoi(valStr.c_str());
				////cout << "pdnDisconnectRequest:" << m_pLTE_NM->mNMInfo.altairStats.NASStats.TxpdnDisconnectRequest << endl;
			}
			else if(strstr(tmpStr.c_str(),"bearResourceAllocationRequest"))
			{
				m_pLTE_NM->mNMInfo.altairStats.NASStats.TxbearResourceAllocationRequest = atoi(valStr.c_str());
				////cout << "bearResourceAllocationRequest:" << m_pLTE_NM->mNMInfo.altairStats.NASStats.TxbearResourceAllocationRequest << endl;
			}
		}
	}
}
#endif


#if 0
void UAltairParser::parse_RRC_STATs(vector<std::string>& vStr)
{
	string tmpStr = "";
	string valStr = "";
	for(int i=3;i<vStr.size();++i)
	{
		iPos = 0;
		tmpStr.clear();
		valStr.clear();
		if( ((iPos = vStr[i].find(":",0)) != string::npos))
		{
			tmpStr.clear();
			tmpStr = vStr[i].substr(0,iPos);
			valStr = vStr[i].substr(iPos+1);
			if(strstr(tmpStr.c_str(),"connection setup"))
			{
				m_pLTE_NM->mNMInfo.altairStats.RRCStats.connection_setup = atoi(valStr.c_str());
				////cout << "connection setup:" << m_pLTE_NM->mNMInfo.altairStats.RRCStats.connection_setup << endl;
			}
			else if(strstr(tmpStr.c_str(),"connection reject"))
			{
				m_pLTE_NM->mNMInfo.altairStats.RRCStats.connection_reject = atoi(valStr.c_str());
				////cout << "connection reject:" << m_pLTE_NM->mNMInfo.altairStats.RRCStats.connection_reject << endl;
			}
			else if(strstr(tmpStr.c_str(),"connection reconfig"))
			{
				m_pLTE_NM->mNMInfo.altairStats.RRCStats.connection_reconfig = atoi(valStr.c_str());
				////cout << "connection reconfig:" << m_pLTE_NM->mNMInfo.altairStats.RRCStats.connection_reconfig << endl;
			}
			else if(strstr(tmpStr.c_str(),"connection release"))
			{
				m_pLTE_NM->mNMInfo.altairStats.RRCStats.connection_release = atoi(valStr.c_str());
				////cout << "connection release:" << m_pLTE_NM->mNMInfo.altairStats.RRCStats.connection_release << endl;
			}
			else if(strstr(tmpStr.c_str(),"connection reestablishment"))
			{
				m_pLTE_NM->mNMInfo.altairStats.RRCStats.connection_reestablishment = atoi(valStr.c_str());
				////cout << "connection reestablishment:" << m_pLTE_NM->mNMInfo.altairStats.RRCStats.connection_reestablishment << endl;
			}
			else if(strstr(tmpStr.c_str(),"connection reestablishment reject"))
			{
				m_pLTE_NM->mNMInfo.altairStats.RRCStats.connection_reestablishment_reject = atoi(valStr.c_str());
				////cout << "connection reestablishment reject:" << m_pLTE_NM->mNMInfo.altairStats.RRCStats.connection_reestablishment_reject << endl;
			}
			else if(strstr(tmpStr.c_str(),"paging"))
			{
				m_pLTE_NM->mNMInfo.altairStats.RRCStats.paging = atoi(valStr.c_str());
				////cout << "paging:" << m_pLTE_NM->mNMInfo.altairStats.RRCStats.paging << endl;
			}
			else if(strstr(tmpStr.c_str(),"information tranfer"))
			{
				m_pLTE_NM->mNMInfo.altairStats.RRCStats.information_tranfer = atoi(valStr.c_str());
				////cout << "information tranfer:" << m_pLTE_NM->mNMInfo.altairStats.RRCStats.information_tranfer << endl;
			}
			else if(strstr(tmpStr.c_str(),"measurement reports"))
			{
				m_pLTE_NM->mNMInfo.altairStats.RRCStats.measurement_reports = atoi(valStr.c_str());
				////cout << "measurement reports:" << m_pLTE_NM->mNMInfo.altairStats.RRCStats.measurement_reports << endl;
			}
			else if(strstr(tmpStr.c_str(),"master information block"))
			{
				m_pLTE_NM->mNMInfo.altairStats.RRCStats.master_information_block = atoi(valStr.c_str());
				////cout << "master information block:" << m_pLTE_NM->mNMInfo.altairStats.RRCStats.master_information_block << endl;
			}
			else if(strstr(tmpStr.c_str(),"security mode command"))
			{
				m_pLTE_NM->mNMInfo.altairStats.RRCStats.security_mode_command = atoi(valStr.c_str());
				////cout << "security mode command:" << m_pLTE_NM->mNMInfo.altairStats.RRCStats.security_mode_command << endl;
			}
			else if(strstr(tmpStr.c_str(),"ue capability enquiry"))
			{
				m_pLTE_NM->mNMInfo.altairStats.RRCStats.ue_capability_enquiry = atoi(valStr.c_str());
				////cout << "ue capability enquiry:" << m_pLTE_NM->mNMInfo.altairStats.RRCStats.ue_capability_enquiry << endl;
			}
			else if(strstr(tmpStr.c_str(),"system information"))
			{
				m_pLTE_NM->mNMInfo.altairStats.RRCStats.system_information = atoi(valStr.c_str());
				////cout << "system information:" << m_pLTE_NM->mNMInfo.altairStats.RRCStats.system_information << endl;
			}
			else if(strstr(tmpStr.c_str(),"SIB_Type1"))
			{
				m_pLTE_NM->mNMInfo.altairStats.RRCStats.SIB_Type1 = atoi(valStr.c_str());
				////cout << "SIB_Type1:" << m_pLTE_NM->mNMInfo.altairStats.RRCStats.SIB_Type1 << endl;
			}
			else if(strstr(tmpStr.c_str(),"sib1"))
			{
				m_pLTE_NM->mNMInfo.altairStats.RRCStats.sib1 = atoi(valStr.c_str());
				////cout << "sib1:" << m_pLTE_NM->mNMInfo.altairStats.RRCStats.sib1 << endl;
			}
			else if(strstr(tmpStr.c_str(),"sib2"))
			{
				m_pLTE_NM->mNMInfo.altairStats.RRCStats.sib2 = atoi(valStr.c_str());
				////cout << "sib2:" << m_pLTE_NM->mNMInfo.altairStats.RRCStats.sib2 << endl;
			}
			else if(strstr(tmpStr.c_str(),"sib3"))
			{
				m_pLTE_NM->mNMInfo.altairStats.RRCStats.sib3 = atoi(valStr.c_str());
				////cout << "sib3:" << m_pLTE_NM->mNMInfo.altairStats.RRCStats.sib3 << endl;
			}
			else if(strstr(tmpStr.c_str(),"sib4"))
			{
				m_pLTE_NM->mNMInfo.altairStats.RRCStats.sib4 = atoi(valStr.c_str());
				////cout << "sib4:" << m_pLTE_NM->mNMInfo.altairStats.RRCStats.sib4 << endl;
			}
			else if(strstr(tmpStr.c_str(),"sib5"))
			{
				m_pLTE_NM->mNMInfo.altairStats.RRCStats.sib5 = atoi(valStr.c_str());
				////cout << "sib4:" << m_pLTE_NM->mNMInfo.altairStats.RRCStats.sib5 << endl;
			}
			else if(strstr(tmpStr.c_str(),"connection setup Complete"))
			{
				m_pLTE_NM->mNMInfo.altairStats.RRCStats.connection_setup_Complete = atoi(valStr.c_str());
				////cout << "connection setup Complete:" << m_pLTE_NM->mNMInfo.altairStats.RRCStats.connection_setup_Complete << endl;
			}
			else if(strstr(tmpStr.c_str(),"connection requests"))
			{
				m_pLTE_NM->mNMInfo.altairStats.RRCStats.connection_requests = atoi(valStr.c_str());
				////cout << "connection requests:" << m_pLTE_NM->mNMInfo.altairStats.RRCStats.connection_requests << endl;
			}
			else if(strstr(tmpStr.c_str(),"connection reconfiguration complete"))
			{
				m_pLTE_NM->mNMInfo.altairStats.RRCStats.connection_reconfiguration_complete = atoi(valStr.c_str());
				////cout << "connection reconfiguration complete:" << m_pLTE_NM->mNMInfo.altairStats.RRCStats.connection_reconfiguration_complete << endl;
			}
			else if(strstr(tmpStr.c_str(),"connection reestablishment Request"))
			{
				m_pLTE_NM->mNMInfo.altairStats.RRCStats.connection_reestablishment_Request = atoi(valStr.c_str());
				////cout << "connection reestablishment Request:" << m_pLTE_NM->mNMInfo.altairStats.RRCStats.connection_reestablishment_Request << endl;
			}
			else if(strstr(tmpStr.c_str(),"connection reestablishment Complete"))
			{
				m_pLTE_NM->mNMInfo.altairStats.RRCStats.connection_reconfiguration_complete = atoi(valStr.c_str());
				////cout << "connection reestablishment Complete:" << m_pLTE_NM->mNMInfo.altairStats.RRCStats.connection_reconfiguration_complete << endl;
			}
			else if(strstr(tmpStr.c_str(),"security mode complete"))
			{
				m_pLTE_NM->mNMInfo.altairStats.RRCStats.security_mode_complete = atoi(valStr.c_str());
				////cout << "security mode complete:" << m_pLTE_NM->mNMInfo.altairStats.RRCStats.security_mode_complete << endl;
			}
			else if(strstr(tmpStr.c_str(),"security mode failure"))
			{
				m_pLTE_NM->mNMInfo.altairStats.RRCStats.security_mode_failure = atoi(valStr.c_str());
				////cout << "security mode failure:" << m_pLTE_NM->mNMInfo.altairStats.RRCStats.security_mode_failure << endl;
			}
			else if(strstr(tmpStr.c_str(),"ul information transfer"))
			{
				m_pLTE_NM->mNMInfo.altairStats.RRCStats.ul_information_transfer = atoi(valStr.c_str());
				////cout << "ul information transfer:" << m_pLTE_NM->mNMInfo.altairStats.RRCStats.ul_information_transfer << endl;
			}
			else if(strstr(tmpStr.c_str(),"ue capability info"))
			{
				m_pLTE_NM->mNMInfo.altairStats.RRCStats.ue_capability_info = atoi(valStr.c_str());
				////cout << "ue capability info:" << m_pLTE_NM->mNMInfo.altairStats.RRCStats.ue_capability_info << endl;
			}
		}
	}
}
#endif

#if 0
void UAltairParser::parse_Timer_STATs(vector<std::string>& vStr)
{
	vector<string> vTmpStr;

	string tmpStr = "";
	string valStr = "";
	for(int i=2;i<vStr.size();++i)
	{
		vTmpStr.clear();
		tokeniseInformation(vStr[i],vTmpStr,',');
		
		iPos = 0;
		tmpStr.clear();
		valStr.clear();
		if( ((iPos = vTmpStr[0].find(":",0)) != string::npos))
		{
			tmpStr.clear();
			tmpStr = vTmpStr[0].substr(0,iPos);
			valStr = vTmpStr[0].substr(iPos+1);
			if(strstr(tmpStr.c_str(),"T3402"))
			{
				m_pLTE_NM->mNMInfo.altairStats.TimerStats.T3402_triggered = atoi(valStr.c_str());
				////cout << "Trigger T3402:" << m_pLTE_NM->mNMInfo.altairStats.TimerStats.T3402_triggered << endl;

				if( ((iPos = vTmpStr[1].find(":",0)) != string::npos))
				{
					tmpStr.clear();
					tmpStr = vTmpStr[1].substr(0,iPos);
					valStr = vTmpStr[1].substr(iPos+1);

					m_pLTE_NM->mNMInfo.altairStats.TimerStats.T3402_expired = atoi(valStr.c_str());
					////cout << "Expire T3402:" << m_pLTE_NM->mNMInfo.altairStats.TimerStats.T3402_expired << endl;
				}
			}
			else if(strstr(tmpStr.c_str(),"T3411"))
			{
				m_pLTE_NM->mNMInfo.altairStats.TimerStats.T3411_triggered = atoi(valStr.c_str());
				////cout << "Trigger T3411:" << m_pLTE_NM->mNMInfo.altairStats.TimerStats.T3411_triggered << endl;

				if( ((iPos = vTmpStr[1].find(":",0)) != string::npos))
				{
					tmpStr.clear();
					tmpStr = vTmpStr[1].substr(0,iPos);
					valStr = vTmpStr[1].substr(iPos+1);

					m_pLTE_NM->mNMInfo.altairStats.TimerStats.T3411_expired = atoi(valStr.c_str());
					////cout << "Expire T3411:" << m_pLTE_NM->mNMInfo.altairStats.TimerStats.T3411_expired << endl;
				}
			}
			else if(strstr(tmpStr.c_str(),"T311"))
			{
				m_pLTE_NM->mNMInfo.altairStats.TimerStats.T311_triggered = atoi(valStr.c_str());
				////cout << "Trigger T311:" << m_pLTE_NM->mNMInfo.altairStats.TimerStats.T311_triggered << endl;

				if( ((iPos = vTmpStr[1].find(":",0)) != string::npos))
				{
					tmpStr.clear();
					tmpStr = vTmpStr[1].substr(0,iPos);
					valStr = vTmpStr[1].substr(iPos+1);

					m_pLTE_NM->mNMInfo.altairStats.TimerStats.T311_expired = atoi(valStr.c_str());
					////cout << "Expire T311:" << m_pLTE_NM->mNMInfo.altairStats.TimerStats.T311_expired << endl;
				}
			}
			else if(strstr(tmpStr.c_str(),"T300"))
			{
				m_pLTE_NM->mNMInfo.altairStats.TimerStats.T300_triggered = atoi(valStr.c_str());
				////cout << "Trigger T300:" << m_pLTE_NM->mNMInfo.altairStats.TimerStats.T300_triggered << endl;

				if( ((iPos = vTmpStr[1].find(":",0)) != string::npos))
				{
					tmpStr.clear();
					tmpStr = vTmpStr[1].substr(0,iPos);
					valStr = vTmpStr[1].substr(iPos+1);

					m_pLTE_NM->mNMInfo.altairStats.TimerStats.T301_expired = atoi(valStr.c_str());
					////cout << "Expire T300:" << m_pLTE_NM->mNMInfo.altairStats.TimerStats.T301_expired << endl;
				}
			}
			else if(strstr(tmpStr.c_str(),"T301"))
			{
				m_pLTE_NM->mNMInfo.altairStats.TimerStats.T301_triggered = atoi(valStr.c_str());
				////cout << "Trigger T301:" << m_pLTE_NM->mNMInfo.altairStats.TimerStats.T301_triggered << endl;

				if( ((iPos = vTmpStr[1].find(":",0)) != string::npos))
				{
					tmpStr.clear();
					tmpStr = vTmpStr[1].substr(0,iPos);
					valStr = vTmpStr[1].substr(iPos+1);

					m_pLTE_NM->mNMInfo.altairStats.TimerStats.T301_expired = atoi(valStr.c_str());
					////cout << "Expire T301:" << m_pLTE_NM->mNMInfo.altairStats.TimerStats.T301_expired << endl;
				}
			}
		}
	}
}
#endif



void UAltairParser::parse_Meas(vector<std::string>& vStr)
{
	vector<string> vTmpStr;
	int measType = 0;
	int iPos = 0;

	string tmpStr = "";
	string valStr = "";
	string encodeStr = "";


	StatsEncodeInfo statsEncodeInfo;

	statsEncodeInfo.encodedStats.level = 0;
	statsEncodeInfo.encodedStats.rangeFlag = 0;
	statsEncodeInfo.encodedStats.valuePresent = 0;
	statsEncodeInfo.encodedStats.reserved = 0;
	


	for(unsigned int i=0;i<vStr.size();++i)
	{
		valStr = vStr[i];
		if( ((iPos = vStr[i].find(":",0)) != string::npos))
		{
			tmpStr.clear();
			tmpStr = vStr[i].substr(0,iPos+1);
			valStr = vStr[i].substr(iPos+1);

			statsEncodeInfo.statName = tmpStr;
			statsEncodeInfo.statValueStr = valStr;

			if( ((iPos = tmpStr.find("RSRP:",0)) != string::npos))
			{
				measType = 1;
				statsEncodeInfo.encodedStats.level = 0;
				statsEncodeInfo.encodedStats.valuePresent = 0;
				statsEncodeInfo.statName = "RSRP:";
				encodeStr += m_pLTE_NM->mNMInfo.m_nMeasStats.dlEarfcn;
				encodeStr += m_pLTE_NM->mNMInfo.m_nMeasStats.bands;
				encodeStr += m_pLTE_NM->mNMInfo.m_nMeasStats.bandwidth;
				encodeStr += m_pLTE_NM->mNMInfo.m_nMeasStats.cellID;
				encodeStr += encodeStats(statsEncodeInfo);

				
				if(strstr(valStr.c_str(),"Reported"))
				{
					/*replaceAll(valStr,"Reported = ","");

					U_SINT32 statValue=1;


					if(valStr.at(1) == '-')
					{
						replaceAll(valStr,"-","");
						replaceAll(valStr," ","");
						statValue = -1;
					}


					statValue = statValue*atoi(valStr.c_str());
					m_pLTE_NM->mNMInfo.m_nMeasStats.rssiVal = statValue;*/
				//	string rssi = valStr.c_str();
			
					strncpy(m_pLTE_NM->mNMInfo.m_nMeasStats.rssiVal,valStr.c_str(),
						sizeof(m_pLTE_NM->mNMInfo.m_nMeasStats.rssiVal)-1);

				}
				
				
				setEventID(UCONN_EVT_LTE_MODEM_RSRP_MEASUREMENT);
			}
			else if( ((iPos = tmpStr.find("RSRQ:",0)) != string::npos))
			{
				measType = 2;
				statsEncodeInfo.encodedStats.level = 0;
				statsEncodeInfo.encodedStats.valuePresent = 0;
				statsEncodeInfo.statName = "RSRQ:";
				encodeStr += encodeStats(statsEncodeInfo);
				setEventID(UCONN_EVT_LTE_MODEM_RSRQ_MEASUREMENT);
			}
			else if( ((iPos = tmpStr.find("SINR:",0)) != string::npos))
			{
				measType = 3;
				
				statsEncodeInfo.encodedStats.level = 0;
				statsEncodeInfo.encodedStats.valuePresent = 0;
				statsEncodeInfo.statName = "SINR:";
				encodeStr += encodeStats(statsEncodeInfo);

				
				if(strstr(valStr.c_str(),"Reported"))
				{
			/*		replaceAll(valStr,"Reported = ","");

					U_SINT32 statValue=1;


					if(valStr.at(1) == '-')
					{
						replaceAll(valStr,"-","");
						replaceAll(valStr," ","");
						statValue = -1;
					}


					statValue = statValue*atoi(valStr.c_str());*/

			  	strncpy(m_pLTE_NM->mNMInfo.m_nMeasStats.cinrVal,valStr.c_str(),
					 sizeof(m_pLTE_NM->mNMInfo.m_nMeasStats.cinrVal)-1);
				}
			  

				setEventID(UCONN_EVT_LTE_MODEM_SINR_MEASUREMENT);
			}
		}
			
		iPos = 0;
		string valueStr = ""; 
		
		if( ((iPos = valStr.find("=",0)) != string::npos))
		{
			tmpStr.clear();
			tmpStr = valStr.substr(0,iPos);

			statsEncodeInfo.statName = tmpStr;

			statsEncodeInfo.statValueStr.clear();
			statsEncodeInfo.statValueStr = valStr.substr(iPos+1);

			if(measType == 1)
			{
				if(strstr(tmpStr.c_str(),"Reported"))
				{
					m_pLTE_NM->mNMInfo.Signal_Strength = atoi(valueStr.c_str());
				}
			}

			statsEncodeInfo.encodedStats.level = 1;
			statsEncodeInfo.encodedStats.valuePresent = 1;
			encodeStr += encodeStats(statsEncodeInfo);
			
		}
	}
	
	if(measType == 1)
	{
		strncpy(m_pLTE_NM->mNMInfo.m_nMeasStats.RSRPencodStr,encodeStr.c_str(),encodeStr.size());
		m_pLTE_NM->mNMInfo.m_nMeasStats.RSRPencodStr[encodeStr.size()]='\0';
	}
	else if(measType == 2)
	{
		strncpy(m_pLTE_NM->mNMInfo.m_nMeasStats.RSRQencodStr,encodeStr.c_str(),encodeStr.size());
		m_pLTE_NM->mNMInfo.m_nMeasStats.RSRQencodStr[encodeStr.size()]='\0';
	}
	else if(measType == 3)
	{
		strncpy(m_pLTE_NM->mNMInfo.m_nMeasStats.SINRencodStr,encodeStr.c_str(),encodeStr.size());
		m_pLTE_NM->mNMInfo.m_nMeasStats.SINRencodStr[encodeStr.size()]='\0';
	}
}




#if 0
void UAltairParser::parse_Meas(vector<std::string>& vStr)
{
	vector<string> vTmpStr;
	int measType = 0;
	int iPos = 0;

	string tmpStr = "";
	string valStr = "";
	string encodeStr = "";
	for(int i=0;i<vStr.size();++i)
	{
		valStr = vStr[i];
		if( ((iPos = vStr[i].find(":",0)) != string::npos))
		{
			tmpStr.clear();
			tmpStr = vStr[i].substr(0,iPos+1);
			valStr = vStr[i].substr(iPos+1);
			if( ((iPos = tmpStr.find("RSRP:",0)) != string::npos))
			{
				measType = 1;
			}
			else if( ((iPos = tmpStr.find("RSRQ:",0)) != string::npos))
			{
				measType = 2;
			}
			else if( ((iPos = tmpStr.find("SINR:",0)) != string::npos))
			{
				measType = 3;
			}
		}
			
		iPos = 0;
		string valueStr = ""; 
		if( ((iPos = valStr.find("=",0)) != string::npos))
		{
			tmpStr.clear();
			tmpStr = valStr.substr(0,iPos);

			valueStr.clear();
			valueStr = valStr.substr(iPos+1);

			updateMeasurement(measType,tmpStr,valueStr,encodeStr);
		}
	}
	//strcpy(m_pLTE_NM->mNMInfo.m_nL1AStats.L1AencodStr,encodeStr.c_str());
	strcpy(m_pLTE_NM->mNMInfo.m_nRSRPStats.MeasencodStr,encodeStr.c_str());
}
#endif 






#if 0
	if(noOfEntries >= 7)
	{
		/*For getting APN name from vStr[2]*/
		strcpy(m_pLTE_NM->mNMInfo.apn_name,vtempStr[2].c_str());
		//cout << "APN Name:" <<m_pLTE_NM->mNMInfo.apn_name << endl;

		/*to get the no. of dots in the ipaddress*/
		tempStr1 = vtempStr[3];
		tokeniseInformation(tempStr1,vDotsCount,'.');

		int nDotCount = vDotsCount.size();
		if(nDotCount <= 8)
		{
			/*set for IPv4 addresss*/
			//cout << "tempStr1" << tempStr1 << endl;
			string tStr = "";
			string subnetMaskAddress = "";
			string ipAddress = "";
			getAnyFormatIPAddrSubnetMaskAddr(tempStr1,ipAddress,subnetMaskAddress,4);
			strcpy(m_pLTE_NM->mNMInfo.ip_address,ipAddress.c_str());
			strcpy(m_pLTE_NM->mNMInfo.subnet_mask,subnetMaskAddress.c_str());
			//cout << "ipv4 address:" << m_pLTE_NM->mNMInfo.ip_address << "and subnetmask address:" << m_pLTE_NM->mNMInfo.subnet_mask << endl;

			if(vtempStr.size() == 5)
			{
				/*For getting gateway address from vStr[4]*/
				strcpy(m_pLTE_NM->mNMInfo.gw_address,vtempStr[4].c_str());
				//cout << "gateway address:" << m_pLTE_NM->mNMInfo.gw_address << endl;
			}
			
			/*For getting DNS_prim_addr from vStr[5]*/
			if(vtempStr.size() == 6)
			{
				strcpy(m_pLTE_NM->mNMInfo.pri_dns,vtempStr[5].c_str());
				strcpy(m_pLTE_NM->mNMInfo.dyn_dns, m_pLTE_NM->mNMInfo.pri_dns);	
				//cout << "Primary dns address:" << m_pLTE_NM->mNMInfo.pri_dns << endl;
			}
		
			
			/*For getting DNS_sec_addr from vStr[6]*/
			if(vtempStr.size() == 7)
			{
				strcpy(m_pLTE_NM->mNMInfo.sec_dns,vtempStr[6].c_str());
				//cout << "Secondary dns address:" << m_pLTE_NM->mNMInfo.sec_dns << endl;
			}
			
			/*For getting P_CSCF_prim_addr from vStr[7]*/
			if(vtempStr.size() == 8)
			{
				strcpy(m_pLTE_NM->mNMInfo.p_cscf_prim_addr,vtempStr[7].c_str());
				//cout << "p_cscf_prim_addr:" << m_pLTE_NM->mNMInfo.p_cscf_prim_addr << endl;
			}
			
			if(vtempStr.size() == 9)
			{
			///*For getting P_CSCF_sec_addr from vStr[8]*/
				strcpy(m_pLTE_NM->mNMInfo.p_cscf_sec_addr,vtempStr[8].c_str());
				//cout << "P_CSCF_sec_addr:" << m_pLTE_NM->mNMInfo.p_cscf_sec_addr << endl;
			}
		}
		else
		{
			/*set for IPv6 addresss*/
			//cout << "tempStr1" << tempStr1 << endl;
			string tStr = "";
			string subnetMaskAddress = "";
			string ipAddress = "";
			getAnyFormatIPAddrSubnetMaskAddr(tempStr1,ipAddress,subnetMaskAddress,16);
			strcpy(m_pLTE_NM->mNMInfo.ipv6_address,ipAddress.c_str());
			strcpy(m_pLTE_NM->mNMInfo.subnetv6_mask,subnetMaskAddress.c_str());
			//cout << "ipv6 address:" << m_pLTE_NM->mNMInfo.ipv6_address << "and subnetmask address:" << m_pLTE_NM->mNMInfo.subnetv6_mask << endl;

			/*For getting gateway address from vStr[4]*/
			if(vtempStr.size() == 5)
			{
				strcpy(m_pLTE_NM->mNMInfo.gwv6_address,vtempStr[4].c_str());
				//cout << "gateway address:" << m_pLTE_NM->mNMInfo.gwv6_address << endl;
			}
			/*For getting DNS_prim_addr from vStr[5]*/
			if(vtempStr.size() == 6)
			{
				strcpy(m_pLTE_NM->mNMInfo.priv6_dns,vtempStr[5].c_str());
				//cout << "Primary dns address:" << m_pLTE_NM->mNMInfo.priv6_dns << endl;
			}
			/*For getting DNS_sec_addr from vStr[6]*/
			if(vtempStr.size() == 7)
			{
				strcpy(m_pLTE_NM->mNMInfo.secv6_dns,vtempStr[6].c_str());
				//cout << "Secondary dns address:" << m_pLTE_NM->mNMInfo.secv6_dns << endl;
			}
			/*For getting P_CSCF_prim_addr from vStr[7]*/
			if(vtempStr.size() == 8)
			{
				strcpy(m_pLTE_NM->mNMInfo.p_cscfv6_prim_addr,vtempStr[7].c_str());
				//cout << "p_cscf_prim_addr:" << m_pLTE_NM->mNMInfo.p_cscfv6_prim_addr << endl;
			}
			///*For getting P_CSCF_sec_addr from vStr[8]*/
			if(vtempStr.size() == 9)
			{
				strcpy(m_pLTE_NM->mNMInfo.p_cscfv6_sec_addr,vtempStr[8].c_str());
				//cout << "P_CSCF_sec_addr:" << m_pLTE_NM->mNMInfo.p_cscfv6_sec_addr << endl;
			}
		}
	}
#endif

	




