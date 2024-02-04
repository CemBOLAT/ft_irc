#include "../include/Client.hpp"
#include "../include/Room.hpp"
#include "../include/Executor.hpp"
#include "../include/Exception.hpp"
#include "Define.hpp"
#include <iostream>
#include <string>
#include <vector>

#define ERR_CHANOPRIVSNEEDED(source, channel) ": 482 " + source + " " + channel + " :You're not the channel operator" + "\r\n"

namespace
{
	int getClientPosInRoom(Room &room, const Client &client)
	{
		for (size_t i = 0; i < room.getClients().size(); i++)
		{
			if (client.getNick() == room.getClients()[i]->getNick())
				return i;
		}
		return -1;
	}
}

void Server::op(C_VECT_STR_R params, Client &client)
{
	Room room = getRoom(params[1]);
	if (client.getNick() == room.getOperator()->getNick())
	{
		vector<Client *>::iterator it = room.getClients().begin();
		for (; it != room.getClients().end(); ++it)
		{
			if ((*it)->getNick() == params[2])
				break;
		}
		if (it == room.getClients().end())
			return;
		Client *newOp = room.getClient(params[2]);
		Client *oldOp = room.getClient(client.getNick());

		for (vector<Room>::iterator it = channels.begin(); it != channels.end(); it++)
		{
			if (params[1] == it->getName() && getClientPosInRoom(*it, *oldOp) != -1 && getClientPosInRoom(*it, *newOp) != -1)
			{
				Client *tmp = it->getClients()[getClientPosInRoom(*it, *oldOp)];
				it->getClients()[getClientPosInRoom(*it, *oldOp)] = it->getClients()[getClientPosInRoom(*it, *newOp)];
				it->getClients()[getClientPosInRoom(*it, *newOp)] = tmp;
				it->setOperator(it->getClients()[getClientPosInRoom(*it, *newOp)]);
			}
		}
	}
	else
	{
		FD_SET(client.getFd(), &writefds);
		client.getmesagesFromServer().push_back(ERR_CHANOPRIVSNEEDED(client.getUserByHexChat(), room.getName()));
	}
}
