/*****************************************************************************
**
** uconn_OSAL.cpp
** Implements OS Abstraction Layer for UConnect Client and Server
**
** Author: Sriram V Iyer
** Date  : 28-DEC-2010
** (c) United Mobile Apps Pvt Ltd (2010 - 2015)

    VER		  DATE		AUTHOR		DESCRIPTION
	0.1		28/12/10     SVI		Creation
	0.2		08/01/11	 SVI		Move Server-Socket related calls
****************************************************************************/
#include <cstdio>
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <algorithm>

#include "uconn_OSAL.h"
#include "uconn_Event.h"
#include "ULog.h"

extern CM_Info_s gCMInfo;

extern UConnServer *gConnServer;
using namespace std;




#ifdef WIN32
#pragma comment(lib, "Ws2_32.lib")
HANDLE gTimerQueue = NULL;
#endif

#ifdef QTPROJECT
    //#define _countof(arr) (sizeof(arr) / sizeof((arr)[0]))
#endif

#if (defined(__linux__)||defined(__MACH__))
    #define _countof(arr) (sizeof(arr) / sizeof((arr)[0]))
    #define _stricmp strcasecmp
    struct stat statbuf;

#endif

#define CONCAT(dest,src) 	strncat(dest, src, _countof(dest) - strlen(dest)-1)

int isnumeric(char *str)
{
  while(*str)
  {
    if(!isdigit(*str))
      return 0;
    str++;
  }
  return 1;
}

U_MUTEX mallocLock ;

U_BOOL  gbMalloLockInitialized= false;

void getMallocLock(){
if(gbMalloLockInitialized == false){
		initialize_lock(&mallocLock);
		gbMalloLockInitialized = true;
	}
	
	take_lock(&mallocLock);

}

void releaseMallocLock(){
	if(gbMalloLockInitialized){
		release_lock(&mallocLock);
	}

}



U_MUTEX msgQLock ;

U_BOOL  gbmsgQLockInitialized= false;

void getmsgQLock(){
if(gbmsgQLockInitialized == false){
		initialize_lock(&msgQLock);
		gbmsgQLockInitialized = true;
	}
	
	take_lock(&msgQLock);

}

void releaseMsgQLock(){
	if(gbmsgQLockInitialized){
		release_lock(&msgQLock);
	}

}


void* operator new(size_t size) 
{
	getMallocLock();
	void *returnPtr = malloc(size);
	releaseMallocLock();
	return returnPtr;
}
/******************************************************************************
*	Initialise the Client - Since the socket communication is always client 
*	server, we maintain that structure here as well
*	The function currently habdles WIN32 sockets
*
*	Returns
*	0 on Success
*	-1 on Failure
******************************************************************************/
U_SINT32 init_client()
{
#ifdef WIN32
	U_SINT32 iResult;
	WSADATA wsaData;

	/* Initialize Winsock */
	iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (iResult != 0) 
	{
		printf("WSAStartup failed: %d\n", iResult);
		return -1;
	}
	return 0;
#elif (defined(__linux__)|| defined(__MACH__))
    /* Linux does not require startup */
	return 0;
#endif 
}

/******************************************************************************
*	Called by the client, and connects to the server
*
*	Parameters
*	SeverIp_p - Pointer to the server IP string
*	serverPort_p - Pointer to the server Port string
*
*	Returns 
*	Socket descriptor
******************************************************************************/
U_SOCKET connect_to_server(U_SINT8 *serverIp_p, U_SINT8 *serverPort_p)
{
#ifdef WIN32
	U_SINT32 iResult;

	struct addrinfo *result = NULL,
					*ptr = NULL,
					hints;

	U_SOCKET ConnectSocket = INVALID_SOCKET;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;


	/* Resolve the server address and port */
	iResult = (U_SINT32)getaddrinfo((const U_SINT8*)serverIp_p, serverPort_p, &hints, &result);
	if (iResult != 0) 
	{
		printf("getaddrinfo failed: %d\n", iResult);
		return INVALID_SOCKET;
	}

	/* Attempt to connect to the first address returned by
	 the call to getaddrinfo */
	ptr = result;

	/* Create a SOCKET for connecting to server */
	ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, 
		ptr->ai_protocol);


	if (ConnectSocket == INVALID_SOCKET) 
	{
		printf("Error at socket(): %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		return INVALID_SOCKET;
	}

	/* Connect to server. */
	iResult = connect( ConnectSocket, ptr->ai_addr, (U_SINT8)ptr->ai_addrlen);
	if (iResult == SOCKET_ERROR) 
	{
		closesocket(ConnectSocket);
		ConnectSocket = INVALID_SOCKET;
	}

	/* Should really try the next address returned by getaddrinfo
	 if the connect call failed
	 But for this simple example we just free the resources
	 returned by getaddrinfo and print an error message */

	freeaddrinfo(result);

	if (ConnectSocket == INVALID_SOCKET) 
	{
		printf("Unable to connect to server!\n");
		return INVALID_SOCKET;
	}

	return ConnectSocket;

#elif (defined(__linux__)|| defined(__MACH__))
    U_SINT32 iResult;

	struct addrinfo *result = NULL,
					*ptr = NULL,
					hints;

	U_SOCKET ConnectSocket = INVALID_SOCKET;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;


	/* Resolve the server address and port */
	iResult = (U_SINT32)getaddrinfo((const U_SINT8*)serverIp_p, serverPort_p, &hints, &result);
	if (iResult != 0) 
	{
		printf("getaddrinfo failed: %d\n", iResult);
		return INVALID_SOCKET;
	}

	/* Attempt to connect to the first address returned by
	 the call to getaddrinfo */
	ptr = result;

	/* Create a SOCKET for connecting to server */
	ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, 
		ptr->ai_protocol);


	if (ConnectSocket == INVALID_SOCKET) 
	{
		printf("Client Socket creation failed\n");
		freeaddrinfo(result);
		return INVALID_SOCKET;
	}

	/* Connect to server. */
	iResult = connect( ConnectSocket, ptr->ai_addr, (U_SINT8)ptr->ai_addrlen);
	if (iResult == SOCKET_ERROR) 
	{
		close(ConnectSocket);
		ConnectSocket = INVALID_SOCKET;
	}

	/* Should really try the next address returned by getaddrinfo

	 if the connect call failed
	 But for this simple example we just free the resources
	 returned by getaddrinfo and print an error message */

	freeaddrinfo(result);

	if (ConnectSocket == INVALID_SOCKET) 
	{
		printf("Unable to connect to server!\n");
		return INVALID_SOCKET;
	}

	return ConnectSocket;
#endif /* WIN32 */
}


//#define DEFAULT_BUFLEN 8192
#define DEFAULT_BUFLEN 4096

/******************************************************************************
*	Called by the client / server, and sends data over socket
*
*	Parameters
*	ConnectSocket - Socket descriptor
*	data_p - Pointer to the data
*	len	-	Length of the data
*
*	Returns 
*	0 on Success
*	-1 on Failure
******************************************************************************/
U_SINT32 send_data(U_SOCKET ConnectSocket, U_SINT8* data_p, U_SINT32 len)
{
#ifdef WIN32
	U_SINT8 *sendbuf_p = data_p; 
	U_SINT32 iResult;

	if(ConnectSocket == INVALID_SOCKET)
		return -1;
	/* Send an initial buffer */
	if ((iResult = send(ConnectSocket, sendbuf_p, len, 0)) == SOCKET_ERROR) 
	{
		//printf("send failed: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		//WSACleanup();
		return -1;
	}
	return 0;
#elif (defined(__linux__)|| defined(__MACH__))
    U_SINT8 *sendbuf_p = data_p; 
	U_SINT32 iResult;

	if(ConnectSocket == INVALID_SOCKET)
		return -1;
	/* Send an initial buffer */
	if ((iResult = send(ConnectSocket, sendbuf_p, len, 0)) == SOCKET_ERROR) 
	{
		printf("Send failed\n");
		close(ConnectSocket);
		return -1;
	}
	return 0;
#endif
}

/******************************************************************************
*	Called by the client / server, and reads data over socket
*
*	Parameters
*	ConnectSocket - Socket descriptor
*	recv_data_p - Pointer to the data placeholder
*	len_p	-	Pointer to Length of the data read
*	recvbuflen	- Lenghth supposed to be read
*
*	Returns 
*	0 on Success
*	-1 on Failure
******************************************************************************/
U_SINT32 rcv_data(U_SOCKET ConnectSocket, U_SINT8* recv_data_p, 
							   U_SINT32* len_p, U_SINT32 recvbuflen)
{
#if defined(WIN32) || defined(__linux__) ||defined(__MACH__)
	U_SINT32 iResult;
	assert(recv_data_p != NULL);

	*len_p = recv(ConnectSocket, recv_data_p, recvbuflen, 0);
	if (*len_p > 0)
	{
		iResult = 0;
	}
	else if (*len_p == 0)
	{
		printf("Connection closed\n");
		iResult = -1;
	}
	else
	{
		iResult = -1;
	}
	return iResult;
#endif 
}

/******************************************************************************
*	Called by the client, and disconnects from the server
*
*	Parameters
*	socket - Socket descriptor
******************************************************************************/
U_VOID disconnect_fromServer (U_SOCKET socket)
{
#ifdef WIN32
		shutdown(socket, SD_SEND);
		closesocket(socket);
#elif (defined(__linux__)|| defined(__MACH__))
    
		close(socket);
#endif
}

/******************************************************************************
*	Called by the server, and initialises socket communication for the process
******************************************************************************/
U_SOCKET create_init_server(U_SINT8 *serverIp_p, U_SINT8 *serverPort_p)
{
#ifdef WIN32
	U_SINT32 iResult;
	WSADATA wsaData;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (iResult != 0) {
		//printf("WSAStartup failed: %d\n", iResult);
		return INVALID_SOCKET;
	}

	// Create Server Socket

    struct addrinfo *result = NULL, hints;
	ZeroMemory(&hints, sizeof (hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the local address and port to be used by the server
	iResult = getaddrinfo(serverIp_p, serverPort_p, &hints, &result);
	
	if (iResult != 0) {
		//printf("getaddrinfo failed: %d\n", iResult);
		WSACleanup();
		return INVALID_SOCKET;
	}

	// Create a socket to listen to clients
	U_SOCKET ListenSocket = INVALID_SOCKET;
	// Create a SOCKET for the server to listen for client connections
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

	// Check if the socket was ok	
	if (ListenSocket == INVALID_SOCKET) {
		//printf("Error at socket(): %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return INVALID_SOCKET;
	}

	// Setup the TCP listening socket
    iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        //printf("bind failed with error: %d\n", WSAGetLastError());

        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return INVALID_SOCKET;
    }

	// Listen for connections
	if ( listen( ListenSocket, SOMAXCONN ) == SOCKET_ERROR ) {
		//printf( "Listen failed with error: %ld\n", WSAGetLastError() );
		closesocket(ListenSocket);
		WSACleanup();
		return INVALID_SOCKET;
	}

	return ListenSocket;

#elif (defined(__linux__)|| defined(__MACH__))
    U_SINT32 iResult;

	// Create Server Socket

	struct addrinfo *result = NULL, *ptr = NULL, hints;
	memset(&hints, 0, sizeof (hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the local address and port to be used by the server
	iResult = getaddrinfo(serverIp_p, serverPort_p, &hints, &result);
	
	if (iResult != 0) {
		printf("getaddrinfo failed: %d\n", iResult);
		return INVALID_SOCKET;
	}
        
        // Create a socket to listen to clients
	U_SOCKET ListenSocket = INVALID_SOCKET;
	// Create a SOCKET for the server to listen for client connections
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

	// Check if the socket was ok	
	if (ListenSocket == INVALID_SOCKET) {
		printf("Error creating socket()\n");
		freeaddrinfo(result);
	
		close(ListenSocket);
		return INVALID_SOCKET;
	}
        // Setup the TCP listening socket
        iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
           printf("bind failed with error.\n");
           freeaddrinfo(result);
           close(ListenSocket);
           return INVALID_SOCKET;
       }
       // Listen for connections
       if ( listen( ListenSocket, SOMAXCONN ) == SOCKET_ERROR ) {
		printf( "Listen failed with error \n");
		close(ListenSocket);
		return INVALID_SOCKET;
	}
       return ListenSocket;
#endif /* LINUX */
}

/*****************************************************************
Closes the server socket.
*******************************************************************/

U_VOID close_server_socket(U_SOCKET server_socket)
{
#ifdef WIN32
	if(server_socket != INVALID_SOCKET)
	{
		closesocket(server_socket);
	}
#endif

}

/******************************************************************************
*	Called by the sever, and accepts a new connection from the client
*
*	Parameters
*	server_socket - Socket descriptor
*
*	Returns
*	Socket descriptor for the connection
******************************************************************************/
U_SOCKET accept_client(U_SOCKET server_socket)
{
#ifdef WIN32
	// Accept client connections
	U_SOCKET ClientSocket = INVALID_SOCKET;

	// Accept a client socket
	ClientSocket = accept(server_socket, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET) {
		printf("accept failed: %d\n", WSAGetLastError());
		closesocket(server_socket);
		WSACleanup();
		return 1;
	}

	return ClientSocket;
#elif (defined(__linux__)|| defined(__MACH__))
    // Accept client connections
	U_SOCKET ClientSocket = INVALID_SOCKET;

	// Accept a client socket
	ClientSocket = accept(server_socket, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET) {
		printf("accept failed\n");
		close(server_socket);
		return 1;
	}
	return ClientSocket;
#endif /* Linux */
}

#ifdef WIN32
/******************************************************************************
*	Spawn out a thread. 
*
*	Parameters
*	attr - Security Parameters
*	stacksize - Stacksize of the thread
*	threadFunc - Thread function pointer
*	params - parameters to the function
*	creationFlags - permissions
*	threadId - Identifier
*
*	Returns
*	Handle to the new thread
******************************************************************************/
HANDLE spawn_thread (LPSECURITY_ATTRIBUTES attr, 
					 DWORD stacksize,
					 LPTHREAD_START_ROUTINE threadFunc,
					 LPVOID params,
					 DWORD creationFlags,
					 LPDWORD threadId)
{
    HANDLE taskid =  CreateThread(attr, stacksize, threadFunc, params, creationFlags, threadId);
    SetThreadPriority(taskid, THREAD_PRIORITY_LOWEST);
    return taskid;
}

/******************************************************************************
*	Wait for all the threads to exit
*
*	Parameters
*	ncount - nmber of threads
*	handles - pointer to the array of handles
*	waitForAll - TRUE if wait for for all threads to exit
*	dwmilliseconds - timeout for wait
*
*	Returns
*	DWORD
******************************************************************************/
DWORD waitForThreads_exit (DWORD ncount,
						   HANDLE *handles,
						   BOOL fwaitForAll, 
						   DWORD dwmilliseconds)
{
	return WaitForMultipleObjects(ncount, handles, fwaitForAll, dwmilliseconds);
}
#endif

/******************************************************************************
*	Initialize a mutex
*
*	Parameters
*	lpCriticalSection - mutex
******************************************************************************/
U_VOID initialize_lock (U_PCRITICAL_SECTION lpCriticalSection)
{
#ifdef WIN32
	InitializeCriticalSection(lpCriticalSection);
#elif (defined(__linux__)|| defined(__MACH__))
 	//pthread_mutexattr_t mutexattr;   // Mutex attribute variable
	// Set the mutex as a recursive mutex
	//pthread_mutexattr_settype(&mutexattr, PTHREAD_MUTEX_RECURSIVE_NP);

	// create the mutex with the attributes set
	//int result = pthread_mutex_init(lpCriticalSection, &mutexattr);
	//if(result != 0)
	//{
    //       printf("Mutex Init failed\n");
	//}
	//After initializing the mutex, the thread attribute can be destroyed
	//pthread_mutexattr_destroy(&mutexattr);
#endif
}

/******************************************************************************
*	Delete a mutex
*
*	Parameters
*	lpCriticalSection - mutex
******************************************************************************/
U_VOID delete_lock (U_PCRITICAL_SECTION lpCriticalSection)
{
#ifdef WIN32
	DeleteCriticalSection(lpCriticalSection);
#elif (defined(__linux__)|| defined(__MACH__))
    //	int result = pthread_mutex_destroy(lpCriticalSection);
 //       if(result != 0)
//	{
 //          printf("Mutex destroy failed\n");
//	}
#endif
}

/******************************************************************************
*	Lock before critical section
*
*	Parameters
*	lpCriticalSection - mutex
******************************************************************************/
U_VOID take_lock (U_PCRITICAL_SECTION lpCriticalSection)
{
#ifdef WIN32
	EnterCriticalSection(lpCriticalSection);
#elif (defined(__linux__)|| defined(__MACH__))
 	pthread_mutex_lock(lpCriticalSection);
#endif
}

/******************************************************************************
*	Unlock after critical section
*
*	Parameters
*	lpCriticalSection - mutex
******************************************************************************/
U_VOID release_lock (U_PCRITICAL_SECTION lpCriticalSection)
{
#ifdef WIN32
	LeaveCriticalSection(lpCriticalSection);
#elif (defined(__linux__)|| defined(__MACH__))
 	pthread_mutex_unlock(lpCriticalSection);
#endif
}


/*****************************************************************************
*	Destruction of semaphore, locking of semaphore
*	release of semaphore
*	
*	Parameter
*	mSemID - semaphore
******************************************************************************/
USemaphore::USemaphore()
{
	// Create a semaphore with initial and max.
#ifdef WIN32
	mSemId = CreateSemaphore( 0,0,1,0);  
#endif
}

USemaphore::~USemaphore()
{
	// Create a semaphore with initial and max.
#ifdef WIN32
	CloseHandle(mSemId);  
#elif (defined(__linux__)|| defined(__MACH__))
 	sem_close(&mSemId);
#endif
}

 U_VOID USemaphore::lock(int locktime)
 {
#ifdef WIN32

    if(locktime <= 0){

           WaitForSingleObject(mSemId,INFINITE);

    }

    else

    {

           WaitForSingleObject(mSemId,locktime);

    

         }


#elif (defined(__linux__)|| defined(__MACH__))
     /*struct timespec ts;
	int t = 10000;
	if (clock_gettime(CLOCK_REALTIME, &ts) == -1)
    	handle_error("clock_gettime");

   	ts.tv_sec += t;
	sem_timedwait(&mSemId,&ts);*/
	sem_wait(&mSemId);

#endif
 }

 U_VOID USemaphore::release()
 {
#ifdef WIN32
	ReleaseSemaphore(mSemId,1, NULL);
#elif (defined(__linux__)|| defined(__MACH__))
     sem_post(&mSemId);
#endif
 }







/******************************************************************************
*	UEventQueue::UEventQueue
*
*	Parameters
*	u16MaxQDepth - Max Queue depth
*	u32Flags	 - flags to be set
******************************************************************************/

UEventQueue::UEventQueue(U_UINT32 u16MaxQDepth,
                                   U_UINT32 u32Flags)
{
#ifdef WIN32
    (void)u16MaxQDepth;
    (void) u32Flags;
	CreatePipe(&pvReadPipe,&pvWritePipe,NULL,0);
#elif (defined(__linux__)|| defined(__MACH__))
    int pipefd[2];
        if(pipe(pipefd) == -1) {
			printf("Pipe creation failed\n");
			pvReadPipe = 0;
			pvWritePipe = 0;
        }
		else {
			pvReadPipe = pipefd[0];
			pvWritePipe = pipefd[1];
		}
#endif
}

/******************************************************************************
*	UEventQueue::MsgQPost
*
*	Parameters
*	pMsg - Message to be posted
******************************************************************************/

OSA_RESULT  UEventQueue::MsgQPost(U_UINT8 *pMsg  )
{
	U_UINT32 u32Written = 0;
#ifdef WIN32
	getmsgQLock();
	if(!WriteFile(pvWritePipe, &pMsg,sizeof(U_UINT8*),(LPDWORD)&u32Written,NULL))
	{
		u32Written =  -1;
	}
	if(u32Written != sizeof(U_UINT8*))
	{
		u32Written = -1;
	}
	releaseMsgQLock();
	return u32Written;
#elif (defined(__linux__)|| defined(__MACH__))
    getmsgQLock();
	u32Written = write(pvWritePipe, &pMsg, sizeof(U_UINT8*));
        if(u32Written == -1)
        {
		u32Written = -1;
        }
        if(u32Written != sizeof(U_UINT8*))
	{
		u32Written = -1;
	}
        releaseMsgQLock();
	return u32Written;
#endif
}

/******************************************************************************
*	UEventQueue::MsgQGet
*
*	Parameters
*	ppMsg - Message received from the Poster
******************************************************************************/

OSA_RESULT  UEventQueue::MsgQGet(U_UINT8 **ppMsg)
{
	U_UINT32 u32NumRead = 0;
#ifdef WIN32
	if(!ReadFile(pvReadPipe,ppMsg,sizeof(U_UINT8*),(LPDWORD)&u32NumRead,NULL))
		return -1;
	if(sizeof(U_UINT8*) != u32NumRead)
		return -1;
	return u32NumRead;
#elif (defined(__linux__)|| defined(__MACH__))
 	u32NumRead = read(pvReadPipe, ppMsg, sizeof(U_UINT8*));
	if(u32NumRead == -1)
		return -1;
        if(sizeof(U_UINT8*) != u32NumRead)
		return -1;
	return u32NumRead;
#endif
}

/******************************************************************************
*	UEventQueue::MsgQGetDepth
*	- Obtain the Message Queue depth
******************************************************************************/

U_UINT32  UEventQueue::MsgQGetDepth()
{
	U_UINT32 u32NumBytesAvail = 0;
#ifdef WIN32
	PeekNamedPipe(pvReadPipe,NULL,0,NULL,(LPDWORD)&u32NumBytesAvail,NULL);
	return u32NumBytesAvail;
#elif (defined(__linux__)|| defined(__MACH__))
    int status = fstat(pvReadPipe, &statbuf);
	if(status == -1) {
		printf("Error in getting status\n");
		return 0;
	}
     	u32NumBytesAvail = statbuf.st_size;
	return u32NumBytesAvail>>2;
#endif
}

/******************************************************************************
*	UEventQueue::MsgQFlush
*	- Flush a Message Queue
******************************************************************************/

void UEventQueue::MsgQFlush(U_BOOL )
{
	U_SINT32 s32Depth = MsgQGetDepth();
	U_PSINT32 u32TempRead;
	while(s32Depth > 0)
	{
		s32Depth-=MsgQGet((U_UINT8**)&u32TempRead);
	}
}

/******************************************************************************
*	UEventQueue::~UEventQueue
*	- Message Queue destructor
******************************************************************************/

UEventQueue::~UEventQueue ()
{
#ifdef WIN32
	CloseHandle(pvReadPipe);
	CloseHandle(pvWritePipe);
#elif (defined(__linux__)|| defined(__MACH__))
 	close(pvReadPipe);
        close(pvWritePipe);		
#endif
}

/******************************************************************************
*	EventHandlerEntry
*	- Event handler entry
*
*	Parameters
*	pArgs - Arguments
******************************************************************************/

void* EventHandlerEntry(void *pArgs)
{
    UProcess *pObj = reinterpret_cast<UProcess*>(pArgs);
    pObj->EventHandler();
    return NULL;
}

/******************************************************************************
*	UProcess::Init
*	- Process initialization function - Thread initialized here
*
*	Parameters
*	pu8TaskName		- Task Name
*	u32Priority		- Task priority
*	u32StackSize	- Stack size for the task
*	u16MaxQDepth	- Queue Depth
*	pvContext		- Context pointer 
******************************************************************************/

void UProcess::Init(U_UINT8 *pu8TaskName ,U_UINT32 u32Priority,
                     U_UINT32 u32StackSize,U_UINT32 u16MaxQDepth, U_PVOID pvContext)
{ 

    (void)pvContext;
    (void)u32StackSize;
    (void)pu8TaskName;
	
    // create a message queue for the task
    messageQueue = new UEventQueue(u16MaxQDepth);

	
	U_UINT32 u32ThreadId = 0;
#ifdef WIN32
	taskId = CreateThread(NULL, 0,
			(LPTHREAD_START_ROUTINE)EventHandlerEntry,this,0,(LPDWORD)&u32ThreadId);
	//int pri = GetThreadPriority(taskId);  
	SetThreadPriority(taskId, u32Priority); 
#elif (defined(__linux__)|| defined(__MACH__))
 	int handle = pthread_create(&taskId, 
                                     NULL,
	         		     EventHandlerEntry,
                                     (void*)this);
         if(handle != 0) 
         {
            printf("Thread creation failed with error code : %d\n", handle);
         }
#endif
}

/******************************************************************************
*	UProcess::MsgPost
*	- Process Message post function
*
*	Parameters
*	pMsg		- Message to be posted
******************************************************************************/

OSA_RESULT  UProcess::MsgPost(U_UINT8 *pMsg)
{
    return(messageQueue->MsgQPost(pMsg));
}



UProcess::UProcess(){
	pu8Stack = NULL;
	messageQueue = NULL;
	taskId = 0;
}
	

/******************************************************************************
*	UProcess::~UProcess
*	- Process Destructor
******************************************************************************/

UProcess::~UProcess()
{

		// Free the message queue
		if(messageQueue != NULL){
			messageQueue->MsgQFlush(0);
			delete messageQueue;
		}
		if(taskId != 0)
		{
#ifdef WIN32
			CloseHandle(taskId);
			taskId = 0;
#endif

		}
	}



/******************************************************************************
*	USetTimer
*	- Timer setup function
*
*	Parameters
*	duration		- Period of the timer
*	mode			- 0 -> One time, 1 -> Periodic
******************************************************************************/
#if defined(__linux__)||defined(__MACH__)
U_TIMERID    timerHead = NULL;
#endif

/*
U_TIMERID USetTimer(U_UINT32 duration, U_UINT32 mode)
{
#ifdef WIN32

	//U_TIMERID hTimer;
	timer_node* pTimer = new timer_node();

	if(gTimerQueue == NULL){
    // Create the timer queue.
		gTimerQueue = CreateTimerQueue();
 	}
	if (NULL == gTimerQueue)
    {
        printf("CreateTimerQueue failed (%d)\n", GetLastError());
		return NULL;
    }
	if (!CreateTimerQueueTimer( &pTimer->timer_ID, gTimerQueue, 
		(WAITORTIMERCALLBACK)TimerCallBack, &pTimer->timer_ID , duration, 
			(duration * mode), 0))
    {
        printf("CreateTimerQueueTimer failed (%d)\n", GetLastError());
        return NULL;
    }

	return pTimer->timer_ID;

#endif
}*/


U_TIMERID USetTimer(U_UINT32 duration, U_UINT32 mode, U_CALLBACK cb)
{
  #ifdef WIN32
    //U_TIMERID hTimer;
    timer_node* pTimer = new timer_node();

	pTimer->mode = mode;


    if(gTimerQueue == NULL){
    // Create the timer queue.
        gTimerQueue = CreateTimerQueue();
    }
    if (NULL == gTimerQueue)
    {
        printf("CreateTimerQueue failed (%d)\n", GetLastError());
        return NULL;
    }

    if (!CreateTimerQueueTimer( &pTimer->timer_ID, gTimerQueue,
        (WAITORTIMERCALLBACK)cb, pTimer , duration,
            (duration * mode), 0))
    {
        printf("CreateTimerQueueTimer failed (%d)\n", GetLastError());
        return NULL;
    }

    return pTimer;


#elif (defined(__linux__)|| defined(__MACH__))
    sigset_t new_set;
	
  
    if( (sigemptyset(&new_set) == -1) ||  ( sigaddset(&new_set, SIGALRM )== -1 ) )
    {
	  //cout << "Error in signal mask" << endl;
	  return NULL;
    }
	duration = duration/1000;

	U_TIMERID tId= new time_node();
	tId ->original_time =  duration;
	tId ->delta_time =  duration;
	tId ->cb = cb;
	tId->mode = mode;
	
	if(sigprocmask( SIG_BLOCK, &new_set, NULL) == -1)
	{
	  //cout<<"Error blocking the Signal"<<endl;
	  return NULL;
	}
	tId->mode = mode;
	insertNode(tId);	
	timerHead->itimer.it_value.tv_sec = timerHead->delta_time;
	timerHead->itimer.it_value.tv_usec = 0;
	timerHead->itimer.it_interval.tv_sec = 0;
	timerHead->itimer.it_interval.tv_usec = 0;
	setitimer(ITIMER_REAL, &timerHead->itimer, 0);
	signal(SIGALRM, callback_timeout);
	if(sigprocmask( SIG_UNBLOCK, &new_set, NULL)==-1)
	{
	  //cout<<"Error in Unblocking the signal "<<endl;
	  return NULL;
	}
	return tId;


#endif
}

U_TIMERID USetTimer_NM(U_UINT32 duration, U_UINT32 mode, 
					   UConnServer* pConnServer, U_CALLBACK cb)
{
  #ifdef WIN32
    //U_TIMERID hTimer;
    timer_node* pTimer = new timer_node();

	pTimer->mode = mode;

	Timer_struct_s* timer_block = new Timer_struct_s();
	timer_block->mConnServer = pConnServer;
	timer_block->mTimer = pTimer;


    if(gTimerQueue == NULL){
    // Create the timer queue.
        gTimerQueue = CreateTimerQueue();
    }
    if (NULL == gTimerQueue)
    {
        printf("CreateTimerQueue failed (%d)\n", GetLastError());
        return NULL;
    }

    if (!CreateTimerQueueTimer( &pTimer->timer_ID, gTimerQueue,
		(WAITORTIMERCALLBACK)cb, timer_block , duration,
            (duration * mode), 0))
    {
        printf("CreateTimerQueueTimer failed (%d)\n", GetLastError());
        return NULL;
    }

    return pTimer;


#elif (defined(__linux__)|| defined(__MACH__))
    sigset_t new_set;
	
  
    if( (sigemptyset(&new_set) == -1) ||  ( sigaddset(&new_set, SIGALRM )== -1 ) )
    {
	  //cout << "Error in signal mask" << endl;
	  return NULL;
    }
	duration = duration/1000;

	U_TIMERID tId= new time_node();
	tId ->original_time =  duration;
	tId ->delta_time =  duration;
	tId ->cb = cb;
	tId->mode = mode;
	
	if(sigprocmask( SIG_BLOCK, &new_set, NULL) == -1)
	{
	  //cout<<"Error blocking the Signal"<<endl;
	  return NULL;
	}
	tId->mode = mode;
	insertNode(tId);	
	timerHead->itimer.it_value.tv_sec = timerHead->delta_time;
	timerHead->itimer.it_value.tv_usec = 0;
	timerHead->itimer.it_interval.tv_sec = 0;
	timerHead->itimer.it_interval.tv_usec = 0;
	setitimer(ITIMER_REAL, &timerHead->itimer, 0);
	signal(SIGALRM, callback_timeout);
	if(sigprocmask( SIG_UNBLOCK, &new_set, NULL)==-1)
	{
	  //cout<<"Error in Unblocking the signal "<<endl;
	  return NULL;
	}
	return tId;


#endif
}


#if (defined(__linux__)|| defined(__MACH__))

void insertNode(U_TIMERID tId )
{
  if(timerHead == NULL)
	{	
		tId->next = (U_TIMERID)NULL;
		timerHead = tId;
		
	}
	
	else{
		// Update the delta time of the timer head 
		
		getitimer(ITIMER_REAL, &timerHead->itimer);
		//cout<<"The Itimer vaule :: "<<timerHead->itimer.it_value.tv_sec<<endl;
		if(timerHead->mode ==1)
		{
		    //timerHead->delta_time = timerHead->original_time;
		    timerHead->itimer.it_value.tv_sec=  timerHead->original_time;
		   // timerHead->delta_time =  timerHead->itimer.it_value.tv_sec;
		    //cout << tId << "insertNode::" << "  Original Time:" << tId->original_time << "   Delta Time " << tId->delta_time << endl;
		}
		else
		timerHead->delta_time = timerHead->itimer.it_value.tv_sec;
		//cout<<"The timer value is ::"<<timerHead->delta_time<<endl;
		

        	if(timerHead->delta_time > tId->delta_time )
        	{

			tId->next=timerHead;
			timerHead->delta_time -= tId->delta_time;
			timerHead=tId;

		}
        	else
        	{
			U_TIMERID iterator,prevNode;

			iterator= timerHead;
			prevNode = timerHead;
            		while(iterator != NULL)
            		{
				if( iterator->delta_time <= tId->delta_time ){
					tId->delta_time -= iterator->delta_time;
					prevNode = iterator;
                			iterator=iterator->next;
				}
				else
				{
				   
					break;
				}
				
            		}

      			prevNode->next = tId;
			tId->next = iterator;
			if(iterator != NULL)
			{
				iterator->delta_time -= tId->delta_time;
				
			}
        	}
    	}
}
#endif

#if (defined(__linux__)|| defined(__MACH__))
void deleteNode(U_TIMERID tId)
{
  
  U_TIMERID temporaryTimer, temp, cur,prev;
  
  temporaryTimer=timerHead;
		
  if(timerHead == tId)
		{
			if(timerHead->next!=NULL)
			{
			        //cout << "Deleting timer with ID:" << timerHead << "Original val:" << timerHead->original_time << endl;
				temporaryTimer = timerHead->next;
				temporaryTimer->delta_time+= timerHead->delta_time;
				delete timerHead;
				timerHead = temporaryTimer;
				timerHead->itimer.it_value.tv_sec = timerHead->delta_time;
				setitimer(ITIMER_REAL, &timerHead->itimer, 0);
				signal(SIGALRM, callback_timeout);
			}
			else
			{
			        //cout << "Deleting timer with ID:" << timerHead << "Original val:" << timerHead->original_time << endl;
				delete timerHead;
				timerHead = NULL;
			}

		}
		
		else
		{
		  
			while(temporaryTimer->next != NULL && temporaryTimer->next != tId)
			{
	
				temporaryTimer=temporaryTimer->next;
			}
			
			if(temporaryTimer->next == tId)
			{

				temp=temporaryTimer->next;
				temporaryTimer->next=temp->next;
				if(temp->next != NULL){
				temporaryTimer->next->delta_time += temp->delta_time;
				}
				//temporaryTimer->next += cur->delta_time;	
				delete temp;
			}		
		}
		
	
}

#endif

#if (defined(__linux__)|| defined(__MACH__))
void callback_timeout(int n)
{

	////cout << " Timer Expired" << n << endl;

    sigset_t new_set;
	
  
    if( (sigemptyset(&new_set) == -1) ||  ( sigaddset(&new_set, SIGALRM )== -1 ) )
    {
	  //cout << "Error in signal mask" << endl;
	  return;
    }
	if(timerHead!=NULL &&timerHead->cb != NULL)
	{
	    
		timerHead->cb((U_PVOID)timerHead,false);
	}
	

	if(sigprocmask( SIG_BLOCK, &new_set, NULL) == -1)
	{
	  //cout<<"Error blocking the Signal"<<endl;
	  return ;
	}

	if(timerHead != NULL &&timerHead->mode!=1)
	{
		U_TIMERID tid= timerHead;
		timerHead = timerHead->next;
		delete tid;
	}
	else if(timerHead != NULL &&timerHead->mode==1 && timerHead->delta_time>0)
	{
	    
	   // //cout<<"##################### the Delta time is ############# "<<timerHead->delta_time<<endl;
	      U_TIMERID tid= timerHead;
	      tid->delta_time = tid->original_time;
	      timerHead = tid->next;
	      tid->next = NULL;
	      insertNode(tid);
	      timerHead->itimer.it_value.tv_sec = timerHead->delta_time;
	      setitimer(ITIMER_REAL, &timerHead->itimer, 0);
	      signal(SIGALRM, callback_timeout);
	        
	}
	      // Start timer for new timer head
	while(timerHead != NULL)
	{
		if(timerHead->delta_time <= 0)
		{
		  //cout<<"The timer delta time is "<<timerHead->delta_time<<endl;
			// callback the function
			
			if(timerHead->mode ==1)
			{
			  
			  //cout<<"The timer id is ::"<<timerHead<<endl;
			  U_TIMERID tid= timerHead;
			  tid->delta_time = tid->original_time;
			  //cout<<"The Delta time !!!!!!! is "<< tid->delta_time<<endl;
			  timerHead = tid->next;
			  tid->next = NULL;
			  insertNode(tid);
			  timerHead->itimer.it_value.tv_sec = timerHead->delta_time;
			  setitimer(ITIMER_REAL, &timerHead->itimer, 0);
			  signal(SIGALRM, callback_timeout); 
			  //cout<<"after instering the tid:::::"<<tid->delta_time<<endl;
			}
			else{
			  timerHead = timerHead->next;
			//break;
			}
		}
		else{
			timerHead->itimer.it_value.tv_sec = timerHead->delta_time;
			setitimer(ITIMER_REAL, &timerHead->itimer, 0);
			signal(SIGALRM, callback_timeout);
			////cout << " Starting timer for " << timerHead->delta_time << "seconds" << endl;
			break;
		}

	}

}
#endif

/******************************************************************************
*	UKillAllTimers
*	- All Timer Kill function
******************************************************************************/

U_VOID UKillAllTimers(){
#ifdef WIN32
	if (!DeleteTimerQueue(gTimerQueue)){
        printf("DeleteTimerQueue failed (%d)\n", GetLastError());
	}

#elif (defined(__linux__)|| defined(__MACH__))
    U_TIMERID temporaryTimer;
	temporaryTimer=timerHead;

	while(temporaryTimer!=NULL)
	{
		UKillTimer(temporaryTimer);
		temporaryTimer=temporaryTimer->next;
	}

	timerHead=NULL;
#endif

	
}

/******************************************************************************
*	UKillTimer
*	- Kills the timer, whose timer ID is provided
*
*	Parameters
*	tId		- Timer ID
******************************************************************************/

U_VOID UKillTimer(U_TIMERID tId)
{
#ifdef WIN32
	if((gTimerQueue != NULL)&& (tId != NULL)){
		DeleteTimerQueueTimer(gTimerQueue, tId->timer_ID,INVALID_HANDLE_VALUE);
	}
	
	
	if(tId != NULL)
	{
		U_TIMERID timerId = reinterpret_cast<U_TIMERID>(tId);
		delete timerId;
		timerId = NULL;
	}
	
#elif (defined(__linux__)|| defined(__MACH__))
if(tId != NULL && timerHead != NULL)
{
		sigset_t new_set;
	
  
    if( (sigemptyset(&new_set) == -1) ||  ( sigaddset(&new_set, SIGALRM )== -1 ) )
    {
	  //cout << "Error in signal mask" << endl;
	  return ;
    }
  
	    
	if(sigprocmask( SIG_BLOCK, &new_set, NULL) == -1)
	{
	  //cout<<"Error blocking the Signal"<<endl;
	  return;
	}
	deleteNode(tId);
	if(sigprocmask( SIG_UNBLOCK, &new_set, NULL)==-1)
	{
	  //cout<<"Error in Unblocking the signal "<<endl;
	  return;
	}
}

#endif


}

#ifndef QTPROJECT

/******************************************************************************
*	isIpV6supported
*	Returns 1 if IPV6 issupported else returns 0
******************************************************************************/
U_BOOL isIpV6supported ()
{
#ifdef WIN32
	/* create an ipv6 socket and if call succeeds return true */
	/* cleanup */
	WSADATA wsaData;
	U_SOCKET lsock = INVALID_SOCKET;
	SOCKADDR_STORAGE serverAddr = {0};
	U_SINT32 port = 8000;

	WSAStartup(MAKEWORD(2,2), &wsaData); 

	serverAddr.ss_family = AF_INET6;
	//INETADDR_SETANY((SOCKADDR *)&serverAddr);
	SS_PORT(&serverAddr) = htons(port);
	lsock = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);
	if(lsock == INVALID_SOCKET)
	{
		return 0;
	}
	else
	{
		closesocket(lsock);
		return 1;
	}
#elif (defined(__linux__)|| defined(__MACH__))
 	return 0;
#endif
}


/******************************************************************************
*	addIPaddress
*	Add an IP address to the interface. The interface name is known to the server
*
*	Parameters
*	interfaceName_p - pointer to the interface name
*	ipAddress - ip address in string format. For ipv6 it should be in colon 
*	seperated form
*	gwAddress_p - Gateway address in string format.
*	isIpv6 - 1 for ipv6 else 0
******************************************************************************/
U_VOID addIPaddress (U_SINT8 *interfaceName_p, U_SINT8 *ipAddress_p,
					 U_SINT8 *gwAddress_p, U_BOOL isIpv6,U_SINT8 *dnsAddress_p)
{
        U_SINT8 cmdLine[400];
	
	 bool isValidIP = isValidIPv4Address(ipAddress_p);
	 bool isValidgw = isValidIPv4Address(gwAddress_p);
        if(!ipAddress_p) return;
#ifdef WIN32
	if(isIpv6)
	{
		strcpy(cmdLine, (const U_SINT8*)"netsh interface ipv6 set address ");
	}
	else
	{
		strcpy(cmdLine, (const U_SINT8*)"netsh interface ip set address ");
	}
	//CONCAT(cmdLine, "\"");
	//CONCAT(cmdLine, (U_SINT8*)interfaceName_p);
	//CONCAT(cmdLine, "\"");
	//CONCAT(cmdLine, (U_SINT8*)" ");
	//CONCAT(cmdLine, (U_SINT8*)ipAddress_p);
	CONCAT(cmdLine, "\"");
	CONCAT(cmdLine, (U_SINT8*)interfaceName_p);
	CONCAT(cmdLine, "\"");
	
	//bool isValidIP = isValidIPv4Address(ipAddress_p);
	//bool isValidgw = isValidIPv4Address(gwAddress_p);
	if(isIpv6 == false)
	{
		CONCAT(cmdLine, (U_SINT8*)" static ");
		CONCAT(cmdLine, (U_SINT8*)ipAddress_p);
		CONCAT(cmdLine, (U_SINT8*)" 255.0.0.0");
		string length = string(gwAddress_p);
		int len = strlen(gwAddress_p);
		len = strlen(length.c_str());
		if(IsWinXP())
		{
			
			if(isValidgw)
			{
				printf("GW Address = %s\n", gwAddress_p);
				CONCAT(cmdLine, (U_SINT8*)" ");
				CONCAT(cmdLine, (U_SINT8*)gwAddress_p);
				CONCAT(cmdLine, " 30\n");
			}
		}
		else
		{
			bool isVaildDns = isValidIPv4Address(dnsAddress_p);

			if(isVaildDns)
			{
				strcpy(gwAddress_p,dnsAddress_p);
			}
			else
			{
				CONCAT(cmdLine, (U_SINT8*)" ");
			}
			CONCAT(cmdLine, (U_SINT8*)gwAddress_p);
		}
	}
	else /* IPv6 case */
	{
		CONCAT(cmdLine, (U_SINT8*)" ");
		CONCAT(cmdLine, (U_SINT8*)ipAddress_p);
	}
	LOG(ULOG_INFO_LEVEL, "IP Setting : %s\n", cmdLine);
	printf("%s\n", cmdLine);
	CONCAT(cmdLine, "> NUL 2> NUL");
	system(cmdLine);

	if((isIpv6 == false) && (isValidIP) && (isValidgw))
	{
		/* Adding the Default route with the Gateway and IP Address */
		strcpy(cmdLine, (U_SINT8*)"route add ");
		CONCAT(cmdLine, (U_SINT8*)gwAddress_p);
		CONCAT(cmdLine, (U_SINT8*)" ");
		CONCAT(cmdLine, (U_SINT8*)ipAddress_p);
		printf("%s\n", cmdLine);
		system(cmdLine);
	}

	LOG(ULOG_INFO_LEVEL, "IP Setting : %s\n", cmdLine);

	CONCAT(cmdLine, "> NUL 2> NUL");

#elif __linux__
    strcpy(cmdLine, (const U_SINT8*)"ifconfig ");
    CONCAT(cmdLine, (U_SINT8*)interfaceName_p);
    CONCAT(cmdLine, (U_SINT8*)" ");
    if(isIpv6)
    {
        CONCAT(cmdLine, (U_SINT8*)"inet6 add ");
        CONCAT(cmdLine, (U_SINT8*)"ipAddress_p");
    }
    else
    {
        CONCAT(cmdLine, (U_SINT8*)"ipAddress_p");
    }
    LOG(ULOG_INFO_LEVEL, "IP Setting : %s\n", cmdLine);
    printf("%s\n", cmdLine);
    CONCAT(cmdLine, "> NUL 2> NUL");
    system(cmdLine);

    
    if((isIpv6 == false) && (isValidgw) && (isValidIP)) //temp patch since the emulator is sending space in the gateway address 
    {
            /* Adding the Default route with the Gateway and IP Address */
            strcpy(cmdLine, (U_SINT8*)"route add default gw ");
            CONCAT(cmdLine, (U_SINT8*)gwAddress_p);
            system(cmdLine);
    }
    LOG(ULOG_INFO_LEVEL, "IP Setting : %s\n", cmdLine);

#elif __MACH__
    
        strcpy(cmdLine, (const U_SINT8*)"ifconfig ");
        CONCAT(cmdLine, (U_SINT8*)interfaceName_p);
        CONCAT(cmdLine, (U_SINT8*)" ");
        if(isIpv6)
        {
            CONCAT(cmdLine, (U_SINT8*)"inet6 ");
            CONCAT(cmdLine, (U_SINT8*)"ipAddress_p");
        }
        else
        {
            CONCAT(cmdLine, (U_SINT8*)"ipAddress_p");
        }
        LOG(ULOG_INFO_LEVEL, "IP Setting : %s\n", cmdLine);
        printf("%s\n", cmdLine);
        CONCAT(cmdLine, "> NUL 2> NUL");
        system(cmdLine);

        if((isIpv6 == false) && (isValidgw) && (isValidIP))
        {
                /* require in some versions(10.6.x) to delete the existing default route,
                   as a new default gw will not be written
                */
                system("route delete default");
                /* Adding the Default route with the Gateway and IP Address */
                strcpy(cmdLine, (U_SINT8*)"route add default ");
                CONCAT(cmdLine, (U_SINT8*)gwAddress_p);
                system(cmdLine);
        }
        LOG(ULOG_INFO_LEVEL, "IP Setting : %s\n", cmdLine);
#endif /*WIN32*/
}

/******************************************************************************
*	deleteIPaddress
*	Delete an IP address to the interface. The interface name is known to the server
*
*	Parameters
*	interfaceName_p - pointer to the interface name
*	ipAddress - ip address in string format. For ipv6 it should be in colon 
*	seperated form
*	isIpv6 - 1 for ipv6 else 0
******************************************************************************/
U_VOID deleteIPaddress (U_SINT8 *interfaceName_p, U_SINT8 *ipAddress_p,
						U_BOOL isIpv6)
{
        U_SINT8 cmdLine[400];
#ifdef WIN32
	if(isIpv6)
	{
		strcpy(cmdLine, (const U_SINT8*)"netsh interface ipv6 delete address ");
	}
	else
	{
		strcpy(cmdLine, (const U_SINT8*)"netsh interface ip delete address ");
	}

	CONCAT(cmdLine, "\"");
	CONCAT(cmdLine, (U_SINT8*)interfaceName_p);
	CONCAT(cmdLine, "\"");
	CONCAT(cmdLine, (U_SINT8*)" ");
	CONCAT(cmdLine, (U_SINT8*)ipAddress_p);
	if(isIpv6 == false)
	{
		CONCAT(cmdLine, (U_SINT8*)" all");
	}
	LOG(ULOG_INFO_LEVEL, "Del : %s\n", cmdLine);
	printf("%s\n", cmdLine);
	CONCAT(cmdLine, "> NUL 2> NUL");
	system(cmdLine);
#elif __linux__
        strcpy(cmdLine, (const U_SINT8*)"ip address delete ");
        CONCAT(cmdLine, (U_SINT8*)ipAddress_p);
        CONCAT(cmdLine, (U_SINT8*)" ");
        CONCAT(cmdLine, (U_SINT8*)"dev ");
        CONCAT(cmdLine, (U_SINT8*)interfaceName_p);
        system(cmdLine);
#elif __MACH__
        strcpy(cmdLine, (const U_SINT8*)"ifconfig ");
        CONCAT(cmdLine, (U_SINT8*)interfaceName_p);
        CONCAT(cmdLine, (U_SINT8*)" remove ");
        CONCAT(cmdLine, (U_SINT8*)ipAddress_p);
        system(cmdLine);
#endif /*WIN32*/
}

/******************************************************************************
*	setIfStatus
*	Set interface up / down
*
*	Parameters
*	interfaceName_p - pointer to the interface name
*	status_p - "up" / "down"
******************************************************************************/
U_VOID setIfStatus (U_SINT8 *interfaceName_p, U_SINT8 *status_p)
{
        U_SINT8 cmdLine[400];
#ifdef WIN32
	strcpy(cmdLine, "netsh int set int ");
	CONCAT(cmdLine, "\"");
	CONCAT(cmdLine, interfaceName_p);
	CONCAT(cmdLine, "\"");
	if(!_stricmp(status_p, "up"))
	{
		CONCAT(cmdLine, " enabled");
	}
	else if(!_stricmp(status_p, "down"))
	{
		CONCAT(cmdLine, " disabled");
	}
	CONCAT(cmdLine, "> NUL 2> NUL");
	system(cmdLine);
#elif (defined(__linux__)||defined(__MACH__))
        strcpy(cmdLine, "ifconfig ");
        CONCAT(cmdLine, interfaceName_p);
        CONCAT(cmdLine, " ");
        CONCAT(cmdLine, status_p);
        CONCAT(cmdLine, "> NUL 2> NUL");
        system(cmdLine);
#endif /*WIN32*/
}

/******************************************************************************
*	setDnsAddresses
*	Add DNS addresses
*
*	Parameters
*	interfaceName_p - pointer to the interface name
*	primaryDNS_p - pointer to primary dns address
*	secondaryDNS_p - pointer to secondary dns address
*	isIPv6 - 1/0
******************************************************************************/
U_VOID setDnsAddresses (U_SINT8 *interfaceName_p, U_SINT8 *primaryDNS_p,
					U_SINT8 *secondaryDNS_p, U_BOOL isIpv6)
{
        U_SINT8 cmdLine1[200];
    (void)secondaryDNS_p;
#ifdef WIN32
	//U_SINT8 cmdLine1[200];/*, cmdLine2[200], cmdLine3[400];*/
	
	if((strlen(primaryDNS_p) == 0) || (strcmp(primaryDNS_p, " ")))
	{
		LOG(ULOG_ERROR_LEVEL, "Primary DNS invalid - %s\n", primaryDNS_p);
		return;
	}

	/* For Windows XP, The DNS address addition command is different */
	if(IsWinXP())
	{
		strcpy(cmdLine1, (const U_SINT8*)"netsh interface ip set dns ");
		CONCAT(cmdLine1, "\"");
		CONCAT(cmdLine1, interfaceName_p);
		CONCAT(cmdLine1, "\"");
		CONCAT(cmdLine1, " static ");
		CONCAT(cmdLine1, primaryDNS_p);
		LOG(ULOG_INFO_LEVEL, "DNS Setting : %s\n", cmdLine1);
		printf("%s\n", cmdLine1);
		CONCAT(cmdLine1, "> NUL 2> NUL");
		system(cmdLine1);
		return;
	}

	if(isIpv6 == TRUE)
	{
		strcpy(cmdLine1, "netsh interface ipv6 set dnsserver "); 
	}
	else
	{
		strcpy(cmdLine1, "netsh interface ip set dnsserver "); 
	}

	CONCAT(cmdLine1, "\"");
	CONCAT(cmdLine1, interfaceName_p);
	CONCAT(cmdLine1, "\"");
	CONCAT(cmdLine1, " static ");
	CONCAT(cmdLine1, primaryDNS_p);
	LOG(ULOG_INFO_LEVEL, "DNS Setting : %s\n", cmdLine1);
	printf("%s\n", cmdLine1);
	CONCAT(cmdLine1, "> NUL 2> NUL");
	system(cmdLine1);

#elif __linux__
	FILE *uconnect_File = NULL;
 
        char pDns[100]="nameserver ",sDns[100]= "nameserver ";
	
	size_t primarydns,secondarydns;
	bool  isValidprimarydns = isValidIPv4Address(primaryDNS_p);
	bool  isValidsecondarydns = isValidIPv4Address(secondaryDNS_p);
	
	if(isValidprimarydns)
	{
	  strcat(pDns,primaryDNS_p);
	  primarydns = strlen(pDns);
	  uconnect_File = fopen(FILENAME, "w");
	  if (uconnect_File== NULL) 
	  {
	  	LOG(ULOG_INFO_LEVEL, "Failed to Open the File : %s\n",FILENAME);
	  }
	  else
	  {
			char newline[2] = "\n";
			fwrite(pDns, primarydns, 1,uconnect_File );
			fwrite(newline,strlen(newline), 1,uconnect_File );
			LOG(ULOG_INFO_LEVEL, "DNS setting done!\n");
			fclose(uconnect_File);
	  }

	}
	else
	{
	    LOG(ULOG_INFO_LEVEL, "Invalid PrimaryDNS Address\n");
	    return;
	}
	
	if(isValidsecondarydns)
	{
	    strcat(sDns,secondaryDNS_p);
	    secondarydns = strlen(sDns);
	    uconnect_File = fopen(FILENAME, "a");
	    if (uconnect_File== NULL) 
	    {
		  LOG(ULOG_INFO_LEVEL, "Failed to Open the File : %s\n",FILENAME);
	    }
	    else
	    {
			  fwrite(pDns, secondarydns, 1,uconnect_File );
			  LOG(ULOG_INFO_LEVEL, "DNS setting done!\n");
			  fclose(uconnect_File);
	    }

	}
	else
	{
	    LOG(ULOG_INFO_LEVEL, "Invalid SecondaryDNS Address\n");
	    return;
	}
	
	
	


#elif __MACH__
        strcpy(cmdLine1, "networksetup -setdnsservers ");
        CONCAT(cmdLine1, primaryDNS_p);
        CONCAT(cmdLine1, " ");
        CONCAT(cmdLine1, secondaryDNS_p);
        CONCAT(cmdLine1, "> NUL 2> NUL");
        system(cmdLine1);
#endif /*WIN32*/
}

/******************************************************************************
*	disableNS
*	Disable Neighbor solicitation packets
*
******************************************************************************/
U_VOID disableNS()
{
	/* Since this is not available on all platforms, we should be calling the
	DAL API to disable NS packets in chipset */
}

/******************************************************************************
*	enableDHCP
*	Enable DHCP
*
*	Parameters
*	interfaceName_p - pointer to the interface name
*	isIPv6 - 1/0
******************************************************************************/
U_VOID enableDHCP (U_SINT8 *interfaceName_p, U_BOOL isIpv6)
{
	U_SINT8 cmdLine[400];

#ifdef WIN32
        if(isIpv6 == false)
	{
		strcpy(cmdLine, "netsh interface ip set address ");
		CONCAT(cmdLine, "\"");
		CONCAT(cmdLine, interfaceName_p);
		CONCAT(cmdLine, "\"");
		CONCAT(cmdLine, " dhcp");
		system(cmdLine);
	}
	else
	{
		/* disable router discovery */
		strcpy(cmdLine, "netsh interface ipv6 set int ");
		CONCAT(cmdLine, "\"");
		CONCAT(cmdLine, interfaceName_p);
		CONCAT(cmdLine, "\"");
		CONCAT(cmdLine, " routerdiscovery=disable");
		system(cmdLine);

		memset(cmdLine, 0, sizeof(cmdLine));
		/* enable managed address */
		strcpy(cmdLine, "netsh interface ipv6 set int ");
		CONCAT(cmdLine, "\"");
		CONCAT(cmdLine, interfaceName_p);
		CONCAT(cmdLine, "\"");
		CONCAT(cmdLine, " managedaddress=enable");
		CONCAT(cmdLine, "> NUL 2> NUL");
		system(cmdLine);
	}
#elif __linux__
        if(isIpv6 == false)
        {
            strcpy(cmdLine, "dhclient ");
        }
        else
        {
            strcpy(cmdLine, "dhclient -6 ");
        }
        CONCAT(cmdLine, interfaceName_p);
        CONCAT(cmdLine, "> NUL 2> NUL");
        system(cmdLine);
#elif __MACH__
        strcpy(cmdLine, "ipconfig set ");
        CONCAT(cmdLine, interfaceName_p);
        CONCAT(cmdLine, " DHCP");
        CONCAT(cmdLine, "> NUL 2> NUL");
        system(cmdLine);
#endif /*WIN32*/
}

/* Functions added for the Device Detection, Adapter scanning */


/******************************************************************************
*	setDnsAddresses
*	Finds the presence of an ANSI string in another ANSI string - 
*	NON CASE SENSITIVE
******************************************************************************/
#ifdef WIN32
char* __fastcall stristrA(const char* pszMain, const char* pszSub)
#else
char* __attribute__((fastcall)) stristrA(const char* pszMain, const char* pszSub)
#endif
{
#if defined(WIN32)
      char* pszTmp1;
      char* pszTmp2;
      char  lowerch, upperch;

      // We keep the first character of pszSub in lowerch and upperch (lower and
      // upper case). First we loop trying to find a match for this character. Once
      // we have found a match, we start with the second character of both pszMain
      // and pszSub and walk through both strings doing a CharLower on both
      // characters before comparing. If we make it all the way through pszSub with
      // matches, then we bail with a pointer to the string's location in pszMain.

      _asm {
            mov esi, ecx    // pszMain
                  mov edi, edx    // pszSub

                  // Check for NULL pointers

                  test ecx, ecx
                  je short NoMatch // NULL pointer for pszMain
                  test edx, edx
                  je short NoMatch // NULL pointer for pszSub

                  sub eax, eax
                  mov al, [edi]
                  push eax
                        call DWORD PTR CharLower
                        mov lowerch, al
                        push eax
                        call DWORD PTR CharUpper
                        mov upperch, al

                        push edi    // increment the second string pointer
                        call DWORD PTR CharNext
                        mov  edi, eax

                        mov pszTmp2, edi
                        mov edi, DWORD PTR CharNext // faster to call through a register

Loop1:
                  mov al, [esi]
                  test al, al
                        je short NoMatch        // end of main string, so no match
                        cmp al, lowerch
                        je short CheckString    // lowercase match?
                        cmp al, upperch
                        je short CheckString    // upppercase match?
                        push esi
                        call edi                // Call CharNext to update main string pointer
                        mov esi, eax
                        jmp short Loop1

CheckString:
                  mov pszTmp1, esi    // save current pszMain pointer in case its a match
                        push esi
                        call edi            // first character of both strings match,
                        mov  esi, eax       // so move to next pszMain character
                        mov edi, pszTmp2
                        mov al, [edi]
                        jmp short Branch1

Loop3:
                        push esi
                              call DWORD PTR CharNext    // CharNext to change pszMain pointer
                              mov  esi, eax
                              push edi
                              call DWORD PTR CharNext    // CharNext to change pszSub pointer
                              mov  edi, eax

                              mov al, [edi]
Branch1:
                              test al, al
                                    je short Match       // zero in sub string, means we've got a match
                                    cmp al, [esi]
                                    je short Loop3

                                          // Doesn't match, but might be simply a case mismatch. Lower-case both
                                          // characters and compare again

                                          sub ecx, ecx
                                          mov cl, al  // character from pszSub
                                          push ecx
                                          call DWORD PTR CharLower
                                          mov cl, al
                                          sub eax, eax
                                          mov al,  [esi]   // character from pszMain
                                          push ecx    // preserve register
                                                push eax
                                                call DWORD PTR CharLower
                                                pop ecx
                                                cmp al, cl
                                                je short Loop3  // we still have a match, keep checking

                                                // No match, put everything back, update pszMain to the next character
                                                // and try again from the top

                                                mov esi, pszTmp1
                                                mov  edi, DWORD PTR CharNext
                                                push esi
                                                call edi
                                                mov  esi, eax
                                                jmp short Loop1

Match:
                                          mov eax, pszTmp1
                                                jmp short Done  // Don't just return -- always let the C portion of the code handle the return

NoMatch:
                                          sub eax, eax
Done:
      }

      // Note lack of return in the C portion of the code. Return value is always in
      // eax register which we have set by the time we get here

#else
    return NULL;
#endif
}

void populateDeviceInfo(U_UINT8 dalMode, U_UINT32 VidPidCount, 
						U_SINT8 VidPidList[3][100], U_UINT32 APNId,
						U_SINT8* Adapter_desc, string& com_port,
						U_SINT8* Adapter_name, U_SINT8* HW_Addr,
						U_SINT8* Adapter_index)
{

	getMallocLock();
    U_SINT32 devfound = 0;
	/* Here, the OSAL APIs for the USB device search and the Adapter 
	   description need to be called */
	devfound = IsDevicePresent(dalMode, VidPidCount, 
		VidPidList, &APNId, Adapter_desc,
        com_port);

	if(devfound)	/* Yes, we have a match! */
	{
		adapter_info(Adapter_name, Adapter_desc, HW_Addr, Adapter_index);
	}
	releaseMallocLock();
}

/******************************************************************************
*	ComPortFinder
*	Finds the COM Port associated with the a particular Modem
******************************************************************************/
bool ComPortFinder(string& ComPort, U_SINT8* hwID)
{
#if defined(WIN32)
	HDEVINFO hdevinfo = SetupDiGetClassDevs(&GUID_DEVCLASS_PORTS, 
						NULL, 
						NULL, 
						DIGCF_PRESENT);
	if (hdevinfo == INVALID_HANDLE_VALUE)
	{	
		printf("Invalid Handle = %s\n", hdevinfo);
		return FALSE;
	}

	SP_DEVINFO_DATA devinfodata;
	devinfodata.cbSize = sizeof(SP_DEVINFO_DATA); 
	int iIndex = 0;

	while (SetupDiEnumDeviceInfo(hdevinfo, iIndex, &devinfodata))
	{
		HKEY hkey = SetupDiOpenDevRegKey(hdevinfo, 
										&devinfodata, 
										DICS_FLAG_GLOBAL, 
										0, 
										DIREG_DEV, 
										KEY_QUERY_VALUE);

		if (hkey)
		{
			TCHAR szPort[256];
			DWORD dwSize = sizeof(szPort);
			DWORD dwType = 0;
			if ((RegQueryValueEx(hkey, 
								L"PortName", 
								NULL, 
								&dwType, 
								(LPBYTE)szPort, 
								&dwSize) == ERROR_SUCCESS))
			{
				if(dwType == REG_SZ)
				{
					TCHAR szPortName[256];
					DWORD dwSizeName = sizeof(szPortName);
					size_t convSize;
					char COMPort[100];
					char nstring[100];
					strcpy(COMPort, "");
					wcstombs_s(&convSize, COMPort, wcslen(szPort) + 1, szPort, 256); 
					//printf("szPort = %s\n", COMPort);
					if (SetupDiGetDeviceRegistryProperty(hdevinfo, 
						&devinfodata, 
						SPDRP_HARDWAREID, 
						NULL, 
						(PBYTE)szPortName, 
						dwSizeName,
						&dwSizeName) && (dwType == REG_SZ))
					{
						wcstombs_s(&convSize, nstring, wcslen(szPortName) + 1, szPortName, 256); 
						//printf("szPortName = %s\n", nstring);
						if(!_strnicmp(nstring, hwID, strlen(hwID)))
						{
							printf("This is the COM port we need = %s\n", COMPort);
							string tempCOMPort(COMPort);
							ComPort.clear();
							ComPort += tempCOMPort;
							RegCloseKey(hkey);
							BOOL ret = SetupDiDestroyDeviceInfoList(hdevinfo);
							if(ret == false)
							{
								//cout << "error in closing SetupDiDestroyDeviceInfoList()" << GetLastError() << endl;
							}
							return true;
						}
					}
					else
					{
						
					}
				}
			}
			//RegCloseKey(hkey);
		}
		iIndex++;
	}

	//printf("Error if Any = %d\n", GetLastError());
	//Cleaning
	SetupDiDestroyDeviceInfoList(hdevinfo);
#endif /* QTPROJECT */
	return false;
}

/******************************************************************************
*	ComPortFinder_3GDevices
*	Finds the COM Port associated with the a particular Modem
******************************************************************************/
bool ComPortFinder_3GDevices(string& ComPort)
{
#if defined(WIN32)
	HDEVINFO hdevinfo = SetupDiGetClassDevs(&GUID_DEVCLASS_PORTS, 
						NULL, 
						NULL, 
						DIGCF_PRESENT);
	if (hdevinfo == INVALID_HANDLE_VALUE)
	{	
		printf("Invalid Handle = %s\n", hdevinfo);
		return FALSE;
	}

	SP_DEVINFO_DATA devinfodata;
	devinfodata.cbSize = sizeof(SP_DEVINFO_DATA); 
	int iIndex = 0;

	while (SetupDiEnumDeviceInfo(hdevinfo, iIndex, &devinfodata))
	{
		HKEY hkey = SetupDiOpenDevRegKey(hdevinfo, 
										&devinfodata, 
										DICS_FLAG_GLOBAL, 
										0, 
										DIREG_DEV, 
										KEY_QUERY_VALUE);

		if (hkey)
		{
			TCHAR szPort[256];
			DWORD dwSize = sizeof(szPort);
			DWORD dwType = 0;
			if ((RegQueryValueEx(hkey, 
								L"PortName", 
								NULL, 
								&dwType, 
								(LPBYTE)szPort, 
								&dwSize) == ERROR_SUCCESS))
			{
				if(dwType == REG_SZ)
				{
					TCHAR szPortName[256];
					DWORD dwSizeName = sizeof(szPortName);
					size_t convSize;
					char COMPort[100];
					char nstring[100];
					strcpy(COMPort, "");
					wcstombs_s(&convSize, COMPort, wcslen(szPort) + 1, szPort, 256); 
					//printf("szPort = %s\n", COMPort);
					if (SetupDiGetDeviceRegistryProperty(hdevinfo, 
						&devinfodata, 
						SPDRP_DEVICEDESC, 
						NULL, 
						(PBYTE)szPortName, 
						dwSizeName,
						&dwSizeName) && (dwType == REG_SZ))
					{
						wcstombs_s(&convSize, nstring, wcslen(szPortName) + 1, szPortName, 256); 
						//printf("szPortName = %s\n", nstring);
						if(!_stricmp(nstring, "HUAWEI Mobile Connect - 3G PC UI Interface"))
						{
							printf("This is the COM port we need = %s\n", COMPort);
							string tempCOMPort(COMPort);
							ComPort.clear();
							ComPort += tempCOMPort;
							RegCloseKey(hkey);
							BOOL ret = SetupDiDestroyDeviceInfoList(hdevinfo);
							if(ret == false)
							{
								//cout << "error in closing SetupDiDestroyDeviceInfoList()" << GetLastError() << endl;
							}
							return true;
						}
					}
					else
					{
						
					}
				}
			}
			//RegCloseKey(hkey);
		}
		iIndex++;
	}

	//printf("Error if Any = %d\n", GetLastError());
	//Cleaning
	SetupDiDestroyDeviceInfoList(hdevinfo);
#endif /* QTPROJECT */
	return false;
}

/******************************************************************************
*	IsDevicePresent
*	Finds the presence of the USB Device in the VID_PID List of the CM
******************************************************************************/
int IsDevicePresent(U_UINT8 dalMode,
					U_UINT32 VidPidCount,
					U_SINT8 VidPidList[3][100],
					U_UINT32* APNId,
					U_SINT8* Adapter_desc,
					U_STRING com_port)
{
      U_BOOL            Found    = false;
#ifdef WIN32
      HDEVINFO        DeviceInfoSet;
      SP_DEVINFO_DATA DeviceInfoData;
      DWORD           i        = 0;
	  U_UINT32        VidCount = 0;
      
	  if(dalMode == 2)
	  {
		  DeviceInfoSet = SetupDiGetClassDevs(&GUID_DEVCLASS_PORTS, // All Classes
				NULL, //enumerator is USB bus
				NULL, 
				DIGCF_PRESENT ); // All devices present on system
	  }
	  else if(dalMode == 1)
	  {
		  DeviceInfoSet = SetupDiGetClassDevs(&GUID_DEVCLASS_NET, // All Classes
				L"USB", //enumerator is USB bus
				NULL, 
				DIGCF_ALLCLASSES | DIGCF_PRESENT); // All devices present on system
	  }
	  else
	  {
		  DeviceInfoSet = SetupDiGetClassDevs(&GUID_DEVCLASS_NET, // All Classes
				L"USB", //enumerator is USB bus
				NULL, 
				DIGCF_ALLCLASSES | DIGCF_PRESENT); // All devices present on system
	  }

      if (DeviceInfoSet == INVALID_HANDLE_VALUE)
      {
            //DisplayError(TEXT("GetClassDevs(All Present Devices)"));        
			LOG(ULOG_INFO_LEVEL, "GetClassDevs(All Present Devices)");
            return 1;
      }
      DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
      for (i=0;SetupDiEnumDeviceInfo(DeviceInfoSet,i,&DeviceInfoData);i++)
      {
            DWORD DataT;
            LPTSTR buffer = NULL;
            DWORD buffersize = 0;


            while (!SetupDiGetDeviceRegistryProperty(
                  DeviceInfoSet,
                  &DeviceInfoData,
                  SPDRP_HARDWAREID,
                  &DataT,
                  (PBYTE)buffer,
                  buffersize,
                  &buffersize))
            {
                  if (GetLastError() == ERROR_INVALID_DATA)
                  {
                        break;
                  }
                  else if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
                  {
                        if (buffer) 
                              LocalFree(buffer);
                        buffer = (LPTSTR)LocalAlloc(LPTR,buffersize);
                  }
                  else
                  {
                        goto cleanup_DeviceInfo;
                  }            
            }

            if (GetLastError() == ERROR_INVALID_DATA) 
                  continue;

            //for (p=buffer;*p&&(p<&buffer[buffersize]);p+=lstrlen(p)+sizeof(TCHAR))
            {                       
				  for(VidCount = 0; VidCount < VidPidCount; VidCount++)
                  {
					    size_t convSize;
						char nstring[100];
                        //DisplayMessage("%s - %s \n",p,g_VidPidList[VidCount]);
					    //printf("%s - %s \n",p,g_VidPidList[VidCount]);
						/*LOG(ULOG_DEBUG_LEVEL, "Length of CLASS = %d\n", 
							wcslen(buffer));*/
						wcstombs_s(&convSize, nstring, wcslen(buffer) + 1, buffer, 100); 
                        /* Case insensitive search ﾖ our own api*/
						if (!strnicmp(nstring,VidPidList[VidCount], 
							strlen(VidPidList[VidCount])))
                        {
                              //DisplayMessage(" --Found %s --\n", g_VidPidList[VidCount]);
							  LOG(ULOG_INFO_LEVEL, 
								" --Found %s --\n", VidPidList[VidCount]);
							  printf(" --Found %s --\n", VidPidList[VidCount]);

							  /* Get the MI id from the VID_PID value */
							  std::string vidpid(VidPidList[VidCount]);
							  U_UINT32 pos = vidpid.find("Mi_");
							  std::string tmpid = vidpid.substr(pos + 3, 2);
							  *APNId = atoi(tmpid.c_str());

							  if(pos == string::npos)
							  {
								pos = vidpid.find("MI_");
								std::string tmpid = vidpid.substr(pos + 3, 2);
								*APNId = atoi(tmpid.c_str());
							  }

							  if(pos == string::npos)
							  {
								pos = vidpid.find("mi_");
								std::string tmpid = vidpid.substr(pos + 3, 2);
								*APNId = atoi(tmpid.c_str());
							  }

							  LOG(ULOG_INFO_LEVEL, "APN ID = %d\n", 
								  *APNId);
                              Found = TRUE;
							  /* Extracting COM PORT */
							  if (!SetupDiGetDeviceRegistryProperty(
								  DeviceInfoSet,
								  &DeviceInfoData,
								  SPDRP_FRIENDLYNAME,
								  &DataT,
								  (PBYTE)buffer,
								  buffersize,
								  &buffersize))
							  {
								  LOG(ULOG_ERROR_LEVEL, 
									  "Error in getting Description - Error %d \n",
									  GetLastError());
							  }
							  else
							  {
								  wcstombs_s(&convSize, Adapter_desc, 
									  wcslen(buffer) + 1, buffer, 100); 
								  LOG(ULOG_INFO_LEVEL, 
									  "Adapter Description = %s\n", 
									  Adapter_desc);
								  string temp_COM(Adapter_desc);
								  pos = temp_COM.find("(COM");
								  U_UINT32 endpos = temp_COM.find(")");
								  tmpid = temp_COM.substr(pos + 1, (endpos - pos - 1));
								  com_port = tmpid;
							  }

							  /* Extracting Interface name */
							  if (!SetupDiGetDeviceRegistryProperty(
								  DeviceInfoSet,
								  &DeviceInfoData,
								  SPDRP_DEVICEDESC,
								  &DataT,
								  (PBYTE)buffer,
								  buffersize,
								  &buffersize))
							  {
								  LOG(ULOG_ERROR_LEVEL, 
									  "Error in getting Description - Error %d \n",
									  GetLastError());
							  }
							  else
							  {
								  wcstombs_s(&convSize, Adapter_desc, 
									  wcslen(buffer) + 1, buffer, 100); 
								  LOG(ULOG_INFO_LEVEL, 
									  "Adapter Description = %s\n", 
									  Adapter_desc);
								  string temp_COM(Adapter_desc);
							  }
                              break;
                        }
                  }
                  if(Found)
                        break;
            }

            if (buffer) 
			{
                  LocalFree(buffer);
				  buffer = NULL;
			}
            if(Found)
                  break;
      }

cleanup_DeviceInfo:
      SetupDiDestroyDeviceInfoList(DeviceInfoSet);
#endif
      return Found; 
}

/******************************************************************************
*	adapter_info
*	Finds the information for the chosen Adapter, to be used for IP config
******************************************************************************/
int adapter_info(U_SINT8* Adapter_name,
				 U_SINT8* Adapter_desc,
				 U_SINT8* HW_Addr,
				 U_SINT8* Adapter_index)
{
#ifdef WIN32
    /* Declare and initialize variables */

// It is possible for an adapter to have multiple
// IPv4 addresses, gateways, and secondary WINS servers
// assigned to the adapter. 
//
// Note that this sample code only prints out the 
// first entry for the IP address/mask, and gateway, and
// the primary and secondary WINS server for each adapter. 

    PIP_ADAPTER_INFO pAdapterInfo;
    PIP_ADAPTER_INFO pAdapter = NULL;
    DWORD dwRetVal = 0;
    UINT i;

/* variables used to print DHCP time info */

    ULONG ulOutBufLen = sizeof (IP_ADAPTER_INFO);
    pAdapterInfo = (IP_ADAPTER_INFO *) MALLOC(sizeof (IP_ADAPTER_INFO));
    if (pAdapterInfo == NULL) {
		LOG(ULOG_ERROR_LEVEL, 
			"Error allocating memory needed to call GetAdaptersinfo\n");
        return 1;
    }
// Make an initial call to GetAdaptersInfo to get
// the necessary size into the ulOutBufLen variable
    if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW) {
        FREE(pAdapterInfo);
        pAdapterInfo = (IP_ADAPTER_INFO *) MALLOC(ulOutBufLen);
        if (pAdapterInfo == NULL) {
            LOG(ULOG_ERROR_LEVEL,
				"Error allocating memory needed to call GetAdaptersinfo\n");
            return 1;
        }
    }

    if ((dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) == NO_ERROR) {
        pAdapter = pAdapterInfo;
        while (pAdapter) {
            LOG(ULOG_DEBUG_LEVEL,"\tComboIndex: \t5d\n", 
				pAdapter->ComboIndex);
            LOG(ULOG_DEBUG_LEVEL,"\tAdapter Name: \t%s\n", 
				pAdapter->AdapterName);
			printf("SEARCHED ADAPTER NAME = %s\n", pAdapter->Description);
			if(!strncmp(Adapter_desc, pAdapter->Description, 
				strlen(Adapter_desc)))
			{
				LOG(ULOG_INFO_LEVEL, 
					"FOUND A MATCH = %s\n", pAdapter->AdapterName);
				strcpy(Adapter_name, pAdapter->AdapterName);
				printf("ADAPTER NAME DISCOVERED = %s\n", Adapter_name);
				char subaddr[10];
				for (i = 0; i < pAdapter->AddressLength; i++) 
				{
					if (i == (pAdapter->AddressLength - 1))
					{
#if 0
						//printf("%.2X\n", (int) pAdapter->Address[i]);
						if((int)(pAdapter->Address[i]&0x00ff) < 0x10)
						{
							subaddr[0] = '0';
							//itoa((int)pAdapter->Address[i]&0x00ff, &subaddr[1], 16);
							sprintf(&subaddr[1], "%02x", (int)pAdapter->Address[i]&0x00ff);
						}
						else
						{
							//itoa((int)pAdapter->Address[i]&0x00ff, subaddr, 16);
							sprintf(subaddr, "%02x", (int)pAdapter->Address[i]&0x00ff);
						}
#endif /* if 0 */
						//itoa((int)pAdapter->Address[i]&0x00ff, subaddr, 16);
						sprintf(subaddr, "%02x", (int)pAdapter->Address[i]&0x00ff);
						strcat(HW_Addr, subaddr);
					}
					else
					{
#if 0
						//printf("%.2X-", (int) pAdapter->Address[i]);
						if((int)(pAdapter->Address[i]&0x00ff) < 0x10)
						{
							subaddr[0] = '0';
							//itoa((int)pAdapter->Address[i]&0x00ff, &subaddr[1], 16);
							sprintf(&subaddr[1], "%02x", (int)pAdapter->Address[i]&0x00ff);
						}
						else
						{
							itoa((int)pAdapter->Address[i]&0x00ff, subaddr, 16);
							sprintf(subaddr, "%02x", (int)pAdapter->Address[i]&0x00ff);
						}
#endif /* if 0 */
						sprintf(subaddr, "%02x", (int)pAdapter->Address[i]&0x00ff);
						strcat(HW_Addr, subaddr);
						strcat(HW_Addr, ":");
					}
				}
				/* Copy Adapter Index for IP Configuration */
				//itoa(pAdapter->Index, gCMInfo.Adapter_index, 10);
				sprintf(Adapter_index, "%d", pAdapter->Index);
			}

            LOG(ULOG_DEBUG_LEVEL,"\tAdapter Desc: \t%s\n", 
				pAdapter->Description);
            LOG(ULOG_DEBUG_LEVEL,"\tAdapter Addr: \t%s\n", 
				HW_Addr);
            LOG(ULOG_DEBUG_LEVEL,"\tIndex: \t%d\n", pAdapter->Index);
            LOG(ULOG_DEBUG_LEVEL,"\tType: \t");
#if 0
            switch (pAdapter->Type) {
            case MIB_IF_TYPE_OTHER:
                printf("Other\n");
                break;
            case MIB_IF_TYPE_ETHERNET:
                printf("Ethernet\n");
                break;
            case MIB_IF_TYPE_TOKENRING:
                printf("Token Ring\n");
                break;
            case MIB_IF_TYPE_FDDI:
                printf("FDDI\n");
                break;
            case MIB_IF_TYPE_PPP:
                printf("PPP\n");
                break;
            case MIB_IF_TYPE_LOOPBACK:
                printf("Lookback\n");
                break;
            case MIB_IF_TYPE_SLIP:
                printf("Slip\n");
                break;
            default:
                printf("Unknown type %ld\n", pAdapter->Type);
                break;
            }

            printf("\tIP Address: \t%s\n",
                   pAdapter->IpAddressList.IpAddress.String);
            printf("\tIP Mask: \t%s\n", pAdapter->IpAddressList.IpMask.String);

            printf("\tGateway: \t%s\n", pAdapter->GatewayList.IpAddress.String);
            printf("\t***\n");


            if (pAdapter->DhcpEnabled) {
                printf("\tDHCP Enabled: Yes\n");
                printf("\t  DHCP Server: \t%s\n",
                       pAdapter->DhcpServer.IpAddress.String);

                printf("\t  Lease Obtained: ");
                /* Display local time */
                error = _localtime32_s(&newtime, (__time32_t*) &pAdapter->LeaseObtained);
                if (error)
                    printf("Invalid Argument to _localtime32_s\n");
                else {
                    // Convert to an ASCII representation 
                    error = asctime_s(buffer, 32, &newtime);
                    if (error)
                        printf("Invalid Argument to asctime_s\n");
                    else
                        /* asctime_s returns the string terminated by \n\0 */
                        printf("%s", buffer);
                }

                printf("\t  Lease Expires:  ");
                error = _localtime32_s(&newtime, (__time32_t*) &pAdapter->LeaseExpires);
                if (error)
                    printf("Invalid Argument to _localtime32_s\n");
                else {
                    // Convert to an ASCII representation 
                    error = asctime_s(buffer, 32, &newtime);
                    if (error)
                        printf("Invalid Argument to asctime_s\n");
                    else
                        /* asctime_s returns the string terminated by \n\0 */
                        printf("%s", buffer);
                }
            } else
                printf("\tDHCP Enabled: No\n");
            if (pAdapter->HaveWins) {
                printf("\tHave Wins: Yes\n");
                printf("\t  Primary Wins Server:    %s\n",
                       pAdapter->PrimaryWinsServer.IpAddress.String);
                printf("\t  Secondary Wins Server:  %s\n",
                       pAdapter->SecondaryWinsServer.IpAddress.String);
            } else
                printf("\tHave Wins: No\n");
#endif /* 0 */
            pAdapter = pAdapter->Next;
            //printf("\n");
        }
		printf("\tADAPTER ADDRESS : %s\n", HW_Addr);
    } 
	else 
	{
		LOG(ULOG_ERROR_LEVEL, 
			"GetAdaptersInfo failed with error: %d\n", dwRetVal);
    }
    if (pAdapterInfo)
	{
        FREE(pAdapterInfo);
		pAdapterInfo = NULL;
	}
#endif
    return 0;
}
#endif

/******************************************************************************
*	IsWinXP
*	Finds the Windows version 
******************************************************************************/
bool IsWinXP() {
#ifdef WIN32
    DWORD version = GetVersion();
    DWORD major = (DWORD) (LOBYTE(LOWORD(version)));
    DWORD minor = (DWORD) (HIBYTE(LOWORD(version)));

    return ((major == 5) && (minor == 1));
#endif
    return false;    
}

/******************************************************************************
*	NAStoIPv6AddrConverter
*	Converts a NAS string to an IPv6 address with ::fe80 prefixed 
******************************************************************************/
void NAStoIPv6AddrConverter(U_SINT8* NASString, U_SINT8* IPv6Addr)
{
	U_SINT8* tempNAS = NASString;
	U_SINT8 strFound1[10], hex1[10];
	U_SINT8 strFound2[10], hex2[10];
	U_SINT8* ipv6 = IPv6Addr;

	if(strlen(tempNAS) == 0)
	{
		strcpy(ipv6, "");
		return;
	}

	/* Append the Link-local prefix to this address */
	strcpy(ipv6, "fe80::");

	/* Skipping the first 8 octets of the Received message */
	for(int j = 0; j < 8; j++)
	{
		int i = strcspn(tempNAS, ".");
		tempNAS += (i + 1);
	}
	/*-----------------------------------------------------*/

	for(int k = 0; k < 3; k++)
	{
		int i = strcspn(tempNAS, ".");
		int j = strxfrm(strFound1, tempNAS, i);
		strFound1[i] = '\0';
		int num = atoi(strFound1);
		sprintf(hex1, "%02x", num);
		tempNAS += (i + 1);

		i = strcspn(tempNAS, ".");
		j = strxfrm(strFound2, tempNAS, i);
		strFound2[i] = '\0';
		num = atoi(strFound2);
		sprintf(hex2, "%02x", num);
		tempNAS += (i + 1);	

		strcat(ipv6, hex1);
		strcat(ipv6, hex2);
		strcat(ipv6, ":");
	}
	int i = strcspn(tempNAS, ".");
	int j = strxfrm(strFound1, tempNAS, i);
	strFound1[i] = '\0';
	int num = atoi(strFound1);
	sprintf(hex1, "%02x", num);
	tempNAS += (i + 1);

	i = strcspn(tempNAS, ".");
	j = strxfrm(strFound2, tempNAS, i);
	strFound2[i] = '\0';
	num = atoi(strFound2);
	sprintf(hex2, "%02x", num);
	tempNAS += (i + 1);	

	strcat(ipv6, hex1);
	strcat(ipv6, hex2);
}

/******************************************************************************
*	IPv4v6AddressSplitter
*	Splits a NAS string to an IPv4 and IPv6 address 
******************************************************************************/
void IPv4v6AddressSplitter(U_SINT8* NASString, U_SINT8* IPv6Addr, 
						   U_SINT8* IPv4Addr)
{
	U_SINT8* tempNAS = NASString;
	U_UINT32 dotcount = 0;
	/* Find 8 dots in the string and that would be the IPv6 string */
	for(U_UINT32 i = 0; i < 8; i++)
	{
		dotcount = strcspn(tempNAS, ".");
		tempNAS += (dotcount + 1);
	}
	strcpy(IPv4Addr, tempNAS);
	U_UINT32 Ipv4_len = strlen(tempNAS);
	U_UINT32 Ipv6_len = strlen(NASString) - Ipv4_len;
	strcpy(IPv6Addr, NASString);
	IPv6Addr[Ipv6_len - 1] = '\0';
}

/******************************************************************************
*	disableRouterDiscovery
*	Disable Router Discovery on the Interface 
******************************************************************************/
void disableRouterDiscovery(U_SINT8 *interfaceName_p)
{
	U_SINT8 cmdLine[400];

	strcpy(cmdLine, (const U_SINT8*)"netsh interface ipv6 set interface ");
	CONCAT(cmdLine, "\"");
	CONCAT(cmdLine, interfaceName_p);
	CONCAT(cmdLine, "\"");
	CONCAT(cmdLine, " routerdiscovery=disabled");
	system(cmdLine);
}

/******************************************************************************
*	enableRouterDiscovery
*	Enable Router Discovery on the Interface 
******************************************************************************/
void enableRouterDiscovery(U_SINT8 *interfaceName_p)
{
	U_SINT8 cmdLine[400];

	strcpy(cmdLine, (const U_SINT8*)"netsh interface ipv6 set interface ");
	CONCAT(cmdLine, "\"");
	CONCAT(cmdLine, interfaceName_p);
	CONCAT(cmdLine, "\"");
	CONCAT(cmdLine, " routerdiscovery=enabled");
	system(cmdLine);

	/* For Windows XP, Renew the IPv6 interfaces to Enable Router Solicitation */
	if(IsWinXP())
	{
		memset(cmdLine, 0, sizeof(cmdLine));
		strcpy(cmdLine, (const U_SINT8*)"netsh interface ipv6 renew");
		system(cmdLine);
	}
	else
	{
		strcpy(cmdLine, (const U_SINT8*)"netsh interface ipv6 set interface ");
		CONCAT(cmdLine, "\"");
		CONCAT(cmdLine, interfaceName_p);
		CONCAT(cmdLine, "\"");
		CONCAT(cmdLine, " routerdiscovery=enabled");
		system(cmdLine);
	}

}


#ifdef WIN32
int snprintf1(char *str,size_t size,const char *fmt,...)
{
	int ret;
	va_list ap;
	va_start(ap,fmt);
	ret = vsnprintf(str,size,fmt,ap);
	str[size-1] = '\0';
	va_end(ap);
	return ret;
}
#endif

#ifndef QTPROJECT
/******************************************************************************
*	isRSRAComplete
*	Function to check if RS/RA negotiation has completed for IPv6 
******************************************************************************/
U_UINT32 isRSRAComplete(U_CSTR Adapter_desc)
{
#if   defined (WIN32)
    /* Declare and initialize variables */

    DWORD dwSize = 0;
    DWORD dwRetVal = 0;

	U_UINT32 i = 0;

    // Set the flags to pass to GetAdaptersAddresses
    ULONG flags = GAA_FLAG_INCLUDE_PREFIX;

    // default to ipv6 address family 
	ULONG family = AF_INET6;

    LPVOID lpMsgBuf = NULL;

    PIP_ADAPTER_ADDRESSES pAddresses = NULL;
    ULONG outBufLen = 0;
    ULONG Iterations = 0;

    PIP_ADAPTER_ADDRESSES pCurrAddresses = NULL;
    PIP_ADAPTER_UNICAST_ADDRESS pUnicast = NULL;
    PIP_ADAPTER_ANYCAST_ADDRESS pAnycast = NULL;
    PIP_ADAPTER_MULTICAST_ADDRESS pMulticast = NULL;
    IP_ADAPTER_DNS_SERVER_ADDRESS *pDnServer = NULL;
    IP_ADAPTER_PREFIX *pPrefix = NULL;

    // Allocate a 15 KB buffer to start with.
    outBufLen = WORKING_BUFFER_SIZE;

    do 
	{
        pAddresses = (IP_ADAPTER_ADDRESSES *) MALLOC(outBufLen);
        if (pAddresses == NULL) 
		{
            printf
                ("Memory allocation failed for IP_ADAPTER_ADDRESSES struct\n");
            return ROUTER_ADV_FAILURE;
        }

        dwRetVal =
            GetAdaptersAddresses(family, flags, NULL, pAddresses, &outBufLen);

        if (dwRetVal == ERROR_BUFFER_OVERFLOW) 
		{
            FREE(pAddresses);
            pAddresses = NULL;
        } 
		else 
		{
            break;
        }

        Iterations++;

    } while ((dwRetVal == ERROR_BUFFER_OVERFLOW) && (Iterations < MAX_TRIES));

    if (dwRetVal == NO_ERROR) 
	{
		size_t convSize;
		char nstring[100];
        // If successful, output some information from the data we received
        pCurrAddresses = pAddresses;
		while (pCurrAddresses) 
		{
			wcstombs_s(&convSize, nstring, wcslen(pCurrAddresses->Description) + 1, 
				pCurrAddresses->Description, 100); 
			/* Check for the Adapter description that we are looking for */
			if(!strnicmp(Adapter_desc, nstring, strlen(Adapter_desc)))
			{		
				pUnicast = pCurrAddresses->FirstUnicastAddress;
				if (pUnicast != NULL) 
				{
					for (i = 0; pUnicast != NULL; i++)
					{
						//printf("PREFIX ORIGIN = %d \n", pUnicast->PrefixOrigin);
						if(pUnicast->PrefixOrigin == IpPrefixOriginRouterAdvertisement)
						{
							printf("ROUTER ADVERTISEMENT DONE \n");
							if (pAddresses) 
							{
								FREE(pAddresses);
							}
							return ROUTER_ADV_SUCCESS;
						}
						pUnicast = pUnicast->Next;
					}
				} 
				else
				{
					printf("\tNo Unicast Addresses\n");
				}
			}
			pCurrAddresses = pCurrAddresses->Next;
		}
	}
	else 
	{
        printf("Call to GetAdaptersAddresses failed with error: %d\n",
               dwRetVal);
        if (dwRetVal == ERROR_NO_DATA)
            printf("\tNo addresses were found for the requested parameters\n");
        else 
		{
            if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
                    FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, 
                    NULL, dwRetVal, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),   
                    // Default language
                    (LPTSTR) & lpMsgBuf, 0, NULL)) 
			{
                printf("\tError: %s", lpMsgBuf);
                LocalFree(lpMsgBuf);
                if (pAddresses)
                    FREE(pAddresses);
				return ROUTER_ADV_FAILURE;
            }
        }
    }
    if (pAddresses) 
	{
        FREE(pAddresses);
	}
	return ROUTER_ADV_FAILURE;
    
#elif  (defined(__linux__)|| defined(__MACH__))
	return ROUTER_ADV_FAILURE;
#else
    return ROUTER_ADV_FAILURE;
#endif /* WIN32 */
}

/******************************************************************************
*	isValidIPv4Address
*	Function to check if the IPv4 address is a valid one 
******************************************************************************/
U_BOOL isValidIPv4Address(U_CSTR ipv4_string)
{
#ifdef WIN32
    U_DWORD ulAddr = INADDR_NONE;
    // Declare and initialize variables
    WSADATA wsaData;
    U_UINT32 iResult;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        LOG(ULOG_ERROR_LEVEL, "WSAStartup failed: %d\n", iResult);
        return false;
    }

	//--------------------------------
	// Call inet_addr(). If the call succeeds,
	// the result variable will hold a IN_ADDR
    ulAddr = inet_addr(ipv4_string);
    if ( ulAddr == INADDR_NONE ) {
		LOG(ULOG_ERROR_LEVEL, "inet_addr failed and returned INADDR_NONE\n");
        WSACleanup();
        return false;
    }   
    
    if (ulAddr == INADDR_ANY) {
        LOG(ULOG_ERROR_LEVEL, "inet_addr failed and returned INADDR_ANY\n");
        WSACleanup();
        return false;  
    }

    LOG(ULOG_INFO_LEVEL, "inet_addr returned success\n");
    WSACleanup();
	return true;

#elif  (defined(__linux__)|| defined(__MACH__))
	
	
    struct sockaddr_in sa;
    int result = inet_pton(AF_INET,ipv4_string , &(sa.sin_addr));
    if(result==1)
    {
       LOG(ULOG_INFO_LEVEL, "inet_addr returned success\n");
      return true;
    }
    else
      return false;

	
#endif
}

//bool ComPortFinder(string& ComPort)
bool getComPortFromDevMngsModemSection(string& ComPort)
{
    //WriteLog("start point of getComPortFromDevMngsModemSection()");
#if defined(WIN32)
	HDEVINFO hdevinfo = SetupDiGetClassDevs(&GUID_DEVCLASS_MODEM, 
						NULL, 
						NULL, 
						DIGCF_PRESENT);
	if (hdevinfo == INVALID_HANDLE_VALUE)
	{
//		WriteLog("Error in SetupDiGetClassDevs() call");
		//printf("Invalid Handle = %s\n", hdevinfo);
		return FALSE;
	}
//	WriteLog("Success in SetupDiGetClassDevs() call");

	SP_DEVINFO_DATA devinfodata;
	devinfodata.cbSize = sizeof(SP_DEVINFO_DATA); 
	int iIndex = 0;

	while (SetupDiEnumDeviceInfo(hdevinfo, iIndex, &devinfodata))
	{
		HKEY hkey = SetupDiOpenDevRegKey(hdevinfo, 
										&devinfodata, 
										DICS_FLAG_GLOBAL, 
										0, 
										DIREG_DEV, 
										KEY_QUERY_VALUE);

		if (hkey)
		{
			TCHAR szPort[256];
			DWORD dwSize = sizeof(szPort);
			DWORD dwType = 0;
			if ((RegQueryValueEx(hkey, 
								L"PortName", 
								NULL, 
								&dwType, 
								(LPBYTE)szPort, 
								&dwSize) == ERROR_SUCCESS))
			{
				if(dwType == REG_SZ)
				{
					TCHAR szPortName[256];
					DWORD dwSizeName = sizeof(szPortName);
					size_t convSize;
					char COMPort[100];
					char nstring[100];
					strcpy(COMPort, "");
					wcstombs_s(&convSize, COMPort, wcslen(szPort) + 1, szPort, 256); 
					//printf("szPort = %s\n", COMPort);
					if (SetupDiGetDeviceRegistryProperty(hdevinfo, 
						&devinfodata, 
						SPDRP_DEVICEDESC, 
						NULL, 
						(PBYTE)szPortName, 
						dwSizeName,
						&dwSizeName) && (dwType == REG_SZ))
					{
						wcstombs_s(&convSize, nstring, wcslen(szPortName) + 1, szPortName, 256); 
						//printf("szPortName = %s\n", nstring);
						if(!_stricmp(nstring, "Broadcom 4G Modem"))
						{
							//WriteSerLogStr("This is the COM port we need = ",(string)COMPort);
							//printf("This is the COM port we need = %s\n", COMPort);
							string tempCOMPort(COMPort);
							ComPort.clear();
							ComPort += tempCOMPort;
							//gCMInfo.com_port = ComPort;
							//WriteSerLogStr("gCMInfo.com_port--",gCMInfo.com_port);
							RegCloseKey(hkey);
							BOOL ret = SetupDiDestroyDeviceInfoList(hdevinfo);
							if(ret == false)
							{
//								WriteLog("error in closing SetupDiDestroyDeviceInfoList()");
								////cout << "error in closing SetupDiDestroyDeviceInfoList()" << GetLastError() << endl;
							}
							//gCMInfo.device_status = 1;
							return true;
						}
					}
					else
					{
						
					}
				}
			}
			//RegCloseKey(hkey);
		}
		iIndex++;
	}

	//printf("Error if Any = %d\n", GetLastError());
	//Cleaning
	SetupDiDestroyDeviceInfoList(hdevinfo);
#endif /* QTPROJECT */
	return false;
}



#if 0
U_BOOL handleConsoleCloseMessage()
{
//#ifndef QTPROJECT
	if (SetConsoleCtrlHandler((PHANDLER_ROUTINE)ConsoleHandler,TRUE)==FALSE)
	{
		LOG(ULOG_ERROR_LEVEL, "Unable to install SetConsoleCtrlHandler!\n");
		return false;;
	}
	LOG(ULOG_INFO_LEVEL, "Installed SetConsoleCtrlHandler successfully\n");
	return true;
//#endif
}
#endif

string getExecDirectory(string exe)
{
#ifdef WIN32
	size_t convSize;
	WCHAR wideBuffer[2000];
	GetModuleFileName(NULL,wideBuffer,2000);
	GetModuleFileName(NULL,wideBuffer,2000);

	char buffer[2000];

	wcstombs_s(&convSize, buffer, wcslen(wideBuffer) + 1, wideBuffer, 256); 
	string serverPath = buffer;
	string serverName = exe;
	string dirPath = "";
	for(unsigned int i=0;i<serverPath.length() - serverName.length();i++)
	{
		dirPath += serverPath.at(i);
	}
	return dirPath;
#elif  (defined(__linux__)|| defined(__MACH__))
    
    char path [2000];
    getcwd(path,sizeof(path));
    string dirPath = path;
    dirPath = dirPath + "/";
    return dirPath;
#endif //WIN32

}

#endif






