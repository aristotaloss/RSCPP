#include "PendingPacket.h"


PendingPacket::PendingPacket(int opcode, int size, uint8_t *buffer) {
	this->opcode = opcode;
	this->size = size;
	this->buffer = buffer;
}

PendingPacket::~PendingPacket() {
	free(buffer);
	//TODO see how we handle the memory...
}

int PendingPacket::getOpcode() {
	return this->opcode;
}

int PendingPacket::getSize() {
	return this->size;
}

uint8_t *PendingPacket::getBuffer() {
	return this->buffer;
}