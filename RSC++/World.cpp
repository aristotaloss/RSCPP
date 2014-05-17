#include "World.h"


World::World(){
}


World::~World(){
}

void World::process(void) {
	/* Pre update lapse */
	for (int i = 1; i < MAX_PLAYERS; i++) {
		if (players[i] != NULL) {
			players[i]->processPendingPackets();
			players[i]->getMovement()->process();
		}
	}

	/* Main player lapse */
	for (int i = 1; i < MAX_PLAYERS; i++) {
		if (players[i] != NULL) {
			players[i]->getUpdater()->update();
		}
	}

	/* Post update lapse */
	for (int i = 1; i < MAX_PLAYERS; i++) {
		if (players[i] != NULL) {
			players[i]->getUpdateSet()->clearAll();
			players[i]->getMovement()->clearAll();
		}
	}
}

int World::addPlayer(Player *player) {
	for (int i = 1; i < MAX_PLAYERS; i++) {
		if (players[i] == NULL) {
			players[i] = player;
			return i;
		}
	}

	return -1;
}

bool World::removePlayer(Player *player) {
	for (int i = 1; i < MAX_PLAYERS; i++) {
		if (players[i] == player) {
			players[i] = NULL;
			return true;
		}
	}

	return false;
}

Player *World::getPlayer(int index) {
	if (index < 0 || index >= MAX_PLAYERS) {
		return NULL;
	}

	return players[index];
}