#include "Packet.h"
#include "Logging.h"

Packet::Packet() {
	writePos = 0;
	readPos = 0;
	sizetype = PacketSize::FIXED;

	buffer = (uint8_t *)malloc(DEFAULT_BUFFER_SIZE);
	memset(buffer, 0, DEFAULT_BUFFER_SIZE);
}

Packet::Packet(int size) {
	writePos = 0;
	readPos = 0;
	bitpos = -1;
	sizetype = PacketSize::FIXED;

	buffer = (uint8_t *)malloc(size);
	memset(buffer, 0, size);
}

Packet::Packet(uint8_t *buffer) {
	this->buffer = buffer;
	writePos = readPos = 0;
	bitpos = -1;

	sizetype = PacketSize::FIXED;
}

Packet::~Packet() {
	//free(buffer);
}

void Packet::cleanup(void) {
	if (buffer != NULL)
		free(buffer);
}

void Packet::writeByte(uint8_t byte_) {
	buffer[writePos++] = byte_;
}

int8_t Packet::readByte(void) {
	return buffer[readPos++];
}

void Packet::writeByteA(uint8_t byte_) {
	buffer[writePos++] = (byte_ & 0xFF) + 128;
}

void Packet::writeByteN(uint8_t byte_) {
	buffer[writePos++] = -(byte_ & 0xFF);
}

void Packet::writeOpcode(uint8_t byte_) {
	buffer[writePos++] = byte_;
}

void Packet::writeShort(uint16_t short_) {
	buffer[writePos++] = (short_ >> 8);
	buffer[writePos++] = (short_ & 0xFF);
}

int Packet::readShort(void) {
	return ((buffer[readPos++] & 0xFF) << 8) | (buffer[readPos++] & 0xFF);
}

int Packet::readShortALE(void) {
	int p1 = (((buffer[readPos++] & 0xFF) - 128) & 0xFF);
	return p1 | ((buffer[readPos++] & 0xFF) << 8);
}

int Packet::readShortLE(void) {
	int p1 = buffer[readPos++];
	int p2 = buffer[readPos++];
	return (p1 & 0xFF) | ((p2 & 0xFF) << 8);
}

void Packet::writeShortA(uint16_t short_) {
	buffer[writePos++] = (short_ >> 8);
	buffer[writePos++] = ((short_ & 0xFF) + 128);
}

void Packet::writeShortALE(uint16_t short_) {
	buffer[writePos++] = (short_ + 128);
	buffer[writePos++] = (short_ >> 8);
}

void Packet::writeShortLE(uint16_t short_) {
	buffer[writePos++] = (short_);
	buffer[writePos++] = (short_ >> 8);
}

void Packet::writeInt(uint32_t int_) {
	buffer[writePos++] = (int_ >> 24);
	buffer[writePos++] = (int_ >> 16);
	buffer[writePos++] = (int_ >> 8);
	buffer[writePos++] = (int_ & 0xFF);
}

int Packet::readInt(void) {
	return ((buffer[readPos++] & 0xFF) << 24) | ((buffer[readPos++] & 0xFF) << 16) | ((buffer[readPos++] & 0xFF) << 8) | (buffer[readPos++] & 0xFF);
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

void Packet::writeString(const char *str) {
	for (int i = 0; i < strlen(str); i++)
		buffer[writePos++] = str[i];
	buffer[writePos++] = 10;
}

// Heap allocated; do not forget to free
char *Packet::readString(void) {
	int p = readPos;
	int a = buffer[readPos];
	while (buffer[readPos++] != 10);

	logf("Allocating [%d] %d bytes for str buffer.\n", a, (readPos - p));

	char *str = (char *) malloc(readPos - p);
	memcpy(str, buffer + p, (readPos - p));
	str[readPos - p - 1] = 0; // Null terminator

	return str;
}

int Packet::getWritePos(void) {
	return this->writePos;
}

uint8_t *Packet::getBuffer(void) {
	return this->buffer;
}

void Packet::beginBitMode(void) {
	bitpos = 7;
}

void Packet::endBitMode(void) {
	if (bitpos != 7)
		writePos++;
	bitpos = -1;
}

void Packet::writeBits(int count, int value) {
	for (int bp = count - 1; bp >= 0; bp--) {
		if ((value & (1 << bp)) != 0) {
			buffer[writePos] |= (1 << bitpos);
			//logf("[%d %d] Enabling bit %d at bpos %d and btpos %d.\n", count, value, bp, writePos, bitpos);
		}

		if (bitpos == 0) {
			bitpos = 7;
			writePos++;
		} else {
			bitpos--;
		}
	}
}

void Packet::writeByteSize(void) {
	sizetype = PacketSize::BYTE_SIZE;
	lenpos = writePos;
	writePos++;
}

void Packet::writeShortSize(void) {
	sizetype = PacketSize::SHORT_SIZE;
	lenpos = writePos;
	writePos += 2;
}

void Packet::endPacket(void) {
	int cur = writePos;

	if (sizetype == PacketSize::BYTE_SIZE) {
		writePos = lenpos;
		writeByte(cur - lenpos - 1);
	} else if (sizetype == PacketSize::SHORT_SIZE) {
		writePos = lenpos;
		writeShort(cur - lenpos - 2);
	}

	writePos = cur;
}