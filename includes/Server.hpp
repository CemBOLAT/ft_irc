#pragma once

#include <string>
#include "./Socket.hpp"

class Server{
	public:
		Server(std::string ip, std::string port);
		void run();
	private:
		std::string ip;
		std::string port;
		Socket socket;
		Server();
		Server(const Server &server);
		Server &operator=(const Server &server);
};
