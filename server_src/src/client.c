#include "server.h"

// TODO: Do a circular list for perf

int new_client(t_client **head, int fd)
{
	t_client *new_client;

	new_client = malloc(sizeof(t_client));
	if (!new_client)
		return (0);
	bzero(new_client, sizeof(t_client));
	new_client->fd = fd;
	strcpy(new_client->channel, DEFAULT_CHANNEL);
	strcpy(new_client->nick, DEFAULT_NICK);
	if (!(*head))
	{
		*head = new_client;
		new_client->previous = new_client;
		return (1);
	}
	(*head)->previous->next = new_client;
	new_client->previous = (*head)->previous;
	(*head)->previous = new_client;
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
