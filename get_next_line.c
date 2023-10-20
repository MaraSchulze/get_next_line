/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marschul <marschul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 16:02:57 by marschul          #+#    #+#             */
/*   Updated: 2023/10/20 13:34:55 by marschul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "get_next_line.h"

char	*error_wrapper(t_stash *stash, t_readbuffer *readbuffer, \
			int *status, char *ret_value)
{
	if (ret_value != NULL && *status == 1)
		return (ret_value);
	free(stash->buffer);
	free(readbuffer->buffer);
	free(stash);
	free(readbuffer);
	if (ret_value != NULL && *status == 2)
		return (ret_value);
	*status = 0;
	return (NULL);
}

void	*init(t_stash **stash, t_readbuffer **readbuffer, int *status)
{
	if (*status == 0)
	{
		*stash = (t_stash *) malloc(sizeof(t_stash));
		if (*stash == NULL)
		{
			free(*stash);
			return (NULL);
		}
		*readbuffer = (t_readbuffer *) malloc(sizeof(t_readbuffer));
		if (*readbuffer == NULL)
		{
			free(*readbuffer);
			free(*stash);
			return (NULL);
		}
		(*stash)->buffer = malloc(BUFFER_SIZE);
		if ((*stash)->buffer == NULL)
		{
			free(*stash);
			free(*readbuffer);
			return (NULL);
		}
		(*readbuffer)->buffer = malloc(BUFFER_SIZE);
		if ((*stash)->buffer == NULL || (*readbuffer)->buffer == NULL)
		{
			free((*stash)->buffer);
			free(*stash);
			free(*readbuffer);
			return (NULL);
		}
		(*stash)->end = (*stash)->buffer;
		(*stash)->size = BUFFER_SIZE;
		(*readbuffer)->start = (*readbuffer)->buffer;
		(*readbuffer)->size = BUFFER_SIZE;
		(*readbuffer)->read_bytes = -1;
		*status = 1;
		return (*stash);
	}
	if (*status == 1)
		return (*stash);
	*status = 0;
	return (NULL);
}

char	*copy_rb_to_stash(t_stash *stash, t_readbuffer *readbuffer)
{
	ssize_t	size;

	if (readbuffer->read_bytes == -1)
		return (readbuffer->buffer);
	size = readbuffer->buffer + readbuffer->read_bytes - readbuffer->start;
	if (stash_write(stash, readbuffer->start, size) == NULL)
		return (NULL);
	readbuffer->start = readbuffer->buffer;
	readbuffer->read_bytes = -1;
	return (readbuffer->buffer);
}

char	*copy_stash_and_rb_to_retb(t_stash *stash, t_readbuffer *readbuffer, \
	ssize_t length2)
{
	char	*return_buffer;
	ssize_t	length1;

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
	static t_stash		*stash;
	static t_readbuffer	*readbuffer;
	static int			status;
	ssize_t				length;

	if (init(&stash, &readbuffer, &status) == NULL)
		return (NULL);
	while (readbuffer->read_bytes != 0)
	{
		length = seek(readbuffer);
		if (length != -1)
			return (error_wrapper(stash, readbuffer, &status, \
				copy_stash_and_rb_to_retb(stash, readbuffer, length)));
		else
			if (copy_rb_to_stash(stash, readbuffer) == NULL)
				return (error_wrapper(stash, readbuffer, &status, NULL));
		readbuffer->read_bytes = read(fd, readbuffer->buffer, BUFFER_SIZE);
		if (readbuffer->read_bytes == -1)
		{
			return (error_wrapper(stash, readbuffer, &status, NULL));
		}
	}
	status = 2;
	return (error_wrapper(stash, readbuffer, &status, \
		copy_stash_and_rb_to_retb(stash, readbuffer, readbuffer->read_bytes)));
}
