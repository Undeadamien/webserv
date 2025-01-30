/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtrala <dtrala@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 23:47:55 by dtrala            #+#    #+#             */
/*   Updated: 2025/01/30 12:34:45 by dtrala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <netinet/in.h>

class Socket
{
public:
	Socket();
	Socket(const Socket &other);
	Socket &operator=(const Socket &other);
	~Socket();

	void setFd(int value);
	void setAddress(sockaddr_in address);

	int getFd();
	sockaddr_in getAddress();

protected:
private:
	int _fd;
	sockaddr_in _address;
};

#endif
