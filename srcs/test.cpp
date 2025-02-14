#include <netinet/in.h>
#include <sys/socket.h>

#include <cerrno>
#include <csignal>
#include <cstdlib>
#include <fstream>
#include <map>

#include "Request.hpp"
#include "Response.hpp"
#include "colors.hpp"
#include "usefull.hpp"

int fdSocket = -1;

// could delete this signal handler and use a destructor
void handle_signal(int signal)
{
	if (signal != SIGINT) return;
	std::cout << "\rStopping the server " << std::endl;
	if (fdSocket != -1) close(fdSocket);
	exit(EXIT_SUCCESS);
}

std::string get_file_content(std::string path)
{
	std::ifstream inFile(path.c_str());
	if (!inFile) throw std::runtime_error("Error: opening '" + path + "'");
	std::string content;
	std::string line;
	while (std::getline(inFile, line)) content += line;
	return (content);
};

std::string get_content_type(std::string target)
{
	std::string::size_type start = target.find_last_of(".") + 1;
	if (start == std::string::npos) return "application/octet-stream";
	std::string extension = target.substr(start);
	if (extension == "css") return "text/css";
	if (extension == "html") return "text/html";
	if (extension == "jpeg") return "image/jpeg";
	if (extension == "jpg") return "image/jpeg";
	if (extension == "js") return "application/javascript";
	if (extension == "json") return "application/json";
	if (extension == "png") return "image/png";
	return "application/octet-stream";
}

int simple_server()
{
	signal(SIGINT, handle_signal);
	if ((fdSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		std::cerr << "Error: socket" << std::endl;
		return (EXIT_FAILURE);
	}
	sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_port = htons(PORT);
	address.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(fdSocket, (const sockaddr *)(&address), sizeof(address)) == -1)
	{
		close(fdSocket);
		std::cerr << "Error: bind, error num " << errno << std::endl;
		return (EXIT_FAILURE);
	}
	if (listen(fdSocket, 10) == -1)
	{
		close(fdSocket);
		std::cerr << "Error: listen" << std::endl;
		return (EXIT_FAILURE);
	}

	int connection;
	int it = 0;
	char buffer[BUFFER_SIZE];
	while (true)
	{
		unsigned long resultLen = sizeof(sockaddr);
		std::cout << INVERSE << "Request number: " << it++ << RESET
				  << std::endl;
		std::cout << "Listening on Port: " << PORT << std::endl;
		connection = accept(fdSocket, (struct sockaddr *)(&address),
							(socklen_t *)&resultLen);
		if ((connection) == -1)
		{
			close(fdSocket);
			std::cerr << "Error: accept" << std::endl;
			return (EXIT_FAILURE);
		}

		bzero(buffer, sizeof(buffer));	// clear the buffer between each request
		read(connection, buffer, BUFFER_SIZE);
		std::cout << "\n"
				  << UNDERLINE << "Request" << RESET << "\n"
				  << "\"" << buffer << "\"" << std::endl;

		Request request;
		try
		{
			request = Request(buffer);
			std::cout << request.getBody() << std::endl;
		}
		catch (std::exception &e)
		{
			std::cerr << e.what() << std::endl;
			exit(1);
		};

		std::string content;
		std::string contentLength;
		std::string target = request.getTarget();
		std::string root = "web";
		if (target == "/") target = "/index.html";

		Response response;
		try
		{
			content = get_file_content(root + target);
			contentLength = itos(content.length());

			std::map<std::string, std::string> headers;
			headers["Content-Type"] = get_content_type(target);
			headers["Content-Length"] = contentLength;

			response.setProtocol("HTTP/1.1");
			response.setStatusCode(200);
			response.setStatusText("OK");
			response.setHeaders(headers);
			response.setBody(content);
		}
		catch (std::exception &e)
		{
			std::cerr << e.what() << "\n";
			content = "Ressource not found";

			std::map<std::string, std::string> headers;
			headers["Content-Type"] = "text/html";
			headers["Content-Length"] = itos(content.length());

			response.setProtocol("HTTP/1.1");
			response.setStatusCode(404);
			response.setStatusText("NOK");
			response.setHeaders(headers);
			response.setBody(content);
		}

		// need to change this to Response
		std::string response_str = response.toString();
		std::cout << UNDERLINE << "Response" << RESET << "\n"
				  << response.toString() << "\n"
				  << std::endl;
		send(connection, response_str.c_str(), response_str.size(), 0);
		close(connection);
	}
	close(fdSocket);
	return (EXIT_SUCCESS);
}

void testResponse()
{
	std::string content = "Body of test response";

	std::map<std::string, std::string> headers;
	headers["Content-Type"] = "text/html";
	headers["Content-Length"] = itos(content.length());

	Response response;
	response.setProtocol("HTTP/1.1");
	response.setStatusCode(200);
	response.setStatusText("OK");
	response.setHeaders(headers);
	response.setBody(content);

	std::cout << response << std::endl;
	std::cout << std::endl;
	std::cout << response.toString() << std::endl;
}

void testRequest()
{
	std::string str =
		"POST /help.txt HTTP/1.1\r\n"
		"Host: www.example.re\r\n"
		"Content-Type: text/plain\r\n"
		"Content-Length: 51\r\n"
		"\r\n"
		"Please visit www.example.re for the latest updates!";
	Request request(str);

	std::cout << request << std::endl;
	std::cout << std::endl;
	std::cout << request.toString() << std::endl;
}
