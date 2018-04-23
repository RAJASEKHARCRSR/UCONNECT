/*****************************************************************************
**
** uconn_sockComm.h
** This file has the data structure defns for communication between CLI 
** Server and Client that communicate using Sockets
**
** Author: Sathyanarayanan S
** Date  : 29-DEC-10
** (c) United Mobile Apps Pvt Ltd (2010 - 2015)

    VER		  DATE		AUTHOR		DESCRIPTION
	0.1		29/12/10     SS			Creation
****************************************************************************/

#ifndef _UCONN_SOCK_COMM_H_
#define _UCONN_SOCK_COMM_H_

#include "UTypes.h"
#include "uconn_SAPIwrapper.h"



#define UCONN_CMD_LTE_EQUIP_CONFIG_GET				1
#define UCONN_CMD_LTE_EQUIP_CONFIG_SET				2
#define UCONN_CMD_LTE_EQUIP_INFO_GET				3
#define UCONN_CMD_LTE_NET_SEARCH					4
#define UCONN_CMD_LTE_NET_ATTACH					5
#define UCONN_CMD_LTE_NET_CFG_GET					6
#define UCONN_CMD_LTE_NET_CFG_SET					7
#define UCONN_CMD_LTE_NET_DETACH					8

#if 0
typedef struct uconnSockCommHdr_s {
	unsigned int tokenid;	/* unique ID to match response with command */
	unsigned int commandId;	/* command identifier */
	unsigned int length;	/* length of the payload params */
}uconnSockCommHdr_t;

typedef struct servCmd {
	uconnSockCommHdr_t sockCommHdr;
	char data[1];			/* points to the first byte of a stream of data */
} servCmd_t;


typedef struct
{
	U_UINT32 u32Bitfields;
    T_PLMN_SELECT_MODE plmn_select_mode;
    T_PLMN plmn;
    T_RAT rat;
    U_UINT32 primary_ctx_id;
} T_NET_ATTACH_REQ;

typedef struct
{
    U_UINT32 plmn_search_timer;
}T_NET_SEARCH_REQ;

typedef struct
{
    T_RAT rat;
}T_NET_STATUS_GET_REQ;
#endif


#endif /*_UCOMM_SOCK_COMM_H_*/