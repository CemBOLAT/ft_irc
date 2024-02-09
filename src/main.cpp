#include "../include/Exception.hpp"
#include "../include/TextEngine.hpp"
#include "../include/Utils.hpp"
#include "../include/Server.hpp"
#include <signal.h>
#include <stdlib.h>
using namespace std;

Server	 *serverInstance = NULL;

void	signalHandler(int signum){
	std::cout << "\rCaught signal " << signum << std::endl;
	if (signum == SIGINT)
	{
		if (serverInstance){
			delete serverInstance;
			serverInstance = NULL;
		}
		exit(0);
	}
}

int main(int argc, char** argv)
{
	try
	{
		if (argc != 3){
			throw Exception(USAGE);
		}
		Server	*server = new Server(argv[1], argv[2]);
		serverInstance = server;
		signal(SIGPIPE, SIG_IGN); // signal makes : 
		signal(SIGINT, signalHandler); // catch ctrl+c (macos and linux have different signals)
		server->run();
	}
	catch(const Exception& e)
	{
		TextEngine::red(e.what(), cout) << endl;
	}
	return 0;
}
