#ifndef __UALTAIRPARSER_H
#define __UALTAIRPARSER_H

#include "UBufferParser.h"
#include "UAltairParser.h"

class UAltairParser : public UBufferParser
{

private:

	void parse_L1A_STATs(vector<std::string>& vStr);
	void parse_Meas(vector<std::string> &vStr);
	//void updateMeasurement(U_SINT32 &type, string &param,string &value,string &encodeStr);
	//void parse_NAS_STATs(vector<std::string>& vStr);
	//void parse_RRC_STATs(vector<std::string>& vStr);
	//void parse_Timer_STATs(vector<std::string>& vStr);
	

public:
	
	UAltairParser(UNetworkManager* p_NM);
	//UCM_ParsedInfo parsedInfo;
	//AltairStats altairStats;

	void processAtCommandResp(string &atCmdRespStr);
	void processUnSolResp(string &unSolicitedRespStr);
		
};

#endif