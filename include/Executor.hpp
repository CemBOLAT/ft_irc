#pragma once
#include <string>

class Client;
#define VECT_STR std::vector<std::string>

namespace Executor
{
    void password(const VECT_STR &params, Client &client, const std::string &password);
}