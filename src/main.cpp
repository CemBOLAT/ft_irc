#include "Exception.hpp"
#include "TextEngine.hpp"
#include "Utils.hpp"
#include "Server.hpp"
#include <signal.h>
#include <stdlib.h>
using namespace std;

Server	 *serverInstance = NULL;
volatile sig_atomic_t sigint = 0; // volatile sig_atomic_t is a type that is guaranteed to be accessed atomically even in the presence of signals.

void	signalHandler(int signum){
	sigint = 1;
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
