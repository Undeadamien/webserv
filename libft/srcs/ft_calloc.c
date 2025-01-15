/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eviala <eviala@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 12:33:59 by eviala            #+#    #+#             */
/*   Updated: 2024/05/22 10:04:45 by eviala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nb, size_t size_type)
{
	void	*res;

	if (size_type && SIZE_MAX / size_type < nb)
		return (NULL);
	res = (void *)malloc(nb * size_type);
	if (!res)
		return (NULL);
	ft_bzero(res, nb * size_type);
	return (res);
}
