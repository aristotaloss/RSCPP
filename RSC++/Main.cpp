#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include "ServerSocket.h"
#include "Logging.h"

#pragma comment(lib, "Ws2_32.lib")

int main(void) {
	int error;
	ServerSocket *server = new ServerSocket("43594", &error);

	logf("We are listening on port 43594.\n");

	PClientSocket client = server->acceptSocket();
	logf("Accepted client: %d\n", (int)client);

	char *buffer = (char *) malloc(256);
	memset(buffer, 0, 256);
	//int read_ = recv(client, buffer, 256, 0);

	//printf("Amount read: %d.\n", read_);
	logf("Read: %s\n", buffer);

	logf("deleting!\n");
	delete server;
	logf("deleting2!\n");
	delete client;
	int a;
	std::cin >> a;
	return 0;
}