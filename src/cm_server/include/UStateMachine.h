/*****************************************************************************
**
** UStateMachine.h
** This header file describes the interfaces for the Generic 
** State Machine
**
** Author: Sathyanarayanan S
** Date  : 29-DEC-10
** (c) United Mobile Apps Pvt Ltd (2010 - 2015)

    VER		  DATE		AUTHOR		DESCRIPTION
	0.1		29/12/10     SS			Creation
****************************************************************************/

#ifndef USTATEMACHINE_H
#define USTATEMACHINE_H

#include "UState.h"

class UStateMachine{
public:
	U_UINT32 commandId;
        UStateMachine():m_tParentProtocolSM(NULL),m_tCurrentState(NULL){
	}


        UStateMachine(UStateMachine* pSM):
	m_tParentProtocolSM(pSM),m_tCurrentState(NULL){

	}

	void setState(UState* state){ 
		m_tCurrentState = state;
	}

	UState* getState() {
		return m_tCurrentState;
	}


	virtual U_UINT32 handle(UEvent *evnt);
	void transit(UState* state);
	
	UStateMachine* getParentProtocolStateMachine(){
		return m_tParentProtocolSM;
	}
	void setParentProtocolStateMachine(UStateMachine*  pSM){
		m_tParentProtocolSM = pSM;
	}


private:
	UState*    m_tCurrentState;
	UStateMachine*  m_tParentProtocolSM;

};


#endif
