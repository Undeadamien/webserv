/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtrala <dtrala@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 23:48:41 by dtrala            #+#    #+#             */
/*   Updated: 2025/01/21 15:15:31 by dtrala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include <cstddef>
#include <sstream>
#include <string>

Request::Request(std::string content) : _content(content), _headers(parseHeaders(content)) {};
Request::Request(const Request &other) : _content(other._content), _headers(parseHeaders(other._content)) {};
Request &Request::operator=(const Request &other)
{
    if (this == &other)
    {
        this->_content = other._content;
        this->_headers = other._headers;
    }
    return (*this);
};
Request::~Request() {};

// function to parse the first line of the body
// method, target/file, HTTP/1.1

std::map<std::string, std::string> Request::parseHeaders(std::string content)
{
    std::map<std::string, std::string> headers;
    std::istringstream str_stream(content.c_str());
    std::string line;
    size_t sep;
    while (std::getline(str_stream, line))
    {
        if ((sep = line.find(":")) == std::string::npos)
            continue; // need to be improved, by trimming whitespaces
        std::string key = line.substr(0, sep);
        std::string value = line.substr(sep + 2);
        headers.insert(std::pair<std::string, std::string>(key, value));
    }
    return (headers);
};

// [] overload to wrap and control the access of the headers
// << overload to cout the object ?

void Request::setBody(std::string body) { this->_content = body; };
void Request::setHeaders(std::map<std::string, std::string> headers) { this->_headers = headers; };

std::string Request::getBody() const { return (this->_content); };
std::map<std::string, std::string> Request::getHeaders() const { return (this->_headers); };
