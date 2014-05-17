#include "PacketSender.h"


PacketSender::PacketSender(ClientSocket *socket) {
	this->socket = socket;
}


PacketSender::~PacketSender() {
}

void PacketSender::sendMapBase(int rx, int rz) {
	Packet *p = new Packet(5);
	p->writeOpcode(73);
	p->writeShortA(rx);
	p->writeShort(rz);
	socket->writePacket(p);

	printf("Map base %d, %d\n", rx, rz);

	socket->getPlayer()->setSceneBase(rx - 6, rz - 6);
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
	p->writeShortALE(1);
	socket->writePacket(p);
}

void PacketSender::sendCameraReset(void) {
	Packet *p = new Packet(1);
	p->writeOpcode(107);
	socket->writePacket(p);
}

void PacketSender::sendMessage(const char *msg) {
	Packet *p = new Packet(strlen(msg) + 3);
	p->writeOpcode(253);
	p->writeByteSize();
	p->writeString(msg);
	p->endPacket();
	socket->writePacket(p);
}

void PacketSender::sendTab(int target, int id) {
	Packet *p = new Packet(4);
	p->writeOpcode(71);
	p->writeShort(id);
	p->writeByteA(target);
	socket->writePacket(p);
}