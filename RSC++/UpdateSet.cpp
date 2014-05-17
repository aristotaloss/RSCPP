#include "UpdateSet.h"


UpdateSet::UpdateSet() {
	/* Set defaults for clarification */
	settings = 0;
	animation = NO_ANIMATION;
}


UpdateSet::~UpdateSet() {

}

bool UpdateSet::needsUpdating(void) {
	return settings;
}

int UpdateSet::getSettings(void) {
	return settings;
}

void UpdateSet::clearAll(void) {
	settings = 0;
	animation = NO_ANIMATION;
	animationSetting = 0;
}

void UpdateSet::addSetting(int m) {
	settings |= m;
}

void UpdateSet::setAnimation(int animationId, int settings) {
	animation = animationId;
	animationSetting = settings;
}

int UpdateSet::getAnimation(void) {
	return animation;
}

int UpdateSet::getAnimationSettings(void) {
	return animationSetting;
}