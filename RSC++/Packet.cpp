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

void Packet::writeByteA(uint8_t byte_) {
	buffer[writePos++] = (byte_ & 0xFF) + 128;
}

void Packet::writeOpcode(uint8_t byte_) {
	buffer[writePos++] = byte_;
}

void Packet::writeShort(uint16_t short_) {
	buffer[writePos++] = (short_ >> 8);
	buffer[writePos++] = (short_ & 0xFF);
}

void Packet::writeShortA(uint16_t short_) {
	buffer[writePos++] = (short_ >> 8);
	buffer[writePos++] = ((short_ & 0xFF) + 128);
}

void Packet::writeAShortLE(uint16_t short_) {
	buffer[writePos++] = ((short_ & 0xFF) + 128);
	buffer[writePos++] = (short_ >> 8);
}

void Packet::writeInt(uint32_t int_) {
	buffer[writePos++] = (int_ >> 24);
	buffer[writePos++] = (int_ >> 16);
	buffer[writePos++] = (int_ >> 8);
	buffer[writePos++] = (int_ & 0xFF);
}

void Packet::writeLong(uint64_t long_) {
	buffer[writePos++] = (long_ >> 56);
	buffer[writePos++] = (long_ >> 48);
	buffer[writePos++] = (long_ >> 40);
	buffer[writePos++] = (long_ >> 32);
	buffer[writePos++] = (long_ >> 24);
	buffer[writePos++] = (long_ >> 16);
	buffer[writePos++] = (long_ >> 8);
	buffer[writePos++] = (long_ & 0xFF);
}

int Packet::getWritePos(void) {
	return this->writePos;
}

uint8_t *Packet::getBuffer(void) {
	return this->buffer;
}
