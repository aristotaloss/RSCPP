#pragma once

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")

class ClientSocket {
private:
	SOCKET socket;
public:
	ClientSocket(SOCKET);
	~ClientSocket();
};

typedef ClientSocket *PClientSocket;

