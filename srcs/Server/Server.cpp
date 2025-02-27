#include "Server.hpp"

#include <vector>

Server::Server() : _step(S_STEP_INIT), _epollFD(-1) {}

Server::~Server()
{
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

void Server::init(void)
{
	Log::log(Log::DEBUG, "[Server::init] Create epoll instance...");
	this->setEpollFD(VerifFatalCallFonc(epoll_create1(O_CLOEXEC),
										"Failed to create epoll instance"));

	Log::log(Log::DEBUG, " Create listening sockets...");

	MapServers& servers = this->_parser.getServers();
	for (MapServers::iterator it = servers.begin(); it != servers.end(); ++it)
	{
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

void Server::_handleClientConnect(int fd)
{
	Log::log(
		Log::DEBUG,
		"|Server::_handleClientConnect| New client connected on fd : { %d }",
		fd);

	struct sockaddr_in addr;
	socklen_t addrLen = sizeof(addr);

	int clientFD = createClientSocket(fd, addr, addrLen);
	if (clientFD == -1)
	{
		return;	 // VerifFatalCallFonc gère déjà l'affichage de l'erreur
	}

	Client* newClient = createClient(clientFD, fd);
	if (!newClient)
	{
		Log::log(Log::ERROR, "Failed to create new client.");
		close(clientFD);
		return;
	}

	if (!setNonBlocking(clientFD))
	{
		Log::log(Log::ERROR, "Failed to set client socket to non-blocking.");
		this->_clients.erase(
			clientFD);	// Supprimer de la map avant suppression
		delete newClient;
		close(clientFD);
		return;
	}

	addSocketEpoll(this->_epollFD, clientFD, REQUEST_FLAGS);
}

int Server::createClientSocket(int fd, struct sockaddr_in& addr,
							   socklen_t& addrLen)
{
	return VerifFatalCallFonc(accept(fd, (struct sockaddr*)&addr, &addrLen),
							  "Error with accept function");
}

Client* Server::createClient(int clientFD, int fd)
{
	Client* newClient = new Client(clientFD, this->_sockets[fd]);
	this->_clients[clientFD] = newClient;
	return newClient;
}

bool Server::setNonBlocking(int clientFD)
{
	return VerifFatalCallFonc(fcntl(clientFD, F_SETFL, O_NONBLOCK),
							  "Error with fcntl function") != -1;
}

void Server::_handleClientDisconnect(int fd)
{
	Log::log(Log::DEBUG,
			 "|Server::_handleClientDisconnection| Client disconnected on fd : "
			 "{ %d }",
			 fd);

	removeSocketFromEpoll(fd);
	removeClient(fd);
}

void Server::removeSocketFromEpoll(int fd)
{
	deleteSocketEpoll(this->_epollFD, fd);
}

void Server::removeClient(int fd)
{
	// Utilisez le type explicite de l'itérateur
	std::map<int, Client*>::iterator it = this->_clients.find(fd);
	if (it != this->_clients.end())
	{
		delete it->second;
		this->_clients.erase(it);
	}
}

void Server::handleEvent(epoll_event* events, int i)
{
	uint32_t event = events[i].events;
	int fd = events[i].data.fd;

	try
	{
		if (event & (EPOLLHUP | EPOLLERR |
					 EPOLLRDHUP))  // Error with the file descriptor
			throw Client::DisconnectedException();
		if (event & EPOLLIN)
		{
			if (this->_clients.find(fd) ==
				this->_clients.end())  // New client connection
				_handleClientConnect(fd);
			else
			{
				this->_clients[fd]->updateActivity();
				this->_clients[fd]->processClientRequest();
			}
		}
		if (event & EPOLLOUT)
		{
			this->_clients[fd]->updateActivity();
			this->_clients[fd]->checkCgi();
			if (this->_clients[fd]->getRequest() &&
				this->_clients[fd]->getRequest()->getStep() == Request::FINISH)
				this->_clients[fd]->sendClientResponse(this->_epollFD);
		}
	}
	// catch (ChildProcessException& e)
	//{  // Child process error (CGI)
	//	throw ChildProcessException();
	// }
	catch (Client::DisconnectedException& e)
	{  // Client disconnected
		this->_handleClientDisconnect(fd);
	}
	catch (const std::exception& e)
	{  // Other exceptions
		Log::log(Log::ERROR, "[Server::handleEvent] Error with client %d : %s",
				 fd, e.what());
		this->_handleClientDisconnect(fd);
	}
}

void Server::_checkTimeouts(time_t currentTime)
{
	std::map<int, Client*>::iterator it = this->_clients.begin();
	while (it != this->_clients.end())
	{
		it->second->getRequest()->checkTimeout();
		if (currentTime - it->second->getLastActivity() > INACTIVITY_TIMEOUT)
		{
			Log::log(Log::DEBUG, "[Server::_checkTimeouts] Client %d timed out",
					 it->first);
			deleteSocketEpoll(this->_epollFD, it->first);
			delete it->second;
			this->_clients.erase(it++);
		}
		else
			++it;
	}
}

void Server::execute(void)
{
	if (this->getStep() != S_STEP_READY)
		Log::log(Log::FATAL, "Server is not ready to run");
	this->setStep(S_STEP_EXEC);

	time_t lastTimeoutCheck = time(NULL);

	epoll_event events[MAX_EVENTS];
	while (this->getStep() == S_STEP_EXEC)
	{
		int nfds = VerifFatalCallFonc(
			epoll_wait(this->_epollFD, events, MAX_EVENTS, SD_EPOLL_WAIT),
			"Error with epoll_wait function");
		Log::log(Log::DEBUG,
				 "|Server::run| There are %d file descriptors ready for I/O "
				 "after epoll wait",
				 nfds);

		for (int i = 0; i < nfds; i++) handleEvent(events, i);

		time_t currentTime = time(NULL);
		if (currentTime - lastTimeoutCheck >= TIMEOUT_CHECK_INTERVAL)
		{
			this->_checkTimeouts(currentTime);
			lastTimeoutCheck = currentTime;
		}
	}
}

void Server::setStep(int step)
{
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
