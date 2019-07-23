#include "server.h"

int remove_response(t_response *response)
{
	// printf("freeing %p\n", response);
	response->activity -= 1;
	if (response->activity)
		return (0);
	free(response);
	return (1);
}

int add_to_queue(t_client *client, t_response *response)
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

void set_message(t_response *response, char *message, size_t message_len)
{
	bzero(response->buffer, sizeof(response->buffer));
	memcpy(response->buffer, message, message_len);
	response->message_length = message_len;
	response->activity = 0;
}

int send_to_channel(t_client **clients, char *channel, t_response *message)
{
	t_client *tmp;

	tmp = (*clients);
	while (tmp)
	{
		if (strncmp(tmp->channel, channel, CHANNEL_NAME_LEN) == 0)
			add_to_queue(tmp, message);
		tmp = tmp->next;
	}
}