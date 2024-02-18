#include "Client.hpp"
#include "Executor.hpp"
#include "Utils.hpp"
#include "Exception.hpp"
#include "TextEngine.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "Define.hpp"

/*
   Parameters: <username> <hostname> <servername> <realname>
   1.  ERR_ALREADYREGISTRED
   2.  ERR_NEEDMOREPARAMS
   since only after both USER and NICK have been
   received from a client does a user become registered.
   
   It must be noted that realname parameter must be the last parameter,
   because it may contain space characters and must be prefixed with a
   colon (':') to make sure this is recognised as such.
*/

namespace Executor
{
	void	user(C_STR_REF params, Client &client)
	{
		if (params.empty())
		{
			Utils::instaWrite(client.getFd(), ": 461 " + client.getUserByHexChat() + " USER :Not enough parameters\r\n");
			return;
		}
		if (client.getIsRegistered())
		{
			Utils::instaWrite(client.getFd(), ": 462 " + client.getUserByHexChat() + " USER :You may not reregister\r\n");
			return;
		}
		std::stringstream ss(params);
		std::string username, hostname, servername, realname;
		ss >> username;
		ss >> hostname;
		ss >> servername;
		ss >> realname;
		if (username.empty() || hostname.empty() || servername.empty() || realname.empty())
		{
			Utils::instaWrite(client.getFd(), ": 461 " + client.getUserByHexChat() + " USER :Not enough parameters\r\n");
			return;
		}
		if (username == hostname)
		{
			Utils::instaWrite(client.getFd(), ": 462 " + client.getUserByHexChat() + " USER :Username and hostname cannot be the same\r\n");
			return;
		}
		client.setUserName(username);
		client.setHostName(hostname);
		client.setServerName(servername);
		client.setRealName(realname);
		if (!client.getNick().empty()){
			client.setRegistered(true);
			Utils::instaWrite(client.getFd(), ": 001 " + client.getNick() + " :Welcome to the Internet Relay Network " + client.getNick() + "!" + client.getUserName() + "@" + client.getRealName() + "\r\n");
			TextEngine::magenta("User " + client.getNick() + " has been registered", TextEngine::printTime(std::cout)) << std::endl;
		}
	}
}
