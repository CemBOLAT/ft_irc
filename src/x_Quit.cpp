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

void Server::quit(Client& client) {
    //// Remove client from all channels
	std::vector<Room>::iterator it = this->channels.begin();
	for (; it != this->channels.end(); ++it) {
		if (it->isClientInChannel(client.getFd())) {
			it->removeClient(client.getFd());
			responseAllClientResponseToGui(client, *it);
		}
	}
	//// Remove client from the server
	if (FD_ISSET(client.getFd(), &writefds)) {
		FD_CLR(client.getFd(), &writefds);
	}
	if (FD_ISSET(client.getFd(), &readfds)) {
		FD_CLR(client.getFd(), &readfds);
	}
	//// Close the client's socket
	close(client.getFd());
	// Send a message to all clients
	std::vector<Client>::iterator it3 = this->clients.begin();
	for (; it3 != this->clients.end(); ++it3) {
		if (it3->getFd() != client.getFd()) {
			Utils::instaWrite(it3->getFd(), RPL_QUIT(it3->getNick(), client.getNick()));
		}
	}
	std::vector<Client>::iterator it2 = this->clients.begin();
	for (; it2 != this->clients.end(); ++it2) {
		if (it2->getFd() == client.getFd()) {
			this->clients.erase(it2);
			break;
		}
	}
	// Print a message to the server
	TextEngine::blue("Client ", TextEngine::printTime(cout)) << client._ip << ":" << client.getPort() << " quited !" << std::endl;
}