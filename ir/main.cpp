#include "./includes/Server.hpp"
#include "./includes/Client.hpp"

#include <iostream>
#include <stdexcept>

using namespace std;

int main  (int argc, char *argv[]) {
	try {
		if (argc != 3) {
			throw std::invalid_argument("Usage: ./server <port>");
		}
		Server server(argv[1], argv[2]);
		server.run();
	}
	catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
	return 0;
}
