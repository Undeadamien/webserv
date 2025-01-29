/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtrala <dtrala@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 23:49:03 by dtrala            #+#    #+#             */
/*   Updated: 2025/01/21 14:49:54 by dtrala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <map>
#include <string>

class Request
{
public:
    Request(std::string content);
    Request(const Request &other);
    Request &operator=(const Request &other);
    virtual ~Request();

    std::map<std::string, std::string> parseHeaders(std::string content);

    void setBody(std::string body);
    void setHeaders(std::map<std::string, std::string> headers);

    std::string getBody() const;
    std::map<std::string, std::string> getHeaders() const;

protected:
private:
    std::string _content;
    std::map<std::string, std::string> _headers;
};

#endif
