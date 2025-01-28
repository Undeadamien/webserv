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
bool EmptyFileChecker();
std::string trimLine(const std::string &str);
std::vector<std::string> split(const std::string &str, char delimiter);

#endif
