/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putunbr_fd_printf.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eviala <eviala@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 12:27:16 by eviala            #+#    #+#             */
/*   Updated: 2024/07/24 11:31:25 by eviala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putunbr_fd_printf(unsigned int n, int fd)
{
	int	count;

	count = 0;
	if (n >= 10)
	{
		count += ft_putunbr_fd_printf(n / 10, fd);
		count += ft_putchar_fd_printf(n % 10 + '0', fd);
	}
	else
		count += ft_putchar_fd_printf(n + '0', fd);
	return (count);
}
