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
		Packet *packet = new Packet(17);

		for (int i = 0; i < 8; i++)
			packet->writeByte(0);

		packet->writeByte(0); // Response code
		packet->writeLong(0);

		connection->writePacket(packet);
	} else if (opcode == OPCODE_LOGIN_DATA) {
		Packet *packet = new Packet(3);

		packet->writeByte(2); // Response code
		packet->writeByte(2); // Rights
		packet->writeByte(0); // Flagged

		connection->writePacket(packet);
		connection->setGameState(GameState::INGAME);

		connection->getSender()->sendPlayerInformation();
		connection->getSender()->sendCameraReset();
		connection->getSender()->sendMapBase(3222 / 8, 3222 / 8);
		connection->getSender()->sendClearScreen();
	}
}
