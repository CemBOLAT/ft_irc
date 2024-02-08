#pragma once
#include <string>

#include "Define.hpp"
#include "Server.hpp"

class Client;
class Server;

namespace Executor
{
	void pass(const std::string &param, Client &client, C_STR_REF password);
	void cap(C_STR_REF params, Client &client);
	void user(C_STR_REF params, Client &client);
}
