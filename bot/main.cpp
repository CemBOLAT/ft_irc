#include "Bot.hpp"
#include "Exception.hpp"
#include "Utils.hpp"
#include "TextEngine.hpp"
#include "Define.hpp"

Bot	*botInstance = NULL;
volatile sig_atomic_t sigint = 0;
void	signalHandler(int signum){
	sigint = 1;
	std::cout << "\rCaught signal " << signum << std::endl;
	if (signum == SIGINT)
	{
		if (botInstance){
			delete botInstance;
			botInstance = NULL;
		}
		exit(0);
	}
}

int main(int ac, char **av)
{
	try {
		if (ac != 3){
			throw Exception(BOT_USAGE);
		}
		Bot	*bot = new Bot(av[1], av[2]);
		botInstance = bot;
		signal(SIGINT, signalHandler);
		bot->run();
		//Bot	bot(av[1], av[2]);
		//bot.run();
	} catch (Exception &e) {
		TextEngine::red(e.what(), cout) << endl;
	}
	return 0;
}