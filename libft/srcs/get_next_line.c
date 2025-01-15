/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eviala <eviala@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 10:27:53 by eviala            #+#    #+#             */
/*   Updated: 2024/10/16 14:29:11 by eviala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_clean_stash(char *the_line)
{
	char	*c;
	ssize_t	i;

	i = 0;
	while (the_line[i] != '\n' && the_line[i] != '\0')
		i++;
	if (the_line[i] == 0 || the_line[1] == 0)
		return (NULL);
	c = ft_substr(the_line, i + 1, ft_strlen(the_line) - i);
	if (!c)
		return (NULL);
	if (*c == 0)
	{
		free(c);
		c = NULL;
	}
	the_line[i + 1] = 0;
	return (c);
}

static char	*ft_fill_the_line(int fd, char *stash_c, char *buffer)
{
	ssize_t(read_bytes) = 1;
	if (stash_c && ft_strchr(stash_c, '\n'))
		return (stash_c);
	while (read_bytes > 0)
	{
		read_bytes = read(fd, buffer, BUFFER_SIZE);
		if (read_bytes < 0)
			return (NULL);
		else if (read_bytes == 0)
			break ;
		buffer[read_bytes] = '\0';
		if (!stash_c)
			stash_c = ft_strdup("");
		if (!stash_c)
			return (NULL);
		stash_c = ft_strjoin_free(stash_c, buffer);
		if (ft_strchr(buffer, '\n'))
			break ;
	}
	return (stash_c);
}

char	*get_next_line(int fd)
{
	static char	*stash_c[8192];
	char		*the_line;
	char		*buffer;

	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, 0, 0) < 0)
	{
		free(stash_c[fd]);
		stash_c[fd] = NULL;
		return (NULL);
	}
	buffer = (char *)malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!buffer)
		return (NULL);
	the_line = ft_fill_the_line(fd, stash_c[fd], buffer);
	free(buffer);
	if (!the_line)
	{
		free(stash_c[fd]);
		stash_c[fd] = NULL;
		return (NULL);
	}
	stash_c[fd] = ft_clean_stash(the_line);
	return (the_line);
}
