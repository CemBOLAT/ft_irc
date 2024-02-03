#pragma once
#include <string>

#include "Define.hpp"

class Client;
class Server;

namespace Executor
{
    void pass(C_VECT_STR params, Client &client, C_STR_REF password);
    void cap(C_VECT_STR_R params, Client &client);
    void nick(C_VECT_STR_R params, Client &client);
    void user(C_VECT_STR_R params, Client &client);
}
