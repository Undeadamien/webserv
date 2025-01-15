/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eviala <eviala@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 14:00:26 by eviala            #+#    #+#             */
/*   Updated: 2024/09/12 13:47:15 by dtrala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	const char	*s_tmp;
	char		*d_tmp;
	size_t		i;

	if (!dst || !src)
		return ((void *)0);
	s_tmp = (const char *)src;
	d_tmp = (char *)dst;
	i = 0;
	if (d_tmp > s_tmp)
		while (len-- > 0)
			d_tmp[len] = s_tmp[len];
	else
	{
		while (i < len)
		{
			d_tmp[i] = s_tmp[i];
			i++;
		}
	}
	return (dst);
}
