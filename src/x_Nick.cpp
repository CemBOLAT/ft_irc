#include "../include/Client.hpp"
#include "../include/Executor.hpp"
#include "../include/Exception.hpp"
#include <iostream>
#include <string>
#include <vector>

#define RPL_NICK(nick, user, ip, newnick) ":" + nick + "!" + user + "@" + ip + " NICK :" + newnick + "\r\n"
#define ERR_NICKNAMEEMPTY(source)				    ": 433 " + source + " " + source  + " :Nickname cannot empty" + "\r\n"

namespace Executor
{
	void nick(C_VECT_STR_R params, Client &client, fd_set &fd)
	{
		FD_SET(client.getFd(), &fd);
        if (params[1].empty()){
			client.getmesagesFromServer().push_back(ERR_NICKNAMEEMPTY(client.getUserByHexChat()));
		}
		else{
			client.getmesagesFromServer().push_back(RPL_NICK(client.getNick(), client.getUserName(), client._ip, params[1]));
			client.setNick(params[1]);
		}
	}
}
