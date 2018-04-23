/*****************************************************************************
**
** uconn_NWListState.h
** This header file describes the interfaces for the NW List State
**
** Author: Sathyanarayanan S
** Date  : 03-JAN-2011
** (c) United Mobile Apps Pvt Ltd (2010 - 2015)

    VER		  DATE		AUTHOR		DESCRIPTION
	0.1		03/01/11     SS			Creation
****************************************************************************/

#ifndef UCONN_NWLISTSTATE_H
#define UCONN_NWLISTSTATE_H

#include "UState.h"
#include "uconn_ServerAPI.h"
#include "uconn_Event.h"
#include "uconn_NwSearchEvent.h"

class UCMNWListState:public UState
{
public:
	UCMNWListState(UStateMachine *tSM):UState(tSM) {};
	virtual void entry();
	virtual U_UINT32 handle(UEvent* evt);
	virtual void exit();
};

#endif