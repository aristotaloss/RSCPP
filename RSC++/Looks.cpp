#include "Looks.h"


Looks::Looks(Player *player) {
	this->player = player;

	looks = new int[12];
	colors = new int[5];
	animations = new int[7];

	/* Set memory to zeroes */
	memset(looks, 0, 12 * sizeof(int));
	memset(colors, 0, 5 * sizeof(int));
	memset(animations, 0, 7 * sizeof(int));

	setDefaults();
}


Looks::~Looks() {
	delete[] looks;
	delete[] colors;
	delete[] animations;
}


void Looks::setDefaults(void) {
	looks[4] = 18; /* Head */
	looks[6] = 26;
	looks[7] = 36;
	looks[8] = 1;
	looks[9] = 33;
	looks[10] = 42;
	looks[11] = 10;

	/* Stand and turn animations */
	animations[0] = 0x328;
	animations[1] = 0x337;
	animations[2] = 0x333;
	animations[3] = 0x334;
	animations[4] = 0x335;
	animations[5] = 0x336;
	animations[6] = 0x338;
}


void Looks::update(void) {
	Packet *p = new Packet(56);

	/* Gender and head icon */
	p->writeByte(0);
	p->writeByte(0);

	/* Looks and equipment */
	for (int i = 0; i < 12; i++) {
		//TODO! this does not support head model 0!
		if (looks[i]) {
			p->writeByte(1);
			p->writeByte(looks[i]);
		} else {
			p->writeByte(0);
		}
	}
	 
	/* Colors */
	for (int i = 0; i < 5; i++) {
		p->writeByte(colors[i]);
	}

	/* Animations */
	for (int i = 0; i < 7; i++)
		p->writeShort(animations[i]);

	p->writeLong(Utils::nameToLong(player->getUsername())); /* Bart */
	p->writeByte(3); /* Combat level */
	p->writeShort(100); /* Skill total */

	/* Copy to cached buffer */
	if (cached != NULL)
		free(cached);

	cachedlen = p->getWritePos();
	cached = (uint8_t *)malloc(cachedlen);
	memcpy(cached, p->getBuffer(), cachedlen);

	//TODO support dirty/clean :)
	player->getUpdateSet()->addSetting(PLAYER_SETTING_LOOKS);
	logf("Set look\n");

	delete p;
}


uint8_t *Looks::getCached(void) {
	if (cached == NULL)
		update();
	return cached;
}


int Looks::getCachedLen(void) {
	return cachedlen;
}