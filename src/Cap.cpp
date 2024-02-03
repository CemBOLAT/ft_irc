#include "../include/Executor.hpp"
#include "../include/Define.hpp"
#include "../include/Client.hpp"

namespace Executor
{
	void cap(C_VECT_STR  &params, Client &client)
	{
		if (params.size() < 2)
		{
			client.getmesagesFromServer().push_back("Invalid number of parameters for CAP\n");
		}
		else
		{
			if (params[1] == "LS")
			{
				client.setType(hexchat);
			}
			else if (params[1] == "NC")
			{
				client.setType(nc);
			}
			else if (params[1] == "BOT")
			{
				client.setType(bot);
			}
			else
			{
				client.getmesagesFromServer().push_back("Invalid parameters for CAP\n");
			}
		}
	}
}
