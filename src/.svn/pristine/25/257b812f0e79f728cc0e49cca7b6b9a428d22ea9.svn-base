/*----------------------------------------------------------------
	uconn_IdleEvent.h

	This header file describes the interfaces for the Idle State
	events
	
	Author: Sathyanarayanan S
	Date  : 07-JAN-2011
*/



/*
    Revision History

	  DATE		VER		AUTHOR		DESCRIPTION
	07-JAN-11	0.1		 SS	    Creation - Added this header
----------------------------------------------------------------*/
#ifndef UCONN_IDLE_EVENT_H_
#define UCONN_IDLE_EVENT_H_

#include "uconn_Event.h"
#include "uconn_AT_DAL.h"
#include "uconn_ServerAPI.h"
//#include "uconn_bcm_DAL.h"


#if 0
class UIdleEvent:UEvent
{
private:
	
public:
	/* Declare all the variables for the events that would be handled
	by the IDLE state */
	UIdleEvent(U_UINT32 timer):UEvent(UCONN_CMD_LTE_NET_SEARCH)
	{
		plmn_timer = timer;
		setSize(sizeof(*this));
	}

	UIdleEvent(U_UINT32 uconfig_mode, U_UINT32 uplmn_id, U_UINT32 urat)
		:UEvent(UCONN_CMD_LTE_NET_ATTACH)
	{
		setSize(sizeof(*this));
		/* Need to check on the setting of primary_ctx_id */
		select_mode = uconfig_mode;
		plmn_id = uplmn_id;
		rat = urat;
		context_id = 0;
		lte_reg_status = 0;
		ctx_config = new T_PS_CTX_CFG();
		pri_params = new T_PS_CTX_PRIMARY_PARAMS();
		sec_params = new T_PS_CTX_SECONDARY_PARAMS();
		apn_table = new T_APN_TABLE();
	}

	UIdleEvent():UEvent(UCONN_EVT_LTE_REG_STATUS) {
	setSize(sizeof(*this));};

	UIdleEvent(U_UINT32 cmdID, U_UINT32 id):UEvent(cmdID) {
	setSize(sizeof(*this));};

	U_UINT32 plmn_timer; /* For Network Search */
	U_UINT32 num_plmns;
	U_UINT32 select_mode;/* The others for net_attach */
	U_UINT32 plmn_id;
	U_UINT32 rat;
	U_UINT32 context_id;
	U_UINT32 lte_reg_status;
	U_UINT32 roam_status;
	T_PS_CTX_CFG* ctx_config;
	T_PLMN_EARCH_RESULT_ENTRY nwlist[MAX_APN]; /* Needs to refined later */
	T_PS_CTX_PRIMARY_PARAMS* pri_params;
	T_PS_CTX_SECONDARY_PARAMS* sec_params;
	T_APN_TABLE *apn_table;
	
};
#endif

#endif