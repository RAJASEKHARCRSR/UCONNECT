/*****************************************************************************
**
** uconn_OSAL.h
** Implements OS Abstraction Layer for UConnect Client and Server
**
** Author: Sriram V Iyer
** Date  : 28-DEC-2010
** (c) United Mobile Apps Pvt Ltd (2010 - 2015)

    VER		  DATE		AUTHOR		DESCRIPTION
	0.1		28/12/10     SVI		Creation
	0.2		08/01/11	 SVI		Move Server-Socket related calls
****************************************************************************/


#ifndef UCONN_OSAL_H_
#define UCONN_OSAL_H_

/////////////////////////////////////////////////////////////////////////////
/////////////////////////// INCLUDE FILES ///////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
#include "UTypes.h"
#ifdef QTPROJECT
//#define WINVER 0x0501
//#define _WIN32_WINNT 0x0501
#define CALLBACK __stdcall
typedef void *PVOID,*LPVOID;
typedef unsigned char BYTE;
typedef BYTE BOOLEAN,*PBOOLEAN;
//typedef void(CALLBACK *WAITORTIMERCALLBACK)(PVOID,BOOLEAN);
#endif



#if (defined(__linux__)||defined(__MACH__))

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <signal.h>
#define THREAD_BASE_PRIORITY_MAX 2 // FIXME
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <pthread.h>
#include <signal.h>
#include <time.h>
#include <string.h>
#include <stddef.h>             /* offsetof */
#include <net/if.h>
#include <netinet/in.h>
#include <net/ethernet.h>
#include <errno.h>
#define FILENAME "/etc/resolv.conf"

#define _strnicmp strncasecmp 
#include <stdlib.h>
using namespace std;
#define _strdup strdup
#define fopen_s fopen
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1

#define ifreq_offsetof(x)  offsetof(struct ifreq, x)

#define __stricmp strcasecmp
#define _stricmp strcasecmp
#define INVALID_SOCKET -1
#endif

#define WORKING_BUFFER_SIZE 15000
#define MAX_TRIES 3
#define ROUTER_ADV_SUCCESS	1
#define ROUTER_ADV_FAILURE	0





#ifdef WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#include <Windows.h>
#include <setupapi.h>
#include <devguid.h>
#include <iphlpapi.h>
#include <stdio.h>



#pragma comment(lib, "IPHLPAPI.lib")
#pragma comment(lib, "Setupapi.lib")
#endif /* WIN32 */


#include <assert.h>
#include "UTypes.h"
#include <iostream>
//#include "ULog.h"
/* Macros for the Setup API calls */
#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))

#ifdef QTPROJECT
//
// Macro that works for both IPv4 and IPv6
//
#define SS_PORT(ssp) (((PSOCKADDR_IN)(ssp))->sin_port)
#endif
// Client APIs


#ifdef WIN32
	int snprintf(char *str,size_t size,const char *fmt,...);
#else
	typedef unsigned int U_SOCKET;
#endif
#define ITOA(val,buff,size)    snprintf(buff, size, "%d", val)

/////////////////////////////////////////////////////////////////////////////
/////////////////////////// EXPORTED CALLS //////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/****************** Client Socket Communication APIs ***********************/
U_SINT32 init_client();
U_SOCKET connect_to_server(U_SINT8 *serverIp_p, U_SINT8 *serverPort_p);
U_VOID disconnect_fromServer (U_SOCKET socket);
U_SINT32 send_data(U_SOCKET ConnectSocket, U_SINT8* data_p, U_SINT32 len);
U_SINT32 rcv_data(U_SOCKET ConnectSocket, U_SINT8* recv_data_p, 
							   U_SINT32* len_p, U_SINT32 recvbuflen);

/****************** IP stack APIs ***********************/
U_BOOL isIpV6supported ();
U_VOID addIPaddress (U_SINT8 *interfaceName_p, U_SINT8 *ipAddress_p,
					 U_SINT8 *gwAddress_p, U_BOOL isIpv6,U_SINT8 *dnsAddress_p);
U_VOID setIfStatus (U_SINT8 *interfaceName_p, U_SINT8 *status_p);
U_VOID setDnsAddresses (U_SINT8 *interfaceName_p, U_SINT8 *primaryDNS_p,
					U_SINT8 *secondaryDNS_p, U_BOOL isIpv6);
U_VOID disableNS();
U_VOID enableDHCP (U_SINT8 *interfaceName_p, U_BOOL isIpv6);

/***************** Device Detection APIs ********************/
#ifdef WIN32
char* __fastcall stristrA(const char* pszMain, const char* pszSub);
#else
//char* __attribute__((fastcall)) stristrA(const char* pszMain, const char* pszSub);
#endif /* WIN32 */
bool getComPortFromDevMngsModemSection(string& ComPort);
bool ComPortFinder(string& ComPort, U_SINT8* hwID);
bool ComPortFinder_3GDevices(string& ComPort);
int IsDevicePresent(U_UINT8 dalMode,
					U_UINT32 VidPidCount,
					U_SINT8 VidPidList[3][100],
					U_UINT32* APNId,
					U_SINT8* Adapter_desc,
					U_STRING com_port);
int adapter_info(U_SINT8* Adapter_name,
				 U_SINT8* Adapter_desc,
				 U_SINT8* HW_Addr,
				 U_SINT8* Adapter_index);
void populateDeviceInfo(U_UINT8 dalMode, U_UINT32 VidPidCount, 
						U_SINT8 VidPidList[3][100], U_UINT32 APNId,
						U_SINT8* Adapter_desc, string& com_port,
						U_SINT8* Adapter_name, U_SINT8* HW_Addr,
						U_SINT8* Adapter_index);
bool IsWinXP();
void NAStoIPv6AddrConverter(U_SINT8* NASString, U_SINT8* IPv6Addr);
void IPv4v6AddressSplitter(U_SINT8* NASString, U_SINT8* IPv6Addr, 
						   U_SINT8* IPv4Addr);
void disableRouterDiscovery(U_SINT8 *interfaceName_p);
void enableRouterDiscovery(U_SINT8 *interfaceName_p);
U_VOID deleteIPaddress (U_SINT8 *interfaceName_p, U_SINT8 *ipAddress_p,
						U_BOOL isIpv6);
						
// asper at_commands

U_UINT32 isRSRAComplete(U_CSTR Adapter_desc);
U_BOOL isValidIPv4Address(U_CSTR ipv4_string);



/****************** Client Socket Communication APIs ***********************/

U_SOCKET create_init_server(U_SINT8 *serverIp_p, U_SINT8 *serverPort_p);

U_VOID close_server_socket(U_SOCKET server_socket);

U_SOCKET accept_client(U_SOCKET server_socket);

typedef U_VOID (*U_CALLBACK) (U_PVOID lpParam, U_BOOL val);
//typedef U_VOID (*U_CALLBACK) (U_PVOID* lpParam);

#if (defined(__linux__)||defined(__MACH__))
typedef pthread_mutex_t 	U_MUTEX; 
typedef pthread_t			U_THREAD;
typedef void* 			U_THREAD_RETURN_TYPE;
typedef void* 				U_THREAD_ARG;
typedef pthread_mutex_t* 	U_PCRITICAL_SECTION; 
typedef sem_t           U_SEMAPHORE_ID;
//typedef  timer_t U_TIMERID;


 typedef struct time_node
{
	int original_time;
	int delta_time;
	int mode ;
	U_CALLBACK cb;
	struct time_node* next;
	
	struct itimerval itimer;
	
	time_node(){
		original_time = 0;
		delta_time = 0;
		next = NULL;
		cb = NULL;
	}
	~time_node(){
		original_time = 0;
		delta_time = 0;
		next = NULL;
		cb = NULL;
	}

	
}time_node;

typedef time_node* U_TIMERID;
void insertNode(U_TIMERID);
void deleteNode(U_TIMERID);
class UConnServer;
typedef struct Timer_struct
{
	time_node* mTimer;
	UConnServer* mConnServer;
}Timer_struct_s;
#endif /* Linux */


#ifdef WIN32
typedef CRITICAL_SECTION	U_MUTEX;
typedef HANDLE				U_THREAD;
typedef DWORD				U_THREAD_RETURN_TYPE;
typedef LPVOID				U_THREAD_ARG;

typedef LPCRITICAL_SECTION  U_PCRITICAL_SECTION;
typedef HANDLE              U_SEMAPHORE_ID;

class UConnServer;

typedef struct timer_node
{
public:
	HANDLE    timer_ID;
	U_UINT32  mode;
}timer_node;

typedef timer_node*              U_TIMERID;

typedef struct Timer_struct
{
	timer_node* mTimer;
	UConnServer* mConnServer;
}Timer_struct_s;

HANDLE spawn_thread (LPSECURITY_ATTRIBUTES attr, 
					 DWORD stacksize,
					 LPTHREAD_START_ROUTINE threadFunc,
					 LPVOID params,
					 DWORD creationFlags,
					 LPDWORD threadId);
DWORD waitForThreads_exit (DWORD ncount,
						   HANDLE *handles,
						   BOOL fwaitForAll, 
						   DWORD dwmilliseconds);

#endif /* WIN32 */
U_VOID initialize_lock (U_PCRITICAL_SECTION lpCriticalSection);
U_VOID delete_lock (U_PCRITICAL_SECTION lpCriticalSection);
U_VOID take_lock (U_PCRITICAL_SECTION lpCriticalSection);
U_VOID release_lock (U_PCRITICAL_SECTION lpCriticalSection);


void* operator new(size_t size) ;

typedef     U_UINT32      OSA_RESULT;

#ifdef WIN32
typedef     U_PVOID       Q_ID;
#elif (defined(__linux__)||defined(__MACH__))
  typedef   int  Q_ID;
#endif


#define MAX_TASK_Q_DEPTH	60

#define MAX_MSG_LENGTH    4

#ifdef WIN32 
	#define B_DEFAULT_TASK_PRIORITY   THREAD_PRIORITY_NORMAL       // 75
#endif

#define B_DEFAULT_STACK_SIZE   600000

#define B_DEFAULT_QUEUE_DEPTH  20



#define B_NO_TIMEOUT INFINITE


#ifdef WIN32
#define OsaSleep(x) Sleep(x)
#elif (defined(__linux__) || defined(__MACH__))
#define OsaSleep(x) sleep(x/1000)
#endif



#define OSA_ASSERT(cond) {\
	if (!(cond)) \
	{\
	printf("Assert failed at file %s:%d \n", __FILE__, __LINE__);\
		while(1)	OsaSleep(1);\
	}\
}

int isnumeric(char *str);

class USemaphore{
public:
	USemaphore();
	~USemaphore();
	U_VOID    lock(int locktime = 0);
	U_VOID    release();
private:
	U_SEMAPHORE_ID  mSemId;

};





class UEventQueue{
public:
    UEventQueue( U_UINT32 u16MaxQDepth=64, U_UINT32 u32Flags = 0 );
    OSA_RESULT               MsgQPost(U_UINT8 *pMsg  );
    OSA_RESULT               MsgQGet(U_UINT8 **ppMsg);
    U_UINT32                   MsgQGetDepth();
    void                     MsgQFlush(U_BOOL bMode);
    ~UEventQueue();

private:
    Q_ID                     pvReadPipe,pvWritePipe;
};

/// \class UProcess

/// < Defines the Task and implements OSAL for Vxworks and ACE>
class UProcess{
public:
    /** \brief Initializes the task*/
    void                     Init(U_UINT8 *pu8TaskName=0,U_UINT32 u32Priority=THREAD_BASE_PRIORITY_MAX,
                            U_UINT32 u32StackSize=100,U_UINT32 u16MaxQDepth=128, U_PVOID pvContext=NULL);
	 /** \brief Resets the task*/
	virtual void Reset() { }
    /** \brief Post a message to the task*/
    OSA_RESULT       MsgPost(U_UINT8 *pMsg);
    /** \brief Task Handler for the task*/
	virtual U_BOOL			 EventHandler() = 0;
    /** \brief Message queue Id for the Task*/
    UEventQueue*           GetQ()  
                             {return (messageQueue);}
    ~UProcess();
	UProcess();
protected:
    /** \brief Stack for the task*/
    U_UINT8*                 pu8Stack;
    /** \brief Message queue for the task*/
    UEventQueue*            messageQueue;

	U_THREAD                  taskId;


};
////////////////////////////////////////////////////////////////////////////////
// Task functions:
////////////////////////////////////////////////////////////////////////////////

typedef void* (*TaskProc)(void*);
OSA_RESULT OsaTaskCreate(U_UINT8 *pu8TaskName=0,
                         U_UINT32 u32Priority=0, U_CSTR pStack=0,
                         U_UINT32 u32StackSize=0, TaskProc taskHandler=0,
                         U_PVOID pvArg0=0);

//U_TIMERID USetTimer(U_UINT32 duration, U_UINT32 mode);
U_TIMERID USetTimer(U_UINT32 duration, U_UINT32 mode,U_CALLBACK cb);
U_TIMERID USetTimer_NM(U_UINT32 duration, U_UINT32 mode, 
					   UConnServer* pConnServer, U_CALLBACK cb);

U_VOID UKillTimer(U_TIMERID tId);

U_VOID UKillAllTimers( );

#ifdef WIN32
U_TIMERID USetTimer(U_UINT32 duration, U_UINT32 mode,U_CALLBACK cb);
#elif (defined(__linux__)||defined(__MACH__))
U_TIMERID USetTimer(U_UINT32 duration, U_UINT32 mode,U_CALLBACK cb);
#endif /*WIN32*/

class USrPortCommUsage
{
public:

	USrPortCommUsage();
	~USrPortCommUsage();


    bool UInitialiseSerialPort(string szPortName, 
		U_UINT32 dwBaudRate, U_UINT8 byParity, U_UINT8 byStopBits, 
		U_UINT8 byByteSize, U_UINT8 dalMode);
	

	U_BOOL write(const char* data,U_DWORD dwSize,U_DWORD &status);
	U_UINT32 read(char* buffer,int maxBytes,U_DWORD &status);

	//void openDeviceWithID(P_AT_CALLBACK_WITH_ID callback_fn);
#ifdef WIN32
    void getcommSerialErrorReport(COMSTAT &comStat,U_DWORD &status);
    void waitingForBytesFromSerialPort();
    U_UINT32 getNumOfBytesToRead(U_DWORD &status);
	HANDLE m_Port;
#elif (defined(__linux__) ||defined(__MACH__))
	/*File descriptor for the serial port*/
	U_INT serialPort_fd;
#endif
};

#if (defined(__linux__)|| defined(__MACH__))
    void callback_timeout(int n);
#endif /*__linux__*/


//returns executable file location
string getExecDirectory(string exe);

#endif /* UCONN_OSAL_H_  */

