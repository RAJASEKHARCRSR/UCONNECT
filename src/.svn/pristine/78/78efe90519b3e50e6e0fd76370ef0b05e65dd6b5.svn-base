/*****************************************************************************
**
** UTypes.h
** This header file describes the different Type specifiers
**
** Author: Sriram V Iyer
** Date  : 31-DEC-10
** (c) United Mobile Apps Pvt Ltd (2010 - 2015)

    VER		  DATE		AUTHOR		DESCRIPTION
	0.1		31/12/10     SVI		Creation
	0.2		03/01/11	 SS			Added New Data types
****************************************************************************/

#ifndef UTYPES_H_
#define UTYPES_H_

#include <string>

#if (defined(__linux__)|| defined(__MACH__))

#include<sys/types.h>
#include<time.h>
#include<signal.h>
#endif

using namespace std;
#ifdef WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#include <Windows.h>
typedef SOCKET U_SOCKET;
#pragma warning(disable: 4996)

#else

typedef unsigned int U_SOCKET;

#endif

typedef unsigned int	U_UINT32;
typedef unsigned int U_UINT;
typedef unsigned int* U_PUINT;

typedef int				U_SINT32;
typedef int                     U_INT;
typedef int*			U_PINT;

typedef U_SINT32*		U_PSINT32;
typedef U_UINT32*		U_PUINT32;
typedef string			U_STRING;
typedef float			U_FLOAT;
typedef unsigned short	U_UINT16;
typedef unsigned short U_USHORT;
typedef unsigned short* U_PUSHORT;

typedef short			U_INT16;
typedef short			U_SHORT;
typedef short*                  U_PSHORT;

typedef bool			U_BOOL;

typedef void*			U_PVOID;
typedef void			U_VOID;

typedef	char*		    U_CSTR;
typedef string          U_STRING;
typedef unsigned char   U_UINT8;
typedef char			U_SINT8;
typedef unsigned char*  U_PCHAR;
typedef U_UINT8*        U_PUINT8;

typedef unsigned char U_UCHAR;
typedef unsigned char*  U_PUCHAR;

typedef unsigned long U_ULONG;
typedef unsigned long U_DWORD;

typedef long U_LONG;
typedef long long  U_ULONG64;

typedef double			U_DOUBLE; //asper at_command

#define IS_INSTANCE_OF(var,type) ((dynamic_cast<type*>(var) == NULL) ? false : true)
typedef U_VOID(*P_AT_CALLBACK) (U_STRING sResp);
//typedef U_VOID(*P_AT_CALLBACK_WITH_ID) (U_STRING sResp,U_UINT32 cmdId);

/* Added by UMA */
/*
typedef int T_PLMN_SELECT_MODE ;
typedef int T_PLMN;
typedef int T_RAT;
typedef int T_LIST_MEMBERSHIP;
*/
#ifdef win32
typedef void* U_TIMERID;

#endif
typedef U_UINT32 T_REG_INFO;
typedef U_UINT32 T_PS_CTX_CFG_TYPE;
typedef U_UINT32 T_IP_IF_STATUS;

#define	MAX_FREE_TEXT						100
#define MAX_NUM_PLMN                        10
#define MAX_PLMN_ID_LEN                     16
#define MAX_NO_OF_CIDS				        10
#define	MAX_APN_SIZE	                    101
#define	MAX_NW_SIZE                         50
#define	MAX_USBEP_SIZE                      50
#define	MAX_ADDRESS_SIZE                    140
#define	MAX_NUM_APN                         10
#define	MAX_PDP_ADDRESS_SIZE            	140
#define MAX_PLMN_ID			            	10 
#define MAX_GW_ADDRESS_SIZE			    	140
#define MAX_DNS_PRIMARY_ADDRESS_SIZE	    140
#define MAX_DNS_SECONDARY_ADDRESS_SIZE	    140
#define MAX_PCSCF_PRIMARY_ADDRESS_SIZE	    140
#define MAX_PCSCF_SECONDARY_ADDRESS_SIZE	140
#define MAX_SRC_ADDRESS_SIZE				140
#define MAX_PORT_RANGE						12
#define MAX_SECURITY_PARAMETER_INDEX_RANGE	8
#define MAX_TRAFFIC_CLASS_AND_MASK_RANGE	8
#define MAX_TOS_SIZE						10 //check
#define MAX_LOG_CMD_SIZE					64 //check
#define MAX_LOG_BUFFER_SIZE					2000 //check
#define MAX_AT_COMMAND_REQUEST				200
#define MAX_AT_COMMAND_RESPONSE				800
#define MAX_LOG_LENGTH_AT_CLIENT			700
#define MAX_VERSION_STRING_LEN				500
#define	MAX_LOG_FILE_NAME_WITH_PATH         250


//T_PS_CTX_CFG_TYPE
#define E_CFG_QOS_2G                    1
#define E_CFG_MIN_QOS_2G                    2
#define E_CFG_REQ_QOS_3G                    3
#define E_CFG_MIN_QOS_3G                    4
#define E_CFG_QOS_4G                        5
#define E_CFG_TFT                       6
#define E_CFG_PCO_MS                        7

//#define endl "\n"

#endif
