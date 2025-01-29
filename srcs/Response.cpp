/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtrala <dtrala@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 23:50:42 by dtrala            #+#    #+#             */
/*   Updated: 2025/01/29 12:49:11 by dtrala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

#include <iostream>
#include <string>

#include "colors.hpp"

typedef std::map<std::string, std::string> mapHeaders;

Response::Response() {};
Response::Response(const Response &other) { (void)other; };
Response &Response::operator=(const Response &other)
{
	if (this != &other)
	{
		this->_protocol = other._protocol;
		this->_status_code = other._status_code;
		this->_status_text = other._status_text;
		this->_headers = other._headers;
		this->_body = other._body;
	}
	return (*this);
};
Response::~Response() {};

void Response::setProtocol(std::string protocol) { this->_protocol = protocol; };
void Response::setStatusCode(int code) { this->_status_code = code; };
void Response::setStatusText(std::string text) { this->_status_text = text; };
void Response::setHeaders(mapHeaders headers) { this->_headers = headers; };
void Response::setBody(std::string body) { this->_body = body; };

const std::string &Response::getProtocol() const { return (this->_protocol); };
const int &Response::getStatusCode() const { return (this->_status_code); };
const std::string &Response::getStatusText() const { return (this->_status_text); };
const mapHeaders &Response::getHeaders() const { return (this->_headers); };
const std::string &Response::getBody() const { return (this->_body); };

std::string Response::toString() const
{
	std::string str;
	str += this->getProtocol() + " " + itos(this->getStatusCode()) + " " + this->getStatusText() + "\n";
	mapHeaders headers = this->getHeaders();
	for (mapHeaders::iterator it = headers.begin(); it != headers.end(); it++)
		str += it->first + ": " + it->second + "\n";
	str += "\n";
	str += this->getBody();
	return (str);
};

std::ostream &operator<<(std::ostream &os, const Response &response)
{
	os << GREEN << BOLD << "Protocol:" << RESET << "\n"
	   << response.getProtocol() << "\n";
	os << GREEN << BOLD << "Status code:" << RESET << "\n"
	   << response.getStatusCode() << "\n";
	os << GREEN << BOLD << "Status text:" << RESET << "\n"
	   << response.getStatusText() << "\n";
	mapHeaders headers = response.getHeaders();
	os << GREEN << BOLD << "Headers:" << RESET << "\n";
	for (mapHeaders::iterator it = headers.begin(); it != headers.end(); it++)
		os << it->first << ": " << it->second << std::endl;
	os << GREEN << BOLD << "Body:" << RESET << "\n"
	   << response.getBody();
	return (os);
};
