#include "../include/Client.hpp"
#include "../include/Executor.hpp"
#include "../include/Exception.hpp"
#include <iostream>
#include <string>
#include <vector>

#define VECT_STR std::vector<std::string>

namespace Executor
{
    void password(const VECT_STR &params, Client &client, const std::string &password){
        if (params.size() != 2)
        {
            client.setBuffer("Wrong number of arguments\n");
        }
        if (client.getIsPassworded())
        {
            client.setBuffer("Password already set\n");
        }
        if (params[1] != password) 
        {
            client.setBuffer("Wrong password\n");
        }
        client.setPassworded(true);
        client.setBuffer("Password accepted\n");
    }

}