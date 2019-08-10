/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmorer <gmorer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/08 15:47:47 by gmorer            #+#    #+#             */
/*   Updated: 2019/08/10 11:10:32 by gmorer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

t_client	*new_client(t_client **head, int fd)
{
	t_client	*client;

	client = malloc(sizeof(*client));
	if (!client)
		return (0);
	ft_bzero(client, sizeof(*client));
	client->fd = fd;
	ft_strcpy(client->nick, DEFAULT_NICK);
	if (!(*head))
	{
		*head = client;
		client->previous = client;
		return (client);
	}
	(*head)->previous->next = client;
	client->previous = (*head)->previous;
	(*head)->previous = client;
	return (client);
}

void		remove_client(t_client *client)
{
	int index;

	close(client->fd);
	if (!client->queue_len)
		return ;
	index = 0;
	while (index < client->queue_len)
	{
		remove_response(client->queue[index]);
		index += 1;
	}
}

int			rm_client(t_client **head, t_client *client)
{
	if (client->previous && *head != client)
		client->previous->next = client->next;
	if (client->next)
		client->next->previous = client->previous;
	else
		(*head)->previous = client->previous;
	if (*head == client)
	{
		if (client->next)
			*head = client->next;
		else
			*head = NULL;
	}
	remove_client(client);
	free(client);
	return (1);
}
