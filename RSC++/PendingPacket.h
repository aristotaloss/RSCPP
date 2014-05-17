#pragma once

#include <cstdint>
#include <malloc.h>
#include "Logging.h"

class PendingPacket {
private:
	int opcode;
	int size;
	uint8_t *buffer;
public:
	PendingPacket(int opcode, int size, uint8_t *buffer);
	~PendingPacket();
	int getOpcode();
	int getSize();
	uint8_t *getBuffer();
};

