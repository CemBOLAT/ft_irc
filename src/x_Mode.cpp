#include "../include/Client.hpp"
#include "../include/Room.hpp"
#include "../include/Executor.hpp"
#include "../include/Exception.hpp"
#include "Define.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>

#define ERR_NEEDMOREPARAMS(source, command)			": 461 " + source + " " + command + " :Not enough parameters" + "\r\n"              //MODE
#define ERR_NOSUCHCHANNEL(source, channel)			": 403 " + source + " " + channel + " :No such channel" + "\r\n"                    //MODE
#define ERR_CHANOPRIVSNEEDED(source, channel)		": 482 " + source + " " + channel + " :You're not the channel operator" + "\r\n"    //MODE
#define RPL_MODE(source, channel, modes, args)		":" + source + " MODE " + channel + " " + modes + " " + args + "\r\n"

void	Server::mode(C_VECT_STR_R params, Client &client){
	if (params.size() > 4){
		FD_SET(client.getFd(), &writefds);
		client.getmesagesFromServer().push_back(ERR_NEEDMOREPARAMS(client.getNick(), "MODE"));
		return;
	}
	if (params.size() == 2){
		return;
	}
	VECT_ITER_CHA it = this->channels.begin();
	for (; it != this->channels.end(); ++it){
		if (it->getName() == params[1]){
			break;
		}
	}
	if (it == this->channels.end()){
		FD_SET(client.getFd(), &writefds);
		client.getmesagesFromServer().push_back(ERR_NOSUCHCHANNEL(client.getNick(), params[1]));
		return;
	}
	if (it->getOperator()->getNick() != client.getNick()){
		FD_SET(client.getFd(), &writefds);
		client.getmesagesFromServer().push_back(ERR_CHANOPRIVSNEEDED(client.getNick(), params[1]));
		return;
	}
	if (params[2] == "+k"){
		if (params.size() == 4){
			FD_SET(client.getFd(), &writefds);
			it->setKey(params[3]);
			it->setKeycode(it->getKeycode() | KEY_CODE);
			client.getmesagesFromServer().push_back(RPL_MODE(client.getNick(), it->getName(), "+k", params[3]));
		}
	}
	if (params[2] == "-k"){
		if (params.size() == 3){
			FD_SET(client.getFd(), &writefds);
			it->setKey("");
			it->setKeycode(it->getKeycode() ^ KEY_CODE);
			client.getmesagesFromServer().push_back(RPL_MODE(client.getNick(), it->getName(), "-k", ""));
		}
	}
	if (params[2] == "+l"){
		if (params.size() == 4){
			FD_SET(client.getFd(), &writefds);
			it->setChanelLimit(atoi(params[3].c_str()));
			it->setKeycode(it->getKeycode() | LIMIT_CODE);
			client.getmesagesFromServer().push_back(RPL_MODE(client.getNick(), it->getName(), "+l", params[3]));
		}
	}
	if (params[2] == "-l"){
		if (params.size() == 3){
			FD_SET(client.getFd(), &writefds);
			it->setKeycode(it->getKeycode() ^ LIMIT_CODE);
			client.getmesagesFromServer().push_back(RPL_MODE(client.getNick(), it->getName(), "-l", ""));
		}
	}
	if (params[2] == "+o"){
		if (params.size() == 4){
			op(params, client);
		}
	}
}
