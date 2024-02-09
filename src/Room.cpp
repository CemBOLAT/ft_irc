#include "Room.hpp"


Room::Room() 
    : _clients(), _name(""), _topic(""), Operator(NULL), _keycode(0),
    _key(""), _chanelLimit(0){}

Room::~Room(){}
Room::Room(const Room &other){
	*this = other;
}

Room &Room::operator=(const Room &other)
{
	if (this != &other)
	{
		while (!_clients.empty())
			_clients.pop_back();
		for (size_t i = 0; i < other._clients.size(); i++)
			_clients.push_back(other._clients[i]);
		_name = other._name;
		_topic = other._topic;
		_keycode = other._keycode;
		_key = other._key;
		_chanelLimit = other._chanelLimit;
		Operator = other.Operator;
	}
	return *this;
}

void    Room::removeClient(int fd){
    for (size_t i = 0; i < _clients.size(); i++){
        if (_clients[i].getFd() == fd)
		{
		    _clients.erase(_clients.begin() + i);
			break;
		}
    }
}

bool Room::isClientInChannel(const string &nick) const
{
	for (size_t i = 0; i < _clients.size(); i++)
	{
		if (_clients[i].getNick() == nick)
			return true;
	}
	return false;
}
bool Room::isClientInChannel(int fd) const
{
	for (size_t i = 0; i < _clients.size(); i++)
	{
		if (_clients[i].getFd() == fd)
			return true;
	}
	return false;
}
Client &Room::getClient(const string &nick)
{
	std::vector<Client>::iterator it = _clients.begin();
	for (; it != _clients.end(); ++it)
	{
		if (it->getNick() == nick)
			break;
	}
		return *it;
	}
bool	Room::isOperator(const Client &client) const
{
	if (Operator)
		return Operator->getNick() == client.getNick();
	return false;
}