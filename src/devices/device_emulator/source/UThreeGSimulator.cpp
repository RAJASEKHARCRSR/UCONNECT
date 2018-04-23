#include "UThreeGSimulator.h"

UThreeGSimulator::UThreeGSimulator(UEventQueue* bearerManagerQueue):UNetworkManager(bearerManagerQueue)
{
}

U_VOID UThreeGSimulator::initNetMngr(void* config)
{
	ENMStateForBM newState = NM_NOT_READY_STATE;
	checkStateAndPostToBM(newState);
}

U_VOID UThreeGSimulator::enableNetMngr()
{
	ENMStateForBM newState = NM_READY_STATE;
	checkStateAndPostToBM(newState);

}

U_VOID UThreeGSimulator::connectToNetwork()
{
	ENMStateForBM newState = NM_CONNECTED_STATE;
	checkStateAndPostToBM(newState);

	string encodeStr = "";
	encodeStr += "RSSI;";
	encodeStr += "110";
	encodeStr += "\n";

	encodeStr += "CINR;";
	encodeStr += "20";
	encodeStr += "\n";

	encodeStr += "DL_Rate;";
	encodeStr += "10244";
	encodeStr += "\n";

	encodeStr += "UL_Rate;";
	encodeStr += "10243";
	encodeStr += "\n";

	encodeStr += "Duration;";
	encodeStr += "01:01:01";
	encodeStr += "\n";

	UNMThreeGStats* threeGStats = new UNMThreeGStats(encodeStr);
	this->postEventToBearerManager((UEvent*)(threeGStats));

}
U_VOID UThreeGSimulator::disconnectFromNetwork()
{
	ENMStateForBM newState = NM_NOT_READY_STATE;
	checkStateAndPostToBM(newState);
	newState = NM_READY_STATE;
	checkStateAndPostToBM(newState);
}

U_VOID UThreeGSimulator::disableNetMngr()
{
	ENMStateForBM newState = NM_DISABLED_STATE;
	checkStateAndPostToBM(newState);
	
}
ETechSelect UThreeGSimulator::getTechID()
{
	return THREE_G_TECH;
}
U_VOID UThreeGSimulator::searchNetworks()
{
}

U_VOID UThreeGSimulator::getParam(Get_NM_Param paramId)
{
	string param = "";
	switch(paramId)
	{
	case NETWORK_NAME:
		{
			param = "TATA PHOTON+";
		}
		break;
	case VERSION:
		{
			param = "Version 0.8";
		}
		break;
	case NETWORK_ACCESS_TYPE:
		{
			param = "GSM";
		}
		break;
	default :
		break;
	}
	UBearerGetParamRsp* paramRsp = new UBearerGetParamRsp(this->getTechID(),paramId,param);
	postEventToBearerManager(reinterpret_cast<UEvent*>(paramRsp));

}

U_UINT32 UThreeGSimulator::setParam(U_UINT32 paramId,std::string param)
{
	return 0;
}

U_VOID UThreeGSimulator::checkStateAndPostToBM(ENMStateForBM newState)
{
	if(this->state != newState)
	{
		this->state = newState;
		updateNMStateAndPost(this->getTechID(),this->state);
		
	}
	
}

