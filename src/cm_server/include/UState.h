/*****************************************************************************
**
** UState.h
** This header file describes the interfaces for the Generic 
** State 
**
** Author: Sathyanarayanan S
** Date  : 29-DEC-10
** (c) United Mobile Apps Pvt Ltd (2010 - 2015)

    VER		  DATE		AUTHOR		DESCRIPTION
	0.1		29/12/10     SS			Creation
****************************************************************************/

#ifndef USTATE_H
#define USTATE_H

#include <stdio.h>
#include "UTypes.h"
#include "uconn_Event.h"

class UStateMachine;
class UState{
public:
        UState(UStateMachine* sm):pSM(sm){};
	virtual void  entry(){}
	virtual void   exit(){}
    virtual U_UINT32   handle(UEvent *){return 0;}

	UStateMachine*  pSM;
	UStateMachine*   getStateMachine(){
		return pSM;
	}

};

#endif
