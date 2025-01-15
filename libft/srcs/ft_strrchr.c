/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eviala <eviala@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 14:51:15 by eviala            #+#    #+#             */
/*   Updated: 2024/07/20 10:13:16 by eviala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char	*str;

	str = (char *)s + ft_strlen(s);
	while (str >= s)
	{
		if (*str == (char)c)
			return (str);
		str--;
	}
	return (NULL);
}

int	ft_strrchr_str(const char *haystack, const char *needle)
{
	int	haystack_len;
	int	needle_len;
	int	i;

	haystack_len = ft_strlen(haystack);
	needle_len = ft_strlen(needle);
	if (needle_len == 0)
		return (haystack_len);
	i = haystack_len - needle_len;
	while (i >= 0)
	{
		if (ft_strncmp(&haystack[i], needle, needle_len) == 0)
			return (i);
		i--;
	}
	return (-1);
}
