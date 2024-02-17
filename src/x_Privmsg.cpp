#include "Client.hpp"
#include "Executor.hpp"
#include "Exception.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include "Room.hpp"
#include "Utils.hpp"
#include <iostream>
#include <string>
#include <vector>
#include "Define.hpp"

#include <iostream>
#include <unistd.h>
#include <stdlib.h>

int isInRoom(Client &client, Server &server, std::string room) {
	for (VECT_ITER_CHA it = server.getRooms().begin(); it != server.getRooms().end(); it++) {
		if (it->getName() == room) {
			for (VECT_ITER_CLI it2 = it->getClients().begin(); it2 != it->getClients().end(); it2++) {
				if (it2->getFd() == client.getFd())
					return 1;
			}
		}
	}
	return 0;
}


void Server::privmsg(std::string &input, Client &client) {
	VECT_STR params = Utils::ft_split(input, " ");
   if (params.size() < 2) {
		Utils::instaWrite(client.getFd(), ERR_NEEDMOREPARAMS(client.getNick(), "PRIVMSG"));
		return;
	}
	for (VECT_ITER_CLI it = this->getClients().begin(); it != this->getClients().end(); it++) {
		 if (it->getFd() != client.getFd() && isInRoom(*it, *this, params[0])) {
			if (!isInRoom(client, *this, params[0]))
				return;
			std::string message = Utils::ft_join(params, " ", 1);
			if (params[1][0] == ':')
				message = message.substr(1, message.length());
			(*it).getmesagesFromServer().push_back(RPL_PRIVMSG(client.getUserByHexChat(), params[0], message));
			FD_SET((*it).getFd(), &this->writefds);
		}
		else if (params[0] == (*it).getUserName() || params[0] == (*it).getNick()) {
			if (params[1].find("PING") != std::string::npos) {
				input = input + " " +client.getNick();
				Server::ping(input, client);
				break;
			}
			std::string message = Utils::ft_join(params, " ", 1);
			if (params[1][0] == ':')
				message = message.substr(1, message.length());
			(*it).getmesagesFromServer().push_back(RPL_PRIVMSG(client.getUserByHexChat(), params[0], message));
			FD_SET((*it).getFd(), &this->writefds);
		}
	}
}
