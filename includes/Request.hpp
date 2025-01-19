/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtrala <dtrala@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 23:49:03 by dtrala            #+#    #+#             */
/*   Updated: 2025/01/19 23:51:37 by dtrala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
#define REQUEST_HPP

class Request
{
public:
    Request();
    Request(const Request &other);
    Request &operator=(const Request &other);
    virtual ~Request();

protected:
private:
};

#endif
