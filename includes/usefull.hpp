#ifndef USEFULL_HPP
#define USEFULL_HPP

#include <cstring>
#include <iostream>
#include <unistd.h>
//#include <netinet/in.h>
#include <sstream> // Ajouté pour utiliser std::stringstream
//#include <sys/socket.h>


#define PORT 1234
#define BUFFER_SIZE 4096

enum e_Methods
{
	GET,
	POST,
	DELETE,
	PUT,
	UNKNOWN
};

std::string itos(int value);

#endif
