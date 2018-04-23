#ifndef __UHUAWEIPARSER_H
#define __UHUAWEIPARSER_H

#include "UBufferParser.h"
#include <cmath>


class UHuawei4GParser : public UBufferParser
{
public:

	UHuawei4GParser(UNetworkManager* p_NM);

	void processAtCommandResp(string &atCmdRespStr);
	void processUnSolResp(string &unSolicitedRespStr);

};


#endif //__UHUAWEIPARSER_H