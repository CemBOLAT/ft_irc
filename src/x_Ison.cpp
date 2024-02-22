#include "Client.hpp"

#include "Exception.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include "Room.hpp"
#include "Utils.hpp"
#include "TextEngine.hpp"
#include "Define.hpp"
#include "ErrorRPL.hpp"

#include <iostream>
#include <string>
#include <sstream>
#include <vector>

/*
5.8 Ison message

      Command: ISON
   Parameters: <nickname>{<space><nickname>}

   The ISON command was implemented to provide  a  quick  and  efficient
   means  to get a response about whether a given nickname was currently
   on IRC. ISON only takes one (1) parameter: a space-separated list of
   nicks.  For  each  nickname in the list that is present, the server
   adds that to its reply string.  Thus the reply string may return
   empty (none  of  the given  nicks are present), an exact copy of the
   parameter string (all of them present) or as any other subset of the
   set of nicks  given  in  the parameter.  The only limit on the number
   of nicks that may be checked is that the combined length must not be
   too large as to cause the server to chop it off so it fits in 512
   characters.

   ISON is only be processed by the server local to the client sending
   the command and thus not passed onto other servers for further
   processing.

   Numeric Replies:

           RPL_ISON                ERR_NEEDMOREPARAMS

   Examples:

   ISON phone trillian WiZ jarlek Avalon Angel Monstah
                                   ; Sample ISON request for 7 nicks.

*/

// tam bitmedi 
void Server::ison(C_STR_REF param, Client &client) {
    std::istringstream iss(param);
    std::string nick;
    std::string response = "";
    bool found = false;

    while (iss >> nick) {
        for (std::vector<Client>::const_iterator it = clients.begin(); it != clients.end(); ++it) {
            if (it->getNick() == nick) {
                response += nick + " ";
                found = true;
                break;
            }
        }
    }

    if (found) {
        response.erase(response.length() - 1); 
        Utils::instaWrite(client.getFd(), "303 " + client.getNick() + " :" + response);
    } else {

        Utils::instaWrite(client.getFd(), "303 " + client.getNick() + " :");
    }
}