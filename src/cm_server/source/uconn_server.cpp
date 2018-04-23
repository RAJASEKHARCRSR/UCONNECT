/*****************************************************************************
**
** uconn_server.cpp
** This header file describes the interfaces for processes in the CM
**
** Author: Sathyanarayanan S
** Date  : 10-FEB-2011
** (c) United Mobile Apps Pvt Ltd (2010 - 2015)

    VER		  DATE		AUTHOR		DESCRIPTION
	0.1		10/02/11     SS			Creation
****************************************************************************/
#ifndef _UCONN_SERVER_H_
#define _UCONN_SERVER_H_

#include "uconn_server.h"
#include "uconn_StateMachine.h"

U_BOOL UConnServer::EventHandler(){

	//mCMSM.UCMStartUp();
	//mCMSM.getState()->entry(); 
	while(1)
	{
		//try {
			//printf("event handler try\n");
			// Wait for Message from Message Queue
			UEvent *evt;
			if(GetQ()->MsgQGet((U_UINT8**)&evt) != -1)	
			{

				//printf("got a message\n");
				// Ideally we should not have msgqdepth increasing, but if it is then 
				// there are too many messages, so lets drop them instead of processing
				if(GetQ()->MsgQGetDepth() < 32)
				{
				// Do any processing on this event if required
				// Post this event to the statemachine
					mCMSM.handle(evt);
				}
				else{
				// ALARM LOG HERE , DROP THE EVENT as we have too many events
					LOG(ULOG_ERROR_LEVEL, "ALARM : Msg Q overload Dropped Event id: %d size:%d\n",evt->getCmdId(),evt->getSize());
					//cout << "ALARM : Msg Q overload Dropped Event id:" <<  evt->getCmdId() << " size:" << evt->getSize() <<endl;
				}
				delete evt;
				evt = NULL;

			}
		//}
		//catch(bad_alloc& ba) {
		//	printf("Out of Memory!\n");
		//}
		//catch(exception& e) {
		//	printf("Unknown Exception received\n");
		//}
	}
}



void UConnServer::initialize(){
	
	// Initialize the Task
	this->Init();

  


	// Initialize all data structures;

	// Initialize your statemachine

}

/*
void UConnServer::Reset() 
{ }
*/

#endif