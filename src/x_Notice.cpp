#include "../include/Client.hpp"
#include "../include/Room.hpp"
#include "../include/Executor.hpp"
#include "../include/Exception.hpp"
#include "Define.hpp"
#include "Utils.hpp"
#include <iostream>
#include <string>
#include <vector>

#define ERR_NOSUCHNICK(nick, target) ":401 " + nick + " " + target + " :No such nick/channel\r\n"
#define ERR_NOSUCHCHANNEL(nick, channel) ":403 " + nick + " " + channel + " :No such channel\r\n"
#define ERR_NOTONCHANNEL(nick, channel) ":442 " + nick + " " + channel + " :You're not on that channel\r\n"
#define RPL_NOTICE(source, target, message) ":" + source + " NOTICE " + target + " :" + message + "\r\n"
#define ERR_NEEDMOREPARAMS(nick, command) ":461 " + nick + " " + command + " :Not enough parameters\r\n"


// /notice #chan message
// /notice user message
void	Server::notice(std::string	&input, Client &fd){
	std::vector<std::string> params = Utils::ft_split(input, " ");
	if (params.size() < 2){
		Utils::instaWrite(fd.getFd(), ERR_NEEDMOREPARAMS(fd.getNick(), "NOTICE"));
		return;
	}
	if (params[0][0] == '#'){
		vector<Room>::iterator it = this->channels.begin();
		for (; it != this->channels.end(); ++it){
			if (it->getName() == params[0]){
				break;
			}
		}
		if (it == this->channels.end()){
			Utils::instaWrite(fd.getFd(), ERR_NOSUCHCHANNEL(fd.getNick(), params[0]));
			return;
		}
		if (!it->isClientInChannel(fd.getFd())){
			Utils::instaWrite(fd.getFd(), ERR_NOTONCHANNEL(fd.getNick(), params[0]));
			return;
		}
		std::string message = Utils::ft_join(params, " ", 1);
		if (params[1][0] == ':'){
			message = message.substr(1, message.length());
		}
		Utils::instaWriteAll(it->getClients(), RPL_NOTICE(fd.getUserByHexChat(), params[0], message));
	}
	else {
		vector<Client>::iterator it = this->clients.begin();
		for (; it != this->clients.end(); ++it){
			if (it->getNick() == params[0]){
				break;
			}
		}
		if (it == this->clients.end()){
			Utils::instaWrite(fd.getFd(), ERR_NOSUCHNICK(fd.getNick(), params[0]));
			return;
		}
		std::string message = Utils::ft_join(params, " ", 1);
		if (params[1][0] == ':'){
			message = message.substr(1, message.length());
		}
		Utils::instaWrite(it->getFd(), RPL_NOTICE(fd.getUserByHexChat(), it->getNick(), message));
	}
}
