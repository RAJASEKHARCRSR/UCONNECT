#include "UBufferParser.h"
#ifdef WIN32
#include "UEmulateSerialPort.h"
#endif
#include "uconn_Event.h"

#include <cmath>




UBufferParser::UBufferParser(UNetworkManager* p_NM)
{
	/* Initialise the NM pointer */
	m_pNM = p_NM;
	if(m_pNM->getTechID() == 0) /* For LTE NM */
	{
		m_pLTE_NM = reinterpret_cast<ULTE_NM*>(m_pNM);
	}

	powerState = 0;

	pat1013String = "";
	pat1013String += 13;//<CR>
	pat1013String += 10;//<LF>

	patOK1013String = "OK" + pat1013String;
	patERROR1013String = "ERROR" + pat1013String;

	genStr = "";
	buffer = "";
	processStr = "";
	iPos = 0;

	versionString.clear();

	//memset( &parsedInfo.stat, 0, sizeof( ParsedStats ) );
}
UBufferParser::~UBufferParser()
{
	m_pNM = NULL;
	m_pLTE_NM = NULL;
	pat1013String.clear();
	patOK1013String.clear();
	patERROR1013String.clear();
	genStr.clear();
	buffer.clear();
	processStr.clear();
}

void UBufferParser::adjustBuffer(const string &patString)
{
	genStr = "";
	genStr = buffer.substr(0,iPos);
	buffer.erase(0,genStr.length());
	buffer.erase(0,patString.length());
	genStr += patString;
	processStr += genStr;
	return;
}

void UBufferParser::CommandParseForParameter(string &cmdStr, string &infoStr)
{
#if 0
	vector<string> vStr;
	if(strstr(cmdStr.c_str(),"+CEREG:"))
	{
		tokeniseInformation(infoStr,vStr);
		for(int i=0;i<vStr.size();++i)
		{
			if(i==0)
			{
				parsedInfo.setModem_State(atoi(vStr[0].c_str()));
				vStr.clear();
				return;
			}
		}
	}
#endif
}

void UBufferParser::tokeniseInformation(string &rspStr, vector<std::string>& vStr)
{
	string s = rspStr;
	string::size_type prev_pos = 0, pos = 0;     
	while( (pos = s.find(',', pos)) != string::npos )     
	{         
		string substring( s.substr(prev_pos, pos-prev_pos) );
		vStr.push_back(substring);      
		prev_pos = ++pos;     
	}     
	string substring( s.substr(prev_pos, pos-prev_pos) ); 
	vStr.push_back(substring);
	return;
}

#if 0
vector<std::string>& UBufferParser::tokeniseInformation(string &rspStr, vector<std::string>& vStr, char ch)
{
	string s = rspStr;
	string sChar;
	sChar.clear();
	sChar += ch;
	string::size_type prev_pos = 0, pos = 0;     
	while( (pos = s.find(ch, pos)) != string::npos )     
	{         
		string substring( s.substr(prev_pos, pos-prev_pos) );
		substring += sChar;
		vStr.push_back(substring);      
		//vStr.push_back(sChar);
		prev_pos = ++pos;     
	}     
	string substring( s.substr(prev_pos, pos-prev_pos) ); 
	vStr.push_back(substring);
	return vStr;
}
#endif
#if 1
void UBufferParser::tokeniseInformation(string &rspStr, vector<std::string>& vStr, char ch)
{
	string s = rspStr;
	vStr.clear();
	string::size_type prev_pos = 0, pos = 0;     
	while( (pos = s.find(ch, pos)) != string::npos )     
	{         
		string substring( s.substr(prev_pos, pos-prev_pos) );
		vStr.push_back(substring);      
		prev_pos = ++pos;     
	}     
	string substring( s.substr(prev_pos, pos-prev_pos) ); 
	vStr.push_back(substring);
	return;
}
#endif

void UBufferParser::processUnSolResp(string &unSolicitedRespStr)
{

	setEventID(0);
	//printf("################# The EVT string received = %s #############\n", unSolicitedRespStr.c_str());
	
	
}





void UBufferParser::processAtCommandResp(string &atCmdRespStr)
{
	vector<string> vStr;
	string tmpStr = "";
	setEventID(0);

	if(strstr(atCmdRespStr.c_str(),"+CFUN:"))
	{
		/* Parsing of the +CFUN: response here */
		////cout << "The Response for AT command received is:" << atCmdRespStr << endl;
		tokeniseInformation(atCmdRespStr,vStr,':');
		if(vStr.size() >= 1)
        {
			parsedInfo.setPowerup_state(atoi(vStr[1].c_str()));
			////cout << "Power up State is:" <<vStr[1].c_str() << endl;
			setEventID(UCONN_POWERUP_RSP);
        }
		
		vStr.clear();
	}
	else if(strstr(atCmdRespStr.c_str(),"+CGATT:"))
	{
		////cout << "The Response for AT command received is:" << atCmdRespStr << endl;
		tokeniseInformation(atCmdRespStr,vStr,'\n');

		iPos = 0;
		string tmpStr = "";
		string valStr = "";
		if( ((iPos = vStr[1].find(":",0)) != string::npos))
		{
			tmpStr.clear();
			tmpStr = vStr[1].substr(0,iPos);
			valStr = vStr[1].substr(iPos+1);

			if(0 == atoi(valStr.c_str()))
			{
				/*Store it in CMInfo*/
				////cout << "PS is Detached" << endl;
				m_pLTE_NM->mNMInfo.Modem_state = 0;
				 
				setEventID(UCONN_EVT_NET_DETACH);
			}
			else if(1 == atoi(valStr.c_str()))
			{
				/*Store it in CMInfo*/
				////cout << "PS is Attached" << endl;
				m_pLTE_NM->mNMInfo.Modem_state = 1;
				setEventID(UCONN_EVT_PRIMARY_ACT);
			}
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



	
	vStr.clear();
	atCmdRespStr.clear();
	return;

}


string UBufferParser::convertStatsToHexString(U_UINT8 code) 
{

	string hexString = "";
	int hexCode=0;
	
	for(int i=0;i<4;i++)
	{
		if(code & 1)
		{
			hexCode += (int)pow((float)2,i);
		}
		code=code >> 1;
		
	}

	if(hexCode<10)
		hexString+=hexCode+48;
	else
		hexString+=hexCode+55;


	return hexString;


}

string UBufferParser::encodeStats(StatsEncodeInfo& statsEncodeInfo,STATS_RANGE_CODE rangeCode)
{

	if(rangeCode == RANGE_NOT_REQUIRED)
	{
		statsEncodeInfo.encodedStats.rangeFlag = 0;
	}

	else if(rangeCode == RANGE_SIGNED)
	{
		if(statsEncodeInfo.rangeValueInfo.rangeSigned.statValue<statsEncodeInfo.rangeValueInfo.rangeSigned.startRange_signed ||
			statsEncodeInfo.rangeValueInfo.rangeSigned.statValue>statsEncodeInfo.rangeValueInfo.rangeSigned.endRange_signed)
		{
			statsEncodeInfo.encodedStats.rangeFlag = 1;
			
		}
		else
		{
			statsEncodeInfo.encodedStats.rangeFlag = 0;
		}

	}
	else if(rangeCode == RANGE_UNSIGNED)
		{
		if(statsEncodeInfo.rangeValueInfo.rangeUnsigned.statValue<statsEncodeInfo.rangeValueInfo.rangeUnsigned.startRange_unsigned ||
			statsEncodeInfo.rangeValueInfo.rangeUnsigned.statValue>statsEncodeInfo.rangeValueInfo.rangeUnsigned.endRange_unsigned)
		{
			statsEncodeInfo.encodedStats.rangeFlag = 1;
		}
		else
		{
			statsEncodeInfo.encodedStats.rangeFlag = 0;
		}

	}

	else if(rangeCode == RANGE_FLOAT)
		{
		if(statsEncodeInfo.rangeValueInfo.rangeFloat.statValue<statsEncodeInfo.rangeValueInfo.rangeFloat.startRange_float||
				statsEncodeInfo.rangeValueInfo.rangeFloat.statValue>statsEncodeInfo.rangeValueInfo.rangeFloat.endRange_float)
		{
			statsEncodeInfo.encodedStats.rangeFlag = 1;
		}
		else
		{
			statsEncodeInfo.encodedStats.rangeFlag = 0;
		}

	}

	U_UINT8 code;

	string encodeStr = "";

	string value1 = "";
	string value2 = "";
	
	memcpy((void*)&code,(void*)&statsEncodeInfo.encodedStats,sizeof(statsEncodeInfo.encodedStats));

	value1 += convertStatsToHexString(code);
	code = code >> 4;
	value2 += convertStatsToHexString(code);

	encodeStr += value2+value1+";";

	encodeStr += statsEncodeInfo.statName;
	encodeStr += ";";

	if(statsEncodeInfo.encodedStats.valuePresent != 0)
	{
		encodeStr += statsEncodeInfo.statValueStr;
	}

	encodeStr += "\n";



	return encodeStr;

}


void UBufferParser::parseIPAddress(string &respStr)
{
	string tempStr =  respStr;
	string tempStr1 =  "";
	vector<string> vtempStr;
	vector<string> vDotsCount;

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

}


void UBufferParser::getAnyFormatIPAddrSubnetMaskAddr(string &tempStr1,string &ipAddress,string &subnetMaskAddress,int dotCountBoundry)
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

}







