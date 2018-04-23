/*****************************************************************************/
/* (c) Broadcom Communications Inc.                                          */
/*     All Rights Reserved                                                   */
/*                                                                           */
/*  Description     :                                                        */
/*             - List of Command ids, type defns and error defns             */
/*  used by API, Applications, API Router      								 */
/*                                                                           */
/*****************************************************************************/
#ifndef CAPI_COMMON_H
#define CAPI_COMMON_H


/*   Revision History:                                                      
08.00.00.01 : Added version for CAPI Common header file 
             
*/

#define CAPI_HEADER_VERSION		"08.00.00.01"		

/*****************************************************************************/
/*								Command IDs									 */
/*****************************************************************************/

/* Common command base : START */
#define CAPI_CMD_WIMAX_BASE                        0x00000000
#define CAPI_CMD_LTE_BASE                          0x10000000
#define CAPI_CMD_DSD_BASE                          0x20000000
#define CAPI_CMD_TRANSPARENT_MODE_BASE             0x30000000
#define CAPI_CMD_SYSCTL_BASE					   0x80000000
/* Common command base : END */

/* Sys control Command ID list : START */

/* Sys control Command ID requests */
#define SYSCTL_CMD_REQ_BASE							(CAPI_CMD_SYSCTL_BASE + 0x0)

#define SYSCTL_CMD_RDM								(SYSCTL_CMD_REQ_BASE + 3)
#define SYSCTL_CMD_WRM								(SYSCTL_CMD_REQ_BASE + 4)
#define SYSCTL_CMD_GET_VERSION_INFO					(SYSCTL_CMD_REQ_BASE + 5)


/* Sys control Command ID notifications */
#define SYSCTL_CMD_NOTI_BASE						(CAPI_CMD_SYSCTL_BASE + 0x00100000)

#define SYSCTL_NOTI_DEVICE_ERROR					(SYSCTL_CMD_NOTI_BASE + 1)
#define SYSCTL_NOTI_DEVICE_NOT_OPEN					(SYSCTL_CMD_NOTI_BASE + 2)
#define SYSCTL_NOTI_DEVICE_OPEN						(SYSCTL_CMD_NOTI_BASE + 3)
#define SYSCTL_NOTI_DISCONNECTED					(SYSCTL_CMD_NOTI_BASE + 4)
#define SYSCTL_NOTI_DEVICE_CONNECTED				(SYSCTL_CMD_NOTI_BASE + 5)
#define SYSCTL_NOTI_DEVICE_DISCONNECTED				(SYSCTL_CMD_NOTI_BASE + 6)
#define SYSCTL_NOTI_MODULE_DEBUG  					(SYSCTL_CMD_NOTI_BASE + 7)


/* Sys control Command ID list : END */

/* LTE Command IDs : START */
#define CAPI_CMD_LTE_FUNC_BASE                     (CAPI_CMD_LTE_BASE + 0x000)
#define CAPI_CMD_LTE_EVENTS_BASE                   (CAPI_CMD_LTE_BASE + 0x100)


#define CAPI_CMD_TM_CAL_BASE                       (CAPI_CMD_TRANSPARENT_MODE_BASE + 0x100)
#define CAPI_CMD_TM_STAT_BASE                      (CAPI_CMD_TRANSPARENT_MODE_BASE + 0x200)
#define CAPI_CMD_TM_VSG_BASE                       (CAPI_CMD_TRANSPARENT_MODE_BASE + 0x300)
#define CAPI_CMD_TM_DEVMODE_BASE                   (CAPI_CMD_TRANSPARENT_MODE_BASE + 0x400)


/* LTE Events  */
#define CAPI_CMD_LTE_EVT_RSSI_2G3G                 (CAPI_CMD_LTE_EVENTS_BASE+ 1)
#define CAPI_CMD_LTE_EVT_CS_REG_STATUS             (CAPI_CMD_LTE_EVENTS_BASE+ 2)
#define CAPI_CMD_LTE_EVT_UMTS_REG_STATUS           (CAPI_CMD_LTE_EVENTS_BASE+ 3)
#define CAPI_CMD_LTE_EVT_LTE_REG_STATUS            (CAPI_CMD_LTE_EVENTS_BASE+ 4)
#define CAPI_CMD_LTE_EVT_ROAMING_IND               (CAPI_CMD_LTE_EVENTS_BASE+ 5)
#define CAPI_CMD_LTE_EVT_PRIMARY_ACT               (CAPI_CMD_LTE_EVENTS_BASE+ 6)
#define CAPI_CMD_LTE_EVT_SECONDARY_ACT             (CAPI_CMD_LTE_EVENTS_BASE+ 7)
#define CAPI_CMD_LTE_EVT_PRIMARY_DEACT             (CAPI_CMD_LTE_EVENTS_BASE+ 8)
#define CAPI_CMD_LTE_EVT_SECONDARY_DEACT           (CAPI_CMD_LTE_EVENTS_BASE+ 9)
#define CAPI_CMD_LTE_EVT_CTX_MODIFY                (CAPI_CMD_LTE_EVENTS_BASE+ 10)
#define CAPI_CMD_LTE_EVT_DETACH                (CAPI_CMD_LTE_EVENTS_BASE+ 11)


/* LTE functions cmd ids */
#define CAPI_CMD_LTE_EQUIP_CONFIG_GET              (CAPI_CMD_LTE_FUNC_BASE+ 1)
#define CAPI_CMD_LTE_EQUIP_CONFIG_SET              (CAPI_CMD_LTE_FUNC_BASE+ 2)
#define CAPI_CMD_LTE_EQUIP_INFO_GET                (CAPI_CMD_LTE_FUNC_BASE+ 3)
#define CAPI_CMD_LTE_NET_SEARCH                    (CAPI_CMD_LTE_FUNC_BASE+ 4)
#define CAPI_CMD_LTE_NET_ATTACH                    (CAPI_CMD_LTE_FUNC_BASE+ 5)
#define CAPI_CMD_LTE_NET_CFG_GET                   (CAPI_CMD_LTE_FUNC_BASE+ 6)
#define CAPI_CMD_LTE_NET_CFG_SET                   (CAPI_CMD_LTE_FUNC_BASE+ 7)
#define CAPI_CMD_LTE_NET_DETACH                    (CAPI_CMD_LTE_FUNC_BASE+ 8)
#define CAPI_CMD_LTE_NET_STATUS_GET                (CAPI_CMD_LTE_FUNC_BASE+ 9)
#define CAPI_CMD_LTE_NET_SIG_QUAL_GET              (CAPI_CMD_LTE_FUNC_BASE+ 10)
#define CAPI_CMD_LTE_PS_DFN_INFO_GET               (CAPI_CMD_LTE_FUNC_BASE+ 11)
#define CAPI_CMD_LTE_PS_CTX_DEFINE_PRIMARY         (CAPI_CMD_LTE_FUNC_BASE+ 12)
#define CAPI_CMD_LTE_PS_CTX_DEFINE_SECONDARY       (CAPI_CMD_LTE_FUNC_BASE+ 13)
#define CAPI_CMD_LTE_PS_CTX_UNDEFINE_PRIMARY       (CAPI_CMD_LTE_FUNC_BASE+ 14)
#define CAPI_CMD_LTE_PS_CTX_UNDEFINE_SECONDARY     (CAPI_CMD_LTE_FUNC_BASE+ 15)
#define CAPI_CMD_LTE_PS_CTX_CFG_GET                (CAPI_CMD_LTE_FUNC_BASE+ 16)
#define CAPI_CMD_LTE_PS_CTX_CFG_SET                (CAPI_CMD_LTE_FUNC_BASE+ 17)
#define CAPI_CMD_LTE_PS_CTX_INFO_GET               (CAPI_CMD_LTE_FUNC_BASE+ 18)
#define CAPI_CMD_LTE_PS_CTX_ACTIVATE               (CAPI_CMD_LTE_FUNC_BASE+ 19)
#define CAPI_CMD_LTE_PS_CTX_DEACTIVATE             (CAPI_CMD_LTE_FUNC_BASE+ 20)
#define CAPI_CMD_LTE_PS_CTX_MODIFY                 (CAPI_CMD_LTE_FUNC_BASE+ 21)
#define CAPI_CMD_ENCAPSULATED_LTE_AT               (CAPI_CMD_LTE_FUNC_BASE+ 22)
#define CAPI_CMD_LTE_LOG                           (CAPI_CMD_LTE_FUNC_BASE+ 23)
#define CAPI_CMD_LTE_APN_TABLE_GET                 (CAPI_CMD_LTE_FUNC_BASE+ 24)
#define CAPI_CMD_LTE_APN_TABLE_SET                 (CAPI_CMD_LTE_FUNC_BASE+ 25)
#define CAPI_CMD_LTE_IP_IF_STATUS                 (CAPI_CMD_LTE_FUNC_BASE+ 26)

/* DSD functions cmd ids*/
#define DSD_ARE_DSD_CHECKSUM_VALID					(CAPI_CMD_DSD_BASE+1)
#define DSD_GET_INFO								(CAPI_CMD_DSD_BASE+2)
#define DSD_READ_FIELD								(CAPI_CMD_DSD_BASE+3)
#define DSD_WRITE_FIELD								(CAPI_CMD_DSD_BASE+4)
#define DSD_READ_TLV								(CAPI_CMD_DSD_BASE+5)
#define DSD_WRITE_TLV								(CAPI_CMD_DSD_BASE+6)
#define DSD_READ_RAW								(CAPI_CMD_DSD_BASE+7)
#define DSD_WRITE_RAW								(CAPI_CMD_DSD_BASE+8)
#define DSD_SELECT_CURRENT_DSD						(CAPI_CMD_DSD_BASE+9)
#define DSD_GET_CHECKSUMS							(CAPI_CMD_DSD_BASE+10)
#define DSD_COPY_SECTION							(CAPI_CMD_DSD_BASE+11)
#define DSD_GET_LENGTH								(CAPI_CMD_DSD_BASE+12)
#define DSD_UNLOCK_SECTION_FOR_READ					(CAPI_CMD_DSD_BASE+13)
#define DSD_UNLOCK_SECTION_FOR_WRITE				(CAPI_CMD_DSD_BASE+14)
#define DSD_LOCK_SECTION_FOR_READ					(CAPI_CMD_DSD_BASE+15)
#define DSD_LOCK_SECTION_FOR_WRITE					(CAPI_CMD_DSD_BASE+16)
#define DSD_GET_SECTION_LENGTH						(CAPI_CMD_DSD_BASE+17)
#define DSD_ADD_SECTION								(CAPI_CMD_DSD_BASE+18)

#define DSD_GET_MAC									(CAPI_CMD_DSD_BASE+19)
#define DSD_SET_MAC									(CAPI_CMD_DSD_BASE+20)
#define DSD_UPDATE_SHADOW							(CAPI_CMD_DSD_BASE+21)
#define DSD_REFRESH_CONTEXT_TABLE                   (CAPI_CMD_DSD_BASE+22)

#define DSD_CPU1_OFFSET 100

#define CPU1_DSD_ARE_DSD_CHECKSUM_VALID                 (CAPI_CMD_DSD_BASE+DSD_CPU1_OFFSET+1)
#define CPU1_DSD_GET_INFO                               (CAPI_CMD_DSD_BASE+DSD_CPU1_OFFSET+2)
#define CPU1_DSD_READ_FIELD                             (CAPI_CMD_DSD_BASE+DSD_CPU1_OFFSET+3)
#define CPU1_DSD_WRITE_FIELD                            (CAPI_CMD_DSD_BASE+DSD_CPU1_OFFSET+4)
#define CPU1_DSD_READ_TLV                               (CAPI_CMD_DSD_BASE+DSD_CPU1_OFFSET+5)
#define CPU1_DSD_WRITE_TLV                              (CAPI_CMD_DSD_BASE+DSD_CPU1_OFFSET+6)
#define CPU1_DSD_READ_RAW                               (CAPI_CMD_DSD_BASE+DSD_CPU1_OFFSET+7)
#define CPU1_DSD_WRITE_RAW                              (CAPI_CMD_DSD_BASE+DSD_CPU1_OFFSET+8)
#define CPU1_DSD_SELECT_CURRENT_DSD                     (CAPI_CMD_DSD_BASE+DSD_CPU1_OFFSET+9)
#define CPU1_DSD_GET_CHECKSUMS                          (CAPI_CMD_DSD_BASE+DSD_CPU1_OFFSET+10)
#define CPU1_DSD_COPY_SECTION                           (CAPI_CMD_DSD_BASE+DSD_CPU1_OFFSET+11)
#define CPU1_DSD_GET_LENGTH                             (CAPI_CMD_DSD_BASE+DSD_CPU1_OFFSET+12) 

/* LTE Command IDs : END */


/*****************************************************************************/
/*								Typedefines	of CAPI							 */
/*****************************************************************************/

/* type defns of CAPI : START */


#ifdef NUCLEUS_OS
#include "bcm_types.h"
#endif


typedef void BRCM_CAPI_VOID;
typedef void *BRCM_CAPI_PVOID;
typedef void *BRCM_CAPI_LPVOID;

typedef char BRCM_CAPI_CHAR;
typedef char* BRCM_CAPI_PCHAR;
typedef unsigned char BRCM_CAPI_UCHAR;
typedef unsigned char *BRCM_CAPI_PUCHAR;
typedef unsigned char BRCM_CAPI_BYTE;

typedef signed short BRCM_CAPI_SHORT;
typedef signed short *BRCM_CAPI_PSHORT;

typedef unsigned short BRCM_CAPI_USHORT;
typedef unsigned short *BRCM_CAPI_PUSHORT;

typedef signed int BRCM_CAPI_INT, BRCM_CAPI_INT32;
typedef signed int *BRCM_CAPI_PINT, *BRCM_CAPI_PINT32;

typedef unsigned int BRCM_CAPI_UINT;
typedef unsigned int *BRCM_CAPI_PUINT, *BRCM_CAPI_PUINT32;

typedef long BRCM_CAPI_LONG;
typedef long *BRCM_CAPI_PLONG;

#if !defined(__APPLE__)
typedef unsigned long BRCM_CAPI_ULONG;
#endif
typedef unsigned long *BRCM_CAPI_PULONG;

typedef unsigned long long BRCM_CAPI_ULONG64;
typedef unsigned long BRCM_CAPI_ULONG32;

typedef unsigned long BRCM_CAPI_DWORD;
typedef BRCM_CAPI_DWORD *BRCM_CAPI_PDWORD;
typedef BRCM_CAPI_DWORD *BRCM_CAPI_LPDWORD;

typedef float BRCM_CAPI_FLOAT;

typedef BRCM_CAPI_UCHAR BRCM_CAPI_BOOLEAN,BRCM_CAPI_BOOL;
typedef double BRCM_CAPI_DOUBLE;

typedef signed char		BRCM_CAPI_INT8;
typedef unsigned char	BRCM_CAPI_UINT8;
typedef signed short	BRCM_CAPI_INT16;
typedef unsigned short	BRCM_CAPI_UINT16;
typedef unsigned int	BRCM_CAPI_UINT32;
typedef signed int	BRCM_CAPI_SINT32;
typedef signed char		BRCM_CAPI_SINT8;

#pragma pack (push,1)

#define HEADER_LENGTH_PER_PACKET 4

#define TYPE_MASK 0x000000FF  //To get Type ID

typedef enum {
    eTYPE_UCHAR = 1,		// BRCM_CAPI_UCHAR buffer
    eTYPE_PUCHAR,			// Pointer to BRCM_CAPI_UCHAR buffer
    eTYPE_CHAR,
    eTYPE_PCHAR,
    eTYPE_USHORT,
    eTYPE_PUSHORT,
    eTYPE_SSHORT,
    eTYPE_PSSHORT,
    eTYPE_UINT,
    eTYPE_PUINT,
    eTYPE_SINT,
    eTYPE_PSINT,
    eTYPE_ULONG_LONG,
    eTYPE_PULONG_LONG,
    eTYPE_SLONG_LONG,
    eTYPE_PSLONG_LONG,
    eTYPE_DOUBLE,
    eMAX_NUM_OF_TYPES
} E_TYPE_DEFNS, *P_E_TYPE_DEFNS;


//TLV interface with the application
typedef struct ST_TLV {
    BRCM_CAPI_UINT32 uiType;
    BRCM_CAPI_UINT32 uiLength;
    BRCM_CAPI_UCHAR uchaValue[1];
} ST_TLV, *P_ST_TLV;

#ifdef QTPROJECT
typedef unsigned long *PULONG;

typedef unsigned long long ULONG64;
//typedef unsigned long ULONG32;

typedef unsigned long DWORD;
typedef DWORD *PDWORD;
typedef DWORD *LPDWORD;

typedef float FLOAT;

//typedef UCHAR BOOLEAN,BOOL;
typedef double DOUBLE;

typedef signed char		INT8;
typedef unsigned char	UINT8;
typedef signed short	INT16;
typedef unsigned short	UINT16;
typedef unsigned int	UINT32;
#endif

//packet buffer to 4G-APIMgr.
typedef struct ST_SEND_BUF {
    BRCM_CAPI_UINT8 ui8Header[HEADER_LENGTH_PER_PACKET];
    BRCM_CAPI_UINT32 uiLength;
    BRCM_CAPI_UINT32 uiCommandID;
    BRCM_CAPI_UINT32 uiNumTLVs;
    ST_TLV astTLVBuffer[1];
} ST_SEND_BUF, *P_ST_SEND_BUF;

//packet buffer from 4G-APIMgr. -for internal use
typedef struct ST_RCV_BUF {
    BRCM_CAPI_UINT8 ui8Header[HEADER_LENGTH_PER_PACKET];
    BRCM_CAPI_UINT32 uiLength;
    BRCM_CAPI_UINT32 uiCommandID;
    BRCM_CAPI_INT32 iStatus;
    BRCM_CAPI_UINT32 uiNumTLVs;
    ST_TLV astTLVBuffer[1];
} ST_RCV_BUF, *P_ST_RCV_BUF;



#pragma pack(pop)


/* type defns of CAPI : END*/

/*****************************************************************************/
/*						structures and #defines for LTE stack		    	 */
/*****************************************************************************/

/* type defns LTE Section: START*/
#define CAPI_STATUS_OK              0
#define CAPI_STATUS_ERR             1

//TBD: All the values here need to be checked
#define MAX_FREE_TEXT                       100     
#define MAX_NUM_PLMN                        10
#define MAX_PLMN_ID_LEN                     16
#define MAX_NO_OF_CIDS                        10
#define    MAX_APN_SIZE                        50
#define    MAX_NW_SIZE                         50
#define    MAX_USBEP_SIZE                      50
#define    MAX_ADDRESS_SIZE                    140
#define    MAX_NUM_APN                         10
#define    MAX_PDP_ADDRESS_SIZE                140
#define MAX_PLMN_ID                            10 
#define MAX_GW_ADDRESS_SIZE                    140
#define MAX_DNS_PRIMARY_ADDRESS_SIZE        140
#define MAX_DNS_SECONDARY_ADDRESS_SIZE        140
#define MAX_PCSCF_PRIMARY_ADDRESS_SIZE        140
#define MAX_PCSCF_SECONDARY_ADDRESS_SIZE    140
#define MAX_SRC_ADDRESS_SIZE                140
#define MAX_PORT_RANGE                        12
#define MAX_SECURITY_PARAMETER_INDEX_RANGE    8
#define MAX_TRAFFIC_CLASS_AND_MASK_RANGE    8
#define MAX_TOS_SIZE                        10 //check
#define MAX_LOG_CMD_SIZE                    64 //check
#define MAX_LOG_BUFFER_SIZE                    2000 //check


//capi response status
typedef BRCM_CAPI_UINT32 CAPI_REQ_STATUS;
typedef BRCM_CAPI_UINT32 CAPI_RSP_STATUS;

//enums
typedef BRCM_CAPI_UINT32 T_PLMN_ID_FORMAT;
typedef BRCM_CAPI_UINT32 T_RAT;
typedef BRCM_CAPI_UINT32 T_LIST_MEMBERSHIP;
typedef BRCM_CAPI_UINT32 T_PLMN_SELECT_MODE;
typedef BRCM_CAPI_UINT32 T_PS_CTX_CFG_TYPE;
typedef BRCM_CAPI_UINT32 T_REG_INFO;
typedef BRCM_CAPI_UINT32 T_PS_CTX_INFO_TYPE;
typedef BRCM_CAPI_UINT32 T_PS_PDP_TYPE;
typedef BRCM_CAPI_UINT32 T_IP_IF_STATUS;

//T_PLMN_ID_FORMAT
#define E_PLMN_ID_LONG_ALPHA                0
#define E_PLMN_ID_SHORT_ALPHA               1
#define E_PLMN_ID_NUMERIC                   2

//T_RAT
#define E_RAT_GSM                           0
#define E_RAT_GSM_COMPACT                   1
#define E_RAT_UTRAN                         2
#define E_RAT_EGPRS                         3
#define E_RAT_HSDPA                         4
#define E_RAT_HSUPA                         5
#define E_RAT_UTRAN_HSDPA_HSUPA             6
#define E_RAT_EUTRAN                        7

//T_REG_INFO
#define E_NOT_REG                           0
#define E_REG_HOME                          1
#define E_SEARCH                            2
#define E_DENIED                            3
#define E_UNKNOWN                           4
#define E_REG_ROAM                          5


//T_LIST_MEMBERSHIP
#define E_EHPLMN                            0
#define E_FPLMN                             1
#define E_UHPLMN                            2
#define E_OHPLMN                            3
#define E_NONE                              4
          

//T_PLMN_SELECT_MODE
#define E_PLMN_SELECT_AUTO                  0
#define E_PLMN_SELECT_MANUAL                1
#define E_PLMN_SELECT_MANUAL_AUTO           2


//T_PLMN_ID_FORMAT
#define E_PLMN_ID_LONG_ALPHA                0
#define E_PLMN_ID_SHORT_ALPHA               1
#define E_PLMN_ID_NUMERIC                   2


//T_PS_PDP_TYPE
#define E_PDP_TYPE_IPV4                        1
#define    E_PDP_TYPE_IPV6                        2
#define    E_PDP_TYPE_IPV4V6                    3
#define    E_PDP_TYPE_UNKNOWN                    4


//T_PS_DATA_COMP
#define    E_DATA_COMP_V42                        0
#define    E_DATA_COMP_MANUF                    1
#define    E_DATA_COMP_NONE                    2

//T_PS_HDR_COMP
#define    E_HDR_COMP_RFC1144                  0
#define    E_HDR_COMP_RFC2507                    1
#define    E_HDR_COMP_ROHC                        2    //RFC3095
#define    E_HDR_COMP_MANUF                    3
#define    E_HDR_COMP_NONE                        4


//T_PS_CTX_CFG_TYPE
#define E_CFG_QOS_2G                    1
#define E_CFG_MIN_QOS_2G                    2
#define E_CFG_REQ_QOS_3G                    3
#define E_CFG_MIN_QOS_3G                    4
#define E_CFG_QOS_4G                        5
#define E_CFG_TFT                       6
#define E_CFG_PCO_MS                        7

//T_PS_CTX_INFO_TYPE
#define    E_INFO_NEG_QOS_2G                    0
#define    E_INFO_NEG_QOS_3G                    1
#define    E_INFO_NEG_QOS_4G                    2
#define    E_INFO_ACTIVATION_STATUS            3
#define    E_INFO_BYTES_RCVED                    4
#define    E_INFO_BYTES_SENT                    5
#define    E_INFO_TOTAL_BYTES_RCVED            6
#define    E_INFO_TOTAL_BYTES_SENT                7
#define    E_INFO_DYN_DNS                        8
#define    E_PDP_ADDRESS_TYPE                    9
#define    E_INFO_PDP_ADDRESS                    10
#define    E_INFO_APN_NAME                        11
#define    E_INFO_PRIMARY_BEARER_ID               12
#define E_INFO_SECONDARY_BEARER_ID            13
#define    E_INFO_SUBNET_MASK                    14
#define    E_INFO_GW_ADDRESS                    15
#define E_INFO_DYN_P_CSCF                    16
#define    E_INFO_TFT                            17

//T_IP_IF_STATUS
#define E_IP_IF_STATUS_UP                        1
#define E_IP_IF_STATUS_DOWN                      2
#define E_IP_IF_STATUS_FAILURE                   3 



typedef enum T_NET_CFG {
	E_CFG_ACTIVATED_RAT = 0,
	E_CFG_OPER_CLASS = 1,
	E_PDP_TYPE = 2,
	E_NS_TYPE = 3
}T_NET_CFG;



typedef struct
{
    BRCM_CAPI_UINT32 rssi;
}T_GET_RSSI_RSP;

typedef struct
{
    T_PLMN_ID_FORMAT plmnFormat;
    BRCM_CAPI_SINT8  plmn_id[MAX_PLMN_ID_LEN + 1];
}T_PLMN;

typedef struct
{
    BRCM_CAPI_UINT32 plmn_search_timer;
}T_NET_SEARCH_REQ;

typedef struct
{
    BRCM_CAPI_UINT32 u32Bitfields;
    T_PLMN PLMN_id;
    T_RAT rat;
    T_LIST_MEMBERSHIP list_membership;
    BRCM_CAPI_SINT8 free_text[MAX_FREE_TEXT];
}T_PLMN_SEARCH_RESULT_ENTRY;

typedef struct
{
    BRCM_CAPI_UINT32 PLMN_count;
    T_PLMN_SEARCH_RESULT_ENTRY plmn_search_entry[MAX_NUM_PLMN];
}T_PLMN_SEARCH_RESULT_LIST;

typedef struct
{
    T_PLMN_SEARCH_RESULT_LIST result_list;
}T_NET_SEARCH_RSP;

typedef struct
{
    BRCM_CAPI_UINT32 u32Bitfields;
    T_PLMN_SELECT_MODE plmn_select_mode;
    T_PLMN plmn;
    T_RAT rat;
    BRCM_CAPI_UINT32 primary_ctx_id;
}T_NET_ATTACH_REQ;

typedef struct
{
    T_RAT rat;
}T_NET_STATUS_GET_REQ;

typedef struct
{
    BRCM_CAPI_UINT32 net_status;
}T_NET_STATUS_GET_RSP;

typedef struct
{
    T_RAT rat;
}T_NET_SIG_QUAL_GET_REQ;

typedef struct
{
    BRCM_CAPI_UINT32 rssi;
    BRCM_CAPI_UINT32 ber;
}T_NET_SIG_QUAL_GET_RSP;



typedef struct
{
    BRCM_CAPI_UINT32 u32Bitfields;
    T_PS_PDP_TYPE  pdp_type;
    BRCM_CAPI_SINT8    apn[MAX_APN_SIZE];
    BRCM_CAPI_SINT8    pdp_addr[MAX_PDP_ADDRESS_SIZE];
    BRCM_CAPI_UINT32    data_comp;
    BRCM_CAPI_UINT32    hdr_comp;
}T_PS_CTX_PRIMARY_PARAMS;

typedef struct
{
    BRCM_CAPI_UINT32                         ctx_id;
    T_PS_CTX_PRIMARY_PARAMS     primaryParams;
}T_PS_PRIMARY_INFO;


typedef struct
{
    BRCM_CAPI_UINT32 u32Bitfields;
    BRCM_CAPI_UINT32    data_comp;
    BRCM_CAPI_UINT32    hdr_comp;
}T_PS_CTX_SECONDARY_PARAMS;


typedef struct
{
    BRCM_CAPI_UINT32                         ctx_id;
    BRCM_CAPI_UINT32                         primary_ctx_id;
    T_PS_CTX_SECONDARY_PARAMS   secondaryParams;
}T_PS_SECONDARY_INFO;

typedef struct
{
    BRCM_CAPI_UINT32                  primaryInfoCount;
    T_PS_PRIMARY_INFO       primaryInfo[10];
    BRCM_CAPI_UINT32                  secondaryInfoCount;
    T_PS_SECONDARY_INFO   secondaryInfo[10];
}T_PS_INFO;

typedef struct
{
    T_PS_INFO ps_info;
}T_PS_DFN_INFO_GET_RSP;

typedef struct
{
    T_PS_PRIMARY_INFO ps_primary_info;
}T_PS_CTX_DEFINE_PRIMARY_REQ;

typedef struct
{
    T_PS_SECONDARY_INFO ps_secondary_info;
}T_PS_CTX_DEFINE_SECONDARY_REQ;

typedef struct
{
    BRCM_CAPI_UINT32 ctx_id;
}T_PS_CTX_UNDEFINE_PRIMARY_REQ;

typedef struct
{
    BRCM_CAPI_UINT32 ctx_id;
}T_PS_CTX_UNDEFINE_SECONDARY_REQ;

typedef struct
{
    BRCM_CAPI_UINT32 u32Bitfields;
    BRCM_CAPI_UINT32 qci;
    BRCM_CAPI_UINT32 dl_gbr;
    BRCM_CAPI_UINT32 ul_gbr;
    BRCM_CAPI_UINT32 dl_mbr;
    BRCM_CAPI_UINT32 ul_mbr;
}T_PS_CTX_QOS_4G;

typedef struct
{
    BRCM_CAPI_UINT32 u32Bitfields;
    BRCM_CAPI_UINT32 packet_filter_id;
    BRCM_CAPI_UINT32 eval_prec_index;
    BRCM_CAPI_SINT8 source_addr[MAX_ADDRESS_SIZE];
    BRCM_CAPI_UINT32 protocol_number;
    BRCM_CAPI_SINT8 dest_port[MAX_PORT_RANGE];
    BRCM_CAPI_SINT8 source_port[MAX_PORT_RANGE];
    BRCM_CAPI_UINT32  spi;
    BRCM_CAPI_SINT8 tos[MAX_TOS_SIZE];
    BRCM_CAPI_UINT32 flow_label;
    BRCM_CAPI_UINT32  direction;
    BRCM_CAPI_UINT32  NW_packet_filter_id;
}T_TFT;

typedef union
{
    T_PS_CTX_QOS_4G ps_ctx_qos_4g;
    T_TFT tft;
}T_PS_CTX_CFG;

typedef struct
{
    BRCM_CAPI_UINT32 ctx_id;
    T_PS_CTX_CFG_TYPE cfg_type;
}T_PS_CTX_CFG_GET_REQ;

typedef struct
{
    BRCM_CAPI_UINT32 u32Bitfields;
    T_PS_CTX_CFG ctx_cfg;
}T_PS_CTX_CFG_GET_RSP;

typedef struct
{
    BRCM_CAPI_UINT32 u32Bitfields;
    BRCM_CAPI_UINT32 ctx_id;
    T_PS_CTX_CFG_TYPE cfg_type;
    T_PS_CTX_CFG ctx_cfg;
}T_PS_CTX_CFG_SET_REQ;

typedef struct
{
    BRCM_CAPI_UINT32 ctx_id;
    T_PS_CTX_INFO_TYPE info_type;
}T_PS_CTX_INFO_GET_REQ;

typedef union
{
    T_PS_CTX_QOS_4G ps_ctx_qos_4g;
    BRCM_CAPI_UINT32 info_activation_status;
    BRCM_CAPI_UINT32 bytes_received;
    BRCM_CAPI_UINT32 bytes_sent;

    BRCM_CAPI_UINT32 total_bytes_received;
    BRCM_CAPI_UINT32 total_bytes_sent;
    BRCM_CAPI_SINT8 dyn_dns[MAX_DNS_PRIMARY_ADDRESS_SIZE];
    T_PS_PDP_TYPE ps_pdp_type;
    BRCM_CAPI_SINT8    pdp_addr[MAX_PDP_ADDRESS_SIZE];
    BRCM_CAPI_SINT8    apn[MAX_APN_SIZE];
    BRCM_CAPI_UINT32 prim_bearer_id;
    BRCM_CAPI_SINT8 subnet_mask[MAX_ADDRESS_SIZE];
    BRCM_CAPI_SINT8 gw_addr[MAX_ADDRESS_SIZE];
    BRCM_CAPI_SINT8 P_CSCF_prim_addr[MAX_ADDRESS_SIZE];
    T_TFT tft;
    BRCM_CAPI_UINT32 sec_bearer_id;
}T_PS_CTX_INFO;

typedef struct
{
    BRCM_CAPI_UINT32 u32Bitfields;
    T_PS_CTX_INFO ps_ctx_info;
}T_PS_CTX_INFO_GET_RSP;

typedef struct
{
    BRCM_CAPI_UINT32 ctx_id;
}T_PS_CTX_ACTIVATE_REQ;

typedef struct
{
    BRCM_CAPI_UINT32 ctx_id;
}T_PS_CTX_MODIFY_REQ;

typedef struct
{
    BRCM_CAPI_UINT32 ctx_id;
}T_PS_CTX_DEACTIVATE_REQ;

typedef struct
{
    T_REG_INFO reg_info;
}T_EVT_LTE_REG_STATUS;

typedef struct
{
    BRCM_CAPI_UINT32 ctx_id;
}T_EVT_PRIMARY_ACT;

typedef struct
{
    BRCM_CAPI_UINT32 ctx_id;
}T_EVT_SECONDARY_ACT;

typedef struct
{
    BRCM_CAPI_UINT32 ctx_id;
}T_EVT_PRIMARY_DEACT;

typedef struct
{
    BRCM_CAPI_UINT32 ctx_id;
}T_EVT_SECONDARY_DEACT;

typedef struct
{
    BRCM_CAPI_UINT32 ctx_id;
}T_EVT_CTX_MODIFY;

typedef struct
{
    BRCM_CAPI_UINT32 u32Bitfields;
    BRCM_CAPI_SINT8 apn[MAX_APN_SIZE];
    T_PS_PDP_TYPE apn_type;
    BRCM_CAPI_UINT32 cid;
    BRCM_CAPI_UINT32 APN_id;
    BRCM_CAPI_SINT8 network_interface[MAX_NW_SIZE];
    BRCM_CAPI_SINT8 usb_endpoint[MAX_USBEP_SIZE];
    BRCM_CAPI_SINT8 source_addr[MAX_ADDRESS_SIZE];
}T_APN_TABLE_ENTRY;

typedef struct
{
    BRCM_CAPI_UINT32 u32ApnTableEntryCount;
    T_APN_TABLE_ENTRY apn_table_entry[MAX_NUM_APN];
}T_APN_TABLE;

typedef struct
{
    T_APN_TABLE apn_table;
}T_APN_TABLE_GET_RSP;

typedef struct
{
    T_APN_TABLE apn_table;
}T_APN_TABLE_SET_REQ;

typedef struct
{
    T_APN_TABLE_ENTRY apn_entry;
}T_APN_ENTRY_SET_REQ;


typedef struct
{
    BRCM_CAPI_SINT8 *pBuffer;
}T_ENCAPSULATED_AT_REQ;

typedef struct
{
    BRCM_CAPI_SINT8 *pBuffer;
}T_ENCAPSULATED_AT_RSP;


#define MAX_BUFFER_FOR_LOGCMD 50
typedef struct
{
    BRCM_CAPI_UINT8 bPending;
    BRCM_CAPI_UINT16 u16BufLen;
    BRCM_CAPI_UINT16 u16BufLenRead;	
    BRCM_CAPI_SINT8 pBuffer[MAX_BUFFER_FOR_LOGCMD];
}T_LOG_4G_CMD_REQ;

typedef struct
{
    BRCM_CAPI_UINT32 buf_len;
    BRCM_CAPI_SINT8 *pBuffer;
}T_LOG_4G_BUFFER_RSP;




/***********************************************************************/
/* Statistics structure for the connection manager					   */
/***********************************************************************/

#define CAPI_SUB_CMD_CM_STATS      10


/// \brief DL TB stats
typedef struct stDLTBStats {
    /// \brief  Number of DL TBs
    BRCM_CAPI_UINT32 u32NumTBs;
    /// \brief  Number of TBs in error
    BRCM_CAPI_UINT32 u32NumErrTBs;
    /// \brief  Pre-HARQ error rates
    BRCM_CAPI_UINT32 u32PreHarqPercentageErrRate;
    /// \brief  Post-HARQ error rates
    BRCM_CAPI_UINT32 u32PostHarqPercentageErrRate;
}stDLTBStats;

/// \brief UL TB stats
typedef struct stULTBStats {
    /// \brief Number of UL TBs
    BRCM_CAPI_UINT32 u32NumTBs;
    /// \brief Number of TBs in error
    BRCM_CAPI_UINT32 u32NumReTxTBs;  
}stULTBStats;



typedef struct {
    BRCM_CAPI_UINT32 u32PhyIdCell;
    BRCM_CAPI_UINT32 u32DLEARFCN;
    BRCM_CAPI_UINT32 u32DLBw;
    BRCM_CAPI_UINT32 u32ULEARFCN;
    BRCM_CAPI_UINT32 u32ULBw;
    BRCM_CAPI_UINT32 u32BandType;
    BRCM_CAPI_UINT32 u32NumPorts;
    BRCM_CAPI_SINT32 s32RSRP;
    BRCM_CAPI_SINT32 s32RSRQ;
    BRCM_CAPI_SINT32 s32RssiAnt0;
    BRCM_CAPI_SINT32 s32RssiAnt1;
    BRCM_CAPI_SINT32 s32UlTxPower;
    BRCM_CAPI_UINT32 u32NumDCIs;
    BRCM_CAPI_UINT32 u32TxMode;
    BRCM_CAPI_SINT32 s32DeviceCinr;
    
}stCMPHYStats_t;

typedef struct {
    /// \brief LMAC stats
    BRCM_CAPI_UINT32 u32PLMNId;
    BRCM_CAPI_UINT32 u32FrameNumber;
    BRCM_CAPI_UINT32 u32NumSRs;
    BRCM_CAPI_UINT32 u32NumPUCCHTransmissions;
    BRCM_CAPI_UINT32 u32NumPRACH;
    BRCM_CAPI_UINT32 u32CRNTI;
    BRCM_CAPI_UINT32 u32NumDRBs;
    stDLTBStats sDLTBStats;
    stULTBStats sULTBStats;

    /// \brief UE stats
    BRCM_CAPI_UINT32 u32RASuccessCount;
    BRCM_CAPI_UINT32 u32RAFailureCount;
    BRCM_CAPI_UINT32 u32HandoverCount;
    BRCM_CAPI_UINT32 u32IntegrityFailureCount;
    BRCM_CAPI_UINT32 u32SuccessfulSecurityActivationCount;
    BRCM_CAPI_UINT32 u32FailedSecurityActivationCount;
    BRCM_CAPI_UINT32 u32CellReselectionCount;
    BRCM_CAPI_UINT32 u32OutOfCoverageCount;
    BRCM_CAPI_UINT32 u32MeasReportsCount;
    BRCM_CAPI_UINT32 u32NumMacPdusRx;
    BRCM_CAPI_UINT32 u32NumMacPdusTx;
}stCMMACStats_t;

typedef struct  {

    BRCM_CAPI_UINT32 u32ModemState;
    BRCM_CAPI_UINT32 u32DLThruput;
    BRCM_CAPI_UINT32 u32ULThruput;
    BRCM_CAPI_UINT32 u32NumPacketsRx;
    BRCM_CAPI_UINT32 u32NumPacketsTx;
    BRCM_CAPI_UINT32 u32NumOfResync;
    BRCM_CAPI_UINT32 u32NumOfConnects;
}stCMGenericStats_t;

typedef struct {
	BRCM_CAPI_UINT uiTotalStatsLength;
	BRCM_CAPI_UINT uiGenericStatsLength;	   	
    stCMGenericStats_t sGenericStats;
	BRCM_CAPI_UINT uiPHYStatsLength;                
    stCMPHYStats_t sPHYStats;
	BRCM_CAPI_UINT uiMacstatsLength;
    stCMMACStats_t sMacStats;
}stCMStats_t;

/* Modem states */
typedef enum
{
	MODEM_STATE_WAIT_FOR_MEASURE_BAND = 0,
	MODEM_STATE_WAIT_FOR_SEARCH_CELL,
	MODEM_STATE_WAIT_FOR_DECODE_BCH,
	MODEM_STATE_WAIT_FOR_CAMP_CELL,
	MODEM_STATE_CELL_CAMPED,
	MODEM_STATE_IDLE,
	MODEM_STATE_CONNECTED
} E_MODEM_STATES;



typedef struct
{
	BRCM_CAPI_UINT32	u32RFICVer;
	BRCM_CAPI_UINT32	u32ChipVer;
	BRCM_CAPI_UINT32	u32FlasMapVer;
	BRCM_CAPI_UINT32	u32DSDVer;
	BRCM_CAPI_UINT32	u32CAPIVer[3];
	BRCM_CAPI_UINT32	u32RSAVer[3];
	BRCM_CAPI_UINT32	u32SySCntrlCPU0Ver[3]; 
	BRCM_CAPI_UINT32	u32FWVer[3];
	BRCM_CAPI_UINT32	u32DSPPhyVer[3];
	BRCM_CAPI_UINT32	u32UbootVer[3];

}ST_COMPONENT_VERSION_INFO_4G_CAPI,*PST_COMPONENT_VERSION_INFO_4G_CAPI;



/* type defns LTE Section: END*/

/* type defn for DSD functions */
#define MAX_NUM_DSD_SECTION 32
#define MAX_TAG_DEPTH		10


typedef struct {

	BRCM_CAPI_UINT uiNumOfDSDSections;
	BRCM_CAPI_UINT uiDSDSectionValidBitField;
	BRCM_CAPI_UINT uiSectionIDs[MAX_NUM_DSD_SECTION];
}stAreDSDChecksumsValidResponse_t;

typedef struct {
	
	BRCM_CAPI_UINT uiNumOfDSDs;
	BRCM_CAPI_UINT uiNumOfSections;
	BRCM_CAPI_UINT uiDSDSize;
	BRCM_CAPI_UINT uiDSDTotalSize;	
	BRCM_CAPI_UINT uiNVMSize;	
	BRCM_CAPI_UINT uiSectionIDs[MAX_NUM_DSD_SECTION];
	BRCM_CAPI_UINT uiCheckSum[MAX_NUM_DSD_SECTION];

}stGetDSDinfo_Response_t;




typedef struct {
	
	BRCM_CAPI_UINT uiID;
	BRCM_CAPI_UINT uiLockCode;	
	BRCM_CAPI_UINT uiOffset;
	BRCM_CAPI_UINT uiSize;
}stRead_DSD_Field_Request_t;	

typedef struct {
	
	BRCM_CAPI_UINT uiID;
	BRCM_CAPI_UINT uiLockCode;	
	BRCM_CAPI_UINT uiOffset;
	BRCM_CAPI_UINT uiSize;
	BRCM_CAPI_UINT uiWriteFlag;
}stWrite_DSD_Field_Request_t;	



typedef struct {

	BRCM_CAPI_UINT uiID;
	BRCM_CAPI_UINT uiLockCode;	
	BRCM_CAPI_UINT uiFlag;	
	BRCM_CAPI_UINT uiDepth;
	BRCM_CAPI_UINT uiTAGTree[MAX_TAG_DEPTH];	
}stRead_DSD_TLV_Request_t;	



typedef struct {

	BRCM_CAPI_UINT uiID;
	BRCM_CAPI_UINT uiLockCode;	
	BRCM_CAPI_UINT uiFlag;	
	BRCM_CAPI_UINT uiWriteBytes;	
	BRCM_CAPI_UINT uiDepth;
	BRCM_CAPI_UINT uiTAGTree[MAX_TAG_DEPTH];	
}stWrite_DSD_TLV_Request_t;	


typedef struct {
	
	BRCM_CAPI_UINT uiAddress;
	BRCM_CAPI_UINT uiSize;
}stRead_DSD_Raw_t;

typedef struct {
	
	BRCM_CAPI_UINT uiAddress;
	BRCM_CAPI_UINT uiSize;
	BRCM_CAPI_UINT u32WriteFlags;
}stWrite_DSD_Raw_t;


typedef struct {

	BRCM_CAPI_UINT uiNumOfSections;
	BRCM_CAPI_UINT uiSectionIDs[MAX_NUM_DSD_SECTION];
	BRCM_CAPI_UINT uiCheckSum[MAX_NUM_DSD_SECTION];

}stGetDSD_CheckSums_Response_t;	




typedef struct {

	BRCM_CAPI_UINT uiSRCSection;
	BRCM_CAPI_UINT uiDestSection;
	BRCM_CAPI_UINT uiOffset;
	BRCM_CAPI_UINT uiNumOfBytes;

}stCopyDSDSection_Request_t;
	


typedef struct {

	BRCM_CAPI_UINT uiID;
	BRCM_CAPI_UINT uiLockCode;	
	BRCM_CAPI_UINT uiDepth;
	BRCM_CAPI_UINT uiTAGTree[MAX_TAG_DEPTH];	

}stGetDSD_Length_Request_t;

typedef struct {
	BRCM_CAPI_UINT uiLength;
}stGetDSD_Length_Response_t;


typedef struct {
	
	BRCM_CAPI_UINT uiSectionId;
	BRCM_CAPI_UINT uiLockCode;
}stLock_Unlock_Read_Write_Request_t;

typedef struct {

	BRCM_CAPI_UINT uiSectionId;
	BRCM_CAPI_UINT uiLockStatus;
	BRCM_CAPI_UINT uiReadLockCode; 
	BRCM_CAPI_UINT uiWriteLockCode;
	BRCM_CAPI_UINT u32WriteFlags;
}stDSD_AddSection_Request_t;

typedef struct {
	BRCM_CAPI_CHAR cIndex;
	BRCM_CAPI_CHAR cMacAddress[6];
}stDSD_MacAddress_t;


/*****************************************************************************/
/*								Errordefines							     */
/*****************************************************************************/


/* Error defines in  CAPI for Applications: START */
typedef enum E_ERROR_DEFNS {
    ERROR_CODE_SUCCESS = 0,
    ERROR_CODE_MEMORY_ALLOCATION_FALIED,
    ERROR_CODE_INVALID_HANDLE,
    ERROR_CODE_INVALID_PARAM,
    ERROR_CODE_INVALID_TYPE,
    ERROR_CODE_CONNECTION_FAILED,
    ERROR_CODE_CALL_RESPONSE_TIMEOUT,
    ERROR_CODE_TYPE_RCVD_INVALID,
    ERROR_CODE_LENGTH_RCVD_INVALID,
    ERROR_CODE_TLV_COUNT_DIDNOT_MATCH,
    ERROR_CODE_TLV_LENGTH_MISMATCH_WITH_TOTAL_LENGTH,
    ERROR_CODE_API_EXEC_FAILED,
    ERROR_CODE_API_DISCONNECT_FAILED,
	ERROR_CODE_DSD_INIT_NOT_DONE ,
    ERROR_CODE_DSD_SECTION_ID_INVALID ,
    ERROR_CODE_DSD_REQUESTED_SIZE_INVALID ,
    ERROR_CODE_DSD_INVLAID_BUFFER ,
    ERROR_CODE_DSD_REQSTED_ADDR_INVALID ,
    ERROR_CODE_DSD_NO_TLV_AREA_DEFINED_IN_SECTION ,
    ERROR_CODE_DSD_INVALID_TAGS ,
    ERROR_CODE_DSD_NOT_ENOUGH_DSD_AREA ,
    ERROR_CODE_SRC_DSD_NOT_PRESENT ,
    ERROR_CODE_DST_DSD_NOT_PRESENT,
    ERROR_CODE_DSD_REQST_NOT_SUPRTD_IN_NVM,
    ERROR_CODE_DSD_INVALID_LOCK_CODE,
    ERROR_CODE_DSD_INVALID_OPRTN_IN_RO_SECTION ,
    ERROR_CODE_DSD_INVALID_MAC_ADDR_INDEX,
    ERROR_CODE_DSD_ALREADY_LOCKED,
    ERROR_CODE_DSD_ALREADY_UNLOCKED,
    ERROR_CODE_DSD_INVALID_SECTION_CHECKSUM        
} E_ERROR_DEFNS, *P_E_ERROR_DEFNS;
/* Error defines in  CAPI for Applications: END */

/* Error defines in LTE Section : START */
/* Error defines in LTE Section : END*/


/*****************************************************************************/
/*								Debugging support through CAPI					     */
/*****************************************************************************/


/* enum _DBG_MODULE maps each of the modules to a particular value. 
Each module need to create an entry in the enum DBG_MODULE. 
The entry should be created before the DBG_MAX_MODULE.*/

typedef enum _DBG_MODULE_TYPE{
            DBG_CMN_DRVR = 0,
            DBG_SDIO_FN_DRVR,
            DBG_SDIO_CPD_DRVR,
            DBG_SDIO_CONTROLLER,
            DBG_MAX_MODULE
}DBG_MODULE_TYPE;

/*CAPI need to send the the structure _S_DBG_MODULE_INFO as a single TLV with three integer values.
1) dbg_module_type, 2) debug_level, 3) reserved*/

typedef struct _S_DBG_MODULE_INFO{
	DBG_MODULE_TYPE dbg_module_type;
	int 			debug_level;
	int 			reserved;
}S_DBG_MODULE_INFO,*PS_DBG_MODULE_INFO; 

#endif