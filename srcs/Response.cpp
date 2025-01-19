/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtrala <dtrala@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 23:50:42 by dtrala            #+#    #+#             */
/*   Updated: 2025/01/19 23:51:37 by dtrala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response() {};
Response::Response(const Response &other) { (void)other; };
Response &Response::operator=(const Response &other)
{
    if (this != &other)
    {
        (void)other; // need to be replaced
    }
    return (*this);
};
Response::~Response() {};
