/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtrala <dtrala@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 23:51:05 by dtrala            #+#    #+#             */
/*   Updated: 2025/02/06 01:42:48 by dtrala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <map>
#include <string>

#include "usefull.hpp"

typedef std::map<std::string, std::string> mapHeaders;

class Response
{
public:
	Response();
	Response(std::string &protocol, int status_code, std::string &status_text, mapHeaders headers, std::string &body);
	Response(const Response &other);
	Response &operator=(const Response &other);
	~Response();

	void setProtocol(std::string protocol);
	void setStatusCode(int code);
	void setStatusText(std::string text);
	void setHeaders(std::map<std::string, std::string> headers);
	void setBody(std::string body);

	const std::string &getProtocol() const;
	const int &getStatusCode() const;
	const std::string &getStatusText() const;
	const std::map<std::string, std::string> &getHeaders() const;
	const std::string &getBody() const;

	std::string toString() const;

protected:
private:
	std::string _protocol;
	int _status_code;
	std::string _status_text;
	std::map<std::string, std::string> _headers;
	std::string _body;
};

std::ostream &operator<<(std::ostream &os, const Response &response);

#endif
