#pragma once
#include <string>

#include "Define.hpp"
#include "Server.hpp"

class Client;
class Server;

namespace Executor
{
	void pass(const std::string &param, Client &client, C_STR_REF password, fd_set &fd);
	void cap(C_STR_REF params, Client &client);
	void nick(C_VECT_STR_R params, Client &client, fd_set &fd);
	void user(C_STR_REF params, Client &client, fd_set &fd);
}
