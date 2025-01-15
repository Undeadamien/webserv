/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eviala <eviala@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 10:23:00 by eviala            #+#    #+#             */
/*   Updated: 2024/09/19 13:35:57 by eviala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_whichprint(int fd, const char *str, int *i, va_list args)
{
	int	count;

	count = 0;
	(*i)++;
	if (str[*i] == 'c')
		count += ft_putchar_fd_printf(va_arg(args, int), fd);
	else if (str[*i] == 's')
		count += ft_putstr_fd_printf(va_arg(args, char *), fd);
	else if (str[*i] == 'd' || str[*i] == 'i')
		count += ft_putnbr_fd_printf(va_arg(args, int), fd);
	else if (str[*i] == 'u')
		count += ft_putunbr_fd_printf(va_arg(args, unsigned int), fd);
	else if (str[*i] == 'x')
		count += ft_puthex_fd_printf(va_arg(args, unsigned int), 'x', fd);
	else if (str[*i] == 'X')
		count += ft_puthex_fd_printf(va_arg(args, unsigned int), 'X', fd);
	else if (str[*i] == 'p')
		count += ft_putptr_fd_printf(va_arg(args, unsigned long), fd);
	else if (str[*i] == '%')
		count += ft_putchar_fd_printf('%', fd);
	return (count);
}

int	ft_printf(int fd, const char *str, ...)
{
	va_list	args;

	int (i) = 0;
	int (count) = 0;
	if (!str)
		return (-1);
	va_start(args, str);
	while (str[i])
	{
		if (str[i] == '%')
			count += ft_whichprint(fd, str, &i, args);
		else
			count += ft_putchar_fd_printf(str[i], fd);
		i++;
	}
	va_end(args);
	return (count);
}
