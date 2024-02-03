#include "../include/Utils.hpp"
#include "../include/Exception.hpp"

#include <string>
#include <iostream>
#include <vector>

using namespace std;
#define VECT_STR std::vector<std::string>

int	Utils::ft_stoi(const std::string& str)
{
	int		result = 0;
	int		sign = 1;
	int		i = 0;

	if (str[i] == '-')
	{
		sign = -1;
		i++;
	}
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
		{
			throw Exception("Invalid Port Conversion");
		}
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result * sign);
}


string	Utils::ft_trim(const string& str, const string &delims){
	size_t first = str.find_first_not_of(delims);
	size_t last = str.find_last_not_of(delims);
	return str.substr(first, (last - first + 1));
}

void	Utils::clearBuffer(char *buffer, int size)
{
	for (int i = 0; i < size; i++)
	{
		buffer[i] = '\0';
	}
}

VECT_STR	Utils::ft_split(const string& str, const string &delims){
	VECT_STR	result;
	size_t		start = 0;
	size_t		end = 0;

	while (end != string::npos)
	{
		start = str.find_first_not_of(delims, end);
		if (start == string::npos)
			break ;
		end = str.find_first_of(delims, start);
		result.push_back(str.substr(start, end - start));
	}
	return (result);
}


bool	Utils::isEqualNonSensitive(const string& str1, const string& str2)
{
	if (str1.size() != str2.size())
		return false;
	for (size_t i = 0; i < str1.size(); i++)
	{
		if (tolower(str1[i]) != tolower(str2[i]))
			return false;
	}
	return true;
}

void	Utils::getTime()
{
	time_t now = time(0);
    tm *ltm = localtime(&now);
    if (ltm->tm_hour < 10)
        std::cout << "0" << ltm->tm_hour << ":" << ltm->tm_min << ":" << ltm->tm_sec << std::endl;
    else if (ltm->tm_min < 10)
        std::cout << ltm->tm_hour << ":" << "0" << ltm->tm_min << ":" << ltm->tm_sec << std::endl;
    else if (ltm->tm_sec < 10)
        std::cout <<  ltm->tm_hour << ":" << ltm->tm_min << ":" << "0" << ltm->tm_sec << std::endl;
    else 
        std::cout << ltm->tm_hour << ":" << ltm->tm_min << ":" << ltm->tm_sec << std::endl;
}