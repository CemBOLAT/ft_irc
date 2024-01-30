#include "../includes/Server.hpp"
#include "../includes/Utils.hpp"
#include "../includes/Client.hpp"
#include <iostream>
#include <stdexcept>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <poll.h>

using namespace std;

static string ip = "127.0.0.1";

Server::Server(char *port, char *password) : serverSocket(-1), nfds(1){
	try {
		this->port = stoi(port);
		this->password = stoi(password);
	}
	catch (const invalid_argument &e) {
		throw invalid_argument("Invalid argument !");
		//cout << "Invalid argument !" << e.what() << endl;
	}
	catch (const out_of_range &e) {
		throw out_of_range("Out of range !");
		//cout << "Out of range" << endl;
	}
}

int	Server::socketCreate(sockaddr_in &serverAddress) const {
	// SOCK_STREAM: TCP
	// AF_INET: IPv4
	int	_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (_socket == -1) {
		throw runtime_error("Socket creation failed !");
	}

	memset(&serverAddress, 0, sizeof(serverAddress)); // fill with zeros because of padding
	serverAddress.sin_family = AF_INET; 
	serverAddress.sin_port = htons(this->port);
	serverAddress.sin_addr.s_addr = INADDR_ANY; // INADDR_ANY: bind to all available interfaces

	/*
 	* Bind a name to a socket.
 	* When a socket is created with socket(2), it exists in a name space (address family) but has no address assigned to it.
 	* bind() assigns the address specified by addr to the socket referred to by the file descriptor sockfd.
 	* addrlen specifies the size, in bytes, of the address structure pointed to by addr.
 	*/
	if (bind(_socket, (sockaddr *)&serverAddress, sizeof(serverAddress)) == -1) {
		close(_socket); // close socket
		throw runtime_error("Socket binding failed !");
	}
	/*
 	* Maximum queue length specifiable by listen.
 	*/
 	if (listen(_socket, SOMAXCONN) == -1) {
 		close(_socket); // close socket
 		throw runtime_error("Socket listening failed !");
 	}
	return (_socket);
}

void	Server::run() {

	this->serverSocket = socketCreate(this->serverAddress);
	cout << "Server is listening on port " << this->port << endl;
	struct pollfd a;
	a.fd = this->serverSocket;
	a.events = POLLIN;
	fds.push_back(a);
	while (true) {
		for (size_t i = 0; i < clientList.size(); i++){
			fds[nfds].fd = clientList[i]->getSocket();
			fds[nfds].events = POLLIN;
			nfds++;
		}
		int ret = poll(&(fds[0]), nfds, -1);
		if (ret == -1) {
			throw runtime_error("Poll failed !");
		}
		if (ret == 0) {
			continue ;
		}
		if (fds[0].revents & POLLIN) {
			int clientSocket = accept(this->serverSocket, NULL, NULL);
			if (clientSocket == -1) {
				throw runtime_error("Socket accepting failed !");
			}
			cout << "New client connected !" << endl;
            this->clientList.push_back(new Client(clientSocket, this->port));
            fds[nfds].fd = clientSocket;
            fds[nfds].events = POLLIN;
            ++nfds;
		}
		for (int i = 1; i < nfds; i++) {
			if (fds[i].revents & POLLIN) {
				char buffer[1024];
				int ret = recv(fds[i].fd, buffer, 1024, 0);
				if (ret == -1) {
					throw runtime_error("Socket receiving failed !");
				}
				if (ret == 0) {
					close(fds[i].fd);
					fds[i].fd = -1;
					continue ;
				}
				buffer[ret] = '\0';
				string msg = buffer;
				vector<string> commands = Utils::split(msg, ' ');
				if (commands.size() > 0)
					functionGenerator(commands, fds[i].fd);
				//for (size_t i = 0; i < clientList.size(); i++)
				//{
				//	if (clientList[i]->getSocket() == fds[i].fd)
				//	{
				//		clientList[i]->sendMessage(msg, fds[i].fd);
				//	}
				//}
			}
		}
	}
	close(this->serverSocket); // close socket
}


Server::~Server() {
	for (vector<Client *>::iterator it = this->clientList.begin(); it != this->clientList.end(); it++) {
		delete *it;
	}
	close(this->serverSocket); // close socket
}

void	Server::functionGenerator(const vector<string> &command, int fd){
	if (command[0] == "PASS"){ // true olduğunda pass döndürme
		if (command.size() == 2)
			this->pass(command, fd);
		else
			sendMessage("530 Incorrect password\n", fd);
	}
	else if (command[0] == "JOIN") {
		if (getClientBySocket(fd)->getPassword() == true)
			this->join(command, fd);
		else
			sendMessage("530 Please login with USER and PASS\n", fd);
	}
}

void	Server::pass(const vector<string> &commands, int fd){
	if (commands[1] == to_string(this->password)){
		sendMessage("230 Password ok, continue\n", fd);
		getClientBySocket(fd)->setPassword(true);
	}
	else{
		sendMessage("530 Incorrect password\n", fd);
	}
}

void	Server::sendMessage(const string &msg, int socket){
	for (size_t i = 0; i < clientList.size(); i++)
	{
		if (clientList[i]->getSocket() == socket)
		{
			clientList[i]->sendMessage(msg, socket);
		}
	}
}

Client	*Server::getClientBySocket(int socket){
	for (size_t i = 0; i < clientList.size(); i++)
	{
		if (clientList[i]->getSocket() == socket)
		{
			return (clientList[i]);
		}
	}
	return (NULL);
}

void	Server::join(const vector<string> &coms, int fd){
	if (coms.size() == 2){
		string chanelName = coms[1];
		for (size_t i = 0; i < roomList.size(); i++)
		{
			if (roomList[i]->getName() == chanelName){
				roomList[i]->addClient(getClientBySocket(fd));
				sendMessage("correct chanel\n", fd);
				return ;
			}
		}
		roomList.push_back(new Room(getClientBySocket(fd), chanelName));
		roomList[roomList.size() - 1]->addClient(getClientBySocket(fd));
		sendMessage("correct chanel\n", fd);
	}
	else{
		sendMessage("530 Incorrect channel name\n", fd);
	}
}