#include "../include/Client.hpp"
#include "../include/Executor.hpp"
#include "../include/Exception.hpp"
#include "../include/Client.hpp"
#include "../include/Server.hpp"
#include "../include/Room.hpp"
#include "../include/Utils.hpp"

#include <iostream>
#include <string>
#include <sstream>
#include <vector>

# define RPL_LISTSTART(nick, numusers)			    ": 321 " + nick + " Channel : "+ numusers + "\r\n"
# define RPL_LIST(nick, channel, numusers, topic)   ": 322 " + nick + " " + channel + " " + numusers + " " + topic + "\r\n"
# define RPL_LISTEND(nick)						    ": 323 " + nick + " :End of /LIST\r\n"

namespace {
    string intToString(int num){
        std::stringstream ss;
        ss << num;
        return ss.str();
    }
}

void    Server::list(Client &fd){
    std::cout << "List" << std::endl;
    Utils::instaWrite(fd.getFd(), RPL_LISTSTART(fd.getNick(), intToString(channels.size())));
    for (std::vector<Room>::iterator it = channels.begin(); it != channels.end(); it++){
        if (it->getTopic().empty()){
            Utils::instaWrite(fd.getFd(), RPL_LIST(fd.getNick(), it->getName(), intToString(it->getClients().size()), " Empty"));
        }
        else{
            std::cout << "Topic: " << it->getTopic() << std::endl;       
            Utils::instaWrite(fd.getFd(), RPL_LIST(fd.getNick(), it->getName(), intToString(it->getClients().size()), it->getTopic()));
        }
    }
    Utils::instaWrite(fd.getFd(), RPL_LISTEND(fd.getNick()));
}