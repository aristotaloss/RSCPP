#include "Player.h"


Player::Player(ClientSocket *channel) {
	this->socket = channel;
	this->packetqueue = new std::queue<PendingPacket *>();
	this->updateSet = new UpdateSet();
	this->tile = new Tile(0, 0);
	this->sceneBase = new Tile(0, 0);
	this->updater = new PlayerUpdater(this);
	this->looks = new Looks(this);
	this->movement = new MovementProcessor(this);

	teleport(3222, 3222);
}

Player::~Player() {
	//TODO XXX Clean up pending packets incase the client disconnects without all being decoded.
	delete packetqueue;
	delete updateSet;
	delete tile;
	delete sceneBase;
	delete updater;
	delete looks;
	delete movement;
}

void Player::setInGame(bool ingame) {
	this->ingame = ingame;
}

bool Player::isInGame(void) {
	return this->ingame;
}

void Player::schedulePacket(PendingPacket *packet) {
	this->packetqueue->push(packet);
}

std::queue<PendingPacket *> *Player::getPacketQueue(void) {
	return this->packetqueue;
}

void Player::setIndex(int index) {
	this->index = index;
}

int Player::getIndex(void) {
	return this->index;
}

void Player::processPendingPackets(void) {
	while (!this->packetqueue->empty()) {
		PendingPacket *packet = this->packetqueue->front();

		if (packet != NULL) {
			this->socket->getDecoder()->decode((char *) packet->getBuffer(), packet->getOpcode(), packet->getSize());
		}

		delete packet;
		this->packetqueue->pop();
	}
}

void Player::teleport(int x, int z) {
	tile->x = x;
	tile->z = z;
	movement->setTeleportTarget(x, z, tile->level);
}

ClientSocket *Player::getSocket(void) {
	return socket;
}

PlayerUpdater *Player::getUpdater(void) {
	return this->updater;
}

Tile *Player::getTile(void) {
	return this->tile;
}

void Player::setSceneBase(int rx, int rz) {
	sceneBase->x = rx * 8;
	sceneBase->z = rz * 8;
}

Tile *Player::getSceneBase(void) {
	return this->sceneBase;
}

UpdateSet *Player::getUpdateSet(void) {
	return this->updateSet;
}

Looks *Player::getLooks(void) {
	return looks;
}

MovementProcessor *Player::getMovement(void) {
	return this->movement;
}

void Player::sendMessage(const char *msg) {
	socket->getSender()->sendMessage(msg);
}

void Player::setUsername(char *user) {
	int len = strlen(user);
	this->username = (char *)malloc(len + 1);
	memcpy(username, user, len + 1);
}

void Player::setPassword(char *pass) {
	int len = strlen(pass);
	this->password = (char *)malloc(len + 1);
	memcpy(password, pass, len + 1);
}

void Player::setUID(int id) {
	this->uid = id;
}

char *Player::getUsername(void) {
	return this->username;
}

char *Player::getPassword(void) {
	return this->password;
}

int Player::getUID(void) {
	return this->uid;
}

void Player::sendMessagef(const char *format, ...) {
	va_list args;
	va_start(args, format);

	char dest[255];
	int len = vsprintf_s(dest, 254, format, args);
	printf(dest);
	sendMessage(dest);

	va_end(args);
}

void Player::animate(int anim) {
	updateSet->setAnimation(anim, 0);
	updateSet->addSetting(PLAYER_SETTING_ANIMATION);
}