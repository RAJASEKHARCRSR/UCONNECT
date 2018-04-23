/*****************************************************************************
**
** uconn_bcm_DAL.h
** Implements Device Abstraction Layer for Beceem (Broadcom) Device
**
** Author: Sriram V Iyer
** Date  : 28-DEC-2010
** (c) United Mobile Apps Pvt Ltd (2010 - 2015)

    VER		  DATE		AUTHOR		DESCRIPTION
	0.1		28/12/10     SVI		Creation
	0.2		08/01/11	 SVI		Reorganize header into sections
****************************************************************************/

#ifndef UCONN_DAL_H_
#define UCONN_DAL_H_

/////////////////////////////////////////////////////////////////////////////
/////////////////////////// INCLUDE FILES ///////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#include "UTypes.h"
#include "uconn_Config.h"
#include "ULog.h"
#include "LTE_NM.h"


// new enums added
namespace UConnConfig {
	enum CONFIG_CODE {INIT_CONFIG,
					  POWER_UP,
					  SET_CONTEXT,
					  IMS_CONFIG,
					  IMS_RESPONSE,
					  IMS_CONFIG_DETACH,
					  VERSION_INFO,
					  IMS_CONFIG_TEST,
					  POWER_STATUS_QUERY,
					  SET_FREQ,
					  ENABLED_STATE_ENTRY
					  };
};

namespace UConnSetStatus {
enum SET_STATUS {SET_NET_STATUS,
				 IP_IF_STATUS_UP,
				 IP_IF_STATUS_DOWN,
				 ATP_CTX_SET,
				 POWER_DOWN};
}

namespace UConnGetStatus {
enum GET_STATUS {GET_NET_STATUS,
				 SIG_QUAL,
				 IP_STACK,
				 GET_APN_NAME,
				 POWER_STATUS,
				 AT_QUERY_CONTEXT};
}

namespace UConnGetStatistics {
enum GET_STATISTICS {STAT_GENERAL,
					 STAT_MAC,
					 STAT_PHY,
					 STAT_ALL,
					STAT_MEAS};
}

namespace UConnConfigNetParams {
enum NETWORK_PARAMS {PDP_CONFIG,
					 PCO_CONFIG,
					 APN_CONFIG,
					 PRIMARY_CTX,
					 CTX_ID,
					 CTX_QUERY_RSP,
					 CONTEXT_PARAMS,
					 SET_CONTEXT};
}

namespace UConnNetAttach {
enum ATTACH_MODE {CONNECT_DEFAULT,
				  CONNECT_MANUAL};
}

typedef int DAL_STATUS;

using namespace UConnConfig;
using namespace UConnSetStatus;
using namespace UConnGetStatus;
using namespace UConnGetStatistics;
using namespace UConnConfigNetParams;
using namespace UConnNetAttach;

class uconnDal
{
public:
	U_BOOL virtual isDevicePresent()=0;

	DAL_STATUS   virtual uconnInit(UNetworkManager* pNM)=0;

	DAL_STATUS  virtual uconnDeInit()=0;

	DAL_STATUS  virtual uconnATResetDevice()=0;

	DAL_STATUS virtual uconnReset(void*)=0;

	DAL_STATUS virtual uconnConfigure(CONFIG_CODE configCode)=0;

	DAL_STATUS virtual uconnNetSearch()=0;

	DAL_STATUS  virtual uconnNetSearchStatus()=0;

	DAL_STATUS virtual uconnNetAttach(ATTACH_MODE,void*,int ctxId)=0;

	DAL_STATUS virtual uconnConfigNetParams(NETWORK_PARAMS,void*,int ctxId=1)=0;

	DAL_STATUS virtual uconnGetStatus(GET_STATUS,void*)=0;

	DAL_STATUS virtual uconnSetStatus(SET_STATUS,void*,int ctxId=1)=0;

	DAL_STATUS virtual uconnNetDetach()=0;

	DAL_STATUS virtual uconnGetStatistics(GET_STATISTICS,void*)=0;

	void  virtual  callBackHandler(string strResp,unsigned int cmdId)=0;

	//more to be added
};

#endif
