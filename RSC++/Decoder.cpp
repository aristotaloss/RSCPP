#include "Decoder.h"


Decoder::Decoder(ClientSocket *connection) {
	this->connection = connection;
}

Decoder::~Decoder() {
}

void Decoder::decode(char *buffer, int opcode, int len) {
	logf("Incoming opcode: %d\n", opcode);

	if (this->connection->getGameState() == GameState::PRE_GAME) {
		decodePreGame(buffer, opcode, len);
	}
}

void Decoder::decodePreGame(char *buffer, int opcode, int len) {
	if (opcode == OPCODE_LOGIN_REQUEST) {
		Packet *packet = new Packet(11);

		for (int i = 0; i < 8; i++)
			packet->writeByte(0);

		packet->writeByte(2); // Response
		packet->writeByte(2); // Rights
		packet->writeByte(0); // Flagged

		connection->writePacket(packet);
	}
}
