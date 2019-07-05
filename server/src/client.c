#include "server.h"

int	new_client(t_client **head, int fd)
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

int rm_from_fd(t_client **head, int fd)
{
	t_client *tmp;

	tmp = *head;
	while (tmp->fd != fd && tmp->next)
		tmp = tmp->next;
	if (!tmp->next && tmp->fd != fd)
		return (0);
	if (tmp->previous)
		tmp->previous->next = tmp->next;
	if (tmp->next)
		tmp->next->previous = tmp->previous;
	remove_client(tmp);
	free(tmp);
}
