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
	for (VECT_ITER_CLI it = this->getClients().begin(); it != this->getClients().end(); it++){
		if (params[0] == (*it).getUserName() || params[0] == (*it).getNick()){
			std::string message = Utils::ft_join(params, " ", 1);
			if (params[1][0] == ':')
				message = message.substr(1, message.length());
			(*it).getmesagesFromServer().push_back(RPL_NOTICE(fd.getUserByHexChat(), params[0], message));
			FD_SET((*it).getFd(), &this->writefds);
		}
		if (params[0][0] == '#' && isClientInRoom(*it, params[0])){
			std::string message = Utils::ft_join(params, " ", 1);
			if (params[1][0] == ':')
				message = message.substr(1, message.length());
			(*it).getmesagesFromServer().push_back(RPL_NOTICE(fd.getUserByHexChat(), params[0], message));
			FD_SET((*it).getFd(), &this->writefds);
		}
	}
}
