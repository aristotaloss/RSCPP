#pragma once

#include "Player.h"

class Player;

class World {
private:
	World();
	~World();
public:
	static void process();
	static int addPlayer(Player *player);
	static bool removePlayer(Player *player);
	static Player *getPlayer(int index);
};

static Player *players[2048];

