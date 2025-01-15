/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eviala <eviala@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 15:13:30 by eviala            #+#    #+#             */
/*   Updated: 2024/05/21 14:01:45 by eviala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*str;
	unsigned char	c_tmp;
	size_t			i;

	str = (unsigned char *)s;
	c_tmp = c;
	i = 0;
	while (i < n)
	{
		if (str[i] == c_tmp)
			return ((void *) &str[i]);
		i++;
	}
	return (NULL);
}
