#include "../includes/Client.hpp"
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
Client::Client(int socket, int port) : socket(socket), port(port)
{}

Client::~Client()
{
	close(this->socket);
}

string	Client::getWrittenName() const
{
	string res = this->getNickname();

	if (!(username.empty()))
		res += "!" + this->getUsername();
	if (!(hostname.empty()))
		res += "@" + this->getHostname();
	return (res);
}

void	Client::sendMessage(const string &msg, int socket)
{
	send(socket, msg.c_str(), msg.length(), 0); // maybe change it 
}
