/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd_printf.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eviala <eviala@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 12:11:44 by eviala            #+#    #+#             */
/*   Updated: 2024/09/09 15:06:42 by eviala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putstr_fd_printf(char *str, int fd)
{
	int	count;

	if (!str)
	{
		count = ft_putstr_fd_printf("(null)", fd);
		return (count);
	}
	count = 0;
	size_t (len) = ft_strlen(str);
	write(fd, str, len);
	return (count);
}
