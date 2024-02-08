#include "../include/Client.hpp"
#include "../include/Executor.hpp"
#include "../include/Utils.hpp"
#include "../include/Exception.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

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
		//if (!client.getNick().empty()){
			client.setRegistered(true);
			Utils::instaWrite(client.getFd(), ": 001 " + client.getNick() + " :Welcome to the Internet Relay Network " + client.getNick() + "!" + client.getUserName() + "@" + client.getRealName() + "\r\n");
		//}
	}
}
