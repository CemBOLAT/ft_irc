#include "Bot.hpp"
#include "Exception.hpp"
#include "Utils.hpp"
#include "Define.hpp"

int main(int ac, char **av)
{
	try {
		if (ac != 3){
			throw Exception(BOT_USAGE);
		}
		Bot	bot(av[1], av[2]);
		bot.run();
	} catch (Exception &e) {
		std::cerr << e.what() << std::endl;
	}
	return 0;
}
