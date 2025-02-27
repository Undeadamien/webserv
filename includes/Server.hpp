#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <map>
# include <vector>
# include <sys/epoll.h>
# include <arpa/inet.h>
# include <algorithm>

# include "ConfParser.hpp"
# include "Socket.hpp"
# include "Client.hpp"
# include "BlockServer.hpp"
# include "Response.hpp"
# include "usefull.hpp"
# include "Request.hpp"
#include "Socket_utils.hpp"

class BlockServer;
class ConfParser;
class Socket;

# define SD_EPOLL_WAIT 500
#define TIMEOUT_CHECK_INTERVAL 5
#define INACTIVITY_TIMEOUT 60

#define TIMEOUT_CGI 30

// define step for server
enum ServerSTEP
{
	S_STEP_INIT = 0,
	S_STEP_READY,
	S_STEP_EXEC,
	S_STEP_STOP
};

class Server
{
	private:
		int						_step;
		int 					_epollFD;
		ConfParser				_parser;
		std::map<int, Socket*>	_sockets;
		std::map<int, Client*>	_clients;

		/* SETTERS */
		void setStep(int step);
		void setEpollFD(int epollFD) { _epollFD = epollFD; }

		/* UTILS */
		void _checkTimeouts(time_t currentTime);

		// void sendResponse(Client* client);
		void handleEvent(epoll_event *events, int i);


		/* HANDLE */
		void	_handleClientConnect(int fd);
		void	_handleClientDisconnect(int fd);

		/* HANDLE UTILS */
		int createClientSocket(int fd, struct sockaddr_in& addr,
			socklen_t& addrLen);
		Client* createClient(int clientFD, int fd);
		bool setNonBlocking(int clientFD);
		void addClientToEpoll(int clientFD, Client* newClient);

		void removeSocketFromEpoll(int fd);
		void removeClient(int fd);

	public:


		Server(void);
		~Server(void);

		void init(void);
		void execute(void);
		void stop(void);

		/* GETTERS */
		int getStep(void) const { return _step; }
		int getEpollFD(void) const { return _epollFD; }
		ConfParser& getParser(void) { return _parser; }
		std::map<int, Socket*> getSockets(void) const { return _sockets; }
		Socket* getSocket(int fd) { return _sockets[fd]; }
		std::map<int, Client*> getClients(void) const { return _clients; }
		Client* getClient(int fd) { return _clients[fd]; }
};


#endif
