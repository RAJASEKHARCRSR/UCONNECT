#include "uconn_ATHandler.h"
#include "uconn_AT_DAL.h"


static bool notOpenCallback = false;


UConnATHandler::UConnATHandler()
{
	m_pNM = NULL;
	mWriteLock = new USemaphore();   
	bStartThreadOnce = false;
     
}


UConnATHandler::~UConnATHandler()
{
	if(!m_pNM)
		return;

	if (!m_pNM->mCommonInfo.isEmulatorMode)
        {
                                //mWriteLock->release();
            LOG(ULOG_DEBUG_LEVEL, "deleted mWriteLock semaphore mWriteLock value: \n %d",mWriteLock);
            ////cout<<"deleted mWriteLock semaphore mWriteLock:" << mWriteLock<< endl;
            //delete mWriteLock;
        }

  //  printf("deleted mWriteLock semaphore\n");

	

        if(m_pNM->mCommonInfo.isEmulatorMode) {

          //  printf("the UConnATHandler destructor is called");
            LOG(ULOG_DEBUG_LEVEL, "the UConnATHandler destructor is called \n ");
		delete uEmulator;
	}
}

void UConnATHandler::Init(U_UINT8 *pu8TaskName ,U_UINT32 u32Priority,
						  U_UINT32 u32StackSize,U_UINT32 u16MaxQDepth, U_PVOID pvContext) {

	  

      if (m_pNM->mCommonInfo.isEmulatorMode)
	  {
			return;
	  }
	  else 
	  {
		  UProcess::Init();
	  }

}

U_BOOL UConnATHandler::initialize(U_STRING sComPort,
								  P_AT_CALLBACK_WITH_ID callback_fn,
								  UNetworkManager* pNM)
{

	this->m_pNM = pNM;

	if(m_pNM->mCommonInfo.isEmulatorMode)
	{
		uEmulator = new UEmulator(this->m_pNM);
	}



	////cout << "UConnATHandler::initialize is called" << endl;
//	if(false == initialiseCallBackFunctionOnce)
//	{
		m_callbackfnWithId_ptr = callback_fn;
//	}
	//mSerialPort.openDeviceWithID(callback_fn);
	/*Initialise the serial port here */
	//string portName="\\\\.\\" + sComPort;
	string portName = sComPort;
	//bool ret1 = InitialiseSerialPort(portName,9600,0,1,8);115200
    bool ret1;
//#ifdef WIN32

    if(!m_pNM->mCommonInfo.isEmulatorMode)
	{
		#if defined(WIN32) && !defined(QTPROJECT)
		ret1 = mSerialPort.UInitialiseSerialPort(portName,9600,0,1,8,
			m_pNM->mCommonInfo.dalMode);
		#endif
	}
	else
	{
		//printf("inside emulator mode returning true in intialize\n");

		sComPort = "COM 3";
		portName = sComPort;
		ret1 = true;
	}
//#endif
	if(ret1 == false)
	{
        //cout << "ERROR::InitialiseSerialPort() failed\n" << endl;
		return DAL_ERROR;
	}
	

	if(false == bStartThreadOnce)
	{
		this->Init();
		bStartThreadOnce = true;
	}

	/* To indicate that the device is Open */
	//m_callbackfnWithId_ptr("", 3);//Has to remove other than emulator

	
	//OsaSleep(1000);
	mWriteLock->release();

	return DAL_SUCCESS;
}



U_BOOL UConnATHandler::EventHandler(){
	string respStr="";

	while(1)
	{
		char szBuf[4000] = {0};
		int nBytesRead = 0;
		vector<string> vRespStr;

		U_DWORD status = 0;
#if defined(WIN32) && !defined(QTPROJECT)

		nBytesRead = mSerialPort.read(szBuf,4000,status);
#endif
		//if((status == 995) && (nBytesRead == 0))
		//{
			if(status != 0 )
			{
				if((status != 995) && (nBytesRead == 0))
				{
					//LOG(ULOG_DEBUG_LEVEL, "Reader read with status non zero");
					mWriteLock->release();
					deviceStatus(status);
					//Sleep(500);
					continue;
				}
			}
		//}

		if(nBytesRead > 0)// && status == 0)
		{
			notOpenCallback = false;
			if(!strcmp(respStr.c_str(),"\n"))
			{
				respStr.clear();
			}
			respStr = respStr + szBuf;

			vRespStr.clear();

			tokeniseInformation(respStr,vRespStr,'\n');

			respStr.clear();

			partiallyParse(vRespStr,respStr);
		}
		else
		{
			OsaSleep(100);
		}
	}
}

void UConnATHandler::tokeniseInformation(string &rspStr, vector<std::string>& vStr, char ch)
{
	string s = rspStr;
	string sChar;
	sChar.clear();
	sChar += ch;
	string::size_type prev_pos = 0, pos = 0;     
	while( (pos = s.find(ch, pos)) != string::npos )     
	{         
		string substring( s.substr(prev_pos, pos-prev_pos) );
		vStr.push_back(substring);      
		vStr.push_back(sChar);
		prev_pos = ++pos;     
	}     
	string substring( s.substr(prev_pos, pos-prev_pos) ); 
	vStr.push_back(substring);
}

void UConnATHandler::partiallyParse(vector<std::string>& vRespStr,string &historyStr)
{
	historyStr.clear();
	unsigned int vSize = vRespStr.size();
	for(U_UINT32 i=0;i<vRespStr.size();++i)
	{
		int iPos = 0;
		if( ((iPos = vRespStr[i].find("OK\r",0)) != string::npos))
		{
			historyStr = historyStr + vRespStr[i];
			//historyStr = historyStr + '\n';
			////cout << "**************START***************" << endl;
			////cout << historyStr << endl;
			////cout << "**************END***************" << endl;

			LOG(ULOG_DEBUG_LEVEL,"AT Command Response: \n%s\n",historyStr.c_str());

			m_callbackfnWithId_ptr(historyStr, 1, this->m_pNM);
			historyStr.clear();
			mWriteLock->release();
			////cout << "LOCK released###########################################################" << endl;
		}
		else if( ((iPos = vRespStr[i].find("ERROR\r",0)) != string::npos))
		{
			historyStr = historyStr + vRespStr[i];
			//historyStr = historyStr + '\n';
			////cout << "**************START***************" << endl;
			////cout << historyStr << endl;
			////cout << "**************END***************" << endl;
			LOG(ULOG_DEBUG_LEVEL,"ERROR: \n%s\n",historyStr.c_str());
			m_callbackfnWithId_ptr(historyStr, 1, m_pNM);
			historyStr.clear();
			mWriteLock->release();
			////cout << "LOCK released###########################################################" << endl;
		}
		else if( ((iPos = vRespStr[i].find("+CME ERROR:",0)) != string::npos))
		{
			historyStr = historyStr + vRespStr[i];
			//historyStr = historyStr + '\n';
			////cout << "**************START***************" << endl;
			////cout << historyStr << endl;
			////cout << "**************END***************" << endl;
			LOG(ULOG_DEBUG_LEVEL,"CME ERROR: \n%s\n",historyStr.c_str());
			m_callbackfnWithId_ptr(historyStr, 1, m_pNM);
			historyStr.clear();
			mWriteLock->release();
			////cout << "LOCK released###########################################################" << endl;
		}
		else if ( ((iPos = vRespStr[i].find("+CGREG:",0)) != string::npos))
		{
			if ( ((iPos = vRespStr[i].find(",",0)) != string::npos))
			{
				historyStr = vRespStr[i] +  vRespStr[i+1] ;
				//historyStr = historyStr + '\n';
				////cout << "**************START***************" << endl;
				////cout << historyStr << endl;
				////cout << "**************END***************" << endl;
				LOG(ULOG_ERROR_LEVEL,"CEREG resp: \n%s\n",historyStr.c_str());
				m_callbackfnWithId_ptr(historyStr, 1, m_pNM);
				historyStr.clear();
			}
			else 
			{
				historyStr = vRespStr[i];
				//historyStr = historyStr + '\n';
				////cout << "**************START***************" << endl;
				////cout << historyStr << endl;
				////cout << "**************END***************" << endl;
				LOG(ULOG_DEBUG_LEVEL,"CEREG resp: \n%s\n",historyStr.c_str());
				m_callbackfnWithId_ptr(historyStr, 2, m_pNM);
				historyStr.clear();
			}
		}
		else if ( ((iPos = vRespStr[i].find("^RSSI",0)) != string::npos))
		{
			historyStr = vRespStr[i];
			//historyStr = historyStr + '\n';
			////cout << "**************START***************" << endl;
			////cout << historyStr << endl;
			////cout << "**************END***************" << endl;
			LOG(ULOG_DEBUG_LEVEL,"CEREG resp: \n%s\n",historyStr.c_str());
			m_callbackfnWithId_ptr(historyStr, 2, m_pNM);
			historyStr.clear();
		}
		else if( ((iPos = vRespStr[i].find("+CUSD:",0)) != string::npos))
		{
			//historyStr = vRespStr[i];
			
			for ( unsigned int ii=i; ii< vSize; ii++)
			{
				historyStr = historyStr + vRespStr[ii];
				if ( ((iPos = vRespStr[ii].find(",15",0)) != string::npos))
				{
						break;
				}
			}

			//historyStr = historyStr + '\n';
			////cout << "**************START***************" << endl;
			////cout << historyStr << endl;
			////cout << "**************END***************" << endl;
			LOG(ULOG_DEBUG_LEVEL,"CUSD: \n%s\n",historyStr.c_str());
			m_callbackfnWithId_ptr(historyStr, 2, m_pNM);
			historyStr.clear();
		}
		else if( ((iPos = vRespStr[i].find("+CGEV:",0)) != string::npos) ||
				 ((iPos = vRespStr[i].find("+MIMSEV:",0)) != string::npos) ||
				 ((iPos = vRespStr[i].find("+CEREG:",0)) != string::npos) ||
				 ((iPos = vRespStr[i].find("*MRDY:",0)) != string::npos) ||
				 ((iPos = vRespStr[i].find("*MSDI:",0)) != string::npos))
		{
			historyStr = historyStr + vRespStr[i];
			//historyStr = historyStr + '\n';
			////cout << "**************START***************" << endl;
			////cout << historyStr << endl;
			////cout << "**************END***************" << endl;
			LOG(ULOG_DEBUG_LEVEL,"AT Command resp: \n%s\n",historyStr.c_str());
			m_callbackfnWithId_ptr(historyStr, 2, m_pNM);
			historyStr.clear();
		}
		else
		{
			int len = strlen(vRespStr[i].c_str());
			
			if((vSize == (i+1)) && len==0)
			{
				historyStr = historyStr + vRespStr[i];
			}
			else
			{
				historyStr = historyStr + vRespStr[i];
				//historyStr = historyStr + '\n';
			}
		}
	}
}


U_BOOL UConnATHandler::sendATCommand(const char* data,U_DWORD dwSize)
{

	if(!m_pNM)
	{
		return false;
	}

        if(m_pNM->mCommonInfo.isEmulatorMode) {

		//printf("the command data sent is %s\n",data);

		//U_STRING dataCopy(data,dwSize);

		U_STRING dataCopy(data);


		//strcpy(dataCopy,data);

		//printf("dataCopy is %s\n",dataCopy);

		UEmulatorCommand* emulatorCommand = new UEmulatorCommand(dataCopy);

		

		//uEmulator->commandLock->lock();

		this->uEmulator->MsgPost((U_PUINT8)emulatorCommand);

		return true;

	}




#if defined(WIN32) && !defined(QTPROJECT)
	COMSTAT comStat={0};
	U_DWORD status = 0;
	mSerialPort.getcommSerialErrorReport(comStat,status);
	if(status != 0)
	{
		deviceStatus(status);
		return false;
	}

	if(comStat.cbInQue > 1)
	{
		////cout << "some bytes present in Input buffer*****************************************" << endl;
	}
	mWriteLock->lock(2000);
	////cout << "GOT LOCK ###########################################################" << endl;
	LOG(ULOG_DEBUG_LEVEL, "Sending AT Command: \n %s\n",data);
	status = 0;
	U_BOOL ret =  mSerialPort.write(data,dwSize,status);

	/*call callback function for device open here*/
	if(status != 0)
	{
		//mWriteLock->release();
		deviceStatus(status);
		return false;
	}
	notOpenCallback = false;

	return ret;
#else
    return true;
#endif    
}

void UConnATHandler::deviceStatus(U_DWORD &status)
{
        if (m_pNM->mCommonInfo.isEmulatorMode) {

		return;

	}

#if defined(WIN32) && !defined(QTPROJECT)
	if(status != 0)
	{
		switch(status)
		{
		
		case 995:
			if(m_pNM->mCommonInfo.dalMode == 2)
			{
				break;
			}
		case 1167:
		case 5:
		case 6:
		case 2:
			{
				//cout << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << endl;
				//cout << "[" << GetCurrentThreadId() << "]"<< "ERROR DEVICE is REMOVED" << endl;
				if(mSerialPort.m_Port != INVALID_HANDLE_VALUE)
				{
					CloseHandle(mSerialPort.m_Port);
					mSerialPort.m_Port = INVALID_HANDLE_VALUE;
				}
				string str = "";
				if(notOpenCallback == false)
				{
					//m_callbackfnWithId_ptr(str, 4);//Device is removed
					notOpenCallback = true;
				}
			
				//cout << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << endl;
				
				::Sleep(1000);
			}
			break;
		default:
			////cout << "Handle this error:" << status << endl;
			break;
		}
	}	
#endif    
}


void UConnATHandler::resetComPort()
{
#ifdef WIN32
	if(m_pNM == NULL) {
		return;
	}

	if(m_pNM->mCommonInfo.dalMode == 2)
	{
		if(mSerialPort.m_Port != INVALID_HANDLE_VALUE)
		{
			CloseHandle(mSerialPort.m_Port);
		}
		mSerialPort.m_Port = INVALID_HANDLE_VALUE;
	}

	if(notOpenCallback == false)
	{
		notOpenCallback = true;
	}
	m_pNM->mCommonInfo.com_port.clear();
	m_pNM->mCommonInfo.device_status = 0;
#endif
    
	//if(m_pNM->mCommonInfo.comPortCanBeOpened == false)
	//m_pNM->mCommonInfo.comPortCanBeOpened = true;
}

