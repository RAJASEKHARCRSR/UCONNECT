/*****************************************************************************
**
** uconn_ConnectedState.h
** This header file describes the interfaces for the Connnected State
**
** Author: Sathyanarayanan S
** Date  : 03-JAN-2011
** (c) United Mobile Apps Pvt Ltd (2010 - 2015)

    VER		  DATE		AUTHOR		DESCRIPTION
	0.1		03/01/11     SS			Creation
****************************************************************************/

#ifndef UCONN_CONNECTEDSTATE_H
#define UCONN_CONNECTEDSTATE_H

#include "UState.h"
#include "uconn_ConnectedEvent.h"

class UCMConnectedState:public UCMEnabledState
{
public:
	UCMConnectedState(UStateMachine *tSM):UCMEnabledState(tSM) {};
	virtual void entry();
	virtual U_UINT32 handle(UEvent* evt);
	virtual void exit();
};

#endif