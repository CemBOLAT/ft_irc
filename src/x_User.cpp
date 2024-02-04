#include "../include/Client.hpp"
#include "../include/Executor.hpp"
#include "../include/Exception.hpp"
#include <iostream>
#include <string>
#include <vector>

namespace Executor
{
	void	user(C_VECT_STR_R params, Client &client, fd_set &fd)
	{
		FD_SET(client.getFd(), &fd);
		if (params.size() < 4)
		{
			client.getmesagesFromServer().push_back("USER :Not enough parameters\n");
			return;
		}
		if (client.getIsRegistered())
		{
			client.getmesagesFromServer().push_back("Unauthorized command (already registered)\n");
			return;
		}
		if (params[1] == params[2])
		{
			client.getmesagesFromServer().push_back("Nick name and second choice cannot be the same\n");
			return;
		}
		client.setUserName(params[1]);
		client.setHostName(params[2]);
		client.setServerName(params[3]);
		std::string realName;
		for (size_t i = 4; i < params.size(); i++)
		{
			realName += params[i];
			if (i + 1 < params.size())
				realName += " ";
		}
		client.setRealName(realName);
		client.setRegistered(true);
		client.getmesagesFromServer().push_back("Welcome to the Internet Relay Network " + client.getNick() + "!" + client.getUserName() + "@" + client.getRealName() + "\n");
	}
}
