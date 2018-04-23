/*****************************************************************************
**
** uconn_EnabledState.h
** This header file describes the interfaces for the Enabled State
**
** Author: Sathyanarayanan S
** Date  : 10-FEB-2011
** (c) United Mobile Apps Pvt Ltd (2010 - 2015)

    VER		  DATE		AUTHOR		DESCRIPTION
	0.1		10/02/11     SS			Creation
****************************************************************************/

#ifndef UCONN_ENABLEDSTATE_H
#define UCONN_ENABLEDSTATE_H

#include "UState.h"
#include "uconn_Config.h"

#define UCONN_DEFAULT_PERSIST_FILE "./persist.dat"

class UCMEnabledState:public UState
{
public:
	UCMEnabledState(UStateMachine *tSM):UState(tSM) {};
	virtual void entry();
	virtual U_UINT32 handle(UEvent* evt);
	virtual void exit();
	U_UINT32 rat;
};

#endif
