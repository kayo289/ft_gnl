/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkikuchi <kkikuchi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/12 16:01:25 by kkikuchi          #+#    #+#             */
/*   Updated: 2020/11/04 01:44:21 by kkikuchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static void			*ft_memmove(void *buf1, const void *buf2, size_t n)
{
	size_t		i;

	if (!buf1 || !buf2)
		return (NULL);
	if (buf1 > buf2)
		while (n--)
			((char*)buf1)[n] = ((char*)buf2)[n];
	else
	{
		i = 0;
		while (i < n)
		{
			((char *)buf1)[i] = ((char *)buf2)[i];
			i++;
		}
	}
	return (buf1);
}

static char			*ft_strchr(char *buf, char c)
{
	while (*buf != '\0')
	{
		if (*buf == c)
			return (buf);
		buf++;
	}
	return (buf);
}

static char			*ft_strjoin(char const *s1, char const *s2, size_t s2_len)
{
	size_t		merge_len;
	size_t		s1_len;
	char		*str;
	size_t		index;
	size_t		i;

	if (!s1 || !s2)
		return (NULL);
	s1_len = (ft_strchr((char*)s1, '\0') - s1);
	merge_len = s1_len + s2_len;
	if (!(str = malloc((merge_len + 1) * sizeof(char))))
		return (NULL);
	index = 0;
	i = 0;
	while (i < s1_len)
		str[index++] = s1[i++];
	i = 0;
	while (i < s2_len)
		str[index++] = s2[i++];
	str[index] = '\0';
	return (str);
}

static int			get_line(int fd, char **buf, char **line)
{
	char	*p;
	int		len;

	p = ft_strchr(*buf, '\n');
	if (!(*line = ft_strjoin(*line, *buf, p - *buf)))
		return (ERROR);
	if (*p == '\n')
	{
		*buf = ft_memmove(*buf, p + 1, ft_strchr(p + 1, '\0') - p);
		return (SUCESS);
	}
	if ((len = read(fd, *buf, BUFFER_SIZE)) < 0)
		return (ERROR);
	(*buf)[len] = '\0';
	return (len == 0 ? END : get_line(fd, buf, line));
}

int					get_next_line(int fd, char **line)
{
	static char	*buf[1024];
	int			result;

	if (fd < 3 || BUFFER_SIZE < 1 || !(*line = (char *)malloc(1)))
		return (ERROR);
	*line[0] = '\0';
	if (!buf[fd] && !(buf[fd] = malloc((BUFFER_SIZE + 1) * sizeof(char))))
		return (ERROR);
	if ((result = get_line(fd, &buf[fd], line)) != SUCESS)
	{
		free(buf[fd]);
		buf[fd] = NULL;
	}
	return (result);
}
