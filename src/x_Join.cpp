#include "../include/Client.hpp"
#include "../include/Executor.hpp"
#include "../include/Exception.hpp"
#include "../include/Client.hpp"
#include "../include/Server.hpp"
#include "../include/Room.hpp"
#include "../include/Utils.hpp"

#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#define JOIN_RESPONSE(nick, ip, channel) ":" + nick + "!" + nick + "@" + ip + " JOIN " + channel + "\r\n"
#define RPL_TOPIC(nick, ip, channel, topic) ":" + nick + "!" + nick + "@" + ip + " TOPIC " + channel + " :" + topic + "\r\n"
#define ERR_CHANNELISFULL(source, channel)			": 471 " + source + " " + channel + " :Cannot join channel (+l)" + "\r\n"           //JOIN
#define ERR_BADCHANNELKEY(source, channel)			": 475 " + source + " " + channel + " :Cannot join channel (+k)" + "\r\n"           //JOIN

namespace {
	string	getMessage(C_VECT_STR_R params){
		string message;
		for (unsigned int i = 1; i < params.size(); i++){
			message += params[i];
			if (i + 1 < params.size())
				message += " ";
		}
		if (message[0] == ':')
			message = message.substr(1, message.size() - 1);
	return message;
}
}

void Server::join(C_VECT_STR_R params, Client &client)
{
	string message = getMessage(params) , roomName, key;
	std::cout << "$" << message << "$" << std::endl;
	std::stringstream ss(message);
	if (!message.empty()){
		ss >> roomName;
		if (roomName[0] == '#'){
			roomName = roomName.substr(1, roomName.size() - 1);
		}
		ss >> key;
		if (isRoom(roomName)){
			std::cout << "hay amq" << std::endl;
			for (std::vector<Room>::iterator it = channels.begin(); it != channels.end(); it++){
				if (roomName == (*it).getName()){
					if (!it->isClientInChannel(client.getFd())){
						if ((it->getKeycode() & KEY_CODE) && (it->getKeycode() & LIMIT_CODE))
                        {
                            if (it->getChanelLimit() <= (int) it->getClients().size()) {
								FD_SET(client.getFd(), &writefds);
								client.getmesagesFromServer().push_back(ERR_CHANNELISFULL(client.getNick(), roomName));
                            }
                            else if (it->getKey() != key) {
								FD_SET(client.getFd(), &writefds);
								client.getmesagesFromServer().push_back(ERR_BADCHANNELKEY(client.getNick(), roomName));
                            }
                            else {
                                (*it).getClients().push_back(client);
								FD_SET(client.getFd(), &writefds);
								client.getmesagesFromServer().push_back(JOIN_RESPONSE(client.getNick(), client._ip , roomName));
                                if (!(*it).getTopic().empty()){
									FD_SET(client.getFd(), &writefds);
									client.getmesagesFromServer().push_back(RPL_TOPIC(client.getNick(), client._ip , roomName, (*it).getTopic()));
								}
							}
						}
						else if ((it->getKeycode() & KEY_CODE)) {
							if (it->getKey() != key) {
								FD_SET(client.getFd(), &writefds);
								client.getmesagesFromServer().push_back(ERR_BADCHANNELKEY(client.getNick(), roomName));
							}
							else {
								(*it).getClients().push_back(client);
								FD_SET(client.getFd(), &writefds);
								client.getmesagesFromServer().push_back(JOIN_RESPONSE(client.getNick(), client._ip , roomName));
								if (!(*it).getTopic().empty()){
									FD_SET(client.getFd(), &writefds);
									client.getmesagesFromServer().push_back(RPL_TOPIC(client.getNick(), client._ip , roomName, (*it).getTopic()));
								}
							}
                        }
						else if ((it->getKeycode() & LIMIT_CODE)) {
							if (it->getChanelLimit() <= (int) it->getClients().size()) {
								FD_SET(client.getFd(), &writefds);
								client.getmesagesFromServer().push_back(ERR_CHANNELISFULL(client.getNick(), roomName));
							}
							else {
								(*it).getClients().push_back(client);
								FD_SET(client.getFd(), &writefds);
								client.getmesagesFromServer().push_back(JOIN_RESPONSE(client.getNick(), client._ip , roomName));
								if (!(*it).getTopic().empty()){
									FD_SET(client.getFd(), &writefds);
									client.getmesagesFromServer().push_back(RPL_TOPIC(client.getNick(), client._ip , roomName, (*it).getTopic()));
								}
							}
						}
						else {
							(*it).getClients().push_back(client);
							FD_SET(client.getFd(), &writefds);
							client.getmesagesFromServer().push_back(JOIN_RESPONSE(client.getNick(), client._ip , roomName));
							if (!(*it).getTopic().empty()){
								FD_SET(client.getFd(), &writefds);
								client.getmesagesFromServer().push_back(RPL_TOPIC(client.getNick(), client._ip , roomName, (*it).getTopic()));
							}
						}
					} else {
						FD_SET(client.getFd(), &writefds);
						client.getmesagesFromServer().push_back("You already in the chanel\n");
					}
					break;
				}
			}
		} else {
			std::cout << "hay amq2" << std::endl;
			Room room;
			room.setName(roomName);
			room.setOperator(&client);
			room.addClient(client);
			channels.push_back(room);
			Utils::instaWrite(client.getFd(), JOIN_RESPONSE(client.getNick(), client._ip , roomName));
		}
	}
	else {
		std::cout << "hay amq3" << std::endl;
		FD_SET(client.getFd(), &writefds);
		client.getmesagesFromServer().push_back("JOIN :Not enough parameters");
	}
	responseAllClientResponseToGui(client, getRoom(roomName));
}

/*
int    Server::Join(std::string &s, Client& cli) {
    std::stringstream ss(s);
    std::string chaName, key;
    std::string msg;
    if (!s.empty()) {
        if(cli.nick.empty() || cli.user.empty()) {
            cli.messageBox.push_back("You cannot joined chanel cause you don't have a nickname or username\n");
            FD_SET(cli.cliFd, &this->writeFds);
            return 0;
        }
        ss >> chaName;
        ss >> key;
        std::cout << PURPLE << "s: " << s << " chaName: " << chaName << " name: " << cli.nick << RESET << std::endl;
        if(!Utilities::checkChannel(chaName)) {
            Utilities::writeRpl(cli.cliFd, ERR_NOSUCHCHANNEL(cli.getPrefix(), chaName));
            return 0;
        }
        if (findChanel(chaName, this->chanels)) {
            for (ChanelIterator it = chanels.begin(); it != chanels.end(); ++it) {
                if (chaName == (*it).name) {//if chanel exist try ot join chanel
                    if (!isClientIn((*it), cli.cliFd)) {
                        if ((it->keycode & K_CODE) && (it->keycode & L_CODE))
                        {
                            if (it->users <= (int) it->clients.size()) {
                                Utilities::writeRpl(cli.cliFd, ERR_CHANNELISFULL(cli.nick, chaName));
                            }
                            else if (it->key != key) {
                                Utilities::writeRpl(cli.cliFd, ERR_BADCHANNELKEY(cli.nick, chaName));
                            }
                            else {
                                (*it).clients.push_back(cli);
                                Utilities::writeRpl(cli.cliFd, RPL_JOIN(cli.nick, cli.ipAddr, chaName));
                                if (!(*it).topic.empty())
                                    Utilities::writeRpl(cli.cliFd, RPL_TOPIC(cli.nick, cli.ipAddr, chaName, (*it).topic));
                            }
                        }
                        else if ((it->keycode & K_CODE)) {
                            if (it->key != key) {
                                Utilities::writeRpl(cli.cliFd, ERR_BADCHANNELKEY(cli.nick, chaName));
                            }
                            else {
                                (*it).clients.push_back(cli);
                                Utilities::writeRpl(cli.cliFd, RPL_JOIN(cli.nick, cli.ipAddr, chaName));
                                if (!(*it).topic.empty())
                                    Utilities::writeRpl(cli.cliFd, RPL_TOPIC(cli.nick, cli.ipAddr, chaName, (*it).topic));
                            }
                        }
                        else if ((it->keycode & L_CODE)) {
                            if (it->users <= (int) it->clients.size()) {
                                Utilities::writeRpl(cli.cliFd, ERR_CHANNELISFULL(cli.nick, chaName));
                            }
                            else {
                                (*it).clients.push_back(cli);
                                Utilities::writeRpl(cli.cliFd, RPL_JOIN(cli.nick, cli.ipAddr, chaName));
                                if (!(*it).topic.empty())
                                    Utilities::writeRpl(cli.cliFd, RPL_TOPIC(cli.nick, cli.ipAddr, chaName, (*it).topic));
                            }
                        }
                        else {
                            (*it).clients.push_back(cli);
                            Utilities::writeRpl(cli.cliFd, RPL_JOIN(cli.nick, cli.ipAddr, chaName));
                            if (!(*it).topic.empty())
                                Utilities::writeRpl(cli.cliFd, RPL_TOPIC(cli.nick, cli.ipAddr, chaName, (*it).topic));
                        }

                    }//client is in chanel or not
                    else {
                        cli.messageBox.push_back("You already in the chanel");
                        FD_SET(cli.cliFd, &this->writeFds);
                    }
                    break;
                }
            }
        }
        else {//if chanel does not exist, create one and add the client to the chanel vector.
            Chanel  newChanel(chaName);
            newChanel.op = &cli;
            newChanel.clients.push_back(cli);
            this->chanels.push_back(newChanel); // Chanels cached in server.
            Utilities::writeRpl(cli.cliFd, RPL_JOIN(cli.nick, cli.ipAddr, chaName));
        }
        showRightGui(cli, getChanel(chaName));
    }
    else {//if join command input empty write this message
        cli.messageBox.push_back("you cannot joined chanel cause use command correct JOIN <chanel name>");
        FD_SET(cli.cliFd, &this->writeFds);
    }
    return 0;
}


*/
