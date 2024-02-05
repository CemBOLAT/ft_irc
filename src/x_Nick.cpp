#include "../include/Client.hpp"
#include "../include/Executor.hpp"
#include "../include/Server.hpp"
#include "../include/Exception.hpp"
#include "../include/Utils.hpp"
#include <iostream>
#include <string>
#include <vector>

#define RPL_NICK(nick, user, ip, newnick) ":" + nick + "!" + user + "@" + ip + " NICK :" + newnick + "\r\n"
#define ERR_NICKNAMEEMPTY(source)				    ": 433 " + source + " " + source  + " :Nickname cannot empty" + "\r\n"
#define ERR_NICKNAMEINUSE(source)				    ": 433 " + source + " " + source  + " :Nickname is already in use" + "\r\n"


int	isNickExist(const std::string &s, const std::vector<Client> &clients) {
	for (std::vector<Client>::const_iterator it = clients.begin(); it != clients.end(); ++it) {
		if (s == it->getNick())
			return 1;
	}
	return 0;
}

void Server::nick(C_VECT_STR_R params, Client &client, fd_set &fd)
{
	if (params[1].empty()){
		Utils::instaWrite(client.getFd(), ERR_NICKNAMEEMPTY(client.getUserByHexChat()));
	}
	else if (isNickExist(params[1], getClients())){
		Utils::instaWrite(client.getFd(), ERR_NICKNAMEINUSE(client.getUserByHexChat()));
	}
	else{
		FD_SET(client.getFd(), &fd);
		client.getmesagesFromServer().push_back(RPL_NICK(client.getNick(), client.getUserName(), client._ip, params[1]));
		for (std::vector<Room>::iterator it = channels.begin(); it != channels.end(); ++it) {
			for (std::vector<Client>::iterator cit = it->getClients().begin(); cit != it->getClients().end(); ++cit) {
				if (client.getNick() == cit->getNick())
				{
					cit->setNick(params[1]);
					break;
				}
			}
		}
	}
}
