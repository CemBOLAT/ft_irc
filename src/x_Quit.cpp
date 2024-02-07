#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <stdlib.h>
#include "../include/Client.hpp"
#include "../include/Executor.hpp"
#include "../include/Exception.hpp"
#include "../include/Client.hpp"
#include "../include/Server.hpp"
#include "../include/Room.hpp"
#include "../include/Utils.hpp"

#define RPL_QUIT(source, message)                   (std::string(":") + source + " QUIT :" + message + "\r\n")
#define RPL_PART(source, channel)                   (std::string(":") + source + " PART :" + channel + "\r\n")


//quit is broken

void Server::quit(const std::string& input, Client& client) {
    // Remove client from all channels
    for (std::vector<Room>::iterator it = channels.begin(); it != channels.end(); ++it) {
        if (it->isClientInChannel(client.getFd())) {
            if (it->getClients().size() == 1) {
                Server::part(it->getName(), client);
            } else {
                it->removeClient(client.getFd());
                Utils::instaWrite(client.getFd(), RPL_PART(client.getUserByHexChat(), input));
            }
        }
    }

    // Erase client from the clients vector
    std::vector<Client>::iterator eraseBegin = clients.end();
    for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it) {
        if (it->getFd() == client.getFd()) {
            eraseBegin = it;
            break;
        }
    }

    if (eraseBegin != clients.end()) {
        clients.erase(eraseBegin);
    }

    // Send quit message to the client
    std::string msg = input;
    if (!msg.empty() && msg[0] == ':') {
        msg = msg.substr(1);
    }
    Utils::instaWrite(client.getFd(), RPL_QUIT(client.getUserByHexChat(), msg.c_str()));

    // Clear file descriptors if necessary
    if (FD_ISSET(client.getFd(), &writefds)) {
        FD_CLR(client.getFd(), &writefds);
    }
    if (FD_ISSET(client.getFd(), &readfds)) {
        FD_CLR(client.getFd(), &readfds);
    }

    // Close client socket
    close(client.getFd());
}
