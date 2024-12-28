#include "Network.h"

Network::Network() = default;

void Network::process_commands(vector<Client> &clients, vector<string> &commands, int message_limit, const string &sender_port, const string &receiver_port) {
	this->limitMsg = message_limit;
	this->portS = sender_port;
	this->portR = receiver_port;

	for (string &comm: commands) {
		cout << string(comm.size() + 9, '-') << "\nCommand: " << comm << "\n" << string(comm.size() + 9, '-') << endl;

		stringstream ss(comm);
		vector<string> command;
		string unit;
		while (ss >> unit) {
			command.push_back(unit);
		}

		if (command[0] == "MESSAGE") {
			Message(clients, command);

		} else if (command[0] == "SEND") {
			Send(clients);

		} else if (command[0] == "RECEIVE") {
			Receive(clients);

		} else if (command[0] == "SHOW_FRAME_INFO") {
			FrameInfo(clients, command);

		} else if (command[0] == "SHOW_Q_INFO") {
			QInfo(clients, command);

		} else if (command[0] == "PRINT_LOG") {
			PrintLog(clients, command);

		} else {
			cout << "Invalid command." << endl;
		}
	}
}

vector<Client> Network::read_clients(const string &filename) {
	vector<Client> clients;

	ifstream file(filename);
	string line;
	if (file.is_open()) {
		getline(file, line);

		while (getline(file, line)) {
			if (line.empty()) continue;

			stringstream ss(line);
			vector<string> split;
			string unit;
			while (ss >> unit) {
				split.push_back(unit);
			}

			Client client(split[0], split[1], split[2]);
			clients.push_back(client);
		}
	}
	return clients;
}

void Network::read_routing_tables(vector<Client> &clients, const string &filename) {
	ifstream file(filename);
	string line;
	int index{};
	if (file.is_open()) {
		while (getline(file, line)) {
			if (line.empty()) continue;

			if (line == "-") {
				index++;
				continue;
			}
			clients[index].routing_table[string(1, line[0])] = string(1, line[2]);
		}
	}
}

vector<string> Network::read_commands(const string &filename) {
	vector<string> commands;

	ifstream file(filename);
	string line;
	if (file.is_open()) {
		getline(file, line);

		while (getline(file, line)) {
			if (line.empty()) continue;

			commands.push_back(line);
		}
	}
	return commands;
}

void Network::Message(vector<Client> &clients, const vector<string> &command) const {
	vector<string> messages = smashMessage(command);

	auto clientS = findID(clients, command[1]);
	auto clientR = findID(clients, command[2]);
	auto clientWay = findID(clients, clientS->routing_table[command[2]]);

	int frame{};
	string msg;
	for (const string &message: messages) {
		frame++;

		auto *appPacket = new ApplicationLayerPacket(APPLICATION_LAYER_ID, clientS->client_id, clientR->client_id, message);
		auto *transPacket = new TransportLayerPacket(TRANSPORT_LAYER_ID, portS, portR);
		auto *netPacket = new NetworkLayerPacket(NETWORK_LAYER_ID, clientS->client_ip, clientR->client_ip);
		auto *phyPacket = new PhysicalLayerPacket(PHYSICAL_LAYER_ID, clientS->client_mac, clientWay->client_mac);

		int hops = calculateHops(clients, appPacket->sender_ID, clientS->client_id, appPacket->receiver_ID);
		msg.append(message);

		cout << "Frame #" << frame << endl;
		phyPacket->print();
		netPacket->print();
		transPacket->print();
		appPacket->print();
		cout << "Message chunk carried: \"" << appPacket->message_data << "\"" << endl;
		cout << "Number of hops so far: " << hops << endl;
		cout << "--------" << endl;

		stack<Packet *> stack;
		stack.push(appPacket);
		stack.push(transPacket);
		stack.push(netPacket);
		stack.push(phyPacket);
		clientS->outgoing_queue.push(stack);

		if (msg[msg.size() - 1] == '!' || msg[msg.size() - 1] == '.' || msg[msg.size() - 1] == '?') {
			clientS->log_entries.emplace_back("2023-11-22 20:30:03", msg, frame, hops, appPacket->sender_ID, appPacket->receiver_ID, true, ActivityType::MESSAGE_SENT);
			frame = 0;
			msg = "";
		}
	}
}

void Network::Send(vector<Client> &clients) {
	for (Client &clientS: clients) {
		if (!clientS.outgoing_queue.empty()) {
			int frame{};
			string msg;
			while (!clientS.outgoing_queue.empty()) {
				frame++;

				auto *phyPacket = dynamic_cast<PhysicalLayerPacket *>(clientS.outgoing_queue.front().top());
				auto clientR = findMAC(clients, phyPacket->receiver_MAC_address);
				clientR->incoming_queue.push(clientS.outgoing_queue.front());
				clientS.outgoing_queue.front().pop();

				auto *netPacket = dynamic_cast<NetworkLayerPacket *>(clientS.outgoing_queue.front().top());
				clientS.outgoing_queue.front().pop();

				auto *transPacket = dynamic_cast<TransportLayerPacket *>(clientS.outgoing_queue.front().top());
				clientS.outgoing_queue.front().pop();

				auto *appPacket = dynamic_cast<ApplicationLayerPacket *>(clientS.outgoing_queue.front().top());
				clientS.outgoing_queue.front().pop();

				msg.append(appPacket->message_data);
				int hops = calculateHops(clients, appPacket->sender_ID, clientR->client_id, appPacket->receiver_ID);

				cout << "Client " << clientS.client_id << " sending frame #" << frame << " to client " << clientR->client_id << endl;
				phyPacket->print();
				netPacket->print();
				transPacket->print();
				appPacket->print();
				cout << "Message chunk carried: \"" << appPacket->message_data << "\"" << endl;
				cout << "Number of hops so far: " << hops << endl;
				cout << "--------" << endl;

				if (msg[msg.size() - 1] == '!' || msg[msg.size() - 1] == '.' || msg[msg.size() - 1] == '?') {
					frame = 0;
					msg = "";
				}

				clientS.outgoing_queue.pop();
			}
		}
	}
}

void Network::Receive(vector<Client> &clients) {
	for (Client &clientR: clients) {
		if (!clientR.incoming_queue.empty()) {
			int frame{};
			string msg;
			while (!clientR.incoming_queue.empty()) {
				frame++;

				auto *phyPacket = dynamic_cast<PhysicalLayerPacket *>(clientR.incoming_queue.front().top());
				clientR.incoming_queue.front().pop();

				auto *netPacket = dynamic_cast<NetworkLayerPacket *>(clientR.incoming_queue.front().top());
				clientR.incoming_queue.front().pop();

				auto *transPacket = dynamic_cast<TransportLayerPacket *>(clientR.incoming_queue.front().top());
				clientR.incoming_queue.front().pop();

				auto *appPacket = dynamic_cast<ApplicationLayerPacket *>(clientR.incoming_queue.front().top());
				clientR.incoming_queue.front().pop();

				msg.append(appPacket->message_data);
				int hops = calculateHops(clients, appPacket->sender_ID, clientR.client_id, appPacket->receiver_ID);
				string senderID = findMAC(clients, phyPacket->sender_MAC_address)->client_id;
				Client *clientN = findID(clients, clientR.routing_table[appPacket->receiver_ID]);

				if (clientR.client_id == appPacket->receiver_ID) {
					cout << "Client " << appPacket->receiver_ID << " receiving frame #" << frame << " from client " << senderID << ", originating from client " << appPacket->sender_ID << endl;
					phyPacket->print();
					netPacket->print();
					transPacket->print();
					appPacket->print();
					cout << "Message chunk carried: \"" << appPacket->message_data << "\"" << endl;
					cout << "Number of hops so far: " << hops << endl;
					cout << "--------" << endl;

					if (msg[msg.size() - 1] == '!' || msg[msg.size() - 1] == '.' || msg[msg.size() - 1] == '?') {
						clientR.log_entries.emplace_back("2023-11-22 20:30:03", msg, frame, hops, appPacket->sender_ID, appPacket->receiver_ID, true, ActivityType::MESSAGE_RECEIVED);
						cout << "Client " << appPacket->receiver_ID << " received the message \"" << msg << "\" from client " << appPacket->sender_ID << "." << endl;
						cout << "--------" << endl;
						frame = 0;
						msg = "";
					}

					deletePacket(appPacket, transPacket, netPacket, phyPacket);

				} else if (clientN == nullptr) {
					cout << "Client " << clientR.client_id << " receiving frame #" << frame << " from client " << senderID << ", but intended for client " << appPacket->receiver_ID << ". Forwarding..." << endl;
					cout << "Error: Unreachable destination. Packets are dropped after " << hops << " hops!" << endl;

					if (msg[msg.size() - 1] == '!' || msg[msg.size() - 1] == '.' || msg[msg.size() - 1] == '?') {
						clientR.log_entries.emplace_back("2023-11-22 20:30:03", msg, frame, hops, appPacket->sender_ID, appPacket->receiver_ID, false, ActivityType::MESSAGE_DROPPED);
						cout << "--------" << endl;
						frame = 0;
						msg = "";
					}

					deletePacket(appPacket, transPacket, netPacket, phyPacket);

				} else {
					if (frame == 1) cout << "Client " << clientR.client_id << " receiving a message from client " << senderID << ", but intended for client " << appPacket->receiver_ID << ". Forwarding..." << endl;

					phyPacket->sender_MAC_address = clientR.client_mac;
					phyPacket->receiver_MAC_address = clientN->client_mac;

					stack<Packet *> store;
					store.push(appPacket);
					store.push(transPacket);
					store.push(netPacket);
					store.push(phyPacket);
					clientR.outgoing_queue.push(store);

					cout << "Frame #" << frame << " MAC address change: New sender MAC " << phyPacket->sender_MAC_address << ", new receiver MAC " << phyPacket->receiver_MAC_address << endl;

					if (msg[msg.size() - 1] == '!' || msg[msg.size() - 1] == '.' || msg[msg.size() - 1] == '?') {
						clientR.log_entries.emplace_back("2023-11-22 20:30:03", msg, frame, hops, appPacket->sender_ID, appPacket->receiver_ID, true, ActivityType::MESSAGE_FORWARDED);
						cout << "--------" << endl;
						frame = 0;
						msg = "";
					}
				}
				clientR.incoming_queue.pop();
			}
		}
	}
}

void Network::FrameInfo(vector<Client> clients, const vector<string> &command) {
	Client *client = findID(clients, command[1]);
	queue<stack<Packet *>> queue = (command[2] == "out") ? client->outgoing_queue : client->incoming_queue;
	string que = (command[2] == "out") ? "outgoing" : "incoming";

	int frame{};
	if (!queue.empty()) {
		while (!queue.empty()) {
			frame++;

			auto *phyPacket = dynamic_cast<PhysicalLayerPacket *>(queue.front().top());
			queue.front().pop();

			auto *netPacket = dynamic_cast<NetworkLayerPacket *>(queue.front().top());
			queue.front().pop();

			auto *transPacket = dynamic_cast<TransportLayerPacket *>(queue.front().top());
			queue.front().pop();

			auto *appPacket = dynamic_cast<ApplicationLayerPacket *>(queue.front().top());
			queue.front().pop();

			if (frame == stoul(command[3])) {
				int hops = calculateHops(clients, appPacket->sender_ID, client->client_id, appPacket->receiver_ID);

				cout << "Current Frame #" << command[3] << " on the " << que << " queue of client " << client->client_id << endl;
				cout << "Carried Message: \"" << appPacket->message_data << "\"" << endl;
				cout << "Layer 0 info: ";
				appPacket->print();
				cout << "Layer 1 info: ";
				transPacket->print();
				cout << "Layer 2 info: ";
				netPacket->print();
				cout << "Layer 3 info: ";
				phyPacket->print();
				cout << "Number of hops so far: " << hops << endl;
				return;
			}
			queue.pop();
		}
	}
	cout << "No such frame." << endl;
}

void Network::QInfo(vector<Client> clients, const vector<string> &command) {
	Client *client = findID(clients, command[1]);
	queue<stack<Packet *>> queue = (command[2] == "out") ? client->outgoing_queue : client->incoming_queue;
	string que = (command[2] == "out") ? "Outgoing" : "Incoming";

	cout << "Client " << client->client_id << " " << que << " Queue Status" << endl;
	cout << "Current total number of frames: " << queue.size() << endl;
}

void Network::PrintLog(vector<Client> clients, const vector<string> &command) {
	Client *client = findID(clients, command[1]);
	if (!client->log_entries.empty()) {
		cout << "Client " << client->client_id << " Logs:" << endl;

		int entry{};
		for (const Log &log: client->log_entries) {
			entry++;
			cout << "--------------" << endl;

			cout << "Log Entry #" << entry << ":" << endl;
			if (log.activity_type == ActivityType::MESSAGE_SENT) {
				cout << "Activity: Message Sent" << endl;
			} else if (log.activity_type == ActivityType::MESSAGE_FORWARDED) {
				cout << "Activity: Message Forwarded" << endl;
			} else if (log.activity_type == ActivityType::MESSAGE_DROPPED) {
				cout << "Activity: Message Dropped" << endl;
			} else if (log.activity_type == ActivityType::MESSAGE_RECEIVED) {
				cout << "Activity: Message Received" << endl;
			}
			cout << "Timestamp: " << log.timestamp << endl;
			cout << "Number of frames: " << log.number_of_frames << endl;
			cout << "Number of hops: " << log.number_of_hops << endl;
			cout << "Sender ID: " << log.sender_id << endl;
			cout << "Receiver ID: " << log.receiver_id << endl;
			cout << "Success: " << (log.success_status ? "Yes" : "No") << endl;
			if (log.activity_type == ActivityType::MESSAGE_SENT || log.activity_type == ActivityType::MESSAGE_RECEIVED) {
				cout << "Message: \"" << log.message_content << "\"" << endl;
			}
		}
	}
}

vector<string> Network::smashMessage(vector<string> command) const {
	string message;
	for (int i = 3; i < command.size(); i++) {
		message = (i == 3) ? message.append(command[i].substr(1, command[i].size() - 1)) : message.append(" " + command[i]);
	}
	message.pop_back();

	cout << "Message to be sent: \"" << message << "\"\n\n";

	vector<string> messages;
	int piece = (message.size() % limitMsg == 0) ? (int) message.size() / limitMsg : (int) message.size() / limitMsg + 1;
	for (int i = 0; i < piece; i++) {
		int length = (message.size() / limitMsg < 1) ? (int) message.size() : limitMsg;
		messages.push_back(message.substr(i * limitMsg, length));
	}
	return messages;
}

Client *Network::findID(vector<Client> &clients, const string &id) {
	for (Client &client: clients) {
		if (client.client_id == id) {
			return &client;
		}
	}
	return nullptr;
}

Client *Network::findMAC(vector<Client> &clients, const string &mac) {
	for (Client &client: clients) {
		if (client.client_mac == mac) {
			return &client;
		}
	}
	return nullptr;
}

int Network::calculateHops(vector<Client> &clients, string &firstID, string &receiverID, string &lastID) {
	int hops{};
	string id = firstID;
	while (id != receiverID) {
		for (Client &client: clients) {
			if (client.client_id == id) {
				id = client.routing_table[lastID];
				hops++;
				break;
			}
		}
	}
	return hops;
}

void Network::deletePacket(ApplicationLayerPacket *app, TransportLayerPacket *trans, NetworkLayerPacket *net, PhysicalLayerPacket *phy) {
	delete app;
	delete trans;
	delete net;
	delete phy;
}

Network::~Network() = default;
