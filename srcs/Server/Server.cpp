#include "Server.hpp"

#include <vector>

class BlockServer;
class ConfParser;

Server::Server() : _step(S_STEP_INIT), _epollFD(-1) {}

Server::~Server()
{
	if (this->_epollFD != -1)
		VerifFatalCallFonc(close(_epollFD), "Faild to close epoll instance", false);
	// Delete all the sockets
	for (std::map<int, Socket*>::iterator it = this->_sockets.begin(); it != this->_sockets.end(); ++it)
		delete it->second;
	this->_sockets.clear();
	// Delete all the clients
	for (std::map<int, Client*>::iterator it = this->_clients.begin(); it != this->_clients.end(); ++it)
		delete it->second;
	this->_clients.clear();
}

void Server::stop( void )
{
	this->setStep(S_STEP_STOP);
}

void Server::init(void)
{
	Log::log(Log::DEBUG, "[Server::init] Create epoll instance...");
	this->setEpollFD(VerifFatalCallFonc(epoll_create1(O_CLOEXEC), "Failed to create epoll instance"));

	Log::log(Log::DEBUG, " Create listening sockets...");

	std::map<std::string, std::vector<BlockServer> > &servers = this->_parser.getServers();
	for (std::map<std::string, std::vector<BlockServer> >::iterator it = servers.begin(); it != servers.end(); ++it)
	{
		int socketFD = VerifFatalCallFonc(socket(AF_INET, SOCK_STREAM, 0), "Error with socket function");
		this->_sockets[socketFD] = new Socket(socketFD, extractPort(it->first), extractPort(it->first), &it->second);
		addSocketEpoll(this->_epollFD, socketFD, REQUEST_FLAGS);
	}
	this->setStep(S_STEP_READY);
}
