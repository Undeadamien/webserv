#include "Arguments.hpp"
#include "ConfParser.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "Server.hpp"
#include "colors.hpp"
#include "usefull.hpp"

std::string get_file_content(std::string path)
{
	(void)path;
	std::ifstream inFile(path.c_str());
	if (!inFile)
		throw std::exception();
	std::string content;
	std::string line;
	while (std::getline(inFile, line)) content += line;
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
		std::cout << INVERSE << "Iteration number: " << iteration++ << RESET
				  << std::endl;
		std::cout << "Listening on Port: " << PORT << std::endl;
		if ((connection = accept(fdSocket, (struct sockaddr *)(&address),
								 (socklen_t *)&resultLen)) == -1)
		{
			std::cerr << "Error: accept() function\n";
			return (EXIT_FAILURE);
		}

		bzero(buffer, sizeof(buffer));	// clear the buffer between each request
		read(connection, buffer, BUFFER_SIZE);

		std::cout << "\n"
				  << UNDERLINE << "Request" << RESET << "\n"
				  << "\"" << buffer << "\"" << std::endl;
		try
		{
			Request request(buffer);
			std::cout << request.getBody() << std::endl;
		}
		catch (std::exception &e)
		{
			std::cerr << e.what() << std::endl;
			exit(1);
		};

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

		std::string response =
			"HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " +
			contentLength;
		response += "\n\n";	 // the content of the response must be separated by
							 // two new-line
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

void testResponse()
{
	std::map<std::string, std::string> headers;
	headers["Content"] = "Hello";
	headers["Language"] = "French";

	Response response;
	response.setProtocol("HTTP");
	response.setStatusCode(200);
	response.setStatusText("OK");
	response.setHeaders(headers);
	response.setBody("test");

	std::cout << response << std::endl;
	std::cout << std::endl;
	std::cout << response.toString() << std::endl;
}

void testRequest()
{
	std::string str =
		"POST /help.txt HTTP/1.1\n"
		"Host: www.example.re\n"
		"Content-Type: text/plain\n"
		"Content-Length: 51\n"
		"\n"
		"Please visit www.example.re for the latest updates!";
	Request request(str);

	std::cout << request << std::endl;
	std::cout << std::endl;
	std::cout << request.toString() << std::endl;
}
