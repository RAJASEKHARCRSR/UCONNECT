#ifndef THREE_G_DAL_H_
#define THREE_G_DAL_H_

#include "uconn_DAL.h"
#include "uconn_AT_DAL.h"

class UThreeGDal : public uconnAtDal
{
public:
	DAL_STATUS ATCommandInitNetworkSearch();
	DAL_STATUS ATCommandEchoOff();
	DAL_STATUS ATCommandSetAutomaticNetworkMode();
	DAL_STATUS ATCommandGetModemInfo();
	DAL_STATUS ATCommandSetFunctionality();
	DAL_STATUS ATCommandGetSignalStrength();
	DAL_STATUS ATCommandRegisterNetwork();
	DAL_STATUS ATCommandRegisterNetworkStatus();
	DAL_STATUS ATCommandSignalStrength();
	DAL_STATUS ATCommandUSSDStr(U_STRING ussd_str);
	DAL_STATUS ATCommandGetHardwareInfo();
	DAL_STATUS ATCommandGetSystemConfigInfo();

public:
	
	UThreeGDal(UNetworkManager* pThreeGNM);
	~UThreeGDal();  

	U_BOOL isDevicePresent();
	DAL_STATUS uconnGetVersionInfo();
	DAL_STATUS uconnATModemConfig(U_UINT32 mConfig, U_UINT32 ctxId);
	void processBufferParserSetEvent(string strResp,unsigned int cmdId);
	DAL_STATUS uconnConfigure(CONFIG_CODE configCode);
	DAL_STATUS uconnConfigNetParams(NETWORK_PARAMS,void*,int ctxId=1);
	DAL_STATUS uconnSetStatus(SET_STATUS,void*,int ctxId=1);
	DAL_STATUS uconnGetStatistics(GET_STATISTICS,void*);
	U_VOID initNetMngr(void *configInfo);
	DAL_STATUS uconnReset(void* param);

	void callBackHandler(string strResp,unsigned int cmdId);
		
};


#endif // THREE_G_DAL_H_