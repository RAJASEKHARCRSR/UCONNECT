//#include "USrPortCommUsage.h"
//#include <windows.h>
#include "uconn_OSAL.h"
#ifdef WIN32
#include <process.h>
#endif /*WIN32*/
#include <string>
//#include "UEmulateSerialPort.h"
#include "uconn_ServerAPI.h"
#include "uconn_Event.h"
#include "uconn_AT_DAL.h"
#define EMULATOR_MODE 0

#if (defined(__linux__)||defined(__MACH__))
#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#endif /*__linux__*/

extern P_AT_CALLBACK_WITH_ID emulator_callbackfnWithId_ptr;

USrPortCommUsage::USrPortCommUsage()
{
#ifdef WIN32
	m_Port = INVALID_HANDLE_VALUE;
#endif /*WIN32*/

}

USrPortCommUsage::~USrPortCommUsage()
{


}

bool USrPortCommUsage::UInitialiseSerialPort(string szPortName="COM5", 
											 U_UINT32 dwBaudRate= 9600, 
											 U_UINT8 byParity = 0, 
											 U_UINT8 byStopBits =1, 
											 U_UINT8 byByteSize = 8, 
											 U_UINT8 dalMode = 1)
{
#if(EMULATOR_MODE)
	{
		emulator_callbackfnWithId_ptr("",3);
		return true;
	}
#endif

#ifdef WIN32
    std::wstring comID;
#ifndef QTPROJECT
    wstring wstr(szPortName.length(),L'\0');
	copy(szPortName.begin(),szPortName.end(),wstr.begin());
	std::wstring comPrefix = L"\\\\.\\"; 
    comID = comPrefix + wstr;
	//std::wstring comPrefix = L"\\\\.\\COM19"; 
	//comID = comPrefix + wstr;
#endif
	m_Port = ::CreateFile((LPCWSTR)comID.c_str(),  // Pointer to the name of the port
								GENERIC_READ | GENERIC_WRITE,//access ( read and write) mode
								0,	//(share mode) 0:cannot share the COM port						
								0,	//security  (None)				
								OPEN_EXISTING,// creation : open_existing
								0,//FILE_FLAG_OVERLAPPED,// we want overlapped operation
								NULL);// no templates file for COM port...

	if ( m_Port == INVALID_HANDLE_VALUE )
	{
		////cout<< "Failed to open COM Port Reason:" << GetLastError() << endl;
		int err = GetLastError();
		//LOG(ULOG_DEBUG_LEVEL, "COM port opend failed with: %s\n",err);
		return false;
	}
	////cout << "GetLastError():" << GetLastError() << endl;

	
	////cout << "COM port opened successfully" << endl;
	
	//set the COM port settings and the timeouts	
	
	/* Setting Serial port parameters */
	DCB dcb = {0};
	
	/* Initialize the DCBlength member*/
	dcb.DCBlength = sizeof(DCB);
	
	// Get the default port setting information.
	if (!GetCommState (m_Port,&dcb))
	{
		////cout << "Failed to Get Comm State Reason" << GetLastError() << endl;
		return false;
	}
	////cout << "GetLastError():" << GetLastError() << endl;
	
	// Change the DCB structure settings with new inputs.
	dcb.BaudRate	= dwBaudRate;
	dcb.ByteSize	= byByteSize;
	dcb.Parity		= byParity;
	if ( byStopBits == 1 )
		dcb.StopBits	= ONESTOPBIT;
	else if (byStopBits == 2 ) 
		dcb.StopBits	= TWOSTOPBITS;
	else 
		dcb.StopBits	= ONE5STOPBITS;

	dcb.fDsrSensitivity = 0;
	dcb.fDtrControl = DTR_CONTROL_ENABLE;
	dcb.fOutxDsrFlow = 0;

	/*Configure the port according to the specifications of the DCB structure. */
	if (!::SetCommState (m_Port,&dcb))
	{
		////cout << "Failed to Set Comm State Reason:" << GetLastError() << endl;
		return false;
	}
	////cout << "GetLastError():" << GetLastError() << endl;
	////cout << "New Port Settings done in accordance with input parameters"<< endl; 
	
	////cout << "Current Port Settings, Baud Rate:" << dcb.BaudRate;
	////cout  << "\nParity:" << (int)dcb.Parity;
	////cout  << "\nByte Size:" << (int)dcb.ByteSize;
	////cout  << "\nStop Bits:" << (int)dcb.StopBits << endl;
		
	//now set the timeouts ( we control the timeout overselves using WaitForXXX()
	COMMTIMEOUTS timeouts;
	
	/*timeouts.ReadIntervalTimeout					= MAXDWORD; 
	timeouts.ReadTotalTimeoutMultiplier		= 0;
	timeouts.ReadTotalTimeoutConstant			= 0;
	timeouts.WriteTotalTimeoutMultiplier	= 0;
	timeouts.WriteTotalTimeoutConstant		= 0;*/
	/*timeouts.ReadIntervalTimeout = 0xFFFFFFFF;
    timeouts.ReadTotalTimeoutMultiplier = 0;
    timeouts.ReadTotalTimeoutConstant = 0;
    timeouts.WriteTotalTimeoutConstant = 0;
    timeouts.WriteTotalTimeoutMultiplier = 1;*/
	
	if(dalMode == 2)
	{
		timeouts.ReadIntervalTimeout = 0xFFFFFFFF;
		timeouts.ReadTotalTimeoutMultiplier = 0;
		timeouts.ReadTotalTimeoutConstant = 2;
		timeouts.WriteTotalTimeoutConstant = 100;
		timeouts.WriteTotalTimeoutMultiplier = 0;
	}
	else
	{
		timeouts.ReadIntervalTimeout = 0xFFFFFFFF;
		timeouts.ReadTotalTimeoutMultiplier = 0;
		timeouts.ReadTotalTimeoutConstant = 0;
		timeouts.WriteTotalTimeoutConstant = 0;
		timeouts.WriteTotalTimeoutMultiplier = 0;
	}

	if (!SetCommTimeouts(m_Port, &timeouts))
	{
		////cout << "CSerialCommHelper :  Error setting time-outs:" << GetLastError() << endl;
		return false;
	}

	////cout << "GetLastError():" << GetLastError() << endl;

	// Specify a set of events to be monitored for the port.
	if (!SetCommMask(m_Port,EV_RXCHAR|EV_TXEMPTY|EV_CTS|EV_DSR|EV_RLSD|EV_RING|EV_ERR|EV_BREAK) )
	{
		////cout << "Failed to Set Comm Mask Reason:" << GetLastError() << endl;
		return false;
	}
	////cout << "GetLastError():" << GetLastError() << endl;
	////cout << "SetCommMask() success" << endl;

	PurgeComm(m_Port,PURGE_TXCLEAR | PURGE_TXABORT | PURGE_RXCLEAR | PURGE_RXABORT);
	////cout << "GetLastError():" << GetLastError() << endl;

	COMSTAT comStat={0};
	U_DWORD status = 0;
	getcommSerialErrorReport(comStat,status);
	////cout << "GetLastError():" << GetLastError() << endl;

	if(comStat.cbInQue > 1)
	{
		////cout << "*******************************************************" << endl;
		////cout << "FOUND SOME OLD BYTES FROM PORT--->ALLLLLLLLLLLLLLAAAAAAAAAARRRRRRRRMMMMMMMM" << endl;
		////cout << "*******************************************************" << endl;
	}

	PurgeComm(m_Port,PURGE_TXCLEAR | PURGE_TXABORT | PURGE_RXCLEAR | PURGE_RXABORT);
	////cout << "GetLastError():" << GetLastError() << endl;

	return true;
#endif /*WIN32*/

#if (defined(__linux__)||defined(__MACH__))
	char sPortName[100];
	//printf("in OpenAdrPort port#=%s\n", sPortNumber);
	sprintf(sPortName, "/dev/ttyS%s", szPortName);
	//printf("sPortName=%s\n", sPortName);

	// make sure port is closed
	if(serialPort_fd > 0)
	close(serialPort_fd);


	serialPort_fd = open(sPortName, O_RDWR | O_NOCTTY | O_NDELAY);
	if (-1 == serialPort_fd)
	{
		/*Could not open the port.*/
		//printf("open error %d %s\n", errno, strerror(errno));
		//cout << "open error:" << strerror(errno);
	}
	else
		fcntl(serialPort_fd, F_SETFL, 0);


	/*configure the serial port here for linux*/
	struct termios options;
	/*Get the current options for the port...*/
	tcgetattr(serialPort_fd, &options);

    	/*Set the baud rates to 19200...*/
	//cfsetispeed(&options, dwBaudRate);
	//cfsetospeed(&options, dwBaudRate);

	/*Enable the receiver and set local mode...*/
	//options.c_cflag |= (CLOCAL | CREAD);

	/*Set the new options for the port...*/
	//tcsetattr(fd, TCSANOW, &options);

	// set new port settings for canonical input processing
	U_UINT8 STOPBITS;
	switch (byStopBits)
	{
	 case 1:
	 default:
	    STOPBITS = 0;
	    break;
	 case 2:
	    STOPBITS = CSTOPB;
	    break;
	}  //end of switch stop bits

	U_UINT8 DATABITS;
	switch (byByteSize)
	{
	case 8:
	default:
		DATABITS = CS8;
		break;
	}  //end of switch data_bits

	U_UINT32 BAUD;
	switch (dwBaudRate)
	{
	case 38400:
	default:
		BAUD = B38400;
		break;
	case 19200:
		BAUD  = B19200;
		break;
	case 9600:
		BAUD  = B9600;
		break;
	}
 
	options.c_cflag = BAUD | CRTSCTS | DATABITS | STOPBITS | byParity | CLOCAL | CREAD;
	//options.c_iflag = IGNPAR;
	options.c_oflag = 0;
	options.c_lflag = 0;       //ICANON;
	//newtio.c_cc[VMIN]=1;
	//newtio.c_cc[VTIME]=0;
	tcflush(serialPort_fd, TCIFLUSH);
	
	/*Set the baud rates to BAUD...*/
	cfsetispeed(&options, BAUD);
	cfsetospeed(&options, BAUD);

	tcsetattr(serialPort_fd,TCSANOW,&options);

	return true;

#endif /*__linux__*/
}



#ifdef WIN32
U_UINT32 USrPortCommUsage::getNumOfBytesToRead(U_DWORD &status)
{
		COMSTAT comStat={0};
		this->getcommSerialErrorReport(comStat,status);
		return comStat.cbInQue;
}
#endif /*WIN32*/


U_UINT32 USrPortCommUsage::read(char* buffer,int maxBytes,U_DWORD &status)
{
#ifdef WIN32
	int n = 0;
	BOOL abRet = false;
	U_DWORD  dwIncommingReadSize = 0;


	if (m_Port == NULL)
		return 0;

	if(buffer!= NULL)
	{
		abRet = ReadFile(m_Port, buffer, maxBytes, &dwIncommingReadSize, NULL);
		if(!abRet)
		{
			status = GetLastError();
			//::Sleep(10);
			////cout << "Errors in ReadFile:" << status << endl;
			return 0;
		}
	}

	return dwIncommingReadSize;
#endif /*WIN32*/

#if (defined(__linux__)||defined(__MACH__))
	int iIn;
	if (serialPort_fd < 1)
	{
		//cout << "Port is not opened" << endl;
		return 0;
	}
	iIn = ::read((int)serialPort_fd, (void*)buffer, (size_t)maxBytes);
	if (iIn < 0)
	{
		if (errno == EAGAIN)
		{
			return 0; // assume that command generated no response
		}
		else
		{
			//printf("read error %d %s\n", errno, strerror(errno));
			//cout << "read error:" << errno << endl;
		} // end if
		iIn = 0;
	}
	else
	{
		//psResponse[iIn<iMax?iIn:iMax] = '\0';
		//printf("read %d chars: %s\n", iIn, psResponse);
		//cout << "No. of chars read:" << iIn << endl;
		//cout << "Chars read are:" << buffer << endl;
		
	} // end if

	return (U_UINT32)iIn;

#endif /*__linux__*/

 }


U_BOOL USrPortCommUsage::write(const char* data,U_DWORD dwSize,U_DWORD &status)
{
#if(EMULATOR_MODE)
	{
		string tmpCmdStr(data);
		Emulator_For_AT_Commands(tmpCmdStr);
		return true;
	}
#endif

	bool iRet = false;
	U_DWORD dwBytesWritten =0;
#ifdef WIN32
	

	if(m_Port == INVALID_HANDLE_VALUE)
	{
		////cout << "Port is not opened" << endl;
		return false;
	}

	////cout << "AT Command:" << data << endl;
	LOG(ULOG_DEBUG_LEVEL, "AT Command: \n %s\n",data);
	iRet = WriteFile (m_Port,data,dwSize,&dwBytesWritten  ,0);

	if(!iRet)
	{
		status = GetLastError();
		////cout << "Error in WriteFile:" << status << endl;
		return false;
	}

	if(dwBytesWritten < 1)
	{
		////cout << "ERROR::Write Port Operation failed" << endl;
		status = GetLastError();
		////cout << "Errors in WriteFile:" << status << endl;
		return false;
	}
	
	return true;
#endif /*WIN32*/
	
#if (defined(__linux__)||defined(__MACH__))

	//U_BOOL iRet = false;
	if (serialPort_fd < 1)
	{
		//printf(" port is not open\n");
		//cout << "port is not open" << endl;
		return false;
	}


	U_INT iOut = ::write(serialPort_fd, data, dwBytesWritten);
	if (iOut < 0)
	{
		//fputs("write() of 4 bytes failed!\n", stderr);
		//cout << "write error: " << errno << endl;
		iRet = false;
	}
	else
	{
		//cout << "write these many chars: " << iOut << endl;
		iRet = true;
	}

	return iRet;
#endif /*__linux__*/
}

#ifdef WIN32
void USrPortCommUsage::getcommSerialErrorReport(COMSTAT &comStat,U_DWORD &status)
{
		// Get and clear current errors on the port.      
		DWORD   dwErrors;
		if (!ClearCommError(m_Port, &dwErrors, &comStat))  
		{
			////cout << "Reporting error" << endl;
			status = GetLastError();
			////cout << "Errors in ClearCommError:" << status << endl;
			return;
		}
		if(dwErrors)
		{
			////cout << "serial port error::dwErrors-->" << dwErrors << endl;
		}

		if (comStat.fCtsHold)
		{
			////cout << "comStat.fCtsHold" << endl;
		}
		if (comStat.fDsrHold)
		{
			////cout << "comStat.fDsrHold" << endl;
		}
		if (comStat.fRlsdHold)
		{
			////cout << "comStat.fRlsdHold" << endl;
		}
		if (comStat.fXoffHold)
		{
			////cout << "comStat.fXoffHold" << endl;
		}
		if (comStat.fXoffSent)
		{
			////cout << "comStat.fXoffSent" << endl;
		}
		if (comStat.fEof)
		{
			////cout << "comStat.fEof" << endl;
		}
		if (comStat.fTxim)
		{
			////cout << "comStat.fTxim" << endl;
		}
		if (comStat.cbInQue)
		{
			////cout << "comStat.cbInQue********************************:" << comStat.cbInQue << endl;
		}
		if (comStat.cbOutQue)
		{
			////cout << "comStat.cbOutQue" << endl;
		}
}
#endif /*WIN32*/

