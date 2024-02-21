/*

5.1 Away

      Command: AWAY
   Parameters: [message]

   With the AWAY message, clients can set an automatic reply string for
   any PRIVMSG commands directed at them (not to a channel they are on).
   The automatic reply is sent by the server to client sending the
   PRIVMSG command.  The only replying server is the one to which the
   sending client is connected to.

   The AWAY message is used either with one parameter (to set an AWAY
   message) or with no parameters (to remove the AWAY message).

   Numeric Replies:

	   RPL_UNAWAY		      RPL_NOWAWAY

   Examples:

   AWAY :Gone to lunch.  Back in 5 ; set away message to "Gone to lunch.
				   Back in 5".

   :WiZ AWAY		       ; unmark WiZ as being away.
*/

#include "Server.hpp"
#include "Client.hpp"
#include "Define.hpp"
#include "Utils.hpp"

void    Server::away(C_STR_REF msg, Client &client) // hexchat testi
{
	if (msg.empty())
	{
		client.setIsAway(false);
		client.setAwayMSG("");
		Utils::instaWrite(client.getFd(), RPL_UNAWAY);
	}
	else
	{
		client.setIsAway(true);
		client.setAwayMSG(msg);
		Utils::instaWrite(client.getFd(), RPL_NOWAWAY);
	}
}
