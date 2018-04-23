/*****************************************************************************
**
** uconn_DisabledState.h
** This header file describes the interfaces for the Disabled State
**
** Author: Sathyanarayanan S
** Date  : 10-FEB-2011
** (c) United Mobile Apps Pvt Ltd (2010 - 2015)

    VER		  DATE		AUTHOR		DESCRIPTION
	0.1		10/02/11     SS			Creation
****************************************************************************/

#ifndef UCONN_DISABLEDSTATE_H
#define UCONN_DISABLEDSTATE_H

#include "UState.h"
#include "uconn_OSAL.h"

class UCMDisabledState:public UState
{
public:
	UCMDisabledState(UStateMachine *tSM):UState(tSM) {};
	virtual void entry();
	virtual U_UINT32 handle(UEvent* evt);
	virtual void exit();
};

#endif