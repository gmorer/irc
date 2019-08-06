#ifndef CLIENT_H
#define CLIENT_H

#include <stdio.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <strings.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ncurses.h>

#define INVALID_ADDR "Invalid address.\n"
#define BUFFER_SIZE 4096

/* connect.c */
int connect_to_server(int *fd, char *hostname, char *portstr);

/* window.c */
void display(WINDOW **screen, WINDOW **input);

/* getline.c */
int wgetline(WINDOW *input, char *buffer, size_t buffer_len);

#endif