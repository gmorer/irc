/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmorer <gmorer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/08 15:49:06 by gmorer            #+#    #+#             */
/*   Updated: 2019/08/10 11:59:21 by gmorer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

t_response	*get_input(t_client **clients, t_client *client)
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
	if (!(new = malloc(sizeof(*new))))
		return (0);
	ft_bzero(new, sizeof(*new));
	if (ret == 0)
	{
		new->message_length = snprintf(new->buffer, sizeof(new->buffer),
			"* %s as disconnected.\n", client->nick);
		close(client->fd);
		rm_client(clients, client);
		send_to_channel(clients, "general", new);
		return (NULL);
	}
	ft_memcpy(new->buffer, buffer, ret);
	new->message_length = MIN(ret, BUFFER_LEN - 11);
	return (new);
}

void		send_message(t_client **clients, t_client *client, t_response *msg)
{
	t_client	*tmp;
	size_t		nick_len;

	nick_len = ft_strlen(client->nick);
	tmp = (*clients);
	ft_memmove(msg->buffer + nick_len + 2, msg->buffer, msg->message_length);
	ft_memcpy(msg->buffer, client->nick, nick_len);
	ft_memcpy(msg->buffer + nick_len, ": ", 2);
	msg->message_length += nick_len + 2;
	send_to_channel(clients, client->channel, msg);
}

void		execute_message(t_client **clients, t_client *cli, t_response *msg)
{
	if (msg->buffer[0] == '/')
		action(clients, cli, msg);
	else
		send_message(clients, cli, msg);
}

void		master_sock(t_client **clients, int sockfd)
{
	t_client			*client;
	t_response			*response;
	int					clifd;
	struct sockaddr_in	cliaddr;
	unsigned int		addrlen;

	addrlen = sizeof(cliaddr);
	clifd = accept(sockfd, (struct sockaddr *)&cliaddr, &addrlen);
	if (clifd < 0)
		return ;
	client = new_client(clients, clifd);
	if ((response = malloc(sizeof(*response))))
		send_to_client(client, response, HELLO_MESSAGE, sizeof(HELLO_MESSAGE));
}

int			input(t_client **clients, fd_set *readfs, int *activity, int sockfd)
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
