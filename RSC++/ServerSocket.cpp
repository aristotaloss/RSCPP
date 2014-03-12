#include "ServerSocket.h"

/*
Construct a new ServerSocket with the port and a pointer to any (possible) error code from WSA actions
 */
ServerSocket::ServerSocket(const char *port, int *error) {
	// We need the port for later, so we save it
	this->port = new char[strlen(port) + 1];
	strcpy_s(this->port, strlen(port) + 1, port);

	int res = WSAStartup(MAKEWORD(2, 2), &wsa);

	// Set the socket hints
	addrinfo *pHints = new addrinfo;
	memset(pHints, 0, sizeof(addrinfo));
	setHints(pHints, AF_INET, SOCK_STREAM, IPPROTO_TCP, AI_PASSIVE);

	// Obtain the address info based on the hints, and free up the hints.
	res = getaddrinfo(NULL, port, pHints, &result);
	delete pHints;

	// Create a new socket
	listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (listenSocket == INVALID_SOCKET) {
		logf("Error at socket(): %d\n", WSAGetLastError());
		*error = WSAGetLastError();
		freeaddrinfo(result);
		WSACleanup();
		return;
	}

	// Bind to the address and port
	res = bind(listenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (res == SOCKET_ERROR) {
		logf("Bind failed with error: %d\n", WSAGetLastError());
		*error = WSAGetLastError();
		freeaddrinfo(result);
		closesocket(listenSocket);
		WSACleanup();
		return;
	}

	// Free the address results and listen on the port, accepting connections.
	freeaddrinfo(result);
	if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR) {
		logf("Listen failed with error: %d\n", WSAGetLastError());
		*error = WSAGetLastError();
		closesocket(listenSocket);
		WSACleanup();
	}
}

void ServerSocket::binad() {

}

SOCKET ServerSocket::getSocket() {
	return listenSocket;
}

SOCKET *ServerSocket::getPSocket() {
	return &listenSocket;
}

void ServerSocket::setHints(addrinfo *hints, int family, int type, IPPROTO protocol, int flags) {
	hints->ai_family = family;
	hints->ai_socktype = type;
	hints->ai_protocol = protocol;
	hints->ai_flags = flags;
}

ServerSocket::~ServerSocket() {
	logf("ServerSocket destroyed!\n");

	cleanup();
	delete[] this->port;
}

void ServerSocket::cleanup(void) {
	WSACleanup();
}

PClientSocket ServerSocket::acceptSocket(void) {
	SOCKET sc = INVALID_SOCKET;

	// Accept a client socket
	sc = accept(listenSocket, NULL, NULL);
	if (sc == INVALID_SOCKET) {
		logf("accept failed: %d\n", WSAGetLastError());
		return NULL;
	}

	return new ClientSocket(sc);
}