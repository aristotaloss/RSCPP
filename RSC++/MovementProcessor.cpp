#include "MovementProcessor.h"


MovementProcessor::MovementProcessor(Player *player) {
	this->player = player;
	this->pendingTiles = new std::queue<Tile>();
	walk_dir = -1;
	run_dir = -1;
}


MovementProcessor::~MovementProcessor() {
	delete pendingTiles;
}

std::queue<Tile> *MovementProcessor::getQueue(void) {
	return this->pendingTiles;
}

bool MovementProcessor::hasStep(void) {
	return walk_dir != -1 || run_dir != -1;
}

bool MovementProcessor::hasTeleported(void) {
	return teleport_target != 0;
}

void MovementProcessor::setTeleportTarget(int x, int z, int level) {
	if (teleport_target != 0)
		delete teleport_target;

	this->teleport_target = new Tile(x, z, level);
}

bool MovementProcessor::needsUpdating(void) {
	return hasTeleported() || hasStep();
}

Tile MovementProcessor::popStep(void) {
	Tile val = pendingTiles->empty() ? Tile(0, 0) : pendingTiles->front();
	if (!pendingTiles->empty())
		pendingTiles->pop();
	return val;
}

void MovementProcessor::walkInterpolated(int x, int z) {
	int cur_x = player->getTile()->x;
	int cur_z = player->getTile()->z;

	if (!pendingTiles->empty()) {
		Tile t = pendingTiles->back();
		cur_x = t.x;
		cur_z = t.z;
	}

	while (cur_x != x || cur_z != z) {
		if (cur_x < x)
			cur_x++;
		else if (cur_x > x)
			cur_x--;

		if (cur_z < z)
			cur_z++;
		else if (cur_z > z)
			cur_z--;

		addStep(cur_x, cur_z);
	}
}

void MovementProcessor::addStep(int x, int z) {
	pendingTiles->push(Tile(x, z));
}

int MovementProcessor::directionForDelta(int dx, int dz) {
	if (dx == -1 && dz == 1)
		return 0;
	else if (dx == 0 && dz == 1)
		return 1;
	else if (dx == 1 && dz == 1)
		return 2;
	else if (dx == -1 && dz == 0)
		return 3;
	else if (dx == 1 && dz == 0)
		return 4;
	else if (dx == -1 && dz == -1)
		return 5;
	else if (dx == 0 && dz == -1)
		return 6;
	return 7;
}

void MovementProcessor::process(void) {
	if (hasTeleported()) {
		player->getTile()->x = teleport_target->x;
		player->getTile()->z = teleport_target->z;

		/* Clear the queue */
		clearQueue();
	} else {
		if (!pendingTiles->empty()) {
			Tile to = popStep();
			Tile me = *player->getTile();
			int dx = to.x - me.x;
			int dz = to.z - me.z;

			walk_dir = directionForDelta(dx, dz);
			player->getTile()->x = to.x;
			player->getTile()->z = to.z;
		}
		else {
			walk_dir = -1;
			run_dir = -1;
		}
	}
}

void MovementProcessor::clearQueue(void) {
	while (!pendingTiles->empty()) {
		pendingTiles->pop();
	}
}

int MovementProcessor::getWalkDir(void) {
	return walk_dir;
}

void MovementProcessor::clearAll(void) {
	walk_dir = run_dir = -1;
	if (teleport_target != 0)
		delete teleport_target;
	teleport_target = 0;
}