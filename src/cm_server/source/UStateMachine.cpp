/*****************************************************************************
**  UStateMachine.cpp
**  This file describes the methods to handle the Generic States
**  of the State Machine
**
** Author: Sriram V Iyer
** Date  : 30-DEC-2010
** (c) United Mobile Apps Pvt Ltd (2010 - 2015)

    VER		  DATE		AUTHOR		DESCRIPTION
	0.1		30/12/10     SVI		Creation
****************************************************************************/

#include "UStateMachine.h"

/***********************************************************************
*	UStateMachine::handle -
*	State Machine handler
*
*	Parameters
*	evt	- Pointer to the Event type
***********************************************************************/

U_UINT32 UStateMachine::handle(UEvent *evt)
{
	U_UINT32 status = -1;
	if(m_tCurrentState != NULL){
	 status = m_tCurrentState->handle(evt);
	}
	return status;
}

/***********************************************************************
*	UStateMachine::transit -
*	State Machine transit method
*
*	Parameters
*	newstate	- Pointer to the new State to be moved to
***********************************************************************/

void UStateMachine::transit(UState *newstate)
{
	if(newstate != m_tCurrentState)
	{
		m_tCurrentState->exit();
		m_tCurrentState = newstate;
		m_tCurrentState->entry();
	}
}

