/*****************************************************************************
**  ULog.cpp
**  This file describes the methods for the Logger Utilities
**
** Author: Sriram V Iyer
** Date  : 30-DEC-2010
** (c) United Mobile Apps Pvt Ltd (2010 - 2015)

    VER		  DATE		AUTHOR		DESCRIPTION
	0.1		30/12/10     SVI		Creation
****************************************************************************/

#include "ULog.h"
#include <iostream>
#include <stdarg.h>

#ifdef __linux__
#include <cstdio>
#endif

#ifdef QTPROJECT
#include <cstdio>
#endif
using namespace std;


ULog* gLogger;

ULog* getLogger(){
		 if(gLogger == NULL){
			 gLogger = new ULog();
			 gLogger->Init();
		 }
		return gLogger;
	 }


/***********************************************************************
*	ULog::log -
*	Logger function 
*
*	Parameters
*	level		- Log level (Debug, Info, Error)
*	message		- Message to be displayed
***********************************************************************/

 //void ULog::log(int level, string message) 
void ULog::log(int level, U_SINT8* format, ...) 
{
#if 0
	if(level == ULOG_ERROR_LEVEL)
	{
		va_list log1List;
		va_start(log1List, format);
		U_SINT8 buffer1[200];
		int len = vsnprintf(buffer1, 200, format, log1List);
		va_end(log1List);
	//	string message(buffer1);
		printf("%s",buffer1);

	}
#endif
	//return;
	va_list logList;
	va_start(logList, format);
	U_SINT8 buffer[MAX_LOG_LENGTH_AT_CLIENT];
    vsnprintf(buffer, MAX_LOG_LENGTH_AT_CLIENT-30, format, logList);
	va_end(logList);
	string message(buffer);

	/* Update the Time to the log */
	time_t now;
	time(&now);
	message.append(ctime(&now));
	message.append("\n");

	ULoggerMsg* uLoggerMsg = new ULoggerMsg(level,message);

	
	this->MsgPost((U_PUINT8)uLoggerMsg);
	

	

}
ULog::~ULog()
	{
		if(logfile_p!=NULL)
		{	
			fclose(logfile_p);
		}
	}

/***********************************************************************
*	ULog::debugLog -
*	Debug Logger function 
*
*	Parameters
*	message		- Message to be displayed
***********************************************************************/

	 void ULog::debugLog(string message) {
		 string temp_str = "DEBUG: " + message;
		// getDebugLog().append("\n");
         //getDebugLog().append("DEBUG: " + message);
		if(logLevel <= DEBUG_LEVEL)
		{
			////cout << temp_str << endl;
			if(EnableFileLog)
			{
				if(logCallBack != NULL){
				//logCallBack(temp_str);		
				}
				/* If File logging enabled, write the message to the file */
				if(logfile_p){
					fprintf(logfile_p,"%s", temp_str.c_str());
				}
				
			}	
		}
        // getDebugLog().append("\n");
   }

/***********************************************************************
*	ULog::infoLog -
*	Information Logger function 
*
*	Parameters
*	message		- Message to be displayed
***********************************************************************/

     void ULog::infoLog(string message) {
		 string temp_str = "INFO: " + message;
        /*getInfoLog().append("\n");
        getInfoLog().append("INFO:  " + message);
        getInfoLog().append("\n");*/
		if(logLevel <= INFO_LEVEL)
		{
			////cout << temp_str << endl;
			if(EnableFileLog)
			{
				if(logCallBack != NULL){
					//logCallBack(temp_str);	
				}
				/* If File logging enabled, write the message to the file */
				if(logfile_p)
				{
					fprintf(logfile_p, "%s",temp_str.c_str());
					if(ftell(logfile_p) > 1048576)
					{
						/*fclose(logfile_p);
						logfile_p = NULL;*/
						fseek(logfile_p, 0, SEEK_SET);
					}
				}
			}	
		}
   }

/***********************************************************************
*	ULog::errorLog -
*	Error Logger function 
*
*	Parameters
*	message		- Message to be displayed
***********************************************************************/

    void ULog::errorLog(string message) {
		 string temp_str = "ERROR: " + message;
        /*getErrorLog().append("\n");
        getErrorLog().append("ERROR: " + message);
        getErrorLog().append("\n"); */ 
		if(logLevel <= ERROR_LEVEL)
		{
			////cout << temp_str << endl;
			if(EnableFileLog)
			{
				if(logCallBack != NULL){
					//logCallBack(temp_str);
				}
					/* If File logging enabled, write the message to the file */
					if(logfile_p){
						fprintf(logfile_p, "%s",temp_str.c_str());
					}
				
			}	
		}
   }

	U_BOOL ULog::EventHandler() {

	while(true) {

        //try {
			//printf("event handler try for Emulator\n");
			ULoggerMsg* loggerMsg;
	
			if(GetQ()->MsgQGet((U_UINT8**)&loggerMsg) != -1)	
			{
				logfile_p=fopen(logfilename_p, "a");
				if(GetQ()->MsgQGetDepth() < 32)
				{

			    switch (loggerMsg->level) 
					{
					case INFO_LEVEL: {
						infoLog(loggerMsg->message);
						break;
						}

					case DEBUG_LEVEL: {
					   debugLog(loggerMsg->message);
						break;
						}
				   case ERROR_LEVEL: {
						errorLog(loggerMsg->message);
						break;
						}
				    }				
				}
				if(logfile_p!=NULL)
				{
					fclose(logfile_p);
				}
				else
				{  
					//printf("message queue depth > 20\n");
				}

				if(loggerMsg!= NULL){
					{
						delete loggerMsg;
						loggerMsg = NULL;
					}
				}

				
			}
    //	}
//		catch(bad_alloc& ba) {
			//printf("Out of Memory!\n");
//		}
    //	catch(exception& e) {
			//printf("Unknown Exception received\n");
    //	}
	}

	return true;

}

	
