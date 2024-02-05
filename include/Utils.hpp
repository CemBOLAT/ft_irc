#pragma once


#define USAGE "./ircserv [port] [password]"

#include <string>
#include <vector>

#define VECT_STR std::vector<std::string>

using std::string;
class Client;

namespace Utils {
	int			ft_stoi(const std::string& str);
	void		clearBuffer(char *buffer, int size);
	std::string	ft_trim(const std::string& str, const std::string &delims);
	VECT_STR	ft_firstWord(const std::string& str);
	VECT_STR	ft_split(const std::string& str, const std::string &delims);
	bool		isEqualNonSensitive(const string& str1, const string& str2);
	void 		getTime();
	void		instaWriteAll(std::vector<Client> clients, std::string message);
	void		instaWrite(int fd, std::string message);

}
