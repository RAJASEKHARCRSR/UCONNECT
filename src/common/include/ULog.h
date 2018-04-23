/*****************************************************************************
**
** ULog.h
** This header file describes the Logger interfaces
**
** Author: Sriram V Iyer
** Date  : 31-DEC-10
** (c) United Mobile Apps Pvt Ltd (2010 - 2015)

    VER		  DATE		AUTHOR		DESCRIPTION
	0.1		31/12/10     SVI		Creation
****************************************************************************/

#include <string>
#include "UTypes.h"
#include "time.h"
#include "uconn_Event.h"
using namespace std;

#define ULOG_DEBUG_LEVEL 0
#define ULOG_INFO_LEVEL  1
#define ULOG_ERROR_LEVEL 2

#ifndef ULOG_H
#define ULOG_H



//#define LOG getLogger()->log
#define LOG   getLogger()->log
class ULoggerMsg;

class ULog : public UProcess  {

 private:
      int logLevel;
	  int EnableFileLog;
 
 public:
	 static const int DEBUG_LEVEL = ULOG_DEBUG_LEVEL;
     static const int INFO_LEVEL = ULOG_INFO_LEVEL;
     static const int ERROR_LEVEL = ULOG_ERROR_LEVEL;
	
	 FILE* logfile_p;
	 U_SINT8 logfilename_p[200];

	//void log(int level, string message);
        void log(int level, U_SINT8* format, ...);

	void debugLog(string message);

	void infoLog(string message);

	void errorLog(string message);

	void (*logCallBack)(string message);
	
	ULog():logfile_p(NULL){
		strcpy(logfilename_p,"");
		logfile_p=fopen(logfilename_p,"w");
	}

	~ULog();

	void setLoglevel(int level)
	{
		logLevel = level;
	}
	int getLogLevel()
	{
		return logLevel;
	}

	void setFileLogEnable(int setting)
	{
		EnableFileLog = setting;
	}

	int getLogFileEnable() 
	{
		return EnableFileLog;
	}

	 
U_BOOL EventHandler();
//void (*logCallBack)(string message);
};



ULog* getLogger();

class ULoggerMsg : public UEvent
{
public:
	
	int level;
	string message;

	ULoggerMsg(int level,string message):UEvent(UCONN_EVT_LOG)
	{
		this->level = level;
		this->message = message;
	}
	~ULoggerMsg() {}

};




#endif
