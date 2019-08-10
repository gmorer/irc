/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmorer <gmorer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/08 12:22:33 by gmorer            #+#    #+#             */
/*   Updated: 2019/08/10 10:59:16 by gmorer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_H
# define CLIENT_H

# include <sys/types.h>
# include <sys/uio.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <sys/select.h>
# include <sys/ioctl.h>
# include <stdio.h>
# include <unistd.h>
# include <strings.h>
# include <stdlib.h>
# include <netdb.h>
# include "libft.h"

# define HELP_STR_1		"Commands:\n/connect [hostname] [port]: connect"
# define HELP_STR_2		"to a server\n/disconnect: disconnect from a server\n"
# define HELP_STR		(HELP_STR_1 HELP_STR_2)
# define CLEAR			"\033c"
# define INVALID_ADDR	"Invalid address.\n"
# define BUFFER_SIZE	4096
# define RED			"\033[0;31m"
# define BLUE			"\033[0;34m"
# define DFLT			"\033[0m"
# define BOLD			"\033[1m"
# define GOTOXY(x,y)	printf("\033[%d;%dH", (y), (x))

typedef struct	s_norme
{
	char		**screen;
	int			height;
	int			width;
	int			fd;
	int			mem_index;
	int			scr_index;
}				t_norme;

void			add_to_screen(t_norme *norme, char *str);
int				connect_to_server(int *fd, char *hostname, char *portstr);
void			drawline(int y, int width);
void			clearline(int y, int width);
void			clear(void);
void			put_str(char *str);
int				howmuchline(char *str);
void			ft_connect(t_norme *norme, char *str);
void			ft_disconnect(t_norme *norme);
void			ft_help(t_norme *norme);

#endif
