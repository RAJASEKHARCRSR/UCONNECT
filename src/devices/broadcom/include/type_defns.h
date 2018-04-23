
/*________________________________________________________________________
 * This file contains structs,enums,#defines and any other definitions required for
 * 4G-CAPI usage by application.
 * _______________________________________________________________________*/
#ifndef BECM_4G_CAPI_TYPE_DEFNS
#define BECM_4G_CAPI_TYPE_DEFNS
//#include "osal_basic_types.h"
#include <stdio.h>
#include <string.h>
#ifndef WIN32
	#include <stdint.h>
	#include <stdlib.h>
#endif
#pragma pack (push,1)
//
#define IN
#define OUT
#define IN_OUT
//
#define HEADER_LENGTH_PER_PACKET 4

typedef enum {
    eTYPE_UCHAR = 1,		// UCHAR buffer
    eTYPE_PUCHAR,		//Pointer to UCHAR buffer
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
    eTYPE_ULONG,
    eTYPE_PULONG,
    eTYPE_SLONG,
    eTYPE_PSLONG,
    eTYPE_ULONG_LONG,
    eTYPE_PULONG_LONG,
    eTYPE_SLONG_LONG,
    eTYPE_PSLONG_LONG,
    eMAX_NUM_OF_TYPES
} E_TYPE_DEFNS, *P_E_TYPE_DEFNS;


typedef intptr_t MEMORY_ADDRESS_TYPE;
typedef enum {
    eLocal =1,
    eRemote
} E_OPERATING_MODE, *P_E_OPERATING_MODE;

/**
connection parameters - Valid for 
*/
typedef struct ST_CONNECT_PARAMS {
    CHAR szIpAddress[128];
			 /**<IP address */
    UINT32 u32Port;
		/**< Port number*/
} ST_CONNECT_PARAMS, *P_ST_CONNECT_PARAMS;
typedef struct ST_OPEN_PARAM {
    E_OPERATING_MODE eOperatingMode;
    ST_CONNECT_PARAMS stConnectParams;
} ST_OPEN_PARAM, *P_ST_OPEN_PARAM;

//TLV interface with the application
typedef struct ST_TLV {
    UINT32 uiType;
    UINT32 uiLength;
    UCHAR uchaValue[1];
} ST_TLV, *P_ST_TLV;

//packet buffer to 4G-APIMgr.
typedef struct ST_SEND_BUF {
    UINT8 ui8Header[HEADER_LENGTH_PER_PACKET];
    UINT32 uiLength;
    UINT32 uiCommandID;
    UINT32 uiNumTLVs;
    ST_TLV astTLVBuffer[1];
} ST_SEND_BUF, *P_ST_SEND_BUF;

//packet buffer from 4G-APIMgr. -for internal use
typedef struct ST_RCV_BUF {
    UINT8 ui8Header[HEADER_LENGTH_PER_PACKET];
    UINT32 uiLength;
    UINT32 uiCommandID;
    INT32 iStatus;
    UINT32 uiNumTLVs;
    ST_TLV astTLVBuffer[1];
} ST_RCV_BUF, *P_ST_RCV_BUF;

//Callback buffer to give to application
typedef struct ST_CALLBACK_INFO {
    UINT32 uicmd_id;
    INT32 iStatus;
    UINT32 uinum_tlvs;
    ST_TLV pTLVBuf[1];
} *P_ST_CALLBACK_INFO;

typedef struct ST_4G_API_PKT
{
    UINT32 uiNumTLVs;
    UINT32 uiTLVTotalLength;
    ST_TLV astTLVBuffer[1];
}ST_4G_API_PKT,*P_ST_4G_API_PKT;

typedef struct ST_API_SEND_DATA
{
    UINT uiLength; //for pointers it would be T+L+Address if data(array/contigouous) it is T+L+Value
    P_ST_TLV pstTLVPkt;
}ST_API_SEND_DATA,*P_ST_API_SEND_DATA;

    
typedef VOID(*P_FN_CALLBACK) (UINT uiLength, P_ST_CALLBACK_INFO pCallbackbuf);


//Macros to be moved:

#define ALLOCATE_MEMORY(ptr,data_type,usize,uoldsize) do\
			{\
			    UINT uiMemSet = (ptr == NULL)? TRUE:FALSE;\
			    ptr=(data_type *)realloc(ptr,(size_t)usize);\
			    if(!ptr)\
                 {\
                    printf("\n realloc error:%s.%d: " ,__FUNCTION__, __LINE__);\
                 }\
                else if(uiMemSet)\
                {\
                memset(ptr,0,usize);\
                }\
                else if(uoldsize < usize)\
                {\
                    PUCHAR puchNewMem = (PUCHAR )ptr + uoldsize;\
                    memset(puchNewMem,0,usize - uoldsize);\
                }\
			}\
			while(0)


#define COPY_MEMORY(ptr_dest,ptr_src,length) do\
			{\
			    if(!ptr_dest)\
                 {\
                    printf("\n dest ptr is NULL @:%s.%d: " ,__FUNCTION__, __LINE__);\
                 }\
                 else if(!ptr_src)\
                    {\
                        printf("\n src ptr is NULL @:%s.%d: " ,__FUNCTION__, __LINE__);\
                    }\
                 if(length == 0)\
                    {\
                        printf("\n length is NULL @:%s.%d: " ,__FUNCTION__, __LINE__);\
                    }\
                   memcpy(ptr_dest ,ptr_src,length);\
			}\
			while(0)

#define FREE_MEMORY(ptr) do\
			{\
			    if(!ptr)\
                 {\
                    printf("\n dest ptr is NULL @:%s.%d: " ,__FUNCTION__, __LINE__);\
                 }\
                 else\
                    {\
                         free(ptr);\
                         ptr = NULL;\
                 }\
			}\
			while(0)

				
#ifndef WIN32
#define SECURE_PRINT_FN(buffer,buffersize,fmt_spr, args...) \
    do \
        {\
    snprintf ((PVOID)buffer,buffersize,fmt_spr,##args);\
        }\
        while(0);

#else
#define SECURE_PRINT_FN(buffer,buffersize,fmt_spr, ...) \
    do \
        {\
    sprintf_s ((PVOID)buffer,buffersize,fmt_spr,__VA_ARGS__);\
        }\
        while(0);
        
#endif

#ifndef WIN32
#define SECURE_STRCPY_FN(dest, src,  n) \
    do \
        {\
            strncpy (dest,src,n);\
        }\
        while(0);

#else
#define SECURE_STRCPY_FN(dest, src,  n) \
    do \
        {\
        strcpy_s(dest,n,src);\
        }\
        while(0);
        
#endif

#ifndef WIN32
#define SECURE_SCANF_FN(format, argument,  arg_size) \
    do \
        {\
           scanf(format,argument);\
        }\
        while(0);

#else
#define SECURE_SCANF_FN(format, argument,  arg_size) \
    do \
        {\
        scanf_s(format,argument,arg_size);\
        }\
        while(0);
        
#endif


#pragma pack(pop)
//

#endif
