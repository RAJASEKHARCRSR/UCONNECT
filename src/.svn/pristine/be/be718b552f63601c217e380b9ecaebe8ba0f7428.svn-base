/*****************************************************************************
**
** uconn_ConnectingState.h
** This header file describes the interfaces for the Connecting State
**
** Author: Sathyanarayanan S
** Date  : 03-JAN-2011
** (c) United Mobile Apps Pvt Ltd (2010 - 2015)

    VER		  DATE		AUTHOR		DESCRIPTION
	0.1		03/01/11     SS			Creation
****************************************************************************/

#ifndef UCONN_CONNECTINGSTATE_H
#define UCONN_CONNECTINGSTATE_H

#ifdef WIN32
#include <tchar.h>
#endif

#include "UState.h"
#include "uconn_ConnectingEvent.h"
#include "UStateMachine.h"

typedef enum
{
	Define_Primary = 1000,
	Define_Secondary = 1001,
	Net_attach = 1002
}Connection_State;

class UCMConnectingState:public UCMEnabledState
{
public:
	U_UINT32 conn_state;
	UCMConnectingState(UStateMachine *tSM):UCMEnabledState(tSM) {};
	virtual void entry();
	virtual U_UINT32 handle(UEvent *evt);
	virtual void exit();
};

class UCMWaitPrimaryAct:public UCMConnectingState
{
public:
	UCMWaitPrimaryAct(UStateMachine* tSM):UCMConnectingState(tSM) {};
	virtual void entry();
	virtual U_UINT32 handle(UEvent* evt);
	virtual void exit();
};

class UCMWaitRegStatusAct:public UCMConnectingState
{
public:
	UCMWaitRegStatusAct(UStateMachine* tSM):UCMConnectingState(tSM) {};
	virtual void entry();
	virtual U_UINT32 handle(UEvent* evt);
	virtual void exit();
};

/*class UCMDefSecondaryState:public UCMConnectingState
{
public:
	UCMDefSecondaryState(UStateMachine* tSM):UCMConnectingState(tSM) {};
	virtual void entry();
	virtual U_UINT32 handle(UEvent* evt);
	virtual void exit();
};
class UCMDefPrimaryState:public UCMConnectingState
{
public:
	UCMDefPrimaryState(UStateMachine* tSM):UCMConnectingState(tSM) {};
	virtual void entry();
	virtual U_UINT32 handle(UEvent* evt);
	virtual void exit();
};*/


/*class UCMNetAttachState:public UCMConnectingState
{
public:
	UCMNetAttachState(UStateMachine* tSM):UCMConnectingState(tSM) {};
	virtual void entry();
	virtual U_UINT32 handle(UEvent* evt);
	virtual void exit();
};*/

#ifdef WIN32
U_UINT32 IPv4_config(_TCHAR* mode, _TCHAR* device);
#else
U_UINT32 IPv4_config(char* mode, char* device);
#endif

#endif