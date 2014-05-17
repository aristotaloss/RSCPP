#include "PlayerUpdater.h"


PlayerUpdater::PlayerUpdater(Player *player) {
	this->player = player;
	this->playerIndices = new int[MAX_PLAYERS];
	this->playersNeedingMaskUpdate = new int[MAX_PLAYERS];
}


PlayerUpdater::~PlayerUpdater() {
	delete[] playerIndices;
	delete[] playersNeedingMaskUpdate;
}


void PlayerUpdater::update(void) {
	/* Check if we have to refresh our region */
	int map_x = player->getTile()->getSceneX(player->getSceneBase());
	int map_z = player->getTile()->getSceneZ(player->getSceneBase());

	if (map_x < 14 || map_z < 14 || map_x > 88 || map_z > 88) {
		player->getSocket()->getSender()->sendMapBase(player->getTile()->x / 8, player->getTile()->z / 8);
	}

	Packet *packet = new Packet(1024); // TODO improve this (or work on dynamic reallocation)
	packet->writeOpcode(81);
	packet->writeShortSize();

	packet->beginBitMode();
	encodeMyMovement(packet);
	encodeOtherMovement(packet);
	encodeNewPlayers(packet);
	packet->endBitMode();

	// Masks
	encodePlayerSettings(packet);

	packet->endPacket();
	player->getSocket()->writePacket(packet);

	reset();
}


void PlayerUpdater::encodeMyMovement(Packet *packet) {
	UpdateSet *set = player->getUpdateSet();

	packet->writeBits(1, set->needsUpdating() || player->getMovement()->needsUpdating()); // Did move

	if (set->needsUpdating() || player->getMovement()->needsUpdating()) {
		if (player->getMovement()->hasTeleported()) {
			packet->writeBits(2, 3); // Move type

			packet->writeBits(2, player->getTile()->level); // Level
			packet->writeBits(1, 1); // Unknown
			packet->writeBits(1, set->getSettings() != 0); // Mask update
			packet->writeBits(7, player->getTile()->getSceneZ(player->getSceneBase())); // Z
			packet->writeBits(7, player->getTile()->getSceneX(player->getSceneBase())); // X
		} else if (player->getMovement()->hasStep()) {
			packet->writeBits(2, 1); // Move type

			packet->writeBits(3, player->getMovement()->getWalkDir());
			packet->writeBits(1, set->getSettings() != 0); // Mask update
		}
		else if (set->getSettings() != 0) {
			packet->writeBits(2, 0);
		}

		if (set->getSettings() != 0) {
			playersNeedingMaskUpdate[maskUpdatePtr++] = player->getIndex();
		}
	}
}


void PlayerUpdater::encodeOtherMovement(Packet *packet) {
	packet->writeBits(8, 0); // Local player count
}


void PlayerUpdater::encodeNewPlayers(Packet *packet) {
	packet->writeBits(11, 2047); // No more players
}


void PlayerUpdater::encodePlayerSettings(Packet *packet) {
	for (int i = 0; i < maskUpdatePtr; i++) {
		Player *p = World::getPlayer(playersNeedingMaskUpdate[i]);

		if (p != NULL) {
			UpdateSet *us = p->getUpdateSet();

			int settings = us->getSettings();
			packet->writeByte(settings);

			if (settings & PLAYER_SETTING_ANIMATION) {
				packet->writeShortLE(us->getAnimation());
				packet->writeByteN(us->getAnimationSettings());
			}

			if (settings & PLAYER_SETTING_LOOKS) {
				int len = p->getLooks()->getCachedLen();
				uint8_t *buf = p->getLooks()->getCached();

				packet->writeByte(-len);
				for (int b = 0; b < len; b++) {
					packet->writeByte(buf[b]);
				}
			}
		}
	}
}


void PlayerUpdater::reset(void) {
	memset(playersNeedingMaskUpdate, 0, sizeof(int)* MAX_PLAYERS);
	maskUpdatePtr = 0;
}