#include "../include/Client.hpp"
#include "../include/Executor.hpp"
#include "../include/Exception.hpp"
#include "../include/Utils.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

#define RPL_WHOISUSER(nick, user, ip)				": 311 " + nick + " " + nick + " " + nick + " " + ip + " * :" + user + "\r\n"
#define RPL_WHOISSERVER(source, server)				": 312 " + source + " * :" + server + "\r\n"
#define RPL_ENDOFWHOIS(source, nick)				": 318 " + source + " " + nick + " :End of /WHOIS list" + "\r\n"

void	Server::whois(std::string &s, Client &cli){
	std::vector<std::string> cmd = Utils::ft_split(s, " ");
	std::string who = cmd[0];
	(void)cli;
	for(std::vector<Client>::iterator it = this->clients.begin(); it != this->clients.end(); ++it) {
		if (who == (*it).getNick()){
			Utils::instaWrite(cli.getFd(), RPL_WHOISUSER((*it).getNick(), (*it).getUserName(), (*it)._ip));
			Utils::instaWrite(cli.getFd(), RPL_WHOISSERVER((*it).getNick(), "user"));
			Utils::instaWrite(cli.getFd(), RPL_ENDOFWHOIS((*it).getNick(), (*it).getNick()));
			break;
		}
	}
}


/*

int Server::Whois(std::string &s, Client &cli) {

    std::vector<std::string> cmd = Utilities::tokenCmd(s, 0);
    std::string who = cmd[0];
    (void)cli;
    for(ClientIterator it = this->clients.begin(); it != this->clients.end(); ++it) {
        if (who == (*it).nick) {
            Utilities::writeRpl(cli.cliFd, RPL_WHOISUSER((*it).nick, (*it).user, (*it).ip));
            Utilities::writeRpl(cli.cliFd, RPL_WHOISSERVER((*it).nick, "masterbaiter"));
            Utilities::writeRpl(cli.cliFd, RPL_ENDOFWHOIS((*it).nick, (*it).nick));
            break;
        }
    }
    return 1;
}

*/
