#ifndef NETWORK_H
#define NETWORK_H

#include <vector>
#include <iostream>
#include "Packet.h"
#include "Client.h"

using namespace std;

class Network {

public:
	Network();

	~Network();

	void process_commands(vector<Client> &clients, vector<string> &commands, int message_limit, const string &sender_port, const string &receiver_port);

	static vector<Client> read_clients(string const &filename);

	static void read_routing_tables(vector<Client> &clients, string const &filename);

	static vector<string> read_commands(const string &filename);

	void Message(vector<Client> &clients, const vector<string> &command) const;

	static void Send(vector<Client> &clients);

	static void Receive(vector<Client> &clients);

	static void FrameInfo(vector<Client> clients, const vector<string> &command);

	static void QInfo(vector<Client> clients, const vector<string> &command);

	static void PrintLog(vector<Client> clients, const vector<string> &command);

	vector<string> smashMessage(vector<string> command) const;

	static Client *findID(vector<Client> &clients, const string &id);

	static Client *findMAC(vector<Client> &clients, const string &mac);

	static int calculateHops(vector<Client> &clients, string &firstID, string &receiverID, string &lastID);

	static void deletePacket(ApplicationLayerPacket *app, TransportLayerPacket *tra, NetworkLayerPacket *net, PhysicalLayerPacket *phy);

	int limitMsg{};
	string portS;
	string portR;
};

#endif
