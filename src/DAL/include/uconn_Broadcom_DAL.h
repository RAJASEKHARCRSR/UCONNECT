#ifndef BROADCOM_DAL_
#define BROADCOM_DAL_
#include "uconn_AT_DAL.h"




class uconnBroadcomDal : public uconnAtDal
{

private:

	DAL_STATUS	uconnIMSConfig(U_UINT32 cfgType,
						   U_UINT32 cfgValue);

	DAL_STATUS	uconnIMSResponse(U_UINT32 rspMode);
	DAL_STATUS uconnATStatsUpdate(U_UINT32 statCmd);



	DAL_STATUS uconnATPsCtxCfgSet(U_UINT32 bit_fields,
							   U_UINT32 ctx_id,
							   U_PS_CTX_CFG_TYPE cfg_type,
							   U_PS_CTX_CFG *ctx_cfg);

	DAL_STATUS uconnATpropAPNTableSet(U_APN_TABLE* apn_table);

	DAL_STATUS uconnATpropAPNTableGet();

	DAL_STATUS uconnIMSNWSupport();

	DAL_STATUS uconnATpropAPNTableDelete(U_APN_TABLE* apn_table);

public:
	
	uconnBroadcomDal(UNetworkManager* p_NM);
	~uconnBroadcomDal();  

	void virtual processBufferParserSetEvent(string strResp,unsigned int cmdId);

	DAL_STATUS uconnGetVersionInfo();

	U_BOOL isDevicePresent();

	DAL_STATUS  uconnConfigure(CONFIG_CODE configCode);

	DAL_STATUS uconnSetStatus(SET_STATUS setState,void* params,int ctxId=1);
	DAL_STATUS  uconnGetStatistics(GET_STATISTICS,void*);
	DAL_STATUS	uconnATModemConfig(U_UINT32 mConfig,
								U_UINT32 ctxId);

	DAL_STATUS uconnReset(void* param);
	DAL_STATUS uconnATpropIPIFStatus(U_UINT32 ctx_id,U_IP_IF_STATUS IF_Status);
	DAL_STATUS uconnConfigNetParams(NETWORK_PARAMS,void*,int ctxId=1);
};

#endif