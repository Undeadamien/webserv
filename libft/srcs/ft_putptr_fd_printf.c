/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putptr_fd_printf.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eviala <eviala@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 12:48:45 by eviala            #+#    #+#             */
/*   Updated: 2024/07/24 11:33:58 by eviala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	puthex_ptr(unsigned long n, char c, int fd)
{
	int	count;

	count = 0;
	if (n >= 16)
	{
		count += puthex_ptr(n / 16, c, fd);
		count += ft_putchar_fd_printf("0123456789abcdef"[n % 16], fd);
	}
	else
		count += ft_putchar_fd_printf("0123456789abcdef"[n], fd);
	return (count);
}

int	ft_putptr_fd_printf(unsigned long n, int fd)
{
	int	count;

	count = 0;
	if (n == 0)
	{
		count += ft_putstr_fd_printf("(nil)", fd);
		return (count);
	}
	count += ft_putstr_fd_printf("0x", fd);
	count += puthex_ptr(n, 'x', fd);
	return (count);
}
