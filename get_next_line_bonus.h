/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marschul <marschul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 15:59:08 by marschul          #+#    #+#             */
/*   Updated: 2023/10/20 18:28:12 by marschul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_BONUS_H
# define GET_NEXT_LINE_BONUS_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1024
# endif

# define FDS 4096

# include <stdlib.h>
# include <string.h>

typedef struct s_stash
{
	char	*buffer;
	char	*end;
	size_t	size;
}	t_stash;

typedef struct readbuffer
{
	char	*buffer;
	char	*start;
	size_t	size;
	ssize_t	read_bytes;
}	t_readbuffer;

typedef struct single
{
	int				single_status;
	t_stash			*stash_arr[FDS];
	t_readbuffer	*readbuffer_arr[FDS];
	int				status_arr[FDS];
}	t_single;

char	*get_next_line(int fd);
ssize_t	seek(t_readbuffer *readbuffer);
void	copy(char *newbuffer, char *oldbuffer, size_t size);
void	*stash_resize(t_stash *stash);
char	*stash_write(t_stash *stash, char *src, ssize_t size);
void	*gnl_malloc(t_single *single, int fd);
#endif