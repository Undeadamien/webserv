#ifndef USEFULL_HPP
#define USEFULL_HPP

#include <unistd.h>

#include <algorithm>
#include <cstring>
#include <iostream>
#include <sstream>
#include <vector>

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

std::string ullToStr(unsigned long long ull);
std::string ltrim(std::string str, std::string set);
std::string rtrim(std::string str, std::string set);
std::string trim(std::string str, std::string set);
bool EmptyFileChecker();
std::vector<std::string> split(const std::string &str, char delimiter);
std::string intToString(int value);
std::string UIntToString(unsigned int value);

#endif
