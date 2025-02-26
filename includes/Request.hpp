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

#include "usefull.hpp"

typedef std::map<std::string, std::string> mapHeaders;

class Request
{
public:
	enum	e_parse_step
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

	const e_Methods &getMethod() const;
	const std::string &getTarget() const;
	const std::string &getProtocol() const;
	const std::map<std::string, std::string> &getHeaders() const;
	const std::string &getBody() const;
	e_parse_step getStep() const { return _step; };

	std::string toString() const;

	void	Request::checkTimeout(void);


protected:
private:
	e_Methods _method;
	std::string _target;
	std::string _protocol;
	std::map<std::string, std::string> _headers;
	std::string _body;
	e_parse_step _step;
};

std::ostream &operator<<(std::ostream &os, const Request &request);

#endif
