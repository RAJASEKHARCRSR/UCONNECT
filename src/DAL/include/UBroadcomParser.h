#ifndef __UBROADCOMPARSER_H
#define __UBROADCOMPARSER_H

#include "UBufferParser.h"
#include "UBroadcomParser.h"

class UBroadcomParser : public UBufferParser
{

private:
	void parseSTATISTICs(vector<std::string>& vStr);

public:

	UBroadcomParser(UNetworkManager* p_NM);

	

	void processUnSolResp(string &unSolicitedRespStr);

	void processAtCommandResp(string &atCmdRespStr);

	void getAnyFormatIPAddrSubnetMaskAddr(string &tempStr1,string &ipAddress,string &subnetMaskAddress,int dotCountBoundry);
	
	void parseIPAddress(string &respStr);
	
		
};

#endif