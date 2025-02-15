/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtrala <dtrala@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 23:48:41 by dtrala            #+#    #+#             */
/*   Updated: 2025/02/13 01:49:07 by dtrala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

#include <cstddef>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#include "colors.hpp"
#include "usefull.hpp"

Request::Request()
	: _method(UNKNOWN), _target(""), _protocol(""), _headers(), _body("") {};
Request::Request(const std::string &content)
	: _method(parseMethod(content)),
	  _target(parseTarget(content)),
	  _protocol(parseProtocol(content)),
	  _headers(parseHeaders(content)),
	  _body(parseBody(content)) {};
Request::Request(const Request &other)
	: _method(other._method),
	  _target(other._target),
	  _protocol(other._protocol),
	  _headers(other._headers),
	  _body(other._body) {};
Request &Request::operator=(const Request &other)
{
	if (this != &other)
	{
		this->_method = other._method;
		this->_target = other._target;
		this->_protocol = other._protocol;
		this->_headers = other._headers;
		this->_body = other._body;
	}
	return (*this);
};
Request::~Request() {};

e_Methods Request::parseMethod(const std::string &content)
{
	std::istringstream str_stream(content);
	std::string line;
	if (!std::getline(str_stream, line))
		throw std::runtime_error("Error: parsing method of request");
	if (line[line.length() - 1] == '\r')
		line = line.substr(0, line.length() - 1);
	line = trim(line, " ");
	size_t end = line.find_first_of(" ");
	if (end == std::string::npos)
		throw std::runtime_error("Error: parsing method of request");
	std::string method = line.substr(0, end);
	// this if block could be refactored
	if (method == "GET")
		return (GET);
	if (method == "POST")
		return (POST);
	if (method == "DELETE")
		return (DELETE);
	return (UNKNOWN);  // throw an error ?
};
std::string Request::parseTarget(const std::string &content)
{
	std::istringstream str_stream(content);
	std::string line;
	if (!std::getline(str_stream, line))
		throw std::runtime_error("Error: parsing target or request");
	if (line[line.length() - 1] == '\r')
		line = line.substr(0, line.length() - 1);
	line = trim(line, " ");
	size_t start = line.find_first_of(" ") + 1;
	size_t end = line.find_last_of(" ") - 1;
	if (start == std::string::npos || end == std::string::npos)
		throw std::runtime_error("Error: parsing target or request");
	return (line.substr(start, end - start + 1));
};
std::string Request::parseProtocol(const std::string &content)
{
	std::istringstream str_stream(content);
	std::string line;
	if (!std::getline(str_stream, line))
		throw std::runtime_error("Error: parsing protocol or request");
	if (line[line.length() - 1] == '\r')
		line = line.substr(0, line.length() - 1);
	line = trim(line, " ");
	size_t start = line.find_last_of(" ") + 1;
	if (start == std::string::npos)
		throw std::runtime_error("Error: parsing protocol or request");
	return (line.substr(start));
};
mapHeaders Request::parseHeaders(const std::string &content)
{
	mapHeaders headers;
	size_t sep;
	std::istringstream str_stream(content);
	std::string key, value, line;

	std::getline(str_stream, line);	 // skip the startline
	while (std::getline(str_stream, line))
	{
		if (line[line.length() - 1] == '\r')
			line = line.substr(0, line.length() - 1);
		if (line.empty())
			break;
		line = trim(line, " ");
		if ((sep = line.find(":")) == std::string::npos)
			throw std::runtime_error("Error: parsing missing ':' in headers");
		key = trim(line.substr(0, sep), " ");
		value = trim(line.substr(sep + 1), " ");
		headers[key] = value;
	}
	return (headers);
};
std::string Request::parseBody(const std::string &content)
{
	std::istringstream str_stream(content.c_str());
	std::string line;
	size_t delimiter = content.find("\r\n\r\n");
	if (delimiter == std::string::npos)
		throw std::runtime_error("Error: parsing body of request");
	return (content.substr(delimiter + 4));
};

// I should add some checks
void Request::setMethod(e_Methods method) { this->_method = method; };
void Request::setTarget(std::string target) { this->_target = target; };
void Request::setProtocol(std::string protocol) { this->_protocol = protocol; };
void Request::setHeaders(mapHeaders headers) { this->_headers = headers; };
void Request::setBody(std::string body) { this->_body = body; };

const e_Methods &Request::getMethod() const { return (this->_method); };
const std::string &Request::getTarget() const { return (this->_target); };
const std::string &Request::getProtocol() const { return (this->_protocol); };
const mapHeaders &Request::getHeaders() const { return (this->_headers); };
const std::string &Request::getBody() const { return (this->_body); };

std::string Request::toString() const
{
	std::string str;
	std::string method;
	// this if block could be refactored
	if (this->getMethod() == GET)
		method = "GET";
	else if (this->getMethod() == PUT)
		method = "PUT";
	else if (this->getMethod() == POST)
		method = "POST";
	str += method + " " + this->getTarget() + " " + this->getProtocol();
	str += "\r\n";
	mapHeaders headers = this->getHeaders();
	for (mapHeaders::iterator it = headers.begin(); it != headers.end(); it++)
		str = str + it->first + ": " + it->second + "\r\n";
	str += "\r\n";
	str = str + this->getBody();
	return (str);
};

// might be useless
std::ostream &operator<<(std::ostream &os, const Request &request)
{
	os << GREEN << BOLD << "Method:" << RESET << "\n"
	   << request.getMethod() << "\n";
	os << GREEN << BOLD << "Target:" << RESET << "\n"
	   << request.getTarget() << "\n";
	os << GREEN << BOLD << "Protocol:" << RESET << "\n"
	   << request.getProtocol() << "\n";
	mapHeaders headers = request.getHeaders();
	os << GREEN << BOLD << "Headers:" << RESET << "\n";
	for (mapHeaders::iterator it = headers.begin(); it != headers.end(); it++)
		os << it->first << ": " << it->second << std::endl;
	os << GREEN << BOLD << "Body:" << RESET << "\n" << request.getBody();
	return (os);
};
