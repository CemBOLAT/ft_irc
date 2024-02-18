#include "Client.hpp"
#include "Executor.hpp"
#include "Exception.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include "Room.hpp"
#include "Utils.hpp"
#include "TextEngine.hpp"
#include <iostream>
#include <string>
#include <vector>
#include "Define.hpp"
#include <stdlib.h>

/*
	4.2.2 Part message

	Command: PART
	Parameters: <channel>{,<channel>}

	The PART message causes the client sending the message to be removed
	from the list of active users for all given channels listed in the
	parameter string.

	Numeric Replies:

	        ERR_NEEDMOREPARAMS              ERR_NOSUCHCHANNEL
	        ERR_NOTONCHANNEL

	Examples:

	PART #twilight_zone             ; leave channel "#twilight_zone"

	PART #oz-ops,&group5            ; leave both channels "&group5" and
                                   "#oz-ops".
*/

void Server::part(C_STR_REF params, Client &client)
{
	VECT_ITER_CHA it = channels.begin();
	VECT_STR param = Utils::ft_split(params, " ");
	if (param[0][0] != '#'){
		param[0] = "#" + param[0];
	}
	for (; it != channels.end(); ++it)
	{
		if (it->getName() == param[0])
		{
			if (it->isClientInChannel(client.getFd()))
			{
				//std::cout << client.getNick() << std::endl;
				if (it->getClients().size() == 1) // delete room.
				{
					TextEngine::magenta("Room " + it->getName() + " has been deleted", TextEngine::printTime(std::cout)) << std::endl;
					channels.erase(it);
					Utils::instaWrite(client.getFd(), RPL_PART(client.getUserByHexChat(), param[0]));
				}
				else if (it->isOperator(client)) // if client is operator
				{
					it->removeClient(client.getFd());
					it->setOperator((it->getClients()[0]));
					string reason = (param.size() > 1) ? Utils::ft_join(param, " ", 1) : "";
					Utils::instaWrite(client.getFd(), RPL_PART(client.getUserByHexChat(), param[0]));
					Utils::instaWrite(client.getFd(), RPL_PART_REASON(client.getUserByHexChat(), param[0], reason));
					responseAllClientResponseToGui(client, *it);
				}
				else {
					it->removeClient(client.getFd());
					string reason = (param.size() > 1) ? Utils::ft_join(param, " ", 1) : "";
					Utils::instaWrite(client.getFd(), RPL_PART(client.getUserByHexChat(), param[0]));
					Utils::instaWrite(client.getFd(), RPL_PART_REASON(client.getUserByHexChat(), param[0], reason));
					responseAllClientResponseToGui(client, *it);
				}
				return ;
			}
			else
			{
				Utils::instaWrite(client.getFd(), ERR_NOTONCHANNEL(client.getNick(), params));
				return ;
			}
		}
	}
}
