#include "Client.hpp"
#include "Executor.hpp"
#include "Exception.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include "Room.hpp"
#include "Utils.hpp"
#include <iostream>
#include <string>
#include <vector>
#include "Define.hpp"

void Server::ping(C_STR_REF params, Client& client) {
    if (params.empty()) {
        return;
    }
    
    VECT_STR split = Utils::ft_split(params, " ");
    
    if (split.size() < 2) {
        // Invalid ping request
        return;
    }
    
    std::string targetNick = split[1];
    std::string timestamp;
    if (split.size() > 2) {
        // Extract timestamp from the parameters
        timestamp = split[2];
    }
    
    std::string response = "PONG " + targetNick;
    
    if (!timestamp.empty()) {
        // If there's a timestamp, include it in the response
        response += " :" + timestamp;
    }
    
    Utils::instaWrite(client.getFd(), ":" + client.getUserByHexChat() + " " + response + "\r\n");
}
