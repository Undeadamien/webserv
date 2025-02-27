#include "test.hpp"

#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>

#include <cerrno>
#include <csignal>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <fstream>
#include <map>
#include <stdexcept>

#include "BlockServer.hpp"
#include "BlockLocation.hpp"
#include "ConfParser.hpp"
#include "Log.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "colors.hpp"
#include "usefull.hpp"


std::string get_content_type(std::string target)
{
	static std::map<std::string, std::string> mapMime;
	if (mapMime.empty())
	{
		mapMime["css"] = "text/css";
		mapMime["html"] = "text/html";
		mapMime["jpeg"] = "image/jpeg";
		mapMime["jpg"] = "image/jpeg";
		mapMime["js"] = "application/javascript";
		mapMime["json"] = "application/json";
		mapMime["png"] = "image/png";
		//...
	}

	std::string::size_type start = target.find_last_of(".") + 1;
	if (start == std::string::npos) return "application/octet-stream";

	std::string extension = target.substr(start);
	std::map<std::string, std::string>::iterator it = mapMime.find(extension);
	if (it != mapMime.end()) return it->second;
	return "application/octet-stream";
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

Response create_response_error(std::string protocol, std::string status_code,
							   std::string status_text)
{
	Response response;
	std::string content;
	std::map<std::string, std::string> headers;

	content += "<html>";
	content += "<body>";
	content += "<p>SpiderServ</p>";
	content += "<p>Error " + status_code + "</p>";
	content += "</body>";
	content += "</html>";

	headers["Content-Type"] = "text/html";
	headers["Content-Length"] = ft_itos(content.length());

	response.setProtocol(protocol);
	response.setStatusCode(status_code);
	response.setStatusText(status_text);
	response.setHeaders(headers);
	response.setBody(content);

	return (response);
};

Response create_response_error(std::string protocol, std::string status_code,
							   std::string status_text, std::string path)
{
	Response response;
	std::string content;
	std::map<std::string, std::string> headers;

	try
	{
		std::string file_content = get_file_content(path);
		if (file_content.empty())
			throw std::runtime_error("Error: empty error page");
		content = file_content;
	}
	catch (std::exception &e)
	{
		Log::log(Log::ERROR, e.what());
		content += "<html>";
		content += "<body>";
		content += "<p>SpiderServ</p>";
		content += "<p>Error " + status_code + "</p>";
		content += "</body>";
		content += "</html>";
	}

	headers["Content-Type"] = "text/html";
	headers["Content-Length"] = ft_itos(content.length());

	response.setProtocol(protocol);
	response.setStatusCode(status_code);
	response.setStatusText(status_text);
	response.setHeaders(headers);
	response.setBody(content);

	return (response);
};

Response create_response_page(Request request, BlockServer server)
{
	Response response;
	std::map<std::string, std::string> headers;
	std::string content;
	std::string contentLength;
	std::string target = request.getTarget();
	std::string root = server.getRoot();

	if (target == "/") target += server.getIndexes()[0];  // need to be adapted

	content = get_file_content(root + target);	// can throw
	contentLength = ft_itos(content.length());

	headers["Content-Type"] = get_content_type(target);
	headers["Content-Length"] = contentLength;

	response.setProtocol("HTTP/1.1");
	response.setStatusCode("200");
	response.setStatusText("OK");
	response.setHeaders(headers);
	response.setBody(content);

	return (response);
};

std::string cgi_execute(const Request &request, const BlockServer &conf_server,
						char **env)
{
	int fds[2];
	int pid;
	std::string method_str;
	size_t start, end;
	std::string target;	 // might change the name
	std::string path;

	target = conf_server.getRoot() + request.getTarget();
	start = target.find("web/cgi-bin/");
	end = target.rfind("?");
	if (start == std::string::npos || end == std::string::npos)
		throw std::runtime_error("Error: invalid cgi script path ?");
	path = target.substr(start, end - 2);

	if (pipe(fds) == -1) throw std::runtime_error("Error: cgi pipe failed");

	pid = fork();
	if (pid == -1) throw std::runtime_error("Error: cgi fork failed");
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

		// need to be changed
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
		char buffer[1024];
		int bytes_read;
		int status;
		std::string output;

		close(fds[1]);

		while ((bytes_read = read(fds[0], buffer, sizeof(buffer) - 1)) > 0)
		{
			buffer[bytes_read] = '\0';
			output += buffer;
		}
		close(fds[0]);
		waitpid(pid, &status, 0);
		if (output.empty()) throw std::runtime_error("Error: cgi empty output");
		return (output);
	}
	return ("");
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
			exit(1);  // should be replaced
		};

		if (parser.CheckerMethod(e_Methods_to_String(request.getMethod())) ==
			false)
		{
			Log::log(Log::ERROR, "Error: invalid method");
			close(connection);
			continue;
		}

		Response response;
		std::string target = request.getTarget();
		if (target == "/")
			target += server.getIndexes()[0];  // need to be adapted
		try
		{
			if (target.find("/cgi-bin/") == 0)
				response = Response(cgi_execute(request, server, env));
			else
				response = create_response_page(request, server);
		}
		catch (std::exception &e)
		{
			Log::log(Log::ERROR, e.what());
			response = create_response_error("HTTP/1.1", "404", "NOK", "");
		}
		std::cout << UNDERLINE << "Response" << RESET << "\n"
				  << response.toString() << "\n"
				  << std::endl;

		std::string response_char = response.toString();
		send(connection, response_char.c_str(), response_char.size(), 0);
		close(connection);
	}
	close(fdSocket);
	return (EXIT_SUCCESS);
}

void testResponse()
{
	Response response;
	std::map<std::string, std::string> headers;
	std::string content;

	content = "Body of test response";

	headers["Content-Type"] = "text/html";
	headers["Content-Length"] = ft_itos(content.length());

	response.setProtocol("HTTP/1.1");
	response.setStatusCode("200");
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
