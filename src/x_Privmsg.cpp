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

string	getMessage(C_VECT_STR_R params){
	string message;
	for (unsigned int i = 2; i < params.size(); i++){
		message += params[i];
		if (i + 1 < params.size())
			message += " ";
	}
	if (message[0] == ':')
		message = message.substr(1, message.size() - 1);
	return message;
}

void	Server::privmsg(C_VECT_STR_R params, Client &client){
	FD_SET(client.getFd(), &writefds);
	if (params.size() < 3){
		client.getmesagesFromServer().push_back("PRIVMSG :Not enough parameters\n");
		return;
	}
	std::string message;
	std::string target = params[1];
	message = getMessage(params);

	for (std::vector<Room>::iterator it = channels.begin(); it != channels.end(); ++it){
		if (it->getName() == target){
			if (it->isClientInChannel(client.getNick())){
				for (std::vector<Client>::iterator it2 = it->getClients().begin(); it2 != it->getClients().end(); ++it2){
					FD_SET(it2->getFd(), &writefds);
					it2->getmesagesFromServer().push_back(RPL_PRIVMSG(client.getUserByHexChat(), target, message));
				}
				return;
			}
			else{
				client.getmesagesFromServer().push_back("PRIVMSG :You are not in this room\n");
				return;
			}
		}
	}
	for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it){
		if (it->getNick() == target || it->getUserName() == target){
			FD_SET(it->getFd(), &writefds);
			it->getmesagesFromServer().push_back(RPL_PRIVMSG(client.getUserByHexChat(), target, message));
			return;
		}
	}
}
