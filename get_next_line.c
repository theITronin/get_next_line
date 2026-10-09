/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbustama <dbustama@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/09 12:11:48 by dbustama          #+#    #+#             */
/*   Updated: 2026/10/09 18:55:22 by dbustama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

void	*get_stash(char *stash, char *str, char *line)
{
	size_t	len_l;
	size_t	len;

	len_l = ft_strlen(line);
	len = ft_strlen(str);
	stash = ft_substr(str, len_l, len - len_l);
	free (str);
	return (stash);
}

char	*get_lines(char	*str)
{
	int		l_line;
	char	*line;

	l_line = ft_strchr(str, '\n');
	line = ft_substr(str, 0, l_line);
	if (!line)
		return (NULL);
	return (line);
}

char	*get_str(int fd, char *str)
{
	char	*new;
	ssize_t	buffer;

	if (!str)
	{
		str = malloc((BUFFER_SIZE + 1) * sizeof(char));
		if (!str)
			return (NULL);
		buffer = read(fd, str, BUFFER_SIZE);
	}
	new = malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!new)
	{
		free (str);
		return (NULL);
	}
	buffer = read(fd, new, BUFFER_SIZE);
	if (buffer < 0)
		return (NULL);
	if (buffer == 0)
		return (str);
	str = ft_strjoin(str, new);
	if (ft_strchr(str, '\n') == -1)
		get_str(fd, str);
	return (str);
}

char	*get_next_line(int fd)
{
	static char	*stash[1024];
	char		*str;
	char		*line;

	if (!fd || fd < 0)
		return (NULL);
	str = NULL;
	str = get_str(fd, str);
	if (!str)
		return (NULL);
	line = get_lines(str);
	if (!line)
		return (NULL);
	stash[fd] = get_stash(stash[fd], str, line);
	return (line);
}
