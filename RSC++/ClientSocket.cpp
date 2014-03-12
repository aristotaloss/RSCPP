#include "ClientSocket.h"


ClientSocket::ClientSocket(SOCKET sock) {
	socket = sock;
}


ClientSocket::~ClientSocket() {
	printf("ClientSocket destroyed!");
}
