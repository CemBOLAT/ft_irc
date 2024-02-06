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
#define MSG_GROUP(nick,user, host, room, message) ":" + nick + "!" + user + "@" + host + " PRIVMSG #" + room + " :" + message + "\r\n"
#define ERR_NEEDMOREPARAMS(nick, command) ":461 " + nick + " " + command + " :Not enough parameters\r\n"
#define ERR_NOTONCHANNEL(nick, channel) ":442 " + nick + " " + channel + " :You're not on that channel\r\n"
#define ERR_NOSUCHCHANNEL(nick, channel) ":403 " + nick + " " + channel + " :No such channel\r\n"
#define ERR_NOSUCHNICK(nick, target) ":401 " + nick + " " + target + " :No such nick/channel\r\n"

std::string	getPrefixName(Client &client){
    std::string prefixName = client.getNick();
    if (!(client.getUserName().empty()))
        prefixName += '!' + client.getUserName();
    if (!(client.getHostName().empty()))
        prefixName += '@' + client.getHostName();
    return (prefixName);
}
void Server::privmsg(const std::string &input, Client &client) {
    std::vector<std::string> params = Utils::ft_split(input, " ");

    if (params.size() < 2) {
        Utils::instaWrite(client.getFd(), ERR_NEEDMOREPARAMS(client.getNick(), "PRIVMSG"));
        return;
    }

    std::string target = params[0];
    std::string message = Utils::ft_join(params, " ", 1);
    if (message[0] == ':') {
        message = message.substr(1);
    }

    if (target[0] == '#') { // Channel message
        std::string channelName = target.substr(1);
        for (std::vector<Room>::iterator it = this->channels.begin(); it != this->channels.end(); ++it) {
            if (channelName == it->getName()) {
                if (it->isClientInChannel(client.getFd())) {
                    std::string prefix = getPrefixName(client);
                    std::string msg = MSG_GROUP(client.getNick(), client.getUserName(), client.getHostName(), channelName, message);
                    for (std::vector<Client>::iterator it2 = it->getClients().begin(); it2 != it->getClients().end(); ++it2) {
						if (it2->getFd() != client.getFd()){
                       		Utils::instaWrite(it2->getFd(), msg);
							std::cout << "MSG sent to " << it2->getNick() << std::endl;
						}
                    }
                } else {
                	Utils::instaWrite(client.getFd(), ERR_NOTONCHANNEL(client.getNick(), channelName));
                }
                return;
            }
        }
        Utils::instaWrite(client.getFd(), ERR_NOSUCHCHANNEL(client.getNick(), channelName));
    } else { // User message
        for (std::vector<Client>::iterator it = this->clients.begin(); it != this->clients.end(); ++it) {
            if (target == it->getNick()) {
                std::string msg = RPL_PRIVMSG(getPrefixName(client), target, message);
                Utils::instaWrite(it->getFd(), msg);
                return;
            }
        }
        std::string channelName = target;
		for (std::vector<Room>::iterator it = this->channels.begin(); it != this->channels.end(); ++it) {
            if (channelName == it->getName()) {
                if (it->isClientInChannel(client.getFd())) {
                    std::string prefix = getPrefixName(client);
                    std::string msg = MSG_GROUP(client.getNick(), client.getUserName(), client.getHostName(), x, message);
                    for (std::vector<Client>::iterator it2 = it->getClients().begin(); it2 != it->getClients().end(); ++it2) {
						if (it2->getFd() != client.getFd())
                        	Utils::instaWrite(it2->getFd(), msg);
                    }
                } else {
                    Utils::instaWrite(client.getFd(), ERR_NOTONCHANNEL(client.getNick(), channelName));
                }
                return;
            }
        }
        Utils::instaWrite(client.getFd(), ERR_NOSUCHNICK(client.getNick(), target));
    }
}
