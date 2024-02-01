#include "../include/Server.hpp"
#include "../include/Exception.hpp"
#include "../include/Utils.hpp"
#include "../include/TextEngine.hpp"
#include "../include/Client.hpp"
#include "../include/Executor.hpp"
#include <string>
#include <iostream>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/select.h>

using std::cout;

#define VECT_ITER std::vector<Client>::iterator
#define VECT_STR std::vector<std::string>

Server::Server(const std::string &port, const std::string &password)
	: port(0), password(password)
{
	try
	{
		this->port = Utils::ft_stoi(port);
		if (this->port < 0 || this->port > 65535)
		{
			throw Exception("Invalid port");
		}
		initSocket();
	}
	catch (const Exception &e)
	{
		throw e;
	}
}

Server::~Server(){
	if (this->_socket > 0)
	{
		close(this->_socket);
	}
}

void Server::run()
{
	sockaddr_in clientAddress;
	bool		isReadyToSelect = true;
	int			bytesRead = 0;

	FD_ZERO(&readfds);
	FD_ZERO(&writefds);
	FD_ZERO(&readFdsCopy);
	FD_ZERO(&writeFdsCopy);

	FD_SET(this->_socket, &readfds); // Add socket to readfds set
	FD_SET(this->_socket, &writefds); // Add socket to writefds set
	while (true) {
		while (isReadyToSelect)
		{
			readFdsCopy = readfds;
			writeFdsCopy = writefds;
			/*
				0 - 1 - 2
				3 server socket
				4 is for select function
			*/
			if (select(clients.size() + 4, &readFdsCopy, &writeFdsCopy, nullptr, nullptr) < 0)
			{
				throw Exception("Select failed");
			}
			isReadyToSelect = false;
		}
		if (FD_ISSET(this->_socket, &this->readFdsCopy)){
			// Accept new connection
			socklen_t	templen = sizeof(sockaddr_in);
			int newSocket = accept(this->_socket, (sockaddr *)&clientAddress, &templen);
			if (newSocket < 0)
			{
				throw Exception("Accept failed");
			}
			int	port = ntohs(clientAddress.sin_port);
			Client newClient(newSocket, port);
			inet_ntop(AF_INET, &(clientAddress.sin_addr), newClient._ip, INET_ADDRSTRLEN); // Convert IP to string and save it to newClient
			clients.push_back(newClient);
			FD_SET(newSocket, &readfds);
			FD_SET(newSocket, &writefds);

			TextEngine::green("New connection from ", cout) << newClient._ip << ":" << newClient.getPort() << std::endl;
			isReadyToSelect = true;
			continue;
		}
		for (VECT_ITER a = clients.begin(); a != clients.end() && !isReadyToSelect; a++)
		{
			if (FD_ISSET(a->getFd(), &this->readFdsCopy))
			{
				// Read from socket
				bytesRead = read(a->getFd(), this->buffer, 1024);
				if (bytesRead <= 0){
					FD_CLR(a->getFd(), &readfds);
					FD_CLR(a->getFd(), &writefds);
					close(a->getFd());
					TextEngine::blue("Client ", cout) << a->_ip << ":" << a->getPort() << " disconnected" << std::endl;
					clients.erase(a);
					isReadyToSelect = true;
				}
				else {
					this->buffer[bytesRead] = '\0';
					string msg = this->buffer;
					if (msg == "\n"){
						isReadyToSelect = true;
						break; // Continue to next client if message is empty
					}
					if (msg[msg.length() - 1] != '\n'){
						a->setBuffer(a->getBuffer() + msg);
						isReadyToSelect = true;
						break;
					}
					/*
						komutu ele alacan
					*/
					runCommand(msg, *a);
					//isReadyToSelect = true;
				}
				isReadyToSelect = true;
				break;
			}
			//isReadyToSelect = true;
		}
		for (VECT_ITER a = clients.begin(); a != clients.end() && !isReadyToSelect; a++)
		{
			if (FD_ISSET(a->getFd(), &this->writeFdsCopy))
			{
				// Write to socket
				if (a->getBuffer().length() > 0)
				{
					int bytesWritten = write(a->getFd(), a->getBuffer().c_str(), a->getBuffer().length());
					if (bytesWritten < 0)
					{
						throw Exception("Write failed");
					}
					if (bytesWritten == 0)
					{
						FD_CLR(a->getFd(), &writefds);
						FD_CLR(a->getFd(), &readfds);
						close(a->getFd());
						this->clients.erase(a);
						TextEngine::blue("Client ", cout) << a->_ip << ":" << a->getPort() << " disconnected" << std::endl;
					}
					a->setBuffer(a->getBuffer().substr(bytesWritten));
				}
				Utils::clearBuffer(this->buffer, 1024);
				isReadyToSelect = true;
				break;
			}
		}
	}
}

void Server::initSocket()
{
	this->_socket = socket(AF_INET, SOCK_STREAM, 0);
	// AF_INE IPV4
	// SOCK_STREAM TCP //SOCK_DGRAM UDP
	// TCP: Transmission Control Protocol : Veri paketlerinin karşı tarafa ulaşmasını garanti eder. Yavaştır.
	// UDP: User Datagram Protocol : Veri paketlerinin karşı tarafa ulaşmasını garanti etmez. Hızlıdır.
	if (_socket < 0)
	{
		throw Exception("Socket creation failed");
	} else {
		TextEngine::green("Socket created successfully", cout) << std::endl;
	}
	int opt = 1;
	// setsockopt: Sets socket options
	// SOL_SOCKET: Socket level : Socket options apply to the socket layer
	// SO_REUSEADDR: Reuse address : Allows other sockets to bind() to this port, unless there is an active listening socket bound to the port already
	// &opt: Option value // NULL
	// sizeof(int): Option length // NULL
	if (setsockopt(this->_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int)) < 0)
	{
		throw Exception("Socket option failed");
	} else {
		TextEngine::green("Socket option set successfully", cout) << std::endl;
	}

	memset(&address, 0, sizeof(address)); // Zeroing address
	address.sin_family = AF_INET; // IPv4
	address.sin_addr.s_addr = INADDR_ANY; // TCP
	address.sin_port = htons(this->port); // ENDIANNESS

	// Big Endinian : 1 2 3 4 5
	// Little Endian : 5 4 3 2 1

	// htons: Host to network short
	// bind socket to port 
	if (bind(this->_socket, (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		throw Exception("Socket bind failed");
	} else {
		TextEngine::green("Socket binded successfully", cout) << std::endl;
	}
	
	/*
	* Maximum queue length specifiable by listen.
	*/
	if (listen(this->_socket, SOMAXCONN) < 0)
	{
		throw Exception("Socket listen failed");
	} else {
		TextEngine::green("Socket listening successfully", cout) << std::endl;
	}
}

void Server::runCommand(const std::string &command, Client &client)
{
	string trimmed = Utils::ft_trim(command, " \n\t\r");
	//cout << "trimmed:#" << trimmed << "#" << std::endl;

	VECT_STR  params = Utils::ft_split(trimmed, " \n\t\r");
	for (VECT_STR::iterator a = params.begin(); a != params.end(); a++)
	{
		cout << "param:#" << *a << "#" << std::endl;
	}
	//for (size_t i = 0; i < params.size(); ++i){
	//	string &param = params[i];
	//	
	//}
	(void)client;
}