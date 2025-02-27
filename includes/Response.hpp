/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtrala <dtrala@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 23:51:05 by dtrala            #+#    #+#             */
/*   Updated: 2025/02/20 03:17:17 by dtrala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <map>
#include <string>

#include "Client.hpp"

typedef std::map<std::string, std::string> mapHeaders;

class Response
{
public:
	enum e_response_state
	{
		INIT,
		PROCESS,
		CHUNK,
		FINISH
	};

	Response();
	Response(Client *client);
	Response(std::string raw);
	Response(const Response &other);
	Response &operator=(const Response &other);
	~Response();

	std::string parseProtocol(const std::string &content);
	std::string parseStatusCode(const std::string &content);
	std::string parseStatusText(const std::string &content);
	std::map<std::string, std::string> parseHeaders(const std::string &content);
	std::string parseBody(const std::string &content);

	void setProtocol(std::string protocol);
	void setStatusCode(std::string code);
	void setStatusText(std::string text);
	void setHeaders(std::map<std::string, std::string> headers);
	void setBody(std::string body);

	const std::string &getProtocol() const;
	const std::string &getStatusCode() const;
	const std::string &getStatusText() const;
	const std::map<std::string, std::string> &getHeaders() const;
	const std::string &getBody() const;

	std::string toString() const;

protected:
private:
	std::string _protocol;
	std::string _status_code;
	std::string _status_text;
	std::map<std::string, std::string> _headers;
	std::string _body;
	Request *_request;
	//CgiHandler _cgiHandler;
	e_response_state _step;
	int _fileFd;
};

std::ostream &operator<<(std::ostream &os, const Response &response);

#endif
