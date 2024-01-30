#pragma once

#include <string>
#include <vector>

class Chanel;

using namespace std;

class Client
{
	public:
		Client(int socket, int port);
		~Client();

		int getSocket() const { return socket; }
		int getPort() const { return port; }
		string getNickname() const { return nickname; }
		string getUsername() const { return username; }
		string getRealname() const { return realname; }
		string getHostname() const { return hostname; }
		string getPassword() const { return password; }
		vector<Chanel *> getChanelList() const { return chanelList; }

		void setSocket(int socket) { this->socket = socket; }
		void setPort(int port) { this->port = port; }
		void setNickname(const string &nickname) { this->nickname = nickname; }
		void setUsername(const string &username) { this->username = username; }
		void setRealname(const string &realname) { this->realname = realname; }
		void setHostname(const string &hostname) { this->hostname = hostname; }
		void setPassword(const string &password) { this->password = password; }

		string	getWrittenName() const;

		void	sendMessage(const string &msg, int socket);


	private:
		int socket;
		int port;

		string nickname;
		string username;
		string realname;
		string hostname;
		string password;

		vector<Chanel *> chanelList;

		Client();
		Client(Client const &src);
		Client &operator=(Client const &rhs);
};
