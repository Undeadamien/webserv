/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtrala <dtrala@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 23:48:41 by dtrala            #+#    #+#             */
/*   Updated: 2025/01/19 23:51:37 by dtrala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request() {};
Request::Request(const Request &other) { (void)other; };
Request &Request::operator=(const Request &other)
{
    if (this != &other)
    {
        (void)other; // need to be replaced
    }
    return (*this);
};
Request::~Request() {};
