#ifndef USEFULL_HPP
#define USEFULL_HPP

#include <cstring>
#include <iostream>
#include <unistd.h>
#include <algorithm>
#include <sstream>

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
