#include "../includes/Room.hpp"
#include <sys/socket.h>

Room::Room(Client *_c, string name, string password) {
	this->admin = _c;
	this->name = name;
	this->password = password;
	this->isPrivate = false;
	this->clientCount = 1;
	this->maxClientCount = 10;
	this->clients.push_back(_c);
}


void	Room::removeClient(Client *client) {
	for (vector<Client *>::iterator it = clients.begin(); it != clients.end(); it++) {
		if (*it == client) {
			if (client == admin) {
				if (clients.size() > 1) {
					admin = clients[1];
					string msg = client->getNickname() + " left the room. " + admin->getNickname() + " is the new admin of the room.\r\n";
					send(client->getSocket(), msg.c_str(), msg.length(), 0);
					

				}
				else {
					admin = NULL;
					send(client->getSocket(), "You are the last client in the room. Room is deleted.\r\n", 52, 0);
				}
			}
			clients.erase(it);
			clientCount--;
			break ;
		}
	}
}
