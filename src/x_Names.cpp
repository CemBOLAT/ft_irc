#include "Client.hpp"
#include "Executor.hpp"
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

void    Server::names(Client &fd, C_STR_REF channel){
    string message;
    for (VECT_ITER_CHA it = channels.begin(); it != channels.end(); it++){
        if (it->getName() == channel){
            for (VECT_ITER_CLI it2 = it->getClients().begin(); it2 != it->getClients().end(); it2++){
                if (it2->getFd() == it->getOperator()->getFd())
                    message += "@";
                message += (*it2).getNick() + " ";
            }
            Utils::instaWrite(fd.getFd(), RPL_NAMES(fd.getNick(), channel, message));
            Utils::instaWrite(fd.getFd(), RPL_NAMESEND(fd.getNick(), channel));
            return;
        }
    }
}
