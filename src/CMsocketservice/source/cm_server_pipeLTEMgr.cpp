
#include "UClientProcess.h"
#include "uconn_ClientCommon.h"
#include "modeSwitch.h"
#include "UCommonUtils.h"

#define SLEEP_TIME_FOR_DEBUG 10000
#define DEBUG 1


const int nBufferSize = 500;

#ifdef WIN32
CHAR cStr[nBufferSize+1];
SERVICE_STATUS ServiceStatus;
SERVICE_STATUS_HANDLE hStatus;
void pipetoLTEMgr();
/******************************************************************************
*	Function called to Restart the LTE Manager application for Altair devices 
******************************************************************************/
int restartLTEMgr ()
{
      HANDLE hPipe; 
      LPTSTR lpvMessage=TEXT("Default message from client."); 
      //TCHAR  chBuf[BUFSIZE]; 
      BOOL   fSuccess = FALSE; 
      DWORD  cbRead, cbToWrite, cbWritten, dwMode; 
      LPTSTR lpszPipename = TEXT("\\\\.\\pipe\\AltairLteManager"); 

      lpvMessage = (LPTSTR)L"Restart LTE manager";

      while (1) 
      { 
            hPipe = CreateFile( 
            lpszPipename,   // pipe name 
            GENERIC_READ |  // read and write access 
            GENERIC_WRITE, 
            0,              // no sharing 
            NULL,           // default security attributes
            OPEN_EXISTING,  // opens existing pipe 
            0,              // default attributes 
            NULL);          // no template file 
             
            // Break if the pipe handle is valid. 
             
            if (hPipe != INVALID_HANDLE_VALUE) 
                  break; 
             
            // Exit if an error other than ERROR_PIPE_BUSY occurs. 
             
            if (GetLastError() != ERROR_PIPE_BUSY) 
            {
                  _tprintf( TEXT("Could not open pipe. GLE=%d\n"), GetLastError() );
				  goto err0;
                  //return -1;
            }
            
            // All pipe instances are busy, so wait for 20 seconds. 
             
            if ( ! WaitNamedPipe(lpszPipename, 10000)) 
            { 
                  printf("Could not open pipe: 10 second wait timed out."); 
				  goto err0;
                  //return -1;
            } 
      } 
      printf("PIPE IS CONNECTED\n");
	  printf("hPipe = %x\n", hPipe);
      // Send a message to the pipe server. 
       
      cbToWrite = (lstrlen(lpvMessage)+1)*sizeof(TCHAR);
      _tprintf( TEXT("Sending %d byte message: \"%s\"\n"), cbToWrite, lpvMessage); 

      fSuccess = WriteFile( 
            hPipe,                  // pipe handle 
            lpvMessage,             // message 
            cbToWrite,              // message length 
            &cbWritten,             // bytes written 
            NULL);                  // not overlapped 

      if ( ! fSuccess) 
      {
            _tprintf( TEXT("WriteFile to pipe failed. GLE=%d\n"), GetLastError() ); 
            goto err0;
			//return -1;
      }

      Sleep(3000);
      printf("\nMessage sent to server\n");
err0:
	  printf("CloseHandle called \n");
      CloseHandle(hPipe); 
       
      return 0; 
}


void StopService();

TCHAR* serviceName = TEXT("CMSocketService");
SERVICE_STATUS serviceStatus;
SERVICE_STATUS_HANDLE serviceStatusHandle = 0;
HANDLE stopServiceEvent = 0;
#endif
P_AT_CALLBACK_WITH_ID m_callBackPtrGlobal;

U_MUTEX	initLock, writeLock;
extern U_MUTEX	sendDataLock;
clientDetails_t gClientDetails[UCONN_MAX_CLIENTS_SUPPORTED];

#ifdef WIN32
SC_HANDLE service;
HDEVNOTIFY notifierHandle;
#endif


UClientProcess *gProcessClient = NULL;


void initialiseCallBackFunction(P_AT_CALLBACK_WITH_ID callback_fn)
{
	m_callBackPtrGlobal = callback_fn;
}

void registerForDeviceNotification()
{
	//Sleep(10000);
	P_AT_CALLBACK_WITH_ID fnptr = &InitCallbackWithId;
	initialiseCallBackFunction(fnptr);
#ifdef WIN32 
	DEV_BROADCAST_DEVICEINTERFACE NotificationFilter;
	ZeroMemory( &NotificationFilter, sizeof(NotificationFilter));
	NotificationFilter.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
    NotificationFilter.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
 	notifierHandle = RegisterDeviceNotification(serviceStatusHandle, 
												&NotificationFilter, DEVICE_NOTIFY_SERVICE_HANDLE|DEVICE_NOTIFY_ALL_INTERFACE_CLASSES);
	if(!notifierHandle)
	{
		int err = GetLastError();
	}
#endif
}

void StartUConnectGUI()
{
#if DEBUG
	return;
#endif
#ifdef WIN32
	HANDLE hToken;
 TCHAR* progName = TEXT("");
	char errStr[100];
	DWORD dwCreationFlag = NORMAL_PRIORITY_CLASS | CREATE_NEW_CONSOLE;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	int k = WTSQueryUserToken (WTSGetActiveConsoleSessionId (), &hToken);
	if(k) 
	{
	}
	else
	{
	}
	ZeroMemory( &si, sizeof( STARTUPINFO ) );
	si.cb = sizeof( STARTUPINFO );
//#ifndef _DEBUG
//	char* buffer = "winsta0\\default";
//	WCHAR bufferChar[100];
//	mbstowcs(bufferChar, buffer, 100);
//	si.lpDesktop = bufferChar;
//#else
	si.lpDesktop = (LPSTR)"winsta0\\default";
//#endif
	ZeroMemory( &pi,sizeof(pi));
	if ( !CreateProcessAsUser(
						  hToken,
						  progName,
						  NULL,  
						  NULL,
						  NULL,
						  FALSE,
						  dwCreationFlag,
						  NULL,
						  NULL,
						  &si,
						  &pi
			  ) )
	{
		sprintf(errStr, "CreateProcessAsUser Failed %d\n", GetLastError());
	} 
	else
	{
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
		CloseHandle(hToken);
	}
#endif
}

#ifndef WIN32	
int main()
{
    InitCMSocketService();
    while(1)
    {
    }
}
#endif 
// Service initialization
int InitCMSocketService()
{
#ifdef WIN32
	Sleep(SLEEP_TIME_FOR_DEBUG);
#endif
	int result = 0;
	
	registerForDeviceNotification();	

	char devName [] = "VID_2001&PID_A80B";
	
		/* Call Modeswitch from the linked libray */
		char switchPacket[]={
		0x55 ,0x53 ,0x42 ,0x43 ,0x40 ,0xd4 ,0x87 ,0x04 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 
		,0x03 ,0xf0 ,0x01 ,0x01 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00
		,0x00 ,0x00 ,0x00};
		modeSwitch(devName, switchPacket);

	//Sleep(10000);
	UCM_Init(uconn_EventCallback);

	
	//MY_LOG("startClientService() calling \n");
	gProcessClient = new UClientProcess();
	gProcessClient->Init();
	//MY_LOG("startClientService() called \n");

	StartUConnectGUI();
#ifdef WIN32	
	initialiseVpn();
#endif
    return(result);
}

static int static_int = 0;
#ifdef WIN32
void WINAPI ServiceControlHandler(DWORD controlCode,DWORD wEvtype, PVOID lpEvdata, PVOID Context)
{
	switch ( controlCode )
	{
		case SERVICE_CONTROL_INTERROGATE:
	//		MY_LOG("Got SERVICE_CONTROL_INTERROGATE EVENT\n");
			break;

		case SERVICE_CONTROL_SHUTDOWN:
	//		MY_LOG("Got SHUTDOWN EVENT\n");
		case SERVICE_CONTROL_STOP:
	//		MY_LOG("Got STOP EVENT\n");
			//m_pNM->mATHandler->resetComPort();
			UnregisterDeviceNotification(notifierHandle);
			serviceStatus.dwCurrentState = SERVICE_STOP_PENDING;
			SetServiceStatus( serviceStatusHandle, &serviceStatus );
	//		MY_LOG("Releasing Lock\n");
			release_lock(&initLock);
			if(gProcessClient != NULL)
			{
				//close_server_socket(gProcessClient->tServerSocket);
				delete gProcessClient;
				gProcessClient = NULL;
			}
			unInitialsieUCM();
			break;

		case SERVICE_CONTROL_PAUSE:
	//		MY_LOG("Got PAUSE EVENT\n");
			break;

		case SERVICE_CONTROL_CONTINUE:
	//		MY_LOG("Got CONTINUE EVENT\n");
			break;
		case SERVICE_CONTROL_DEVICEEVENT:
			{
				bool ret = false;
				switch(wEvtype)	  
				{		
					// A device has been inserted and is now available.
				case DBT_DEVICEARRIVAL:
					{
	//					MY_LOG("Device Arived\n");
						ret = DeviceChangeType((LPARAM)lpEvdata,wEvtype);
						string callbackStr = "";
						if(ret == true)
						{
							//::Sleep(2000);
							handleCallBackFromDevices(3,UCM_getConfig()->insertedDeviceTechID);
	//						WriteLog("USB arrived");
						}
					}
					break;
					// Device has been removed.
				case DBT_DEVICEREMOVECOMPLETE:
					{
	//					MY_LOG("Device Removed\n");
						ret = DeviceChangeType((LPARAM)lpEvdata,(WPARAM)wEvtype);
						if(true == ret)
						{
							handleCallBackFromDevices(4,UCM_getConfig()->insertedDeviceTechID);
	//						WriteLog("USB removed");
						}
					}
					break;
				default:
					break;
				}
	//			MY_LOG("DEvice notification received\n");
			}
			break;
		default:
	//		MY_LOG("Got unknown EVENT\n");
			if ( controlCode >= 128 && controlCode <= 255 )
				// user defined control code
				break;
			else
				// unrecognised control code
				break;
	}

	SetServiceStatus( serviceStatusHandle, &serviceStatus );
}

void WINAPI ServiceMain( DWORD /*argc*/, TCHAR* /*argv*/[] )
{

	// initialise service status
	serviceStatus.dwServiceType = SERVICE_WIN32;
	serviceStatus.dwCurrentState = SERVICE_STOPPED;
	serviceStatus.dwControlsAccepted = 0;
	serviceStatus.dwWin32ExitCode = NO_ERROR;
	serviceStatus.dwServiceSpecificExitCode = NO_ERROR;
	serviceStatus.dwCheckPoint = 0;
	serviceStatus.dwWaitHint = 0;

	//serviceStatusHandle = RegisterServiceCtrlHandler( serviceName, (LPHANDLER_FUNCTION)ServiceControlHandler );
	serviceStatusHandle = RegisterServiceCtrlHandlerEx( serviceName, (LPHANDLER_FUNCTION_EX)ServiceControlHandler,0 );

	if ( serviceStatusHandle )
	{
		// service is starting
		serviceStatus.dwCurrentState = SERVICE_START_PENDING;
		SetServiceStatus( serviceStatusHandle, &serviceStatus );
		// do initialisation here
		stopServiceEvent = CreateEvent( 0, FALSE, FALSE, 0 );

		// running
		serviceStatus.dwControlsAccepted |= (SERVICE_CONTROL_INTERROGATE | SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN);
		serviceStatus.dwCurrentState = SERVICE_RUNNING;
		SetServiceStatus( serviceStatusHandle, &serviceStatus );
		
		//Sleep(4000);
		// Initialize Service
		int error=0;
		error = InitCMSocketService();
		if (error)
		{
	///		MY_LOG("Error Starting CM Sock Service\n");
			 // Initialization failed
			ServiceStatus.dwCurrentState       = SERVICE_STOPPED;
			ServiceStatus.dwWin32ExitCode      = -1;
			SetServiceStatus(hStatus, &ServiceStatus);
			return;
		}
		//MY_LOG("CM Sock Service Started\n");
		take_lock(&initLock);
		//MY_LOG("Got Stop Event\n");
		// service was stopped
		serviceStatus.dwCurrentState = SERVICE_STOP_PENDING;
		SetServiceStatus( serviceStatusHandle, &serviceStatus );

		// do cleanup here
		CloseHandle( stopServiceEvent );
		stopServiceEvent = 0;

		// service is now stopped
		serviceStatus.dwControlsAccepted &= ~(SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN);
		serviceStatus.dwCurrentState = SERVICE_STOPPED;
		SetServiceStatus( serviceStatusHandle, &serviceStatus );
	}
}
#endif
void RunService()
{
#ifdef WIN32
	initialize_lock(&initLock);
	
	take_lock(&initLock);
	SERVICE_TABLE_ENTRY serviceTable[] =
	{
		{ serviceName, ServiceMain },
		{ 0, 0 }
	};

	//MY_LOG("Before StartServiceCtrlDispatcher");
	if(!StartServiceCtrlDispatcher( serviceTable ))
	{
		long nError = GetLastError();
		strcpy(cStr,"");
		sprintf(cStr, "ERROR starting the CMSocketService, error code = %d\n", nError);
	//	WriteLog(cStr);		
	}	
	//MY_LOG("After StartServiceCtrlDispatcher");
#endif
}

void InstallService()
{
#ifdef WIN32
	SC_HANDLE serviceControlManager = OpenSCManager( 0, 0, SC_MANAGER_CREATE_SERVICE );

	if ( serviceControlManager )
	{
		TCHAR path[ _MAX_PATH + 1 ];
		if ( GetModuleFileName( 0, path, sizeof(path)/sizeof(path[0]) ) > 0 )
		{
			service = CreateService( serviceControlManager,
							serviceName, serviceName,
							SERVICE_ALL_ACCESS, SERVICE_WIN32_OWN_PROCESS,
							SERVICE_AUTO_START, SERVICE_ERROR_IGNORE, path,
							0, 0, 0, 0, 0 );
	//		MY_LOG("Installed");
			StartService(service,0,0);
			if ( service )
				CloseServiceHandle( service );
		}

		CloseServiceHandle( serviceControlManager );
	}
#endif
}

void UninstallService()
{
#ifdef WIN32
	
	StopService();

	SC_HANDLE serviceControlManager = OpenSCManager( 0, 0, SC_MANAGER_CONNECT );

	if ( serviceControlManager )
	{
		SC_HANDLE service = OpenService( serviceControlManager,
			serviceName, SERVICE_QUERY_STATUS | DELETE );
		if ( service )
		{
			SERVICE_STATUS serviceStatus;
			if ( QueryServiceStatus( service, &serviceStatus ) )
			{
				if ( serviceStatus.dwCurrentState == SERVICE_STOPPED )
				{
					DeleteService( service );
	//				MY_LOG("UnInstalled");
				}
			}

			CloseServiceHandle( service );
		}

		CloseServiceHandle( serviceControlManager );
	}
#endif
}

#ifdef WIN32
int _tmain( int argc, TCHAR* argv[] )
{
	int ii;

	initialize_lock(&writeLock);
	initialize_lock(&sendDataLock);
	for(ii = 0; ii < UCONN_MAX_CLIENTS_SUPPORTED; ii++)
		gClientDetails[ii].clientSock = INVALID_SOCKET;
	if ( argc > 1 && lstrcmpi( argv[1], TEXT("install") ) == 0 )
	{
		//restartLTEMgr();
		InstallService();
	}
	else if ( argc > 1 && lstrcmpi( argv[1], TEXT("uninstall") ) == 0 )
	{
		UninstallService();
	}
	else if ( argc > 1 && lstrcmpi( argv[1], TEXT("stop") ) == 0 )
	{
		StopService();
	}
	else
	{
		RunService();
	}

	delete_lock(&writeLock);
	return 0;
}
#endif
/******************************************************************************
*	Called to intimate events to the client. Events are passed to all the clients
*	connected. So write into every socket. this function will be called in the
*	server state machine context. 
*	
*	Parameters
*	event_p - pointer to the event
******************************************************************************/
extern U_MUTEX	sendDataLock;

U_VOID uconn_EventCallback (UEvent *event_p)
{

	U_SINT32 idx;
	//MY_LOG("Entered EventCallback\n");
	if(((U_SINT32)event_p->execStatus >= 0) && 
	 ((U_SINT32)event_p->getCmdId() < UCONN_LAST_CMD_ID) &&
	 ((U_SINT32)event_p->getSize() > 0) && ((U_SINT32)event_p->getCmdId() > 0))
	{
		for(idx = 0; idx < UCONN_MAX_CLIENTS_SUPPORTED; idx++)
		{
			take_lock(&sendDataLock);
			//printf("******************************%d: CallBackEvent %d \n", event_p->getCmdId(), event_p->getSize());

			send_data(gClientDetails[idx].clientSock, (U_SINT8 *)event_p, 
				event_p->getSize());
				release_lock(&sendDataLock);
	}
		delete event_p;
		event_p = NULL;
	}
}


bool extractDevVidPidMiString(string &vidPidStr,string &vidStr,string &pidStr,string &miStr,string &onlyVidPidStr)
{
	bool ret = false;
	int idx = 0;
	//transform(vidPidStr.begin(), vidPidStr.end(),vidPidStr.begin(), ::toupper); 
	////cout << "In extractDevVidPidMiString-->devVidPidStr is" << vidPidStr << endl;
	//vidPidStr-->USB\VID_12D1&PID_1506
	string tmpStr;
	string tmpStr1;
	tmpStr = vidPidStr;

	if(strstr(tmpStr.c_str(),"USB\\"))
	{
		int idx = tmpStr.find('\\');
		tmpStr1 = tmpStr.substr(0,idx);

		if(!strstr(tmpStr1.c_str(),"USB"))
			return false;
		else
		{
			idx = tmpStr.find("USB\\");
			tmpStr1 = tmpStr.substr(idx+4,tmpStr.length());
		}
		tmpStr =tmpStr1;
		if(strstr(tmpStr.c_str(),"&"))
		{
			idx = tmpStr.find('&');
			/*extract VID*/
			vidStr = tmpStr.substr(0,idx);
			tmpStr1 = tmpStr.substr(idx+1,tmpStr.length());

			/*extract PID*/
			pidStr = tmpStr1;
			if(strstr(tmpStr1.c_str(),"Mi") || strstr(tmpStr1.c_str(),"MI"))
			{
				idx = tmpStr1.find('&');
				pidStr = tmpStr1.substr(0,idx);
				/*extract Mi*/
				miStr = tmpStr1.substr(idx+1,tmpStr1.length());

				onlyVidPidStr = vidStr;
				onlyVidPidStr += '&';
				onlyVidPidStr += pidStr;
				//onlyVidPidStr += '&';
				//onlyVidPidStr += miStr;
				
				ret = true;
			}
			else
			{
				pidStr = tmpStr1;
				onlyVidPidStr = vidStr;
				onlyVidPidStr += '&';
				onlyVidPidStr += pidStr;
				ret = true;
			}
		}
	}
	else
	{
		ret = false;
	}
	return ret;
}


void uniodeStringToCharStringConversion(short *src,char *dest)
{
	int i = 0;
	dest[0] = '\0';
	while(*src != 0)
	{
		dest[i] = (char)*src++;
		i++;
	}
	dest[i] = '\0';
}


#ifdef WIN32
bool UpdateDevice(PDEV_BROADCAST_DEVICEINTERFACE pDevInf, WPARAM wParam)
{
	bool ret = false;
	//from the config file
	//VidPidName:USB\Vid_198f&Pid_0100&Rev_0327&MI_02
	//VidPidName:USB\Vid_0a5c&Pid_0510&Rev_0100&Mi_03

	// pDevInf->dbcc_name: 
	// \\?\USB#Vid_0a5c&Pid_0510&Mi_03#DUMMY_03#{669305d7-e96c-4f9d-9947-57300ff003d3}

	// pDevInf->dbcc_name: 
	// \\?\USB#Vid_04e8&Pid_503b#0002F9A9828E0F06#{a5dcbf10-6530-11d2-901f-00c04fb951ed}
	// szDevId: USB\Vid_04e8&Pid_503b\0002F9A9828E0F06
	// szClass: USB
		
	//string szDevId = WstringToString(pDevInf->dbcc_name);
	//char temp[100];
	string devVidStr;
	string devPidStr;
	string devMiStr;
	string devClassStr;
	string onlyVidPidStr;
	string strFromDevice;// = pDevInf->dbcc_name;
	string devVidPidStr;// = strFromDevice;
	//sprintf(temp, "%d", pDevInf->dbcc_size);
	//WriteLog(temp);
	//sprintf(temp, "%d", pDevInf->dbcc_devicetype);
	//WriteLog(temp);

	char devRawString[450];
	devRawString[0]='\0';
	short *tmpP = (short*)pDevInf->dbcc_name;
	uniodeStringToCharStringConversion(tmpP,devRawString);

	/* FOR DLINK HSPA USB MODEM */
if(!strnicmp(devRawString, 
		"\\\\?\\USBSTOR#CdRom&Ven_HSPA_USB&Prod_SCSI_CD-ROM&Rev_6229",//#532274404164260&0#{53f56308-b6bf-11d0-94f2-00a0c91efb8b}",
		50))	{
		/* Call Modeswitch from the linked libray */
		char switchPacket[]={
		0x55 ,0x53 ,0x42 ,0x43 ,0x40 ,0xd4 ,0x87 ,0x04 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 
		,0x03 ,0xf0 ,0x01 ,0x01 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00
		,0x00 ,0x00 ,0x00};
		modeSwitch(devRawString, switchPacket);
	}

	//if(!strnicmp(devRawString, "\\\\?\\USB#VID_216F&PID_0041&MI_02#7&cd20479&0&0002#", 49))
	//{
	//	/* Altair Device specific call */
	//	pipetoLTEMgr();
	//}

	
	//WriteLog(devRawString);
	strFromDevice.clear();
	strFromDevice = devRawString;
	devVidPidStr = strFromDevice;
	tmpP = NULL;
	devRawString[0]= '\0';
	//return false;

	//WriteLog("Done with strings\n");

	ret = getVidPidStringFromUSBDeviceArival(devVidPidStr);
	if(false == ret)
	{
//		WriteLog("getVidPidStringFromUSBDeviceArival() failed");
		////cout << "Not expected device inserted-->Ignore callback" << endl;
		return ret;
	}
//	WriteSerLogStr("VidPidString is:",devVidPidStr);

	ret = extractDevVidPidMiString(devVidPidStr,devVidStr,devPidStr,devMiStr,onlyVidPidStr);

	if(false == ret)
	{
//		WriteLog("extractDevVidPidMiString() failed");
		////cout << "Not expected device inserted-->Ignore callback" << endl;
		return ret;
	}
//	WriteSerLogStr("devVidPidStr is:",devVidPidStr);
//	WriteSerLogStr("devVidStr is:",devVidStr);
//	WriteSerLogStr("devPidStr is:",devPidStr);
//	WriteSerLogStr("onlyVidPidStr is:",onlyVidPidStr);

	ret = matchInsertedDeviceInfoWithConfigFile(onlyVidPidStr,devMiStr);
	if(true == ret)
	{
//		WriteLog("FOUND------->device matched with the vidpid of the config file--Hurray");
		////cout << "FOUND------->device matched with the vidpid of the config file" << endl;
		ret = true;
	}
	else
	{
//		WriteLog("NOT FOUND------->device not matched");
		////cout << "NOT FOUND------->device not matched" << endl;
		return ret;
	}
	return ret;

}
#endif


#ifdef WIN32
bool DeviceChangeType(LPARAM lParam,WPARAM wParam)
{
	bool ret = false;
	PDEV_BROADCAST_HDR pHdr = (PDEV_BROADCAST_HDR)lParam;
	if ( DBT_DEVICEARRIVAL == wParam || DBT_DEVICEREMOVECOMPLETE == wParam )
    {
        PDEV_BROADCAST_HDR pHdr = (PDEV_BROADCAST_HDR)lParam;
	//	WriteSerLog("sizeof(DWORD)",sizeof(DWORD));
	//	WriteSerLog("size of struct", sizeof(DEV_BROADCAST_HDR));
	//	WriteSerLog("pHdr->dbch_size:",pHdr->dbch_size);
        switch( pHdr->dbch_devicetype )
        {
            case DBT_DEVTYP_DEVICEINTERFACE:
				{
					//WriteLog("DBT_DEVTYP_DEVICEINTERFACE type");
					PDEV_BROADCAST_DEVICEINTERFACE pDevInf = (PDEV_BROADCAST_DEVICEINTERFACE)(pHdr); /* temp only REVISIT */
					ret = UpdateDevice(pDevInf, wParam);
				}
                break;
			case DBT_DEVTYP_PORT:
				{
					PDEV_BROADCAST_PORT pDevPort = (PDEV_BROADCAST_PORT)pHdr;
					ret = false;
					// do something...
				}
                break;
			default:
					ret = false;
				break;
		}
	}
	return ret;
}


#endif


bool matchInsertedDeviceInfoWithConfigFile(string &onlyVidPidStr,string &devMiStr)
{
#ifdef WIN32
	bool ret = false;
	//WriteLog("start point of matchInsertedDeviceInfoWithConfigFile()");

	string tmpStr;
	//for(int VidCount = 0; VidCount < gCMInfo.VidPidCount; VidCount++)
	for(unsigned int VidCount = 0; VidCount < UCM_getConfig()->lteConfigInfo.vidpidCount; VidCount++)
    {
		tmpStr.clear();
	//	//cout << "onlyVidPidStr:" << onlyVidPidStr << endl;
	//	//cout << "config file's vid pid::" << UCM_getConfig()->lteConfigInfo.VidPidName[VidCount] << endl;
		////cout << gCMInfo.VidPidList[VidCount] << endl;

		tmpStr = UCM_getConfig()->lteConfigInfo.VidPidName[VidCount];
		if(stristr((char *)tmpStr.c_str(),onlyVidPidStr.c_str()))
		{
			int idx = 0;
			if(strstr(tmpStr.c_str(),"&"))
			{
				idx = tmpStr.rfind('&');
				tmpStr = tmpStr.substr(idx+1,tmpStr.length());

				transform(tmpStr.begin(), tmpStr.end(),tmpStr.begin(), ::toupper);
				transform(devMiStr.begin(), devMiStr.end(),devMiStr.begin(), ::toupper);

				if(devMiStr.length() >= 1)
				{
					if(!tmpStr.find(devMiStr))
					{
	//					//cout << "Device is mathed with the config file------>Hurray" << endl;
						ret = true;
						UCM_getConfig()->insertedDeviceTechID = LTE_TECH;

						U_UINT32 pos = devMiStr.find("MI_");
						std::string tmpid = devMiStr.substr(pos + 3, 2);
						//gCMInfo.APNId = atoi(tmpid.c_str());
						UCM_getConfig()->lteConfigInfo.APNId = atoi(tmpid.c_str());
					}
				}
				else
				{
	//				//cout << "Device is mathed with the config file------>Hurray" << endl;
					ret = true;
					UCM_getConfig()->insertedDeviceTechID = LTE_TECH;
				}
			}
		}
	}

	if(ret == true)
		return true;

	/*for device related to 3G is check here*/
	if(ret == false)
	{
		tmpStr.clear();
		for(unsigned int VidCount = 0; VidCount < UCM_getConfig()->threeGConfigInfo.ThreeG_vidpidCount; VidCount++)
		{
			tmpStr.clear();
	//		//cout << "onlyVidPidStr:" << onlyVidPidStr << endl;
	//		//cout << "config file's vid pid::" << UCM_getConfig()->threeGConfigInfo.ThreeG_VidPidName[VidCount] << endl;
			////cout << gCMInfo.VidPidList[VidCount] << endl;

			tmpStr = UCM_getConfig()->threeGConfigInfo.ThreeG_VidPidName[VidCount];
			if(stristr((char *)tmpStr.c_str(),onlyVidPidStr.c_str()))
			{
				int idx = 0;
				if(strstr(tmpStr.c_str(),"&"))
				{
					idx = tmpStr.rfind('&');
					tmpStr = tmpStr.substr(idx+1,tmpStr.length());

					transform(tmpStr.begin(), tmpStr.end(),tmpStr.begin(), ::toupper);
					transform(devMiStr.begin(), devMiStr.end(),devMiStr.begin(), ::toupper);
					if(!tmpStr.find(devMiStr))
					{
	//					//cout << "Device is mathed with the config file------>Hurray" << endl;
						ret = true;
						UCM_getConfig()->insertedDeviceTechID = THREE_G_TECH;

						//U_UINT32 pos = devMiStr.find("MI_");
						//std::string tmpid = devMiStr.substr(pos + 3, 2);
						//gCMInfo.APNId = atoi(tmpid.c_str());
						//UCM_getConfig()->lteConfigInfo.APNId = atoi(tmpid.c_str());
					}
				}
			}
		}
	}


	//WriteLog("end point of matchInsertedDeviceInfoWithConfigFile()");
	return ret;
#endif
}


bool getVidPidStringFromUSBDeviceArival(string &devVidPidStr)
{
	//WriteLog("start point of getVidPidStringFromUSBDeviceArival()");
	//WriteSerLogStr("devVidPidStr:",devVidPidStr);
	bool ret = false;

	if(devVidPidStr.length() < 6)
		return false;

	devVidPidStr = devVidPidStr.substr(4);

	int idx = 0;
	if(strstr(devVidPidStr.c_str(),"#"))
	{
		idx = devVidPidStr.rfind('#');
		devVidPidStr = devVidPidStr.substr(0,idx);
	}

	if(strstr(devVidPidStr.c_str(),"#"))
	{
		idx = devVidPidStr.rfind('#');
		devVidPidStr = devVidPidStr.substr(0,idx);
		replaceAll(devVidPidStr,"#","\\");
		ret = true;
	} 

	return ret;
}

void StopService()
{

#ifdef WIN32
	SERVICE_STATUS_PROCESS ssp;
	DWORD dwBytesNeeded;
	SC_HANDLE serviceControlManager = OpenSCManager( 0, 0, SC_MANAGER_CONNECT );

	if ( serviceControlManager )
	{
		SC_HANDLE service = OpenService( serviceControlManager,
			serviceName, SERVICE_QUERY_STATUS | SERVICE_STOP );
		if ( service )
		{
			//SERVICE_STATUS serviceStatus; // initialize serviceStatus. Maybe this is causing crash
			//if ( QueryServiceStatus( service, &serviceStatus ) )
			 if (!QueryServiceStatusEx(service,SC_STATUS_PROCESS_INFO,(LPBYTE)&ssp,sizeof(SERVICE_STATUS_PROCESS),  &dwBytesNeeded))
			{
				if ( ssp.dwCurrentState == SERVICE_STOPPED )
				{
					DeleteService( service );
	//				MY_LOG("UnInstalled");
				}
			}

			/*if (ssp.dwCurrentState == SERVICE_STOPPED)
			{
				//goto stop_cleanup;
			}*/

			// Send a stop code to the service.
			if (!ControlService(service,SERVICE_CONTROL_STOP,(LPSERVICE_STATUS)&ssp))
			{
				//wprintf(L"ControlService() failed, error %u\n", GetLastError());
				//goto stop_cleanup;
			}
			  //else
					//wprintf(L"ControlService() is OK!\n");

			CloseServiceHandle( service );
		}

		CloseServiceHandle( serviceControlManager );
	}
#endif
}

#ifdef WIN32
/*Initialize Vpn*/
void initialiseVpn()
{

	HANDLE hToken;
	char fileExeChar[256];
	string fileLocation = "";

	/*GetCommandLine gets the path of the current working directory*/
	LPSTR exePath = GetCommandLine();
	//wcstombs(fileExeChar, exePath, wcslen(exePath));
	string exePathStr = exePath;
	strcpy(fileExeChar,exePathStr.c_str());
	string serverExe = "CMsocketservice.exe";

	/*Loop to remove the CMsocketservice.exe from the path obtained */
	for(unsigned int i=0;i<exePathStr.length()-(serverExe.length() + 1);i++)
	{
		fileLocation += fileExeChar[i];
	}

	LPCTSTR fileLocationLp = const_cast<LPCTSTR>(fileLocation.c_str());
	/*Sets the directory path obtained from the getcommandline api*/
	SetCurrentDirectory(fileLocationLp);
	
	LPCSTR progName = (LPCSTR)"vpn.exe";

	char errStr[100];
	DWORD dwCreationFlag = NORMAL_PRIORITY_CLASS ;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	WTSQueryUserToken (WTSGetActiveConsoleSessionId (), &hToken);
	ZeroMemory( &si, sizeof( STARTUPINFO ) );
	si.cb = sizeof( STARTUPINFO );
	si.lpDesktop = (LPSTR)"winsta0\\default";

	ZeroMemory( &pi,sizeof(pi));
	/*Create process as user*/
	if ( !CreateProcessAsUser(
						  hToken,
						  progName,
						  NULL,  
						  NULL,
						  NULL,
						  FALSE,
						  dwCreationFlag,
						  NULL,
						  NULL,
						  &si,
						  &pi
			  ) )
	{
		sprintf(errStr, "CreateProcessAsUser Failed %d\n", GetLastError());
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
		CloseHandle(hToken);
	} 
	else
	{
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
		CloseHandle(hToken);
	}

}
#endif

#ifdef WIN32
/* Initialize LTE Manager for Altair */
void pipetoLTEMgr()
{

	HANDLE hToken;
	char fileExeChar[256];
	string fileLocation = "";

	/*GetCommandLine gets the path of the current working directory*/
	LPSTR exePath = GetCommandLine();
	//wcstombs(fileExeChar, exePath, wcslen(exePath));
	string exePathStr = exePath;
	strcpy(fileExeChar,exePathStr.c_str());
	string serverExe = "CMsocketservice.exe";

	/*Loop to remove the CMsocketservice.exe from the path obtained */
	for(unsigned int i=0;i<exePathStr.length()-(serverExe.length() + 1);i++)
	{
		fileLocation += fileExeChar[i];
	}

	LPCTSTR fileLocationLp = const_cast<LPCTSTR>(fileLocation.c_str());
	/*Sets the directory path obtained from the getcommandline api*/
	SetCurrentDirectory(fileLocationLp);
	
	LPCSTR progName = (LPCSTR)"test_pipe.exe";

	char errStr[100];
	DWORD dwCreationFlag = NORMAL_PRIORITY_CLASS ;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	WTSQueryUserToken (WTSGetActiveConsoleSessionId (), &hToken);
	ZeroMemory( &si, sizeof( STARTUPINFO ) );
	si.cb = sizeof( STARTUPINFO );
	si.lpDesktop = (LPSTR)"winsta0\\default";

	ZeroMemory( &pi,sizeof(pi));
	/*Create process as user*/
	if ( !CreateProcessAsUser(
						  hToken,
						  progName,
						  NULL,  
						  NULL,
						  NULL,
						  FALSE,
						  dwCreationFlag,
						  NULL,
						  NULL,
						  &si,
						  &pi
			  ) )
	{
		sprintf(errStr, "CreateProcessAsUser Failed %d\n", GetLastError());
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
		CloseHandle(hToken);
	} 
	else
	{
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
		CloseHandle(hToken);
	}

}
#endif
