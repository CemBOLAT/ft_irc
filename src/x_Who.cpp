#include "Client.hpp"
#include "Executor.hpp"
#include "Exception.hpp"
#include "Utils.hpp"
#include "Define.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

#define RPL_WHOREPLY(nick, channel, user, host, server, nickName, hops, realName) (std::string(":") + "IRC" + " 352 " + nick + " " + channel + " " + user + " " + host + " " + server + " " + nickName + " " + hops + " " + realName + " H :0 " + realName + "\r\n")

/*
4.5.1 Who query

      Command: WHO
   Parameters: [<name> [<o>]]

   The WHO message is used by a client to generate a query which returns
   a list of information which 'matches' the <name> parameter given by
   the client.  In the absence of the <name> parameter, all visible
   (users who aren't invisible (user mode +i) and who don't have a
   common channel with the requesting client) are listed.  The same
   result can be achieved by using a <name> of "0" or any wildcard which



Oikarinen & Reed                                               [Page 33]

RFC 1459              Internet Relay Chat Protocol              May 1993


   will end up matching every entry possible.

   The <name> passed to WHO is matched against users' host, server, real
   name and nickname if the channel <name> cannot be found.

   If the "o" parameter is passed only operators are returned according
   to the name mask supplied.

   Numeric Replies:

           ERR_NOSUCHSERVER
           RPL_WHOREPLY                    RPL_ENDOFWHO

   Examples:

   WHO *.fi                        ; List all users who match against
                                   "*.fi".

   WHO jto* o                      ; List all users with a match against
                                   "jto*" if they are an operator.

*/

void Server::who(C_STR_REF command, Client &client) {
	VECT_STR params = Utils::ft_split(command, " ");
	if (params.empty()) {
		Utils::instaWrite(client.getFd(), ERR_NEEDMOREPARAMS(client.getNick(), "WHO"));
		return;
	}
	bool channelFound = false;
	for (VECT_ITER_CHA it = channels.begin(); it != channels.end(); ++it) {
		if (it->getName() == params[0]) {
			for (VECT_ITER_CLI cit = it->getClients().begin(); cit != it->getClients().end(); ++cit) {
				std::string operatorSymbol = it->isOperator(*cit) ? "@" : "";
				Utils::instaWrite(client.getFd(), RPL_WHOREPLY(client.getNick(), it->getName(), cit->getUserName(), cit->getHostName(), cit->getServerName(), cit->getNick(), "0", cit->getRealName()));
				channelFound = true;
			}
			Utils::instaWrite(client.getFd(), ":IRC 315 " + client.getNick() + " " + it->getName() + " :End of /WHO list.\r\n");
			return;
		}
	}
	if (!channelFound) {
		Utils::instaWrite(client.getFd(), ERR_NOSUCHCHANNEL(client.getNick(), params[0]));
	}
}