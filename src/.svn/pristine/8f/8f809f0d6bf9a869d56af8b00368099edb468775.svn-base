/*****************************************************************************
**
** uconn_searchingState.h
** This header file describes the interfaces for the NW List State
**
** Author: Sathyanarayanan S
** Date  : 03-JAN-2011
** (c) United Mobile Apps Pvt Ltd (2010 - 2015)

    VER		  DATE		AUTHOR		DESCRIPTION
	0.1		03/01/11     SS			Creation
****************************************************************************/

#ifndef UCONN_SEARCHINGSTATE_H
#define UCONN_SEARCHINGSTATE_H

#include "UState.h"
#include "uconn_ServerAPI.h"
#include "uconn_SearchEvent.h"


class UCMSearchingState:public UCMEnabledState
{
private:
	U_UINT32 timer;
public:
	UCMSearchingState(UStateMachine *tSM);
	virtual void entry();
	virtual U_UINT32 handle(UEvent* evt);
	virtual void exit();
	void set_timer(U_UINT32 plmn_timer) 
	{
		timer = plmn_timer;
	}

	U_UINT32 get_timer()
	{
		return timer;
	}

};

#endif