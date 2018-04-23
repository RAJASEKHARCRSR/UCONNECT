/*****************************************************************************
**
**  uconn_cliDefs.h
**
**	This file contains the definitions of the structures used in Uconnect CLI
**
**	Author: Vasuki Setlur
**	Date  : 05-JAN-2011
**  (c) United Mobile Apps Pvt Ltd - (2010 - 2015)

    VER		  DATE		AUTHOR		DESCRIPTION
	0.1		29/12/10     VS			Creation
*****************************************************************************/
#ifndef _UCONN_CLI_DEFS_H_
#define _UCONN_CLI_DEFS_H_

#include "uconn_SAPIwrapper.h"



/* the maximum number of params a command can have */
#define UCONN_CLI_MAX_PARAMS_SUPPORTED	3

/* util defines */
/* debug logging levels */
#define UCONN_CLI_CONSOLE_PRINT
#if 0
#define UCONN_CLI_DEBUG1_PRINT
#define UCONN_CLI_DEBUG2_PRINT
#endif

#define UCONN_CLI_CONSOLE	0
#define UCONN_CLI_DEBUG1	1
#define UCONN_CLI_DEBUG2	2
#define UCONN_CLI_LOG		3	/* dummy for now */

/* Return values of CLI functions */
typedef enum {
	UCONN_CLI_FAILURE = 0,
	UCONN_CLI_SUCCESS,
	UCONN_CLI_INVALID_PARAMS,
	UCONN_CLI_TOO_MANY_PARAMS,
	UCONN_CLI_TOO_FEW_PARAMS,
	UCONN_CLI_INVALID_COMMAND,
	UCONN_CLI_INVALID_ARGUMENT
}uconnCliStatus_t;


#define IS_CMD_PARAM_NUM(cmdStr) ((_stricmp("int", (cmdStr))== 0)? 1: 0)
#define IS_CMD_PARAM_AUTO(cmdStr) ((_stricmp("auto", (cmdStr))== 0)? 1: 0)
#define IS_CMD_PARAM_STR(cmdStr) ((_stricmp("str", (cmdStr))== 0)? 1: 0)

/* Versions */
#define UCONN_CLI_MAIN_VERSION	1 /* Release version will have 1 */
#define UCONN_CLI_SUB_VERSION	1 /* Will reflect checkin tags */
#define UCONN_CLI_DEFAULT_PROMPT "$"
#define UCONN_CLI_TRUE		1
#define UCONN_CLI_FALSE		0
#define UCONN_MAX_CMD_LINE_SIZE	100
#define UCONN_CLI_FOREVER while(1)


#define UCONN_CLI_MAX_ARGS	100		/* Maximum number of arguments a command can have */
#define UCONN_MAX_CMD_LINE_SIZE	100 /* maximum size of the command line */

#define UCONN_CLI_DEFAULT_LOG_FILE  "./uconnSessionLog.txt"
#define UCONN_CLI_DEFAULT_SERVER_LOG_FILE	"./uconnLogs.txt"
#define UCONN_CLI_MODE_AUTO		0
#define UCONN_CLI_MODE_MANUAL	1

#ifdef UCONN_CLIENT_USE_SOCKETS
#define UCONN_CLI_DEFAULT_SERVER_IP	"127.0.0.1"
#define UCONN_CLI_DEFAULT_SERVER_PORT 55555
#endif

/* CLI Instance. Whenever the CLI application is started, an
instance is initiated and maintained for the life of the application */
typedef struct uconnCli_s {
	U_UINT32 MainVersion;		/* main version of the CLI */
	U_UINT32 SubVersion;		/* sub version of the CLI */
	U_UINT32 logging;			/* on or off */
	U_SINT8 *logFilepath_p;		/* session log file */
	FILE *logFile_p;			/* pointer to the session log file */

	/* the following are used to control logging at the server
	and file the logs at the client */
	U_SINT8 *serverLogFilepath_p; /* server logs file */
	FILE *serverLogFile_p;		/* pointer to the server log file */

	U_UINT8 *prompt_p;			/* pointer to the prompt string */
	U_SINT32 exitFlag;			/* will be set to 1 if the client must exit */
#ifdef UCONN_CLIENT_USE_SOCKETS
	U_SINT8 *servIpAddr_p;		/* server IP address - should we support IPV6? */
	U_UINT16 servPort;			/* server port - client does not care abt proto, SAPI Wrapper handles it */
	U_SINT32 status;			/* 1 for up and connected */
#endif /*UCONN_CLIENT_USE_SOCKETS*/
} uconnCli_t;


/* the command structure that describes the command. an array of these
structures will be used to list down the commands statically */
typedef struct uconnCliCmd_s {
	U_SINT8 *cmdStr_p;							/* the command string */
	U_SINT8 *help_p;						/* the help string */
	U_SINT32 minParams;							/* minimum number of params possible */
	U_SINT32 maxParams;							/* maximum number of params possible */
	U_SINT8  *paramTypes[UCONN_CLI_MAX_PARAMS_SUPPORTED];	/* the param types, either "int" or "str"  or "auto"
														When Auto Verify Params will not do type check*/
	U_SINT32  cmdId;							/* the command Identifier */
	uconnCliStatus_t (*cmdHandler_p)(U_VOID *cliInstance_p,	/* local command handler */
		U_SINT8 *cmdComponents_p[], U_SINT32 numberOfArgs);

} uconnCliCmd_t;


/* Prototypes */
uconnCliCmd_t *getCmdFromList (U_SINT8 *cmdEntered_p);
U_SINT32 verifyParams (uconnCliCmd_t *cmd_p, U_SINT8 *cmdComponents[], U_SINT32 numberOfArgs);
U_SINT32 uconnCliCmdExec (uconnCliCmd_t *cmd_p, U_SINT8 *cmdComponents[], U_SINT32 numberOfArgs);
U_VOID uconnCliRunCmd (U_SINT8 *cmdComponents[], U_SINT32 numberOfArgs);
U_VOID uconnCliGetAndRunCmds (uconnCli_t *cliInstance_p);
static U_VOID uconnCliShowPrompt(uconnCli_t *cliInstance_p);
U_SINT8 *uconnCliGetCmdLine ();
U_SINT32 uconnCliGetCmdArgs (U_SINT8 *cmdLine_p, U_SINT8 *cmdComponents[]);
static uconnCliStatus_t uconnCliHelp(U_VOID *cliInstance_p, U_SINT8 *cmdComponents_p[], 
									 U_SINT32 numberOfArgs);
static uconnCliStatus_t uconnCliQuit(U_VOID *cliInstance_p, U_SINT8 *cmdComponents_p[], 
									 U_SINT32 numberOfArgs);
static uconnCliStatus_t uconnCliVersion(U_VOID *cliInstance_p, U_SINT8 *cmdComponents_p[],
				 U_SINT32 numberOfArgs);
static uconnCliStatus_t uconnCliServerLog(U_VOID *cliInstance_p, 
										U_SINT8 *cmdComponents_p[],
										U_SINT32 numberOfArgs);
U_STRING convertStringToUpperCase1(const U_STRING& str);

#ifdef UCONN_CLIENT_USE_SOCKETS
static uconnCliStatus_t uconnCliServer(U_VOID *cliInstance_p, U_SINT8 *cmdComponents_p[],
				 U_SINT32 numberOfArgs);
#endif
static uconnCliStatus_t uconnCliPrompt(U_VOID *cliInstance_p, U_SINT8 *cmdComponents_p[],
				 U_SINT32 numberOfArgs);
static U_VOID uconnCliCleanup (uconnCli_t *cliInstance_p);
static U_VOID uconnCliDisplay(int logLevel, uconnCli_t *cliInstance_p, const char *format_p, ...);
static U_VOID uconnCliDisplayBanner (uconnCli_t *cliInstance_p);
static uconnCliStatus_t uconnCliLog(U_VOID *cliInstance_p, U_SINT8 *cmdComponents_p[],
				 U_SINT32 numberOfArgs);
U_VOID uconnCliEvtHandler (UEvent* evt, U_SINT8 *data_p, U_VOID *arg_p);

#endif /* _UCONN_CLI_DEFS_H_ */