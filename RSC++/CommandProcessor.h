#pragma once

#include "Player.h"
#include <string>

class Player;

class CommandProcessor {
public:
	CommandProcessor();
	~CommandProcessor();
	static void process(Player *player, char *cmd);
};

