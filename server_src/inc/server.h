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

#define DEFAULT_CHANNEL "general"
#define DEFAULT_NICK "ghost"
#define PORT 8080
#define BACKLOG 10
#define BUFFER_LEN 512
#define NICK_LEN 9
#define CHANNEL_NAME_LEN 9
#define QUEUE_LEN 9
#define MIN(a,b) (((a)<(b))?(a):(b))

#define ACTION_ARRAY (t_action[]) {	\
	{"who", action_who},			\
	{"msg", action_msg},			\
	{"nick", action_nick},			\
	{"join", action_join},			\
	{"leave", action_leave}			\
}

typedef struct	s_reponse
{
	char				buffer[BUFFER_LEN];
	size_t				message_length;
	int					activity;
}				t_response;

typedef struct	s_client
{
	int				fd;
	char			nick[NICK_LEN];
	char			channel[CHANNEL_NAME_LEN];
	char			buffer[BUFFER_LEN]; // do we need a buffer per client?
	int				queue_len;
	t_response		*queue[QUEUE_LEN];
	struct s_client	*next;
	struct s_client	*previous;
}				t_client;

typedef int (*action_fn)(t_client **clients, t_client *client, t_response *message);

typedef struct	s_action
{
	char		*action_name;
	action_fn	action;
}				t_action;


/* main.c */
int error(char *msg);

/* loop.c */
int server_loop(int sockfd, t_client **clients);

/*  client.c */
int new_client(t_client **head, int fd);
int rm_client(t_client **head, t_client *client);

/* response.c */
int add_to_queue(t_client *client, t_response *response);
int remove_response(t_response *message);
void set_message(t_response *response, char *message, size_t message_len);
int send_to_channel(t_client **clients, char *channel, t_response *message);
int send_to_client(t_client *client, t_response *response, char *msg, size_t msg_len);

/* input.c */
int input(t_client **clients, fd_set *readfs, int *activity, int sockfd);

/* output.c */
int output(t_client **clients, fd_set *writefs, int *activity);

/* action.c */
int action(t_client **clients, t_client *client, t_response *message);

#endif
