/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlon.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eviala <eviala@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 12:25:14 by eviala            #+#    #+#             */
/*   Updated: 2024/06/25 11:27:03 by eviala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <limits.h> // Pour LONG_MAX et LONG_MIN

static const char	*skip_whitespace_and_sign(const char *str, int *sign)
{
	while (*str == ' ' || (*str >= '\t' && *str <= '\r'))
		str++;
	*sign = 1;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			*sign = -1;
		str++;
	}
	return (str);
}

static const char	*determine_base(const char *str, int *base)
{
	if (*base == 0)
	{
		if (*str == '0')
		{
			if (str[1] == 'x' || str[1] == 'X')
			{
				*base = 16;
				str += 2;
			}
			else
				*base = 8;
		}
		else
			*base = 10;
	}
	else if (*base == 16 && *str == '0' && (str[1] == 'x' || str[1] == 'X'))
		str += 2;
	return (str);
}

static int	convert_char_to_digit(char c, int base)
{
	int	digit;

	if (c >= '0' && c <= '9')
		digit = c - '0';
	else if (c >= 'a' && c <= 'z')
		digit = c - 'a' + 10;
	else if (c >= 'A' && c <= 'Z')
		digit = c - 'A' + 10;
	else
		return (-1);
	if (digit >= base)
		return (-1);
	return (digit);
}

static int	is_int_or_max(long result, int sign)
{
	if (result > (LONG_MAX - 7) / 10)
	{
		return (1);
	}
	if (result == LONG_MAX && sign == 1)
	{
		return (1);
	}
	if (result == LONG_MIN && sign == -1)
	{
		return (1);
	}
	return (0);
}

long	ft_strlon(const char *str, char **endptr, int base)
{
	int		sign;
	int		digit;

	long (result) = 0;
	str = skip_whitespace_and_sign(str, &sign);
	str = determine_base(str, &base);
	while (*str)
	{
		digit = convert_char_to_digit(*str, base);
		if (digit == -1)
			break ;
		if (result > (LONG_MAX - digit) / base)
		{
			if (endptr)
				*endptr = (char *)str;
			return (is_int_or_max(result, sign));
		}
		result = result * base + digit;
		str++;
	}
	if (endptr)
		*endptr = (char *)str;
	return (sign * result);
}
