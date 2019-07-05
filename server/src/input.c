#include "server.h"


int master_sock(t_client **clients, int sockfd)
{
	int			lifd;
	struct sockaddr_in	cliaddr;
	size_t			addrlen;

	addrlen = sizeof(struct sockaddr_in);
	clifd = accept(sockfd, (struct sockaddr*)&cliaddr, &addrlen);
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
			//get_input();
			*activity -= 1;
		}
		tmp = tmp->next;
	}
	return (1);
}
