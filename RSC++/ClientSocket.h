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

#pragma comment(lib, "Ws2_32.lib")

// Forward declaration
class Decoder;

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
	int getReceived(void);
	void setReceived(int amt);
	void setCurrentSize(int amt);
	void setCurrentOpcode(int op);
	void writePacket(Packet *packet);
	Decoder *getDecoder(void);
};

typedef ClientSocket *PClientSocket;
DWORD WINAPI readThreadMethod(LPVOID param);

const int PACKET_SIZES[] = {
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 0-9
0, 0, 0, 0, 1, 0, 0, 0, 0, 0, // 10-19
0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

typedef enum ReadState {
	PACKET_OPCODE, PACKET_SIZE, PACKET_DATA
};

typedef enum GameState {
	PRE_GAME, INGAME
};

#endif