/*****************************************************************************
**
** uconn_DisconnectingState.h
** This header file describes the interfaces for the Disconnecting State
**
** Author: Sathyanarayanan S
** Date  : 03-JAN-2011
** (c) United Mobile Apps Pvt Ltd (2010 - 2015)

    VER		  DATE		AUTHOR		DESCRIPTION
	0.1		03/01/11     SS			Creation
****************************************************************************/

#ifndef UCONN_DISCONNECTINGSTATE_H
#define UCONN_DISCONNECTINGSTATE_H

#include "UState.h"
#include "uconn_DisconnectingEvent.h"

class UCMDisconnectingState:public UCMEnabledState
{
public:
	UCMDisconnectingState(UStateMachine *tSM):UCMEnabledState(tSM) {};
	virtual void entry();
	virtual U_UINT32 handle(UEvent* evt);
	virtual void exit();
};

#endif