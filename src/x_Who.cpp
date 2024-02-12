#include "Client.hpp"
#include "Executor.hpp"
#include "Exception.hpp"
#include "Utils.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "Define.hpp"

#define RPL_WHOREPLY(nick, channel, user, host, server, nickName, hops, realName) (std::string(":") + "IRC" + " 352 " + nick + " " + channel + " " + user + " " + host + " " + server + " " + nickName + " " + hops + " " + realName + " H :0 " + realName + "\r\n")

void Server::who(C_STR_REF command, Client &client) {
	std::vector<std::string> params = Utils::ft_split(command, " ");
	if (params.empty()) {
		Utils::instaWrite(client.getFd(), ERR_NEEDMOREPARAMS(client.getNick(), "WHO"));
		return;
	}
	bool channelFound = false;
	for (VECT_ITER_CHA it = channels.begin(); it != channels.end(); ++it) {
		if (it->getName() == params[0]) {
			for (VECT_ITER_CLI cit = it->getClients().begin(); cit != it->getClients().end(); ++cit) {
				std::string operatorSymbol = it->isOperator(*cit) ? "@" : "";
				Utils::instaWrite(client.getFd(), RPL_WHOREPLY(client.getNick(), it->getName(), cit->getUserName(), cit->getHostName(), cit->getServerName(), cit->getNick(), "0", cit->getRealName()));
				channelFound = true;
			}
			Utils::instaWrite(client.getFd(), ":IRC 315 " + client.getNick() + " " + it->getName() + " :End of /WHO list.\r\n");
			return;
		}
	}
	//
	if (!channelFound) {
		Utils::instaWrite(client.getFd(), ERR_NOSUCHCHANNEL(client.getNick(), params[0]));
	}
}