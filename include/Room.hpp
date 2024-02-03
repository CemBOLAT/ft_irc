#pragma once

#include <string>
#include <vector>

using std::string;
using std::vector;

class Client;

class Room
{
public:
	Room() : _name(""), _topic("") {}
	~Room() {}
	Room(const Room &other) : _clients(other._clients), _name(other._name), _topic(other._topic) {}
	Room &operator=(const Room &other)
	{
		if (this != &other)
		{
			_clients = other._clients;
			_name = other._name;
			_topic = other._topic;
		}
		return *this;
	}

	vector<Client> &getClients() { return _clients; }
	string &getName() { return _name; }
	string &getTopic() { return _topic; }

	void setName(const string &name) { _name = name; }
	void setTopic(const string &topic) { _topic = topic; }
	void addClient(const Client &client) { _clients.push_back(client); }

private:
	vector<Client> _clients;
	string _name;
	string _topic;
};
