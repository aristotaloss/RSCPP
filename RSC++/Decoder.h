#pragma once

#include "Logging.h"
#include "ClientSocket.h"

#define OPCODE_LOGIN_REQUEST 14
#define OPCODE_LOGIN_DATA 16
#define OPCODE_LOGIN_RECONNECT 18

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

