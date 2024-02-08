#include "../include/Client.hpp"
#include "../include/Executor.hpp"
#include "../include/Exception.hpp"
#include "../include/Client.hpp"
#include "../include/Server.hpp"
#include "../include/Room.hpp"
#include "../include/Utils.hpp"
#include <iostream>
#include <string>
#include <vector>


void Server::pong(const std::string& params, Client& client) {
    if (params.empty()) {
        return;
    }
    
    std::vector<std::string> split = Utils::ft_split(params, " ");
    
    if (split.size() < 2) {
        // Invalid pong request
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