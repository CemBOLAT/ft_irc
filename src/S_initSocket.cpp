#include "Server.hpp"
#include "Exception.hpp"
#include "Utils.hpp"
#include "TextEngine.hpp"
#include "Client.hpp"

#include "Room.hpp"
#include <string>
#include <iostream>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/select.h>
#include "Define.hpp"
#include <fcntl.h>

void Server::initSocket()
{
	this->_socket = socket(AF_INET, SOCK_STREAM, 0); // Create socket 0 for macos and IPPROTO_IP for linux ipv4
	if (_socket < 0)
	{
		throw Exception("Socket creation failed");
	}
	else
	{
		TextEngine::green("Socket created successfully! ", TextEngine::printTime(cout)) << std::endl;
	}
	int dumb = 1;
	if (setsockopt(this->_socket, SOL_SOCKET, SO_REUSEADDR, &dumb, sizeof(int)) < 0) //soket ayarlarları için kullanılır SO_REUSEADDR soketin aynı portu kullanmasına izin verir SOL_SOCKET soket seviyesinde ayarlar
	{
		throw Exception("Socket option failed");
	}
	else
	{
		TextEngine::green("Socket option set successfully! ", TextEngine::printTime(cout)) << std::endl;
	}
	if (fcntl(this->_socket, F_SETFL, O_NONBLOCK) < 0) // soketin bloklanmaması için kullanılır
	{
		throw Exception("Socket fcntl failed on Server");
	}
	else
	{
		TextEngine::green("Socket fcntl set successfully! ", TextEngine::printTime(cout)) << std::endl;
	}

	memset(&address, 0, sizeof(address));  // Clear the structure
	address.sin_family = AF_INET;		  // Set the address family ıpv4
	address.sin_addr.s_addr = INADDR_ANY; // Set the address
	address.sin_port = htons(this->port); // endianness big little endian conversion

	if (::bind(this->_socket, (struct sockaddr *)&address, sizeof(address)) < 0) // bind the socket to the address and port number specified in addr
	{
		throw Exception("Socket bind failed");
	}
	else
	{
		TextEngine::green("Socket binded successfully! ", TextEngine::printTime(cout)) << std::endl;
	}

	/*
	 * Maximum queue length specifiable by listen.
	*/
	if (listen(this->_socket, SOMAXCONN) < 0) //dinleme başlatır maks 128 bağlantıyı kabul eder
	{
		throw Exception("Socket listen failed");
	}
	else
	{
		TextEngine::green("Socket listening successfully! ", TextEngine::printTime(cout)) << 	std::endl;
	 }
}