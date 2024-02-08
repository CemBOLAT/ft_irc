#pragma once

#include <string>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "Define.hpp"
#include "Room.hpp"

#include <unistd.h>

using std::string;
using std::vector;

class Client;
class Room;

class Server
{
public:
	Server(const string &port, const string &password);
	virtual ~Server();
	void	run();
	void	join(C_STR_REF params, Client &client);
	void	part(C_STR_REF params, Client &client);
	void	privmsg(std::string &command, Client &client);
	void	op(C_STR_REF params, Client &client);
	void	mode(C_STR_REF params, Client &client);
	void	nick(C_STR_REF params, Client &client, fd_set &fd);
	void	who(const std::string &command, Client &client);
	void 	topic(const std::string &command, Client &client);
	void	ping(C_STR_REF params, Client &client);
	void	quit(C_STR_REF params, Client &client);
	void	whois(std::string &s, Client &cli);
	Room &getRoom(const string &name){
		vector<Room>::iterator it = this->channels.begin();
		for (; it != this->channels.end(); ++it)
		{
			if (it->getName() == name)
				return *it;
		}
		return *it;
	}

	vector<Room> &getRooms(){
		return this->channels;
	}

	bool isRoom(const string &name){
		vector<Room>::iterator it = this->channels.begin();
		for (; it != this->channels.end(); ++it)
		{
			if (it->getName() == name)
				return true;
		}
		return false;
	}
	void addRoom(const Room &room){
		this->channels.push_back(room);
	}
	void addClient(const Client &client){
		this->clients.push_back(client);
	}

	vector<Client> &getClients(){
		return this->clients;
	}

	void	responseAllClientResponseToGui(Client &client, Room &room);
	bool	isClientInRoom(Room &room, const Client &client){
		vector<Client>::iterator it = room.getClients().begin();
		for (; it != room.getClients().end(); ++it)
		{
			if (it->getNick() == client.getNick())
				return true;
		}
		return false;
	}
	bool	isClientInRoom(Client &client, string &room){
		vector<Room>::iterator it = this->channels.begin();
		for (; it != this->channels.end(); ++it)
		{
			if (it->getName() == room)
			{
				vector<Client>::iterator cit = it->getClients().begin();
				for (; cit != it->getClients().end(); ++cit)
				{
					if (cit->getNick() == client.getNick())
						return true;
				}
			}
		}
		return false;
	}

	void	removeClient(int fd){
		vector<Client>::iterator it = this->clients.begin();
		for (; it != this->clients.end(); ++it)
		{
			if (it->getFd() == fd)
			{
				this->clients.erase(it);
				return ;
			}
		}
	}

	int	getSocket() const{
		return this->_socket;
	}

private:
	Server();
	Server(const Server &other);
	Server &operator=(const Server &other);
	void initSocket();
	void runCommand(const string &command, Client &client);
	void hexChatEntry(VECT_STR &, Client &);

	int port;
	string password;
	int _socket;
	struct sockaddr_in address;
	struct sockaddr_in clientAddress;

	fd_set readfds;
	fd_set writefds;
	/*
		The reason for storing file descriptors in supplementary sets (this->readFdsSup and this->writeFdsSup)
		instead of directly modifying the original sets (this->readFds and this->writeFds) is to avoid race conditions
		and to ensure that the original sets remain unchanged for the next iteration of the loop.


		avoid blocking sockets we need to use select() function
		and copy the original sets to supplementary sets
	*/
	fd_set writeFdsCopy;
	fd_set readFdsCopy;

	vector<Client> clients;
	vector<Room> channels;

	char buffer[1024];
};
