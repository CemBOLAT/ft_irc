#include "Client.hpp"
#include "Room.hpp"
#include "Executor.hpp"
#include "Exception.hpp"
#include "Define.hpp"
#include "Utils.hpp"
#include <iostream>
#include <string>
#include <vector>
#include "Define.hpp"

/*
4.4.2 Notice

      Command: NOTICE
   Parameters: <nickname> <text>

   The NOTICE message is used similarly to PRIVMSG.  The difference
   between NOTICE and PRIVMSG is that automatic replies must never be
   sent in response to a NOTICE message.  This rule applies to servers
   too - they must not send any error reply back to the client on
   receipt of a notice.  The object of this rule is to avoid loops
   between a client automatically sending something in response to
   something it received.  This is typically used by automatons (clients
   with either an AI or other interactive program controlling their
   actions) which are always seen to be replying lest they end up in a
   loop with another automaton.

   See PRIVMSG for more details on replies and examples.


*/
void	Server::notice(std::string	&input, Client &fd){
	VECT_STR params = Utils::ft_split(input, " ");
	if (params.size() < 2){
		Utils::instaWrite(fd.getFd(), ERR_NEEDMOREPARAMS(fd.getNick(), "NOTICE"));
		return;
	}
	for (VECT_ITER_CLI it = this->getClients().begin(); it != this->getClients().end(); it++){
		if (params[0] == (*it).getUserName() || params[0] == (*it).getNick()){
			std::string message = Utils::ft_join(params, " ", 1);
			if (params[1][0] == ':')
				message = message.substr(1, message.length());
			(*it).getmesagesFromServer().push_back(RPL_NOTICE(fd.getUserByHexChat(), params[0], message));
			FD_SET((*it).getFd(), &this->writefds);
		}
		if (params[0][0] == '#' && isClientInRoom(*it, params[0])){
			std::string message = Utils::ft_join(params, " ", 1);
			if (params[1][0] == ':')
				message = message.substr(1, message.length());
			(*it).getmesagesFromServer().push_back(RPL_NOTICE(fd.getUserByHexChat(), params[0], message));
			FD_SET((*it).getFd(), &this->writefds);
		}
	}
}
