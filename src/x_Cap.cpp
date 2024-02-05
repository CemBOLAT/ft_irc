#include "Executor.hpp"
#include "Define.hpp"
#include "Client.hpp"

namespace Executor
{
	void cap(C_STR_REF params, Client &client)
	{
		if (params.empty())
		{
			client.getmesagesFromServer().push_back("Invalid number of parameters for CAP\n\r");
		}
		else
		{
			if (params == "LS")
			{
				client.setType(hexchat);
			}
			else if (params == "NC")
			{
				client.setType(nc);
			}
			else if (params == "BOT")
			{
				client.setType(bot);
			}
			else
			{
				client.getmesagesFromServer().push_back("Invalid parameters for CAP\n\r");
			}
		}
	}
}
