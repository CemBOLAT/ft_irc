#include "../include/Client.hpp"
#include "../include/Executor.hpp"
#include "../include/Exception.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

namespace Executor
{
	void	user(C_STR_REF params, Client &client, fd_set &fd)
	{
		FD_SET(client.getFd(), &fd);
		if (params.empty())
		{
			client.getmesagesFromServer().push_back("USER :Not enough parameters\n\r");
			return;
		}
		if (client.getIsRegistered())
		{
			client.getmesagesFromServer().push_back("Unauthorized command (already registered)\n\r");
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
			client.getmesagesFromServer().push_back("USER :Not enough parameters\n\r");
			return;
		}
		client.setUserName(username);
		client.setHostName(hostname);
		client.setServerName(servername);
		client.setRealName(realname);
		client.setRegistered(true);
		client.getmesagesFromServer().push_back("Welcome to the Internet Relay Network " + client.getNick() + "!" + client.getUserName() + "@" + client.getRealName() + "\n\r");
	}
}
