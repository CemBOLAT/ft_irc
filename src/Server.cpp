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

using std::cout;

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

Server::~Server()
{
	if (this->_socket > 0)
	{
		close(this->_socket);
	}
}

void Server::run()
{
	sockaddr_in clientAddress;
	socklen_t templen = sizeof(sockaddr_in);
	bool isReadyToSelect = true;
	int bytesRead = 0;

	FD_ZERO(&readfds);
	FD_ZERO(&writefds);
	FD_ZERO(&readFdsCopy);
	FD_ZERO(&writeFdsCopy);

	FD_SET(this->_socket, &readfds);  // Add socket to readfds set
	// bu kısımda fd_set yok çünkü fd_set write açık olunca (sunucu asılı kalıyor) ve select fonksiyonu sürekli 0 dönüyor
	//FD_SET(this->_socket, &writefds); // Add socket to writefds set
	while (true)
	{
		while (isReadyToSelect)
		{
			readFdsCopy = readfds;
			writeFdsCopy = writefds;
			/*
				0 - 1 - 2
				3 server socket
				4 is for select function
			*/
			// nullptr is a C++11 feature
			// select: bir thread içerisinde birden fazla soketin okuma ve yazma işlemlerini takip etmek için kullanılır
			// volatile kullanmak bazen sorunlara sebep oluyor bu sebepten dolayı fdleri kopyalayıp
			// onlar üzerinden işlem yapmak daha mantıklı
			if (select(clients.size() + 4, &readFdsCopy, &writeFdsCopy, NULL, 0) < 0)
			{
				throw Exception("Select failed");
			}
			isReadyToSelect = false; // ne zaman veri okumak veya yazmak için hazır olacağımızı belirler
		}
		if (FD_ISSET(this->_socket, &this->readFdsCopy))
		{
			int newSocket = accept(this->_socket, (sockaddr *)&clientAddress, &templen); // Accept new connection (yeni kişinin fdsi)
			if (newSocket < 0)
			{
				throw Exception("Accept failed");
			}
			int port = ntohs(clientAddress.sin_port); // big endian to little endian
			Client newClient(newSocket, port); // Create new client
			inet_ntop(AF_INET, &(clientAddress.sin_addr), newClient._ip, INET_ADDRSTRLEN); // Convert IP to string and save it to newClient
			clients.push_back(newClient); // Add new client to clients vector
			FD_SET(newSocket, &readfds); // kullanıcın okuma ucu açılır

			TextEngine::green("New connection from ", cout) << newClient._ip << ":" << newClient.getPort() << std::endl;
			isReadyToSelect = true;
			continue;
		}

		// okuma işlemi
		for (VECT_ITER a = clients.begin(); a != clients.end() && !isReadyToSelect; a++)
		{
			if (FD_ISSET(a->getFd(), &this->readFdsCopy))
			{
				// Read from socket
				bytesRead = read(a->getFd(), this->buffer, 1024);
				// entera basınca \r \n hexchat bunu gönderiyor
				// nc de ise sadece \n gönderiyor
				if (bytesRead <= 0)
				{
					for (std::vector<Room>::iterator it = this->channels.begin(); it != this->channels.end(); it++){
						if (it->isClientInChannel(a->getFd())){
							it->removeClient(a->getFd());
						}
					}
					FD_CLR(a->getFd(), &readfds);
					FD_CLR(a->getFd(), &writefds);
					close(a->getFd());
					TextEngine::blue("Client ", cout) << a->_ip << ":" << a->getPort() << " disconnected" << std::endl;
					clients.erase(a);
					isReadyToSelect = true;
				}
				else
				{
					this->buffer[bytesRead] = '\0'; // her zaman sonuna \r \n ekliyor
					string msg = this->buffer;
					if (msg == "\n") // tek seferde çoklu komut yollanırsa
					{
						isReadyToSelect = true;
						break; // Continue to next client if message is empty
					}
					if (msg[msg.length() - 1] != '\n') // tek seferde çoklu komut yollanırsa
					{
						a->setBuffer(a->getBuffer() + msg);
						isReadyToSelect = true;
						break;
					}
					/*
						komutu ele alacan
					*/
					runCommand(msg, *a);
				}
				isReadyToSelect = true;
				break;
			}
			// isReadyToSelect = true;
		}
		// yazma işlemi
		for (VECT_ITER a = clients.begin(); a != clients.end() && !isReadyToSelect; ++a)
		{
			if (FD_ISSET(a->getFd(), &this->writeFdsCopy))
			{
				// bu komutun amacı mesajı gönderdikten sonra mesajı silmek ve diğer mesajı göndermek bunun yerine direkt olarak yollasak nasıl olur?
				int bytesWritten = write(a->getFd(), a->getmesagesFromServer()[0].c_str(), a->getmesagesFromServer()[0].length());
				a->getmesagesFromServer().erase(a->getmesagesFromServer().begin());
				if (bytesWritten < 0)
				{
					throw Exception("Write failed");
				}
				if (a->getmesagesFromServer().empty()){
					FD_CLR(a->getFd(), &writefds); // askıya alınmış bir soketin yazma ucu kapatılır
				}
				if (bytesWritten == 0)
				{
					for (std::vector<Room>::iterator it = this->channels.begin(); it != this->channels.end(); it++){
						if (it->isClientInChannel(a->getFd())){
							it->removeClient(a->getFd()); // kullanıcıyı odadan silmek çünkü ikiside farklı objeler
						}
					}
					FD_CLR(a->getFd(), &writefds); // askıya alınmış bir soketin yazma ucu kapatılır
					FD_CLR(a->getFd(), &readfds); // askıya alınmış bir soketin okuma ucu kapatılır
					close(a->getFd()); // soket kapatılır
					this->clients.erase(a);
					TextEngine::blue("Client ", cout) << a->_ip << ":" << a->getPort() << " disconnected" << std::endl;
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

	memset(&address, 0, sizeof(address)); // Zeroing address
	address.sin_family = AF_INET;		  // IPv4
	address.sin_addr.s_addr = INADDR_ANY; // TCP
	address.sin_port = htons(this->port); // ENDIANNESS

	// Big Endinian : 1 2 3 4 5
	// Little Endian : 5 4 3 2 1

	// htons: Host to network short
	// bind socket to port
	if (bind(this->_socket, (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		throw Exception("Socket bind failed");
	}
	else
	{
		TextEngine::green("Socket binded successfully", cout) << std::endl;
	}

	/*
	 * Maximum queue length specifiable by listen.
	 */
	if (listen(this->_socket, SOMAXCONN) < 0)
	{
		throw Exception("Socket listen failed");
	}
	else
	{
		TextEngine::green("Socket listening successfully", cout) << std::endl;
	}
}

void Server::runCommand(const std::string &command, Client &client)
{
	string trimmed = Utils::ft_trim(command, " \r"); // bakacam buraya
	cout << trimmed << "#" << std::endl;
	/*
		CAP LS 302
		PASS 123
		NICK asd
		USER asd asd asd :asd
		#
	*/

	VECT_STR softSplit = Utils::ft_split(trimmed, "\n");
	// iki splitin amacı \n ile gelen mesajları parçalamak
	for (size_t i = 0; i < softSplit.size(); i++)
	{
		string trimmedLine = Utils::ft_trim(softSplit[i], " \t\r");
		if (trimmedLine.empty()) return;
		VECT_STR splitFirst = Utils::ft_firstWord(trimmedLine); // kelimeyi ayırır komut ve parametreleri
		if (splitFirst.size() <= 1) return;
		if (Utils::isEqualNonSensitive(splitFirst[0], "pass"))
		{
			Executor::pass(splitFirst[1], client, this->password, writefds); // doğru
		}
		else if (Utils::isEqualNonSensitive(splitFirst[0], "cap"))
		{
			Executor::cap(splitFirst[1], client); // doğru
		}
		else if (client.getIsPassworded() == false){
			FD_SET(client.getFd(), &writefds);
			client.getmesagesFromServer().push_back("First you need to pass the password\n\r");
		}
		else if (Utils::isEqualNonSensitive(splitFirst[0], "user"))
		{
			Executor::user(splitFirst[1], client, writefds); // doğru
		}
		else if (Utils::isEqualNonSensitive(splitFirst[0], "nick"))
		{
			nick(splitFirst[1], client, writefds); // doğru
		}
		else if (client.getIsRegistered() == false){
			FD_SET(client.getFd(), &writefds);
			client.getmesagesFromServer().push_back("First you need to register\n\r");
		}
		else if (Utils::isEqualNonSensitive(splitFirst[0], "join"))
		{
			this->join(splitFirst[1], client); // doğru
		}
		else if (Utils::isEqualNonSensitive(splitFirst[0], "part"))
		{
			this->part(splitFirst[1], client); // doğru
		}
		else if (Utils::isEqualNonSensitive(splitFirst[0], "op"))
		{
			this->op(splitFirst[1], client);
		}
		else if (Utils::isEqualNonSensitive(splitFirst[0], "mode")){
			this->mode(splitFirst[1], client);
		}
		else if (Utils::isEqualNonSensitive(splitFirst[0], "ping")){
			this->ping(splitFirst[1], client);
		}
		else if (Utils::isEqualNonSensitive(splitFirst[0], "privmsg"))
		{
			this->privmsg(splitFirst[1], client); // doğru
		}
		else if (Utils::isEqualNonSensitive(splitFirst[0], "who"))
		{
			this->who(splitFirst[1], client);
		}
		else if (Utils::isEqualNonSensitive(splitFirst[0], "topic"))
		{
			this->topic(splitFirst[1], client); // doğru
		}
		else if (Utils::isEqualNonSensitive(splitFirst[0], "quit"))
		{
			this->quit(splitFirst[1], client); // bozuk
		}
		else if (Utils::isEqualNonSensitive(splitFirst[0], "whois"))
		{
			this->whois(splitFirst[1], client); // doğru
		}
		else
		{
			FD_SET(client.getFd(), &writefds);
			client.getmesagesFromServer().push_back("Invalid command\n");
		}
	}
	hexChatEntry(softSplit, client);
}


// çoklu komut geldiği için hexchatten giriş olunca buraya düşüyor
void Server::hexChatEntry(VECT_STR &params, Client &client)
{
	if (params[0] != "CAP" && params.size() != 1)
	{
		if (client.getIsPassworded() == false)
		{
			FD_CLR(client.getFd(), &readfds);
			FD_CLR(client.getFd(), &writefds);
			std::cout << "Invalid password" << std::endl;
			close(client.getFd());
			std::cout << "Client " << client._ip << ":" << client.getPort() << " disconnected" << std::endl;
			this->clients.erase(this->clients.begin() + client.getFd() - 4); // maybe wrong
		}
	}
}

#define RPL_NAMREPLY(nick, channel, users)			": 353 " + nick + " = " + channel + " :" + users + "\r\n"
#define RPL_ENDOFNAMES(nick, channel)				": 366 " + nick + " " + channel + " :End of /NAMES list\r\n"

// oda içinki kullanıcıları gösterir (ve değişim yapar hexchat için)
void Server::responseAllClientResponseToGui(Client &client, Room &room)  {
	string message;
	Room tmp = room;
	if (tmp.getName().empty())
		return;
	for (std::vector<Client>::iterator it = tmp.getClients().begin(); it != tmp.getClients().end(); it++){
		if (it->getFd() == tmp.getOperator()->getFd())
			message += "@";
		message += (*it).getNick() + " ";
	}
	Utils::instaWriteAll(tmp.getClients(), RPL_NAMREPLY(client.getNick(), room.getName(), message));
	Utils::instaWriteAll(tmp.getClients(), RPL_ENDOFNAMES(client.getNick(), room.getName()));
}

