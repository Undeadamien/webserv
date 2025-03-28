#ifndef CGI_HANDLER
#define CGI_HANDLER

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <map>
#include <string>

#include "Request.hpp"
#include "usefull.hpp"
#include "Log.hpp"
#include "Server.hpp"


class Log;
class Response;
class Request;
class Server;

#define CGI_TIMEOUT 2 // Timeout en secondes

class CgiHandler
{
    friend class Response;

    private:
        std::string _output;
        std::string _target;
        std::string _cgi_path;
        std::string _cgi_extension;
        bool _isChunked;


    public:
        CgiHandler();
        CgiHandler(const CgiHandler &src);
        ~CgiHandler(void);

        CgiHandler &operator=(const CgiHandler &src);

        /*Getters*/

        Response CgiMaker(const Request *request, const BlockLocation *location, BlockServer *server);

        Response executeCgi(const Request *request, BlockServer *server);

};

#endif
