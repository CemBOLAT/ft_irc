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
#include <sstream>
#include <vector>

using namespace std;
#define KICK_RESPONSE(nick, ip, channel, message) ":" + nick + "!" + nick + "@" + ip + " KICK " + channel + " " + message + "\r\n"
#define ERR_NOSUCHCHANNEL(source, channel) ": 403 " + source + " " + channel + " :No such channel" + "\r\n"				   // KICK
#define ERR_NOSUCHNICK(source, nick) ": 401 " + source + " " + nick + " :No such nick/channel" + "\r\n"					   // KICK
#define ERR_NOTONCHANNEL(source, channel) ": 442 " + source + " " + channel + " :You're not on that channel" + "\r\n"	   // KICK
#define ERR_CHANOPRIVSNEEDED(source, channel) ": 482 " + source + " " + channel + " :You're not channel operator" + "\r\n" // KICK
#define ERR_NEEDMOREPARAMS(source, command) ": 461 " + source + " " + command + " :Not enough parameters" + "\r\n"		   // KICK
#define ERR_OWNKICK(source, channel) ": 443 " + source + " " + channel + " :You can't kick yourself" + "\r\n"			   // KICK

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
	for (vector<Client>::iterator it = room.getClients().begin(); it != room.getClients().end(); it++)
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
