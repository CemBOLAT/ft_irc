#include <iostream>
#include <string>
#include "../include/TextEngine.hpp"
#include "../include/Utils.hpp"

std::ostream &TextEngine::red(const std::string &text, std::ostream &os)
{
	os << "\033[31m" << text << "\033[0m";
	return os;
}
std::ostream &TextEngine::green(const std::string &text, std::ostream &os)
{
	os << "\033[32m" << text << "\033[0m";
	return os;
}
std::ostream &TextEngine::yellow(const std::string &text, std::ostream &os)
{
	os << "\033[33m" << text << "\033[0m";
	return os;
}
std::ostream &TextEngine::blue(const std::string &text, std::ostream &os)
{
	os << "\033[34m" << text << "\033[0m";
	return os;
}
std::ostream &TextEngine::magenta(const std::string &text, std::ostream &os)
{
	os << "\033[35m" << text << "\033[0m";
	return os;
}
std::ostream &TextEngine::cyan(const std::string &text, std::ostream &os)
{
	os << "\033[36m" << text << "\033[0m";
	return os;
}
std::ostream &TextEngine::white(const std::string &text, std::ostream &os)
{
	os << "\033[37m" << text << "\033[0m";
	return os;
}
std::ostream &TextEngine::bold(const std::string &text, std::ostream &os)
{
	os << "\033[1m" << text << "\033[0m";
	return os;
}
std::ostream &TextEngine::underline(const std::string &text, std::ostream &os)
{
	os << "\033[4m" << text << "\033[0m";
	return os;
}
std::ostream &TextEngine::reset(const std::string &text, std::ostream &os)
{
	os << "\033[0m" << text << "\033[0m";
	return os;
}
std::ostream &TextEngine::print(const std::string &text, std::ostream &os)
{
	os << text;
	return os;
}

std::ostream	&TextEngine::printTime(std::ostream &os){
	TextEngine::yellow("[ " + Utils::getTime() + " ] ", os);
	return os;
}