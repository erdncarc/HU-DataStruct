#include "ApplicationLayerPacket.h"

ApplicationLayerPacket::ApplicationLayerPacket(int _layer_ID, const string &_sender_ID, const string &_receiver_ID, const string &_message_data) : Packet(_layer_ID) {
	sender_ID = _sender_ID;
	receiver_ID = _receiver_ID;
	message_data = _message_data;
}

void ApplicationLayerPacket::print() {
	cout << "Sender ID: " << sender_ID << ", Receiver ID: " << receiver_ID << endl;
}

ApplicationLayerPacket::~ApplicationLayerPacket() = default;
