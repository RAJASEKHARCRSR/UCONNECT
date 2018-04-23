/*****************************************************************************
**
** uconn_ATHandler.h
** This header file describes the interfaces for the Server process in the CM
**
** Author: Sathyanarayanan S
** Date  : 10-FEB-2011
** (c) United Mobile Apps Pvt Ltd (2010 - 2015)

    VER		  DATE		AUTHOR		DESCRIPTION
	0.1		10/02/11     SS			Creation
****************************************************************************/

#ifndef UCONN_ATHANDLER_H_
#define UCONN_ATHANDLER_H_

#include "UTypes.h"
#include "uconn_OSAL.h"
#include "UEmulator.h"
#include "uconn_Event.h"
#include <vector>
#include <string>

//#define emulatorMode 1



class UConnATHandler: public UProcess
{
private:
	//USrPortCommUsage* mSerialPort;

	UEmulator* uEmulator;

#if defined(WIN32) && !defined(QTPROJECT)
	USrPortCommUsage mSerialPort;
#endif    
	
	P_AT_CALLBACK_WITH_ID m_callbackfnWithId_ptr;
    U_BOOL   bStartThreadOnce;
public:
	USemaphore*		 mWriteLock;
	UConnATHandler();
	~UConnATHandler();
	virtual void Reset() { }
	virtual U_BOOL	EventHandler();
	U_BOOL sendATCommand(const char* data,U_DWORD dwSize);
	U_BOOL initialize(U_STRING sComPort,
					P_AT_CALLBACK_WITH_ID callback_fn,
					UNetworkManager* pNM);

	void partiallyParse(vector<std::string>& vRespStr,string &mainRespStr);
	void tokeniseInformation(string &rspStr, vector<std::string>& vStr, char ch);
	void deviceStatus(U_DWORD &status);

	void                     Init(U_UINT8 *pu8TaskName=0,U_UINT32 u32Priority=THREAD_BASE_PRIORITY_MAX,
                            U_UINT32 u32StackSize=100,U_UINT32 u16MaxQDepth=64, U_PVOID pvContext=NULL);
	void resetComPort();

	/* Pointer to the Network Manager */
	UNetworkManager* m_pNM;
};


#endif //UCONN_ATHANDLER_H_
