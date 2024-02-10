#include "../include/Client.hpp"
#include "../include/Executor.hpp"
#include "../include/Server.hpp"
#include "../include/TextEngine.hpp"
#include "../include/Exception.hpp"
#include "../include/Utils.hpp"
#include <iostream>
#include <string>
#include <vector>

#define RPL_NICK(nick, user, ip, newnick) ":" + nick + "!" + user + "@" + ip + " NICK :" + newnick + "\r\n"
#define ERR_NICKNAMEEMPTY(source) ": 433 " + source + " " + source + " :Nickname cannot empty" + "\r\n"
#define ERR_NICKNAMEINUSE(source) ": 433 " + source + " " + source + " :Nickname is already in use" + "\r\n"

int isNickExist(const std::string &s, const std::vector<Client> &clients, int fd)
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
	else
	{
		// std::cout << RPL_NICK(client.getNick(), client.getUserName(), client._ip, params); // debug
		client.getmesagesFromServer().push_back(RPL_NICK(client.getNick(), client.getUserName(), client._ip, params));
		FD_SET(client.getFd(), &fd);
		for (std::vector<Room>::iterator it = channels.begin(); it != channels.end(); ++it)
		{
			for (std::vector<Client>::iterator cit = it->getClients().begin(); cit != it->getClients().end(); ++cit)
			{
				if (client.getNick() == cit->getNick())
				{
					cit->setNick(params);
					responseAllClientResponseToGui(*cit, *it); // response to all client in room
					break;
				}
			}
		}
		if (!client.getNick().empty())
			TextEngine::magenta("User " + client.getNick() + " has been changed his nick to " + params , TextEngine::printTime(std::cout)) << std::endl;
		client.setNick(params); // set new nick if not exist
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
