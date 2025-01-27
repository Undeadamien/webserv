/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtrala <dtrala@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 23:48:41 by dtrala            #+#    #+#             */
/*   Updated: 2025/01/27 15:10:20 by dtrala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "colors.hpp"
#include "usefull.hpp"
#include <cstddef>
#include <exception>
#include <iostream>
#include <sstream>
#include <string>

typedef std::map<std::string, std::string> mapHeaders;

Request::Request(const std::string &content) : _method(parseMethod(content)),
                                               _target(parseTarget(content)),
                                               _protocol(parseProtocol(content)),
                                               _headers(parseHeaders(content)),
                                               _body(parseBody(content)) {};
Request::Request(const Request &other) : _method(other._method),
                                         _target(other._target),
                                         _protocol(other._protocol),
                                         _headers(other._headers),
                                         _body(other._body) {};
Request &Request::operator=(const Request &other)
{
    if (this == &other)
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
        throw std::exception();
    line = trim(line, " ");
    size_t end = line.find_first_of(" ");
    if (end == std::string::npos)
        throw std::exception();
    std::string method = line.substr(0, end);
    if (method == "GET")
        return (GET);
    if (method == "POST")
        return (POST);
    if (method == "DELETE")
        return (DELETE);
    return (UNKNOWN); // throw an error ?
};
std::string Request::parseTarget(const std::string &content)
{
    std::istringstream str_stream(content);
    std::string line;
    if (!std::getline(str_stream, line))
        throw std::exception();
    line = trim(line, " ");
    size_t start = line.find_first_of(" ") + 1;
    size_t end = line.find_last_of(" ") - 1;
    if (start == std::string::npos || end == std::string::npos)
        throw std::exception();
    return (line.substr(start, end - start + 1));
};
std::string Request::parseProtocol(const std::string &content)
{
    std::istringstream str_stream(content);
    std::string line;
    if (!std::getline(str_stream, line))
        throw std::exception();
    line = trim(line, " ");
    size_t start = line.find_last_of(" ") + 1;
    if (start == std::string::npos)
        throw std::exception();
    // throw an error if not HTTPS 1.1 ?
    return (line.substr(start));
};
mapHeaders Request::parseHeaders(const std::string &content)
{
    mapHeaders headers;
    std::istringstream str_stream(content.c_str());
    std::string line;
    size_t sep;
    std::getline(str_stream, line); // skip the startline
    while (std::getline(str_stream, line) && !line.empty())
    {
        line = trim(line, " ");
        if ((sep = line.find(":")) == std::string::npos)
            throw std::exception(); // invalid line in header field
        std::string key = trim(line.substr(0, sep), " ");
        std::string value = trim(line.substr(sep + 2), " ");
        headers.insert(std::pair<std::string, std::string>(key, value));
    }
    return (headers);
};
std::string Request::parseBody(const std::string &content)
{
    std::istringstream str_stream(content.c_str());
    std::string line;
    size_t empty_line = content.find("\n\n");
    if (empty_line == std::string::npos)
        return ("");
    return (content.substr(empty_line + 2));
};

// [] overload to wrap and control the access of the headers

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
    if (this->getMethod() == GET)
        method = "GET";
    else if (this->getMethod() == PUT)
        method = "PUT";
    else if (this->getMethod() == POST)
        method = "POST";
    str = str + method + " " + this->getTarget() + " " + this->getProtocol() + "\n";
    mapHeaders headers = this->getHeaders();
    for (mapHeaders::iterator it = headers.begin(); it != headers.end(); it++)
        str = str + it->first + ": " + it->second + "\n";
    str += "\n";
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
    os << GREEN << BOLD << "Body:" << RESET << "\n"
       << request.getBody();

    return (os);
};
