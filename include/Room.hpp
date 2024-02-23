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
	vector<Client>	&getOperatorVector() { return _operatorVector; }
	bool 			isOperator(C_STR_REF nick) const;

	void			addOperator(const Client &client) { _operatorVector.push_back(client); }
	void			removeOperator(const Client &client) {
		for (vector<Client>::iterator it = _operatorVector.begin(); it != _operatorVector.end(); it++) {
			if (it->getNick() == client.getNick()) {
				_operatorVector.erase(it);
				break;
			}
		}
	}
	void			setName(C_STR_REF name) { _name = name; }
	void			setTopic(C_STR_REF topic) { _topic = topic; }
	void			addClient(const Client &client) { _clients.push_back(client); }
	void			setKeycode(int keycode) { _keycode = keycode; }
	void			setKey(C_STR_REF key) { _key = key; }
	void			setChanelLimit(int limit) { _chanelLimit = limit; }
	void			removeClient(int fd);

	bool			isClientInChannel(C_STR_REF nick) const;
	bool			isClientInChannel(int fd) const;
	Client			&getClient(C_STR_REF nick);
	bool			isOperator(const Client &client) const;
	void			addInvite(const Client &client) { _inviteVector.push_back(client); }
	void			removeInvite(const Client &client) {
		for (vector<Client>::iterator it = _inviteVector.begin(); it != _inviteVector.end(); it++) {
			if (it->getNick() == client.getNick()) {
				_inviteVector.erase(it);
				break;
			}
		}
	}
	bool			isInvite(const Client &client) const{
		for (vector<Client>::const_iterator it = _inviteVector.begin(); it != _inviteVector.end(); it++) {
			if (it->getNick() == client.getNick()) {
				return true;
			}
		}
		return false;
	
	}
private:
	vector<Client>	_clients; // bunun için pointer veya referans kullanılmalı
	string			_name;
	string 			_topic;
	vector<Client>	_operatorVector;
	int				_keycode; //kanalın şifreli mi kullanıcı limiti var diye koyduğumuz flag
	string			_key;
	int				_chanelLimit;
	vector<Client>	_inviteVector;
};
