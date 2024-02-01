#include "../include/Exception.hpp"
#include "../include/TextEngine.hpp"
#include "../include/Utils.hpp"
#include "../include/Server.hpp"

using namespace std;

int main(int argc, char** argv)
{
	try
	{
		if (argc != 3){
			throw Exception(USAGE);
		}
		Server	server(argv[1], argv[2]);
		server.run();
	}
	catch(const Exception& e)
	{
		TextEngine::red(e.what(), cout) << endl;
	}
	return 0;
}
