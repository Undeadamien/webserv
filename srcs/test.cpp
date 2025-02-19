#include "test.hpp"

#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>

#include <cerrno>
#include <csignal>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <map>
#include <stdexcept>

#include "ConfParser.hpp"
#include "Log.hpp"
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

std::string cgi_execute(std::string path, Request request, char **env)
{
	int fds[2];
	int pid;
	std::string method_str;

	if (pipe(fds) == -1) throw std::runtime_error("Error: pipe failed");

	pid = fork();
	if (pid == -1) throw std::runtime_error("Error: fork failed");
	if (pid == 0)
	{
		close(fds[0]);
		dup2(fds[1], STDOUT_FILENO);
		close(fds[1]);

		(void)request;
		// method_str = e_Methods_to_String(request.getMethod());
		// setenv("REQUEST_METHOD", method_str.c_str(), 1);
		// setenv("SCRIPT_FILENAME", path.c_str(), 1);

		// determine the wrigth programm to call
		std::string prog;
		std::string::size_type start = path.find_last_of(".") + 1;
		if (start == std::string::npos) return "application/octet-stream";
		std::string extension = path.substr(start);
		if (extension == "py") prog = "/usr/bin/python3";
		if (extension == "pl") prog = "/usr/bin/perl";
		if (extension == "sh") prog = "/usr/bin/bash";

		char *argv[] = {
			const_cast<char *>(prog.c_str()),
			const_cast<char *>(path.c_str()),
			NULL,
		};
		execve(prog.c_str(), argv, env);
		exit(1);
	}
	else
	{
		close(fds[1]);
		char buffer[1024];
		std::string output;
		int bytes_read;
		while ((bytes_read = read(fds[0], buffer, sizeof(buffer) - 1)) > 0)
		{
			buffer[bytes_read] = '\0';
			output += buffer;
		}
		close(fds[0]);
		int status;
		waitpid(pid, &status, 0);
		if (output.empty()) throw std::runtime_error("Error: cgi empty output");
		return (output);
	}

	return ("");
}

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

int simple_server(ConfParser parser, char **env)
{
	// mettre dans un for
	(void)parser;
	MapServers _servers = parser.getConfigs();
	MapServers::iterator iter = _servers.begin();

	std::string server_port_str = iter->first;
	int server_port = extractPort(server_port_str);
	if (server_port == -1)
	{
		std::cerr << "Error: invalid server ip" << std::endl;
		return (EXIT_FAILURE);
	}

	BlockServer server = iter->second[0];

	signal(SIGINT, handle_signal);
	if ((fdSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		std::cerr << "Error: socket" << std::endl;
		return (EXIT_FAILURE);
	}
	sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_port = htons(server_port);
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
		std::cout << "Listening on Port: " << server_port << std::endl;
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

		if (parser.CheckerMethod(e_Methods_to_String(request.getMethod())) ==
			false)
		{
			Log::log(Log::ERROR, "Error: invalid method");
			close(connection);
			continue;
		}

		// bien verifier la priorite des indexes

		std::string content;
		std::string contentLength;
		std::string target = request.getTarget();
		std::string root = server.getRoot();
		if (target == "/") target += server.getIndexes()[0];

		// identify if the request is cgi

		// ugly as fuck, sorry :(
		Response response;
		try
		{
			if (target.find("/cgi-bin/") == 0)
			{
				// need to be improved
				std::string uri = root + target;
				size_t start, end;
				start = uri.find("web/cgi-bin/");
				end = uri.rfind("?");
				if (start == std::string::npos || end == std::string::npos)
					throw std::runtime_error("Error: invalid cgi uri");
				std::string script = uri.substr(start, end - 2);

				std::string response_str = cgi_execute(script, request, env);
				std::cout << UNDERLINE << "Response" << RESET << "\n"
						  << response_str << "\n"
						  << std::endl;
				send(connection, response_str.c_str(), response_str.size(), 0);
				close(connection);
			}
			else
			{
				content = get_file_content(root + target);
				contentLength = ft_itos(content.length());

				std::map<std::string, std::string> headers;
				headers["Content-Type"] = get_content_type(target);
				headers["Content-Length"] = contentLength;

				response.setProtocol("HTTP/1.1");
				response.setStatusCode(200);
				response.setStatusText("OK");
				response.setHeaders(headers);
				response.setBody(content);

				std::string response_str = response.toString();
				std::cout << UNDERLINE << "Response" << RESET << "\n"
						  << response.toString() << "\n"
						  << std::endl;
				send(connection, response_str.c_str(), response_str.size(), 0);
				close(connection);
			}
		}
		// need to identify the error an load the appropriate page
		catch (std::exception &e)
		{
			Log::log(Log::ERROR, e.what());	 // change to info?
			content = "Ressource not found";

			std::map<std::string, std::string> headers;
			headers["Content-Type"] = "text/html";
			headers["Content-Length"] = ft_itos(content.length());

			response.setProtocol("HTTP/1.1");
			response.setStatusCode(404);
			response.setStatusText("NOK");
			response.setHeaders(headers);
			response.setBody(content);
		}

		// need to change this to Response
	}
	close(fdSocket);
	return (EXIT_SUCCESS);
}

void testResponse()
{
	std::string content = "Body of test response";

	std::map<std::string, std::string> headers;
	headers["Content-Type"] = "text/html";
	headers["Content-Length"] = ft_itos(content.length());

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
