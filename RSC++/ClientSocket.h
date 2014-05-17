#ifndef CLIENTSOCKET_H_
#define CLIENTSOCKET_H_ 1

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <cstdint>
#include <Windows.h>
#include "Logging.h"
#include "Packet.h"
#include "Decoder.h"
#include "PacketSender.h"
#include "Player.h"
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#pragma comment(lib, "Ws2_32.lib")

// Forward declaration
class Decoder;
class PacketSender;
class Player;

class ClientSocket {
private:
	SOCKET socket;
	int writeThreadId;
	int readThreadId;
	bool alive;
	enum ReadState readState;
	enum GameState gameState;
	int currentOpcode;
	int currentSize;
	char *dataBuffer;
	int received;
	Decoder *decoder;
	PacketSender *sender;
	Player *player;
public:
	ClientSocket(SOCKET);
	~ClientSocket();
	int ClientSocket::read(char *buffer, int size);
	bool ClientSocket::isAlive(void);
	void ClientSocket::setAlive(bool alive);
	int ClientSocket::write(char *buffer, int size);
	char *getDataBuffer(void);
	void allocateData(int len);
	int getCurrentOpcode(void);
	int getCurrentSize(void);
	void freeDataBuffer(void);
	ReadState getReadState(void);
	GameState getGameState(void);
	int getPacketSize(int pid);
	void setReadState(ReadState state);
	void setGameState(GameState state);
	int getReceived(void);
	void setReceived(int amt);
	void setCurrentSize(int amt);
	void setCurrentOpcode(int op);
	void writePacket(Packet *packet);
	Decoder *getDecoder(void);
	void disconnect(void);
	PacketSender *getSender(void);
	Player *getPlayer(void);
};

typedef ClientSocket *PClientSocket;
DWORD WINAPI readThreadMethod(LPVOID param);

const int PACKET_SIZES[] = {
	0, 0, 0, 1, -1, 0, 0, 0, 0, 0, // 0-9
	0, 0, 0, 0, 8, 0, 1, 2, 2, 0, // 10-19
	0, 2, 0, 0, 0, 10, 0, 0, 0, 0, // 20-29
	0, 0, 0, 0, 0, 0, 0, 0, 0, 2, // 30-39
	2, 6, 0, 6, 0, 0, 0, 0, 0, 0, // 40-49
	0, 0, 0, 4, 0, 0, 0, 0, 0, 0, // 50-59
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 60-69
	6, 0, 2, 2, 8, 0, 0, -1, 0, 6, // 70-79
	0, 0, 0, 0, 0, 0, 4, 6, 0, 0, // 80-89
	0, 0, 0, 0, 0, 3, 0, 0, -1, 0, // 90-99
	0, 13, 0, -1, 0, 0, 0, 0, 0, 0, // 100-109
	0, 0, 0, 0, 0, 0, 0, 6, 0, 0, // 110-119
	0, 0, 6, 0, 0, 0, -1, 0, 0, 6, // 120-129
	0, 0, 6, 8, 0, 6, 0, 0, 0, 2, // 130-139
	0, 0, 0, 0, 0, 6, 0, 0, 0, 0, // 140-149
	0, 0, 0, 0, 0, 2, 0, 0, 0, 0, // 150-159
	0, 0, 0, 0, -1, 0, 0, 0, 0, 0, // 160-169
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 170-179
	0, 0, 0, 0, 0, 2, 0, 0, 8, 0, // 180-189
	0, 0, 12, 0, 0, 0, 0, 0, 0, 0, // 190-199
	0, 0, 0, 0, 0, 0, 0, 0, 4, 0, // 200-209
	4, 0, 0, 0, 7, 8, 0, 0, 8, 0, // 210-219
	0, 0, 0, 0, 0, 0, -1, 0, 0, 0, // 220-229
	0, 0, 0, 0, 0, 0, 6, 8, 0, 0, // 230-239
	0, 4, 0, 0, 0, 0, 0, 0, -1, 4, // 240-249
	0, 0, 6, 6, 0, 0 }; // 250-255

typedef enum ReadState {
	PACKET_OPCODE, PACKET_SIZE, PACKET_DATA
};

typedef enum GameState {
	PRE_GAME, INGAME
};

#endif