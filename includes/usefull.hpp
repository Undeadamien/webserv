#ifndef USEFULL_HPP
#define USEFULL_HPP

#include <unistd.h>

#include <algorithm>
#include <cstring>
#include <iostream>
#include <sstream>
#include <vector>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <csignal>

#define PORT 1234
#define BUFFER_SIZE 4096
extern int fdSocket;

class Server;

enum e_Methods
{
	GET,
	POST,
	DELETE,
	UNKNOWN
};

std::string ullToStr(unsigned long long ull);
std::string ft_itos(int value);
std::string ltrim(std::string str, std::string set);
std::string rtrim(std::string str, std::string set);
std::string trim(std::string str, std::string set);
bool EmptyFileChecker();
std::vector<std::string> split(const std::string& str, char delimiter);
std::string intToString(int value);
std::string UIntToString(unsigned int value);
int extractPort(const std::string& server);
std::string extractIp(const std::string& server);
std::string e_Methods_to_String(e_Methods method);
e_Methods str_to_e_Methods(std::string str);
int VerifFatalCallFonc(int ret, std::string msg, bool isFatal = true);

void handle_signal(int signal);

#define REQUEST_FLAGS EPOLLIN | EPOLLRDHUP | EPOLLERR // Quand on attend une requete
#define RESPONSE_FLAGS EPOLLIN | EPOLLRDHUP | EPOLLERR | EPOLLOUT // Quand la reponse est prete, et qu'on a la possibilite d'envoyer quelquer chose sur le socket
#define MAX_EVENTS 100

extern Server* glo_server;

#endif
