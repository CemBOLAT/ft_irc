#include "../include/Client.hpp"
#include "../include/Executor.hpp"
#include "../include/Exception.hpp"
#include "../include/Client.hpp"
#include "../include/Server.hpp"
#include "../include/Room.hpp"
#include <iostream>
#include <string>
#include <vector>

#define RPL_PRIVMSG(source, target, message)		":" + source + " PRIVMSG " + target + " :" + message + "\r\n"

string	getMessage(C_VECT_STR_R params, unsigned int start){
	string message;
	for (unsigned int i = start; i < params.size(); i++){
		message += params[i];
		if (i + 1 < params.size())
			message += " ";
	}
	if (message[0] == ':')
		message = message.substr(1, message.size() - 1);
	return message;
}

void	Server::privmsg(VECT_STR params, Client &client){
	string	message = getMessage(params, 2);
	for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it){
		if (it->getFd() != client.getFd() && it->getNick() == params[1]){
			if (!isClientInRoom(*it, params[1]))
				return;
			string	newParam1;
			if (params[2][0] == ':')
				newParam1 = params[2].substr(1, params[2].length());
			else
				newParam1 = params[2];
			(*it).getmesagesFromServer().push_back(RPL_PRIVMSG(client.getUserByHexChat(), params[1], newParam1));
			FD_SET((*it).getFd(), &this->writefds);
		}
		else if (params[1] == (*it).getUserName() || params[1] == (*it).getNick()){
			if (params[2].find("PING") != std::string::npos){
				params[2] = params[2] + " " + client.getNick();
				Server::ping(params, client);
				break;
			}
			string	newParam1;
			if (params[2][0] == ':')
				newParam1 += params[2].substr(1, params[2].length());
			else
				newParam1 += params[2];
			(*it).getmesagesFromServer().push_back(RPL_PRIVMSG(client.getUserByHexChat(), params[1], newParam1));
			FD_SET((*it).getFd(), &this->writefds);
		}
	}
}
