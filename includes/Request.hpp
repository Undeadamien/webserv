/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtrala <dtrala@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 23:49:03 by dtrala            #+#    #+#             */
/*   Updated: 2025/02/20 03:28:40 by dtrala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <map>
#include <ostream>
#include <string>

#include "BlockLocation.hpp"
#include "BlockServer.hpp"
#include "Client.hpp"
#include "colors.hpp"
#include "usefull.hpp"
#include "Socket.hpp"
#include "Socket_utils.hpp"

typedef std::map<std::string, std::string> mapHeaders;

#define REQUEST_DEFAULT_STEP_CODE 200
#define REQUEST_DEFAULT_UPLOAD_PATH "./web/upload/"
#define REQUEST_DEFAULT_HEADER_TIMEOUT 10
#define REQUEST_DEFAULT_BODY_TIMEOUT 3600
#define REQUEST_DEFAULT_CGI_TIMEOUT 3

class Client;
class BlockLocation;
class BlockServer;
class Socket;

class Request
{
public:
	enum e_parse_step
	{
		INIT,
		REQUEST_LINE_METHOD,
		REQUEST_LINE_URI,
		REQUEST_LINE_HTTP_VERSION,
		REQUEST_LINE_END,
		HEADERS_INIT,
		HEADERS_PARSE_KEY,
		HEADERS_PARSE_VALUE,
		HEADERS_PARSE_END,
		HEADERS_END,
		BODY_INIT,
		BODY_PROCESS,
		BODY_END,
		CGI_INIT,
		CGI_PROCESS,
		CGI_END,
		FINISH
	};


	Request();
	Request(Client *client);
	Request(const std::string &content);
	Request(const Request &other);
	Request &operator=(const Request &other);
	~Request();

	e_Methods parseMethod(const std::string &content);
	std::string parseTarget(const std::string &content);
	std::string parseProtocol(const std::string &content);
	std::map<std::string, std::string> parseHeaders(const std::string &content);
	std::string parseBody(const std::string &content);

	void setMethod(e_Methods method);
	void setTarget(std::string target);
	void setProtocol(std::string protocol);
	void setHeaders(std::map<std::string, std::string> headers);
	void setBody(std::string body);
	void _setStep(e_parse_step state);

	const e_Methods &getMethod() const;
	const std::string &getTarget() const;
	const std::string &getProtocol() const;
	const std::map<std::string, std::string> &getHeaders() const;
	const std::string &getBody() const;
	e_parse_step getStep() const { return _step; };

	void	_initServer(void);
	void	setError(int code);


	std::string toString() const;

	void checkTimeout(void);

protected:
private:
	e_Methods _method;
	std::string _target;
	std::string _protocol;
	std::map<std::string, std::string> _headers;
	std::string _body;
	e_parse_step _step;
	Client *_client;
	BlockLocation *_location;
	BlockServer *_server;
	std::string _rawRequest;
	std::string _uri;
	std::string _path;
	std::string _httpVersion;
	bool _isChunked;
	// CgiHandler _cgi;
	size_t _contentLength;
	int _chunkSize;
	time_t _timeout;
	int _stepCode;
};

std::ostream &operator<<(std::ostream &os, const Request &request);

#endif
