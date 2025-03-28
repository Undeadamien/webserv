#include "Server.hpp"

#include <sys/epoll.h>

#include <algorithm>
#include <exception>
#include <fstream>
#include <iostream>
#include <stdexcept>

#include "BlockLocation.hpp"
#include "BlockServer.hpp"
#include "CgiHandler.hpp"
#include "ConfParser.hpp"
#include "Log.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "usefull.hpp"

Server::Server() : _step(S_STEP_INIT), _epollFD(-1) {}

Server::~Server() {
	// Fermeture sécurisée du descripteur epoll
	if (this->_epollFD != -1) {
		VerifFatalCallFonc(close(this->_epollFD),
						   "Failed to close epoll instance", false);
		this->_epollFD = -1;  // Éviter un double close accidentel
	}

	// Suppression sécurisée des sockets
	for (std::map<int, Socket*>::iterator it = this->_sockets.begin();
		 it != this->_sockets.end(); ++it) {
		if (it->second)	 // Vérification avant delete
		{
			delete it->second;
			it->second = NULL;	// Sécurisation
		}
	}
	this->_sockets.clear();

	// Suppression sécurisée des clients
	for (std::map<int, Client*>::iterator it = this->_clients.begin();
		 it != this->_clients.end(); ++it) {
		if (it->second)	 // Vérification avant delete
		{
			delete it->second;
			it->second = NULL;	// Sécurisation
		}
	}
	this->_clients.clear();
}

void Server::stop(void) { this->setStep(S_STEP_STOP); }

void Server::init(void) {
	Log::log(Log::DEBUG, "[Server::init] Create epoll instance...");
	this->setEpollFD(VerifFatalCallFonc(epoll_create1(O_CLOEXEC),
										"Failed to create epoll instance"));

	Log::log(Log::DEBUG, " Create listening sockets...");

	MapServers& servers = this->_parser.getServers();
	for (MapServers::iterator it = servers.begin(); it != servers.end(); ++it) {
		int socketFD = VerifFatalCallFonc(socket(AF_INET, SOCK_STREAM, 0),
										  "Error with socket function");
		this->_sockets[socketFD] =
			new Socket(socketFD, extractIp(it->first), extractPort(it->first),
					   &(it->second));
		addSocketEpoll(this->_epollFD, socketFD, REQUEST_FLAGS);
	}
	this->setStep(S_STEP_READY);
}

/*_    _                 _ _
 | |  | |               | | |
 | |__| | __ _ _ __   __| | | ___
 |  __  |/ _` | '_ \ / _` | |/ _ \
 | |  | | (_| | | | | (_| | |  __/
 |_|  |_|\__,_|_| |_|\__,_|_|\___|

								   */

void Server::_handleClientConnect(int fd) {
	std::string msg;
	struct sockaddr_in addr;

	msg = "[Server::_handleClientConnect] New client connected on fd : { %d }";
	Log::log(Log::DEBUG, msg.c_str(), fd);

	socklen_t addrLen = sizeof(addr);

	int clientFD = createClientSocket(fd, addr, addrLen);
	if (clientFD == -1) return;

	Client* newClient = createClient(clientFD, fd);
	if (!newClient) {
		Log::log(Log::ERROR, "Failed to create new client.");
		close(clientFD);
		return;
	}

	if (!setNonBlocking(clientFD)) {
		Log::log(Log::ERROR, "Failed to set client socket to non-blocking.");
		this->_clients.erase(clientFD);
		delete newClient;
		close(clientFD);
		return;
	}

	addSocketEpoll(this->_epollFD, clientFD, REQUEST_FLAGS);
}

int Server::createClientSocket(int fd, struct sockaddr_in& addr,
							   socklen_t& addrLen) {
	return VerifFatalCallFonc(accept(fd, (struct sockaddr*)&addr, &addrLen),
							  "Error with accept function");
}

Client* Server::createClient(int clientFD, int fd) {
	Client* newClient = new Client(clientFD, this->_sockets[fd]);
	this->_clients[clientFD] = newClient;
	return newClient;
}

bool Server::setNonBlocking(int clientFD) {
	return VerifFatalCallFonc(fcntl(clientFD, F_SETFL, O_NONBLOCK),
							  "Error with fcntl function") != -1;
}

void Server::_handleClientDisconnect(int fd) {
	Log::log(Log::DEBUG,
			 "[Server::_handleClientDisconnection] Client disconnected on fd : "
			 "{ %d }",
			 fd);

	removeSocketFromEpoll(fd);
	removeClient(fd);
}

void Server::removeSocketFromEpoll(int fd) {
	deleteSocketEpoll(this->_epollFD, fd);
}

void Server::removeClient(int fd) {
	// Utilisez le type explicite de l'itérateur
	std::map<int, Client*>::iterator it = this->_clients.find(fd);
	if (it != this->_clients.end()) {
		delete it->second;
		this->_clients.erase(it);
	}
}

void Server::handleRequest(Client* client) {
	std::string msg, request_str, raw;
	char buffer[CLIENT_BUFFER_SIZE + 1] = {0};
	int received, buffer_size = CLIENT_BUFFER_SIZE;

	Log::log(Log::DEBUG,
			 "[processClientRequest] Processing request from client %d",
			 client->getFd());

	// we cannot check for errno after recv
	// recv return -1 when non-blocking
	while ((received = recv(client->getFd(), buffer, buffer_size, 0)) > 0) {
		Log::log(Log::DEBUG,
				 "[processClientRequest] Read %d bytes from client %d",
				 received, client->getFd());
		buffer[received] = '\0';
		client->appendRequestBuffer(buffer);
	}
	if (received == 0) throw Client::DisconnectedException();

	if (!Request::isComplete(client->getRequestBuffer())) return;
	raw = client->getRequestBuffer();
	client->setRequestBuffer("");

	try {
		client->setRequest(new Request(raw));
	} catch (std::exception& e) {
		client->setResponse(new Response(
			createResponseError("HTTP/1.1", "400", "Bad Request")));
		return;
	}
	Log::log(Log::DEBUG,
			 "[handleRequest] : Request processing already completed\n%s",
			 client->getRequest()->toString().c_str());
}

void Server::handleResponse(Client* client, int epollFD) {
	Response response;
	int clientFd = client->getFd();
	int sent = -1;
	std::string msg;

	response = this->resolveRequest(client->getRequest());
	client->setResponse(new Response(response));
	Log::log(Log::DEBUG, "Response to be sent: \n%s",
			 client->getResponse()->toString().c_str());

	if (clientFd != -1) {
		std::string str = client->getResponse()->toString();
		sent = send(clientFd, str.c_str(), str.size(), 0);
	}  // might need to throw an error if not openned

	if (sent < 0) throw std::runtime_error("send function failed");

	Log::log(Log::DEBUG, "Transmitted %d bytes to client %d", sent, clientFd);
	Log::log(Log::DEBUG, "Response sent to client %d", clientFd);
	client->reset();
	throw Client::DisconnectedException();	// not sure about this one :(
	modifySocketEpoll(epollFD, clientFd, REQUEST_FLAGS);
}

BlockServer* Server::findServer(Request* request) {
	typedef std::vector<BlockServer> Blocks;
	MapHeaders headers;
	std::string host, port;
	std::vector<std::string> names;
	std::vector<std::string>::iterator find;

	headers = request->getHeaders();
	host = headers["Host"];
	port = ft_itos(extractPort(host));

	if (host.empty()) {
		Log::log(Log::ERROR, "[_findServer] Host not found in headers");
		return NULL;
	}

	Log::log(Log::DEBUG, "[findServer] Host: %s", host.c_str());

	MapServers& servers = this->getParser().getServers();
	for (MapServers::iterator it = servers.begin(); it != servers.end(); it++) {
		if (ft_itos(extractPort(it->first)) == port) return (&it->second[0]);
		std::vector<BlockServer>& blocks = it->second;
		for (Blocks::iterator it2 = blocks.begin(); it2 != blocks.end();
			 it2++) {
			names = it2->getServerNames();
			find = std::find(names.begin(), names.end(), host);
			if (find != names.end()) return (&(*it2));
		}
	}
	return (NULL);
};

BlockLocation* Server::findLocation(BlockServer* server, Request* request) {
	BlockLocation* best_match;
	std::string root, target, current, path;
	std::vector<BlockLocation>* locations;
	std::vector<BlockLocation>::iterator it;
	std::vector<std::string>::iterator it2;

	best_match = NULL;
	target = request->getTarget();
	locations = server->getLocations();

	if (target.empty()) return NULL;

	for (it = locations->begin(); it != locations->end(); it++) {
		path = it->getPath();
		if (path.empty()) continue;
		if (target.find(path) != 0 && target.find(path + "/") != 0) continue;
		if (!best_match || path.size() > best_match->getPath().size())
			best_match = &(*it);
	}
	return (best_match);
};

Response Server::handleGetRequest(Request* request, BlockServer* server,
								  BlockLocation* location) {
	Response response;
	MapHeaders headers;
	std::string content, contentLength, target, root;

	target = request->parsePath();

	if (location) {
		root = location->getRoot();
		if (target[target.size() - 1] == '/')
			target += location->getIndexes()[0];
	} else {
		root = server->getRoot();
		if (target == "/") target += server->getIndexes()[0];
	}

	try {
		content = getFileContent(root + target);  // can throw
	} catch (std::exception& e) {
		return (createResponseError(server, "HTTP/1.1", "404", "Not Found"));
	}
	contentLength = ft_itos(content.length());

	headers["Content-Type"] = getMimeType(root + target);
	headers["Content-Length"] = contentLength;

	response.setProtocol("HTTP/1.1");
	response.setStatusCode("200");
	response.setStatusText("OK");
	response.setHeaders(headers);
	response.setBody(content);

	return (response);
};

std::string Server::extractJsonValue(const std::string& json,
									 const std::string& key) {
	std::size_t keyPos = json.find("\"" + key + "\"");
	if (keyPos == std::string::npos) {
		Log::log(Log::ERROR, "Key not found in JSON");
	}

	std::size_t valueStart = json.find(":", keyPos);
	if (valueStart == std::string::npos) {
		Log::log(Log::ERROR, "Invalid JSON format");
	}

	valueStart += 1;  // Skip the colon
	while (json[valueStart] == ' ' || json[valueStart] == '\t' ||
		   json[valueStart] == '\n') {
		valueStart++;  // Skip whitespace
	}

	if (json[valueStart] != '\"') {
		throw std::runtime_error("Expected string value");
	}

	valueStart++;  // Skip the opening quote
	std::size_t valueEnd = json.find("\"", valueStart);
	if (valueEnd == std::string::npos) {
		Log::log(Log::ERROR, "Invalid JSON format");
	}

	return json.substr(valueStart, valueEnd - valueStart);
}

// Fonction pour analyser le JSON et extraire filename et content
MapJson Server::ParseJson(const std::string& content) {
	MapJson result;
	result["filename"] = extractJsonValue(content, "filename");
	result["content"] = extractJsonValue(content, "content");
	return result;
}

Response Server::handlePostRequest(Request* request, BlockServer* server,
								   BlockLocation* location) {
	MapHeaders headers, requestHeaders;
	MapJson parsedJson;
	Response response;
	std::string body, upload_path, bodyLength, content, content_type, filename;
	std::string message, msg, root, target;

	(void)server;

	// will not change
	headers["Content-Type"] = "application/json";
	response.setProtocol("HTTP/1.1");

	if (!location || !location->isMethodAllowed(POST)) {
		message = "{\"success\":false}";
		headers["Content-Length"] = ft_itos(message.length());
		response.setStatusCode("405");	// Status Created
		response.setStatusText("Not Allowed");
		response.setHeaders(headers);
		response.setBody(message);
		return response;
	}

	upload_path = location->getUploadPath();
	body = request->getBody();
	requestHeaders = request->getHeaders();
	content_type = requestHeaders["Content-Type"];

	if (content_type != "application/json") {
		msg = "[Server::handlePostRequest] Content-Type not handled '%s'";
		Log::log(Log::ERROR, msg.c_str(), content_type.c_str());
		message = "{\"success\":false}";
		headers["Content-Length"] = ft_itos(message.length());
		response.setStatusCode("501");
		response.setStatusText("Not Implemented");
		response.setHeaders(headers);
		response.setBody(message);
		return (response);
	}

	parsedJson = ParseJson(body);
	filename = parsedJson["filename"];

	std::ofstream file((upload_path + "/" + filename).c_str());
	if (!file.is_open()) {
		msg = "[Server::handlePostRequest] Error creating the file %s";
		Log::log(Log::ERROR, msg.c_str(), filename.c_str());
		message = "{\"success\":false}";
		headers["Content-Length"] = ft_itos(message.length());
		response.setStatusCode("500");
		response.setStatusText("Internal Server Error");
		response.setHeaders(headers);
		response.setBody(message);
		return (response);
	}

	content = unescapeJsonString(parsedJson["content"]);
	file << content;
	file.close();

	Log::log(Log::DEBUG, "[POST] | filename : %s", filename.c_str());

	message = "{\"success\":true}";
	headers["Content-Length"] = ft_itos(message.length());
	headers["Location"] = upload_path + "/" + filename;
	response.setStatusCode("201");
	response.setStatusText("Created");
	response.setHeaders(headers);
	response.setBody(message);
	return (response);
};

Response Server::handleDeleteRequest(Request* request, BlockServer* server,
									 BlockLocation* location) {
	Response response;
	std::map<std::string, std::string> headers;
	std::string content, contentLength, target, root, message, filePath;
	struct stat fileStat;

	(void)server;

	headers["Content-Type"] = "text/plain";
	response.setProtocol("HTTP/1.1");

	if (!location || !location->isMethodAllowed(DELETE)) {
		message = "";
		headers["Content-Length"] = ft_itos(message.length());
		response.setStatusCode("405");	// No Allowed
		response.setStatusText("Not Allowed");
		response.setHeaders(headers);
		response.setBody(message);
		return response;
	}

	filePath = request->parsePath();  // might be modified

	if (stat(filePath.c_str(), &fileStat) != 0) {
		message = "File Not Found";
		headers["Content-Length"] = ft_itos(message.length());
		response.setStatusCode("404");
		response.setStatusText("Not Found");
		response.setHeaders(headers);
		response.setBody(message);
		return response;
	}

	if (unlink(filePath.c_str()) != 0) {
		message = "Cannot delete file";
		headers["Content-Length"] = ft_itos(message.length());
		response.setStatusCode("403");	// Forbidden
		response.setStatusText("Forbidden");
		response.setHeaders(headers);
		response.setBody(message);
		return response;
	}

	message = "File deleted succefully";
	headers["Content-Length"] = ft_itos(message.length());
	response.setStatusCode("200");	// OK
	response.setStatusText("OK");
	response.setHeaders(headers);
	response.setBody(message);
	return response;
};

bool Server::isCgi(Request* request, BlockLocation* location) {
	typedef std::map<std::string, std::string> MapCgi;
	MapCgi cgis;
	MapCgi::iterator cgi_path;
	std::string ext;
	struct stat info;

	if (!location) return false;
	cgis = location->getCGI();
	ext = parseFileExtension(request->parsePath());
	if (ext.empty())
		return (Log::log(Log::DEBUG, "Extension file empty"), false);

	cgi_path = cgis.find(ext);
	if (cgi_path == cgis.end()) return (false);

	if (stat((cgi_path->second).c_str(), &info) == 0 &&
		(info.st_mode & S_IFREG) && (info.st_mode & S_IXUSR)) {
		return true;
	}
	Log::log(Log::INFO, "CGI_path : '%s' not valid", cgi_path->second.c_str());
	return (false);
}
bool Server::hasRedirection(BlockLocation* location) {
	std::pair<int, std::string> rewrite;

	if (!location) return false;
	rewrite = location->getRewrite();
	return rewrite.first != 0 && !rewrite.second.empty();
};

Response Server::handleCgiRequest(Request* request, BlockServer* server,
								  BlockLocation* location) {
	CgiHandler cgi;
	return cgi.CgiMaker(request, location, server);
};

Response Server::handleRedirection(Request* request, BlockServer* server,
								   BlockLocation* location) {
	Response response;
	MapHeaders headers;
	std::string new_target, status_code, status_text;
	static std::map<std::string, std::string> MapRedirection;

	(void)request;

	if (MapRedirection.empty()) {
		MapRedirection["300"] = "Multiple Choices";
		MapRedirection["301"] = "Moved Permanently";
		MapRedirection["302"] = "Found";
		MapRedirection["303"] = "See Other";
		MapRedirection["304"] = "Not Modified";
		MapRedirection["305"] = "Use Proxy";
		// 306 unused
		MapRedirection["307"] = "Temporary Redirect";
		MapRedirection["308"] = "Permanent Redirect";
	}

	new_target = location->getRewrite().second;
	status_code = ft_itos(location->getRewrite().first);
	status_text = MapRedirection[status_code];

	if (status_text.empty())
		return (createResponseError(server, "HTTP/1.1", "500",
									"Internal Server Error"));

	headers["Content-Length"] = "0";
	headers["Location"] = new_target;

	response.setProtocol("HTTP/1.1");
	response.setStatusCode(status_code);
	response.setStatusText(status_text);
	response.setHeaders(headers);
	response.setBody("");

	return response;
};

Response Server::resolveRequest(Request* request) {
	BlockLocation* location;
	BlockServer* server;
	Response res;

	server = this->findServer(request);
	if (!server)  // should never happen
		return createResponseError("HTTP/1.1", "500", "Internal Server Error");

	// the request body is too large for the server
	if (request->getBody().length() > server->getClientMaxBodySize())
		return createResponseError(server, "HTTP/1.1", "400", "Bad Request");

	location = this->findLocation(server, request);

	if (location && this->hasRedirection(location))
		return (this->handleRedirection(request, server, location));
	if (location && this->isCgi(request, location))
		return (this->handleCgiRequest(request, server, location));

	if (request->getMethod() == GET)
		return (this->handleGetRequest(request, server, location));
	if (request->getMethod() == POST)
		return (this->handlePostRequest(request, server, location));
	if (request->getMethod() == DELETE)
		return (this->handleDeleteRequest(request, server, location));
	return (createResponseError(server, "HTTP/1.1", "501",
								"Method Not Implemented"));
}

void Server::handleEvent(epoll_event* events, int i) {
	uint32_t event = events[i].events;
	int fd = events[i].data.fd;

	try {
		if (event & (EPOLLHUP | EPOLLERR | EPOLLRDHUP))
			throw Client::DisconnectedException();
		if (event & EPOLLIN) {
			if (this->_clients.find(fd) == this->_clients.end()) {
				_handleClientConnect(fd);
			} else {
				this->_clients[fd]->updateActivity();
				this->handleRequest(this->_clients[fd]);
				modifySocketEpoll(g_server->getEpollFD(), fd, RESPONSE_FLAGS);
			}
		}
		if (event & EPOLLOUT) {
			this->_clients[fd]->updateActivity();
			if (this->_clients[fd]->getRequest() != NULL) {
				this->handleResponse(this->_clients[fd], this->_epollFD);
				modifySocketEpoll(g_server->getEpollFD(), fd, REQUEST_FLAGS);
			}
		}
	} catch (Client::DisconnectedException& e) {
		this->_handleClientDisconnect(fd);
	} catch (const std::exception& e) {
		Log::log(Log::ERROR, "[Server::handleEvent] Error with client %d : %s",
				 fd, e.what());
		this->_handleClientDisconnect(fd);
	}
}

void Server::_checkTimeouts(time_t currentTime) {
	std::map<int, Client*>::iterator it = this->_clients.begin();
	while (it != this->_clients.end()) {
		// it->second->getRequest()->checkTimeout();
		if (currentTime - it->second->getLastActivity() > INACTIVITY_TIMEOUT) {
			Log::log(Log::DEBUG, "[Server::_checkTimeouts] Client %d timed out",
					 it->first);
			deleteSocketEpoll(this->_epollFD, it->first);
			delete it->second;
			this->_clients.erase(it++);
		} else
			++it;
	}
}

void Server::execute(void) {
	if (this->getStep() != S_STEP_READY)
		Log::log(Log::FATAL, "Server is not ready to run");
	this->setStep(S_STEP_EXEC);

	time_t lastTimeoutCheck = time(NULL);

	epoll_event events[MAX_EVENTS];
	while (this->getStep() == S_STEP_EXEC) {
		int nfds = VerifFatalCallFonc(
			epoll_wait(this->_epollFD, events, MAX_EVENTS, SD_EPOLL_WAIT),
			"Error with epoll_wait function");
		for (int i = 0; i < nfds; i++) handleEvent(events, i);
		time_t currentTime = time(NULL);
		if (currentTime - lastTimeoutCheck >= TIMEOUT_CHECK_INTERVAL) {
			this->_checkTimeouts(currentTime);
			lastTimeoutCheck = currentTime;
		}
	}
}

void Server::setStep(int step) {
	if (step == S_STEP_INIT)
		Log::log(Log::INFO, "Parsing completed");
	else if (step == S_STEP_READY)
		Log::log(Log::INFO, "Server is ready to run");
	else if (step == S_STEP_EXEC)
		Log::log(Log::INFO, "Server is running");
	else if (step == S_STEP_STOP)
		Log::log(Log::INFO, "Server is stopping...");
	else
		Log::log(Log::ERROR, "Invalid step for server");
	this->_step = step;
}
