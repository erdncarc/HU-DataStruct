#ifndef TRANSPORT_LAYER_PACKET_H
#define TRANSPORT_LAYER_PACKET_H

#include "Packet.h"

class TransportLayerPacket : public Packet {

public:
	TransportLayerPacket(int _layer_ID, const string &_sender_port, const string &_receiver_port);

	~TransportLayerPacket() override;

	string sender_port_number;
	string receiver_port_number;

	void print() override;
};

#endif
