#ifndef TEXTENGINE_HPP
#define TEXTENGINE_HPP

#include <iostream>

namespace TextEngine
{
	std::ostream &red(const std::string& text, std::ostream &os);
	std::ostream &green(const std::string& text, std::ostream &os);

	std::ostream &yellow(const std::string& text, std::ostream &os);

	std::ostream &blue(const std::string& text, std::ostream &os);

	std::ostream &magenta(const std::string& text, std::ostream &os);

	std::ostream &cyan(const std::string& text, std::ostream &os);

	std::ostream &white(const std::string& text, std::ostream &os);

	std::ostream &bold(const std::string& text, std::ostream &os);

	std::ostream &underline(const std::string& text, std::ostream &os);

	std::ostream &reset(const std::string& text, std::ostream &os);

	std::ostream &print(const std::string& text, std::ostream &os);
}

#endif
