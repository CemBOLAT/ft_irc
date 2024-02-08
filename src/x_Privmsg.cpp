#include "../include/Client.hpp"
#include "../include/Executor.hpp"
#include "../include/Exception.hpp"
#include "../include/Client.hpp"
#include "../include/Server.hpp"
#include "../include/Room.hpp"
#include "../include/Utils.hpp"
#include <iostream>
#include <string>
#include <vector>

#define RPL_PRIVMSG(source, target, message)		":" + source + " PRIVMSG " + target + " :" + message + "\r\n"
#define MSG_GROUP(nick,user, host, room, message) ":" + nick + "!" + user + "@" + host + " PRIVMSG  " + room + " :" + message + "\r\n"
#define ERR_NEEDMOREPARAMS(nick, command) ":461 " + nick + " " + command + " :Not enough parameters\r\n"
#define ERR_NOTONCHANNEL(nick, channel) ":442 " + nick + " " + channel + " :You're not on that channel\r\n"
#define ERR_NOSUCHCHANNEL(nick, channel) ":403 " + nick + " " + channel + " :No such channel\r\n"
#define ERR_NOSUCHNICK(nick, target) ":401 " + nick + " " + target + " :No such nick/channel\r\n"

#include <iostream>
#include <unistd.h>
#include <stdlib.h>

int isInRoom(Client &client, Server &server, std::string room) {
	for (std::vector<Room>::iterator it = server.getRooms().begin(); it != server.getRooms().end(); it++) {
		if (it->getName() == room) {
			for (std::vector<Client>::iterator it2 = it->getClients().begin(); it2 != it->getClients().end(); it2++) {
				if (it2->getFd() == client.getFd())
					return 1;
			}
		}
	}
	return 0;
}


void Server::privmsg(std::string &input, Client &client) {
	std::vector<std::string> params = Utils::ft_split(input, " ");
   if (params.size() < 2) {
		Utils::instaWrite(client.getFd(), ERR_NEEDMOREPARAMS(client.getNick(), "PRIVMSG"));
		return;
	}
	for (std::vector<Client>::iterator it = this->getClients().begin(); it != this->getClients().end(); it++) {
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
