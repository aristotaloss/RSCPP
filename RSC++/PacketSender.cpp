#include "PacketSender.h"


PacketSender::PacketSender(ClientSocket *socket) {
	this->socket = socket;
}


PacketSender::~PacketSender() {
}

void PacketSender::sendMapBase(int rx, int ry) {
	Packet *p = new Packet(5);
	p->writeOpcode(73);
	p->writeShortA(rx);
	p->writeShort(ry);
	socket->writePacket(p);
}

void PacketSender::sendClearScreen(void) {
	Packet *p = new Packet(1);
	p->writeOpcode(219);
	socket->writePacket(p);
}

void PacketSender::sendPlayerInformation(void) {
	Packet *p = new Packet(4);
	p->writeOpcode(249);
	p->writeByteA(1);
	p->writeAShortLE(1);
	socket->writePacket(p);
}

void PacketSender::sendCameraReset(void) {
	Packet *p = new Packet(1);
	p->writeOpcode(107);
	socket->writePacket(p);
}