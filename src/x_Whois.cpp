#include "Client.hpp"
#include "Server.hpp"

#include "Exception.hpp"
#include "Utils.hpp"
#include "Define.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

/*
4.5.2 Whois query

      Command: WHOIS
   Parameters: [<server>] <nickmask>[,<nickmask>[,...]]

   This message is used to query information about particular user.  The
   server will answer this message with several numeric messages
   indicating different statuses of each user which matches the nickmask
   (if you are entitled to see them).  If no wildcard is present in the
   <nickmask>, any information about that nick which you are allowed to
   see is presented.  A comma (',') separated list of nicknames may be
   given.

   The latter version sends the query to a specific server.  It is
   useful if you want to know how long the user in question has been
   idle as only local server (ie. the server the user is directly
   connected to) knows that information, while everything else is
   globally known.

   Numeric Replies:

           ERR_NOSUCHSERVER                ERR_NONICKNAMEGIVEN
           RPL_WHOISUSER                   RPL_WHOISCHANNELS
           RPL_WHOISCHANNELS               RPL_WHOISSERVER
           RPL_AWAY                        RPL_WHOISOPERATOR
           RPL_WHOISIDLE                   ERR_NOSUCHNICK
           RPL_ENDOFWHOIS
   Examples:

   WHOIS wiz                       ; return available user information
                                   about nick WiZ

   WHOIS eff.org trillian          ; ask server eff.org for user
                                   information about trillian

*/

void	Server::whois(std::string &s, Client &cli){
	//VECT_STR cmd = Utils::ft_split(s, " ");
	//std::string who = cmd[0];
	//for(VECT_ITER_CLI it = this->clients.begin(); it != this->clients.end(); ++it) {
	//	if (who == (*it).getNick()){
	//		Utils::instaWrite(cli.getFd(), RPL_WHOISUSER((*it).getNick(), (*it).getUserName(), (*it)._ip));
	//		Utils::instaWrite(cli.getFd(), RPL_WHOISSERVER((*it).getNick(), "user"));
	//		Utils::instaWrite(cli.getFd(), RPL_ENDOFWHOIS((*it).getNick(), (*it).getNick()));
	//		break;
	//	}
	//}
}
