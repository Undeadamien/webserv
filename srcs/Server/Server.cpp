#include "Server.hpp"

#include <algorithm>
#include <exception>
#include <iostream>
#include <stdexcept>

#include "BlockLocation.hpp"
#include "BlockServer.hpp"
#include "ConfParser.hpp"
#include "Log.hpp"
#include "Request.hpp"
#include "usefull.hpp"

Server::Server() : _step(S_STEP_INIT), _epollFD(-1) {}
Server::~Server() {
	if (this->_epollFD != -1)
		VerifFatalCallFonc(close(_epollFD), "Faild to close epoll instance",
						   false);
	// Delete all the sockets
	for (std::map<int, Socket*>::iterator it = this->_sockets.begin();
		 it != this->_sockets.end(); ++it)
		delete it->second;
	this->_sockets.clear();
	// Delete all the clients
	for (std::map<int, Client*>::iterator it = this->_clients.begin();
		 it != this->_clients.end(); ++it)
		delete it->second;
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
	std::string msg, request_str;
	int received;
	char buffer[CLIENT_READ_BUFFER_SIZE + 1] = {0};

	msg = "[processClientRequest] Processing request from client %d";
	Log::log(Log::DEBUG, msg.c_str(), client->getFd());

	received = recv(client->getFd(), buffer, CLIENT_READ_BUFFER_SIZE, 0);
	if (received == 0) throw Client::DisconnectedException();
	if (received < 0) throw std::runtime_error("recv function failed");
	msg = "[processClientRequest] Read %d bytes from client %d";
	Log::log(Log::DEBUG, msg.c_str(), received, client->getFd());
	buffer[received] = '\0';

	std::string requestRaw(buffer, received);
	try {
		client->setRequest(new Request(requestRaw));
	}
	// add some error
	catch (std::exception& e) {
		std::cout << "debug error creating the request data" << std::endl;
		return;
	}
	msg = "[handleRequest] : Request processing already completed\n%s";
	request_str = client->getRequest()->toString().c_str();
	Log::log(Log::DEBUG, msg.c_str(), request_str.c_str());
}

void Server::handleResponse(Client* client, int epollFD) {
	Response response;
	int clientFd = client->getFd();
	int sent = -1;
	std::string msg;

	response = this->generateResponse(client->getRequest());
	client->setResponse(new Response(response));

	Log::log(Log::DEBUG, "Response to be sent: \n%s",
			 client->getResponse()->toString().c_str());

	if (clientFd != -1) {
		std::string str = client->getResponse()->toString();
		sent = send(clientFd, str.c_str(), str.size(), 0);
	}

	if (sent < 0) throw std::runtime_error("send function failed");

	Log::log(Log::DEBUG, "Transmitted %d bytes to client %d", sent, clientFd);
	Log::log(Log::DEBUG, "Response sent to client %d", clientFd);
	client->reset();
	modifySocketEpoll(epollFD, clientFd, REQUEST_FLAGS);
}

BlockServer* Server::findServer(Request* request) {
	typedef std::vector<BlockServer> Blocks;
	MapHeaders headers;
	MapServers servers;
	std::string host, port;
	std::vector<BlockServer> blocks;
	std::vector<std::string> names;
	std::vector<std::string>::iterator find;

	headers = request->getHeaders();
	host = headers["Host"];
	port = ft_itos(extractPort(host));

	if (host.empty()) {
		Log::log(Log::ERROR, "[_findServer] Host not found in headers");
		throw std::runtime_error("Error: Missing 'Host' header in Request");
		return NULL;  // might want to return instead of throwing
	}

	Log::log(Log::DEBUG, "[findServer] Host: %s", host.c_str());

	servers = this->getParser().getConfigs();
	for (MapServers::iterator it = servers.begin(); it != servers.end(); it++) {
		if (ft_itos(extractPort(it->first)) == port) return (&it->second[0]);
		blocks = it->second;
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

	for (it = locations->begin(); it != locations->end(); it++) {
		path = it->getPath();
		if (target.find(path) == 0) {
			if (!best_match) best_match = &(*it);
			if (path.size() > best_match->getPath().size()) best_match = &(*it);
		}
	}
	return (best_match);
};

Response Server::handleGetRequest(Request* request) {
	// check if request target inside location
	// need to retrieve the root
	// need to retrieve the index
	BlockLocation* location;
	BlockServer* server;
	Response response;
	std::map<std::string, std::string> headers;
	std::string content, contentLength, target, root;

	server = this->findServer(request);
	if (!server) throw std::exception();  // should probably return an error
	location = this->findLocation(server, request);
	target = request->getTarget();

	if (location) {
		root = location->getRoot();
		if (target[target.size() - 1] == '/')
			target += location->getIndexes()[0];
		// target = rtrim(target, "?");  // need to parse queries correctly
	} else {
		root = server->getRoot();
		if (target == "/") target += server->getIndexes()[0];
		// target = rtrim(target, "?");  // need to parse queries correctly
	}

	try {
		content = getFileContent(root + target);  // can throw
	} catch (std::exception& e) {
		return (createResponseError("HTTP/1.1", "404", "Not Found"));
	}
	contentLength = ft_itos(content.length());

	headers["Content-Type"] = getMimeType(root + target);  // can throw
	headers["Content-Length"] = contentLength;

	response.setProtocol("HTTP/1.1");
	response.setStatusCode("200");
	response.setStatusText("OK");
	response.setHeaders(headers);
	response.setBody(content);

	return (response);
};
Response Server::handlePostRequest(Request* request) {
	(void)request;
	Response response;
	return (response);
};
Response Server::handleDeleteRequest(Request* request) {
	(void)request;
	Response response;
	return (response);
};
// might need to allocate  the return response
Response Server::generateResponse(Request* req) {
	Response res;
	// handle redirect
	// check for cgi
	// check for chunk

	if (req->getMethod() == GET) return (this->handleGetRequest(req));
	if (req->getMethod() == POST) return (this->handlePostRequest(req));
	if (req->getMethod() == DELETE) return (this->handleDeleteRequest(req));
	return (createResponseError("HTTP/1.1", "405", "Method Not Allowed"));
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
			this->handleResponse(this->_clients[fd], this->_epollFD);
			modifySocketEpoll(g_server->getEpollFD(), fd, REQUEST_FLAGS);
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

		// generate too much clutter
		// Log::log(Log::DEBUG, "|Server::run| There are %d file descriptors
		// ready for I/O " "after epoll wait", nfds);

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
