#pragma once

#include <string>
#include <netinet/in.h>

using std::string;

//#include in.h

class Client {
    public:
        Client(int fd, int port) : _fd(fd), _port(port) , isRegistered(false), isPassworded(false) {}
        Client() : _fd(-1), _port(-1) , isRegistered(false), isPassworded(false) {}
        Client(const Client &other) : _fd(other._fd), _port(other._port) {}
        Client &operator=(const Client &other) {
            if (this != &other) {
                _fd = other._fd;
                _port = other._port;
            }
            return *this;
        }


        int         &getFd() { return _fd; }
        int         &getPort() { return _port; }
        string      &getBuffer() { return buffer; }
        bool        &getIsRegistered() { return isRegistered; }
        bool        &getIsPassworded() { return isPassworded; }

        void        setRegistered(bool val) { isRegistered = val; }
        void        setPassworded(bool val) { isPassworded = val; }
        void        setBuffer(const string &str) { buffer = str; }
        
        virtual	~Client() {}
        char    _ip[INET_ADDRSTRLEN]; // 123.123.123.123 + \0
    private:
        int     _fd;
        int     _port;
        string  buffer;
        bool    isRegistered;
        bool    isPassworded;
};