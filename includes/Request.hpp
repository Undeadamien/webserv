/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtrala <dtrala@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 23:49:03 by dtrala            #+#    #+#             */
/*   Updated: 2025/01/29 12:07:27 by dtrala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <map>
#include <ostream>
#include <string>

#include "usefull.hpp"

class Request
{
public:
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

	std::string toString() const;

protected:
private:
	e_Methods _method;
	std::string _target;
	std::string _protocol;
	std::map<std::string, std::string> _headers;
	std::string _body;
};

std::ostream &operator<<(std::ostream &os, const Request &request);

#endif
