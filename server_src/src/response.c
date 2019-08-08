/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmorer <gmorer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/08 15:56:21 by gmorer            #+#    #+#             */
/*   Updated: 2019/08/08 15:57:42 by gmorer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

int		remove_response(t_response *response)
{
	response->activity -= 1;
	if (response->activity > 0)
		return (0);
	free(response);
	return (1);
}

int		add_to_queue(t_client *client, t_response *response)
{
	int index;

	index = client->queue_len;
	if (index == QUEUE_LEN)
	{
		dprintf(2, "Error the queue is full\n");
		return (0);
	}
	client->queue[index] = response;
	client->queue_len += 1;
	response->activity += 1;
	return (1);
}

void	set_message(t_response *response, char *message, size_t message_len)
{
	bzero(response->buffer, sizeof(response->buffer));
	memcpy(response->buffer, message, message_len);
	response->message_length = message_len;
	response->activity = 0;
}

int		send_to_channel(t_client **clients, char *channel, t_response *message)
{
	t_client	*tmp;

	tmp = (*clients);
	message->activity = 0;
	if (!channel[0])
	{
		remove_response(message);
		return (0);
	}
	while (tmp)
	{
		if (strncmp(tmp->channel, channel, CHANNEL_NAME_LEN) == 0)
			add_to_queue(tmp, message);
		tmp = tmp->next;
	}
	if (!message->activity)
		remove_response(message);
	return (1);
}

int		send_to_client(t_client *cli, t_response *resp, char *msg, size_t len)
{
	set_message(resp, msg, len);
	add_to_queue(cli, resp);
	return (0);
}
