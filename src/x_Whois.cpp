#include "Client.hpp"
#include "Executor.hpp"
#include "Exception.hpp"
#include "Utils.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "Define.hpp"

void	Server::whois(std::string &s, Client &cli){
	std::vector<std::string> cmd = Utils::ft_split(s, " ");
	std::string who = cmd[0];
	for(VECT_ITER_CLI it = this->clients.begin(); it != this->clients.end(); ++it) {
		if (who == (*it).getNick()){
			Utils::instaWrite(cli.getFd(), RPL_WHOISUSER((*it).getNick(), (*it).getUserName(), (*it)._ip));
			Utils::instaWrite(cli.getFd(), RPL_WHOISSERVER((*it).getNick(), "user"));
			Utils::instaWrite(cli.getFd(), RPL_ENDOFWHOIS((*it).getNick(), (*it).getNick()));
			break;
		}
	}
}
