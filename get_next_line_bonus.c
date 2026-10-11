/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbustama <dbustama@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/09 12:11:48 by dbustama          #+#    #+#             */
/*   Updated: 2026/10/11 04:00:24 by dbustama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

size_t	len_line(char *rest)
{
	size_t	len;

	len = 0;
	while (rest[len] && rest[len] != '\n')
		len++;
	if (rest[len] == '\n')
		return (len + 1);
	return (len);
}

char	*memory_free(char *new, char **rest)
{
	if (new)
		free(new);
	if (rest && *rest)
	{
		free(*rest);
		*rest = NULL;
	}
	return (NULL);
}

char	*get_lines(char	**rest, size_t len_l)
{
	char	*line;
	char	*new_rest;

	line = ft_substr(*rest, 0, len_l);
	if (!line)
		return (memory_free(NULL, rest));
	new_rest = ft_substr(*rest, len_l, ft_strlen(*rest) - len_l);
	if (!new_rest)
		return (memory_free(line, rest));
	free(*rest);
	*rest = new_rest;
	return (line);
}

char	*read_buff(int fd, char *rest)
{
	char	*new;
	ssize_t	bytes_read;
	char	*tmp;

	new = malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!new)
		return (memory_free(NULL, &rest));
	bytes_read = 1;
	while (ft_strchr(rest, '\n') == NULL && bytes_read > 0)
	{
		bytes_read = read(fd, new, BUFFER_SIZE);
		if (bytes_read < 0)
			return (memory_free(new, &rest));
		new[bytes_read] = '\0';
		if (new[0] == '\0' && !rest)
			return (memory_free(new, &rest));
		tmp = ft_strjoin(rest, new);
		if (!tmp)
			return (memory_free(new, &rest));
		rest = tmp;
	}
	free(new);
	return (rest);
}

char	*get_next_line(int fd)
{
	static char	*rest[1024];
	char		*line;
	int			len_l;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (ft_strchr(rest[fd], '\n') == NULL)
		rest[fd] = read_buff(fd, rest[fd]);
	if (!rest[fd] || rest[fd][0] == '\0')
		return (memory_free(NULL, &rest[fd]));
	len_l = len_line(rest[fd]);
	line = get_lines(&rest[fd], len_l);
	if (!line)
		return (NULL);
	return (line);
}
