#include "UBroadcomParser.h"
#include "uconn_Event.h"
#include "uconn_ServerAPI.h"
#include "UCommonFunctions.h"
	
UBroadcomParser::UBroadcomParser(UNetworkManager* p_NM):UBufferParser(p_NM)
{
	
}

void UBroadcomParser::processUnSolResp(string &unSolicitedRespStr)
{
	setEventID(0);
	//printf("################# The EVT string received = %s #############\n", unSolicitedRespStr.c_str());
	if(strstr(unSolicitedRespStr.c_str(),"*MRDY:") || strstr(unSolicitedRespStr.c_str(),"*MSDI:"))
	{
		////cout << "Propritery Unsolicited Events:" << unSolicitedRespStr << endl;
		////cout << "Ignoring these events"<< endl;

		/* Check for *MRDY: 4 and set the relevant Event */
		/*
		if(strstr(unSolicitedRespStr.c_str(), "*MRDY: 4"))
		{
			setEventID(UCONN_EVT_READY_FOR_ATTACH);
		}
		*/
		unSolicitedRespStr = "";
		return;
	}
	else if(strstr(unSolicitedRespStr.c_str(),"*MIMSEV:"))
	{
		////cout << "Unsolicited Events:" << unSolicitedRespStr << endl;
		
		if(strstr(unSolicitedRespStr.c_str(),"SUCCESS"))
		{
			setEventID(UCONN_EVT_IMS_REG_SUCCESS);
		}
		else if(strstr(unSolicitedRespStr.c_str(),"FAILURE"))
		{
			setEventID(UCONN_EVT_IMS_REG_FAILURE);
		}
		else if(strstr(unSolicitedRespStr.c_str(),"DEREG"))
		{
			setEventID(UCONN_EVT_IMS_DEREG);
		}
		else if(strstr(unSolicitedRespStr.c_str(),"ATTACH"))
		{
			setEventID(UCONN_EVT_REQ_ATTACH);
		}
		else if(strstr(unSolicitedRespStr.c_str(),"DETACH"))
		{
			setEventID(UCONN_EVT_REQ_DETACH);
		}
		else if(strstr(unSolicitedRespStr.c_str(),"POR"))
		{
			setEventID(UCONN_EVT_REQ_POR);
		}
		else if(strstr(unSolicitedRespStr.c_str(),"MODE"))
		{
			setEventID(UCONN_EVT_REQ_IMS_LTD_MODE);
		}
		else
		{
			//cout<<"invalid response from device\n"<<endl;
		}
	}
	else if(strstr(unSolicitedRespStr.c_str(),"+CEREG:"))
	{
		vector<string> vStr;
		string tempStr = "";
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
	else
	{
		UBufferParser::processUnSolResp(unSolicitedRespStr);
	}

	unSolicitedRespStr = "";
	return;
}


void UBroadcomParser::processAtCommandResp(string &atCmdRespStr)
{
	vector<string> vStr;
	string tmpStr = "";
	setEventID(0);

	if(strstr(atCmdRespStr.c_str(),"*STAT:"))
	{
		/*Parsing of *STAT: response here*/
		////cout << "The Response for AT command received is:" << atCmdRespStr << endl;
		vStr.clear();
		tokeniseInformation(atCmdRespStr,vStr);
		parseSTATISTICs(vStr);
		if(strstr(atCmdRespStr.c_str(),"*STAT: 2"))
		{
			setEventID(UCONN_EVT_LTE_MODEM_STATISTIC);
		}
		vStr.clear();
	}
	else if(strstr(atCmdRespStr.c_str(),"ATI2"))
	{
		/*
		vector<string> temp_verStr;
		U_CSTR verInfo_str = parsedInfo.parsed_VersionInfo.u8VersionString; 
		//Parsing of ATI2 output done here 
		tokeniseInformation(atCmdRespStr,vStr,10);
		tokeniseInformation(vStr[1], temp_verStr, ';');
		strcpy(verInfo_str, " \t ");

		for(U_UINT32 i = 0; i < (temp_verStr.size() - 1); i++)
		{
			strcat(verInfo_str, temp_verStr[i].c_str());
			strcat(verInfo_str, "\n\t");
		}

		*/

		this->versionString = atCmdRespStr;
		replaceAll(this->versionString,"ATI2","");
		replaceAll(this->versionString,"OK","");
		replaceAll(this->versionString,";","\n");
		replaceAll(this->versionString," ","");

		strncpy(parsedInfo.parsed_VersionInfo.u8VersionString,this->versionString.c_str(),this->versionString.size());
		parsedInfo.parsed_VersionInfo.u8VersionString[this->versionString.size()]='\0';
		this->versionString.clear();
		


		setEventID(UCONN_CMD_LTE_VERSION_INFO_RSP);		
		
		//vStr.clear();
	}
	else 	////cout << "AT Command Response is received at the Callback:" << atCmdRespStr << endl;
	if(strstr(atCmdRespStr.c_str(),"+CSQ:"))
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
			////cout << "RSSIValue is:" << parsedInfo.geRSSI() << endl;

			parsedInfo.setCINR(atoi(vStr[1].c_str()));
			////cout << "CINRValue is:" << parsedInfo.getCINR() << endl;

			setEventID(UCONN_CMD_NET_SIGQUAL_GET);
		}
		vStr.clear();
	}
	else if(strstr(atCmdRespStr.c_str(),"+CGCONTRDP:"))
	{
		/*Parsing of AT+CGCONTRDP response here*/
        ////cout << "The Response for AT command received is:" << atCmdRespStr << endl;
		tokeniseInformation(atCmdRespStr,vStr,'+');

		int noOfLinesOfInformation = vStr.size();
		if(noOfLinesOfInformation >= 2)
		{
			for(int i=0;i < (noOfLinesOfInformation-1);++i)
			/*handle both ipv4 and ipv6 address lines*/
			parseIPAddress(vStr[i+1]);
		}
			setEventID(UCONN_EVT_CONTRDP_RSP);
		
		vStr.clear();
	}
	
	else if(strstr(atCmdRespStr.c_str(),"+CEREG:"))
	{
		/*Parsing of +CEREG: response here*/
        ////cout << "The Response for AT command received is:" << atCmdRespStr << endl;
		tokeniseInformation(atCmdRespStr,vStr);
		if(vStr.size() >= 1)
        {
			parsedInfo.setModem_State(atoi(vStr[1].c_str()));
			//cout << "Modem State is:" <<vStr[1].c_str() << endl;
        }
		
		vStr.clear();
	}
    else if(strstr(atCmdRespStr.c_str(),"+CFUN:"))
	{
		/* Parsing of the +CFUN: response here */
		tokeniseInformation(atCmdRespStr,vStr,':');
		if(vStr.size() >= 1)
        {
			parsedInfo.setPowerup_state(atoi(vStr[1].c_str()));
			//cout << "Power up State is:" <<vStr[1].c_str() << endl;
			setEventID(UCONN_POWERUP_RSP);
        }
		
		vStr.clear();
	}

	else
	{
		UBufferParser::processAtCommandResp(atCmdRespStr);
	}

}




void UBroadcomParser::parseSTATISTICs(vector<std::string>& vStr)
{

		string encodeStr = "";

		StatsEncodeInfo_s statsEncodeInfo;
		
		statsEncodeInfo.encodedStats.reserved = 0;

		
		if(strstr(vStr[0].c_str(),"0"))
		{

			statsEncodeInfo.encodedStats.level = 0;
			statsEncodeInfo.encodedStats.valuePresent = 0;
			statsEncodeInfo.encodedStats.rangeFlag = 0;  // Since Generic Stats has no value, its assumed to be within range

			

			statsEncodeInfo.statName = "Generic Stats";

			encodeStr += encodeStats(statsEncodeInfo,RANGE_UNSIGNED);
			
			string tmpStr = "";
			string valStr = "";

		for(U_UINT32 i=1;i<vStr.size();++i)
		{
			iPos = 0;
			tmpStr.clear();
			valStr.clear();
			if( ((iPos = vStr[i].find(":",0)) != string::npos))
			{
				tmpStr.clear();
				tmpStr = vStr[i].substr(0,iPos);
				valStr = vStr[i].substr(iPos+1);

				U_UINT32 statValue = atoi(valStr.c_str());

				statsEncodeInfo.encodedStats.level = 1;
				statsEncodeInfo.encodedStats.valuePresent = 1;
				statsEncodeInfo.statName = tmpStr;
				statsEncodeInfo.statValueStr = valStr;

				statsEncodeInfo.rangeValueInfo.rangeUnsigned.statValue = statValue;

				if(strstr(tmpStr.c_str(),"Modem_State"))
				{
					
					statsEncodeInfo.rangeValueInfo.rangeUnsigned.startRange_unsigned = 0;
					statsEncodeInfo.rangeValueInfo.rangeUnsigned.endRange_unsigned = 6;
					encodeStr += encodeStats(statsEncodeInfo,RANGE_UNSIGNED);			
				}
				else if(strstr(tmpStr.c_str(),"DL_Throughput"))
				{
					statsEncodeInfo.rangeValueInfo.rangeUnsigned.startRange_unsigned = 0;
					statsEncodeInfo.rangeValueInfo.rangeUnsigned.endRange_unsigned = 150000;
					encodeStr += encodeStats(statsEncodeInfo,RANGE_UNSIGNED);
					m_pLTE_NM->mNMInfo.m_nBroadcomStoredStats.u32DLBw = statValue;

				}
				
				else if(strstr(tmpStr.c_str(),"UL_Throughput"))
				{
					statsEncodeInfo.rangeValueInfo.rangeUnsigned.startRange_unsigned = 0;
					statsEncodeInfo.rangeValueInfo.rangeUnsigned.endRange_unsigned = 50000;
					encodeStr += encodeStats(statsEncodeInfo,RANGE_UNSIGNED);
					m_pLTE_NM->mNMInfo.m_nBroadcomStoredStats.u32ULBw = statValue;


			    }
				
				
				else if(strstr(tmpStr.c_str(),"No_of_Connects"))
				{
					statsEncodeInfo.rangeValueInfo.rangeUnsigned.startRange_unsigned = 0;
					statsEncodeInfo.rangeValueInfo.rangeUnsigned.endRange_unsigned = 4294967295;

					string valueStr = "";
							
					for(unsigned int i=0;i<valStr.length();i++)
					{
						if(valStr[i]<48 || valStr[i]>57)
							break;
													
					valueStr +=valStr[i];
					
					}

					statsEncodeInfo.statValueStr = valueStr;
					encodeStr += encodeStats(statsEncodeInfo,RANGE_UNSIGNED);
					
				}

				else if(strstr(tmpStr.c_str(),"OK")) 
				{
					tmpStr.clear();
				}
				else if(strlen(tmpStr.c_str())>2) 
				{
					statsEncodeInfo.rangeValueInfo.rangeUnsigned.startRange_unsigned = 0;
					statsEncodeInfo.rangeValueInfo.rangeUnsigned.endRange_unsigned = 4294967295;
					encodeStr += encodeStats(statsEncodeInfo,RANGE_UNSIGNED);	
				}
				else
				{
					tmpStr.clear();

				}
			}
		}


		memset(&m_pLTE_NM->mNMInfo.m_nBroadcomStats,0,sizeof(Broadcom_Stats_s));
		strncpy(m_pLTE_NM->mNMInfo.m_nBroadcomStats.genStats,encodeStr.c_str(),encodeStr.size());
		m_pLTE_NM->mNMInfo.m_nBroadcomStats.genStats[encodeStr.size()] = '\0';

		int size = encodeStr.size();

	}



	else if(strstr(vStr[0].c_str(),"1"))
	{

		
		statsEncodeInfo.encodedStats.level = 0;
		statsEncodeInfo.encodedStats.valuePresent = 0;
		statsEncodeInfo.encodedStats.rangeFlag = 0;  // Since Mac Stats is the heading, it's assumed to be within range

		statsEncodeInfo.statName = "Mac Stats";

		encodeStr += encodeStats(statsEncodeInfo,RANGE_UNSIGNED);

		string tmpStr = "";
		string valStr = "";
		
		for(U_UINT32 i=1;i<vStr.size();++i)
		{
			iPos = 0;
			tmpStr.clear();
			valStr.clear();
			if( ((iPos = vStr[i].find(":",0)) != string::npos))
			{
				tmpStr.clear();
				tmpStr = vStr[i].substr(0,iPos);
				valStr = vStr[i].substr(iPos+1);

				U_UINT32 statValue = atoi(valStr.c_str());

				statsEncodeInfo.encodedStats.level = 1;
				statsEncodeInfo.encodedStats.valuePresent = 1;
				statsEncodeInfo.statName = tmpStr;
				statsEncodeInfo.statValueStr = valStr;

				statsEncodeInfo.rangeValueInfo.rangeUnsigned.statValue = statValue;

				if(strstr(tmpStr.c_str(),"PLMNId"))
				{
					
					statsEncodeInfo.rangeValueInfo.rangeUnsigned.startRange_unsigned = 0;
					statsEncodeInfo.rangeValueInfo.rangeUnsigned.endRange_unsigned = 10066329;
					encodeStr += encodeStats(statsEncodeInfo,RANGE_UNSIGNED);	
					
				}
				else if(strstr(tmpStr.c_str(),"FrameNum"))
				{
					

					statsEncodeInfo.rangeValueInfo.rangeUnsigned.startRange_unsigned = 0;
					statsEncodeInfo.rangeValueInfo.rangeUnsigned.endRange_unsigned = 10239;
					encodeStr += encodeStats(statsEncodeInfo,RANGE_UNSIGNED);	

				
				}
			
			
				else if(strstr(tmpStr.c_str(),"CRNTI"))
				{
					
					statsEncodeInfo.rangeValueInfo.rangeUnsigned.startRange_unsigned = 1;
					statsEncodeInfo.rangeValueInfo.rangeUnsigned.endRange_unsigned = 65523;
					encodeStr += encodeStats(statsEncodeInfo,RANGE_UNSIGNED);	

					
				}
				else if(strstr(tmpStr.c_str(),"NumDRBs"))
				{
				

					statsEncodeInfo.rangeValueInfo.rangeUnsigned.startRange_unsigned = 1;
					statsEncodeInfo.rangeValueInfo.rangeUnsigned.endRange_unsigned = 11;
					encodeStr += encodeStats(statsEncodeInfo,RANGE_UNSIGNED);	

				}
				else if(strstr(tmpStr.c_str(),"DL_NumTBs"))
				{
					statsEncodeInfo.encodedStats.rangeFlag = 0;
					statsEncodeInfo.encodedStats.valuePresent = 0;

					statsEncodeInfo.statName = "DLTB Stats"; // Adding level 1 heading
					encodeStr += encodeStats(statsEncodeInfo,RANGE_UNSIGNED);
				
					statsEncodeInfo.encodedStats.level = 2;
					statsEncodeInfo.encodedStats.valuePresent = 1;
					

					statsEncodeInfo.statName = tmpStr;
					statsEncodeInfo.rangeValueInfo.rangeUnsigned.startRange_unsigned = 0;
					statsEncodeInfo.rangeValueInfo.rangeUnsigned.endRange_unsigned = 4294967295;
					encodeStr += encodeStats(statsEncodeInfo,RANGE_UNSIGNED);	


				}
									
				else if(strstr(tmpStr.c_str(),"DL_NumErrTBs"))
				{

					statsEncodeInfo.encodedStats.level = 2;
					statsEncodeInfo.rangeValueInfo.rangeUnsigned.startRange_unsigned = 0;
					statsEncodeInfo.rangeValueInfo.rangeUnsigned.endRange_unsigned = 4294967295;
					encodeStr += encodeStats(statsEncodeInfo,RANGE_UNSIGNED);	


				}
				else if(strstr(tmpStr.c_str(),"DL_PreHarqErr")||strstr(tmpStr.c_str(),"DL_PostHarqErr"))
				{

					statsEncodeInfo.encodedStats.level = 2;
					statsEncodeInfo.encodedStats.valuePresent = 1;
					float fStatValue = statValue/(float)10000.0;
					int iStatValue = (int)fStatValue;

					statsEncodeInfo.rangeValueInfo.rangeUnsigned.startRange_unsigned = 0;
					statsEncodeInfo.rangeValueInfo.rangeUnsigned.endRange_unsigned = 100;
					encodeStr += encodeStats(statsEncodeInfo,RANGE_UNSIGNED);	

				}
				
				else if(strstr(tmpStr.c_str(),"UL_NumTBs"))
				{

					statsEncodeInfo.encodedStats.level = 1;
					statsEncodeInfo.encodedStats.rangeFlag = 0;
					statsEncodeInfo.encodedStats.valuePresent = 0;
					statsEncodeInfo.statName = "ULTB Stats";

					encodeStr += encodeStats(statsEncodeInfo,RANGE_UNSIGNED);
					
					statsEncodeInfo.encodedStats.level = 2;
					statsEncodeInfo.encodedStats.valuePresent = 1;
					

					statsEncodeInfo.statName = tmpStr;
					statsEncodeInfo.rangeValueInfo.rangeUnsigned.startRange_unsigned = 0;
					statsEncodeInfo.rangeValueInfo.rangeUnsigned.endRange_unsigned = 4294967295;
					encodeStr += encodeStats(statsEncodeInfo,RANGE_UNSIGNED);	

					
				
				}
				else if(strstr(tmpStr.c_str(),"UL_NumReTxTBs") || strstr(tmpStr.c_str(),"UL_NumErrTBs"))
				{
					statsEncodeInfo.encodedStats.level = 2;
					statsEncodeInfo.rangeValueInfo.rangeUnsigned.startRange_unsigned = 0;
					statsEncodeInfo.rangeValueInfo.rangeUnsigned.endRange_unsigned = 4294967295;
					encodeStr += encodeStats(statsEncodeInfo,RANGE_UNSIGNED);	
				
				}
				
				else if(strstr(tmpStr.c_str(),"UL_PreHarqErr") || strstr(tmpStr.c_str(),"UL_PostHarqErr"))
				{
					statsEncodeInfo.encodedStats.level = 2;
					statsEncodeInfo.rangeValueInfo.rangeUnsigned.startRange_unsigned = 0;
					statsEncodeInfo.rangeValueInfo.rangeUnsigned.endRange_unsigned = 100;
					encodeStr += encodeStats(statsEncodeInfo,RANGE_UNSIGNED);	
				
				}
				else if(strstr(tmpStr.c_str(),"NumPRACH"))
				{
					statsEncodeInfo.statName = "NumPRACH";
					//replaceAll(statsEncodeInfo.statName," ","");
					statsEncodeInfo.encodedStats.level = 1;
					statsEncodeInfo.rangeValueInfo.rangeUnsigned.startRange_unsigned = 0;
					statsEncodeInfo.rangeValueInfo.rangeUnsigned.endRange_unsigned = 4294967295;
					encodeStr += encodeStats(statsEncodeInfo,RANGE_UNSIGNED);	
					//replaceAll(encodeStr," ","");
				
				}

				else if(strstr(tmpStr.c_str(),"NumMacPdusTx"))
				{
					statsEncodeInfo.rangeValueInfo.rangeUnsigned.startRange_unsigned = 0;
					statsEncodeInfo.rangeValueInfo.rangeUnsigned.endRange_unsigned = 4294967295;


					string valueStr = "";
							
					for(unsigned int i=0;i<valStr.length();i++)
					{
						if(valStr[i]<48 || valStr[i]>57)
							break;
													
					valueStr +=valStr[i];
					
					}

					statsEncodeInfo.statValueStr = valueStr;

					encodeStr += encodeStats(statsEncodeInfo,RANGE_UNSIGNED);	
				}

				else if(strstr(tmpStr.c_str(),"OK"))
				{
					//do nothing
				}
				else if(tmpStr.size() < 2)
				{
					//do nothing

				}
				else 
				{
					//general case
					statsEncodeInfo.rangeValueInfo.rangeUnsigned.startRange_unsigned = 0;
					statsEncodeInfo.rangeValueInfo.rangeUnsigned.endRange_unsigned = 4294967295;
					encodeStr += encodeStats(statsEncodeInfo,RANGE_UNSIGNED);	

				}
			}
		}

		strncpy(m_pLTE_NM->mNMInfo.m_nBroadcomStats.macStats,encodeStr.c_str(),encodeStr.size());
		m_pLTE_NM->mNMInfo.m_nBroadcomStats.macStats[encodeStr.size()] = '\0';
		int size = encodeStr.size();

	}
	else if(strstr(vStr[0].c_str(),"2"))
	{

		statsEncodeInfo.encodedStats.level = 0;
		statsEncodeInfo.encodedStats.valuePresent = 0;
		statsEncodeInfo.encodedStats.rangeFlag = 0;  // Since Phy Stats has no value, its assumed to be within range

		statsEncodeInfo.statName = "Phy_Stats";

		encodeStr += encodeStats(statsEncodeInfo,RANGE_UNSIGNED);
		
		string tmpStr = "";
		string valStr = "";

		U_UINT32 u32SignBit,u32IntegerBits,u32FractionBits;

		for(U_UINT32 i=0;i<vStr.size();++i)
		{
			iPos = 0;
			tmpStr.clear();
			valStr.clear();

			



			if( ((iPos = vStr[i].find(":",0)) != string::npos))
			{
				tmpStr.clear();
				tmpStr = vStr[i].substr(0,iPos);
				valStr = vStr[i].substr(iPos+1);


				U_SINT32 statValue = atoi(valStr.c_str());

				statsEncodeInfo.encodedStats.level = 1;
				statsEncodeInfo.encodedStats.valuePresent = 1;
				statsEncodeInfo.encodedStats.rangeFlag = 0; 

				statsEncodeInfo.statName = tmpStr;
				statsEncodeInfo.statValueStr = valStr;



				if(strstr(tmpStr.c_str(),"PhyIdCell"))
				{
					statsEncodeInfo.rangeValueInfo.rangeUnsigned.startRange_unsigned = 0;
					statsEncodeInfo.rangeValueInfo.rangeUnsigned.endRange_unsigned = 503;
					encodeStr += encodeStats(statsEncodeInfo,RANGE_UNSIGNED);	
				}

				else if(strstr(tmpStr.c_str(),"DLEARFCN") || strstr(tmpStr.c_str(),"ULEARFCN") )
				{
				
					statsEncodeInfo.rangeValueInfo.rangeUnsigned.startRange_unsigned = 0;
					statsEncodeInfo.rangeValueInfo.rangeUnsigned.endRange_unsigned = 65535;
					encodeStr += encodeStats(statsEncodeInfo,RANGE_UNSIGNED);	
				}

				else if(strstr(tmpStr.c_str(),"DLBw") || strstr(tmpStr.c_str(),"ULBw"))
				{
					statsEncodeInfo.rangeValueInfo.rangeUnsigned.startRange_unsigned = 0;
					statsEncodeInfo.rangeValueInfo.rangeUnsigned.endRange_unsigned = 5;
					encodeStr += encodeStats(statsEncodeInfo,RANGE_UNSIGNED);	

				}
			
				else if(strstr(tmpStr.c_str(),"BandType"))
				{
					statsEncodeInfo.rangeValueInfo.rangeUnsigned.startRange_unsigned = 0;
					statsEncodeInfo.rangeValueInfo.rangeUnsigned.endRange_unsigned = 1;
					encodeStr += encodeStats(statsEncodeInfo,RANGE_UNSIGNED);
					
				}

				else if(strstr(tmpStr.c_str(),"NumPorts"))
				{
					statsEncodeInfo.rangeValueInfo.rangeUnsigned.startRange_unsigned = 1;
					statsEncodeInfo.rangeValueInfo.rangeUnsigned.endRange_unsigned = 4;
					encodeStr += encodeStats(statsEncodeInfo,RANGE_UNSIGNED);
				}
				else if(strstr(tmpStr.c_str(),"RSRP") || strstr(tmpStr.c_str(),"RSRQ") )
				{

					u32SignBit = 1;
					u32IntegerBits = 15;
					u32FractionBits = 16;
					double rsr;
					rsr = signedToFloat(statValue,u32SignBit,u32IntegerBits,u32FractionBits);

					statsEncodeInfo.rangeValueInfo.rangeFloat.statValue = (float)rsr;

					statsEncodeInfo.rangeValueInfo.rangeFloat.startRange_float = -140;
					statsEncodeInfo.rangeValueInfo.rangeFloat.endRange_float = -44;

					encodeStr += encodeStats(statsEncodeInfo,RANGE_FLOAT);

				
				}

				

				else if(strstr(tmpStr.c_str(),"RssiAnt0"))
				{

					
					u32SignBit = 1;
					u32IntegerBits = 15;
					u32FractionBits = 16;
					double rssiAnt0;
					rssiAnt0 = signedToFloat(statValue,u32SignBit,u32IntegerBits,u32FractionBits);

					statsEncodeInfo.rangeValueInfo.rangeFloat.statValue = (float)rssiAnt0;

					statsEncodeInfo.rangeValueInfo.rangeFloat.startRange_float = -120;
					statsEncodeInfo.rangeValueInfo.rangeFloat.endRange_float = 0;

					encodeStr += encodeStats(statsEncodeInfo,RANGE_FLOAT);
					m_pLTE_NM->mNMInfo.m_nBroadcomStoredStats.rssiAnt0 = rssiAnt0;
					
				}
				else if(strstr(tmpStr.c_str(),"RssiAnt1"))
				{

					
					u32SignBit = 1;
					u32IntegerBits = 15;
					u32FractionBits = 16;
					double rssiAnt1;
					rssiAnt1 = signedToFloat(statValue,u32SignBit,u32IntegerBits,u32FractionBits);

					statsEncodeInfo.rangeValueInfo.rangeFloat.statValue = (float)rssiAnt1;

					statsEncodeInfo.rangeValueInfo.rangeFloat.startRange_float = 0;
					statsEncodeInfo.rangeValueInfo.rangeFloat.endRange_float = 503;

					encodeStr += encodeStats(statsEncodeInfo,RANGE_FLOAT);

					m_pLTE_NM->mNMInfo.m_nBroadcomStoredStats.rssiAnt1 = rssiAnt1;
					
				}
				else if(strstr(tmpStr.c_str(),"UlTxPower"))
				{

					double rssiAnt1 = signedToFloat(statValue,u32SignBit,u32IntegerBits,u32FractionBits);

					statsEncodeInfo.rangeValueInfo.rangeFloat.statValue = (float)rssiAnt1;

					statsEncodeInfo.rangeValueInfo.rangeFloat.startRange_float = -60;
					statsEncodeInfo.rangeValueInfo.rangeFloat.endRange_float = 33;

					encodeStr += encodeStats(statsEncodeInfo,RANGE_FLOAT);

					
				}
				else if(strstr(tmpStr.c_str(),"NumDCIs"))
				{
					statsEncodeInfo.rangeValueInfo.rangeUnsigned.startRange_unsigned = 0;
					statsEncodeInfo.rangeValueInfo.rangeUnsigned.endRange_unsigned = 4294967295;

					encodeStr += encodeStats(statsEncodeInfo,RANGE_UNSIGNED);
					
				}
				else if(strstr(tmpStr.c_str(),"TxMode"))
				{
					statsEncodeInfo.rangeValueInfo.rangeUnsigned.startRange_unsigned = 1;
					statsEncodeInfo.rangeValueInfo.rangeUnsigned.endRange_unsigned = 7;

					encodeStr += encodeStats(statsEncodeInfo,RANGE_UNSIGNED);
					
				}
				else if(strstr(tmpStr.c_str(),"DeviceCinr"))
				{

					u32SignBit = 1;
					u32IntegerBits = 15;
					u32FractionBits = 16;
					double devicecinr;
					devicecinr = signedToFloat(statValue,u32SignBit,u32IntegerBits,u32FractionBits);

					statsEncodeInfo.rangeValueInfo.rangeFloat.statValue = (float)devicecinr;

					statsEncodeInfo.rangeValueInfo.rangeFloat.startRange_float = -6;
					statsEncodeInfo.rangeValueInfo.rangeFloat.endRange_float = 42;

					encodeStr += encodeStats(statsEncodeInfo,RANGE_UNSIGNED);

					m_pLTE_NM->mNMInfo.m_nBroadcomStoredStats.deviceCinr = (float)devicecinr;
					
				}
				else if(strstr(tmpStr.c_str(),"Cal"))
				{


					statsEncodeInfo.rangeValueInfo.rangeUnsigned.statValue = statValue;
					statsEncodeInfo.rangeValueInfo.rangeUnsigned.startRange_unsigned = 0;
					statsEncodeInfo.rangeValueInfo.rangeUnsigned.endRange_unsigned = 1;

					encodeStr += encodeStats(statsEncodeInfo,RANGE_UNSIGNED);


				}
				
			}
		}

		strncpy(m_pLTE_NM->mNMInfo.m_nBroadcomStats.phyStats,encodeStr.c_str(),encodeStr.size());
		m_pLTE_NM->mNMInfo.m_nBroadcomStats.phyStats[encodeStr.size()] = '\0';
		int size = encodeStr.size();

	}

}




	

void UBroadcomParser::getAnyFormatIPAddrSubnetMaskAddr(string &tempStr1,string &ipAddress,string &subnetMaskAddress,int dotCountBoundry)
{
	//string s = rspStr;
	string::size_type prev_pos = 0, pos = 0;
	ipAddress = "";
	subnetMaskAddress = "";
	int loopCount = 0;
	while( (pos = tempStr1.find('.', pos+1)) != string::npos  && loopCount < dotCountBoundry-1)    
	{         
		loopCount++;
	}
		ipAddress = tempStr1.substr(prev_pos, (pos-prev_pos) ) ;
		replaceAll(ipAddress,"\"","");
		replaceAll(ipAddress," ","");

		subnetMaskAddress = tempStr1.substr(pos+1,tempStr1.size() -1 );
		replaceAll(subnetMaskAddress,"\"","");
		replaceAll(subnetMaskAddress," ","");

		//string substring( tempStr1.substr(prev_pos, pos+1) );
		//if(loopCount < dotCountBoundry)
		//{
		//	if(loopCount ==0)
		//		substring = substring.substr(2);
		//	if(loopCount == (dotCountBoundry-1))
		//		substring = substring.substr(0,(substring.size()-1));
		//	ipAddress += substring;
		//}
		//else
		//{
		//	subnetMaskAddress += substring;
		//}
		//vStr.push_back(substring);      
	//	prev_pos = ++pos;
	//	++loopCount;
	//}     
	//string substring( tempStr1.substr(prev_pos, pos-prev_pos) ); 
//	substring = substring.substr(0,(substring.size()-1));
//	subnetMaskAddress += substring;
	//vStr.push_back(substring);
	//return vStr;
}

void UBroadcomParser::parseIPAddress(string &respStr)
{
	string tempStr =  respStr;
	string tempStr1 =  "";
	vector<string> vtempStr;
	vector<string> vDotsCount;
	
	strcpy(m_pLTE_NM->mNMInfo.sec_dns,"");
	strcpy(m_pLTE_NM->mNMInfo.secv6_dns,"");
	/*tokenise parameters here*/
	tokeniseInformation(tempStr,vtempStr);
	int noOfEntries = vtempStr.size();

	int nDotCount = 0;
	int ipv6Format = 0;

	for(int i=2; i<(noOfEntries-1); ++i)
	{

		if(i == 2)
		{
			/*For getting APN name from vStr[2]*/
			strcpy(m_pLTE_NM->mNMInfo.apn_name,vtempStr[i].c_str());
			//cout << "APN Name:" <<m_pLTE_NM->mNMInfo.apn_name << endl;
		}
		else if(i == 3)
		{
			/*to get the no. of dots in the ipaddress*/
			tempStr1 = vtempStr[i];
			tokeniseInformation(tempStr1,vDotsCount,'.');

			nDotCount = vDotsCount.size();
			if(nDotCount <= 8)
			{
				ipv6Format = 0;
			}
			else
			{
				ipv6Format = 1;
			}

			if(ipv6Format == 0)
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
			}
			else if(ipv6Format == 1)
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
			}
		}
		else if(i == 4)
		{
			/*For getting gateway address from vStr[4]*/
			if(ipv6Format == 0)
			{
				int lenght = strlen(vtempStr[i].c_str());
				strcpy(m_pLTE_NM->mNMInfo.gw_address,vtempStr[i].c_str());
				//cout << "gateway address:" << m_pLTE_NM->mNMInfo.gw_address << endl;
			}
			else if(ipv6Format == 1)
			{
				strcpy(m_pLTE_NM->mNMInfo.gwv6_address,vtempStr[i].c_str());
				//cout << "gateway address:" << m_pLTE_NM->mNMInfo.gwv6_address << endl;
			}
		}
		else if(i == 5)
		{
			/*For getting DNS_prim_addr from vStr[5]*/
			if(ipv6Format == 0)
			{
				strcpy(m_pLTE_NM->mNMInfo.pri_dns,vtempStr[i].c_str());
				strcpy(m_pLTE_NM->mNMInfo.dyn_dns, m_pLTE_NM->mNMInfo.pri_dns);	
				//cout << "Primary dns address:" << m_pLTE_NM->mNMInfo.pri_dns << endl;
			}
			else if(ipv6Format == 1)
			{
				strcpy(m_pLTE_NM->mNMInfo.priv6_dns,vtempStr[i].c_str());
				//cout << "Primary dns address:" << m_pLTE_NM->mNMInfo.priv6_dns << endl;
			}
		}
		else if(i == 6)
		{
			/*For getting DNS_sec_addr from vStr[6]*/
			if(ipv6Format == 0)
			{
				strcpy(m_pLTE_NM->mNMInfo.sec_dns,vtempStr[i].c_str());
				//cout << "Secondary dns address:" << m_pLTE_NM->mNMInfo.sec_dns << endl;
			}
			else if(ipv6Format == 1)
			{
				strcpy(m_pLTE_NM->mNMInfo.secv6_dns,vtempStr[i].c_str());
				//cout << "Secondary dns address:" << m_pLTE_NM->mNMInfo.secv6_dns << endl;
			}
		}
		else if(i == 7)
		{
			/*For getting P_CSCF_prim_addr from vStr[7]*/
			if(ipv6Format == 0)
			{
				strcpy(m_pLTE_NM->mNMInfo.p_cscf_prim_addr,vtempStr[i].c_str());
				//cout << "p_cscf_prim_addr:" << m_pLTE_NM->mNMInfo.p_cscf_prim_addr << endl;
			}
			else if(ipv6Format == 1)
			{
				strcpy(m_pLTE_NM->mNMInfo.p_cscfv6_prim_addr,vtempStr[i].c_str());
				//cout << "p_cscf_prim_addr:" << m_pLTE_NM->mNMInfo.p_cscfv6_prim_addr << endl;
			}
		}
		else if(i == 8)
		{
			///*For getting P_CSCF_sec_addr from vStr[8]*/
			if(ipv6Format == 0)
			{
				strcpy(m_pLTE_NM->mNMInfo.p_cscf_sec_addr,vtempStr[i].c_str());
				//cout << "P_CSCF_sec_addr:" << m_pLTE_NM->mNMInfo.p_cscf_sec_addr << endl;
			}
			else if(ipv6Format == 1)
			{
				strcpy(m_pLTE_NM->mNMInfo.p_cscfv6_sec_addr,vtempStr[i].c_str());
				//cout << "P_CSCF_sec_addr:" << m_pLTE_NM->mNMInfo.p_cscfv6_sec_addr << endl;
			}
		}
	}

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
}