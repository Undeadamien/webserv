/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtrala <dtrala@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 23:44:36 by dtrala            #+#    #+#             */
/*   Updated: 2025/01/30 13:30:46 by dtrala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <vector>

#include "Socket.hpp"

class Server
{
public:
	Server();
	Server(const Server &other);
	Server &operator=(const Server &other);
	~Server();

	void start();

protected:
private:
	std::vector<Socket> sockets;

	void setup();
	void run();
};

#endif
