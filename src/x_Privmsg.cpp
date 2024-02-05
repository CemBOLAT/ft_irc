#include "../include/Client.hpp"
#include "../include/Executor.hpp"
#include "../include/Exception.hpp"
#include "../include/Client.hpp"
#include "../include/Server.hpp"
#include "../include/Room.hpp"
#include <iostream>
#include <string>
#include <vector>

#define RPL_PRIVMSG(source, target, message)		":" + source + " PRIVMSG " + target + " :" + message + "\r\n"

string	getMessage(C_VECT_STR_R params, unsigned int start){
	string message;
	for (unsigned int i = start; i < params.size(); i++){
		message += params[i];
		if (i + 1 < params.size())
			message += " ";
	}
	if (message[0] == ':')
		message = message.substr(1, message.size() - 1);
	return message;
}

void	Server::privmsg(VECT_STR params, Client &client){
	string	message = getMessage(params, 2);
	for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it){
		if (it->getFd() != client.getFd() && it->getNick() == params[1]){
			if (!isClientInRoom(*it, params[1]))
				return;
			string	newParam1;
			if (params[2][0] == ':')
				newParam1 = params[2].substr(1, params[2].length());
			else
				newParam1 = params[2];
			(*it).getmesagesFromServer().push_back(RPL_PRIVMSG(client.getUserByHexChat(), params[1], newParam1));
			FD_SET((*it).getFd(), &this->writefds);
		}
		else if (params[1] == (*it).getUserName() || params[1] == (*it).getNick()){
			if (params[2].find("PING") != std::string::npos){
				params[2] = params[2] + " " + client.getNick();
				Server::ping(params, client);
				break;
			}
			string	newParam1;
			if (params[2][0] == ':')
				newParam1 += params[2].substr(1, params[2].length());
			else
				newParam1 += params[2];
			(*it).getmesagesFromServer().push_back(RPL_PRIVMSG(client.getUserByHexChat(), params[1], newParam1));
			FD_SET((*it).getFd(), &this->writefds);
		}
	}
}

/*
int     Server::PrivMsg(std::string &s, Client& cli) {
    std::vector<std::string> params = Utilities::tokenCmd(s, 0); // cmd[0] == # it is channel cmd[0] != # it is client
    if (!cli.passcheku)
        return 0;
    for(std::vector<Client>::iterator it = this->clients.begin(); it != this->clients.end();++it) // search all clients
    {
        if (it->cliFd != cli.cliFd && isClientIn((*it), params[0]))//if you are not yourself and client(it) is in this chanel
        {
            if (!isClientIn(cli, params[0]))
                return 1;
            (*it).messageBox.push_back(RPL_PRIVMSG(cli.getUserByHexChat(), params[0], (params[1][0]==':'?params[1].substr(1, params[1].length()) : params[1])));
            FD_SET((*it).cliFd, &this->writefds);
        }
        else if (params[0] == (*it).user || params[0] == (*it).nick) {
            if (params[1].find("PING") != std::string::npos) {
                s = s + " " +cli.nick;
                Server::Ping(s, cli);
                break;
            }
            (*it).messageBox.push_back(RPL_PRIVMSG(cli.getUserByHexChat(), (*it).nick, (params[1][0]==':'?params[1].substr(1, params[1].length()) : params[1])));
            FD_SET((*it).cliFd, &this->writefds);
        }
    }
    return 1;
}


*/
