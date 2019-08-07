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
#define gotoxy(x,y) printf("\033[%d;%dH", (y), (x))

/* connect.c */
int connect_to_server(int *fd, char *hostname, char *portstr);

/* screen.c */
void drawline(int y, int width);
void clearline(int y, int width);
void clear(void);

#endif