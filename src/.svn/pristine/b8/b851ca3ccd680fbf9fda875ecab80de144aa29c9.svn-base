#include "UHuawei4GParser.h"


UHuawei4GParser::UHuawei4GParser(UNetworkManager* p_NM):UBufferParser(p_NM)
{
	
}


void UHuawei4GParser::processAtCommandResp(string &atCmdRespStr)
{
	vector<string> vStr;
	string tmpStr = "";
	setEventID(0);

	if(strstr(atCmdRespStr.c_str(),"+CFUN:"))
	{

		tokeniseInformation (atCmdRespStr,vStr, '\n' ) ;
		int size = vStr.size();
		for ( int i = 0; i<= size; i++ )
		{
			if(strstr(vStr[i].c_str(),"+CFUN:"))
			{
				tmpStr.clear();
				tmpStr = vStr[i];
				vStr.clear();
				tokeniseInformation(tmpStr,vStr,':');
				int size = vStr.size();
				/* Parsing of the +CFUN: response here */
				////cout << "The Response for AT command received is:" << atCmdRespStr << endl;
				if(vStr.size() >= 1)
				{
					parsedInfo.setPowerup_state(atoi(vStr[1].c_str()));
					////cout << "Power up State is:" <<vStr[1].c_str() << endl;
					setEventID(UCONN_POWERUP_RSP);
				}
		
				vStr.clear();
				break;
			}
		}
		/* Parsing of the +CFUN: response here */
		////cout << "The Response for AT command received is:" << atCmdRespStr << endl;
		
    }
	else if(strstr(atCmdRespStr.c_str(),"+CEREG:"))
	{
		tokeniseInformation (atCmdRespStr,vStr, '\n' ) ;
		int size = vStr.size();
		for ( int i = 0; i<= size; i++ )
		{
			if(strstr(vStr[i].c_str(),"+CFUN:"))
			{
				break;
			}
		}
	}
	else 
	{
		UBufferParser::processAtCommandResp(atCmdRespStr);
	}

}


void UHuawei4GParser::processUnSolResp(string &unSolicitedRespStr)
{

	vector<string> vStr;
	string tmpStr = "";
	setEventID(0);
	//printf("################# The EVT string received = %s #############\n", unSolicitedRespStr.c_str());
	
	if(strstr(unSolicitedRespStr.c_str(),"+CEREG:"))
	{
		tokeniseInformation (unSolicitedRespStr,vStr, '\n' ) ;
		int size = vStr.size();
		for ( int i = 0; i<= size; i++ )
		{
			if(strstr(vStr[i].c_str(),"+CFUN:"))
			{
				break;
			}
		}
	}
	
}
