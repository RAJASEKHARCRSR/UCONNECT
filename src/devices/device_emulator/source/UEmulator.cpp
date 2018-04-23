#include "UEmulator.h"
#include <stdio.h>
#include <stdlib.h>

UEmulator::UEmulator(UNetworkManager* pNM) {

	serialPortEmulator = new UEmulateSerialPort(pNM);
	
	this->Init();

}

UEmulator::~UEmulator() {

	//this->Init();
	
	if(serialPortEmulator!=NULL)
	{
		delete serialPortEmulator;
	}

	
}

U_BOOL UEmulator::EventHandler() {

	while(true) {

		try {
			UEmulatorCommand* emulatorCommand;

			if(GetQ()->MsgQGet((U_UINT8**)&emulatorCommand) != -1)	
			{
				if(GetQ()->MsgQGetDepth() < 32)
				{
					//printf("about to handle emulatorCommand");

					handleEvent(emulatorCommand);
				
				}
				else
				{  
					//printf("message queue depth > 20\n");
				}

				if(emulatorCommand!= NULL){
					delete emulatorCommand;
				}

				
			}
		}
		catch(bad_alloc) {
			//printf("Out of Memory!\n");
		}
		catch(exception) {
			//printf("Unknown Exception received\n");
		}
	}

	return true;

}

void UEmulator::handleEvent(UEmulatorCommand* eCmd) {

	serialPortEmulator->create_AT_Command_Dumps(eCmd->command);

}

UEmulatorCommand::UEmulatorCommand(string command):UEvent(999) 
	{
		this->command=command;
	}

UEmulatorCommand::UEmulatorCommand():UEvent(999) 
	{

	}
