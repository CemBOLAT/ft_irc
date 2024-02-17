#include "Client.hpp"
#include "Executor.hpp"
#include "Exception.hpp"
#include <iostream>
#include <string>
#include <vector>
#include "Utils.hpp"
#include "Define.hpp"

namespace Executor
{
	void pass(C_STR_REF params, Client &client, C_STR_REF password){
		if (params.empty())
		{
			Utils::instaWrite(client.getFd(), ": 461 " + client.getUserByHexChat() + " PASS :Not enough parameters\r\n");
		}
		else if (client.getIsPassworded())
		{
			Utils::instaWrite(client.getFd(), ": 462 " + client.getUserByHexChat() + " PASS :You may not reregister\r\n");
		}
		else if (params != password)
		{
			Utils::instaWrite(client.getFd(), ": 464 " + client.getUserByHexChat() + " PASS :Invalid password\r\n"); 
		}
		else {
			client.setPassworded(true);
			Utils::instaWrite(client.getFd(), ": 001 Password accepted\r\n");
		}
	}
}
