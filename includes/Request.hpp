/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtrala <dtrala@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 23:49:03 by dtrala            #+#    #+#             */
/*   Updated: 2025/01/24 15:24:34 by dtrala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "usefull.hpp"
#include <map>
#include <ostream>
#include <string>

class Request
{
public:
    Request(std::string content);
    Request(const Request &other);
    Request &operator=(const Request &other);
    ~Request();

    e_Methods parseMethod(std::string content);
    std::string parseTarget(std::string content);
    std::string parseProtocol(std::string content);
    std::map<std::string, std::string> parseHeaders(std::string content);

    void setContent(std::string content);
    void setMethod(e_Methods method);
    void setTarget(std::string target);
    void setProtocol(std::string protocol);
    void setHeaders(std::map<std::string, std::string> headers);

    const std::string &getContent() const;
    const e_Methods &getMethod() const;
    const std::string &getTarget() const;
    const std::string &getProtocol() const;
    const std::map<std::string, std::string> &getHeaders() const;

protected:
private:
    std::string _content;
    e_Methods _method;
    std::string _target;
    std::string _protocol;
    std::map<std::string, std::string> _headers;
};

std::ostream &operator<<(std::ostream &os, const Request &request);

#endif
