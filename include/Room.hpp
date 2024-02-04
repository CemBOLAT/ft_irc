#pragma once

#include <string>
#include <vector>

using std::string;
using std::vector;

class Client;
#include "Client.hpp"

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

	vector<Client *> &getClients() { return _clients; }
	string getName() const { return _name; }
	string getTopic() const { return _topic; }
	Client *getOperator() { return Operator; }

	void setOperator(Client *Operator) { this->Operator = Operator; }
	void setName(const string &name) { _name = name; }
	void setTopic(const string &topic) { _topic = topic; }
	void addClient(Client *client) { _clients.push_back(client); }

	bool isClientInChannel(const string &nick)
	{
		for (size_t i = 0; i < _clients.size(); i++)
		{
			if (_clients[i]->getNick() == nick)
				return true;
		}
		return false;
	}
	Client *getClient(const string &nick)
	{
		for (size_t i = 0; i < _clients.size(); i++)
		{
			if (_clients[i]->getNick() == nick)
				return _clients[i];
		}
		return NULL;
	}

private:
	vector<Client*> _clients;
	string	_name;
	string 	_topic;
	Client	*Operator;
};
