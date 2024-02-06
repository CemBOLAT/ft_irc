//#include "../include/Client.hpp"
//#include "../include/Executor.hpp"
//#include "../include/Exception.hpp"
//#include "../include/Client.hpp"
//#include "../include/Server.hpp"
//#include "../include/Room.hpp"
//#include "../include/Utils.hpp"
//#include <iostream>
//#include <string>
//#include <vector>
//
//#define RPL_PING(source, nick, second)				":" + source + " PONG " + nick + " :"+ second + "\r\n"
//
//Client getCli(std::vector<Client> &clients, std::string nick) {
//	std::vector<Client>::iterator it = clients.begin();
//	for (; it != clients.end(); ++it) {
//		if (it->getNick() == nick)
//			return *it;
//	}
//	return *it;
//}
//
//std::string getMesg(C_VECT_STR_R params) {
//	std::string msg;
//	for (size_t i = 1; i < params.size(); i++) {
//		msg += params[i];
//		if (i + 1 < params.size())
//			msg += " ";
//	}
//	return msg;
//}
//
//void	Server::ping(C_STR_REF params, Client &client){
//	if (params.size() == 1){
//		return;
//	}
//	std::string msg = getMesg(params);
//	if (msg.at(0) == '0' && msg.at(1) == '0') {
//		params[1] = params[1].substr(2, params[1].length());
//		Utils::instaWrite(client.getFd(), RPL_PING(client.getUserByHexChat(), params[1], msg.substr(msg.find("PING")+5, msg.length()-1)));
//	} else if (msg.find(":PING") && params[1].size()) {
//		Client target = getCli(this->clients, params[1]);
//		Utils::instaWrite(client.getFd(), RPL_PING(client.getUserByHexChat(), params[1], msg.substr(msg.find("PING")+5, msg.length()-1)));
//		std::string msg = "00" + client.getNick() + " :PING " + params[2];
//		VECT_STR newParams = Utils::ft_split(msg, " ");
//		Server::ping(newParams, target);
//	} else {
//		Utils::instaWrite(clients[0].getFd(), RPL_PING(client.getUserByHexChat(), std::string("FT_IRC_SERVER"), params[1]));
//	}
//}

/*

int     Server::Ping(std::string &s, Client &cli) {
    std::vector<std::string> param;
    if (!s.empty()) {
        param = Utilities::splitString(s);
        if (s.at(0) == '0' && s.at(1) == '0') {
            param[0] = param[0].substr(2, param[0].length());
            Utilities::writeRpl(cli.cliFd, RPL_PING(cli.getPrefix(), param[0], s.substr(s.find("PING")+5, s.length()-1)));
        } else if (s.find(":PING") && param[1].size()) {
            Client target = getCli(this->clients, param[0]);

            Utilities::writeRpl(cli.cliFd, RPL_PING(cli.getPrefix(), param[0], s.substr(s.find("PING")+5, s.length()-1)));
            std::string msg = "00" + cli.nick + " :PING " + param[2];
            Server::Ping(msg, target);
        } else {//if cmd handler is come here will be enter
            Utilities::writeRpl(clients[0].cliFd, RPL_PING(cli.getPrefix(), std::string("FT_IRC_SERVER"), param[0]));
        }
    }
    return 1;
}


*/
