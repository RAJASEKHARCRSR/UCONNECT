#ifndef U_THREE_G_SIMULATOR_H
#define U_THREE_G_SIMULATOR_H
#include "NetworkManager.h"

class UThreeGSimulator : public UNetworkManager {

public:
	U_VOID checkStateAndPostToBM(ENMStateForBM newState);
	UThreeGSimulator(UEventQueue*);
	~UThreeGSimulator(){}

	U_VOID enableNetMngr();
	U_VOID disableNetMngr();
	U_VOID connectToNetwork() ;
	U_VOID searchNetworks() ;
	U_VOID disconnectFromNetwork() ;
	U_VOID initNetMngr(void *cfgInfo);

	U_VOID getParam(Get_NM_Param paramID);
	U_UINT32 setParam(U_UINT32 paramId,std::string param);

	ETechSelect getTechID();
	U_VOID deviceEventsCallBack(U_UINT32 cmdID,U_UINT32 techID) {}
	U_UINT32 getMeasStatisticsNM(){return 0;};
	U_VOID handleEvent(UEvent* evt){delete evt;};

	U_UINT32 getStatisticsNM()
	{
		return 0;
	}

};


#endif //nU_THREE_G_SIMULATOR_H