#include "../include/Client.hpp"
#include "../include/Executor.hpp"
#include "../include/Exception.hpp"
#include "../include/Utils.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

#define RPL_TOPIC(nick, channel, topic) ": 332 " + nick + " " + channel + " :" + topic + "\r\n"
#define RPL_NOTOPIC(nick, channel) ": 331 " + nick + " " + channel + " :No topic is set\r\n"
#define RPL_TOPICSET(nick, channel, topic, time) ": 333 " + nick + " " + channel + " " + time + "\r\n"
#define ERR_NOSUCHCHANNEL(source, channel) "403 " + source + " " + channel + " :No such channel"
#define ERR_NEEDMOREPARAMS(source, command) ": 461 " + source + " " + command + " :Not enough parameters" + "\r\n"

void Server::topic(const std::string &command, Client &client) {
    std::vector<std::string> params = Utils::ft_split(command, " ");
    if (params.size() < 2) {
        Utils::instaWrite(client.getFd(), ERR_NEEDMOREPARAMS(client.getNick(), "TOPIC"));
        return;
    }
	if (params[0][0] == '#'){
		params[0] = params[0].substr(1);
	}
    std::string channelName = params[0]; // Remove the '#' prefix from channel name
    bool channelFound = false;

    for (size_t i = 0; i < this->channels.size(); ++i) {
        Room &channel = this->channels[i];
        if (channelName == channel.getName()) {
            channelFound = true;
            if (params.size() == 1) { // Retrieve topic
                std::string topic = channel.getTopic();
                if (!topic.empty()) {
                    std::string response = RPL_TOPIC(client.getNick(), channel.getName(), topic);
                    Utils::instaWrite(client.getFd(), response);
                } else {
                    Utils::instaWrite(client.getFd(), RPL_NOTOPIC(client.getNick(), channel.getName()));
                }
            } else { // Set topic
                channel.setTopic(params[1]);
                std::string response = RPL_TOPICSET(client.getNick(), channel.getName(), params[1], "0");
                Utils::instaWrite(client.getFd(), response);
            }
            break;
        }
    }

    if (!channelFound) {
        Utils::instaWrite(client.getFd(), ERR_NOSUCHCHANNEL(client.getNick(), channelName));
    }
}
