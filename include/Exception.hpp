#pragma once

#include <stdexcept>
#include <string>
#include <vector>
using namespace std;

class Exception : public std::exception
{
	public:
		Exception(const std::string& message) : message(message) {}
		virtual const char* what() const throw()
		{
			return message.c_str();
		}
		virtual ~Exception() throw() {}
	private:
		std::string message;
};
