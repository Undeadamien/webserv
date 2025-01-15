/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eviala <eviala@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 15:26:56 by eviala            #+#    #+#             */
/*   Updated: 2024/11/04 12:23:28 by eviala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	char	cc;
	size_t	i;

	i = 0;
	cc = (char)c;
	while (s[i] != '\0')
	{
		if (s[i] == cc)
			return ((char *) &s[i]);
		i++;
	}
	if (s[i] == cc)
		return ((char *) &s[i]);
	return (NULL);
}

char	*ft_strchr_char(char *str, char *cmp)
{
	int		i;
	int		y;

	i = 0;
	if (!str || !str[i] || !cmp)
		return (NULL);
	while (str[i])
	{
		y = 0;
		while (str[i + y] == cmp[y] && str[i + y] && cmp[y])
			y++;
		if (cmp[y] == '\0')
			return (&str[i + y]);
		i++;
	}
	return (NULL);
}

int	ft_strchr_char_ref(char *str, char *cmp, char **result)
{
	int		i;
	int		y;

	i = 0;
	*result = NULL;
	if (!str || !cmp)
		return (0);
	while (str[i])
	{
		y = 0;
		while (str[i + y] == cmp[y] && str[i + y] && cmp[y])
			y++;
		if (cmp[y] == '\0')
		{
			*result = &str[i + y];
			return (1);
		}
		i++;
	}
	return (0);
}
