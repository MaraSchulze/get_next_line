/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marschul <marschul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 16:49:58 by marschul          #+#    #+#             */
/*   Updated: 2023/10/20 18:29:59 by marschul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <string.h>
#include "get_next_line_bonus.h"

ssize_t	seek(t_readbuffer *readbuffer)
{
	char	*pointer;

	if (readbuffer->read_bytes == -1)
		return (-1);
	pointer = readbuffer->start;
	while (pointer < readbuffer->buffer + readbuffer->read_bytes)
	{
		if (*pointer == '\n')
			return (pointer - readbuffer->start + 1);
		pointer++;
	}
	return (-1);
}

void	copy(char *newbuffer, char *oldbuffer, size_t size)
{
	while (size != 0)
	{
		*newbuffer = *oldbuffer;
		newbuffer++;
		oldbuffer++;
		size--;
	}
}

void	*stash_resize(t_stash *stash)
{
	size_t	old_buffer_size;
	char	*newbuffer;

	old_buffer_size = stash->size;
	newbuffer = malloc(2 * old_buffer_size);
	if (newbuffer == NULL)
		return (NULL);
	copy(newbuffer, stash->buffer, stash->end - stash->buffer);
	free(stash->buffer);
	stash->buffer = newbuffer;
	stash->end = stash->buffer + old_buffer_size;
	stash->size = 2 * old_buffer_size;
	return (stash->buffer);
}

char	*stash_write(t_stash *stash, char *src, ssize_t size)
{
	while (size != 0)
	{
		if (stash->end == stash->buffer + stash->size)
		{
			if (stash_resize(stash) == NULL)
				return (NULL);
		}
		*stash->end = *src;
		stash->end++;
		src++;
		size--;
	}
	return (stash->buffer);
}

void	*gnl_malloc(t_single *single, int fd)
{
	single->stash_arr[fd] = malloc(sizeof(t_stash));
	single->readbuffer_arr[fd] = malloc(sizeof(t_readbuffer));
	if (single->stash_arr[fd] == NULL || single->readbuffer_arr[fd] == NULL)
	{
		free(single->stash_arr[fd]);
		free(single->readbuffer_arr[fd]);
		return (NULL);
	}
	single->stash_arr[fd]->buffer = malloc(BUFFER_SIZE);
	single->readbuffer_arr[fd]->buffer = malloc(BUFFER_SIZE);
	if (single->stash_arr[fd]->buffer == NULL || \
		single->readbuffer_arr[fd]->buffer == NULL)
	{
		free(single->stash_arr[fd]->buffer);
		free(single->readbuffer_arr[fd]->buffer);
		free(single->stash_arr[fd]);
		free(single->readbuffer_arr[fd]);
		return (NULL);
	}
	return ((void *) 1);
}
