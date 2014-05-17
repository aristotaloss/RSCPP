#pragma once

#include "Player.h"

#define MAX_PLAYERS 2048

class Player;

class PlayerUpdater {

private: 
	Player *player;
	int *playerIndices;
	int *playersNeedingMaskUpdate;
	int maskUpdatePtr;

	void encodeMyMovement(Packet *);
	void encodeOtherMovement(Packet *);
	void encodeNewPlayers(Packet *);
	void encodePlayerSettings(Packet *);
public:
	PlayerUpdater(Player *);
	~PlayerUpdater();
	void update(void);
	void reset(void);
};

