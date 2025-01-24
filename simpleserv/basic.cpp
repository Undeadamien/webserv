#include "colors.hpp"
#include "usefull.hpp"
#include <arpa/inet.h>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <fstream>
#include <iostream>
#include <netinet/in.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>

#ifdef PORT
#undef PORT
#define PORT 1234
#endif

#ifdef BUFFER_SIZE
#undef BUFFER_SIZE
#define BUFFER_SIZE 8192
#endif

std::string get_file_content(std::string path)
{
    (void)path;
    std::ifstream inFile(path.c_str());
    if (!inFile)
        throw std::exception();
    std::string content;
    std::string line;
    while (std::getline(inFile, line))
        content += line;
    return (content);
};

int simple_server()
{
    int fdSocket;
    if ((fdSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        std::cerr << "Error: socket() function\n";
        return (EXIT_FAILURE);
    }

    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);
    address.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(fdSocket, (const sockaddr *)(&address), sizeof(address)) == -1)
    {
        std::cerr << "Error: bind() function\n";
        return (EXIT_FAILURE);
    }

    if (listen(fdSocket, 10) == -1)
    {
        std::cerr << "Error: listen() function\n";
        return (EXIT_FAILURE);
    }

    int connection;
    int iteration = 0;
    char buffer[BUFFER_SIZE];
    while (true)
    {
        unsigned long resultLen = sizeof(sockaddr);
        std::cout << INVERSE << "Iteration number: " << iteration++ << RESET << std::endl;
        std::cout << "Listening on Port: " << PORT
                  << std::endl;
        if ((connection = accept(fdSocket, (struct sockaddr *)(&address), (socklen_t *)&resultLen)) == -1)
        {
            std::cerr << "Error: accept() function\n";
            return (EXIT_FAILURE);
        }

        bzero(buffer, sizeof(buffer)); // clear the buffer between each request
        read(connection, buffer, BUFFER_SIZE);
        std::cout << "\n"
                  << UNDERLINE << "Request" << RESET << "\n"
                  << buffer << std::endl;

        std::string content;
        std::string contentLength;
        try
        {
            content = get_file_content("web/index.html");
            contentLength = itos(content.length());
        }
        catch (std::exception &e)
        {
            std::cerr << "Error: file content loading\n";
            exit(1);
        }

        std::string response = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " + contentLength;
        response += "\n\n"; // the content of the response must be separated by two new-line
        response += content;
        std::cout << UNDERLINE << "Response" << RESET << "\n"
                  << response << "\n"
                  << std::endl;

        send(connection, response.c_str(), response.size(), 0);
        close(connection);
    }

    close(fdSocket);

    return (EXIT_SUCCESS);
}
