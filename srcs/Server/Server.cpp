/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtrala <dtrala@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 23:44:30 by dtrala            #+#    #+#             */
/*   Updated: 2025/01/30 18:39:44 by dtrala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

#include <vector>

#include "BlockServer.hpp"
#include "ConfParser.hpp"

Server::Server() {};
Server::Server(const Server &other) { (void)other; };
Server &Server::operator=(const Server &other)
{
	if (this != &other)
	{
		(void)other;  // need to be replaced
	}
	return (*this);
};
Server::~Server() {};

void Server::start()
{
	this->setup();
	this->run();
};
void Server::setup() {
};
void Server::run()
{
	while (true)
	{
		break;
	}
};
