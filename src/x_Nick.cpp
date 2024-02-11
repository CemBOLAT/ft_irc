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

int isNickExist(C_STR_REF s, const std::vector<Client> &clients, int fd)
{
	for (std::vector<Client>::const_iterator it = clients.begin(); it != clients.end(); ++it)
	{
		if (s == it->getNick() && it->getFd() != fd)
			return 1;
	}
	return 0;
}

void Server::nick(C_STR_REF params, Client &client, fd_set &fd)
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
		// std::cout << RPL_NICK(client.getNick(), client.getUserName(), client._ip, params); // debug
		if (!client.getNick().empty())
			TextEngine::magenta("User " + client.getNick() + " has been changed his nick to " + params , TextEngine::printTime(std::cout)) << std::endl;
		client.setNick(params); // set new nick if not exist
		client.getmesagesFromServer().push_back(RPL_NICK(oldNick, client.getUserName(), client._ip, params));
		FD_SET(client.getFd(), &fd);
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

/*
	oda1:											oda2:
	cemal --> ali (4)								cemal (cemal aliyi bilmiyor) --> ali (4)
	hüseyin (hüseyin cemali bilmiyor) (5)			ali --> cemal (6)
*/
