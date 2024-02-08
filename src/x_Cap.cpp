#include "Executor.hpp"
#include "Define.hpp"
#include "Client.hpp"
#include "Utils.hpp"

namespace Executor
{
	void cap(C_STR_REF params, Client &client)
	{
		if (params.empty())
		{
			Utils::instaWrite(client.getFd(), "CAP LS\r\n");
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
				Utils::instaWrite(client.getFd(), "Invalid parameters for CAP\n\r");
			}
		}
	}
}
