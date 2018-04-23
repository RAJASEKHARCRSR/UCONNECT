#ifndef _UBUFFERPARSER_H
#define _UBUFFERPARSER_H

#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <stdlib.h>
#include "uconn_Config.h"
#include "LTE_NM.h"
#include "UCommonFunctions.h"

using namespace std;


enum STATS_RANGE_CODE {RANGE_NOT_REQUIRED,
					   RANGE_UNSIGNED,
					   RANGE_SIGNED,
					   RANGE_FLOAT};


typedef struct EncodedStats {

	int level :3;
	int valuePresent: 1;
	int rangeFlag: 2;
	int reserved: 2;

}EncodedStats_s;


typedef struct RangeUnsigned
{
	unsigned int startRange_unsigned;
	unsigned int endRange_unsigned;
	unsigned int statValue;

}RangeUnsigned_s;

typedef struct RangeSigned
{
	int startRange_signed;
	int endRange_signed;
	int statValue;

}RangeSigned_s;

typedef struct RangeFloat
{
	float startRange_float;
	float endRange_float;
	float statValue;

}RangeFloat_s;

union RangeValueInfo
{
	RangeSigned_s rangeSigned;
	RangeUnsigned_s rangeUnsigned;
	RangeFloat_s rangeFloat;

};


typedef struct StatsEncodeInfo {

	EncodedStats_s encodedStats;
	RangeValueInfo rangeValueInfo;
	string statName;
	string statValueStr;

}StatsEncodeInfo_s;






class UCM_ParsedInfo
{



public:
	UCM_ParsedInfo()
	{
		modem_state = 0;
		NetworkAccessType = 0xFFFF;
		rssiValue = 0;
		cinrValue = 0;
		NetworkName.clear();
		USSDRsp.clear();
		powerup_state = 0;
	};
	~UCM_ParsedInfo(){};
	void setModem_State(U_UINT32 mState)
	{
		modem_state = mState;
	}
	U_UINT32 getModem_State()
	{
		return modem_state;
	}

	void setNetworkAccessType(U_UINT32 NetworkAccTyp)
	{
		NetworkAccessType = NetworkAccTyp;
	}

	U_UINT32 getNetworkAccessType()
	{
		return NetworkAccessType;
	}

	void setRSSI(int mRSSI)
	{
		rssiValue = mRSSI;
	}
	int geRSSI()
	{
		return rssiValue;
	}

	void setCINR(int mCINR)
	{
		cinrValue = mCINR;
	}
	int getCINR()
	{
		return cinrValue;
	}
	
	void setPowerup_state(U_UINT32 powerState)
	{
		powerup_state = powerState;
	}
	U_UINT32 getPowerup_state()
	{
		return powerup_state;
	}

	void setNetworkName(string mNetworkName)
	{
		NetworkName = mNetworkName;
	}
	string getNetworkName()
	{
		return NetworkName;
	}

	void setVersionInfo(string mVersionInfo)
	{
		strncpy(parsed_VersionInfo.u8VersionString,mVersionInfo.c_str(),mVersionInfo.size());
		parsed_VersionInfo.u8VersionString[mVersionInfo.size()]='\0';
	}
	string getVersionInfo()
	{
		return parsed_VersionInfo.u8VersionString;
	}

	void setUSSDRsp(string USSDString)
	{
		USSDRsp = USSDString;
	}
	string getUSSDRsp()
	{
		return USSDRsp;
	}

	Version_Info_s parsed_VersionInfo;
	
private:
	U_UINT32 modem_state;
	U_UINT32 NetworkAccessType;
	U_UINT32 rssiValue;
	U_UINT32 cinrValue;
	string NetworkName;
	string USSDRsp;
	U_UINT32 powerup_state;
};


class UBufferParser
{
protected:
	string versionString; 

	string convertStatsToHexString(U_UINT8 code);

	string encodeStats(StatsEncodeInfo& statsEncodeInfo,STATS_RANGE_CODE rangeCode = RANGE_NOT_REQUIRED);

	void parseIPAddress(string &respStr);

        void getAnyFormatIPAddrSubnetMaskAddr(string &tempStr1,string &ipAddress,string &subnetMaskAddress,int dotCountBoundry);

public:

	UCM_ParsedInfo parsedInfo;

	UBufferParser(UNetworkManager* p_NM);
	~UBufferParser();

	void adjustBuffer(const string &patString);
	void startParsingBuffer(string &str);
	
	U_UINT32 getEventID() 
	{
		return eventID;
	}
	void setEventID(U_UINT32 evtID)
	{
		eventID = evtID;
	}

	////////////////////////////////////////////////////
	void virtual processUnSolResp(string &unSolicitedRespStr);
	void virtual processAtCommandResp(string &atCmdRespStr);
	////////////////////////////////////////////////////

	int powerState;
	ULTE_NM* m_pLTE_NM;
	UNetworkManager* m_pNM;

protected:
	
	string pat1013String;
	string patOK1013String;
	string patERROR1013String;
	string genStr;
	string buffer;
	string processStr;
	U_UINT32 eventID;
	U_UINT32 iPos;

	void CommandParseForParameter(string &cmdStr, string &infoStr);
	void tokeniseInformation(string &rspStr, vector<std::string>& vStr);
	void tokeniseInformation(string &rspStr, vector<std::string>& vStr, char ch);
	void trimString(string &s1,string &charsToRemove);
};

#endif /*UBUFFERPARSER_H*/