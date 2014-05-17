#pragma once

#include "Logging.h"
#include "ClientSocket.h"
#include "World.h"
#include "CommandProcessor.h"

/* Pre-Game Opcodes */
#define OPCODE_LOGIN_REQUEST 14
#define OPCODE_LOGIN_DATA 16
#define OPCODE_LOGIN_RECONNECT 18

/* Game Opcodes */
#define OPCODE_MOUSE_CLICK 241
#define OPCODE_COMMAND 103
#define OPCODE_WALK_SCENE 164
#define OPCODE_WALK_MINIMAP 248

/* Login Responses */
#define RESPONSE_PROCEED 0
#define RESPONSE_WAIT 1
#define RESPONSE_ACCEPTED 2
#define RESPONSE_INVALID 3
#define RESPONSE_BANNED 4
#define RESPONSE_ALREADY_ON 5
#define RESPONSE_OUT_OF_DATE 6
#define RESPONSE_WORLD_FULL 7
#define RESPONSE_LOGIN_SERVER_OFF 8
#define RESPONSE_IP_AMOUNT_EXCEEDED 9
#define RESPONSE_BAD_SESSION 10
#define RESPONSE_REJECTED_SESSION 11
#define RESPONSE_NON_MEMBERS 12
#define RESPONSE_COULD_NOT_COMPLETE 13
#define RESPONSE_SERVER_BEING_UPDATED 14


// Forward declaration
class ClientSocket;

class Decoder {
private:
	ClientSocket *connection;
	void decodePreGame(char *buffer, int opcode, int len);
	void decodeInGame(char *buffer, int opcode, int len);
public:
	Decoder(ClientSocket *connection);
	~Decoder();
	void decode(char *buffer, int opcode, int len);
};

