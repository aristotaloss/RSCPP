#pragma once

#include "ClientSocket.h"

class ClientSocket;

class PacketSender {
private:
	ClientSocket *socket;
public:
	PacketSender(ClientSocket *socket);
	~PacketSender();

	void sendMapBase(int rx, int ry);
	void sendClearScreen(void);
	void sendPlayerInformation(void);
	void sendCameraReset(void);
};

