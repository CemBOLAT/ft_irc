#include "../include/Client.hpp"
#include "../include/Executor.hpp"
#include "../include/Exception.hpp"
#include "../include/Client.hpp"
#include "../include/Server.hpp"
#include "../include/Room.hpp"
#include "../include/Utils.hpp"
#include <iostream>
#include <string>
#include <vector>

#define ERR_NOTONCHANNEL(s, channel) ":442 " + s + " " + channel + " :You're not on that channel\r\n"
#define RPL_PART(source, channel)					":" + source + " PART :" + channel + "\r\n"

void Server::part(C_STR_REF params, Client &client)
{
	vector<Room>::iterator it = channels.begin();
	vector<string> param = Utils::ft_split(params, " ");

	for (; it != channels.end(); ++it)
	{
		if (it->getName() == param[0])
		{
			if (it->isClientInChannel(client.getFd()))
			{
				std::cout << client.getNick() << std::endl;
				if (it->getClients().size() == 1) // delete room.
				{
					channels.erase(it);
					Utils::instaWrite(client.getFd(), RPL_PART(client.getUserByHexChat(), param[0]));
				}
				else if (it->isOperator(client)) // if client is operator
				{
					it->removeClient(client.getFd());
					it->setOperator((it->getClients().size() > 0) ? &it->getClients()[0] : NULL);
					Utils::instaWrite(client.getFd(), RPL_PART(client.getUserByHexChat(), param[0]));
					responseAllClientResponseToGui(client, *it);
				}
				else {
					it->removeClient(client.getFd());
					Utils::instaWrite(client.getFd(), RPL_PART(client.getUserByHexChat(), param[0]));
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


/*

int Server::Part(std::string &s, Client &cli) {
    std::stringstream ss(s);
    ss >> s;

    std::string msg;
    ChanelIterator it = this->chanels.begin();
    for (;it != this->chanels.end(); ++it)
    {
        if (it->name == s){ //if channel name is the same as the channel I want to quit.
            for (ClientIterator cit = it->clients.begin(); cit != it->clients.end(); ++cit)
            {
                if (cit->getNick() == cli.getNick()) {
                    if (it->clients.size() == 1) {
                        this->chanels.Utils::erase(it);
                        Utilities::writeRpl(cli.cliFd, RPL_PART(cli.getPrefix(), s));
                        return 0;
                    }
                    else {
                        if(cit->getNick() == it->op->getNick()) {
                            it->op = &it->clients[0];
                        }
                        it->clients.Utils::erase(cit);
                    }
                    showRightGui(cli, getChanel(s));
                    Utilities::writeRpl(cli.cliFd, RPL_PART(cli.getPrefix(), s));
                return 1;
                }

            }
        }
    }
    return 0;
}


*/
