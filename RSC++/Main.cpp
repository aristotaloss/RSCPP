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

	while (true) {
		ClientSocket *client = server->acceptSocket();
		logf("Accepted new client: @0x%X\n", (int)client);
	}

	/*uint8_t *buffer = new uint8_t[256];
	memset(buffer, 0, 256); 
	int readcnt = client->read(buffer, 256);*/

	//logf("Amount read: %d.\n", readcnt); 
	//logf("Read: %s\n", buffer);

	//delete server;
	//delete client;

	system("pause");

	return 0;
}