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


# define RPD_NAMESSTART(nick, channel)			    ": 353 " + nick + " = " + channel + " :"
# define RPL_NAMES(nick, channel, names)             ": 353 " + nick + " = " + channel + " : " + names + "\r\n"
# define RPL_NAMESEND(nick, channel)				    ": 366 " + nick + " " + channel + " :End of /NAMES list\r\n"

void    Server::names(Client &fd, const std::string &channel){
  //  std::cout << "Names" << std::endl;

    string message;
    for (std::vector<Room>::iterator it = channels.begin(); it != channels.end(); it++){
        if (it->getName() == channel){
            for (std::vector<Client>::iterator it2 = it->getClients().begin(); it2 != it->getClients().end(); it2++){
                if (it2->getFd() == it->getOperator()->getFd())
                    message += "@";
                message += (*it2).getNick() + " ";
            }
            Utils::instaWrite(fd.getFd(), RPL_NAMES(fd.getNick(), channel, message));
            Utils::instaWrite(fd.getFd(), RPL_NAMESEND(fd.getNick(), channel));
            return;
        }
    }
    if (channel == "localhost"){
        for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); it++){
            message += (*it).getNick() + " ";
        }
        Utils::instaWrite(fd.getFd(), RPL_NAMES(fd.getNick(), channel, message));
    }

}
