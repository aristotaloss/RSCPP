#pragma once

#include "Logging.h"
#include "ClientSocket.h"

#define OPCODE_LOGIN_REQUEST 14

// Forward declaration
class ClientSocket;

class Decoder {
private:
	ClientSocket *connection;
	void decodePreGame(char *buffer, int opcode, int len);
public:
	Decoder(ClientSocket *connection);
	~Decoder();
	void decode(char *buffer, int opcode, int len);
};

