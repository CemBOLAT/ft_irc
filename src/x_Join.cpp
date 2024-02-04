#include "../include/Client.hpp"
#include "../include/Executor.hpp"
#include "../include/Exception.hpp"
#include "../include/Client.hpp"
#include "../include/Server.hpp"
#include "../include/Room.hpp"
#include <iostream>
#include <string>
#include <vector>

#define JOIN_RESPONSE(nick, ip, channel) ":" + nick + "!" + nick + "@" + ip + " JOIN " + channel + "\r\n"
#define RPL_TOPIC(nick, ip, channel, topic) ":" + nick + "!" + nick + "@" + ip + " TOPIC " + channel + " :" + topic + "\r\n"

void Server::join(C_VECT_STR_R params, Client &client)
{
	FD_SET(client.getFd(), &writefds);
	if (params.size() < 2)
	{
		client.getmesagesFromServer().push_back("JOIN :Not enough parameters\n");
		return;
	}
	if (params[1][0] != '#')
	{
		client.getmesagesFromServer().push_back("JOIN :Invalid channel name\n");
		return;
	}
	std::string channelName = params[1].substr(1);
	for (size_t i = 0; i < channels.size(); i++)
	{
		if (channels[i].getName() == channelName)
		{
			if (channels[i].isClientInChannel(client.getNick()))
			{
				client.getmesagesFromServer().push_back("JOIN :You are already in this channel\n");
				return;
			}
			channels[i].addClient(&client);
			client.getmesagesFromServer().push_back("JOIN " + channelName + "\n");
			client.getmesagesFromServer().push_back(JOIN_RESPONSE(client.getNick(), client._ip, channelName));
			if (channels[i].getTopic() != "")
				client.getmesagesFromServer().push_back(RPL_TOPIC(client.getNick(), client._ip, channelName, channels[i].getTopic()));
			return;
		}
	}
	Room newRoom;
	newRoom.setName(channelName);
	newRoom.addClient(&client);
	channels.push_back(newRoom);
	client.getmesagesFromServer().push_back("JOIN " + channelName + "\n");
	client.getmesagesFromServer().push_back(JOIN_RESPONSE(client.getNick(), client._ip, channelName));
	if (newRoom.getTopic() != "")
		client.getmesagesFromServer().push_back(RPL_TOPIC(client.getNick(), client._ip, channelName, newRoom.getTopic()));
}
