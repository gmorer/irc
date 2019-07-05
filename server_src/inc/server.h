#ifndef SERVER_H
#define SERVER_H

#include <sys/socket.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#define PORT 8080
#define BACKLOG 10
#define BUFFER_LEN 1024

typedef struct s_client
{
	int fd;
	char need_write;
	char buffer[BUFFER_LEN]; // do we need a buffer per client?
	size_t data_len;
	struct s_client *next;
	struct s_client *previous;
} t_client;

/* main.c */
int error(char *msg);

/* loop.c */
int server_loop(int sockfd);

/*  client.c */
int new_client(t_client **head, int fd);
int rm_client(t_client **head, t_client *client);

/* input.c */
int read_stuff(t_client **clients, fd_set *readfs, int *activity, int sockfd);

/* output.c */
int write_stuff(t_client **clients, fd_set *writefs, int *activity);

#endif
