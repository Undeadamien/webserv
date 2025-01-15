/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin_free.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eviala <eviala@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 12:51:46 by eviala            #+#    #+#             */
/*   Updated: 2024/09/12 13:25:25 by eviala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin_free(char *s1, char *s2)
{
	if (!s1 || !s2)
		return (NULL);
	char *(tmp_line) = s1;
	size_t (len) = (ft_strlen(s1) + ft_strlen(s2));
	size_t (i) = 0;
	char *(n_stash_c) = (char *)malloc(len + 1 * sizeof(char));
	if (!n_stash_c)
		return (NULL);
	while (i < len)
	{
		while (*s1)
			n_stash_c[i++] = *s1++;
		while (*s2)
			n_stash_c[i++] = *s2++;
	}
	n_stash_c[i] = '\0';
	free(tmp_line);
	return (n_stash_c);
}
