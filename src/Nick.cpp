#include "../include/Client.hpp"
#include "../include/Executor.hpp"
#include "../include/Exception.hpp"
#include <iostream>
#include <string>
#include <vector>

namespace Executor
{
	void nick(C_VECT_STR_R params, Client &client)
	{
		if (params.size() != 2)
		{
			client.getmesagesFromServer().push_back("Invalid Number of Arg\n");
		}
		else
		{
			client.setNick(params[1]);
			client.getmesagesFromServer().push_back("Nick accepted as " + params[1] + "\n");
		}
	}
}
