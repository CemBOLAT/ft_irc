#include "Client.hpp"
#include "Executor.hpp"
#include "Exception.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include "Room.hpp"
#include "Utils.hpp"
#include "TextEngine.hpp"
#include "Define.hpp"

#include <iostream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

void Server::join(C_STR_REF params, Client &client)
{
	stringstream ss(params);
	string roomName, key, message;
	if (!params.empty()){
		ss >> roomName;
		ss >> key;
		if (roomName.empty()){
			client.getmesagesFromServer().push_back("JOIN :Not enough parameters\n\r");
			FD_SET(client.getFd(), &writefds);
			return;
		}
		if (roomName[0] != '#'){
			roomName = "#" + roomName;
		}
		if (isRoom(roomName)){
			for (VECT_ITER_CHA it = channels.begin(); it != channels.end(); it++){
				if (roomName == (*it).getName()){
					if (!it->isClientInChannel(client.getFd())){
						if ((it->getKeycode() & KEY_CODE) && (it->getKeycode() & LIMIT_CODE))
						{
							if (it->getChanelLimit() <= (int) it->getClients().size()) {
								Utils::instaWrite(client.getFd(), ERR_CHANNELISFULL(client.getNick(), roomName));
							}
							else if (key == "" && it->getKey() != key) {
								Utils::instaWrite(client.getFd(), ERR_BADCHANNELKEY(client.getNick(), roomName));
							}
							else {
								(*it).getClients().push_back(client);
								Utils::instaWrite(client.getFd(), JOIN_RESPONSE(client.getNick(), client._ip , roomName));
								if (!(*it).getTopic().empty()){
									Utils::instaWrite(client.getFd(), RPL_TOPIC_JOIN(client.getNick(), client._ip , roomName, (*it).getTopic()));
								}
							}
						}
						else if ((it->getKeycode() & KEY_CODE)) {
							if (key == "" && it->getKey() != key) {
								Utils::instaWrite(client.getFd(), ERR_BADCHANNELKEY(client.getNick(), roomName));
							}
							else {
								(*it).getClients().push_back(client);
								Utils::instaWrite(client.getFd(), JOIN_RESPONSE(client.getNick(), client._ip , roomName));
								if (!(*it).getTopic().empty()){
									Utils::instaWrite(client.getFd(), RPL_TOPIC_JOIN(client.getNick(), client._ip , roomName, (*it).getTopic()));
								}
							}
						}
						else if ((it->getKeycode() & LIMIT_CODE)) {
							if (it->getChanelLimit() <= (int) it->getClients().size()) {
								Utils::instaWrite(client.getFd(), ERR_CHANNELISFULL(client.getNick(), roomName));
							}
							else {
								(*it).getClients().push_back(client);
								Utils::instaWrite(client.getFd(), JOIN_RESPONSE(client.getNick(), client._ip , roomName));
								if (!(*it).getTopic().empty()){
									Utils::instaWrite(client.getFd(), RPL_TOPIC_JOIN(client.getNick(), client._ip , roomName, (*it).getTopic()));
								}
							}
						}
						else {
							(*it).getClients().push_back(client);
							Utils::instaWrite(client.getFd(), JOIN_RESPONSE(client.getNick(), client._ip , roomName));
							if (!(*it).getTopic().empty()){
								Utils::instaWrite(client.getFd(), RPL_TOPIC_JOIN(client.getNick(), client._ip , roomName, (*it).getTopic()));
							}
						}
					} else {
						client.getmesagesFromServer().push_back("You already in the chanel\n\r");
						FD_SET(client.getFd(), &writefds);
					}
					break;
				}
			}
		} else {
			TextEngine::cyan("Room " + roomName + " has been created by " + client.getNick(), TextEngine::printTime(std::cout)) << std::endl;
			Room room;
			room.setName(roomName);
			room.setOperator(&client);
			room.addClient(client);
			channels.push_back(room);
			Utils::instaWrite(client.getFd(), JOIN_RESPONSE(client.getNick(), client._ip , roomName));
		}
		responseAllClientResponseToGui(client, getRoom(roomName));
	} else {
		client.getmesagesFromServer().push_back("JOIN :Not enough parameters");
		FD_SET(client.getFd(), &writefds);
	}
}
