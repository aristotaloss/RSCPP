#pragma once

#define PLAYER_SETTING_LOOKS 0x10
#define PLAYER_SETTING_ANIMATION 0x8

#define NO_ANIMATION -1

#include "Tile.h"

class UpdateSet {
private:
	int settings;
	int animation;
	int animationSetting;
public:
	UpdateSet();
	~UpdateSet();
	bool needsUpdating(void);
	void clearAll(void);
	int getSettings(void);
	void addSetting(int);
	void setAnimation(int, int);
	int getAnimation(void);
	int getAnimationSettings(void);
};

