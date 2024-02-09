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
	Room();
	~Room();
	Room(const Room &other);
	Room &operator=(const Room &other);

	vector<Client>	&getClients() { return _clients; }
	string			getName() const { return _name; }
	string			getTopic() const { return _topic; }
	int				getKeycode() const { return _keycode; }
	string			getKey() const { return _key ; }
	int				getChanelLimit() const { return _chanelLimit; }
	Client			*getOperator() { return Operator; }

	void			setOperator(Client *Operator) { this->Operator = Operator; }
	void			setName(const string &name) { _name = name; }
	void			setTopic(const string &topic) { _topic = topic; }
	void			sddClient(Client client) { _clients.push_back(client); }
	void			setKeycode(int keycode) { _keycode = keycode; }
	void			setKey(const string &key) { _key = key; }
	void			setChanelLimit(int limit) { _chanelLimit = limit; }
	void			removeClient(int fd);

	bool			isClientInChannel(const string &nick) const;
	bool			isClientInChannel(int fd) const;
	Client			&getClient(const string &nick);
	bool			isOperator(const Client &client) const;

private:
	vector<Client>	_clients; // bunun için pointer veya referans kullanılmalı
	string			_name;
	string 			_topic;
	Client			*Operator;
	int				_keycode;
	string			_key;
	int				_chanelLimit;
};
