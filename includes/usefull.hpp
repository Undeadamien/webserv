#ifndef USEFULL_HPP
#define USEFULL_HPP

#include <unistd.h>

#include <algorithm>
#include <cstring>
#include <iostream>
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
bool check_options(int argc, char **argv);
bool filename_parser(const std::string &filename);
bool check_arguments(int argc, char **argv);

#endif
