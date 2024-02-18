#include "Client.hpp"

#include "Exception.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include "Room.hpp"
#include "Utils.hpp"
#include "Define.hpp"

#include <iostream>
#include <string>
#include <sstream>
#include <vector>

/*
    Command: NAMES
    Parameters: [<channel>{,<channel>}]

    By using the NAMES command, a user can list all nicknames that are
    visible to them on any channel that they can see.  Channel names
    which they can see are those which aren't private (+p) or secret (+s)
    or those which they are actually on.  The <channel> parameter
    specifies which channel(s) to return information about if valid.
    There is no error reply for bad channel names.

    If no <channel> parameter is given, a list of all channels and their
    occupants is returned.  At the end of this list, a list of users who
    are visible but either not on any channel or not on a visible channel
    are listed as being on `channel' "*".

    Numerics:

            RPL_NAMREPLY                    RPL_ENDOFNAMES

    Examples:

    NAMES #twilight_zone,#42        ; list visible users on #twilight_zone
                                    and #42 if the channels are visible to
                                    you.

    NAMES                           ; list all visible channels and users
*/

void    Server::names(Client &fd, C_STR_REF channel){
   //string message;
   //for (VECT_ITER_CHA it = channels.begin(); it != channels.end(); it++){
   //    if (it->getName() == channel){
   //        for (VECT_ITER_CLI it2 = it->getClients().begin(); it2 != it->getClients().end(); it2++){
   //            if (it2->getFd() == it->getOperator().getFd())
   //                message += "@";
   //            message += (*it2).getNick() + " ";
   //        }
   //        Utils::instaWrite(fd.getFd(), RPL_NAMES(fd.getNick(), channel, message));
   //        Utils::instaWrite(fd.getFd(), RPL_NAMESEND(fd.getNick(), channel));
   //        return;
   //    }
   //}
}
