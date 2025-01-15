/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd_printf.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eviala <eviala@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 12:10:19 by eviala            #+#    #+#             */
/*   Updated: 2024/07/24 11:32:27 by eviala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putnbr_fd_printf(int n, int fd)
{
	int	count;

	count = 0;
	if (n == -2147483648)
	{
		count += ft_putstr_fd_printf("-2147483648", fd);
		return (count);
	}
	if (n < 0)
	{
		count += ft_putchar_fd_printf('-', fd);
		n = -n;
	}
	if (n >= 10)
	{
		count += ft_putnbr_fd_printf(n / 10, fd);
		count += ft_putchar_fd_printf(n % 10 + '0', fd);
	}
	else
		count += ft_putchar_fd_printf(n + '0', fd);
	return (count);
}
