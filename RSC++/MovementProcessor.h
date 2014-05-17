#pragma once

#include "Player.h"
#include "Tile.h"

class MovementProcessor {
private:
	Player *player;
	std::queue<Tile> *pendingTiles;
	int walk_dir;
	int run_dir;
	Tile *teleport_target;
public:
	MovementProcessor(Player *p);
	~MovementProcessor();
	std::queue<Tile> *getQueue(void);
	bool hasStep(void);
	Tile popStep(void);
	void walkInterpolated(int, int);
	void addStep(int, int);
	void process(void);
	int directionForDelta(int, int);
	bool hasTeleported(void);
	void setTeleportTarget(int, int, int);
	bool needsUpdating(void);
	int getWalkDir(void);
	void clearAll(void);
	void clearQueue(void);
};

