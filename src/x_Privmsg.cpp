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

std::string	getPrefixName(Client &client){
    std::string prefixName = client.getNick();
    if (!(client.getUserName().empty()))
        prefixName += '!' + client.getUserName();
    if (!(client.getHostName().empty()))
        prefixName += '@' + client.getHostName();
    return (prefixName);
}

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
					for (std::vector<Client>::iterator it2 = this->clients.begin(); it2 != this->clients.end(); it2++){
						if (it->isClientInChannel(it2->getFd())){
							if (it2->getFd() != client.getFd()){
								Utils::instaWrite(it2->getFd(), RPL_PRIVMSG(getPrefixName(client), "#" + chanelName, message));
							}
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
		for (std::vector<Client>::iterator it = this->clients.begin(); it != this->clients.end(); it++){ // it works perfectooo
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
					for (std::vector<Client>::iterator it2 = this->clients.begin(); it2 != this->clients.end(); it2++){
						if (it->isClientInChannel(it2->getFd())){
							if (it2->getFd() != client.getFd()){
								std::cout << "aaa" << std::endl;
								Utils::instaWrite(it2->getFd(), RPL_PRIVMSG(getPrefixName(client), "#" + chanelName, message));
							}
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
}
