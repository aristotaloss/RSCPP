#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include "ServerSocket.h"
#include "Logging.h"
#include "World.h"

#pragma comment(lib, "Ws2_32.lib")

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

DWORD WINAPI logicThreadMethod(LPVOID param) {
	while (true) {
		World::process();
		Sleep(600);
	}
}

int main(void) {
	int error;
	//_CrtSetBreakAlloc(173);
	ServerSocket *server = new ServerSocket("43594", &error);
	logf("We are listening on port 43594.\n");

	CreateThread(NULL, 0, &logicThreadMethod, 0, 0, 0);

	while (true) {
		ClientSocket *client = server->acceptSocket();
		logf("Accepted new client: @0x%X\n", (int)client);
	}

	return 0;
}