/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtrala <dtrala@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 23:48:41 by dtrala            #+#    #+#             */
/*   Updated: 2025/01/24 14:55:40 by dtrala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "colors.hpp"
#include "usefull.hpp"
#include <cstddef>
#include <exception>
#include <sstream>
#include <string>

typedef std::map<std::string, std::string> mapHeaders;

Request::Request(std::string content) : _content(content),
                                        _method(parseMethod(content)),
                                        _target(parseTarget(content)),
                                        _protocol(parseProtocol(content)),
                                        _headers(parseHeaders(content)) {};
Request::Request(const Request &other) : _content(other._content),
                                         _method(parseMethod(other._content)),
                                         _target(parseTarget(other._content)),
                                         _protocol(parseProtocol(other._content)),
                                         _headers(parseHeaders(other._content)) {};
Request &Request::operator=(const Request &other)
{
    if (this == &other)
    {
        this->_content = other._content;
        this->_method = other._method;
        this->_target = other._target;
        this->_protocol = other._protocol;
        this->_headers = other._headers;
    }
    return (*this);
};
Request::~Request() {};

e_Methods Request::parseMethod(std::string content)
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
    return (UNKNOWN); // I should throw an error
};
std::string Request::parseTarget(std::string content)
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
std::string Request::parseProtocol(std::string content)
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
mapHeaders Request::parseHeaders(std::string content)
{
    mapHeaders headers;
    std::istringstream str_stream(content.c_str());
    std::string line;
    size_t sep;
    while (std::getline(str_stream, line))
    {
        line = trim(line, " ");
        if ((sep = line.find(":")) == std::string::npos)
            continue; // need to be improved, by trimming whitespaces
        std::string key = trim(line.substr(0, sep), " ");
        std::string value = trim(line.substr(sep + 2), " ");
        headers.insert(std::pair<std::string, std::string>(key, value));
    }
    return (headers);
};

// [] overload to wrap and control the access of the headers
// << overload to cout the object ?

void Request::setContent(std::string content) { this->_content = content; };
void Request::setMethod(e_Methods method) { this->_method = method; };
void Request::setTarget(std::string target) { this->_target = target; };
void Request::setProtocol(std::string protocol) { this->_protocol = protocol; };
void Request::setHeaders(mapHeaders headers) { this->_headers = headers; };

std::string Request::getContent() const { return (this->_content); };
e_Methods Request::getMethod() const { return (this->_method); };
std::string Request::getTarget() const { return (this->_target); };
std::string Request::getProtocol() const { return (this->_protocol); };
mapHeaders Request::getHeaders() const { return (this->_headers); };

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
    return (os);
};
