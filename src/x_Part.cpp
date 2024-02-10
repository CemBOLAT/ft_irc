#include "../include/Client.hpp"
#include "../include/Executor.hpp"
#include "../include/Exception.hpp"
#include "../include/Client.hpp"
#include "../include/Server.hpp"
#include "../include/Room.hpp"
#include "../include/Utils.hpp"
#include "../include/TextEngine.hpp"
#include <iostream>
#include <string>
#include <vector>

#define ERR_NOTONCHANNEL(s, channel) ":442 " + s + " " + channel + " :You're not on that channel\r\n"
#define RPL_PART(source, channel)					":" + source + " PART :" + channel + "\r\n"
#define RPL_PART_REASON(source, channel, reason)	":" + source + " PART " + channel + " :" + reason + "\r\n"

void Server::part(C_STR_REF params, Client &client)
{
	vector<Room>::iterator it = channels.begin();
	vector<string> param = Utils::ft_split(params, " ");
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
					it->setOperator((it->getClients().size() > 0) ? &it->getClients()[0] : NULL);
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
