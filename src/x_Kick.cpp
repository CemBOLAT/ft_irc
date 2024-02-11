#include "Client.hpp"
#include "Executor.hpp"
#include "Exception.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include "Room.hpp"
#include "Utils.hpp"
#include "TextEngine.hpp"
#include "Define.hpp"

#include <iostream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

void Server::kick(std::string &input, Client &fd)
{
	vector<string> splited = Utils::ft_split(input, " ");
	string nick, channel, message;
	if (splited.size() < 3)
	{
		Utils::instaWrite(fd.getFd(), ERR_NEEDMOREPARAMS(fd.getNick(), "KICK"));
		return;
	}
	channel = splited[0];
	nick = splited[1];
	message = Utils::ft_join(splited, " ", 2);
	if (channel[0] != '#')
	{
		channel = "#" + channel;
	}
	if (!isRoom(channel))
	{
		Utils::instaWrite(fd.getFd(), ERR_NOSUCHCHANNEL(fd.getNick(), channel));
		return;
	}
	Room room = getRoom(channel);
	if (!room.isClientInChannel(fd.getFd()))
	{
		Utils::instaWrite(fd.getFd(), ERR_NOTONCHANNEL(fd.getNick(), channel));
		return;
	}
	if (!room.isClientInChannel(nick))
	{
		Utils::instaWrite(fd.getFd(), ERR_NOSUCHNICK(fd.getNick(), nick));
		return;
	}
	if (!room.isOperator(fd))
	{
		Utils::instaWrite(fd.getFd(), ERR_CHANOPRIVSNEEDED(fd.getNick(), channel));
		return;
	}
	if (nick == fd.getNick())
	{
		Utils::instaWrite(fd.getFd(), ERR_OWNKICK(fd.getNick(), channel));
		return;
	}
	for (VECT_ITER_CLI it = room.getClients().begin(); it != room.getClients().end(); it++)
	{
		if (it->getNick() == nick)
		{
			FD_SET(it->getFd(), &writefds);
			it->getmesagesFromServer().push_back(KICK_RESPONSE(fd.getNick(), fd._ip, channel, message));
			room.removeClient(it->getFd());
			responseAllClientResponseToGui(fd, room);
			TextEngine::red("KICKED: " + nick + " from " + channel, TextEngine::printTime(cout)) << endl;
			break;
		}
	}
}
