#include "Client.hpp"
#include "Room.hpp"
#include "Executor.hpp"
#include "Exception.hpp"
#include "Define.hpp"
#include "Utils.hpp"
#include <iostream>
#include <string>
#include <vector>
#include "Define.hpp"

// /notice #chan message
// /notice user message
void	Server::notice(std::string	&input, Client &fd){
	std::vector<std::string> params = Utils::ft_split(input, " ");
	if (params.size() < 2){
		Utils::instaWrite(fd.getFd(), ERR_NEEDMOREPARAMS(fd.getNick(), "NOTICE"));
		return;
	}
	if (params[0][0] == '#'){
		VECT_ITER_CHA it = this->channels.begin();
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
		VECT_ITER_CLI it = this->clients.begin();
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
