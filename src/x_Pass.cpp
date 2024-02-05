#include "../include/Client.hpp"
#include "../include/Executor.hpp"
#include "../include/Exception.hpp"
#include <iostream>
#include <string>
#include <vector>

namespace Executor
{
	void pass(C_STR_REF params, Client &client, C_STR_REF password, fd_set &fd){
		FD_SET(client.getFd(), &fd);
		if (params.empty())
		{
			client.getmesagesFromServer().push_back("Invalid Number of Arg\n\r");
		}
		else if (client.getIsPassworded())
		{
			client.getmesagesFromServer().push_back("Already passworded\n\r");
		}
		else if (params != password)
		{
			client.getmesagesFromServer().push_back("Invalid password\n\r");
		} else {
			client.setPassworded(true);
			client.getmesagesFromServer().push_back("Password accepted\n\r");
		}
	}
}
