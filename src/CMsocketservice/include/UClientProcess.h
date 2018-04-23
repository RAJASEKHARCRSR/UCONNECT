/*****************************************************************************
**
** uconn_MonitorDevice.h
** This header file describes the interfaces for the Server process in the CM
**
** Author: Suresh Lingadalli
** Date  : 16-JAN-2012
** (c) United Mobile Apps Pvt Ltd (2010 - 2015)

    VER		  DATE		AUTHOR		DESCRIPTION
	0.1		16/01/12     SL			Creation
****************************************************************************/

#ifndef UCLIENTPROCESS_H_
#define UCLIENTPROCESS_H_

#ifdef WIN32
#include <process.h>
#endif
#include <algorithm>
#include "uconn_OSAL.h"
#include "ULog.h"
//#include "UClientProcess.h"
#include "uconn_ServerAPI.h"
#include "uconn_ClientCommon.h"


static U_THREAD_RETURN_TYPE process_client (U_THREAD_ARG arg_p);
static U_VOID uconn_ProcessRecvdClientData(U_SINT8* recvbuf, 
										   U_SINT32 recvbuflen, 
										   UEvent **respEvt);
extern clientDetails_t gClientDetails[UCONN_MAX_CLIENTS_SUPPORTED];

class UClientProcess: public UProcess
{
private:
	
public:
	U_SOCKET tServerSocket;
	UClientProcess();
    ~UClientProcess();

	virtual U_BOOL	EventHandler();
	void lookForClient();
};



#endif //UCLIENTPROCESS_H_
