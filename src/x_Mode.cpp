#include "../include/Client.hpp"
#include "../include/Room.hpp"
#include "../include/Executor.hpp"
#include "../include/Exception.hpp"
#include "../include/Utils.hpp"

#include "Define.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>

#define ERR_NEEDMOREPARAMS(source, command)			": 461 " + source + " " + command + " :Not enough parameters" + "\r\n"			  //MODE
#define ERR_NOSUCHCHANNEL(source, channel)			": 403 " + source + " " + channel + " :No such channel" + "\r\n"					//MODE
#define ERR_CHANOPRIVSNEEDED(source, channel)		": 482 " + source + " " + channel + " :You're not the channel operator" + "\r\n"	//MODE
#define RPL_MODE(source, channel, modes, args)		":" + source + " MODE " + channel + " " + modes + " " + args + "\r\n"

void	Server::mode(C_STR_REF input, Client &client){
	std::vector<std::string> params = Utils::ft_split(input, " ");
	if (params.size() > 3){
		Utils::instaWrite(client.getFd(), ERR_NEEDMOREPARAMS(client.getNick(), "MODE"));
		return;
	}
	if (params.size() == 1){
		return;
	}
	VECT_ITER_CHA it = this->channels.begin();
	for (; it != this->channels.end(); ++it){
		if (it->getName() == params[0]){
			break;
		}
	}
	if (it == this->channels.end()){
		Utils::instaWrite(client.getFd(), ERR_NOSUCHCHANNEL(client.getNick(), params[0]));
		return;
	}
	if (it->getOperator()->getNick() != client.getNick()){
		Utils::instaWrite(client.getFd(), ERR_CHANOPRIVSNEEDED(client.getNick(), params[0]));
		return;
	}
	if (params[1] == "+k"){
		if (params.size() == 3){
			it->setKey(params[2]);
			it->setKeycode(it->getKeycode() | KEY_CODE);
			Utils::instaWrite(client.getFd(), RPL_MODE(client.getNick(), it->getName(), "+k", params[2]));
		}
	}
	if (params[1] == "-k"){
		if (params.size() == 2){
			it->setKey("");
			it->setKeycode(it->getKeycode() ^ KEY_CODE);
			Utils::instaWrite(client.getFd(), RPL_MODE(client.getNick(), it->getName(), "-k", ""));
		}
	}
	if (params[1] == "+l"){
		if (params.size() == 3){
			it->setChanelLimit(atoi(params[2].c_str()));
			it->setKeycode(it->getKeycode() | LIMIT_CODE);
			Utils::instaWrite(client.getFd(), RPL_MODE(client.getNick(), it->getName(), "+l", params[2]));
		}
	}
	if (params[1] == "-l"){
		if (params.size() == 2){
			it->setKeycode(it->getKeycode() ^ LIMIT_CODE);
			Utils::instaWrite(client.getFd(), RPL_MODE(client.getNick(), it->getName(), "-l", ""));
		}
	}
	if (params[1] == "+o"){
		if (params.size() == 3){
			op(input, client); // call op function
		}
	}
}
