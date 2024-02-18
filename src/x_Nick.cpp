#include "Client.hpp"
#include "Executor.hpp"
#include "Server.hpp"
#include "TextEngine.hpp"
#include "Exception.hpp"
#include "Utils.hpp"
#include <iostream>
#include <string>
#include <vector>
#include "Define.hpp"

/*
   Parameters: <nickname> [ <hopcount> ]
   1.  ERR_NONICKNAMEGIVEN	
   2.  ERR_ERRONEUSNICKNAME
   3.  ERR_NICKNAMEINUSE
   4.  ERR_NICKCOLLISION
*/

int isNickExist(C_STR_REF s, C_VECT_CLI_R clients, int fd)
{
	for (VECT_ITER_CONST_CLI it = clients.begin(); it != clients.end(); ++it)
	{
		if (s == it->getNick() && it->getFd() != fd)
			return 1;
	}
	return 0;
}

void Server::nick(C_STR_REF params, Client &client, fd_set &writefdd)
{
	if (params.empty())
	{
		Utils::instaWrite(client.getFd(), ERR_NICKNAMEEMPTY(client.getUserByHexChat()));
	}
	else if (isNickExist(params, getClients(), client.getFd()))
	{
		Utils::instaWrite(client.getFd(), ERR_NICKNAMEINUSE(client.getUserByHexChat()));
	}
	else if (params[0] == '#'){
		Utils::instaWrite(client.getFd(), ERR_ERRONEUSNICKNAME(client.getUserByHexChat()));
	}
	else
	{
		string oldNick = client.getNick();
		if (!client.getNick().empty())
			TextEngine::magenta("User " + client.getNick() + " has been changed his nick to " + params , TextEngine::printTime(std::cout)) << std::endl;
		client.setNick(params); // set new nick if not exist
		client.getmesagesFromServer().push_back(RPL_NICK(oldNick, client.getUserName(), client._ip, params));
		FD_SET(client.getFd(), &writefdd);
		for (VECT_ITER_CHA it = channels.begin(); it != channels.end(); ++it)
		{
			for (VECT_ITER_CLI cit = it->getClients().begin(); cit != it->getClients().end(); ++cit)
			{
				if (oldNick == cit->getNick())
				{
					cit->setNick(params);
					responseAllClientResponseToGui(*cit, *it); // response to all client in room
					break;
				}
			}
		}
		if (client.getIsRegistered() == false && !client.getUserName().empty() && !client.getRealName().empty())
		{
			client.setRegistered(true);
			Utils::instaWrite(client.getFd(), ": 001 " + client.getNick() + " :Welcome to the Internet Relay Network " + client.getNick() + "!" + client.getUserName() + "@" + client.getRealName() + "\r\n");
			TextEngine::magenta("User " + client.getNick() + " has been registered", TextEngine::printTime(std::cout)) << std::endl;
		}
	}
}
