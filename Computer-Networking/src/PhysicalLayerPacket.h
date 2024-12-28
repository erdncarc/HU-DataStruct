#ifndef PHYSICAL_LAYER_PACKET_H
#define PHYSICAL_LAYER_PACKET_H

#include "Packet.h"

class PhysicalLayerPacket : public Packet {

public:
	PhysicalLayerPacket(int layerID, const string &senderMAC, const string &receiverMAC);

	~PhysicalLayerPacket() override;

	string sender_MAC_address;
	string receiver_MAC_address;

	void print() override;
};

#endif
