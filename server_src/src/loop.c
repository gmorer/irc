#include "server.h"

void fill_fd(t_client **clients, fd_set *readfds, fd_set *writefds, int *nfds)
{
	t_client *tmp;

	tmp = *clients;
	while (tmp)
	{
		FD_SET(tmp->fd, readfds);
		if (tmp->queue_len)
			FD_SET(tmp->fd, writefds);
		if (tmp->fd > *nfds)
			*nfds = tmp->fd;
		tmp = tmp->next;
	}
}

int server_loop(int sockfd, t_client **clients)
{
	fd_set readfds;
	fd_set writefds;
	int nfds;
	int activity;

	while (1)
	{
		FD_ZERO(&readfds);
		FD_ZERO(&writefds);
		FD_SET(sockfd, &readfds);
		nfds = sockfd;
		fill_fd(clients, &readfds, &writefds, &nfds);
		activity = select(nfds + 1, &readfds, &writefds, NULL, NULL);
		if (activity < 0)
			return (error("select: "));
		output(clients, &writefds, &activity);
		input(clients, &readfds, &activity, sockfd);

	}
	return (1);
}
