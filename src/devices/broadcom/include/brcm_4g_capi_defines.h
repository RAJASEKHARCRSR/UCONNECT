#ifndef _BRCM_4G_CAPI_DEFINES
#define _BRCM_4G_CAPI_DEFINES

#include <stdlib.h>
#include <string.h>

#ifdef QTPROJECT
#include <stdint.h>
#endif

#ifndef WIN32
#include <stdint.h>
#include <unistd.h>
#else
#pragma warning(disable : 4127) 
#endif

#ifndef WIN32

#define TRUE 1
#define FALSE 0
#define Sleep(a) usleep(a*1000)

#endif

#define IN
#define OUT
#define IN_OUT


typedef intptr_t MEMORY_ADDRESS_TYPE;

typedef enum {
    eLocal =1,
    eRemote
} E_OPERATING_MODE, *P_E_OPERATING_MODE;

#pragma pack (push,1)
/**
connection parameters - Valid for 
*/
typedef struct ST_CONNECT_PARAMS {
    BRCM_CAPI_CHAR szIpAddress[128];
			 /**<IP address */
    BRCM_CAPI_UINT32 u32Port;
		/**< Port number*/
} ST_CONNECT_PARAMS, *P_ST_CONNECT_PARAMS;
typedef struct ST_OPEN_PARAM {
    E_OPERATING_MODE eOperatingMode;
    ST_CONNECT_PARAMS stConnectParams;
} ST_OPEN_PARAM, *P_ST_OPEN_PARAM;


//Callback buffer to give to application
typedef struct ST_CALLBACK_INFO {
    BRCM_CAPI_UINT32 uicmd_id;
    BRCM_CAPI_INT32 iStatus;
    BRCM_CAPI_UINT32 uinum_tlvs;
    ST_TLV pTLVBuf[1];
} *P_ST_CALLBACK_INFO;

typedef struct ST_4G_API_PKT
{
    BRCM_CAPI_UINT32 uiNumTLVs;
    BRCM_CAPI_UINT32 uiTLVTotalLength;
    ST_TLV astTLVBuffer[1];
}ST_4G_API_PKT,*P_ST_4G_API_PKT;

typedef struct ST_API_SEND_DATA
{
    BRCM_CAPI_UINT uiLength; //for pointers it would be T+L+Address if data(array/contigouous) it is T+L+Value
    P_ST_TLV pstTLVPkt;
}ST_API_SEND_DATA,*P_ST_API_SEND_DATA;

#pragma pack(pop)


//Utility macros
#define ALLOCATE_MEMORY(ptr,data_type,usize,uoldsize) do\
			{\
			    BRCM_CAPI_UINT uiMemSet = (ptr == NULL)? TRUE:FALSE;\
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
                    BRCM_CAPI_PUCHAR puchNewMem = (BRCM_CAPI_PUCHAR )ptr + uoldsize;\
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
    snprintf ((BRCM_CAPI_PVOID)buffer,buffersize,fmt_spr,##args);\
        }\
        while(0);

#else
#define SECURE_PRINT_FN(buffer,buffersize,fmt_spr, ...) \
    do \
        {\
    sprintf_s ((BRCM_CAPI_PVOID)buffer,buffersize,fmt_spr,__VA_ARGS__);\
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

#endif
