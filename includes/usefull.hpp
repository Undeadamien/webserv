#ifndef USEFULL_HPP
# define USEFULL_HPP
# include "mime.hpp"
# define RED "\x1B[31m"
# define GREEN "\x1B[32m"
# define YELLOW "\x1B[33m"
# define ORANGE "\x1B[34m"
# define PURPLE "\x1B[35m"
# define BLUE "\x1B[36m"
# define BOLD "\x1B[1m"
# define GRAY "\x1B[90m"
# define END "\x1B[0m"
# define ERR -1
# define OK 0
# define LOG 0
# define DIRECTORY 0
# define REGFILE 1
# define BAD_METHOD -1
# define GET 0
# define POST 1
# define DELETE 2
# define HEAD 3
# define OPTIONS 4

// librairies
# include <iostream>
# include <sstream>
# include <fstream>
# include <ostream>
# include <iomanip>

# include <vector>
# include <map>
# include <string>
# include <algorithm>

# include <limits>
# include <utility>

# include <string.h>
# include <stdio.h>
# include <unistd.h>
# include <time.h>
# include <fcntl.h>
# include <errno.h>

# include <cstdlib>
# include <cstdio>
# include <cstring>

# include <arpa/inet.h>
# include <netinet/in.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/epoll.h>
# include <sys/socket.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <dirent.h>
# include <filesystem>

typedef std::vector<std::string>					vecStr;
typedef std::map<std::string, std::string>			mapStr;
typedef std::map<int, std::string>					mapErr;
typedef	std::vector<int>							vecInt;
typedef	std::map<int, int>							mapSock;
typedef std::vector< std::pair<std::string, int> >	vecFiles;
typedef std::pair<std::string, std::string> 		pairStr;
typedef struct dirent fileInfo;
typedef DIR	directory;

template<typename T> std::ostream	&operator<<(std::ostream &o, std::vector<T> vect)
{
	for (size_t i = 0; i + 1 < vect.size(); i++)
		o << vect[i] << " ";
	if (vect.size() > 0)
		o << vect[vect.size() - 1];
	return (o);
}

std::ostream	&operator<<(std::ostream &o, mapErr map);

static const int			nbMethods = 5;
static const std::string	methods[nbMethods] = {"GET", "POST", "DELETE", "HEAD", "OPTIONS"};

static const int			nbError = 5;

static const std::string	errorNb[nbError] = {
	"400",
	"403",
	"404",
	"405",
	"413"
};

static const std::string	errorMsg[nbError] = {
	"Bad Request",
	"Forbiden",
	"Page Not Found",
	"Method Not Allowed",
	"Request Entity Too Large"
};

static const std::string	errorExplanation[nbError] = {
	"The server cannot process the request due to something<br>that is perceived to be a client error (e.g., malformed request syntax,<br>invalid request message framing, or deceptive request routing).",
	"You don't have permission to access<br>this resource.",
	"The page you are looking for might have been removed,<br>	had its name changed or is temporarily unavailable.",
	"The method received in the request-line is not<br>	supported by the target resource.",
	"The body of your post request is too large."
};
