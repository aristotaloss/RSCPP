#include "Tile.h"


Tile::Tile(int x, int z) {
	this->x = x;
	this->z = z;
	this->level = GROUND_LEVEL;
}

Tile::Tile(int x, int z, int level) {
	this->x = x;
	this->z = z;
	this->level = level;
}


Tile::~Tile() {
}


int Tile::getSceneX(Tile *tile) {
	int base = tile->x;
	return x - base;
}

int Tile::getSceneZ(Tile *tile) {
	int base = tile->z;
	return z - base;
}