#pragma once

#include <cstdint>
#include "Player.h"
#include "Packet.h"
#include "PlayerUpdater.h"
#include "Utils.h"

class Looks {
private:
	Player *player;
	uint8_t *cached;
	int cachedlen;
	int *looks;
	int *colors;
	int *animations;
public:
	Looks(Player *);
	~Looks();
	void update(void);
	void setDefaults(void);
	uint8_t *getCached(void);
	int getCachedLen(void);
};

