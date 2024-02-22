#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <stdlib.h>
#include "Client.hpp"

#include "Exception.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include "Room.hpp"
#include "Utils.hpp"
#include "Define.hpp"
#include "TextEngine.hpp"

// /*
// 	4.1.6 Quit

// 	Command: QUIT
// 	Parameters: [<Quit message>]

// 	A client session is ended with a quit message.  The server must close
// 	the connection to a client which sends a QUIT message. If a "Quit
// 	Message" is given, this will be sent instead of the default message,
// 	the nickname.

// 	When netsplits (disconnecting of two servers) occur, the quit message

// 	is composed of the names of two servers involved, separated by a
// 	space.  The first name is that of the server which is still connected
// 	and the second name is that of the server that has become
// 	disconnected.

// 	If, for some other reason, a client connection is closed without the
// 	client  issuing  a  QUIT  command  (e.g.  client  dies and EOF occurs
// 	on socket), the server is required to fill in the quit  message  with
// 	some sort  of  message  reflecting the nature of the event which
// 	caused it to happen.

// 	Numeric Replies:
// 	        None.
// 	Examples:
// 	QUIT :Gone to have lunch        ; Preferred message format.
// */


void Server::quit(C_STR_REF params, Client &client) {
    // Tüm kanallardan istemciyi çıkar
    for (VECT_ITER_CHA it = this->channels.begin(); it != this->channels.end(); ) {
        if (it->isClientInChannel(client.getFd())) {
            if (it->getClients().size() == 1) {
                TextEngine::magenta("Room " + it->getName() + " has been deleted", TextEngine::printTime(std::cout)) << std::endl;
                it = channels.erase(it);
                --it;
            } else {
                    it->removeClient(client.getFd());
			        
				    responseAllClientResponseToGui(client, *it);
            }
        } else {
            ++it;
        }
    }

    // İlgili istemci dosya tanımlayıcısını takip eden bağlantı kümesinden çıkar
    if (FD_ISSET(client.getFd(), &writefds)) {
        FD_CLR(client.getFd(), &writefds);
    }
    if (FD_ISSET(client.getFd(), &readfds)) {
        FD_CLR(client.getFd(), &readfds);
    }

    // İstemciyi kapat
    close(client.getFd());

    // Diğer tüm istemcilere çıkışını bildir
    for (VECT_ITER_CLI it3 = this->clients.begin(); it3 != this->clients.end(); ++it3) {
        if (it3->getFd() != client.getFd()) {
            Utils::instaWrite(it3->getFd(), RPL_QUIT(it3->getNick(), client.getNick()));
        }
    }

    // İstemciyi sunucudan tamamen kaldır
    for (VECT_ITER_CLI it2 = this->clients.begin(); it2 != this->clients.end(); ++it2) {
        if (it2->getFd() == client.getFd()) {
            this->clients.erase(it2);
            break;
        }
    }

    // İstemcinin çıkışını logla
    TextEngine::blue("Client ", TextEngine::printTime(cout)) << client._ip << ":" << client.getPort()<< " " <<client.getNick() << " quited !" << std::endl;
}













// void Server::quit(Client& client) {
// 	VECT_ITER_CHA it = this->channels.begin();
// 	for (; it != this->channels.end(); ++it) {
// 		if (it->isClientInChannel(client.getFd())) {
// 			if (it->getClients().size() == 1) // delete room.
// 			{
// 				TextEngine::magenta("Room " + it->getName() + " has been deleted", TextEngine::printTime(std::cout)) << std::endl;
// 				channels.erase(it);
// 				--it;
// 			}
// 			else if (client.getFd() == it->getOperator().getFd()){
// 				it->removeClient(client.getFd());
// 				it->setOperator(it->getClients()[0]);
// 				responseAllClientResponseToGui(client, *it);
// 			}
// 			else {
// 				it->removeClient(client.getFd());
// 				responseAllClientResponseToGui(client, *it);
// 			}
// 		}
// 	}
// 	if (FD_ISSET(client.getFd(), &writefds)) {
// 		FD_CLR(client.getFd(), &writefds);
// 	}
// 	if (FD_ISSET(client.getFd(), &readfds)) {
// 		FD_CLR(client.getFd(), &readfds);
// 	}
// 	close(client.getFd());
// 	VECT_ITER_CLI it3 = this->clients.begin();
// 	for (; it3 != this->clients.end(); ++it3) {
// 		if (it3->getFd() != client.getFd()) {
// 			Utils::instaWrite(it3->getFd(), RPL_QUIT(it3->getNick(), client.getNick()));
// 		}
// 	}
// 	VECT_ITER_CLI it2 = this->clients.begin();
// 	for (; it2 != this->clients.end(); ++it2) {
// 		if (it2->getFd() == client.getFd()) {
// 			this->clients.erase(it2);
// 			break;
// 		}
// 	}
// 	TextEngine::blue("Client ", TextEngine::printTime(cout)) << client._ip << ":" << client.getPort() << " quited !" << std::endl;
// }
