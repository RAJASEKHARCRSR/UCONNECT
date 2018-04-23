#include "NetworkManager.h"

UNetworkManager::UNetworkManager(UEventQueue* bearerManagerQueue)
{
	this->bearerMngrQueue = NULL;
	this->bearerMngrQueue = bearerManagerQueue;
	this->enableStatus = NM_DISABLED;
	this->connectionStatus = NM_DISCONNECTED;
	this->state = NM_DISABLED_STATE;
	this->mNMServer = NULL;
	this->mATHandler = NULL;
	nmOldPriority = 0;
	nmPriority = 0;
}

UNetworkManager::~UNetworkManager()
{
	this->bearerMngrQueue = NULL;
}

void UNetworkManager::postEventToBearerManager(UEvent* evt)
{
	if(this->bearerMngrQueue!= NULL)
	{
		this->bearerMngrQueue->MsgQPost((U_UINT8*)evt);
	}

}

ENetworkConnectionStatus UNetworkManager::getConnectionStatus()
{

	return connectionStatus;
}

ENetworkEnableStatus UNetworkManager::getEnableStatus()
{
	return enableStatus;
}

U_VOID UNetworkManager::deviceEventsCallBack(U_UINT32 cmdID,U_UINT32 techID)
{

}

U_VOID UNetworkManager::updateNMStateAndPost(ETechSelect techID,ENMStateForBM state)
{
	UNMStateEvent* stateEvent = new UNMStateEvent(techID,state);
	postEventToBearerManager(reinterpret_cast<UEvent*>(stateEvent));
}

U_VOID UNetworkManager::setPriority(U_UINT32 techID,U_UINT32 priority)
{
	nmOldPriority = nmPriority;
	nmPriority = priority;
	//UNMPriorityEvent* priorityEvent = new UNMPriorityEvent(techID,priority);
	//postEventToBearerManager(reinterpret_cast<UEvent*>(priorityEvent));
}

U_UINT32 UNetworkManager::getPriority()
{
	return nmPriority;
}

U_UINT32 UNetworkManager::getOldPriority()
{
	return nmOldPriority;
}

ENMStateForBM UNetworkManager::getStateNM()
{
	return this->state;
}