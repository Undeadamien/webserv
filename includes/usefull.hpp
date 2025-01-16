#ifndef USEFULL_HPP
#define USEFULL_HPP

#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sstream> // Ajouté pour utiliser std::stringstream
#include <sys/socket.h>
#include <unistd.h>

#define PORT 1234
#define BUFFER_SIZE 4096

#define RED "\033[31m"
#define GREEN "\033[32m"

#define INVERSE "\033[7m"
#define RESET "\033[0m"

std::string itos(int value);

#endif
