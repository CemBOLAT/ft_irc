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
	Room() : _name(""), _topic(""), _keycode(0) {}
	~Room() {}
	Room(const Room &other){
		*this = other;
	}
	Room &operator=(const Room &other)
	{
		if (this != &other)
		{
			_clients = other._clients;
			_name = other._name;
			_topic = other._topic;
			_keycode = other._keycode;
			_key = other._key;
			_chanelLimit = other._chanelLimit;
			Operator = other.Operator;
		}
		return *this;
	}

	vector<Client> &getClients() { return _clients; }
	string getName() const { return _name; }
	string getTopic() const { return _topic; }
	int	getKeycode() const { return _keycode; }
	string getKey() const { return _key ; }
	int	getChanelLimit() const { return _chanelLimit; }
	Client *getOperator() { return Operator; }

	void setOperator(Client *Operator) { this->Operator = Operator; }
	void setName(const string &name) { _name = name; }
	void setTopic(const string &topic) { _topic = topic; }
	void addClient(Client client) { _clients.push_back(client); }
	void setKeycode(int keycode) { _keycode = keycode; }
	void setKey(const string &key) { _key = key; }
	void setChanelLimit(int limit) { _chanelLimit = limit; }
	void removeClient(int fd)
	{
		for (size_t i = 0; i < _clients.size(); i++)
		{
			if (_clients[i].getFd() == fd)
			{
				_clients.erase(_clients.begin() + i);
				break;
			}
		}
	}

	bool isClientInChannel(const string &nick) const
	{
		for (size_t i = 0; i < _clients.size(); i++)
		{
			if (_clients[i].getNick() == nick)
				return true;
		}
		return false;
	}
	bool isClientInChannel(int fd) const
	{
		for (size_t i = 0; i < _clients.size(); i++)
		{
			if (_clients[i].getFd() == fd)
				return true;
		}
		return false;
	}
	Client &getClient(const string &nick)
	{
		std::vector<Client>::iterator it = _clients.begin();
		for (; it != _clients.end(); ++it)
		{
			if (it->getNick() == nick)
				break;
		}
		return *it;
	}
	bool	isOperator(const Client &client) const
	{
		if (Operator)
			return Operator->getNick() == client.getNick();
		return false;
	}

private:
	vector<Client> _clients;
	string	_name;
	string 	_topic;
	Client	*Operator;
	int		 _keycode;
	string	_key;
	int		_chanelLimit;
};
