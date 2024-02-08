#include "../include/Server.hpp"
#include "../include/Exception.hpp"
#include "../include/Utils.hpp"
#include "../include/TextEngine.hpp"
#include "../include/Client.hpp"
#include "../include/Executor.hpp"
#include "../include/Room.hpp"
#include <string>
#include <iostream>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/select.h>
#include "../include/Define.hpp"
#include <fcntl.h>
#include "Bot.hpp"

#define RPL_ENTRY(password, nick, user) "CAP BOT\nPASS" + password + "\nNICK " + nick + "\nUSER " + user + " 0 * :turco\n"


Bot::Bot(int port, std::string password) : _port(port), _password(password)
{
	this->_name = "IRC_BOT";
	this->initSocket();
}

void Bot::initSocket()
{
	this->_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_IP); // Create socket 0 for macos
	// AF_INE IPV4
	// SOCK_STREAM TCP //SOCK_DGRAM UDP
	// TCP: Transmission Control Protocol : Veri paketlerinin karşı tarafa ulaşmasını garanti eder. Yavaştır.
	// UDP: User Datagram Protocol : Veri paketlerinin karşı tarafa ulaşmasını garanti etmez. Hızlıdır.
	if (_socket < 0)
	{
		throw Exception("Socket creation failed");
	}
	else
	{
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
	}
	else
	{
		TextEngine::green("Socket option set successfully", cout) << std::endl;
	}
	fcntl(this->_socket, F_SETFL, O_NONBLOCK); // Set socket to non-blocking
	memset(&address, 0, sizeof(address));	   // Zeroing address
	address.sin_family = AF_INET;			   // IPv4
	address.sin_addr.s_addr = INADDR_ANY;	   // TCP
	address.sin_port = htons(this->port);	   // ENDIANNESS

	// Big Endinian : 1 2 3 4 5
	// Little Endian : 5 4 3 2 1

	// htons: Host to network short
	// bind socket to port
	if (connect(this->_socket, (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		throw Exception("Connection failed");
	}
	else
	{
		TextEngine::green("Connection established", cout) << std::endl;
	}
}

#define MESSAGE "PRIVMSG #test :Hello world\n"
#define BOT_WELCOME(nick, msg) "PRIVMSG " + nick + " :" + msg + "\n"

void	Server::run(){
	int	forOnce = 1;
	while (true){
		fd_set	readfds;
		FD_ZERO(&readfds);
		FD_SET(this->_socket, &readfds);
		struct timeval timeout; // Set timeout because select is blocking and we want to check if the socket is ready to read
		timeout.tv_sec = 0;
		timeout.tv_usec = 100000;

		int ret = select(this->_socket + 1, &readfds, NULL, NULL, &timeout);

		if (ret < 0){
			throw Exception("Select failed");
		}
		else if (ret == 0){
			continue;
		}
		else {
			int bytesRead = read(this->_socket, this->_buffer, BUFFER_SIZE);
			if (bytesRead < 0){
				throw Exception("Read failed");
			}
			else if (bytesRead == 0){
				continue;
			}
			else {
				_buffer[bytesRead] = '\0';
				vector<string> messages = Utils::ft_split(_buffer, " ");
				std::cout << _buffer << std::endl;
				if (forOnce){
					Utils::instaWrite(this->_socket, RPL_ENTRY(this->_password, this->_name, "user"));
					forOnce = 0;
				}
				else if (messages[0] == "turco" && messages[0] == "turco"){
					Utils::instaWrite(this->_socket, BOT_WELCOME(messages[1], "Welcome to the server"));
				}
			}
		}
	}
	close(this->_socket);
}
