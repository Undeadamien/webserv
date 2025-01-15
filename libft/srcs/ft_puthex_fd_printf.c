/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_puthex_fd_printf.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eviala <eviala@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 12:29:22 by eviala            #+#    #+#             */
/*   Updated: 2024/07/24 11:31:25 by eviala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_puthex_fd_printf(unsigned int n, char format, int fd)
{
	int	count;

	count = 0;
	if (n >= 16)
	{
		count += ft_puthex_fd_printf(n / 16, format, fd);
		if (format == 'X')
			count += ft_putchar_fd_printf("0123456789ABCDEF"[n % 16], fd);
		else
			count += ft_putchar_fd_printf("0123456789abcdef"[n % 16], fd);
	}
	else
	{
		if (format == 'X')
			count += ft_putchar_fd_printf("0123456789ABCDEF"[n], fd);
		else
			count += ft_putchar_fd_printf("0123456789abcdef"[n], fd);
	}
	return (count);
}
