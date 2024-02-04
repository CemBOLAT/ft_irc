#include "../include/Client.hpp"
#include "../include/Executor.hpp"
#include "../include/Exception.hpp"
#include "../include/Client.hpp"
#include "../include/Server.hpp"
#include "../include/Room.hpp"
#include <iostream>
#include <string>
#include <vector>

void Server::join(C_VECT_STR_R params, Client &client)
{
	if (params.size() < 1)
	{
		client.getmesagesFromServer().push_back("JOIN :Not enough parameters\n");
		return;
	}
	if (params[1][0] != '#')
	{
		client.getmesagesFromServer().push_back("JOIN :Invalid channel name\n");
		return;
	}
	for (size_t i = 0; i < channels.size(); i++)
	{
		if (channels[i].getName() == params[0])
		{
			channels[i].addClient(client);
			client.getmesagesFromServer().push_back("JOIN :" + channels[i].getName() + "\n");
			return;
		}
	}
	Room newRoom;
	newRoom.setName(params[1].substr(1));
	newRoom.addClient(client);
	channels.push_back(newRoom);
	newRoom.setOwner(&client);
	client.getmesagesFromServer().push_back("JOIN : " + newRoom.getName() + "\n");
}
