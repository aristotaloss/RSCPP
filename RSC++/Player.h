#pragma once

#include "ClientSocket.h"

class ClientSocket;

class Player {
private:
	ClientSocket *socket;
public:
	Player();
	~Player();
};

