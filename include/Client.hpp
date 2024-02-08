#pragma once

#include <string>
#include <vector>
#include <netinet/in.h>
#include <mutex>

using std::string;
using std::vector;

// #include in.h

class Client
{
public:
	Client(int fd, int port);
	Client(const Client &other);
	Client &operator=(const Client &other);

	int getFd() const;
	int getPort() const;
	string getBuffer() const;
	bool getIsRegistered() const;
	bool getIsPassworded() const;
	int getType() const;

	vector<string>  &getmesagesFromServer();
	string		  getNick() const;
	string		  getUserName() const;
	string		  getRealName() const;
	string		  getHostName() const;
	string		  getServerName() const;

	void setHostName(const string &hostName);
	void setServerName(const string &serverName);
	void setNick(const string &nick);
	void setRegistered(bool val);
	void setPassworded(bool val);
	void setBuffer(const string &str);
	void setType(int type);
	//static std::lock_guard<std::mutex> &getLock() { return lock; }
	void setUserName(const string &userName);
	void setRealName(const string &realName);
	string getUserByHexChat() const;
	virtual ~Client();
	char _ip[INET_ADDRSTRLEN]; // 123.123.123.123 + \0
private:
	int _type; // 1:hex 2:nc 3:bot
	int _fd;
	int _port;
	string buffer;
	vector<string> _messagesFromServer;
	bool isRegistered;
	bool isPassworded;
	string _nick;
	string _userName;
	string _hostName;
	string _serverName;
	string _realName;

};
