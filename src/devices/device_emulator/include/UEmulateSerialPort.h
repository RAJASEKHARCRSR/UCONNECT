

#ifndef UEMULATESERIALPORT_H
#define UEMULATESERIALPORT_H
#include "UTypes.h"


#include "uconn_AT_DAL.h"

#include <string>
#include <sstream>
using namespace std;

typedef enum TEMPLATE_EMULATOR_STATE
{
	EMULATOR_DETTACHED=0,
	EMULATOR_ATTACHED,
	EMULATOR__UNKNOWN,
}EMULATOR_STATE;

typedef enum TEMPLATE_EMULATOR_MODE
{
	EMULATOR_MODE_AUTO=0,
	EMULATOR_MODE_MANUAL,
	EMULATOR__MODE_UNKNOWN,
}EMULATOR_MODE;

typedef struct U_ST_CALLBACK_INFO
{
	P_AT_CALLBACK	pCallback;
}ST_CALLBACK_INFO;


class UEmulateSerialPort
{
private:
	string mePDNAct;
	string meAct;
	string netDetachStr;
	string regStatus;
	string unSolicitedResp;
	int CEREG_STATUS;
	string resp;
	EMULATOR_STATE emulState;
	EMULATOR_MODE  emulMode;
	ULTE_NM* m_pLTE_NM;
	UNetworkManager* m_pNM;

	void addCRLFAtEnd(string &rspStr);
	void addOKResponseAtEnd(string &rspStr);
	void addERRORResponseAtEnd(string &rspStr);
    void addMCS(string& rspStr,int count);
	string getRandSigParams(string param,int from,int to);

	void genRandom(U_UINT32  from ,U_UINT32  to, string& rspStr);
	//void genRandom(U_SINT32  from ,U_SINT32  to, string& rspStr);


public:
	UEmulateSerialPort(UNetworkManager* pNM);
	~UEmulateSerialPort();
	void create_AT_Command_Dumps(string &str);
	void sendRespOrEvents(string str);
	string getResponse();
	EMULATOR_STATE getEmulatorState();
	UNetworkManager* getNM()
	{
		return m_pNM;
	}

};


void Emulator_For_AT_Commands(string str);
void SendResponseOrUnsolicitedEvents(string rspStr);
void openDevice(P_AT_CALLBACK pRspCallback);
#endif
