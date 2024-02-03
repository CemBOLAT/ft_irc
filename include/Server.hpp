#pragma once


#include <string>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "Define.hpp"

using std::vector;
using std::string;

class Client;

class Server {
	public:
		Server(const string& port, const string& password);
		virtual	~Server();
		void	run();
	private:
		Server();
		Server(const Server& other);
		Server& operator=(const Server& other);
		void	initSocket();
		void	runCommand(const string& command, Client& client);
		void	hexChatEntry(VECT_STR &, Client&);

		int					port;
		string				password;
		int					_socket;
		struct sockaddr_in	address;
		struct sockaddr_in	clientAddress;


		fd_set				readfds;
		fd_set				writefds;
		/*
			The reason for storing file descriptors in supplementary sets (this->readFdsSup and this->writeFdsSup)
			instead of directly modifying the original sets (this->readFds and this->writeFds) is to avoid race conditions
			and to ensure that the original sets remain unchanged for the next iteration of the loop.


			avoid blocking sockets we need to use select() function
			and copy the original sets to supplementary sets
		*/
		fd_set				writeFdsCopy;
		fd_set				readFdsCopy;

		vector<Client>		clients;

		char				buffer[1024];
};
