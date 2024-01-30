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
		void functionGenerator(const vector<string> &commands, int fd);

		void	pass(const vector<string> &commands, int fd);
		void	sendMessage(const string &msg, int socket);
		Client	*getClientBySocket(int socket);
		void	join(const vector<string> &commands, int fd);
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
