/*****************************************************************************
**
**  uconn_cliMain.cpp
**
**	This file contains the implementation of the CLI command interpreter
**	for Uconnect. 
**	Adding a new command:
**	- Register the command handler along with the command string and help
**	strings using uconnCliCmdAdd in uconnCliInit()
**	- Write the command handler
**	- Parameters will be passed to the handlers as null terminated strings**
**
**	Author: Vasuki Setlur
**	Date  : 05-JAN-2011
**  (c) United Mobile Apps Pvt Ltd - (2010 - 2015)

    VER		  DATE		AUTHOR		DESCRIPTION
	0.1		29/12/2010    VS		Creation
	0.2		03/01/2011	  VS		Added config and log support for cmd
									line params
*****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <algorithm>
#include <cctype>
#include <string.h>
#include <stdarg.h>
#include "uconn_cliDefs.h"
#include "uconn_SAPIwrapper.h"
#include <uconn_OSAL.h>


/* following is the global array of commands. any new command needs to be added to
the array. only commands that are handled locally at the client and not sent to the
server will have the command handler as the last element in the array. all server 
run commands are passed over sapi wrapper layer*/
uconnCliCmd_t gCmds[] =
{
	{"help", "help - List commands and usage summary", 0, 1, {"str"}, 0, uconnCliHelp},
#ifdef UCONN_CLIENT_USE_SOCKETS
	{"server","server - Display Server details\n"
		"\t\tserver connect - Connect to server\n\t\tserver disconnect - disconnect from server\n"
		"\t\tserver ip - Set server IP\n\t\tserver port - Set server port",
		0,2,{"str", "auto"},0, uconnCliServer},
#endif /*UCONN_CLIENT_USE_SOCKETS*/
	{"netattach",	 "netattach <plmnIndex> - Connect to a network",1,1,{"int"},UCONN_CMD_LTE_NET_ATTACH},
	{"netdetach", "netdetach - Disonnect from network",0,0,{},UCONN_CMD_LTE_NET_DETACH},
	{"prompt",	"prompt <prompt string> - Change prompt string",1,1,{"str"},0,uconnCliPrompt},
	{"netsearch", "netsearch <plmntimer>- Issue a search command to the server\n"
	"\t\tnetsearch status - Display search results", 1, 1, {"auto"}, UCONN_CMD_LTE_NET_SEARCH,NULL},
    {"set",	 "set <Param> <value> - Set Configuration Parameters\n\t\tParam can be integer or string\n\n\t\tSTRINGS\t\t\tINTEGER\n\t\tenable_4g\t\t 32785 \n\t\tconfigmode\t\t 32786 \n\t\tattachrat\t\t 32787 \n\t\tdisablerate\t\t 32788 \n\t\tlogginglevel \t\t 32789 \n\t\tenablelog \t\t 32790 \n\t\tatmode \t\t 32791 \n\n\t For example:To enable 4G we can either enter \n\t\t\"set enable_4g 1 \" or \"set 32785 1 \" \n",
	2,2,{"auto", "int"},UCONN_SAPI_WRAP_SETCFGPARAM},
	{"get",	 "get <Param> - Get Stats for the Enum\n\t\tParam can be a integer or a string\n\n\t\tSTRINGS\t\t\tINTEGER\n\t\tdatarate\t\t 0 \n\t\tul_datarate\t\t 1 \n\t\tdl_datarate\t\t 2 \n\t\tcurrentstate\t\t 3 \n\t\trssi_val\t\t 4 \n\t\tcinr_val\t\t\t 5 \n\t\troaminginfo\t\t 6 \n\t\tactivationstatus\t 7 \n\t\tmodemstate\t\t 8 \n\t\tsystem_noti\t\t 9 \n\n\tFor example:To get the datarate we can either enter \n\t\t\"get datarate \" or \"get 0 \" \n",1,1,
	{"auto"},UCONN_SAPI_WRAP_GETSTATS},
	{"sessionsave","sessionsave file - Display Log file\n"
		"\t\tsessionsave file <log file>\n\t\tsessionsave <on/off> - Enable/Disable logging",
		1,2,{"str", "str"},0, uconnCliLog},
	{"log", "log - Display Log details\n"
	"\t\tlog file <log file> - Set Log file\n"
	"\t\tlog level <Debug | Info | Error> - Set Logs level\n"
	"\t\tlog <on/off> - Enable/Disable Logs\n",
	0, 2, {"str", "str"},0, uconnCliServerLog},

	{"modemstat_Altr",	 "modemstat_Altr - Modem Statistics",0,0,{"auto"},UCONN_CMD_LTE_MOD_STATISTIC_REQ},
	{"modemstat_Brcm",	 "modemstat_Brcm - Modem Statistics",0,0,{"auto"},UCONN_CMD_LTE_BRCM_STATISTIC_REQ},
	{"quit", "quit - Exit CLI", 0, 0, {}, 0, uconnCliQuit},
    {"getstring",  "getstring <Param>-Get string Stats for the Enum \n\t\tParam can be a integer or a string\n\n\t\tSTRINGS\t\t\tINTEGER\n\t\tapn\t\t\t 10 \n\t\tipv4addr\t\t 11 \n\t\tipv6addr\t\t 12 \n\t\tipv4submask\t\t 13 \n\t\tipv6submask\t\t 14  \n\t\tipv4gwaddr\t\t 15 \n\t\tipv6gwaddr\t\t 16 \n\t\tipv4dns \t\t 17 \n\n\tFor example:To get the apn name we can either enter \n\t\t\" getstring apn \" or \" get 10 \" \n",1,1,{"auto"},UCONN_SAPI_WRAP_GET_STRPARAM_REQ},
	{"getfloat",  "getfloat <Param>-Get string Stats for the Enum \n\t\tParam can be a integer or a string\n\n\t\tSTRINGS\t\t\tINTEGER\n\t\tRssi_val\t\t\t 4 \n\t\tCinr_val\t\t 5 \n\n\tFor example:To get the apn name we can either enter \n\t\t\" getstring cinr_val \" or \" get 5 \" \n",1,1,{"auto"},UCONN_SAPI_WRAP_GET_FLT_REQ},
	{"getversion",	 "getversion - Version Information of Binaries used",0,0,{"auto"},UCONN_CMD_LTE_VERSION_INFO_REQ},
	{"AT",	 "Any command start with AT will enter in AT Mode",0,1,{"auto"},UCONN_EVT_AT_CMD_REQUEST},
	{"enableNM","enableNM techId",1,1,{"int"},UCONN_CMD_BEARER_ENABLED},
	{"disableNM","disableNM techId",1,1,{"int"},UCONN_CMD_BEARER_DISABLED},
	{"connectBearer","connectBearer techId",1,1,{"int"},UCONN_CMD_BEARER_CONNECT},
	{"disconnectBearer","disconnectBearer techId",1,1,{"int"},UCONN_CMD_BEARER_DISCONNECT},
	{"listVpnClients","listVpnClients - List Vpn Clients. No arguments",0,0,{"auto"},UCONN_CMD_BEARER_VPN_LIST_CLIENTS},
	{"listVpnProfiles","listVpnProfiles ListVpnProfiles. No arguments",0,0,{"auto"},UCONN_CMD_BEARER_VPN_LIST_PROFILES},
	{"startVpn","startVpn - Connect to VPN based on selected profile",0,0,{"auto"},UCONN_CMD_BEARER_VPN_START},
	{"stopVpn","stopVpnDisconnect from VPN",0,0,{"auto"},UCONN_CMD_BEARER_VPN_STOP},
	{"setParam","setParam - setParam techId paramId param",3,3,{"auto"},UCONN_CMD_BEARER_SET_PARAM},


	/* nothing beyond this */
	{NULL, NULL, 0, 0, {}, 1}
};


/***********************************************************************
*	Initialises the cli Instance. In case of Socket, this fuction opens 
*	a socket and connets to the Uconnect Server
*	
*	Parameters
*	cliInstance_p	-	Pointer to the CLI instance structure
***********************************************************************/
static uconnCliStatus_t uconnCliInit (uconnCli_t *cliInstance_p, U_SINT8 *ip_p,
					U_UINT16 port, U_SINT8 *conffile_p, U_SINT8 *logfile_p)
{
	/* Initialise the structure, and register 
	all the major cli commands */


#ifdef UCONN_CLIENT_USE_SOCKETS
	/* Initialize sockets */
	if(ip_p) cliInstance_p->servIpAddr_p = _strdup((const U_SINT8*)ip_p);
	else cliInstance_p->servIpAddr_p = _strdup((const U_SINT8*)UCONN_CLI_DEFAULT_SERVER_IP);
	if(port) cliInstance_p->servPort = port;
	else cliInstance_p->servPort = UCONN_CLI_DEFAULT_SERVER_PORT;
#endif /*UCONN_CLIENT_USE_SOCKETS*/

	/* initialize SAPI wrappers */
	if(uconn_SAPIwrapInit(uconnCliEvtHandler, (U_VOID*)cliInstance_p) != 0) {
		goto err;
        }
#ifdef UCONN_CLIENT_USE_SOCKETS
	/* atttempt to connect to server here */
	if(uconn_SAPIwrapConnectToSever(
	((uconnCli_t*)cliInstance_p)->servIpAddr_p, ((uconnCli_t*)cliInstance_p)->servPort)
															!= UCONN_SAPI_WRAP_SUCCESS)
	{
		uconnCliDisplay(UCONN_CLI_CONSOLE, ((uconnCli_t*)cliInstance_p),
		"Connection to server failed\n"); 
	}
	else
	{
		((uconnCli_t*)cliInstance_p)->status = 1;
		uconnCliDisplay(UCONN_CLI_CONSOLE, ((uconnCli_t*)cliInstance_p),
		"Connection to server Established\n"); 
		
	}
#endif /*UCONN_CLIENT_USE_SOCKETS*/
        #ifdef WIN32
	cliInstance_p->prompt_p = (U_UINT8*)(_strdup(UCONN_CLI_DEFAULT_PROMPT));
        #endif
    #if (defined(__linux__)|| defined(__MACH__))
    cliInstance_p->prompt_p = (U_UINT8*)(strdup(UCONN_CLI_DEFAULT_PROMPT));
        #endif
	cliInstance_p->MainVersion = UCONN_CLI_MAIN_VERSION;
	cliInstance_p->SubVersion = UCONN_CLI_SUB_VERSION;
	cliInstance_p->exitFlag = UCONN_CLI_FALSE;
	cliInstance_p->logging = UCONN_CLI_FALSE;

	/* session log file */
	if(logfile_p) cliInstance_p->logFilepath_p = _strdup((const U_SINT8*)logfile_p);
	else cliInstance_p->logFilepath_p = _strdup(UCONN_CLI_DEFAULT_LOG_FILE);

	/* server logs file */
	cliInstance_p->serverLogFilepath_p = _strdup(UCONN_CLI_DEFAULT_SERVER_LOG_FILE);
    
	return UCONN_CLI_SUCCESS;
err:
	uconnCliCleanup(cliInstance_p);
	return UCONN_CLI_FAILURE;
}


/***********************************************************************
*	Entry point function 
*	
*	Command line arguments
*	(-sip) CSCM Server IP address
*	(-p) CSCM Server listening port
*	(-l) Path to the session log file
***********************************************************************/
U_SINT32 main (U_SINT32 argc, U_SINT8 **argv)
{
	uconnCli_t *cliInstance_p;
	U_SINT32 argIdx;
	U_SINT8 *ip_p = NULL, *conffile_p = NULL, *logfile_p = NULL;
	U_UINT16 port = 0;

	if(!(cliInstance_p = (uconnCli_t*)calloc(1, sizeof(uconnCli_t))))
		return -1;

	if(argc > 1)
	{
		argIdx = 0;
		while(argv[++argIdx])
		{
			if(!strcmp(argv[argIdx], "-sip"))
			{
				uconnCliDisplay(UCONN_CLI_DEBUG1, cliInstance_p, 
					"Conf server IP\n");
				if(!argv[argIdx + 1] || (U_SINT8)(*argv[argIdx + 1]) == '-')
					uconnCliDisplay(UCONN_CLI_DEBUG1, cliInstance_p,
					"IP address not specified\n");
				else
					ip_p = argv[argIdx + 1];
			}
			else if(!strcmp(argv[argIdx], "-sp"))
			{
				uconnCliDisplay(UCONN_CLI_DEBUG1, cliInstance_p, 
					"Conf server Port\n");
				if(!argv[argIdx + 1] || (U_SINT8)(*argv[argIdx + 1]) == '-')
					uconnCliDisplay(UCONN_CLI_DEBUG1, cliInstance_p, 
					"Port not specified\n");
				else
					port = (U_UINT16)atoi((const char*)argv[argIdx + 1]);
			}
			else if(!strcmp(argv[argIdx], "-l"))
			{
				uconnCliDisplay(UCONN_CLI_DEBUG1, cliInstance_p, 
					"Log logfile\n");
				if(!argv[argIdx + 1] || (U_SINT8)(*argv[argIdx + 1]) == '-')
					uconnCliDisplay(UCONN_CLI_DEBUG1, cliInstance_p, 
					"Log file not specified\n");
				else
					logfile_p = argv[argIdx + 1];
			}
			else if(*argv[argIdx] == '-')
			{
					uconnCliDisplay(UCONN_CLI_DEBUG1, cliInstance_p, 
					"Invalid Option\n");
			}
		}
	}
	printf("%s", ip_p);
        printf("%d", port);

	/* initialize and register commands */
	if(uconnCliInit (cliInstance_p, ip_p, port, conffile_p, logfile_p)
		!= UCONN_CLI_SUCCESS)
		return -1;
	
	/* Accept and execute commands */
	uconnCliGetAndRunCmds(cliInstance_p);

	return 0;
}

/***********************************************************************
*	Print based on set debug levels
*	
***********************************************************************/
#define UCONN_CLI_LOG_TO_FILE(cliInstance_p, format_p, argLst)\
do {if(cliInstance_p->logging) if(cliInstance_p->logFile_p)\
	vfprintf(cliInstance_p->logFile_p, format_p, argLst);} while(0)

static U_VOID uconnCliDisplay(U_SINT32 logLevel, uconnCli_t *cliInstance_p,
							  const U_SINT8 *format_p, ...)
{
	va_list argLst;

	va_start(argLst, format_p);
#ifdef UCONN_CLI_CONSOLE_PRINT
	if(logLevel == UCONN_CLI_CONSOLE)
	{
		vprintf(format_p, argLst);
		if(cliInstance_p!=NULL)
		UCONN_CLI_LOG_TO_FILE(cliInstance_p, format_p, argLst);
	}
#endif
#ifdef UCONN_CLI_DEBUG1_PRINT
	/* Print to the log file if file specified - TBD */
	if(logLevel == UCONN_CLI_DEBUG1)
	{
		vprintf(format_p, argLst);
	}
#endif
#ifdef UCONN_CLI_DEBUG2_PRINT
	/* Print to the log file if file specified - TBD */
	if(logLevel == UCONN_CLI_DEBUG2)
	{
		vprintf(format_p, argLst);
	}
#endif

	va_end(argLst);
}


/***********************************************************************
*	Cleanup the CLI instance
*	
***********************************************************************/
static U_VOID uconnCliCleanup (uconnCli_t *cliInstance_p)
{
#ifdef UCONN_CLIENT_USE_SOCKETS
	if(cliInstance_p->servIpAddr_p) free(cliInstance_p->servIpAddr_p);
	uconn_SAPIwrapDisconnectFrmSever();
#endif
	if(cliInstance_p->prompt_p) free(cliInstance_p->prompt_p);
	if(cliInstance_p->logFilepath_p) free(cliInstance_p->logFilepath_p);
	if(cliInstance_p->logFile_p) fclose(cliInstance_p->logFile_p);
	if(cliInstance_p->serverLogFilepath_p) free(cliInstance_p->serverLogFilepath_p);
	if(cliInstance_p->serverLogFile_p) fclose(cliInstance_p->serverLogFile_p);
	free(cliInstance_p);
}

U_STRING  convertStringToUpperCase1(U_STRING &str)
{
    U_STRING upper;     
	std::transform(str.begin(), str.end(),std::back_inserter(upper),(int(&)(int))std::toupper);     
	return upper; 
}

/***********************************************************************
*	matches the command entered with the commands in the list and 
*	returns the pointer to the right command structure
*
*	Parameters
*	cmdEntered_p - pointer to the entered command string
*
*	Returns
*	Pointer to the command structure
***********************************************************************/
uconnCliCmd_t *getCmdFromList (U_SINT8 *cmdEntered_p)
{
	uconnCliCmd_t *cmd_p;
	U_SINT32 idx;

	idx = 0;
	cmd_p = &gCmds[idx];
	while(cmd_p->cmdStr_p != NULL)
	{
		if(!_stricmp(cmd_p->cmdStr_p, cmdEntered_p))
			goto done;
		else if(!_stricmp(cmd_p->cmdStr_p, "AT"))
		{
			string str = cmdEntered_p;
			string upperCaseStr = convertStringToUpperCase1(str);
			if(upperCaseStr.find("AT",0)  != string::npos)
				goto done;
		}
		cmd_p = &gCmds[++idx];
	}
	cmd_p = NULL;
done:
	return cmd_p;
}

/***********************************************************************
*	goes thru the command list and prints the help strings
*
*	Parameters
*	cliInstance_p - pointer to the CLI instance
*
*	Returns
***********************************************************************/
 U_VOID printHelpFromList (uconnCli_t *cliInstance_p)

{
	uconnCliCmd_t *cmd_p;
	U_SINT32 idx;
	idx = 0;
	cmd_p = &gCmds[idx];
	while(cmd_p->cmdStr_p != NULL)
	{
		uconnCliDisplay(UCONN_CLI_CONSOLE,(uconnCli_t*)cliInstance_p,
				"\t%-s\n", cmd_p->help_p);
		
		cmd_p = &gCmds[++idx];
	}
}


/***********************************************************************
*	verifies if the parameters passed is of the right type
*
*	Parameters
*	cmd_p - pointer to the command structure
*	cmdComponents - array having the command typed broken down to words
*	numberOfArgs - number of arguments typed
*
*	Returns
*	0 Success
*	>0 Failure with Param number
***********************************************************************/
U_SINT32 verifyParams (uconnCli_t *cliInstance_p,uconnCliCmd_t *cmd_p,
					 U_SINT8 *cmdComponents[], 
					 U_SINT32 numberOfArgs)
{
	U_SINT32 error = 0;
	U_SINT32 idx;

	/* check for min and max number of arguments */
	if(numberOfArgs < cmd_p->minParams)
	{
		error++;
		uconnCliDisplay(UCONN_CLI_CONSOLE,cliInstance_p, "\nToo few parameters\n");
		return error;
	}
	if(numberOfArgs > cmd_p->maxParams)
	{
		error++;
		uconnCliDisplay(UCONN_CLI_CONSOLE,cliInstance_p, "\tToo many parameters\n");
		return error;
	}

	idx = 1; /* first cmd component is always the command string */
	while(idx <= numberOfArgs)
	{
		if(IS_CMD_PARAM_NUM(cmd_p->paramTypes[idx-1]))
		{
			if(!isnumeric(cmdComponents[idx]))
			{
				error++;
				uconnCliDisplay(UCONN_CLI_CONSOLE,cliInstance_p, "\tParameter %d is numeric\n", 
					error+1);
				break;
			}
		}
		idx++;
	}

	return error;
}

/***********************************************************************
*	executes the typed command by calling the right handler
*
*	Parameters
*	cliInstance_p - pointer to the CLI instance
*	cmdComponents - array having the command typed broken down to words
*	numberOfArgs - number of arguments passed
*
*	Returns
*	0 Success
*	>0 Failure with Param number
***********************************************************************/
U_SINT32 uconnCliCmdExec (uconnCli_t *cliInstance_p,uconnCliCmd_t *cmd_p,
					 U_SINT8 *cmdComponents[], 
					 U_SINT32 numberOfArgs)
{
	U_SINT32 param;
	U_SINT32 status = UCONN_CLI_FAILURE;

	/* verify the command components for types */
	if((param = verifyParams(cliInstance_p, cmd_p, cmdComponents, numberOfArgs)) != 0)
	{
		goto done;
	}
	/* check if the command is local or server */

	if(cmd_p->cmdHandler_p != NULL)
	{
		/* Local command */
		status = cmd_p->cmdHandler_p(cliInstance_p, 
				cmdComponents, numberOfArgs);
	}
	else
	{
		/* remote command */
		uconnSAPIstrResp_s *sapiRsp = uconn_SAPIExecute(cmd_p->cmdId,cmdComponents, numberOfArgs);

		
		if(sapiRsp->status == UCONN_SAPI_WRAP_SUCCESS)
		{
			uconnCliDisplay(UCONN_CLI_CONSOLE,cliInstance_p,(U_SINT8 *)sapiRsp->strResp.c_str());
			status = UCONN_CLI_SUCCESS;
		}
		else if(sapiRsp->status == UCONN_SAPI_WRAP_PARAMETER_ERROR)
		{
            uconnCliDisplay(UCONN_CLI_CONSOLE,cliInstance_p,
                "\tParameter Error: %s\n",sapiRsp->strResp.c_str());
		}
		else
		{
			uconnCliDisplay(UCONN_CLI_CONSOLE,cliInstance_p,
                "\tDevice not Found : %s\n",sapiRsp->strResp.c_str());
		}
		delete sapiRsp;
	}
done:
	return status;
}

/***********************************************************************
*	run the cli command
*
*	Parameters
*	cliInstance_p - pointer to the CLI instance
*	cmdComponents - array having the command typed broken down to words
*	numberOfArgs - number of arguments passed
*
***********************************************************************/
U_VOID uconnCliRunCmd (uconnCli_t *cliInstance_p,U_SINT8 *cmdComponents[], 
					 U_SINT32 numberOfArgs)
{	
	uconnCliCmd_t *cmd_p;

	/* empty command */
	if(!cmdComponents[0])
		return;
	/* search for the command in the list */
	if(!(cmd_p = getCmdFromList(cmdComponents[0])))
	{
		printf("");
		uconnCliDisplay(UCONN_CLI_CONSOLE, (uconnCli_t*)cliInstance_p, 
			"Command not found\n");
		goto done;
	}
	/* call the command handler. if it is a local command */
	if(uconnCliCmdExec(cliInstance_p,cmd_p, cmdComponents, numberOfArgs) != UCONN_CLI_SUCCESS)
	{
		/*uconnCliDisplay(UCONN_CLI_CONSOLE, (uconnCli_t*)cliInstance_p, 
			"Command execution failed\n");*/
		goto done;
	}

	/* call the util functions else call the sapi wrap handler */
done:
	return;
}


/***********************************************************************
*	main CLI loop that gets the command types, breaks them down to 
*	components and runs the right handler
*
*	Parameters
*	cliInstance_p - pointer to the CLI instance
*
***********************************************************************/
U_VOID uconnCliGetAndRunCmds (uconnCli_t *cliInstance_p)
{
	U_SINT8 *cmdStr_p;
	U_SINT8 *cmdComponents_p[UCONN_CLI_MAX_PARAMS_SUPPORTED + 1];
	U_SINT32 numberOfArgs;

	uconnCliDisplayBanner(cliInstance_p);

	while(cliInstance_p->exitFlag == UCONN_CLI_FALSE)
	{
		uconnCliShowPrompt(cliInstance_p);
		memset(cmdComponents_p, 0, sizeof(cmdComponents_p));
		cmdStr_p = uconnCliGetCmdLine();
		numberOfArgs = uconnCliGetCmdArgs(cmdStr_p, cmdComponents_p);
		free(cmdStr_p);
		uconnCliRunCmd(cliInstance_p,cmdComponents_p, numberOfArgs);
		while(numberOfArgs >= 0)
			free(cmdComponents_p[numberOfArgs--]);
	}
}


/***********************************************************************
*	gets a command from the command line
*
*	Returns - pointer to the command string
*
***********************************************************************/
U_SINT8 *uconnCliGetCmdLine ()
{
	U_SINT8 *cmd_p, *tmp_p;
	U_SINT32 ch, len;

	if(!(cmd_p = (char*)calloc(UCONN_MAX_CMD_LINE_SIZE, sizeof(char))))
		return NULL;

	tmp_p = cmd_p;
	len = UCONN_MAX_CMD_LINE_SIZE;
	
	/* loop until newline */
	UCONN_CLI_FOREVER
	{
	  
		if((ch = fgetc(stdin)) == EOF) 
			break;
		if(!(--len))
		{
			free(cmd_p);
			return NULL;
		}
		if((*tmp_p++ = ch) == '\n')
			break;
	}
	*tmp_p = '\0';
	return cmd_p;	
}

/***********************************************************************
*	breaks down the command into components. the first component
*	is the command string itself
*
*	Parameters
*	cmdLine_p - pointer to the command line
*	cmdComponents[] - array of command components
*
*	Returns 
*	-1 Failure
*	number of arcuments on Success
*
***********************************************************************/
U_SINT32 uconnCliGetCmdArgs (U_SINT8 *cmdLine_p, 
						U_SINT8 *cmdComponents[])
{
	U_SINT8 *line_p, *end_p;
	U_SINT32 i;

	line_p = cmdLine_p;

	i = 0;
	while((*line_p != '\0') && (*line_p != '\n'))
	{
		while(isspace(*line_p)) line_p++;
		end_p = line_p;
		while(((*end_p != ' ') && (*end_p != '\t') && (*end_p != '\0')
			&& (*end_p != '\n'))) end_p++;
		/* Empty command */
		if(end_p == line_p) break;
		if(!(cmdComponents[i] = (char*)malloc(end_p - line_p + 1)))
			goto err;
		memcpy(cmdComponents[i], line_p, (end_p - line_p));
		cmdComponents[i][end_p - line_p] = '\0';
		line_p = end_p;
		if(i++ > UCONN_CLI_MAX_PARAMS_SUPPORTED) goto err;
	}
	return i-1;
err:
	while(i>=0)
		free(cmdComponents[--i]);
	return -1;
}

/***********************************************************************
*	Prints the CLI banner on startup
*	
*	Parameters
*	cliInstance_p	-	Pointer to the CLI instance structure
***********************************************************************/
static U_VOID uconnCliDisplayBanner (uconnCli_t *cliInstance_p)
{
	uconnCliDisplay
		(UCONN_CLI_CONSOLE,cliInstance_p, 
		"*********************************************************\n");
	uconnCliDisplay
		(UCONN_CLI_CONSOLE,cliInstance_p, 
		"**	LTE Connection Manager Command Line Interface\n");
	uconnCliDisplay
		(UCONN_CLI_CONSOLE, cliInstance_p, 
		"**	Version %u.%u\n", cliInstance_p->MainVersion, 
		cliInstance_p->SubVersion);
	uconnCliDisplay
		(UCONN_CLI_CONSOLE, cliInstance_p, 
		"*********************************************************\n");
}

/***********************************************************************
*	Prints the CLI prompt
*	
*	Parameters
*	cliInstance_p	-	Pointer to the CLI instance structure
***********************************************************************/
static U_VOID uconnCliShowPrompt (uconnCli_t *cliInstance_p)
{
	uconnCliDisplay(UCONN_CLI_CONSOLE, cliInstance_p, 
		"%s", cliInstance_p->prompt_p);
}

U_VOID uconnCliLogHandler (UEvent *evt_p, U_VOID *arg_p)
{
	ULogEvent *logEvt_p = reinterpret_cast<ULogEvent *>(evt_p);
	uconnCli_t *cliInstance_p = (uconnCli_t*)arg_p;

	if(!cliInstance_p->serverLogFile_p)
	{
		/* log file not opened yet */
		if(!(cliInstance_p->serverLogFile_p = 
			fopen(cliInstance_p->serverLogFilepath_p, "a")))
		{
			/* cannot do anything so return */
			uconnCliDisplay(UCONN_CLI_DEBUG1,NULL,
				"Server Log file failed to open: %s\n", cliInstance_p->serverLogFilepath_p);
			return;
		}
	}
	else
	{
		/* write the data to the file */
		fprintf(cliInstance_p->serverLogFile_p, "%s", (const U_SINT8*)logEvt_p->logString);
	}
}

void uconnCliATCmdRespHandler (UEvent *evt_p, U_VOID *arg_p)
{
	//UATCmdResponseEvent *rspEvt_p = reinterpret_cast<UATCmdResponseEvent *>(evt_p);
	UATCmdResponseEvent *rspEvt_p = (UATCmdResponseEvent*)(evt_p);
	/* print AT command response here */
	if(strlen(rspEvt_p->sATCmdResStr) > 2)
	uconnCliDisplay(UCONN_CLI_CONSOLE,NULL,
		"\n%s\n", rspEvt_p->sATCmdResStr);
}


/* event callback handler. the server can send several unsolicited messages
to the clients. these messages will be handled here. this function will be
registered with sapi wrapper as the event callback handler */
U_VOID uconnCliEvtHandler (UEvent *evt_p, U_SINT8 *data_p, U_VOID *arg_p)
{
	uconnCli_t *cliInstance_p;

	cliInstance_p = (uconnCli_t*)arg_p;
	
	/* there is no cliInstance here as these are async messages.
	so only debug print */
	switch(evt_p->getCmdId())
	{
		case UCONN_CMD_LTE_EQUIP_CONFIG_GET:
			uconnCliDisplay(UCONN_CLI_DEBUG1,NULL,
				"Event: LTE EQUIP CONFIG GET\n");
			break;
		case UCONN_CMD_LTE_EQUIP_CONFIG_SET:
			uconnCliDisplay(UCONN_CLI_DEBUG1,NULL,
				"Event: LTE EQUIP CONFIG SET\n");
			break;
		case UCONN_CMD_LTE_EQUIP_INFO_GET:
			uconnCliDisplay(UCONN_CLI_DEBUG1,NULL,
				"Event: LTE EQUIP INFO GET\n");
			break;
		case UCONN_CMD_LTE_NET_SEARCH:
			uconnCliDisplay(UCONN_CLI_DEBUG1,NULL,
				"Event: LTE NET SEARCH\n");
			break;
		case UCONN_CMD_LTE_NET_ATTACH:
			uconnCliDisplay(UCONN_CLI_DEBUG1,NULL,
				"Event: LTE NET ATTACH\n");
			break;
		case UCONN_CMD_LTE_NET_CFG_GET:
			uconnCliDisplay(UCONN_CLI_DEBUG1,NULL,
				"Event: LTE NET CONFIG GET\n");
			break;
		case UCONN_CMD_LTE_NET_CFG_SET:
			uconnCliDisplay(UCONN_CLI_DEBUG1,NULL,
				"Event: LTE NET CONFIG SET\n");
			break;
		case UCONN_CMD_LTE_NET_DETACH:
			uconnCliDisplay(UCONN_CLI_DEBUG1,NULL,
				"Event: LTE NET DETACH\n");
			break;
		case UCONN_CMD_LTE_NET_SEARCH_STATUS:
			uconnCliDisplay(UCONN_CLI_DEBUG1,NULL,
				"Event: LTE NET SEARCH STATUS\n");
			break;
		case UCONN_CMD_LTE_4G_OFF:
			uconnCliDisplay(UCONN_CLI_DEBUG1,NULL,
				"Event: LTE 4G OFF\n");
			break;
		case UCONN_CMD_LTE_4G_ON:
			uconnCliDisplay(UCONN_CLI_DEBUG1,NULL,
				"Event: LTE 4G ON\n");
			break;
		case UCONN_EVT_LTE_REG_STATUS:
			uconnCliDisplay(UCONN_CLI_DEBUG1,NULL,
				"Event: LTE REG STATUS\n");
			break;
		case UCONN_EVT_PRIMARY_ACT:
			uconnCliDisplay(UCONN_CLI_DEBUG1,NULL,
				"Event: LTE PRIMARY ACT\n");
			break;
		case UCONN_EVT_SECONDARY_ACT:
			uconnCliDisplay(UCONN_CLI_DEBUG1,NULL,
				"Event: LTE SECONDARY ACT\n");
			break;
		case UCONN_EVT_PRIMARY_DEACT:
			uconnCliDisplay(UCONN_CLI_DEBUG1,NULL,
				"Event: LTE PRIMARY DEACT\n");
			break;
		case UCONN_EVT_SECONDARY_DEACT:
			uconnCliDisplay(UCONN_CLI_DEBUG1,NULL,
				"Event: LTE SECONDARY DEACT\n");
			break;
		case UCONN_EVT_ROAMING_IND:
			uconnCliDisplay(UCONN_CLI_DEBUG1,NULL,
				"Event: LTE ROAMING INDICATION\n");
			break;
		case UCONN_EVT_LOG:
			uconnCliLogHandler(evt_p, arg_p);
			break;
		case UCONN_EVT_AT_CMD_RESPONSE:
			uconnCliATCmdRespHandler(evt_p, arg_p);
			break;
#ifdef UCONN_CLIENT_USE_SOCKETS
		case UCONN_EVT_SERVER_CONN_LOST:
			uconnCliDisplay(UCONN_CLI_DEBUG1,NULL,
				"Event: CSCM SERVER CONNECTION LOST\n");
			cliInstance_p->status = 0;
			break;
#endif /*UCONN_CLIENT_USE_SOCKETS*/
		default:;
	}
}


/*---------------------------------------------------------------------------*/
/* Command Handlers 
* Beyond this point in the file, all the functions will be handlers of the
* CLI commands. These handlers are registered along with the command name,
* help string et al, when a new command is added. The command handlers will
* call the functions in the SAPI_Wrapper file, which U_SINT32urn will pass the 
* command and parameter to the Uconnect Server.
*/

/* Handler of Help command */
static uconnCliStatus_t uconnCliHelp(U_VOID *cliInstance_p, 
									 U_SINT8 *cmdComponents_p[], 
									 U_SINT32 numberOfArgs)
{
	uconnCliCmd_t *cmd_p;

	/* if number of args is > 1, then error */
	if(numberOfArgs > 1)
	{
		return UCONN_CLI_TOO_MANY_PARAMS;
	}
	else if(!numberOfArgs)
	{
		/* PrU_SINT32 help string for all the registered commands */
		printHelpFromList((uconnCli_t*)cliInstance_p);
	}
	else
	{
		/* Print help string of the queried command */

		if(!(cmd_p = getCmdFromList(cmdComponents_p[1])))
		{
			uconnCliDisplay(UCONN_CLI_CONSOLE, (uconnCli_t*)cliInstance_p, 
			"\tInvalid Command\n");
			return UCONN_CLI_INVALID_COMMAND;
		}
		else
		{
			uconnCliDisplay(UCONN_CLI_CONSOLE, (uconnCli_t*)cliInstance_p, 
			"\t%-s\n", cmd_p->help_p);
		}
	}
	return UCONN_CLI_SUCCESS;
}

static uconnCliStatus_t uconnCliPrompt(U_VOID *cliInstance_p, U_SINT8 *cmdComponents_p[],
				 U_SINT32 numberOfArgs)
{
	if(!numberOfArgs)
		return UCONN_CLI_TOO_FEW_PARAMS;
	if(numberOfArgs > 1)
		return UCONN_CLI_TOO_MANY_PARAMS;

	free(((uconnCli_t*)cliInstance_p)->prompt_p);
	((uconnCli_t*)cliInstance_p)->prompt_p = 
		(U_UINT8*)_strdup((const U_SINT8*)cmdComponents_p[1]);
	return UCONN_CLI_SUCCESS;
}


static uconnCliStatus_t uconnCliLog(U_VOID *cliInstance_p, U_SINT8 *cmdComponents_p[],
				 U_SINT32 numberOfArgs)
{
	if(numberOfArgs > 2)
		return UCONN_CLI_TOO_MANY_PARAMS;
	if(!numberOfArgs)
	{
		uconnCliDisplay(UCONN_CLI_CONSOLE, (uconnCli_t*)cliInstance_p, 
			"\tSession Log file: %s\n\tSession Logging: %s\n",
			((uconnCli_t*)cliInstance_p)->logFilepath_p, 
			(((uconnCli_t*)cliInstance_p)->logging == UCONN_CLI_TRUE? "on": "off"));
		return UCONN_CLI_SUCCESS;
	}
	if(!(_stricmp((const U_SINT8*)cmdComponents_p[1], "file")))
	{
		if(numberOfArgs == 1)
		{
			uconnCliDisplay(UCONN_CLI_CONSOLE, (uconnCli_t*)cliInstance_p, "\tSession Log file: %s\n", 
				((uconnCli_t*)cliInstance_p)->logFilepath_p);
		}
		else
		{
			if(((uconnCli_t*)cliInstance_p)->logFile_p)
				fclose(((uconnCli_t*)cliInstance_p)->logFile_p);
			((uconnCli_t*)cliInstance_p)->logFilepath_p = _strdup(cmdComponents_p[2]);
		}
	}
	else if(!_stricmp((const U_SINT8*)cmdComponents_p[1], "on"))
	{
		if(numberOfArgs > 1) return UCONN_CLI_TOO_MANY_PARAMS;
                #ifdef WIN32
		if(fopen_s(&(((uconnCli_t*)cliInstance_p)->logFile_p), 
			((uconnCli_t*)cliInstance_p)->logFilepath_p, "a+"))
		{
			uconnCliDisplay(UCONN_CLI_CONSOLE,((uconnCli_t*)cliInstance_p),
				"Cannot open Session log file\n");
		}
                #endif
		((uconnCli_t*)cliInstance_p)->logging = UCONN_CLI_TRUE;
	}
	else if(!_stricmp((const U_SINT8*)cmdComponents_p[1], "off"))
	{
		if(numberOfArgs > 1) return UCONN_CLI_TOO_MANY_PARAMS;
		if(((uconnCli_t*)cliInstance_p)->logFile_p) 
			fclose(((uconnCli_t*)cliInstance_p)->logFile_p);
		((uconnCli_t*)cliInstance_p)->logging = UCONN_CLI_FALSE;
	}
	else
	{
		return UCONN_CLI_INVALID_ARGUMENT;
	}

	return UCONN_CLI_SUCCESS;
}


static uconnCliStatus_t uconnCliQuit(U_VOID *cliInstance_p, 
									 U_SINT8 *cmdComponents_p[], 
									 U_SINT32 numberOfArgs)
{
	if(numberOfArgs)
		return UCONN_CLI_TOO_MANY_PARAMS;

	/* cleanup and free memeory */
	uconnCliCleanup((uconnCli_t*)cliInstance_p);
	((uconnCli_t*)cliInstance_p)->exitFlag = UCONN_CLI_TRUE;
	return UCONN_CLI_SUCCESS;
}

static uconnCliStatus_t uconnCliVersion(U_VOID *cliInstance_p, 
										U_SINT8 *cmdComponents_p[],
										U_SINT32 numberOfArgs)
{
	if(numberOfArgs > 1)
	{
		return UCONN_CLI_TOO_MANY_PARAMS;
	}
	else if(numberOfArgs == 1)
	{
		if(!_stricmp(cmdComponents_p[1], "client"))
			uconnCliDisplay(UCONN_CLI_CONSOLE,(uconnCli_t*)cliInstance_p, 
			"\tClient Version: %u.%u\n", 
			((uconnCli_t*)cliInstance_p)->MainVersion,
			((uconnCli_t*)cliInstance_p)->SubVersion);
		else if(!_stricmp(cmdComponents_p[1], "server"))
			uconnCliDisplay(UCONN_CLI_CONSOLE,(uconnCli_t*)cliInstance_p,
			"\tServer Version\n");
		else
			return UCONN_CLI_INVALID_ARGUMENT;
	}
	else
	{
		uconnCliDisplay(UCONN_CLI_CONSOLE,(uconnCli_t*)cliInstance_p,
			"\tClient Version: %u.%u\n", 
			((uconnCli_t*)cliInstance_p)->MainVersion,
			((uconnCli_t*)cliInstance_p)->SubVersion);
		uconnCliDisplay(UCONN_CLI_CONSOLE,(uconnCli_t*)cliInstance_p,
			"\tServer Version\n");
	}
	return UCONN_CLI_SUCCESS;
}

static uconnCliStatus_t uconnCliServerLog(U_VOID *cliInstance_p, 
										U_SINT8 *cmdComponents_p[],
										U_SINT32 numberOfArgs)
{
	uconnSAPIstrResp_s *sapiRsp, *logStatus_p;
	uconnCliStatus_t retVal = UCONN_CLI_SUCCESS;
	U_SINT32 cmdId;

	if(numberOfArgs == 0)
	{
		/* display stuff */
		uconnCliDisplay(UCONN_CLI_CONSOLE, (uconnCli_t*)cliInstance_p, 
			"\tLog File: %s\n", ((uconnCli_t*)cliInstance_p)->serverLogFilepath_p);
		/* get the log status and level */
		logStatus_p = uconn_SAPIExecute(UCONN_CMD_SERVER_LOG_GETSTATUS, 
					cmdComponents_p, numberOfArgs);
		if(logStatus_p->status != UCONN_SAPI_WRAP_SUCCESS)
		{
			retVal = UCONN_CLI_FAILURE;
		}
		else
		{
			/* the string response is in the sapi response */
			uconnCliDisplay(UCONN_CLI_CONSOLE, (uconnCli_t*)cliInstance_p, 
				(U_SINT8*)logStatus_p->strResp.c_str());
		}
		free(logStatus_p);
		goto done;
	}
	else if(!_stricmp(cmdComponents_p[1], "file"))
	{
		/* close the current file */
		if(((uconnCli_t*)cliInstance_p)->serverLogFile_p)
		{
			fclose(((uconnCli_t*)cliInstance_p)->serverLogFile_p);
			free(((uconnCli_t*)cliInstance_p)->serverLogFilepath_p);
			((uconnCli_t*)cliInstance_p)->serverLogFilepath_p = _strdup(cmdComponents_p[2]);
		}
		goto done;
	}
	else if(!_stricmp(cmdComponents_p[1], "on"))
	{
		/* turn logging on at server */
		cmdId = UCONN_CMD_SERVER_LOG_ONOFF;
	}
	else if(!_stricmp(cmdComponents_p[1], "off"))
	{
		/* turn logging off */
		cmdId = UCONN_CMD_SERVER_LOG_ONOFF;
		if(((uconnCli_t*)cliInstance_p)->serverLogFile_p)
		{
			fclose(((uconnCli_t*)cliInstance_p)->serverLogFile_p);
		}
	}
	else if(!_stricmp(cmdComponents_p[1], "level"))
	{
		/* turn logging off */
		if(numberOfArgs < 2)
		{
			retVal = UCONN_CLI_TOO_FEW_PARAMS;
			goto done;
		}
		if(!_stricmp(cmdComponents_p[2], "debug"))
		{
			/* set logging to debug */
			cmdId = UCONN_CMD_SERVER_LOG_LEVEL;
		}
		else if(!_stricmp(cmdComponents_p[2], "info"))
		{
			/* set logging to info */
			cmdId = UCONN_CMD_SERVER_LOG_LEVEL;
		}
		else if(!_stricmp(cmdComponents_p[2], "error"))
		{
			/* set logging to error */
			cmdId = UCONN_CMD_SERVER_LOG_LEVEL;
		}
		else
		{
			retVal = UCONN_CLI_INVALID_ARGUMENT;
			goto done;
		}
	}
	else
	{
		retVal = UCONN_CLI_INVALID_ARGUMENT;
		goto done;
	}
	sapiRsp = uconn_SAPIExecute(cmdId, cmdComponents_p, numberOfArgs);
	if(sapiRsp->status != UCONN_SAPI_WRAP_SUCCESS)
		retVal = UCONN_CLI_FAILURE;
	delete sapiRsp;

done:
	return retVal;
}



#ifdef UCONN_CLIENT_USE_SOCKETS
static uconnCliStatus_t uconnCliServer(U_VOID *cliInstance_p, 
									   U_SINT8 *cmdComponents_p[],
										U_SINT32 numberOfArgs)
{

	if(numberOfArgs > 2)
		return UCONN_CLI_TOO_MANY_PARAMS;
	if(!numberOfArgs)
	{
		/* call sapi wrapper to get server version
		if command fails, then display server not up*/
		uconnCliDisplay(UCONN_CLI_CONSOLE, (uconnCli_t*)cliInstance_p,
			"\tServer version:\n");
		uconnCliDisplay(UCONN_CLI_CONSOLE, (uconnCli_t*)cliInstance_p,
			"\tClient-Server Connection:\t%s\n", (((uconnCli_t*)cliInstance_p)->status)? "Up": "Down");
		uconnCliDisplay(UCONN_CLI_CONSOLE, (uconnCli_t*)cliInstance_p,
			"\tServer IP address:\t\t%s\n", ((uconnCli_t*)cliInstance_p)->servIpAddr_p);
		uconnCliDisplay(UCONN_CLI_CONSOLE,(uconnCli_t*)cliInstance_p,
			"\tServer socket port:\t\t%u\n", ((uconnCli_t*)cliInstance_p)->servPort);
		return UCONN_CLI_SUCCESS;
	}
	else
	{
		if(!_stricmp((const U_SINT8*)cmdComponents_p[1], "connect"))
		{
			if(numberOfArgs > 1)
			{
				return UCONN_CLI_TOO_MANY_PARAMS;
			}
			else
			{
				/* connect to server */
				if(uconn_SAPIwrapConnectToSever(
				((uconnCli_t*)cliInstance_p)->servIpAddr_p, ((uconnCli_t*)cliInstance_p)->servPort)
																		!= UCONN_SAPI_WRAP_SUCCESS)
				{
					uconnCliDisplay(UCONN_CLI_CONSOLE, ((uconnCli_t*)cliInstance_p),
						"Connection to server failed\n"); 
					return UCONN_CLI_FAILURE;
				}

				((uconnCli_t*)cliInstance_p)->status = 1;
				return UCONN_CLI_SUCCESS;
			}
		}
		if(!_stricmp((const U_SINT8*)cmdComponents_p[1], "disconnect"))
		{
			if(numberOfArgs > 1)
			{
				return UCONN_CLI_TOO_MANY_PARAMS;
			}
			else
			{
				/* connect to server */
				uconn_SAPIwrapDisconnectFrmSever();
				((uconnCli_t*)cliInstance_p)->status = 0;
				return UCONN_CLI_SUCCESS;
			}
		}
		if(!_stricmp((const U_SINT8*)cmdComponents_p[1], "ip"))
		{
			if(numberOfArgs < 2)
			{
				return UCONN_CLI_TOO_FEW_PARAMS;
			}
			else
			{
				if(((uconnCli_t*)cliInstance_p)->servIpAddr_p)
					free(((uconnCli_t*)cliInstance_p)->servIpAddr_p);
				((uconnCli_t*)cliInstance_p)->servIpAddr_p = 
					_strdup((const U_SINT8*)cmdComponents_p[2]);
				return UCONN_CLI_SUCCESS;
			}
		}
		if(!_stricmp((const U_SINT8*)cmdComponents_p[1], "port"))
		{
			if(numberOfArgs < 2)
			{
				return UCONN_CLI_TOO_FEW_PARAMS;
			}
			else
			{
				((uconnCli_t*)cliInstance_p)->servPort = 
					(U_UINT16)atoi(cmdComponents_p[2]);
				return UCONN_CLI_SUCCESS;
			}
		}
		else
			return UCONN_CLI_INVALID_ARGUMENT;
	}
	return UCONN_CLI_SUCCESS;
}
#endif /* UCONN_CLIENT_USE_SOCKETS */


/*VOID WriteLog(char* pMsg)
{
	/*
	take_lock(&writeLock);
	try
	{
		SYSTEMTIME oT;
		::GetLocalTime(&oT);
		FILE* pLog = fopen(SERVICE_LOG_LOCATION,"a");
		fprintf(pLog,"%02d/%02d/%04d, %02d:%02d:%02d %s\n",oT.wMonth,oT.wDay,oT.wYear,oT.wHour,oT.wMinute,oT.wSecond,pMsg); 
		fclose(pLog);
	} catch(...) {}
	
}*/

