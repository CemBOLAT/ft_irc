#include "Client.hpp"
#include "Room.hpp"
#include "Executor.hpp"
#include "Exception.hpp"
#include "Utils.hpp"
#include "Define.hpp"

#include "Define.hpp"
#include <iostream>
#include <string>

#include <vector>
#include <cstdlib>

// /mode #chanel (operator) (user)
void	Server::mode(C_STR_REF input, Client &client){
	VECT_STR params = Utils::ft_split(input, " ");
	if (params.size() < 3){
		Utils::instaWrite(client.getFd(), ERR_NEEDMOREPARAMS(client.getNick(), "MODE"));
		return;
	}
	if (params.size() == 1){
		return;
	}
	// odayı buluyom
	VECT_ITER_CHA it = this->channels.begin();
	for (; it != this->channels.end(); ++it){
		if (it->getName() == params[0]){
			break;
		}
	}
	if (it == this->channels.end()){ // oda yoksa
		Utils::instaWrite(client.getFd(), ERR_NOSUCHCHANNEL(client.getNick(), params[0]));
		return;
	}
	// operatör mü
	if (it->getOperator().getNick() != client.getNick()){
		Utils::instaWrite(client.getFd(), ERR_CHANOPRIVSNEEDED(client.getNick(), params[0]));
		return;
	}
	if (params[1] == "+k"){ // sets a key to the channel
		if (params.size() == 3){
			it->setKey(params[2]);
			it->setKeycode(it->getKeycode() | KEY_CODE); // yada (0 | 1) == 1
			Utils::instaWrite(client.getFd(), RPL_MODE(client.getNick(), it->getName(), "+k", params[2]));
		}
	}
	if (params[1] == "-k"){
		if (params.size() == 2){
			it->setKey("");
			it->setKeycode(it->getKeycode() ^ KEY_CODE); // (1i 0 0ı 1 yapar)
			Utils::instaWrite(client.getFd(), RPL_MODE(client.getNick(), it->getName(), "-k", ""));
		}
	}
	if (params[1] == "+l"){ // limits the number of clients in the channel (working properly)
		if (params.size() == 3){
			if (it->getClients().size() < static_cast<size_t>(atoi(params[2].c_str()))){
				Utils::instaWrite(client.getFd(), ERR_NEEDMOREPARAMS(client.getNick(), "MODE"));
				return;
			}
			it->setChanelLimit(atoi(params[2].c_str()));
			it->setKeycode(it->getKeycode() | LIMIT_CODE); 
			Utils::instaWrite(client.getFd(), RPL_MODE(client.getNick(), it->getName(), "+l", params[2]));
		}
	}
	if (params[1] == "-l"){ // removes the limit of clients in the channel (working properly)
		if (params.size() == 2){
			it->setKeycode(it->getKeycode() ^ LIMIT_CODE);
			Utils::instaWrite(client.getFd(), RPL_MODE(client.getNick(), it->getName(), "-l", ""));
		}
	}
	if (params[1] == "+o"){ // gives operator status to a client (working properly)
		if (params.size() == 3){
			std::string joined = Utils::ft_join(params, " ", 2);
			op(params[0] +  " " + joined, client);
		}
	}
}
