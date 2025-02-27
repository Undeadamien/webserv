#include "Client.hpp"

Client::Client(int fd, Socket* socket)
	: _fd(fd),
	  _socket(socket),
	  _request(new Request(this)),
	  _response(new Response(this)),
	  _lastActivity(time(NULL))
{
	Log::log(Log::DEBUG, "[Client] Initializing client with fd %d", fd);
}

Client::~Client(void)
{
	if (this->_fd != -1)
		VerifFatalCallFonc(close(this->_fd),
						   "[~Client] Faild to close client socket", false);
	if (this->_request != NULL) delete this->_request;
	if (this->_response != NULL) delete this->_response;
}

void Client::processClientRequest(void)
{
	Log::log(Log::DEBUG,
			 "[processClientRequest] Processing request from client %d",
			 this->_fd);

	char readBuffer[CLIENT_READ_BUFFER_SIZE + 1] = {0};
	int receivedBytes = recv(this->_fd, readBuffer, CLIENT_READ_BUFFER_SIZE, 0);

	if (receivedBytes > 0)
	{
		Log::log(Log::DEBUG,
				 "[processClientRequest] Read %d bytes from client %d",
				 receivedBytes, this->_fd);
		readBuffer[receivedBytes] = '\0';
	}
	else if (receivedBytes < 0)
		throw std::runtime_error("recv function failed");
	else
		throw Client::DisconnectedException();

	if (this->_request->getStep() == Request::FINISH)
	{
		Log::log(Log::DEBUG,
				 "[processClientRequest] Request processing already completed");
		return;
	}

	std::string requestData(readBuffer, receivedBytes);
	// this->_request->parse(requestData); // Parse the request here
}

void Client::sendClientResponse(int epollFD)
{
	(void)epollFD;
	/*    if (this->_response->prepareResponse(epollFD) == -1)
		{
			return; // Response is not ready
		}

		Log::log(Log::DEBUG, "Response to be sent: \n%s",
	   this->_response->getResponse().c_str());

		int bytesTransmitted = -1;
		if (this->getFd() != -1)
		{
			bytesTransmitted = send(this->getFd(),
	   this->_response->getResponse().c_str(),
	   this->_response->getResponseSize(), 0);
		}

		if (bytesTransmitted < 0)
		{
			throw std::runtime_error("send function failed");
		}
		else
		{
			Log::log(Log::DEBUG, "Transmitted %d bytes to client %d",
	   bytesTransmitted, this->getFd());
		}

		if (this->getResponse()->getState() == Response::FINISH)
		{
			if (this->_request->getStateCode() != Request::FINISH)
			{
				throw Client::DisconnectedException();
			}
			Log::log(Log::DEBUG, "Response successfully sent to client %d",
	   this->getFd()); this->reset(); modifySocketEpoll(epollFD, this->getFd(),
	   REQUEST_FLAGS); } attendre d'avoir fini la class reponse pour le faire*/
	Log::log(Log::DEBUG, "[sendClientResponse] Class reponse not made");
}

void Client::reset(void)
{
	// Reset request
	delete this->_request;
	this->_request = new Request(this);
	// Reset response
	delete this->_response;
	this->_response = new Response(this);
}

void Client::checkCgi(void)
{
	/*	if (this->_request == NULL) // No request
			return ;
		if (!this->_request->_cgi._isCGI) // Not a CGI
			return ;
		return (this->_request->_cgi._checkState());*/
	Log::log(Log::DEBUG, "[checkCgi] Checking CGI not made");
}
