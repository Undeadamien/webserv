/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eviala <eviala@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 11:56:58 by eviala            #+#    #+#             */
/*   Updated: 2024/08/18 16:24:08 by eviala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

static int	count_words(const char *str, char c)
{
	int (i) = 0;
	int (trigger) = 0;
	while (*str)
	{
		if (*str != c && trigger == 0)
		{
			trigger = 1;
			i++;
		}
		else if (*str == c)
			trigger = 0;
		str++;
	}
	return (i);
}

static char	*word_dup(const char *str, int start, int finish)
{
	char	*word;
	int		i;

	i = 0;
	word = malloc((finish - start + 1) * sizeof(char));
	if (!word)
		return (NULL);
	while (start < finish)
		word[i++] = str[start++];
	word[i] = '\0';
	return (word);
}

char	**ft_split(char const *s, char c)
{
	if (!s)
		return (NULL);
	int (words_count) = count_words(s, c);
	char **(split) = malloc((words_count + 1) * sizeof(char *));
	if (!split)
		return (NULL);
	size_t(i) = -1;
	size_t(j) = 0;
	int (index) = -1;
	size_t(len) = ft_strlen(s);
	while (++i <= len)
	{
		if (s[i] != c && index < 0)
			index = i;
		else if ((s[i] == c || i == len) && index >= 0)
		{
			split[j] = word_dup(s, index, i);
			if (!split[j++])
			{
				return (ft_free_tab(split), NULL);
			}
			index = -1;
		}
	}
	return (split[j] = NULL, split);
}
