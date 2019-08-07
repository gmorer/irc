#ifndef CLIENT_H
#define CLIENT_H

#include <sys/types.h>
#include <sys/uio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <strings.h>
#include <stdlib.h>
#include <netdb.h>

#define CLEAR "\033c"
#define INVALID_ADDR "Invalid address.\n"
#define BUFFER_SIZE 4096
#define RED "\033[0;31m"
#define BLUE "\033[0;34m"
#define DFLT "\033[0m"
#define BOLD "\033[1m"
#define gotoxy(x,y) printf("\033[%d;%dH", (y), (x))

typedef struct	s_norme
{
	char	**screen;
	int		height;
	int		width;
	int		fd;
	int		mem_index;
	int		scr_index;
}				t_norme;

/* main */
void add_to_screen(t_norme *norme, char *str);

/* connect.c */
int connect_to_server(int *fd, char *hostname, char *portstr);

/* screen.c */
void drawline(int y, int width);
void clearline(int y, int width);
void clear(void);
void put_str(char *str);

/* command.c */
int howmuchline(char *str);
void ft_connect(t_norme *norme, char *str);
void ft_disconnect(t_norme *norme);
void ft_help(t_norme *norme);

#endif