#include "Client.hpp"

#include "Exception.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include "Room.hpp"
#include "Utils.hpp"
#include <iostream>
#include <string>
#include <vector>
#include "Define.hpp"

#include <iostream>
#include <unistd.h>
#include <stdlib.h>

/*


4.4.1 Private messages

      Command: PRIVMSG
   Parameters: <receiver>{,<receiver>} <text to be sent>

   PRIVMSG is used to send private messages between users.  <receiver>
   is the nickname of the receiver of the message.  <receiver> can also
   be a list of names or channels separated with commas.

   The <receiver> parameter may also me a host mask  (#mask)  or  server
   mask  ($mask).   In  both cases the server will only send the PRIVMSG
   to those who have a server or host matching the mask.  The mask  must
   have at  least  1  (one)  "."  in it and no wildcards following the
   last ".".  This requirement exists to prevent people sending messages
   to  "#*"  or "$*",  which  would  broadcast  to  all  users; from
   experience, this is abused more than used responsibly and properly.
   Wildcards are  the  '*' and  '?'   characters.   This  extension  to
   the PRIVMSG command is only available to Operators.

   Numeric Replies:

           ERR_NORECIPIENT                 ERR_NOTEXTTOSEND
           ERR_CANNOTSENDTOCHAN            ERR_NOTOPLEVEL
           ERR_WILDTOPLEVEL                ERR_TOOMANYTARGETS
           ERR_NOSUCHNICK
           RPL_AWAY

   Examples:

:Angel PRIVMSG Wiz :Hello are you receiving this message ?
                                ; Message from Angel to Wiz.

PRIVMSG Angel :yes I'm receiving it !receiving it !'u>(768u+1n) .br ;
                                Message to Angel.

PRIVMSG jto@tolsun.oulu.fi :Hello !
                                ; Message to a client on server



Oikarinen & Reed                                               [Page 32]

RFC 1459              Internet Relay Chat Protocol              May 1993


                                tolsun.oulu.fi with username of "jto".

PRIVMSG $*.fi :Server tolsun.oulu.fi rebooting.
                                ; Message to everyone on a server which
                                has a name matching *.fi.

PRIVMSG #*.edu :NSFNet is undergoing work, expect interruptions
                                ; Message to all users who come from a
                                host which has a name matching *.edu.
*/

int isInRoom(Client &client, Server &server, std::string room) {
	for (VECT_ITER_CHA it = server.getRooms().begin(); it != server.getRooms().end(); it++) {
		if (it->getName() == room) {
			for (VECT_ITER_CLI it2 = it->getClients().begin(); it2 != it->getClients().end(); it2++) {
				if (it2->getFd() == client.getFd())
					return 1;
			}
		}
	}
	return 0;
}


void Server::privmsg(C_STR_REF input, Client &client) {
//VECT_STR params = Utils::ft_split(input, " ");
//f (params.size() < 2) {
//	Utils::instaWrite(client.getFd(), ERR_NEEDMOREPARAMS(client.getNick(), "PRIVMSG"));
//	return;
//}
//for (VECT_ITER_CLI it = this->getClients().begin(); it != this->getClients().end(); it++) {
//	 if (it->getFd() != client.getFd() && isInRoom(*it, *this, params[0])) {
//		if (!isInRoom(client, *this, params[0]))
//			return;
//		std::string message = Utils::ft_join(params, " ", 1);
//		if (params[1][0] == ':')
//			message = message.substr(1, message.length());
//		(*it).getmesagesFromServer().push_back(RPL_PRIVMSG(client.getUserByHexChat(), params[0], message));
//		FD_SET((*it).getFd(), &this->writefds);
//	}
//	else if (params[0] == (*it).getUserName() || params[0] == (*it).getNick()) {
//		if (params[1].find("PING") != std::string::npos) {
//			input = input + " " +client.getNick();
//			Server::ping(input, client);
//			break;
//		}
//		std::string message = Utils::ft_join(params, " ", 1);
//		if (params[1][0] == ':')
//			message = message.substr(1, message.length());
//		(*it).getmesagesFromServer().push_back(RPL_PRIVMSG(client.getUserByHexChat(), params[0], message));
//		FD_SET((*it).getFd(), &this->writefds);
//	}
//}
}
