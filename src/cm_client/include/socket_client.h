
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma once

int init_client();
SOCKET connect_to_server();
int send_data_to_server(SOCKET ConnectSocket, char* data, int len);
int read_data_from_server(SOCKET ConnectSocket, char* recv_data, int* len, int recvbuflen = 512);

