#include <iostream>
#ifdef win32
#include <windows.h>
#endif
#include <string.h>
#include "UEmulateSerialPort.h"

using namespace std;

ST_CALLBACK_INFO gDeviceParams;

extern P_AT_CALLBACK_WITH_ID callbackfnWithId_ptr;

static int CONTRDP = 1;


UEmulateSerialPort::UEmulateSerialPort(UNetworkManager* pNM)
{
	emulState = EMULATOR__UNKNOWN;
	emulMode = EMULATOR__MODE_UNKNOWN;
	this->m_pNM = pNM;
	if(pNM->getTechID() == 0)
	{
		this->m_pLTE_NM = reinterpret_cast<ULTE_NM*>(pNM);
	}
	
	//for ME_PDN_ACT unsolicited response
	mePDNAct += 13;
	mePDNAct += 10;
	mePDNAct += "CGEV: ME PDN ACT 1";
	mePDNAct += 13;
	mePDNAct += 10;

	//for NW_ACT unsolicited response
	meAct += 13;
	meAct += 10;
	meAct += "CGEV: NW ACT 1,1,0";
	meAct += 13;
	meAct += 10;

	//for ME DEACT unsolicited response
	netDetachStr += 13;
	netDetachStr += 10;
	netDetachStr += "CGEV: ME DEACT 1,\"10.21.32.151\",1";
	netDetachStr += 13;
	netDetachStr += 10;

	//for REG STATUS unsolicited response
	regStatus += 13;
	regStatus += 10;
	regStatus += "CEREG: 1,0,\"00C3\",\"0x01234567\",4";
	regStatus += 13;
	regStatus += 10;
	CEREG_STATUS=0;
}

UEmulateSerialPort::~UEmulateSerialPort()
{}

void UEmulateSerialPort::addCRLFAtEnd(string &rspStr)
{
	rspStr += 13;
	rspStr += 10;
}

/*int generateRandom(int from,int to)
{
	int range = to - from;
	int random = (rand()%range) + from;
	return random;
}*/

void UEmulateSerialPort::genRandom(U_UINT32  from ,U_UINT32  to, string& rspStr)
{

	U_UINT32  range = to - from;
	U_UINT32  random = (rand()%range) + from;
	char buff[20];
	ITOA(random,buff,10);
	rspStr += buff;
	buff[0]='\0';
	strcpy(buff,"");

}

/*
void UEmulateSerialPort::genRandom(U_SINT32  from ,U_SINT32  to, string& rspStr)
{

	U_SINT32  range = to - from;
	U_SINT32  random = (rand()%range) + from;
	char buff[20];
	ITOA(random,buff,10);
	rspStr += buff;
	buff[0]='\0';
	strcpy(buff,"");

}
*/



void UEmulateSerialPort::addMCS(string& rspStr,int count)
{
	string mscString="MCS ";
    rspStr+= mscString;
	rspStr+= "0:(";
	int index=0;
	char buff[20];
	int i;


	while(index<32)
	{
		for(int j=0;j<count;j++)
		{
			genRandom(1,200,rspStr);
			if(j!=count-1)
			{
				rspStr+=",";
			}
		}
		
			rspStr+=")\n";
			++index;
			i = index;
			if(i!=32)
			{
				rspStr+=mscString;
				ITOA(i,buff,10);
				rspStr+=buff;
				rspStr+=":(";
			}
			buff[0]='\0';
			strcpy(buff,"");		
	}

}

void UEmulateSerialPort::addOKResponseAtEnd(string &rspStr)
{
	rspStr += 13;
	rspStr += 10;
	rspStr += "OK";
	rspStr += 13;
	rspStr += 10;
}


string UEmulateSerialPort::getRandSigParams(string param,int from,int to)
{
	if(to<from)
	{
		//getLogger()->log(ULOG_DEBUG_LEVEL,"Wrong range passed in device");
		return "";
	
	}
	
	int range = to - from;
	int sig = (rand()%range) + from;
	
	string rspStr="";
	std::stringstream response;
	response<<param<<": Reported = "<<((rand()%range) + from)<<", Rx0Tx0 = "<<((rand()%range) + from)<<", Rx0Tx1 = "<<((rand()%range) + from)<<", Rx1Tx0 = "<<((rand()%range) + from)<<", Rx1Tx1 = "<<((rand()%range) + from);
	rspStr += response.str();	
		
	return rspStr;

}

void UEmulateSerialPort::addERRORResponseAtEnd(string &rspStr)
{
	rspStr += 13;
	rspStr += 10;
	rspStr += "ERROR";
	rspStr += 13;
	rspStr += 10;
}

void UEmulateSerialPort::sendRespOrEvents(string str)
{
	/*handle for call back*/
	//SendResponseOrUnsolicitedEvents(str);
}

void UEmulateSerialPort::create_AT_Command_Dumps(string &str)
{
	string cmdStr(str);
	string rspStr = "";
	string genStr = "";
	int iPos = 0;
	
	//cout << "Emulator received this AT command:" << cmdStr << endl;



	if(strstr(cmdStr.c_str(),"AT+CMEE"))
	{
		if(strstr(cmdStr.c_str(),"AT+CMEE?"))
		{
			rspStr = "";
			addCRLFAtEnd(rspStr);
			rspStr += "+CMEE: 2"; 
			addCRLFAtEnd(rspStr);
			addOKResponseAtEnd(rspStr);

			callbackfnWithId_ptr(rspStr, 1, getNM());
		}
		else if(strstr(cmdStr.c_str(),"AT+CMEE=?"))
		{
			rspStr = "";
			addCRLFAtEnd(rspStr);
			rspStr += "+CMEE: (0,1,2,3)";
			addCRLFAtEnd(rspStr);
			addOKResponseAtEnd(rspStr);

			callbackfnWithId_ptr(rspStr, 1, getNM());
		}
		else if(strstr(cmdStr.c_str(),"AT+CMEE="))
		{
			rspStr = "";
			/*addCRLFAtEnd(rspStr);
			rspStr += "+CFUN:";
			addCRLFAtEnd(rspStr);*/
			addOKResponseAtEnd(rspStr);

			callbackfnWithId_ptr(rspStr, 1, getNM());
		}
	}

	else if(strstr(cmdStr.c_str(),"AT*MIMSRSP"))
	{
		////cout << "Emulator received this AT command:" << cmdStr << endl;

		if(strstr(cmdStr.c_str(),"AT*MIMSRSP=1"))
		{
			////cout << "Select below no. for emulating the scenario" << endl;
			////cout << "1\tIMS REG SUCCESS\r\n2\tIMS REG FAILURE\r\n3\tREQ POR\r\n4\tREQ IMS LTD MODE\r\n5\tREQ ATTACH" << endl;
			int option = 0;
			cin >> option;
			if(option == 1)
			{
				string eventStr = "\r\n+MIMSEV: IMS REG SUCCESS\r\n";
				callbackfnWithId_ptr(eventStr, 2, getNM());
				eventStr.clear();
				rspStr = "";
			}
			else if(option == 2)
			{
				string eventStr = "\r\n+MIMSEV: IMS REG FAILURE\r\n";
				callbackfnWithId_ptr(eventStr, 2, getNM());
				eventStr.clear();
				rspStr = "";
			}
			else if(option == 3)
			{
				string eventStr = "\r\n+MIMSEV: REQ POR\r\n";
				callbackfnWithId_ptr(eventStr, 2, getNM());
				eventStr.clear();
				rspStr = "";
			}
			else if(option == 4)
			{
				string eventStr = "\r\n+MIMSEV: REQ IMS LTD MODE\r\n";
				callbackfnWithId_ptr(eventStr, 2, getNM());
				eventStr.clear();
				rspStr = "";
			}
			else if(option == 5)
			{
				string eventStr = "\r\n+MIMSEV: REQ ATTACH\r\n";
				callbackfnWithId_ptr(eventStr, 2, getNM());
				eventStr.clear();
				rspStr = "";
			}
		}
	}
	else if(strstr(cmdStr.c_str(),"AT*MIMSCFG"))
	{
	//	//cout << "Emulator received this AT command:" << cmdStr << endl;
		if(strstr(cmdStr.c_str(),"AT*MIMSCFG=6,0"))
		{
			string eventStr = "\r\n+MIMSEV: IMS DEREG\r\n";
			callbackfnWithId_ptr(eventStr, 2, getNM());
			eventStr.clear();
			rspStr = "";
		}
	}
	else if(strstr(cmdStr.c_str(),"AT+CFUN"))
	{
		////cout << "Emulator received this AT command:" << cmdStr << endl;
		if(strstr(cmdStr.c_str(),"AT+CFUN?"))
		{
			if(CEREG_STATUS == 0)
			{
				rspStr = "";
				addCRLFAtEnd(rspStr);
				rspStr += "+CFUN: 0"; 			
				addCRLFAtEnd(rspStr);
				addOKResponseAtEnd(rspStr);
			}
			else
			{
				rspStr = "";
				addCRLFAtEnd(rspStr);
				rspStr += "+CFUN: 1"; 			
				addCRLFAtEnd(rspStr);
				addOKResponseAtEnd(rspStr);
			}

			callbackfnWithId_ptr(rspStr, 1, getNM());
		}
		else if(strstr(cmdStr.c_str(),"AT+CFUN=?"))
		{
			rspStr = "";
			addCRLFAtEnd(rspStr);
			rspStr += "+CFUN: 1,1";
			addCRLFAtEnd(rspStr);
			addOKResponseAtEnd(rspStr);
			callbackfnWithId_ptr(rspStr, 1, getNM());
		}
		else if(strstr(cmdStr.c_str(),"AT+CFUN="))
		{
			if(strstr(cmdStr.c_str(),"AT+CFUN=4"))
			{
				string eventStr = "\r\n*MRDY: 4\r\n";
				CEREG_STATUS=0;
				callbackfnWithId_ptr(eventStr, 2, getNM());
			}
			else if(strstr(cmdStr.c_str(),"AT+CFUN=1"))
			{
				string eventStr = "\r\n+CEREG: 1,1\r\n";
				CEREG_STATUS=1;
				callbackfnWithId_ptr(eventStr, 2, getNM());
				eventStr.clear();
				eventStr = "\r\nME PDN ACT\r\n";
				callbackfnWithId_ptr(eventStr, 2, getNM());
			//	//cout << "EMULATOR---->ATTACHED" << endl;
				CEREG_STATUS=1;
			}
			else if(strstr(cmdStr.c_str(), "AT+CFUN=0"))
			{
				string eventStr = "\r\nME PDN DEACT\r\n";
				CEREG_STATUS=0;
				callbackfnWithId_ptr(eventStr, 2, getNM());
			}
		}
	}
	else if(strstr(cmdStr.c_str(),"AT+COPS"))
	{
		if(strstr(cmdStr.c_str(),"AT+COPS?"))
		{
			rspStr = "";
			addCRLFAtEnd(rspStr);
			rspStr += "+COPS: 0,\"0\",\"3\?,2";
			addCRLFAtEnd(rspStr);
			addOKResponseAtEnd(rspStr);

			callbackfnWithId_ptr(rspStr, 1, getNM());
		}
		else if(strstr(cmdStr.c_str(),"AT+COPS=?"))
		{
			rspStr = "";
			addCRLFAtEnd(rspStr);
			rspStr += "+COPS: (2,\"3\?,\"3\?,\"24004\?,2),(1,\"3\?,\"3\?,\"24008\?,0),(3,\"Sweden 3G\",\"Sweden3G\",2)";
			addCRLFAtEnd(rspStr);
			addOKResponseAtEnd(rspStr);

			callbackfnWithId_ptr(rspStr, 1, getNM());
		}
		else if(strstr(cmdStr.c_str(),"AT+COPS="))
		{
			if(strstr(cmdStr.c_str(),"AT+COPS=1"))
			{
				////cout << "EMULATOR's PLMN selected mode---->MANUAL" << endl;
			}
			else if(strstr(cmdStr.c_str(),"AT+COPS=0"))
			{
				////cout << "EMULATOR's PLMN selected mode---->AUTO" << endl;
			}
			rspStr = "";
			/*addCRLFAtEnd(rspStr);
			rspStr += "+COPS:";
			addCRLFAtEnd(rspStr);*/
			addOKResponseAtEnd(rspStr);
			callbackfnWithId_ptr(rspStr, 1, getNM());
		}
	}
	else if(strstr(cmdStr.c_str(),"AT+CGEREP"))
	{
		////cout << "Emulator received this AT command:" << cmdStr << endl;
		if(strstr(cmdStr.c_str(),"AT+CGEREP?"))
		{
			rspStr = "";
			addCRLFAtEnd(rspStr);
			rspStr += "+CGEREP: 1";
			addCRLFAtEnd(rspStr);
			addOKResponseAtEnd(rspStr);

			callbackfnWithId_ptr(rspStr, 1, getNM());
		}
		else if(strstr(cmdStr.c_str(),"AT+CGEREP=?"))
		{
			rspStr = "";
			addCRLFAtEnd(rspStr);
			rspStr += "+CGEREP: (0,1)";
			addCRLFAtEnd(rspStr);
			addOKResponseAtEnd(rspStr);

			callbackfnWithId_ptr(rspStr, 1, getNM());
		}
		else if(strstr(cmdStr.c_str(),"AT+CGEREP="))
		{
			rspStr = "";
			addOKResponseAtEnd(rspStr);
			callbackfnWithId_ptr(rspStr, 1, getNM());
		}
	}
	else if(strstr(cmdStr.c_str(),"AT+CGDCONT"))/*net config set*/
	{
		////cout << "Emulator received this AT command:" << cmdStr << endl;
		if(strstr(cmdStr.c_str(),"AT+CGDCONT?"))
		{
			rspStr = "";
			addCRLFAtEnd(rspStr);
			rspStr += "+CGDCONT=2, \"IP\", \"abc.com\",,0,0";
			addCRLFAtEnd(rspStr);
			addOKResponseAtEnd(rspStr);

			callbackfnWithId_ptr(rspStr, 1, getNM());
		}
		else if(strstr(cmdStr.c_str(),"AT+CGDCONT=?"))
		{
			rspStr = "";
			addCRLFAtEnd(rspStr);
			rspStr += "+CGDCONT=(1-32),\"IP\",,,(0-1),(0-1)";
			addCRLFAtEnd(rspStr);
			addOKResponseAtEnd(rspStr);

			callbackfnWithId_ptr(rspStr, 1, getNM());
		}
		else if(strstr(cmdStr.c_str(),"AT+CGDCONT="))
		{
			rspStr = "";
			addOKResponseAtEnd(rspStr);
			callbackfnWithId_ptr(rspStr, 1, getNM());
		}
	}
	else if(strstr(cmdStr.c_str(),"AT+CGDSCONT"))
	{
		if(strstr(cmdStr.c_str(),"AT+CGDSCONT?"))
		{
			rspStr = "";
			addCRLFAtEnd(rspStr);
			rspStr += "+CGDSCONT=2,1234";
			addCRLFAtEnd(rspStr);
			addOKResponseAtEnd(rspStr);

			callbackfnWithId_ptr(rspStr, 1, getNM());
		}
		else if(strstr(cmdStr.c_str(),"AT+CGDSCONT=?"))
		{
			rspStr = "";
			addCRLFAtEnd(rspStr);
			rspStr += "+CGDSCONT=(1-32),1234";
			addCRLFAtEnd(rspStr);
			addOKResponseAtEnd(rspStr);

			callbackfnWithId_ptr(rspStr, 1, getNM());
		}
		else if(strstr(cmdStr.c_str(),"AT+CGDSCONT="))
		{
			rspStr = "";
			/*addCRLFAtEnd(rspStr);
			rspStr += "+CGDSCONT:";
			addCRLFAtEnd(rspStr);*/
			addOKResponseAtEnd(rspStr);

			callbackfnWithId_ptr(rspStr, 1, getNM());
		}
	}
	else if(strstr(cmdStr.c_str(),"AT+CGACT"))
	{
		////cout << "Emulator received this AT command:" << cmdStr << endl;
		if(strstr(cmdStr.c_str(),"AT+CGACT?"))
		{
			rspStr = "";
			addCRLFAtEnd(rspStr);
			rspStr += "+CGACT: 1, 1";
			addCRLFAtEnd(rspStr);
			addOKResponseAtEnd(rspStr);

			callbackfnWithId_ptr(rspStr, 1, getNM());
		}
		else if(strstr(cmdStr.c_str(),"AT+CGACT=?"))
		{
			rspStr = "";
			addCRLFAtEnd(rspStr);
			rspStr += "+CGACT: (0-1),(0-1)";
			addCRLFAtEnd(rspStr);
			addOKResponseAtEnd(rspStr);

			callbackfnWithId_ptr(rspStr, 1, getNM());
		}
		else if(strstr(cmdStr.c_str(),"AT+CGACT="))
		{
			string eventStr = "\r\n+CEREG: 1,1\r\n";
			callbackfnWithId_ptr(eventStr, 2, getNM());
			eventStr.clear();
			eventStr = "\r\nME PDN ACT\r\n";
			callbackfnWithId_ptr(eventStr, 2, getNM());
			////cout << "EMULATOR---->ATTACHED" << endl;
		}
	}
	else if(strstr(cmdStr.c_str(),"AT+CGATT"))
	{
		////cout << "Emulator received this AT command:" << cmdStr << endl;
		if(strstr(cmdStr.c_str(),"AT+CGATT?"))
		{
			rspStr = "";
			addCRLFAtEnd(rspStr);
			rspStr += "+CGATT: 1";
			addCRLFAtEnd(rspStr);
			addOKResponseAtEnd(rspStr);

			callbackfnWithId_ptr(rspStr, 1, getNM());
		}
		else if(strstr(cmdStr.c_str(),"AT+CGATT=?"))
		{
			rspStr = "";
			addCRLFAtEnd(rspStr);
			rspStr += "+CGATT: (0-1)";
			addCRLFAtEnd(rspStr);
			addOKResponseAtEnd(rspStr);

			callbackfnWithId_ptr(rspStr, 1, getNM());
		}
		else if(strstr(cmdStr.c_str(),"AT+CGATT="))
		{
			if(strstr(cmdStr.c_str(),"AT+CGATT=1"))
			{
				emulState = EMULATOR_ATTACHED;
				CEREG_STATUS=1;
				rspStr = "";
				addOKResponseAtEnd(rspStr);
				callbackfnWithId_ptr(rspStr, 1, getNM());
				
				

				//regStatus
			OsaSleep(1000);

				string eventStr = "\r\n+CEREG: 1,1\r\n";
				callbackfnWithId_ptr(eventStr, 2, getNM());

				
				OsaSleep(1000);
				unSolicitedResp = "";
				unSolicitedResp = mePDNAct;
				callbackfnWithId_ptr(unSolicitedResp, 2, getNM());

				/*::Sleep(3000);
				unSolicitedResp = "";
				unSolicitedResp = meAct;
				sendRespOrEvents(unSolicitedResp);*/
				

				////cout << "EMULATOR---->ATTACHED" << endl;
			}
			else if(strstr(cmdStr.c_str(),"AT+CGATT=0"))
			{
				string eventStr = "\r\nME PDN DEACT\r\n";
				CEREG_STATUS=0;
				callbackfnWithId_ptr(eventStr, 2, getNM());

				////cout << "EMULATOR---->DETTACHED" << endl;
			}
		}
	}
	else if(strstr(cmdStr.c_str(),"AT+CGPADDR"))
	{
		////cout << "Emulator received this AT command:" << cmdStr << endl;
		if(strstr(cmdStr.c_str(),"AT+CGPADDR?"))
		{
			rspStr = "";
			addCRLFAtEnd(rspStr);
			rspStr += "+CGPADDR:2,\"10.21.32.151\"";
			addCRLFAtEnd(rspStr);
			addOKResponseAtEnd(rspStr);

			callbackfnWithId_ptr(rspStr, 1, getNM());
		}
		else if(strstr(cmdStr.c_str(),"AT+CGPADDR=?"))
		{
			rspStr = "";
			addCRLFAtEnd(rspStr);
			rspStr += "+CGPADDR: (0-32)";
			addCRLFAtEnd(rspStr);
			addOKResponseAtEnd(rspStr);

			callbackfnWithId_ptr(rspStr, 1, getNM());
		}
		else if(strstr(cmdStr.c_str(),"AT+CGPADDR="))
		{
			rspStr = "";
			/*addCRLFAtEnd(rspStr);
			rspStr += "+CGPADDR:";
			addCRLFAtEnd(rspStr);*/
			addOKResponseAtEnd(rspStr);

			callbackfnWithId_ptr(rspStr, 1, getNM());
		}
	}
	else if(strstr(cmdStr.c_str(),"AT+CSQ"))
	{
		////cout << "Emulator received this AT command:" << cmdStr << endl;
		if(strstr(cmdStr.c_str(),"AT+CSQ?"))
		{
			rspStr = "";
			addCRLFAtEnd(rspStr);
			rspStr += "+CSQ:";
			genRandom(0,99,rspStr);
			rspStr +=",";
			genRandom(0,99,rspStr);
			addCRLFAtEnd(rspStr);
			callbackfnWithId_ptr(rspStr, 1, getNM());
		}
		else if(strstr(cmdStr.c_str(),"AT+CSQ=?"))
		{
			rspStr = "";
			addCRLFAtEnd(rspStr);
			rspStr += "+CSQ: (0-31,99),(99)";
			addCRLFAtEnd(rspStr);
			addOKResponseAtEnd(rspStr);
			callbackfnWithId_ptr(rspStr, 1, getNM());
		}
		else if(strstr(cmdStr.c_str(),"AT+CSQ="))
		{
			rspStr = "";
			/*addCRLFAtEnd(rspStr);
			rspStr += "+CSQ:";
			addCRLFAtEnd(rspStr);*/
			rspStr += "+CSQ:80,30";
			addOKResponseAtEnd(rspStr);
			callbackfnWithId_ptr(rspStr, 1, getNM());
		}
	}
	else if(strstr(cmdStr.c_str(),"AT+CEREG"))
	{
		////cout << "Emulator received this AT command:" << cmdStr << endl;
		if(strstr(cmdStr.c_str(),"AT+CEREG?"))
		{
			rspStr = "";
			addCRLFAtEnd(rspStr);
			if(CEREG_STATUS==0)
			{
				rspStr += "+CEREG:0,0";
			}
			else 
			{
				rspStr += "+CEREG:1,1";
			}
			//rspStr += "+CEREG: 1,1";
			addCRLFAtEnd(rspStr);
			//addOKResponseAtEnd(rspStr);
			callbackfnWithId_ptr(rspStr, 2, getNM());
		}
		else if(strstr(cmdStr.c_str(),"AT+CEREG=?"))
		{
			rspStr = "";
			addCRLFAtEnd(rspStr);
			rspStr += "+CEREG: (0-2),(0-5)";
			addCRLFAtEnd(rspStr);
			//addOKResponseAtEnd(rspStr);
			callbackfnWithId_ptr(rspStr, 2, getNM());
		}
		else if(strstr(cmdStr.c_str(),"AT+CEREG="))
		{
			rspStr = "";
			//rspStr = "\r\n+CEREG:1\r\n";
			//addCRLFAtEnd(rspStr);
			addOKResponseAtEnd(rspStr);
			callbackfnWithId_ptr(rspStr, 1, getNM());
		}
	}
	else if(strstr(cmdStr.c_str(),"AT+CGCONTRDP"))
	{
		////cout << "Emulator received this AT command:" << cmdStr << endl;
		if(strstr(cmdStr.c_str(),"AT+CGCONTRDP?"))
		{
			rspStr = "";
			addCRLFAtEnd(rspStr);
			rspStr += "+CGCONTRDP: 1, 5, \"apn\",\"24.c1.c5.45\"\"aa.aa.aa.aa.aa.aa.aa.aa.64.39.b8.3d.c0.a8.00.02\",\"ff.ff.ff.00\",\"00.00.00.00\",\"00.00.00.00\",\"00.00.00.00\",\"00.00.00.00\",";
			addCRLFAtEnd(rspStr);
			addOKResponseAtEnd(rspStr);
			callbackfnWithId_ptr(rspStr, 1, getNM());
		}
		else if(strstr(cmdStr.c_str(),"AT+CGCONTRDP=?"))
		{
			rspStr = "";
			addCRLFAtEnd(rspStr);
			rspStr += "+CGCONTRDP: 1,1,\"APN\",\"10.21.32.151.255.255.255.0\"";
			addCRLFAtEnd(rspStr);
			addOKResponseAtEnd(rspStr);
			callbackfnWithId_ptr(rspStr, 1, getNM());
		}
		else if(strstr(cmdStr.c_str(),"AT+CGCONTRDP="))
		{
            if(strstr(cmdStr.c_str(),"AT+CGCONTRDP=1"))
		    {
				
                string tmpRspStr = "";
			    addCRLFAtEnd(tmpRspStr);
			    //tmpRspStr += "+CGCONTRDP: 1, 5, \"apn\",\"24.c1.c5.45\"\"aa.aa.aa.aa.aa.aa.aa.aa.64.39.b8.3d.c0.a8.00.02\",\"ff.ff.ff.00\",\"00.00.00.00\",\"00.00.00.00\",\"00.00.00.00\",\"00.00.00.00\",";

				switch(CONTRDP)
				{
					case 1:tmpRspStr += "+CGCONTRDP: 1,5,\"APN1\",\"192.168.6.75.255.255.255.0\",,,";//"+CGCONTRDP: 1,5,\"APN1\"";
							CONTRDP++;
							break;
					case 2:tmpRspStr += "+CGCONTRDP: 1,5,\"APN2\",\"192.168.6.75.255.255.255.0\",,,";
							CONTRDP++;
							break;
					case 3:tmpRspStr += "+CGCONTRDP: 1,5,\"APN3\",\"192.168.6.75.255.255.255.0\",192.168.6.1,,";
							CONTRDP++;
							break;
					case 4:tmpRspStr += "+CGCONTRDP: 1,5,\"APN4\",\"192.168.6.75.255.255.255.0\",\"192.168.6.1\",,";
							CONTRDP++;
							break;
					case 5:	tmpRspStr += "+CGCONTRDP: 1,5,\"APN5\",\"192.168.6.75.255.255.255.0\",192.168.6.1,10.21.32.23,";
							CONTRDP++;
							break;
					case 6:	tmpRspStr += "+CGCONTRDP: 1,5,\"APN6\",\"192.168.6.75.255.255.255.0\",\"192.168.6.1\",\"10.21.32.23\",";
							CONTRDP++;
							break;
					case 7: tmpRspStr += "+CGCONTRDP: 1,5,\"APN7\",\"192.168.6.75.255.255.255.0\",192.168.6.1,10.21.32.23,10.21.32.11,";
							CONTRDP++;
							break;
					case 8: tmpRspStr += "+CGCONTRDP: 1,5,\"APN8\",\"192.168.6.75.255.255.255.0\",192.168.6.1,10.21.32.23,10.21.32.11,15.2.155.3,";
							CONTRDP++;
							break;
					case 9: tmpRspStr += "+CGCONTRDP: 1,5,\"APN9\",\"192.168.6.75.255.255.255.0\",192.168.6.1,10.21.32.23,10.21.32.11,\"15.2.155.3\",";
							CONTRDP++;
							break;
					case 10:tmpRspStr += "+CGCONTRDP: 1,5,\"APN10\",\"192.168.6.75.255.255.255.0\",192.168.6.1,10.21.32.23,10.21.32.11,\"15.2.155.3\",192.111.15.15";
							CONTRDP++;
							break;
					case 11:tmpRspStr += "+CGCONTRDP: 1,5,\"APN11\",\"192.168.6.75.255.255.255.0\",192.168.6.1,10.21.32.23,10.21.32.11,\"15.2.155.3\",\"192.111.15.15\"";
							CONTRDP++;
							break;
					default:tmpRspStr += "+CGCONTRDP: 1,5,\"APNDefault\",\"192.168.6.75.255.255.255.0\",,,";
							CONTRDP =1;
							break;
				}
			    addCRLFAtEnd(tmpRspStr);
				
				//addCRLFAtEnd(tmpRspStr);
			    //tmpRspStr += "+CGCONTRDP: 1, 5, \"apn\",\"24.c1.c5.45\"\"aa.aa.aa.aa.aa.aa.aa.aa.64.39.b8.3d.c0.a8.00.02\",\"ff.ff.ff.00\",\"00.00.00.00\",\"00.00.00.00\",\"00.00.00.00\",\"00.00.00.00\",";
				//tmpRspStr += "+CGCONTRDP: 1, 5, \"?apn_aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\", \"170.170.170.170.170.170.170.170.100.57.184.61.192.168.0.2.255.255.255.255.255.255.255.255.255.255.255.255.255.255.255.0\", , , ,";
			    //addCRLFAtEnd(tmpRspStr);
				//tmpRspStr+="OK";
				addOKResponseAtEnd(tmpRspStr);
				callbackfnWithId_ptr(tmpRspStr, 1, getNM());
            }
			/*addCRLFAtEnd(rspStr);
			rspStr += "+CGCONTRDP:";
			addCRLFAtEnd(rspStr);*/
			addOKResponseAtEnd(rspStr);
			sendRespOrEvents(rspStr);
		}
	}
	else if(strstr(cmdStr.c_str(),"AT%EARFCN"))
	{
		if(strstr(cmdStr.c_str(),"AT%EARFCN?"))
		{
			rspStr = "";
			rspStr += "%EARFCN:";
			genRandom(0,65535,rspStr);
			addOKResponseAtEnd(rspStr);
			callbackfnWithId_ptr(rspStr, 1, getNM());
		}
		else if(strstr(cmdStr.c_str(),"AT%EARFCN"))
		{
			rspStr = "";
			addOKResponseAtEnd(rspStr);
			callbackfnWithId_ptr(rspStr, 1, getNM());
		}
	}
	else if (strstr(cmdStr.c_str(),"AT%MEAS"))
	{
		if (strstr(cmdStr.c_str(),"AT%MEAS=\"0\""))
		{
			string param = "RSRP";
			rspStr += getRandSigParams(param,-140,-44);
			addCRLFAtEnd(rspStr); 
			callbackfnWithId_ptr(rspStr, 1, getNM());
		}
		else if (strstr(cmdStr.c_str(),"AT%MEAS=\"1\""))
		{
			string param = "RSRQ";
			rspStr += getRandSigParams(param,-30,0);
			addCRLFAtEnd(rspStr);
			callbackfnWithId_ptr(rspStr, 1, getNM());

		}
		else if (strstr(cmdStr.c_str(),"AT%MEAS=\"2\""))
		{
			string param = "SINR";
			rspStr += getRandSigParams(param,-20,30);
			addCRLFAtEnd(rspStr);
			callbackfnWithId_ptr(rspStr, 1, getNM());

		}

	}

	else if(strstr(cmdStr.c_str(),"AT+CEMODE"))
	{
		////cout << "Emulator received this AT command:" << cmdStr << endl;
		if(strstr(cmdStr.c_str(),"AT+CEMODE?"))
		{
			rspStr = "";
			addCRLFAtEnd(rspStr);
			rspStr += "+CEMODE: 1";
			addCRLFAtEnd(rspStr);
			addOKResponseAtEnd(rspStr);
			callbackfnWithId_ptr(rspStr, 1, getNM());
		}
		else if(strstr(cmdStr.c_str(),"AT+CEMODE=?"))
		{
			rspStr = "";
			addCRLFAtEnd(rspStr);
			rspStr += "+CEMODE: (0-3)";
			addCRLFAtEnd(rspStr);
			addOKResponseAtEnd(rspStr);
			callbackfnWithId_ptr(rspStr, 1, getNM());
		}
		else if(strstr(cmdStr.c_str(),"AT+CEMODE="))
		{
			rspStr = "";
			/*addCRLFAtEnd(rspStr);
			rspStr += "+CEMODE:";
			addCRLFAtEnd(rspStr);*/
			addOKResponseAtEnd(rspStr);
			callbackfnWithId_ptr(rspStr, 2, getNM());
		}
	}
	else if(strstr(cmdStr.c_str(),"AT*IPIFSTATUS"))
	{
		////cout << "Emulator received this AT command:" << cmdStr << endl;
		if(strstr(cmdStr.c_str(),"AT*IPIFSTATUS?"))
		{
			rspStr = "";
			addCRLFAtEnd(rspStr);
			rspStr += "*IPIFSTATUS: 1,1";
			addCRLFAtEnd(rspStr);
			addOKResponseAtEnd(rspStr);
			callbackfnWithId_ptr(rspStr, 1, getNM());
		}
		else if(strstr(cmdStr.c_str(),"AT*IPIFSTATUS="))
		{
			rspStr = "";
			addCRLFAtEnd(rspStr);
			rspStr += "*IPIFSTATUS:";
			addCRLFAtEnd(rspStr);
			addOKResponseAtEnd(rspStr);
			callbackfnWithId_ptr(rspStr, 1, getNM());
		}
	}
	else if(strstr(cmdStr.c_str(),"AT*APN"))
	{
		////cout << "Emulator received this AT command:" << cmdStr << endl;
		if(strstr(cmdStr.c_str(),"AT*APN?"))
		{
			rspStr = "";
			addCRLFAtEnd(rspStr);
			rspStr += "*APN: 1,1,\"IP\",\"APN\",\"MAC ADDRESS\"";
			addCRLFAtEnd(rspStr);
			addOKResponseAtEnd(rspStr);
			callbackfnWithId_ptr(rspStr, 1, getNM());
		}
		else if(strstr(cmdStr.c_str(),"AT*APN="))
		{
			rspStr = "";
			addCRLFAtEnd(rspStr);
			rspStr += "*APN:";
			addCRLFAtEnd(rspStr);
			addOKResponseAtEnd(rspStr);
			callbackfnWithId_ptr(rspStr, 1, getNM());
		}
	}
	else if(strstr(cmdStr.c_str(),"AT*STAT"))
	{
		////cout << "Emulator received this AT command:" << cmdStr << endl;
		if(strstr(cmdStr.c_str(),"AT*STAT=?"))
		{
			rspStr = "";
			addCRLFAtEnd(rspStr);
			rspStr += "*STAT: (0-2)";
			addCRLFAtEnd(rspStr);
			addOKResponseAtEnd(rspStr);
			callbackfnWithId_ptr(rspStr, 1, getNM());
		}
		else if(strstr(cmdStr.c_str(),"AT*STAT="))
		{
			if(strstr(cmdStr.c_str(),"AT*STAT=0"))
			{
				////cout << "Request for general stat" << endl;
				string tmpRspStr = "";
			    addCRLFAtEnd(tmpRspStr);
			    tmpRspStr += "*STAT: 0, Modem_State:";
				genRandom(0,14,tmpRspStr);
				tmpRspStr+=", DL_Throughput:";
				genRandom(0,300000,tmpRspStr);
				tmpRspStr+=", UL_Throughput:";
				genRandom(0,300000,tmpRspStr);
				tmpRspStr+=", Packets_Rxd:";
				genRandom(0,4294967295,tmpRspStr);
				tmpRspStr+=", Packets_Xtd:";
				genRandom(0,4294967295,tmpRspStr);
				tmpRspStr+=", No_of_Resync:";
				genRandom(0,4294967295,tmpRspStr);
				tmpRspStr+=", No_of_Connects:";
				genRandom(0,4294967295,tmpRspStr);
			    addCRLFAtEnd(tmpRspStr);
				addOKResponseAtEnd(tmpRspStr);
				callbackfnWithId_ptr(tmpRspStr, 1, getNM());
				tmpRspStr.clear();
			}
			else if(strstr(cmdStr.c_str(),"AT*STAT=1"))
			{
				////cout << "Request for MAC stat" << endl;
				string tmpRspStr = "";
			    addCRLFAtEnd(tmpRspStr);
			    tmpRspStr += "*STAT: 1, PLMNId:";
				genRandom(0,20066329,tmpRspStr);
				tmpRspStr+=", FrameNum:";
				genRandom(0,20239,tmpRspStr);
				tmpRspStr+=", NumSRs:";
				genRandom(0,(U_UINT32)4299467295,tmpRspStr);
				tmpRspStr+=", NumPUCCHTxns:";
				genRandom(0,(U_UINT32)4299467295,tmpRspStr);
				tmpRspStr+=", NumPRACH:";
				genRandom(0,(U_UINT32)4299467295,tmpRspStr);
				tmpRspStr+=", CRNTI:";
				genRandom(0,65523,tmpRspStr);
				tmpRspStr+=", NumDRBs:";
				genRandom(1,21,tmpRspStr);
				tmpRspStr+=", DL_NumTBs:";
				genRandom(0,(U_UINT32)4299467295,tmpRspStr);
				tmpRspStr+=", DL_NumErrTBs:";
				genRandom(0,(U_UINT32)4299467295,tmpRspStr);
				tmpRspStr+=", DL_PreHarqErr:";
				genRandom(0,100,tmpRspStr);
				tmpRspStr+=", DL_PostHarqErr:";
				genRandom(0,100,tmpRspStr);
				tmpRspStr+=", UL_NumTBs:";
				genRandom(0,(U_UINT32)4299467295,tmpRspStr);
				tmpRspStr+=", UL_NumErrTBs:";
				genRandom(0,(U_UINT32)4299467295,tmpRspStr);
				tmpRspStr+=", UL_PreHarqErr:";
				genRandom(0,200,tmpRspStr);
				tmpRspStr+=", UL_PostHarqErr:";
				genRandom(0,200,tmpRspStr);
				tmpRspStr+=", RASuccessCount:";
				genRandom(0,(U_UINT32)4299467295,tmpRspStr);
				tmpRspStr+=", RAFailureCount:";
				genRandom(0,(U_UINT32)4299467295,tmpRspStr);
				tmpRspStr+=", HO_Count:";
				genRandom(0,(U_UINT32)4299467295,tmpRspStr);
				tmpRspStr+=", IntegrityFailureCount:";
				genRandom(0,(U_UINT32)4299467295,tmpRspStr);
				tmpRspStr+=", SuccessfulSecurityActCount:";
				genRandom(0,(U_UINT32)4299467295,tmpRspStr);
				tmpRspStr+=", FailedSecurityActCount:";
				genRandom(0,(U_UINT32)4299467295,tmpRspStr);
				tmpRspStr+=", CellReselCount:";
				genRandom(0,(U_UINT32)4299467295,tmpRspStr);
				tmpRspStr+=", OutOfCoverageCount:";
				genRandom(0,(U_UINT32)4299467295,tmpRspStr);
				tmpRspStr+=", MeasReportsCount:";
				genRandom(0,(U_UINT32)4299467295,tmpRspStr);
				tmpRspStr+=", NumMacPdusRx:";
				genRandom(0,(U_UINT32)4299467295,tmpRspStr);
				tmpRspStr+= ", NumMacPdusTx:";
				genRandom(0,(U_UINT32)4299467295,tmpRspStr);
			    addCRLFAtEnd(tmpRspStr);
				addOKResponseAtEnd(tmpRspStr);
				callbackfnWithId_ptr(tmpRspStr, 1, getNM());
				tmpRspStr.clear();
			}
			else if(strstr(cmdStr.c_str(),"AT*STAT=2"))
			{
				string calb = "CalB";
				////cout << "Request for Phy stat" << endl;
				string tmpRspStr = "";
			    addCRLFAtEnd(tmpRspStr);
			    tmpRspStr += "*STAT: 2, PhyIdCell:";
				genRandom(0,503,tmpRspStr);
				tmpRspStr+=", DLEARFCN:";
				genRandom(0,65535,tmpRspStr);
				tmpRspStr+=", DLBw:";
				genRandom(0,5,tmpRspStr);
				tmpRspStr+=", ULEARFCN:";
				genRandom(0,65535,tmpRspStr);
				tmpRspStr+=", ULBw:";
				genRandom(0,5,tmpRspStr);
				tmpRspStr+=", BandType:";
				genRandom(0,1,tmpRspStr);
				tmpRspStr+=", NumPorts:";
				genRandom(1,4,tmpRspStr);
				tmpRspStr+=", RSRP:";
				genRandom(-140,-44,tmpRspStr);
				tmpRspStr+=", RSRQ:";
				genRandom(-19,-3,tmpRspStr);
				tmpRspStr+=", RssiAnt0:";
				genRandom(-120,0,tmpRspStr);
				tmpRspStr+=", RssiAnt1:";
				genRandom(-120,0,tmpRspStr);
				tmpRspStr+=", UlTxPower:";
				genRandom(-60,-33,tmpRspStr);
				tmpRspStr+=", NumDCIs:";
				genRandom(0,42949672,tmpRspStr);
				tmpRspStr+=", TxMode:";
				genRandom(0,7,tmpRspStr);
				tmpRspStr+=", DeviceCinr:";
				genRandom(-6,-42,tmpRspStr);
				//tmpRspStr+=","+calb;
				char buff[10];
				for(int i=1;i<44;i++)
				{
					tmpRspStr+=", "+calb;
					ITOA(i,buff,10);
					tmpRspStr+=buff;
					tmpRspStr+=":";
					genRandom(0,1,tmpRspStr);
					buff[0]='\0';
					strcpy(buff,"");
				}
			    addCRLFAtEnd(tmpRspStr);
				addOKResponseAtEnd(tmpRspStr);
				callbackfnWithId_ptr(tmpRspStr, 1, getNM());
				tmpRspStr.clear();
			}
		}
	}
	else if(strstr(cmdStr.c_str(),"AT*PCO"))
	{
		////cout << "Emulator received this AT command:" << cmdStr << endl;
		if(strstr(cmdStr.c_str(),"AT*PCO?"))
		{
			rspStr = "";
			addCRLFAtEnd(rspStr);
			rspStr += "*PCO: 1,\"PCO data string in 3GPP 27.007 format\"";
			addCRLFAtEnd(rspStr);
			addOKResponseAtEnd(rspStr);
			callbackfnWithId_ptr(rspStr, 1, getNM());
		}
		else if(strstr(cmdStr.c_str(),"AT*PCO="))
		{
			rspStr = "";
			addCRLFAtEnd(rspStr);
			rspStr += "*PCO:";
			addCRLFAtEnd(rspStr);
			addOKResponseAtEnd(rspStr);
			callbackfnWithId_ptr(rspStr, 1, getNM());
		}
	}
	else if(strstr(cmdStr.c_str(),"AT*MCNFG"))
	{
		////cout << "Emulator received this AT command:" << cmdStr << endl;
		if(strstr(cmdStr.c_str(),"AT*MCNFG="))
		{
			rspStr = "";
			addCRLFAtEnd(rspStr);
			rspStr += "*MCNFG:";
			addCRLFAtEnd(rspStr);
			addOKResponseAtEnd(rspStr);
			callbackfnWithId_ptr(rspStr, 1, getNM());
		}
	}

	else if(strstr(cmdStr.c_str(),"AT%COUNT=\"NAS\""))
	{
		////cout << "Emulator received this AT command:" << cmdStr << endl;
		rspStr = "";
		addCRLFAtEnd(rspStr);
		rspStr += "NAS Stats: \nRX msg info:\nattachAccept: 0\n";
		addCRLFAtEnd(rspStr);
		addOKResponseAtEnd(rspStr);
		callbackfnWithId_ptr(rspStr, 1, getNM());
	}
    else if(strstr(cmdStr.c_str(),"AT%COUNT=\"L1A\""))
    {

        rspStr = "";
		
        addCRLFAtEnd(rspStr);
        rspStr += "L1A Stats: \nRX info:\nHARQ Discards:";
		genRandom(1,300,rspStr);
		rspStr += "\nTBs Dropped Backpressure:";
		genRandom(1,300,rspStr);
        rspStr += "\nRx total Bytes:";
		genRandom(1,300,rspStr);
		rspStr +="\nReceived TBs per Redundancy Version: RV0=";
		genRandom(1,300,rspStr);
		rspStr +=",RV1=";
		genRandom(1,300,rspStr);
		rspStr +=",RV2=";
		genRandom(1,200,rspStr);
		rspStr +=",RV3=";
		genRandom(1,300,rspStr);
        rspStr += "\nRxThroughput:";
		genRandom(1,500,rspStr);
		rspStr +="\nRX info per MCS(Tbs,Bytes,Crc):\n";
        addMCS(rspStr,3);
		

		rspStr += "TX info:\nTBs Total Transmissions:";
		genRandom(1,300,rspStr);
		rspStr +="\nTBs New Transmissions:";
		genRandom(1,300,rspStr);
        rspStr += "\nTBs Re-Transmissions:";
		genRandom(1,300,rspStr);
		rspStr += "\nCQI Only Tx:";
		genRandom(1,300,rspStr);
		rspStr += "\nnumber Of Adaptive Re-Tx:";
		genRandom(1,300,rspStr);
		rspStr +="\nnumber Of Non Adaptive Re-Tx:";
		genRandom(1,300,rspStr);
		rspStr +="\nnumber Of total bytes:";
		genRandom(1,300,rspStr);
		rspStr +="\nRedundancy Version distribution: RV0=";
		genRandom(1,300,rspStr);
		rspStr +=",RV1=";
		genRandom(1,300,rspStr);
		rspStr +=",RV2=";
		genRandom(1,300,rspStr);
		rspStr +=",RV3=";
		genRandom(1,300,rspStr);
		rspStr += "\nTxThroughput:";
		genRandom(1,300,rspStr);
		rspStr +="\nTX info per MCS(Bytes,Grants,Acks, Nacks, Discards):\n";
		addMCS(rspStr,5);
		addOKResponseAtEnd(rspStr);
		//rspStr += "\r\nOK\r\n";
		//::Sleep(200);
        callbackfnWithId_ptr(rspStr, 1, getNM());
		rspStr.clear();
    }
	else if(strstr(cmdStr.c_str(),"AT%GETCFG"))
      {
            if(strstr(cmdStr.c_str(),"AT%GETCFG=\"BAND\""))
            {
                  rspStr = "";
                  addCRLFAtEnd(rspStr);
                  rspStr += "Bands:  40";                  
                  addCRLFAtEnd(rspStr);
                  addOKResponseAtEnd(rspStr);
            }
            callbackfnWithId_ptr(rspStr, 1, getNM());
      }
      else if(strstr(cmdStr.c_str(),"AT%EARFCN"))
      {
            if(strstr(cmdStr.c_str(),"AT%EARFCN?"))
            {
                  rspStr = "";
				  addCRLFAtEnd(rspStr);
                  rspStr += "%EARFCN:65535";               
                  addCRLFAtEnd(rspStr);
                  addOKResponseAtEnd(rspStr);
            }
            callbackfnWithId_ptr(rspStr, 1, getNM());
      }
	  else if(strstr(cmdStr.c_str(),"AT%PCONI=?"))
      {
			rspStr = "";
			addCRLFAtEnd(rspStr);
			rspStr += "Duplexing mode:TDD";               
			addCRLFAtEnd(rspStr);

			addCRLFAtEnd(rspStr);
			rspStr += "Transmission mode:Close loop MIMO";               
			addCRLFAtEnd(rspStr);

			addCRLFAtEnd(rspStr);
			rspStr += "Bandwidth:5";               
			addCRLFAtEnd(rspStr);

			addCRLFAtEnd(rspStr);
			rspStr += "DL EARFCN:65535";               
			addCRLFAtEnd(rspStr);

			addCRLFAtEnd(rspStr);
			rspStr += "cell ID:1234";               
			addCRLFAtEnd(rspStr);

			addOKResponseAtEnd(rspStr);
			callbackfnWithId_ptr(rspStr, 1, getNM());
      }
	  else if(strstr(cmdStr.c_str(),"ATI"))
	  {
		  rspStr = "";
		  addCRLFAtEnd(rspStr);
		  rspStr += "Manufacturer: ALTAIR-SEMICONDUCTOR\nModel: ALT3100\nRevision: ALT3100_03_00_00_00_26_TF";               
		  addCRLFAtEnd(rspStr);
		  addOKResponseAtEnd(rspStr);
		  callbackfnWithId_ptr(rspStr, 1, getNM());
	  }
	  else if(strstr(cmdStr.c_str(),"AT%VER"))
	  {
		  rspStr = "";
		  addCRLFAtEnd(rspStr);
		  rspStr += "SB Revision: ALT3100_01_09_00_00_09_SB\nMAC Revision: REL_03_00_00_REV_21493\n";
		  rspStr += "MAC Package Version: ALT3100_03_00_00_00_26_TF\nMAC Build Time: Jul_26_2011_22_49_28\n";
		  rspStr += "PHY Revision: 3.0.21440\nASIP Revision: 13526\nBB Product: 3100\n";
		  rspStr += "BB HW Revision: 20\nRFIC_6200 Revision: 09\n3B:\n3B:";
		  addCRLFAtEnd(rspStr);
		  addOKResponseAtEnd(rspStr);
		  callbackfnWithId_ptr(rspStr, 1, getNM());
	  }

   
	rspStr.clear();
	//resp = rspStr;
}

string UEmulateSerialPort::getResponse()
{
	return resp;
}

EMULATOR_STATE UEmulateSerialPort::getEmulatorState()
{
	return emulState;
}
///*********************************************************************************************************************

void Emulator_For_AT_Commands(string str)
{
	string cmdStrFromDAL = "";
	cmdStrFromDAL = str;
	string rspStrToDAL = "";
	ULTE_NM* pLTE_NM = NULL;

	UEmulateSerialPort *emul = new UEmulateSerialPort(pLTE_NM);
	emul->create_AT_Command_Dumps(cmdStrFromDAL);

	delete emul;
}

void SendResponseOrUnsolicitedEvents(string rspStr)
{
	/*send rspStr to the port here*/
	gDeviceParams.pCallback(rspStr);
}

void openDevice(P_AT_CALLBACK pRspCallback)
{
	gDeviceParams.pCallback = pRspCallback;
}