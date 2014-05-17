#pragma once

#include "ClientSocket.h"
#include <queue>
#include "PendingPacket.h"
#include "UpdateSet.h"
#include "Tile.h"
#include "PlayerUpdater.h"
#include "Looks.h"
#include "MovementProcessor.h"

class ClientSocket;
class PlayerUpdater;
class Looks;
class MovementProcessor;

class Player {
private:
	ClientSocket *socket;
	std::queue<PendingPacket *> *packetqueue;
	bool ingame;
	int index;
	UpdateSet *updateSet;
	Tile *tile;
	Tile *sceneBase;
	PlayerUpdater *updater;
	Looks *looks;
	MovementProcessor *movement;
	int uid;
	char *username;
	char *password;
public:
	Player(ClientSocket *channel);
	~Player();
	void setInGame(bool);
	bool isInGame(void);
	void schedulePacket(PendingPacket *packet);
	std::queue<PendingPacket *> *getPacketQueue(void);
	void setIndex(int);
	int getIndex(void);
	void processPendingPackets(void);
	void teleport(int x, int z);
	ClientSocket *getSocket(void);
	PlayerUpdater *getUpdater(void);
	Tile *getTile(void);
	void setSceneBase(int, int);
	Tile *getSceneBase(void);
	UpdateSet *getUpdateSet(void);
	Looks *getLooks(void);
	void sendMessage(const char *);
	void sendMessagef(const char *, ...);
	void setUsername(char *);
	void setPassword(char *);
	void setUID(int);
	char *getUsername(void);
	char *getPassword(void);
	int getUID(void);
	MovementProcessor *getMovement(void);
	void animate(int);
};

