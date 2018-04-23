#ifndef UEMULATOR_H_
#define UEMULATOR_H_

#include "UEmulateSerialPort.h"
#include "uconn_Event.h"

class UEmulatorCommand;

class UEmulator : public UProcess {

UEmulateSerialPort* serialPortEmulator;

public:

	void Reset() { }

	U_BOOL EventHandler();

	~UEmulator();

	UEmulator(UNetworkManager* pNM);

private:
	
	void handleEvent(UEmulatorCommand* eCmd);


};

class UEmulatorCommand : public UEvent
{
public:
	string command;

	UEmulatorCommand();
	UEmulatorCommand(string command) ;
	~UEmulatorCommand() {}
};

#endif //UEMULATOR_H_