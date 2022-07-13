#include <iostream>
#include <WS2tcpip.h>

#pragma comment (lib, "ws2_32.lib")


int main() {

	WSADATA winsock_data;
	WORD version = MAKEWORD(2,2);
	int winsock_ok = WSAStartup(version, & winsock_data);
	if (winsock_ok != 0) { 	// Do check to see if any errors occurred while creating the WSADATA variable.
		std::cerr << "Unable to Initialize WinSock! Exiting Program." << std::endl;
		return EXIT_FAILURE;
	}
	
	SOCKET server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket == INVALID_SOCKET) { //Do a check to see if we successfully created a socket for the server. 
		std::cerr << "Unable to create a socket for the server! Exiting Program." << std::endl;
		return EXIT_FAILURE;
	}

	sockaddr_in server_details;
	server_details.sin_family = AF_INET;
	server_details.sin_port = htons(54000);
	server_details.sin_addr.S_un.S_addr = INADDR_ANY;

	bind(server_socket, (sockaddr*)&server_details, sizeof(server_details));

	listen(server_socket, SOMAXCONN);

	sockaddr_in client_details;

	int client_size = sizeof(client_details);

	SOCKET client_socket = accept(server_socket, (sockaddr*)&client_details, &client_size);
	if (client_socket == INVALID_SOCKET) {//Do a check to see if we successfully created a socket for the client. 
		std::cerr << "Unable to create a socket for the server! Exiting Program." << std::endl;
		return EXIT_FAILURE;
	}

	char host_name[NI_MAXHOST] = { 0 };
	char service_name[NI_MAXHOST] = { 0 };

	if (0 == getnameinfo((sockaddr*)&client_details, client_size, host_name, NI_MAXHOST, service_name, NI_MAXHOST, 0)) {
		std::cout << host_name << " connected on port " << service_name << std::endl;
	}
	else {
		inet_ntop(AF_INET, &client_details.sin_addr, host_name, NI_MAXHOST);
		std::cout << host_name << " connected on port " << ntohs(client_details.sin_port) << std::endl;
	}

	closesocket(server_socket);

	char buffer[4096];
	while (true) {
		memset(buffer, 0, 4096);
		int bytes_recieved = recv(client_socket, buffer, 4096, 0);
		if (bytes_recieved == SOCKET_ERROR || bytes_recieved == 0) {
			std::cerr << "Unable to recieve bytes from socket! Exiting Program." << std::endl;
			break;
			//return EXIT_FAILURE;
		}
		
		send(client_socket, buffer, bytes_recieved + 1, 0); //  If no problems arise, send the data back to the client 
	
	}

	closesocket(client_socket);
	WSACleanup();

}
