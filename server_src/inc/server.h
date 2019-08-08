/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmorer <gmorer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/08 12:57:56 by gmorer            #+#    #+#             */
/*   Updated: 2019/08/08 16:00:08 by gmorer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_H
# define SERVER_H

# include <sys/socket.h>
# include <sys/select.h>
# include <arpa/inet.h>
# include <sys/types.h>
# include <strings.h>
# include <string.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>

# define HELLO_MESSAGE "* Hello on the irc /help for help\n"

# define HELP_MSG1 "List of the commands:\n/who [channel]: list users in "
# define HELP_MSG2 "the channel\n/msg user: send private message to user"
# define HELP_MSG3 "\n/nick newNick: change your nickname\n/join channel:"
# define HELP_MSG4 "join the channel\n/leave: leave the channel\n/list: "
# define HELP_MSG5 "list the channels\n"
# define HELP_MESSAGE (HELP_MSG1 HELP_MSG2 HELP_MSG3 HELP_MSG4 HELP_MSG5)

# define DEFAULT_NICK "ghost"
# define PORT 8080
# define BACKLOG 10
# define BUFFER_LEN 512
# define NICK_LEN 9
# define CHANNEL_NAME_LEN 9
# define QUEUE_LEN 9
# define MIN(a,b) (((a)<(b))?(a):(b))

# define ACT1 {"who", action_who}
# define ACT2 {"msg", action_msg}
# define ACT3 {"nick", action_nick}
# define ACT4 {"join", action_join}
# define ACT5 {"leave", action_leave}
# define ACT6 {"list", action_list}
# define ACT7 {"help", action_help}
# define ACTION_ARRAY (t_action[]) {ACT1, ACT2, ACT3, ACT4, ACT5, ACT6, ACT7}

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
	int				queue_len;
	t_response		*queue[QUEUE_LEN];
	struct s_client	*next;
	struct s_client	*previous;
}				t_client;

typedef int	(*t_action_fn)(t_client **clients,
		t_client *client, t_response *message);

typedef struct	s_action
{
	char		*action_name;
	t_action_fn	action;
}				t_action;

char			*next_word(char *str, int len, int *word_len);
int				action_nick(t_client **clis, t_client *cli, t_response *resp);
int				action_join(t_client **clis, t_client *cli, t_response *resp);
int				action_who(t_client **clients, t_client *cli, t_response *resp);
int				action_msg(t_client **clients, t_client *cli, t_response *resp);
int				error(char *msg);
int				server_loop(int sockfd, t_client **clients);
int				rm_client(t_client **head, t_client *client);
t_client		*new_client(t_client **head, int fd);
int				add_to_queue(t_client *client, t_response *response);
int				remove_response(t_response *message);
void			set_message(t_response *response, char *message, size_t len);
int				send_to_channel(t_client **clis, char *chan, t_response *msg);
int				input(t_client **clis, fd_set *readfs, int *activity, int d);
int				output(t_client **clients, fd_set *writefs, int *activity);
int				action(t_client **clients, t_client *client, t_response *msg);
int				send_to_client(t_client *cli, t_response *resp, char *msg,
		size_t len);

#endif
