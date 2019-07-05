#include "server.h"

void fill_fd(t_client *clients, fd_set *readfds, fd_set *writefds, int *nfds)
{
	t_client *tmp;

	tmp = clients;
	while (tmp)
	{
		FD_SET(tmp->fd, readfds);
		if (tmp->need_write)
			FD_SET(tmp->fd, writefds);
		if (tmp->fd > *nfds)
			*nfds = tmp->fd;
		tmp = tmp->next;
	}
}

int server_loop(int sockfd)
{
	t_client *clients;
	fd_set readfds;
	fd_set writefds;
	int nfds;
	int activity;

	clients = NULL;
	while (1)
	{
		FD_ZERO(&readfds);
		FD_SET(sockfd, &readfds);
		nfds = sockfd;
		fill_fd(clients, &readfds, &writefds, &nfds);
		activity = select(nfds + 1, &readfds, &writefds, NULL, NULL);
		if (activity < 0)
			return (error("select: "));
		write_stuff(&clients, &writefds, &activity);
		read_stuff(&clients, &readfds, &activity, sockfd);
	}
	return (1);
}
