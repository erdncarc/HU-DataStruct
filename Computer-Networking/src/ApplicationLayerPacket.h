#ifndef APPLICATION_LAYER_PACKET_H
#define APPLICATION_LAYER_PACKET_H

#include "Packet.h"

class ApplicationLayerPacket : public Packet {

public:
	ApplicationLayerPacket(int _layer_ID, const string &_sender_ID, const string &_receiver_ID, const string &_message_data);

	~ApplicationLayerPacket() override;

	string sender_ID;
	string receiver_ID;
	string message_data;

	void print() override;
};

#endif
