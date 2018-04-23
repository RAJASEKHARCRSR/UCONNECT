#include "XMLParser.h"
#include "uconn_OSAL.h"
#include "UCommonFunctions.h"
#include <vector>
#include <fstream>
#include <sstream>


void XMLParser::toLowerCase(string &str)
{
	const int length = str.length();
	for(int i=0; i < length; ++i)
	{
		str[i] = tolower(str[i]);
	}
}

bool XMLParser::readTillChar(ifstream &readCfgFile,char ch)
{
	readString.clear();
	char ch1 = readCfgFile.get();
	while( (ch1 != ch) && readCfgFile.good())
	{
		readString += ch1;
		ch1 = readCfgFile.get();
	}
	if(ch1 == ch)
	{
		readString += ch1;
		return true;
	}
	else
	{
		readString += ch1;
		return false;
	}
}

void XMLParser::parseConfigFile()
{
	string::iterator inpIt1;
	string::iterator inpIt2;

	bool ret = false;
	string configFilePath = getExecDirectory("CMsocketservice.exe") + "configFile.xml";
	 
	gCMInfo.isLTE_Enabled = false;
	gCMInfo.isThreeG_Enabled = false;
	gCMInfo.isWIFI_Enabled = false;

	ifstream readCfgFile(configFilePath.c_str());
	if (readCfgFile.is_open())
	{
		while ( readCfgFile.good() )
		{
			readString.clear();
			ret = false;
			ret = readTillChar(readCfgFile,'<');
			//configFileContent += readString;
			if(ret == true)
			{
				unsigned int filepos  = readCfgFile.tellg();
				readCfgFile.seekg(filepos -1);
				inpIt1 = readString.begin();
				inpIt2 = readString.end()-1;
				configFileContent.append(inpIt1,inpIt2);
			}
			else
			{
				inpIt1 = readString.begin();
				inpIt2 = readString.end();
				configFileContent.append(inpIt1,inpIt2);
			}

			ret = readTillChar(readCfgFile,'>');
			configFileContent += readString;
			if(ret == false)
			{
				continue;
			}

			if(strstr(readString.c_str(), "<param "))
			{
				pos = readString.find("\"",0);
				paramName = readString.substr(pos+1,readString.length());
				pos = paramName.find("\"",0);
				paramName = paramName.substr(0,pos);
				////cout << "Before convert of paramName::" << paramName << endl;
				//toLowerCase(paramName);
				////cout << "After convert of paramName::" << paramName << endl;
			}
			else if(strstr(readString.c_str(), "<value>"))
			{
				ret = false;
				ret = readTillChar(readCfgFile,'>');
				configFileContent += readString;
				pos = readString.find("<",0);
				paramValue.clear();
				paramValue = readString.substr(0,pos);
				updateCMInfo();
			}
			else if(strstr(readString.c_str(), "<enable_Tech>"))
			{
				ret = false;
				ret = readTillChar(readCfgFile,'>');
				configFileContent += readString;
				pos = readString.find("<",0);
				/////////////
				string encodedEnablerlist;
				encodedEnablerlist = readString.substr(0,pos);
				gCMInfo.enabledList.clear();
				gCMInfo.enabledList = encodedEnablerlist;
				/////////////
				string tech_name;
				tech_name = readString.substr(0,pos);
				vector<string> vStr, temp_str;
				tokeniseInformation(tech_name, vStr, ';');
				for(U_UINT32 i = 0; i < vStr.size(); i++)
				{
					tokeniseInformation(vStr[i], temp_str, ',');
					if(strstr(temp_str[0].c_str(),"0") && 
						strstr(temp_str[1].c_str(),"1"))
					{
						gCMInfo.isLTE_Enabled = true;
					}
					else if(strstr(temp_str[0].c_str(),"1") && 
						strstr(temp_str[1].c_str(),"1"))
					{
						gCMInfo.isThreeG_Enabled = true;
					}
					else if(strstr(temp_str[0].c_str(),"2") && 
						strstr(temp_str[1].c_str(),"1"))
					{
						gCMInfo.isWIFI_Enabled = true;
					}
				}
				vStr.clear();
				temp_str.clear();
			}
			else if(strstr(readString.c_str(), "<Tech_Priority>"))
			{
				ret = false;
				ret = readTillChar(readCfgFile,'>');
				configFileContent += readString;
				pos = readString.find("<",0);
				string encodedPlist;
				encodedPlist = readString.substr(0,pos);
				gCMInfo.priorityList.clear();
				gCMInfo.priorityList = encodedPlist;
			}
			else if(strstr(readString.c_str(), "<BM_Mode>"))
			{
				ret = false;
				ret = readTillChar(readCfgFile,'>');
				configFileContent += readString;
				pos = readString.find("<",0);
				string mode = readString.substr(0,pos);
				if(strstr(mode.c_str(),"Auto"))
				{
					gCMInfo.isManualMode = false;
				}
				else
				{
					gCMInfo.isManualMode = true;
				}

			}

			else if(strstr(readString.c_str(), "<LTE_Cfg>"))
			{
				particularNM = 1;
			}
			else if(strstr(readString.c_str(), "<3G_Cfg>"))
			{
				particularNM = 2;
			}
			else if(strstr(readString.c_str(), "<WiFi_Cfg>"))
			{
				particularNM = 3;
			}
			else if(strstr(readString.c_str(), "</LTE_Cfg>") || 
				strstr(readString.c_str(), "</3G_Cfg>") ||
				strstr(readString.c_str(), "</3G_Cfg>") )
			{
				particularNM = 0;
			}
		}
		readCfgFile.close();
	}
	//else 
		//cout << "Unable to open file"; 
}


void XMLParser::updateCMInfo()
{
	if(strstr(paramName.c_str(), "PLMNselectmode"))
	{
		if(!_strnicmp(paramValue.c_str(), "Auto", strlen("Auto")))
			gCMInfo.lteConfigInfo.PLMNselectmode = AUTO;
		else if(!_strnicmp(paramValue.c_str(), "Manual", strlen("Manual")))
			gCMInfo.lteConfigInfo.PLMNselectmode = MANUAL;
		else
			gCMInfo.lteConfigInfo.PLMNselectmode = AUTO;
	}
	else if(strstr(paramName.c_str(), "ENABLE4G"))
	{
		if(!_strnicmp(paramValue.c_str(), "ON", strlen("ON")))
			gCMInfo.lteConfigInfo.Enable4G = ON_4G;
		else if(!_strnicmp(paramValue.c_str(), "OFF", strlen("OFF")))
			gCMInfo.lteConfigInfo.Enable4G = OFF_4G;
		else
			gCMInfo.lteConfigInfo.Enable4G = ON_4G;
	}
	else if(strstr(paramName.c_str(), "AttachRAT"))
	{
		gCMInfo.lteConfigInfo.AttachRAT = atoi(paramValue.c_str());
	}
	else if(strstr(paramName.c_str(), "DisableRATs"))
	{
		gCMInfo.lteConfigInfo.DisableRATs = atoi(paramValue.c_str());
	}
	else if(strstr(paramName.c_str(), "LogLevel"))
	{
		if(!_strnicmp(paramName.c_str(), "Debug", strlen("Debug")))
			gCMInfo.lteConfigInfo.LogLevel = atoi("0");
		else if(!_strnicmp(paramName.c_str(), "Info", strlen("Info")))
			gCMInfo.lteConfigInfo.LogLevel = atoi("1");
		else if(!_strnicmp(paramName.c_str(), "Error", strlen("Error")))
			gCMInfo.lteConfigInfo.LogLevel = atoi("2");
		else
			gCMInfo.lteConfigInfo.LogLevel = atoi("0");
	}
	else if(strstr(paramName.c_str(), "TestMode"))
	{
		if(!_strnicmp(paramValue.c_str(), "ENABLE", strlen("ENABLE")))
			gCMInfo.lteConfigInfo.TestMode = 1;
		else if(!_strnicmp(paramValue.c_str(), "DISABLE", strlen("DISABLE")))
			gCMInfo.lteConfigInfo.TestMode = 0;
		else
			gCMInfo.lteConfigInfo.TestMode = 0;
	}
	else if(strstr(paramName.c_str(), "SMSWriteUicc"))
	{
		gCMInfo.lteConfigInfo.SMSWriteUicc = atoi(paramValue.c_str());
	}
	else if(strstr(paramName.c_str(), "SMSFormat"))
	{
		gCMInfo.lteConfigInfo.SMSFormat = atoi(paramValue.c_str());
	}
	else if(strstr(paramName.c_str(), "SMSOverIMS"))
	{
		gCMInfo.lteConfigInfo.SMSOverIMS = atoi(paramValue.c_str());
	}
	else if(strstr(paramName.c_str(), "T1_Timer"))
	{
		gCMInfo.lteConfigInfo.T1_Timer = atoi(paramValue.c_str());
	}
	else if(strstr(paramName.c_str(), "T2_Timer"))
	{
		gCMInfo.lteConfigInfo.T2_Timer = atoi(paramValue.c_str());
	}
	else if(strstr(paramName.c_str(), "TF_Timer"))
	{
		gCMInfo.lteConfigInfo.TF_Timer = atoi(paramValue.c_str());
	}
	else if(strstr(paramName.c_str(), "LogtoFile"))
	{
		gCMInfo.lteConfigInfo.LogtoFile = atoi(paramValue.c_str());
	}
	else if(strstr(paramName.c_str(), "LogFile"))
	{
		gCMInfo.lteConfigInfo.LogFile = paramValue;
	}
	else if(strstr(paramName.c_str(), "VidPidName"))
	{
		if(particularNM == 1)
		{
			if(gCMInfo.lteConfigInfo.vidpidCount <= 2)
				gCMInfo.lteConfigInfo.VidPidName[gCMInfo.lteConfigInfo.vidpidCount++] = paramValue;
		}
		else if(particularNM == 2)
		{
			if(gCMInfo.threeGConfigInfo.ThreeG_vidpidCount <= 2)
				gCMInfo.threeGConfigInfo.ThreeG_VidPidName[gCMInfo.threeGConfigInfo.ThreeG_vidpidCount++] = paramValue;
		}
	}
	else if(strstr(paramName.c_str(), "ComPortHWId"))
	{
		if(particularNM == 1)
		{
				gCMInfo.lteConfigInfo.comPortHWId = paramValue;
		}
	}
	else if(strstr(paramName.c_str(), "PDPType"))
	{
      
		if(!_strnicmp(paramValue.c_str(), "IPv4v6", strlen("IPv4v6")))
        {
			gCMInfo.lteConfigInfo.PDPType = PDP_IPV4V6;
        }
		else if(!_strnicmp(paramValue.c_str(), "IPv6", strlen("IPv6")))
        {
			gCMInfo.lteConfigInfo.PDPType = PDP_IPV6;
        }
    
		else if(!_strnicmp(paramValue.c_str(), "IPv4", strlen("IPv4")))
        {
			gCMInfo.lteConfigInfo.PDPType = PDP_IPV4;
        }
		else
			gCMInfo.lteConfigInfo.PDPType = PDP_IP;
	}
	else if(strstr(paramName.c_str(), "EmulatorMode"))
	{
		if(!_strnicmp(paramValue.c_str(), "1", strlen("1")))
		{
			gCMInfo.lteConfigInfo.EmulatorMode = true;
		}
		else
		{
			gCMInfo.lteConfigInfo.EmulatorMode = false;
		}
	}
	else if(strstr(paramName.c_str(), "DalMode"))
	{
		gCMInfo.lteConfigInfo.DalMode = atoi(paramValue.c_str());
	}
	else if(strstr(paramName.c_str(), "EARCFN"))
	{
		gCMInfo.lteConfigInfo.EARCFN1 = atoi(paramValue.c_str());
	}
	else if(strstr(paramName.c_str(), "InactivityTimer"))
	{
		gCMInfo.lteConfigInfo.InactivityTimer = atoi(paramValue.c_str());
	}
	else if(strstr(paramName.c_str(), "PowerDown"))
	{
		gCMInfo.lteConfigInfo.isPowerdown = atoi(paramValue.c_str());
	}
	else if(strstr(paramName.c_str(), "IMSRegOnly"))
	{
		gCMInfo.lteConfigInfo.IMSAttachOnly = atoi(paramValue.c_str());
	}
	else if(strstr(paramName.c_str(), "APNNameIMS"))
	{
		gCMInfo.lteConfigInfo.APNNameIMS = paramValue;
#if 0
		//cout << "gCMInfo.PLMNselectmode::" << gCMInfo.PLMNselectmode << endl;
		//cout << "gCMInfo.Enable4G::" << gCMInfo.Enable4G << endl;
		//cout << "gCMInfo.AttachRAT::" << gCMInfo.AttachRAT << endl;
		//cout << "gCMInfo.DisableRATs::" << gCMInfo.DisableRATs << endl;
		//cout << "gCMInfo.LogLevel::" << gCMInfo.LogLevel << endl;
		//cout << "gCMInfo.TestMode::" << gCMInfo.TestMode << endl;
		//cout << "gCMInfo.SMSWriteUicc::" << gCMInfo.SMSWriteUicc << endl;
		//cout << "gCMInfo.SMSFormat::" << gCMInfo.SMSFormat << endl;
		//cout << "gCMInfo.SMSOverIMS::" << gCMInfo.SMSOverIMS << endl;
		//cout << "gCMInfo.T1_Timer::" << gCMInfo.T1_Timer << endl;
		//cout << "gCMInfo.T2_Timer::" << gCMInfo.T2_Timer << endl;
		//cout << "gCMInfo.TF_Timer::" << gCMInfo.TF_Timer << endl;
		//cout << "gCMInfo.LogtoFile::" << gCMInfo.LogtoFile << endl;

		//cout << "gCMInfo.LogFile::" << gCMInfo.LogFile << endl;
		//cout << "gCMInfo.PDPType::" << gCMInfo.PDPType << endl;
		//cout << "gCMInfo.EmulatorMode::" << gCMInfo.EmulatorMode << endl;
		//cout << "gCMInfo.DalMode::" << gCMInfo.DalMode << endl;
		//cout << "gCMInfo.EARCFN::" << gCMInfo.EARCFN1 << endl;
		//cout << "gCMInfo.InactivityTimer::" << gCMInfo.InactivityTimer << endl;
		//cout << "gCMInfo.APNNameIMS::" << gCMInfo.APNNameIMS << endl;
		//cout << "gCMInfo.APNName::" << gCMInfo.APNName << endl;

		//cout << "gCMInfo.VidPidName[0]::" << gCMInfo.VidPidName[0] << endl;
		//cout << "gCMInfo.VidPidName[1]::" << gCMInfo.VidPidName[1] << endl;
		//cout << "gCMInfo.VidPidName[2]::" << gCMInfo.VidPidName[2] << endl;
#endif
	}
	else if (strstr(paramName.c_str(), "3GAPNName"))
	{
		if(particularNM == 2)
		{
			gCMInfo.threeGConfigInfo.apnName = paramValue;
			strcpy(rasDetails.apnName,paramValue.c_str());
		}
	}
	else if(strstr(paramName.c_str(), "APNName"))
	{
		gCMInfo.lteConfigInfo.APNName = paramValue;
	}
	else if(strstr(paramName.c_str(), "RASProfile"))
	{
		if(particularNM == 2)
		{
			gCMInfo.threeGConfigInfo.rasProfileName = paramValue;
			strcpy(rasDetails.rasProfileName,paramValue.c_str());
		}
	}
	else if(strstr(paramName.c_str(), "UserName"))
	{
		if(particularNM == 2)
		{
			gCMInfo.threeGConfigInfo.rasUsrName = paramValue;
			strcpy(rasDetails.rasUsrName,paramValue.c_str());
		}
	}
	else if(strstr(paramName.c_str(), "PassWord"))
	{
		if(particularNM == 2)
		{
			gCMInfo.threeGConfigInfo.rasPasswd = paramValue;
			strcpy(rasDetails.rasPasswd,paramValue.c_str());
		}
	}
	else if (strstr(paramName.c_str(), "PhoneNumber"))
	{
		if(particularNM == 2)
		{
			gCMInfo.threeGConfigInfo.rasPhoneNo = paramValue;
			strcpy(rasDetails.rasPhoneNumber,paramValue.c_str());
		}
	}
	else if (strstr(paramName.c_str(), "USSDDecodingMode"))
	{
		if(particularNM == 2)
		{
			if(!_strnicmp(paramValue.c_str(), "1", strlen("1")))
			{
				gCMInfo.threeGConfigInfo.UssdDecodingReqd = true;
			}
			else
			{
				gCMInfo.threeGConfigInfo.UssdDecodingReqd =  false;
			}
		}
	}
}


void XMLParser::updateStr(string &dumpStr,string startTag,string endTag,string &str)
{
	////cout << "Old dump is:\n" << dumpStr << endl;
	string tempStr;
	strBeforeStartTagMatch.clear();
	strAfterMatch.clear();
	int iPos = 0;

	if( ((iPos = dumpStr.find(startTag,0)) != string::npos))
	{
		strBeforeStartTagMatch = dumpStr.substr(0,iPos+(startTag.length()));
		strAfterMatch = dumpStr.substr(iPos+(startTag.length()));
	
		iPos = 0;
		if( ((iPos = strAfterMatch.find(endTag,0)) != string::npos))
		{
			tempStr = strAfterMatch.substr(0,iPos);
			strAfterEndTagMatch = strAfterMatch.substr(iPos);

			dumpStr.clear();
			dumpStr = strBeforeStartTagMatch;
			dumpStr += str;
			dumpStr += strAfterEndTagMatch;
		}
	}
}


void XMLParser::writeToConfigFile()
{
	string currentWorkingDir = getExecDirectory("CMsocketservice.exe");
	string tempConfigFilePath = currentWorkingDir + "configFileTemp.xml";
	string oldConfigFileNameWithPath = currentWorkingDir + "configFile.xml";
	ofstream out(tempConfigFilePath.c_str(), ios::out);

	if(!out) 
	{
		//cout << "Cannot open output file.\n";
		//return 1;
	}
	out.write(configFileContent.c_str(), configFileContent.length());

	out.close();

	remove(oldConfigFileNameWithPath.c_str());
	rename(tempConfigFilePath.c_str(),oldConfigFileNameWithPath.c_str());
}

string XMLParser::convertIntToString(int number)
{
#if 1
	char Result[16]; // string which will contain the number
	sprintf ( Result, "%d", number ); // %d makes the result be a decimal integer
	string res = Result;
	return res;
#endif
}

void XMLParser::updateValue(string& id,string& value,bool isValueTag)
{

	int idPos = 0;
	string configFilePath = getExecDirectory("CMsocketservice.exe") + "configFile.xml";
	fstream readCfgFile(configFilePath.c_str());
	std::stringstream buffer;
	
	buffer  << readCfgFile.rdbuf();
	readCfgFile.close();
	string fileStr = buffer.str();


	string beforeValue = "";
	string afterValue = "";

	if( ((idPos = fileStr.find(id,0)) != string::npos))
	{
		int valPos = 0;
		string valStr = "";
		if(isValueTag)
		{
			valStr = "<value>";
		}
		else
		{
			valStr = "<"+id+">";
			idPos=0;
		}
		if(((valPos = fileStr.find(valStr,idPos)) != string::npos))
		{
			valPos += valStr.size();
			beforeValue = fileStr.substr(0,valPos);
			int valLen = 0;
			string endTag = "";
			if(isValueTag)
			{
				endTag = "</value>";
			}
			else
			{
				endTag = "</"+id+">";
			}

			if(((valLen = fileStr.find(endTag,valPos)) != string::npos))
			{
				afterValue = fileStr.substr(valLen,fileStr.size());
				valLen -= valPos;
			}
			fileStr = beforeValue + value + afterValue;
			configFileContent = fileStr;
			writeToConfigFile();
			return;
		}
	}
}
