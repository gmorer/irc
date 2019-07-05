#include "server.h"

int send_data(t_client *client)
{
	int ret;

	ret = write(client->fd, client->buffer, client->data_len);
	if (ret == -1)
		return (error("write: "));
	client->need_write = 0;
	return (1);
}

int write_stuff(t_client **clients, fd_set *writefs, int *activity)
{
	t_client *tmp;

	tmp = *clients;
	while (*activity && tmp)
	{
		if (tmp->need_write && FD_ISSET(tmp->fd, writefs))
		{
			send_data(tmp);
			*activity -= 1;
		}
		tmp = tmp->next;
	}
	return (1);
}