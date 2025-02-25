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


# define SERVER_DEFAULT_EPOLL_WAIT 500
#define TIMEOUT_CHECK_INTERVAL 5
#define INACTIVITY_TIMEOUT 60

#define TIMEOUT_CGI 30

// define step for server
enum ServerSTEP
{
	S_STEP_INIT = 0,
	S_STEP_READY,
	S_STEP_RUN,
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
		void	_handleClientConnec(int fd);
		void	_handleClientDisco(int fd);

	public:


		Server(void);
		~Server(void);

		void init(void);
		void start(void);
		void stop(void);

		/* GETTERS */
		int getSTEP(void) const { return _step; }
		int getEpollFD(void) const { return _epollFD; }
		ConfParser& getParser(void) { return _parser; }
		std::map<int, Socket*> getSockets(void) const { return _sockets; }
		Socket* getSocket(int fd) { return _sockets[fd]; }
		std::map<int, Client*> getClients(void) const { return _clients; }
		Client* getClient(int fd) { return _clients[fd]; }
};


#endif // SERVER_HPP


#define REQUEST_FLAGS EPOLLIN | EPOLLRDHUP | EPOLLERR // Quand on attend une requete
#define RESPONSE_FLAGS EPOLLIN | EPOLLRDHUP | EPOLLERR | EPOLLOUT // Quand la reponse est prete, et qu'on a la possibilite d'envoyer quelquer chose sur le socket
#define MAX_EVENTS 100
