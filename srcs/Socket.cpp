#include "Socket.hpp"

#include <netinet/in.h>

Socket::Socket(){};
Socket::Socket(const Socket &other)
{
    this->_fd = other._fd;
    this->_address = other._address;
};
Socket &Socket::operator=(const Socket &other)
{
    if (this != &other)
    {
        this->_fd = other._fd;
        this->_address = other._address;
    }
    return (*this);
};
Socket::~Socket(){};

void Socket::setFd(int value) { this->_fd = value; };
void Socket::setAddress(sockaddr_in address) { this->_address = address; };

int Socket::getFd() { return (this->_fd); };
sockaddr_in Socket::getAddress() { return (this->_address); };
