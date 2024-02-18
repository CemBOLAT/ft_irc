#include "Client.hpp"
#include "Executor.hpp"
#include "Exception.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include "Room.hpp"
#include "Utils.hpp"
#include <iostream>
#include <string>
#include <vector>

#include "Define.hpp"


/*
4.6.3 Pong message

      Command: PONG
   Parameters: <daemon> [<daemon2>]

   PONG message is a reply to ping message.  If parameter <daemon2> is
   given this message must be forwarded to given daemon.  The <daemon>
   parameter is the name of the daemon who has responded to PING message
   and generated this message.

   Numeric Replies:

           ERR_NOORIGIN                    ERR_NOSUCHSERVER

   Examples:

   PONG csd.bu.edu tolsun.oulu.fi  ; PONG message from csd.bu.edu to



Oikarinen & Reed                                               [Page 37]

RFC 1459              Internet Relay Chat Protocol              May 1993


                                   tolsun.oulu.fi

*/

void Server::pong(C_STR_REF params, Client& client) {
    if (params.empty()) {
        return;
    }
    
    VECT_STR split = Utils::ft_split(params, " ");
    
    if (split.size() < 2) {
        // Invalid pong request
        return;
    }
    
    std::string targetNick = split[1];
    std::string timestamp;
    if (split.size() > 2) {
        // Extract timestamp from the parameters
        timestamp = split[2];
    }
    
    std::string response = "PONG " + targetNick;
    
    if (!timestamp.empty()) {
        // If there's a timestamp, include it in the response
        response += " :" + timestamp;
    }
    
    Utils::instaWrite(client.getFd(), ":" + client.getUserByHexChat() + " " + response + "\r\n");
}