#include "server.h"

t_response *get_input(t_client **clients, t_client *client)
{
	t_response	*new;
	int			ret;
	char		buffer[BUFFER_LEN];

	ret = read(client->fd, buffer, BUFFER_LEN);
	if (ret < 0)
	{
		error("read: ");
		return (NULL);
	}
	if (!(new = malloc(sizeof(t_response))))
		return (0);
	bzero(new, sizeof(t_response));
	if (ret == 0) // sometime it's not trigered
	{
		new->message_length = snprintf(new->buffer, sizeof(new->buffer),
			"* %s as disconnected.\n", client->nick);
		close(client->fd);
		rm_client(clients, client);
		send_to_channel(clients, "general", new);
		return (NULL);
	}
	memcpy(new->buffer, buffer, ret);
	new->message_length = MIN(ret, BUFFER_LEN - 11); // 11: nick len + ": " len
	return (new);
}

// this fonction does not send message,  it just put the message in the queue
void send_message(t_client **clients, t_client *client, t_response *message)
{
	t_client *tmp;
	size_t	nick_len;

	nick_len = strnlen(client->nick, 9);
	tmp = (*clients);
	memcpy(message->buffer + nick_len + 2, message->buffer, message->message_length);
	memcpy(message->buffer, client->nick, nick_len);
	memcpy(message->buffer + nick_len, ": ", 2);
	message->message_length += nick_len + 2;
	while (tmp)
	{
		if (strncmp(tmp->channel, client->channel, CHANNEL_NAME_LEN) == 0)
			add_to_queue(tmp, message);
		tmp = tmp->next;
	}
}

void execute_message(t_client **clients, t_client *client, t_response *message)
{
	// TODO: commands
	if (message->buffer[0] == '/')
		action(clients, client, message);
	else
		send_message(clients, client, message);
}

int master_sock(t_client **clients, int sockfd)
{
	int clifd;
	struct sockaddr_in cliaddr;
	unsigned int addrlen;

	addrlen = sizeof(struct sockaddr_in);
	clifd = accept(sockfd, (struct sockaddr *)&cliaddr, &addrlen);
	if (clifd < 0)
		return (error("accept :"));
	return new_client(clients, clifd);
}

int input(t_client **clients, fd_set *readfs, int *activity, int sockfd)
{
	t_client	*tmp;
	t_response	*message;
	t_client	*tmp_next;

	tmp = *clients;
	if (FD_ISSET(sockfd, readfs))
	{
		master_sock(clients, sockfd);
		*activity -= 1;
	}
	while (*activity && tmp)
	{
		tmp_next = tmp->next;
		if (FD_ISSET(tmp->fd, readfs))
		{
			message = get_input(clients, tmp);
			*activity -= 1;
			if (message)
				execute_message(clients, tmp, message);
		}
		tmp = tmp_next;
	}
	return (1);
}
