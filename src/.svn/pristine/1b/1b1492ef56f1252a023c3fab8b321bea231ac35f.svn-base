/*****************************************************************************
**
** uconn_server.h
** This header file describes the interfaces for the Server process in the CM
**
** Author: Sathyanarayanan S
** Date  : 10-FEB-2011
** (c) United Mobile Apps Pvt Ltd (2010 - 2015)

    VER		  DATE		AUTHOR		DESCRIPTION
	0.1		10/02/11     SS			Creation
****************************************************************************/

#ifndef UCONN_SERVER_H_
#define UCONN_SERVER_H_

#include "UTypes.h"
#include "uconn_OSAL.h"
#include "uconn_Event.h"
#include "uconn_StateMachine.h"



class UConnServer: public UProcess
{
private:
	UCMStateMachine  mCMSM;



public:

    UConnServer():eventCBHdlr(0){};
	U_VOID (*eventCBHdlr)(UEvent *);
	virtual void Reset() { }
	virtual U_BOOL	EventHandler();

	uconnDal *getDal()
	{
	   return	mCMSM.getDal();
	}

	UCMStateMachine* getSM()
	{
		return &mCMSM;
	}

	void initialize();
};

extern UConnServer *gConnServer;

#endif