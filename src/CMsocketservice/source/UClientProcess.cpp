#include "UClientProcess.h"



U_MUTEX	multiClientLock;
U_MUTEX	sendDataLock;
extern UClientProcess *gProcessClient;


UClientProcess::UClientProcess()
{
	tServerSocket = INVALID_SOCKET;
}

UClientProcess::~UClientProcess()
{
	close_server_socket(tServerSocket);
	tServerSocket = INVALID_SOCKET;
}

U_BOOL UClientProcess::EventHandler()
{
	lookForClient();
	return true;
}


void UClientProcess::lookForClient()
{
	U_SINT32 count = 0;
//	U_SINT32 argIdx;
	U_SINT8 port[10] = {DEFAULT_SERVER_PORT};
	/* initialize the mutex here */
	initialize_lock(&multiClientLock);
	//initialize_lock(&sendDataLock); /* moved to service main */
	tServerSocket = create_init_server(DEFAULT_SERVER_IP, port);
	//assert( tServerSocket != INVALID_SOCKET);
	if(tServerSocket == INVALID_SOCKET)
	{
		return;
	}
	//WriteLog("create_init_server() called \n");

	for(;;) 
	{
		if(tServerSocket == INVALID_SOCKET)
		{
			return;
		}
		//WriteLog( "Waiting for client...\n" );
		U_SOCKET client_socket = accept_client( tServerSocket );
		gClientDetails[count].clientSock = client_socket;
		assert( client_socket != INVALID_SOCKET);
		//WriteLog( "Got a client! .. Processing\n" );

		/* spawn out the client as a seperate thread. here the assumption is that
		the maxiumum concurrent threads will be UCONN_MAX_CLIENTS_SUPPORTED, after which
		the handler space will be reused */
                #ifdef WIN32
		if(!(gClientDetails[count].threadHandle = 
			spawn_thread(NULL, 0, (LPTHREAD_START_ROUTINE)process_client, 
			(U_THREAD_ARG)&client_socket, 0, NULL)))
		{
			/* exit here - no way to run normally */
			//WriteLog("Thread creation failed\n");
			break;
		}
                #elif (defined(__linux__)|| defined(__MACH__))
                   int handle  = pthread_create(&gClientDetails[count].threadHandle, 
                                                NULL,
	         				process_client,
                                                (void*)&client_socket);
                   if(handle != 0) 
                   {
                       printf("Thread creation failed with error code : %d\n", handle);
                       break;
                   }
                #endif
		((count + 1) == UCONN_MAX_CLIENTS_SUPPORTED)? count = 0: count++;
		//printf("%s:%s: Count %d\n", __FILE__, __FUNCTION__, count);

	}
	/* delete the lock */
	delete_lock(&multiClientLock);
	delete_lock(&sendDataLock);
}

/******************************************************************************
*	Process a client request. The server per se, can receive several requests
*	concurrently if multiple clients are connected. the server is oblivious to
*	the number of clients and processes all requests as they come. the locks
*	in the SAPI layer will prevent any set/get race conditions.
*	
*	Parameters
*	ClientSocket - socket descriptor of the client
*
*	Returns
*	0	-	Success
*	-1	-	Failure
******************************************************************************/
static U_THREAD_RETURN_TYPE process_client (U_THREAD_ARG arg_p)
{
	if(gProcessClient == NULL)
	{
		return 0;
	}

	U_SINT8 recvbuf[DEFAULT_BUFLEN];
	U_SINT32 iResult, iSendResult;
	U_SINT32 recvbuflen = DEFAULT_BUFLEN;
	U_SINT32 rcvLen = 0;
	U_SINT32 responseLen = 0;
	U_SOCKET ClientSocket = *(U_SOCKET*)arg_p;

	/* Receive until the peer shuts down the connection */
	//printf("%s: Enter\n", __FUNCTION__);
	do 
	{
	//	try {
			iResult = rcv_data(ClientSocket, recvbuf, &rcvLen, recvbuflen);
			if (iResult == 0) 
			{
				//printf("Bytes received: %d\n", rcvLen);
				/* process the data / command */

			        UEvent *rspEvent = NULL;
					uconn_ProcessRecvdClientData(recvbuf, rcvLen,&rspEvent);

				//printf("******************************%d: RSPEVENT %d \n", rspEvent->getCmdId(), rspEvent->getSize());

			    if(rspEvent != NULL)
			    {
				take_lock(&sendDataLock);
				iSendResult = send_data(ClientSocket, (U_SINT8 *) rspEvent, rspEvent->getSize());
				release_lock(&sendDataLock);
			    }
                                
			    delete rspEvent;
			    rspEvent = NULL;
				//printf("%s: iSendResult %d\n", __FUNCTION__, iSendResult);
			    if (iSendResult != 0) 
			    {
                                #ifdef WIN32
				printf("send failed: %d\n", WSAGetLastError());
				closesocket(ClientSocket);
				WSACleanup();
				return -1;
				#elif (defined(__linux__)|| defined(__MACH__))
				pthread_exit(NULL);
				#endif
			    }
			} 
			else 
			{
				//printf("recv failed: %d result %d\n", WSAGetLastError(), iResult);
                                #ifdef WIN32
				closesocket(ClientSocket);
				return -1;
				#elif (defined(__linux__)|| defined(__MACH__))
				printf("Receive failed\n");
				close(ClientSocket);
				pthread_exit(NULL);
				#endif
			}
		//}
	//	catch(bad_alloc& ba) {
	//		printf("Out of Memory!\n");
	//	}
	//	catch(exception& e) {
	//		printf("Unknown Exception received\n");
	//	}


	} while (iResult == 0);
        printf("Done processing client \n");
	return 0;
}


/******************************************************************************
*	Process a client request. The server per se, can receive several requests
*	concurrently if multiple clients are connected. the server is oblivious to
*	the number of clients and processes all requests as they come. the locks
*	in the SAPI layer will prevent any set/get race conditions.
*	
*	Parameters
*	recvbuf_p - Pointer to the client sent request
*	recvbuflen - Length of the client buffer
*	responseLen_p - pointer to the response that server will send
******************************************************************************/
static U_VOID uconn_ProcessRecvdClientData(U_SINT8* reqbuf, U_SINT32 reqbuflen, UEvent** rspEvent)
{
	UEvent *reqFromClient_p;

	//printf("%s: Entered------------->\n", __FUNCTION__);
	reqFromClient_p = reinterpret_cast<UEvent*>(reqbuf);
	take_lock(&multiClientLock);

	//printf("%s:%s:%d: Calling SAPI_UC_Execute---------->\n", __FILE__, __FUNCTION__, __LINE__);
	//*rspEvent = SAPI_UC_Execute(reqFromClient_p);
	*rspEvent = UCM_Execute(reqFromClient_p);
	release_lock(&multiClientLock);
}






