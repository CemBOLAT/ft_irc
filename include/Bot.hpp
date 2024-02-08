#pragma once

#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>
#include <sstream>
#include <string>


class Bot {
	public:
		Bot(int port, std::string password);
		~Bot() {
			close(this->_socket);
		}
		void	run();
		void	initSocket();

	private:
		std::string	_name;
		int			_socket;
		int			_port;
		std::string	_password;
		struct sockaddr_in	_bot_addr;
		char		_buffer[1024];

};
