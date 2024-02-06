#include "../include/Client.hpp"
#include "../include/Executor.hpp"
#include "../include/Exception.hpp"
#include "../include/Utils.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

#define ERR_NEEDMOREPARAMS(source, command)			": 461 " + source + " " + command + " :Not enough parameters" + "\r\n"
#define ERR_NOSUCHCHANNEL(source, channel)				"403 " + source + " " + channel + " :No such channel"

void Server::who(const std::string &command, Client &client) {
	std::vector<std::string> params = Utils::ft_split(command, " ");
	if (params.empty()) {
		Utils::instaWrite(client.getFd(), ERR_NEEDMOREPARAMS(client.getNick(), "WHO"));
		return;
	}

	std::string channelName = params[0];
	//if (channelName[0] == '#')
	//	channelName = channelName.substr(1);
	bool channelFound = false;

	for (size_t i = 0; i < this->channels.size(); ++i) {
		Room &channel = this->channels[i];
		if (channelName == channel.getName()) {
			channelFound = true;
			for (size_t j = 0; j < channel.getClients().size(); ++j) {
				Client &user = channel.getClients()[j];
				std::string channelOperator = channel.isOperator(user) ? "@" : "";
				std::string hopcount = "0"; // Since this server doesn't forward WHO requests
				std::string response = ":" + channel.getName() + " " +
										user.getUserName() + " " + user.getHostName() + " " + user.getNick() + " " + channelOperator + " :" +
										hopcount + " " + user.getRealName() + "\r\n";
				Utils::instaWrite(client.getFd(), response);
			}
			break;
		}
	}

	if (!channelFound) {
		Utils::instaWrite(client.getFd(), ERR_NOSUCHCHANNEL(client.getNick(), channelName));
	}
}

