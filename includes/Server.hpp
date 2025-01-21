/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtrala <dtrala@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 23:44:36 by dtrala            #+#    #+#             */
/*   Updated: 2025/01/19 23:47:57 by dtrala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

class Server
{
public:
    Server();
    Server(const Server &other);
    Server &operator=(const Server &other);
    virtual ~Server();

protected:
private:
};

#endif
