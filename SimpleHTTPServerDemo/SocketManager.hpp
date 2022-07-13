#pragma once
#include <iostream>
#include <WS2tcpip.h>
class SocketManager {
private:
	SOCKET server_socket;
	SOCKET client_socket;

	sockaddr_in server_details;
	sockaddr_in client_details;
public:


};