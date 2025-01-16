#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <netinet/in.h>
class Socket
{
public:
    Socket();
    Socket(const Socket &other);
    Socket &operator=(const Socket &other);
    virtual ~Socket();

    void setFd(int value);
    void setAddress(sockaddr_in address);

    int getFd();
    sockaddr_in getAddress();

protected:
private:
    int _fd;
    sockaddr_in _address;
};

#endif
