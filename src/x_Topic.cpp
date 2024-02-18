#include "Client.hpp"
#include "Executor.hpp"
#include "Exception.hpp"
#include "Utils.hpp"
#include "Define.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "Define.hpp"

void Server::topic(C_STR_REF command, Client &client)
{
	VECT_STR params = Utils::ft_split(command, " ");
	if (params.size() < 2)
	{
		Utils::instaWrite(client.getFd(), ERR_NEEDMOREPARAMS(client.getNick(), "TOPIC"));
		return;
	}
	std::string channelName = params[0];
	bool channelFound = false;

	for (size_t i = 0; i < this->channels.size(); ++i)
	{
		Room &channel = this->channels[i];
		if (channelName == channel.getName())
		{
			channelFound = true;
			if (client.getFd() != channel.getOperator().getFd()) {
				Utils::instaWrite(client.getFd(), ERR_CHANOPRIVSNEEDED(client.getNick(), channel.getName()));
				return;
			}
			if (params.size() == 1)
			{ // Retrieve topic
				std::string topic = channel.getTopic();
				if (!topic.empty())
				{
					std::string response = RPL_TOPIC(client.getNick(), channel.getName(), topic);
					Utils::instaWrite(client.getFd(), response);
				}
				else
				{
					Utils::instaWrite(client.getFd(), RPL_NOTOPIC(client.getNick(), channel.getName()));
				}
			}
			else
			{ // Set topic
				if (params[1][0] == ':')
				{
					params[1] = params[1].substr(1);
				}
				std::string oldTopic = channel.getTopic(); // Save the old topic
				channel.setTopic(params[1]);
				std::string response = RPL_TOPICSET(client.getNick(), channel.getName(), params[1], Utils::getTime()); // saati bastırıyor
				Utils::instaWriteAll(channel.getClients(), response);
				if (oldTopic != params[1])
				{ // bu kısım sayesinde anlık olarak konunun değişimini herkes görüyor.
					std::string topicChangeNotification = RPL_TOPIC(client.getNick(), channel.getName(), params[1]);
					Utils::instaWriteAll(channel.getClients(), topicChangeNotification);
				}
			}
			break;
		}
	}

	if (!channelFound)
	{
		Utils::instaWrite(client.getFd(), ERR_NOSUCHCHANNEL(client.getNick(), channelName));
	}
}
