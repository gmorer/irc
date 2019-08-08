/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmorer <gmorer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/08 15:55:43 by gmorer            #+#    #+#             */
/*   Updated: 2019/08/08 15:56:08 by gmorer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

int	send_data(t_client *client)
{
	int			ret;
	t_response	*response;

	response = client->queue[0];
	ret = write(client->fd, response->buffer, response->message_length);
	if (ret == -1)
		return (error("write: "));
	remove_response(response);
	client->queue_len -= 1;
	memcpy(client->queue, &(client->queue[1]),
			sizeof(t_response) * client->queue_len);
	return (1);
}

int	output(t_client **clients, fd_set *writefs, int *activity)
{
	t_client	*tmp;

	tmp = *clients;
	while (*activity && tmp)
	{
		if (tmp->queue_len && FD_ISSET(tmp->fd, writefs))
		{
			send_data(tmp);
			*activity -= 1;
		}
		tmp = tmp->next;
	}
	return (1);
}
