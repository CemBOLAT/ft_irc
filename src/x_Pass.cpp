#include "../include/Client.hpp"
#include "../include/Executor.hpp"
#include "../include/Exception.hpp"
#include <iostream>
#include <string>
#include <vector>

namespace Executor
{
	void pass(C_VECT_STR params, Client &client, C_STR_REF password){
		std::cout << "client.getName() = " << client._ip << std::endl;
		if (params.size() != 2)
		{
			client.getmesagesFromServer().push_back("Invalid Number of Arg\n");
		}
		else if (client.getIsPassworded())
		{
			client.getmesagesFromServer().push_back("Already passworded\n");
		}
		else if (params[1] != password)
		{
			client.getmesagesFromServer().push_back("Invalid password\n");
		} else {
			client.setPassworded(true);
			client.getmesagesFromServer().push_back("Password accepted\n");
		}
	}
}
