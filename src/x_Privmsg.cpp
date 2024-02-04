#include "../include/Client.hpp"
#include "../include/Executor.hpp"
#include "../include/Exception.hpp"
#include "../include/Client.hpp"
#include "../include/Server.hpp"
#include "../include/Room.hpp"
#include <iostream>
#include <string>
#include <vector>

void	Server::privmsg(C_VECT_STR_R params, Client &client){
	(void)params;
	(void)client;
	//FD_SET(client.getFd(), &client.getWritefds());
	//if (params.size() < 3){
	//	client.getmesagesFromServer().push_back("PRIVMSG :Not enough parameters\n");
	//	return;
	//}
	//if (params[1][0] != '#'){
	//	// message for a user
	//	for (size_t i = 0; i < clients.size(); i++){
	//		if (clients[i].getNick() == params[1]){
	//			clients[i].getmesagesFromServer().push_back("PRIVMSG " + params[1] + " :" + params[2] + "\n");
	//			return;
	//		}
	//	}
	//	client.getmesagesFromServer().push_back("PRIVMSG :User not found\n");
	//}
	//else{
	//	// message for a channel
	//	for (size_t i = 0; i < channels.size(); i++){
	//		if (channels[i].getName() == params[1]){
	//			for (size_t j = 0; j < channels[i].getClients().size(); j++){
	//				channels[i].getClients()[j].getmesagesFromServer().push_back("PRIVMSG " + params[1] + " :" + params[2] + "\n");
	//			}
	//			return;
	//		}
	//	}
	//	client.getmesagesFromServer().push_back("PRIVMSG :You are not in this channel\n");
	//}
}
