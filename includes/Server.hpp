#pragma once

#include <string>
#include "./Room.hpp"
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>

using namespace std;

#define MAX_CLIENTS 10

class Server{
	public:
		Server(char *port, char *password);
		~Server();
		void run();
		int socketCreate(sockaddr_in &serverAddress) const;
	private:
		vector<Room *>			roomList;
		vector<Client *>		clientList;
		static string			ip;
		int						port;
		int						password;
		int						serverSocket;
		sockaddr_in				serverAddress;
		vector<struct pollfd>	fds;
		int						nfds;

		Server();
		Server(const Server &server);
		Server &operator=(const Server &server);

};
