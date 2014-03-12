#pragma once

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

#include "ClientSocket.h"
#include "Logging.h"

#pragma comment(lib, "Ws2_32.lib")

class ServerSocket {

public:
	ServerSocket(const char *port, int *error);
	~ServerSocket();
	void setHints(addrinfo *hints, int family, int type, IPPROTO protocol, int flags);
	void cleanup(void);
	PClientSocket acceptSocket(void);
	SOCKET getSocket();
	SOCKET *getPSocket();

private:
	WSAData wsa;
	struct addrinfo *result;
	SOCKET listenSocket;
	char *port;

};

typedef ServerSocket *PServerSocket;