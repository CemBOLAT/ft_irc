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

#define RPL_PING(source, nick, second)				":" + source + " PONG " + nick + " :"+ second + "\r\n"

Client getCli(std::vector<Client> &clients, std::string nick) {
	std::vector<Client>::iterator it = clients.begin();
	for (; it != clients.end(); ++it) {
		if (it->getNick() == nick)
			return *it;
	}
	return *it;
}

std::string getMesg(C_VECT_STR_R params) {
	std::string msg;
	for (size_t i = 1; i < params.size(); i++) {
		msg += params[i];
		if (i + 1 < params.size())
			msg += " ";
	}
	return msg;
}

void	Server::ping(C_STR_REF params, Client &client){
	if (params.empty()){
		return;
	}
	std::vector<std::string> split = Utils::ft_split(params, " ");
	if (params.at(0) == '0' && params.at(1) == '0') {
		split[0] = split[0].substr(2, split[0].length());
		Utils::instaWrite(client.getFd(), RPL_PING(client.getUserByHexChat(), split[0], params.substr(params.find("PING")+5, params.length()-1)));
	} else if (params.find(":PING") && split[1].size()) {
		Client target = getCli(this->clients, split[0]);
		Utils::instaWrite(client.getFd(), RPL_PING(client.getUserByHexChat(), split[0], params.substr(params.find("PING")+5, params.length()-1)));
		std::string msg = "00" + client.getNick() + " :PING " + split[2];
		Server::ping(msg, target);
	} else {
		Utils::instaWrite(clients[0].getFd(), RPL_PING(client.getUserByHexChat(), std::string("FT_IRC_SERVER"), split[0]));
	}
}

