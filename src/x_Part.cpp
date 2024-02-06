#include "../include/Client.hpp"
#include "../include/Executor.hpp"
#include "../include/Exception.hpp"
#include "../include/Client.hpp"
#include "../include/Server.hpp"
#include "../include/Room.hpp"
#include <iostream>
#include <string>
#include <vector>


void Server::part(C_STR_REF params, Client &client)
{
	if (params.empty())
	{
		client.getmesagesFromServer().push_back("PART :Not enough parameters\n");
		return;
	}
	if (params[0] != '#')
	{
		client.getmesagesFromServer().push_back("PART :Invalid channel name\n");
		return;
	}
	string channelName = params.substr(1, params.size() - 1);
	for (size_t i = 0; i < channels.size(); i++)
	{
		if (channels[i].getName() == channelName)
		{
			for (size_t j = 0; j < channels[i].getClients().size(); j++)
			{
				if (channels[i].getClients()[j].getNick() == client.getNick())
				{
					channels[i].getClients().erase(channels[i].getClients().begin() + j);
					client.getmesagesFromServer().push_back("PART :" + channels[i].getName() + "\n");
					return;
				}
			}
		}
	}
	client.getmesagesFromServer().push_back("PART :You are not in this channel\n");
}
