#ifndef USEFULL_HPP
#define USEFULL_HPP

#include <unistd.h>
#include <vector>

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
std::string ltrim(std::string str, std::string set);
std::string rtrim(std::string str, std::string set);
std::string trim(std::string str, std::string set);
bool EmptyFileChecker();
std::vector<std::string> split(const std::string &str, char delimiter);
std::string intToString(int value);
std::string UIntToString(unsigned int value);

#endif
