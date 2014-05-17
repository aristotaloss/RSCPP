#pragma once

#define GROUND_LEVEL 0

class Tile {
public:
	Tile(int x, int z);
	Tile(int x, int z, int level);
	~Tile();

	int x;
	int z;
	int level;
	int getSceneX(Tile *base);
	int getSceneZ(Tile *base);
};

