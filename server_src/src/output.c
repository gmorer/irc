#include "server.h"

int send_data(t_client *client)
{
	int			ret;
	t_response	*response;

	response = client->queue[0];
	// printf("sending %p for %p\n", response);
	ret = write(client->fd, response->buffer, response->message_length);
	if (ret == -1)
		return (error("write: "));
	remove_response(response);
	client->queue_len -= 1;
	// printf("array addr %p\nnew addr %p\ncopied: %d\n", client->queue, &client->queue[1], sizeof(t_response) * client->queue_len);
	memcpy(client->queue, &(client->queue[1]), sizeof(t_response) * client->queue_len);
	return (1);
}

int output(t_client **clients, fd_set *writefs, int *activity)
{
	t_client *tmp;

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