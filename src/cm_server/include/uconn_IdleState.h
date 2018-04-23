/*****************************************************************************
**
** uconn_IdleState.h
** This header file describes the interfaces for the Idle State
**
** Author: Sathyanarayanan S
** Date  : 29-DEC-2010
** (c) United Mobile Apps Pvt Ltd (2010 - 2015)

    VER		  DATE		AUTHOR		DESCRIPTION
	0.1		29/12/10     SS			Creation
****************************************************************************/

#ifndef UCONN_IDLESTATE_H
#define UCONN_IDLESTATE_H

#include "UTypes.h"
#include "uconn_EnabledState.h"

class UCMIdleState:public UCMEnabledState
{
public:
	UCMIdleState(UStateMachine *tSM):UCMEnabledState(tSM) {};
	virtual void entry();
	virtual U_UINT32 handle(UEvent *evt);
	virtual void exit();
};

#endif