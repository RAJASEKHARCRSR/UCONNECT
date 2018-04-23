#ifndef ALTAIR_DAL_
#define ALTAIR_DAL_
#include "uconn_AT_DAL.h"

class uconnAltairDal : public uconnAtDal{

private:

	
	DAL_STATUS	uconnCheckConnStatus();

	DAL_STATUS	uconnInitInterface(string initStr);

	DAL_STATUS	uconnGetPINInfo();

	DAL_STATUS	uconnReqStats(string initStr);

	DAL_STATUS uconnSTAT_L1A();

	DAL_STATUS uconnSTAT_MEAS();

	DAL_STATUS	uconnSetFreq();

	DAL_STATUS uconnGetGenInfo();

	

protected:

	
public:
	
	
	uconnAltairDal(UNetworkManager* p_NM);
	~uconnAltairDal();

	U_BOOL isDevicePresent();
	

	void virtual processBufferParserSetEvent(string strResp,unsigned int cmdId);

	//DAL_STATUS  uconnPowerUp();

	DAL_STATUS  uconnGetStatistics(GET_STATISTICS,void*);

	DAL_STATUS	uconnATModemConfig(U_UINT32 mConfig,
								U_UINT32 ctxId);

	DAL_STATUS uconnReset(void* param);

	DAL_STATUS uconnConfigNetParams(NETWORK_PARAMS,void*,int ctxId=1);

	DAL_STATUS uconnConfigure(CONFIG_CODE configCode);

	DAL_STATUS uconnGetVersionInfo();
	
};

#endif

