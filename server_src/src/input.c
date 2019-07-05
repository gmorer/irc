#include "server.h"

int get_input(t_client **head, t_client *client)
{
	int ret;

	ret = read(client->fd, client->buffer, BUFFER_LEN);
	if (ret < 0)
		return (error("read: "));
	if (ret == 0)
	{
		close(client->fd);
		rm_client(head, client);
		return (0);
	}
	client->data_len = ret;
	client->need_write = 1;
	return (1);
}

int master_sock(t_client **clients, int sockfd)
{
	int clifd;
	struct sockaddr_in cliaddr;
	unsigned int addrlen;

	addrlen = sizeof(struct sockaddr_in);
	clifd = accept(sockfd, (struct sockaddr *)&cliaddr, &addrlen);
	if (clifd < 0)
		return (error("accept :"));
	return new_client(clients, clifd);
}

int read_stuff(t_client **clients, fd_set *readfs, int *activity, int sockfd)
{
	t_client *tmp;

	tmp = *clients;
	if (FD_ISSET(sockfd, readfs))
	{
		master_sock(clients, sockfd);
		*activity -= 1;
	}
	while (*activity && tmp)
	{
		if (FD_ISSET(tmp->fd, readfs))
		{
			get_input(clients, tmp);
			*activity -= 1;
		}
		tmp = tmp->next;
	}
	return (1);
}
