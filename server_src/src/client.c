#include "server.h"

int new_client(t_client **head, int fd)
{
	t_client *new_client;
	t_client *tmp;

	new_client = malloc(sizeof(t_client));
	if (!new_client)
		return (0);
	bzero(new_client, sizeof(t_client));
	new_client->fd = fd;
	if (!(*head))
	{
		*head = new_client;
		return (1);
	}
	tmp = *(head);
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_client;
	new_client->previous = tmp;
	return (1);
}

void remove_client(t_client *client)
{
	close(client->fd);
}

int rm_client(t_client **head, t_client *client)
{
	if (client->previous)
		client->previous->next = client->next;
	if (client->next)
		client->next->previous = client->previous;
	if (*head == client)
		*head = NULL;
	remove_client(client);
	free(client);
	return (1);
}
