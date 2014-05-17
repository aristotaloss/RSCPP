#pragma once

#include <cstdint>
#include <cstdlib>

#define DEFAULT_BUFFER_SIZE 32

namespace PacketSize {
	enum PacketSize {
		FIXED, BYTE_SIZE, SHORT_SIZE
	};
}

class Packet {
private:
	uint8_t *buffer;
	int writePos;
	int readPos;
	int bitpos;
	PacketSize::PacketSize sizetype; 
	int lenpos;
public:
	Packet();
	Packet(int size);
	Packet(uint8_t *buffer);
	~Packet();
	int getWritePos(void);
	uint8_t *getBuffer(void);
	void writeByte(uint8_t byte_);
	void writeByteA(uint8_t byte_);
	void writeByteN(uint8_t byte_);
	void writeOpcode(uint8_t byte_);
	void writeShort(uint16_t short_);
	int readShort(void);
	int readShortALE(void);
	int readShortLE(void);
	void writeShortA(uint16_t short_);
	void writeShortLE(uint16_t short_);
	void writeShortALE(uint16_t short_);
	void writeInt(uint32_t int_);
	int readInt(void);
	void writeLong(uint64_t long_);
	void writeString(const char *str);
	int8_t readByte(void);
	char *readString(void);
	void beginBitMode(void);
	void endBitMode(void);
	void writeBits(int, int);
	void writeShortSize(void);
	void writeByteSize(void);
	void endPacket(void);
	void cleanup(void);
};

