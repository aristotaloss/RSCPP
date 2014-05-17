#include "Decoder.h"


Decoder::Decoder(ClientSocket *connection) {
	this->connection = connection;
}

Decoder::~Decoder() {
}

void Decoder::decode(char *buffer, int opcode, int len) {
	logf("Incoming opcode: %d. Size: %d.\n", opcode, len);

	if (this->connection->getGameState() == GameState::PRE_GAME) {
		decodePreGame(buffer, opcode, len);
	} else if (this->connection->getGameState() == GameState::INGAME) {
		decodeInGame(buffer, opcode, len);
	}
}

void Decoder::decodeInGame(char *buffer, int opcode, int len) {
	Player *player = connection->getPlayer();
	Packet *in = new Packet((uint8_t *)buffer);

	if (opcode == OPCODE_COMMAND) {
		char *cmd = in->readString();

		CommandProcessor::process(connection->getPlayer(), cmd);

		free(cmd);
	} else if (opcode == OPCODE_WALK_SCENE || opcode == OPCODE_WALK_MINIMAP) {
		int base_x = in->readShortALE();

		int step_count = (len - (opcode == OPCODE_WALK_MINIMAP ? (14+3) : 3)) / 2;
		int *steps_x = new int[step_count];
		int *steps_z = new int[step_count];
		for (int i = 1; i < step_count; i++) {
			steps_x[i] = in->readByte();
			steps_z[i] = in->readByte();
		}

		int base_z = in->readShortLE();
		int ctrl = -in->readByte();

		/* Make the tiles absolute positions */
		for (int i = 1; i < step_count; i++) {
			steps_x[i] += base_x;
			steps_z[i] += base_z;
		}

		/* Add the base to the step list */
		steps_x[0] = base_x;
		steps_z[0] = base_z;

		/* And make the player walk them */
		MovementProcessor *m = player->getMovement();
		m->clearQueue();
		for (int step = 0; step < step_count; step++) {
			m->walkInterpolated(steps_x[step], steps_z[step]);
		}

		/* Free the arrays */
		delete[] steps_x;
		delete[] steps_z;
	}

	delete in;
}

void Decoder::decodePreGame(char *buffer, int opcode, int len) {
	if (opcode == OPCODE_LOGIN_REQUEST) {
		Packet *packet = new Packet(17);

		/* The server reads a long which is unused. */
		packet->writeLong(0);

		packet->writeByte(0); // Response code
		packet->writeLong(0); // Serversided ISAAC randomness seed

		connection->writePacket(packet);
	} else if (opcode == OPCODE_LOGIN_DATA || opcode == OPCODE_LOGIN_RECONNECT) {
		Packet *in = new Packet((uint8_t *) buffer);

		/* Decode information */
		int v_255 = in->readByte();
		int revision = in->readShort();
		bool lowmem = in->readByte();

		/* The server sends 9 CRC32 checksums */
		int crcs[9];
		for (int i = 0; i < 9; i++)
			crcs[i] = in->readInt();

		int rsalen = in->readByte();
		int v_10 = in->readByte();

		/* 
		 * The 4 clientsided ISAAC keys.
		 * The server uses these but adds 50 to each key.
		 * This is not implemented yet. 
	     */
		int isaac[4];
		for (int i = 0; i < 4; i++)
			isaac[i] = in->readInt();

		int uid = in->readInt();

		char *username = in->readString();
		char *password = in->readString();

		delete in;

		logf("Login request: v_255=%d, revision=%d, lowmem=%d, rsalen=%d, v_10=%d, username=%s, password=%s, uid=%d.\n", v_255, revision, lowmem, rsalen, v_10, username, password, uid);

		/* Strings are copied, memory is not shared so may be freed afterwards */
		connection->getPlayer()->setUsername(username);
		connection->getPlayer()->setPassword(password);
		connection->getPlayer()->setUID(uid);
		connection->getPlayer()->getLooks()->update();

		/* Free heap allocated strings */
		free(username);
		free(password);

		Packet *packet = new Packet(3);

		/* Write response, rights and flag status */
		packet->writeByte(RESPONSE_ACCEPTED);
		packet->writeByte(2);
		packet->writeByte(false);

		connection->writePacket(packet);
		connection->setGameState(GameState::INGAME);

		/* Add the player to the world and set its index */
		Player *player = connection->getPlayer();
		player->setIndex(World::addPlayer(player));
		player->setInGame(true);

		logf("Player %s registered with index %d.\n", player->getUsername(), player->getIndex());

		PacketSender *sender = connection->getSender();

		/* Send the startup packets required to be able to make the game show */
		sender->sendPlayerInformation();
		sender->sendCameraReset();
		sender->sendMapBase(player->getTile()->x / 8, player->getTile()->z / 8);
		sender->sendClearScreen();

		/* Send the game tabs */
		sender->sendTab(0, 2433); // Combat
		sender->sendTab(1, 3917); // Stats
		sender->sendTab(2, 638);  // Quests
		sender->sendTab(3, 3213); // Inventory
		sender->sendTab(4, 1644); // Equipment
		sender->sendTab(5, 5608); // Prayer
		sender->sendTab(6, 1151); // Magic

		sender->sendTab(8, 5065); // Friends
		sender->sendTab(9, 5715); // Ignores
		sender->sendTab(10, 2449);// Logout
		sender->sendTab(11, 4445);// Settings
		sender->sendTab(12, 147); // Emotes
		sender->sendTab(13, 6299);// Music

		/* Welcome our new guest */
		player->sendMessagef("Welcome to this C++ server, %s!", player->getUsername());
	}
}
