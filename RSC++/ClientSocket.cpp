#include "ClientSocket.h"

ClientSocket::ClientSocket(SOCKET sock) {
	socket = sock;
	alive = true;
	gameState = GameState::PRE_GAME;
	readState = ReadState::PACKET_OPCODE;
	received = 0;
	decoder = new Decoder(this);
	sender = new PacketSender(this);
	player = new Player(this);

	CreateThread(NULL, 0, &readThreadMethod, (LPVOID) this, 0, ((DWORD *)&readThreadId));
	logf("Read thread id: %d\n", (LPVOID) this);
}

ClientSocket::~ClientSocket() {
	// Delete the decoder and packet sender because we won't use that anymore
	delete decoder;
	delete sender;
}

int ClientSocket::read(char *buffer, int size) {
	int rd = recv(this->socket, (char *)buffer, size, MSG_WAITALL);
	return rd;
}

int ClientSocket::write(char *buffer, int size) {
	send(this->socket, buffer, size, 0);
	return 0;
}

void ClientSocket::setAlive(bool alive) {
	this->alive = alive;
}

bool ClientSocket::isAlive(void) {
	return this->alive;
}

char *ClientSocket::getDataBuffer(void) {
	return this->dataBuffer;
}

void ClientSocket::allocateData(int len) {
	dataBuffer = (char *)malloc(len);
}

void ClientSocket::freeDataBuffer(void) {
	if (dataBuffer != NULL)
		free(dataBuffer);
}

int ClientSocket::getCurrentOpcode(void) {
	return this->currentOpcode;
}

int ClientSocket::getCurrentSize(void) {
	return this->currentSize;
}

void ClientSocket::setCurrentSize(int amt) {
	this->currentSize = amt;
}

void ClientSocket::setCurrentOpcode(int op) {
	this->currentOpcode = op;
}

ReadState ClientSocket::getReadState(void) {
	return this->readState;
}

GameState ClientSocket::getGameState(void) {
	return this->gameState;
}

void ClientSocket::setReadState(ReadState state) {
	readState = state;
}

void ClientSocket::setGameState(GameState state) {
	gameState = state;
}

/*
 Gets the size of a packet by a specific opcode, automatically determining 
 whether login-based or prelogin-based sizes need to be used.
*/
int ClientSocket::getPacketSize(int pid) {
	if (gameState == GameState::PRE_GAME) {
		if (pid == 14) {
			return 1;
		} else if (pid == 16 || pid == 18) {
			return -1;
		}
	} else if (gameState == GameState::INGAME) {
		return PACKET_SIZES[pid];
	}

	return 0;
}

/*
Gets the amount currently received in the active buffer
*/
int ClientSocket::getReceived(void) {
	return received;
}

/*
Set the amount currently received in the active buffer
*/
void ClientSocket::setReceived(int amt) {
	received = amt;
}

Decoder *ClientSocket::getDecoder(void) {
	return this->decoder;
}

/*
Writes a packet to the buffer and automatically frees the allocated packet
*/
void ClientSocket::writePacket(Packet *packet) {
	send(this->socket, (char *) packet->getBuffer(), packet->getWritePos(), 0);

	packet->cleanup(); /* Release buffer */
	delete packet;
}

/*
Disconnects this client.
*/
void ClientSocket::disconnect(void) {

}

PacketSender *ClientSocket::getSender(void) {
	return this->sender;
}

Player *ClientSocket::getPlayer(void) {
	return this->player;
}

/*
Main thread method for the byte reading
*/
DWORD WINAPI readThreadMethod(LPVOID param) {
	ClientSocket *sck = (ClientSocket *)param;

	while (true) {
		switch (sck->getReadState()) {

		case PACKET_OPCODE: 
		{
			char *buf = (char *)malloc(1);
			int amt = sck->read(buf, 1);
			if (amt < 1) {
				logf("Error in reading: %d\n", WSAGetLastError());
				free(buf);
				sck->disconnect();
				return 0;
			}

			int opcode = buf[0] & 0xFF;
			logf("Incoming packet: %d (%d)\n", opcode, amt);
			free(buf);

			sck->setCurrentOpcode(opcode);
			int psize = sck->getPacketSize(opcode);
			if (psize == -1 || psize == -2) {
				sck->setCurrentSize(psize);
				sck->setReadState(ReadState::PACKET_SIZE);
			} else {
				sck->setReadState(ReadState::PACKET_DATA);
				sck->freeDataBuffer();
				sck->setCurrentSize(psize);
				sck->allocateData(psize);

				logf("Waiting for %d bytes...\n", psize);
				break;
			}
		}

		case PACKET_SIZE: {
			int len = sck->getCurrentSize() == -1 ? 1 : 2;
			char *buf = (char *)malloc(len);
			int amt = sck->read(buf, len);
			if (amt < 1) {
				free(buf);
				return 0;
			}

			int psize = buf[0] & 0xFF;
			if (sck->getCurrentSize() == -2) {
				psize <<= 8;
				psize |= (buf[1] & 0xFF);
			}

			sck->setReadState(ReadState::PACKET_DATA);
			sck->freeDataBuffer();
			sck->setCurrentSize(psize);
			sck->allocateData(psize);

			free(buf);
			logf("Waiting for %d bytes...\n", psize);
		}

		case PACKET_DATA: 
		{
			// Allocate how much we need
			int psize = sck->getCurrentSize();
			char *buf = NULL;
			int amt = 0;

			if (psize > 0) {
				buf = (char *)malloc(psize);

				// Read the required bytes
				amt = sck->read(buf, psize);
				if (amt < 1) {
					free(buf);
					sck->disconnect();
					logf("Error in reading: %d\n", WSAGetLastError());
					return 0;
				}
			}
			
			Player *player = sck->getPlayer();
			if (player->isInGame()) {
				player->schedulePacket(new PendingPacket(sck->getCurrentOpcode(), sck->getCurrentSize(), (uint8_t *) buf));
			} else {
				sck->getDecoder()->decode(buf, sck->getCurrentOpcode(), psize);
				free(buf);
				_CrtDumpMemoryLeaks();
			}

			sck->setReadState(ReadState::PACKET_OPCODE);
			
		}

		}
	}

	logf("Terminated.\n");
	return 0;
}