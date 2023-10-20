/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marschul <marschul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 16:02:57 by marschul          #+#    #+#             */
/*   Updated: 2023/10/20 18:07:22 by marschul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "get_next_line_bonus.h"

char	*error_wrapper(t_single *single, int fd, char *ret_value)
{
	t_stash			*stash;
	t_readbuffer	*readbuffer;
	int				status;

	stash = single->stash_arr[fd];
	readbuffer = single->readbuffer_arr[fd];
	status = single->status_arr[fd];
	if (ret_value != NULL && status == 1)
		return (ret_value);
	free(stash->buffer);
	free(readbuffer->buffer);
	free(stash);
	free(readbuffer);
	if (ret_value != NULL && status == 2)
		return (ret_value);
	single->status_arr[fd] = 0;
	return (NULL);
}

void	*init(t_single *single, int fd)
{
	t_stash			*stash;
	t_readbuffer	*readbuffer;
	int				status;

	status = single->status_arr[fd];
	if (status == 0)
	{
		if (gnl_malloc(single, fd) == NULL)
			return (NULL);
		stash = single->stash_arr[fd];
		readbuffer = single->readbuffer_arr[fd];
		stash->end = stash->buffer;
		stash->size = BUFFER_SIZE;
		readbuffer->start = readbuffer->buffer;
		readbuffer->size = BUFFER_SIZE;
		readbuffer->read_bytes = -1;
		single->status_arr[fd] = 1;
		return ((void *) 1);
	}
	if (status == 1)
		return ((void *) 1);
	single->status_arr[fd] = 0;
	return (NULL);
}

char	*copy_rb_to_stash(t_single single, int fd)
{
	ssize_t			size;
	t_stash			*stash;
	t_readbuffer	*readbuffer;

	stash = single.stash_arr[fd];
	readbuffer = single.readbuffer_arr[fd];
	if (readbuffer->read_bytes == -1)
		return (readbuffer->buffer);
	size = readbuffer->buffer + readbuffer->read_bytes - readbuffer->start;
	if (stash_write(stash, readbuffer->start, size) == NULL)
		return (NULL);
	readbuffer->start = readbuffer->buffer;
	readbuffer->read_bytes = -1;
	return (readbuffer->buffer);
}

char	*copy_stash_and_rb_to_retb(t_single single, int fd, ssize_t length2)
{
	char			*return_buffer;
	ssize_t			length1;
	t_stash			*stash;
	t_readbuffer	*readbuffer;

	stash = single.stash_arr[fd];
	readbuffer = single.readbuffer_arr[fd];
	length1 = stash->end - stash->buffer;
	if (length1 + length2 == 0)
		return (NULL);
	return_buffer = malloc(length1 + length2 + 1);
	if (return_buffer == NULL)
		return (NULL);
	copy(return_buffer, stash->buffer, length1);
	stash->end = stash->buffer;
	copy(return_buffer + length1, readbuffer->start, length2);
	readbuffer->start += length2;
	if (readbuffer->start == readbuffer->buffer + readbuffer->size)
	{
		readbuffer->start = readbuffer->buffer;
		readbuffer->read_bytes = -1;
	}
	return_buffer[length1 + length2] = '\0';
	return (return_buffer);
}

char	*get_next_line(int fd)
{
	static t_single	single;
	ssize_t			length;

	if (init(&single, fd) == NULL)
		return (NULL);
	while (single.readbuffer_arr[fd]->read_bytes != 0)
	{
		length = seek(single.readbuffer_arr[fd]);
		if (length != -1)
			return (error_wrapper(&single, fd, \
				copy_stash_and_rb_to_retb(single, fd, length)));
		else
			if (copy_rb_to_stash(single, fd) == NULL)
				return (error_wrapper(&single, fd, NULL));
		single.readbuffer_arr[fd]->read_bytes = \
			read(fd, single.readbuffer_arr[fd]->buffer, BUFFER_SIZE);
		if (single.readbuffer_arr[fd]->read_bytes == -1)
			return (error_wrapper(&single, fd, NULL));
	}
	single.status_arr[fd] = 2;
	return (error_wrapper(&single, fd, copy_stash_and_rb_to_retb \
		(single, fd, single.readbuffer_arr[fd]->read_bytes)));
}
