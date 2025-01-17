#include "NetworkLayerPacket.h"

NetworkLayerPacket::NetworkLayerPacket(int _layer_ID, const string &_sender_IP, const string &_receiver_IP) : Packet(_layer_ID) {
	sender_IP_address = _sender_IP;
	receiver_IP_address = _receiver_IP;
}

void NetworkLayerPacket::print() {
	cout << "Sender IP address: " << sender_IP_address << ", Receiver IP address: " << receiver_IP_address << endl;
}

NetworkLayerPacket::~NetworkLayerPacket() = default;
