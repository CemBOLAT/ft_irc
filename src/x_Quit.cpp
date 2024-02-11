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
#include "TextEngine.hpp"

void Server::quit(C_STR_REF input, Client& client) {
//quit is broken
    //// Remove client from all channels
    for (VECT_ITER_CHA it = channels.begin(); it != channels.end(); ++it) {
        if (it->isClientInChannel(client.getFd())) {
            Server::part(it->getName(), client);
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

    // Clear file descriptors if necessary
    if (FD_ISSET(client.getFd(), &writefds)) {
        FD_CLR(client.getFd(), &writefds);
    }
    if (FD_ISSET(client.getFd(), &readfds)) {
        FD_CLR(client.getFd(), &readfds);
    }
    Utils::instaWrite(client.getFd(), RPL_QUIT(client.getUserByHexChat(), input.c_str()));
    //// Close client socket
    close(client.getFd());
    TextEngine::blue("Client ", TextEngine::printTime(cout)) << client._ip << ":" << client.getPort() << " disconnected" << std::endl;
}
