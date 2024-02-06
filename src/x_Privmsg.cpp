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

void	Server::privmsg(C_STR_REF input, Client &client){
	std::vector<std::string> params = Utils::ft_split(input, " ");

	for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it){
		std::cout << "params[0] = " << params[0] << std::endl;
		if (it->getFd() != client.getFd() && it->getNick() == params[0]){
			if (!isClientInRoom(*it, params[0])){
			}
			string	newParam1;
			if (params[1][0] == ':')
				newParam1 = params[1].substr(1, params[1].length());
			else
				newParam1 = params[1];
			std::cout << RPL_PRIVMSG(client.getUserByHexChat(), params[0], newParam1) << std::endl;
			(*it).getmesagesFromServer().push_back(RPL_PRIVMSG(client.getUserByHexChat(), params[0], newParam1));
			FD_SET((*it).getFd(), &this->writefds);
		}
		else if (params[1] == (*it).getUserName() || params[1] == (*it).getNick()){
			if (params[1].find("PING") != std::string::npos){
				params[1] = params[0] + " " + client.getNick();
				//Server::ping(params, client);
				break;
			}
			string	newParam1;
			if (params[1][0] == ':')
				newParam1 += params[1].substr(1, params[1].length());
			else
				newParam1 += params[1];
			(*it).getmesagesFromServer().push_back(RPL_PRIVMSG(client.getUserByHexChat(), params[0], newParam1));
			FD_SET((*it).getFd(), &this->writefds);
		}
	}
}
