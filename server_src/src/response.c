#include "server.h"

int remove_response(t_response *response)
{
	// printf("freeing %p\n", response);
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