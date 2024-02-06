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
#define MSG_GROUP(nick,user, host, room, message) ":" + nick + "!" + user + "@" + host + " PRIVMSG " + room + " :" + message + "\r\n"

void	Server::privmsg(C_STR_REF input, Client &client){
	std::vector<std::string> params = Utils::ft_split(input, " ");

	if (params.size() < 2){
		Utils::instaWrite(client.getFd(), "PRIVMSG :Not enough parameters\n\r");
		return;
	}
	if (params[0][0] == '#'){
		string chanelName = params[0].substr(1, params[0].size() - 1);
		for (std::vector<Room>::iterator it = this->channels.begin(); it != this->channels.end(); it++){
			if (chanelName == it->getName()){
				if (it->isClientInChannel(client.getFd())){
					std::string message = Utils::ft_join(params, " ", 1);
					if (message[0] == ':'){
						message = message.substr(1, message.size() - 1);
					}
					std::cout << "chanelName:$" << chanelName << "#" << std::endl;
					for (std::vector<Client>::iterator it2 = it->getClients().begin(); it2 != it->getClients().end(); it2++){
						{
							if (it2->getFd() != client.getFd())
								Utils::instaWrite(it2->getFd(), MSG_GROUP(client.getNick(), client.getUserName(), client.getHostName(), "#" + chanelName, message));
						}
					}
				}
				else {
					Utils::instaWrite(client.getFd(), "PRIVMSG :You are not in that channel\n\r");
				}
				return;
			}
		}
	}
	else {
		for (std::vector<Client>::iterator it = this->clients.begin(); it != this->clients.end(); it++){
			if (params[0] == it->getNick()){
				std::string message = Utils::ft_join(params, " ", 1);
				if (message[0] == ':'){
					message = message.substr(1, message.size() - 1);
				}
				Utils::instaWrite(it->getFd(), RPL_PRIVMSG(client.getUserByHexChat(), it->getNick(), message));
				return;
			}
		}
		string chanelName = params[0];
		for (std::vector<Room>::iterator it = this->channels.begin(); it != this->channels.end(); it++){
			if (chanelName == it->getName()){
				if (it->isClientInChannel(client.getFd())){
					std::string message = Utils::ft_join(params, " ", 1);
					if (message[0] == ':'){
						message = message.substr(1, message.size() - 1);
					}
					Utils::instaWrite(client.getFd(), RPL_PRIVMSG(client.getUserByHexChat(), "#" + chanelName, message));
				}
			}
			else {
				Utils::instaWrite(client.getFd(), "PRIVMSG :You are not in that channel\n\r");
			}
			return;
		}
	}
}
