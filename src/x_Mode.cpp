#include "../include/Client.hpp"
#include "../include/Executor.hpp"
#include "../include/Exception.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>

#define ERR_NEEDMOREPARAMS(source, command)			": 461 " + source + " " + command + " :Not enough parameters" + "\r\n"
#define ERR_NOSUCHCHANNEL(source, channel)			": 403 " + source + " " + channel + " :No such channel" + "\r\n"
#define ERR_CHANOPRIVSNEEDED(source, channel)		": 482 " + source + " " + channel + " :You're not the channel operator" + "\r\n"
#define RPL_MODE(source, channel, modes, args)		":" + source + " MODE " + channel + " " + modes + " " + args + "\r\n"


namespace Executor
{
    void mode(C_VECT_STR_R params, Client &client)
    {
        if (params.size() > 4)
        {
            std::string test = ERR_NEEDMOREPARAMS(client.getNick(), params[1]);

            write(client.getFd(), test.c_str(), test.length())  ?  (std::cout << "MSG cannot send" << std::endl) : (std::cout << "MSG SEND" << std::endl);
        }
        if (params.size() == 1)
            return ;
        std::vector<Chanel>::iterator it = this->chanels.begin();
        for (; it != this->hanels.end(); ++it)
            if (it->name == params[1])
                break;
        if (it == this->chanels.end())
        {
            std::string test = ERR_NEEDMOREPARAMS(client.getNick(), params[1]);

            write(client.getFd(), test.c_str(), test.length()) ? (std::cout << "MSG cannot send" << std::endl) : (std::cout << "MSG SEND" << std::endl);
            return ;
        }
        if (it->op->user != client.getUserName())
        {
            std::string test = ERR_CHANOPRIVSNEEDED(client.getUserName(), params[1]);

            write(client.getFd(), test.c_str(), test.length()); ? (std::cout << "MSG cannot send" << std::endl) : (std::cout << "MSG SEND" << std::endl);   
        }
        if (params[2] == "+k")
        {
            if (params.size() == 4)
            {
                it->key = params[3];
                it->keycode |= K_CODE;
                std::string test = RPL_MODE(client.getUserName(), params[1], "+k", params[3]);
                write(client.getFd(), test.c_str(), test.length()); ? (std::cout << "MSG cannot send" << std::endl) : (std::cout << "MSG SEND" << std::endl);
                std::cout << "key is added" << std::endl;
            }
        }
        if (params[2] == "+l")
        {
            if (params.size() == 4)
            {
                it->users = atoi(params[3].c_str());
                it->keycode |= L_CODE;
                std::string test = RPL_MODE(client.getUserName(), params[1], "+l", params[3]);
                write(client.getFd(), test.c_str(), test.length()); ? (std::cout << "MSG cannot send" << std::endl) : (std::cout << "MSG SEND" << std::endl);
            }
        }
        if (params[2] == "+o")
        {
            if (params.size() == 4)
            {
                std::cout << "OP" << std::endl;
                //Server::Op(s, cli);
            }
        }
        if (params[2] == "-k")
        {
            if (params.size() == 4 && it->key == params[3])
            {
                it->key = "";
                it->keycode ^= K_CODE;
                std::string test = RPL_MODE(client.getUserName(), params[1], "-k", "");
                write(client.getFd(), test.c_str(), test.length()); ? (std::cout << "MSG cannot send" << std::endl) : (std::cout << "MSG SEND" << std::endl);
            }      
        } 
        if (params[2] == "-l")
        {
            if (params.size() == 3)
            {
                it->keycode ^= L_CODE;
                std::string test = RPL_MODE(client.getUserName(), params[1], "-l", "");
                write(client.getFd(), test.c_str(), test.length()); ? (std::cout << "MSG cannot send" << std::endl) : (std::cout << "MSG SEND" << std::endl);   
            }
        }
        return ;
    }
}