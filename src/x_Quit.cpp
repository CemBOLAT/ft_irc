#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <stdlib.h>
#include "Client.hpp"
#include "Executor.hpp"
#include "Exception.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include "Room.hpp"
#include "Utils.hpp"
#include "Define.hpp"

//quit is broken
void Server::quit(C_STR_REF input, Client& client) {
    // Remove client from all channels
    for (VECT_ITER_CHA it = channels.begin(); it != channels.end(); ++it) {
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
    VECT_ITER_CLI eraseBegin = clients.end();
    for (VECT_ITER_CLI it = clients.begin(); it != clients.end(); ++it) {
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
