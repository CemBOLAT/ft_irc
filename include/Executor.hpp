#pragma once
#include <string>

#include "Define.hpp"
#include "Server.hpp"

class Client;
class Server;

namespace Executor
{
    void pass(C_VECT_STR params, Client &client, C_STR_REF password, fd_set &fd);
    void cap(C_VECT_STR_R params, Client &client);
    void nick(C_VECT_STR_R params, Client &client, fd_set &fd);
    void user(C_VECT_STR_R params, Client &client, fd_set &fd);
    void mode(C_VECT_STR_R params, Client &client);
}
