#include "Packet.h"


Packet::Packet() {
	writePos = 0;
	readPos = 0;

	buffer = (uint8_t *)malloc(DEFAULT_BUFFER_SIZE);
}

Packet::Packet(int size) {
	writePos = 0;
	readPos = 0;

	buffer = (uint8_t *)malloc(size);
}

Packet::~Packet() {
	free(buffer);
}

void Packet::writeByte(uint8_t byte_) {
	buffer[writePos++] = byte_;
}

void Packet::writeShort(uint16_t short_) {
	buffer[writePos++] = (short_ >> 8);
	buffer[writePos++] = (short_ & 0xFF);
}

int Packet::getWritePos(void) {
	return this->writePos;
}

uint8_t *Packet::getBuffer(void) {
	return this->buffer;
}
