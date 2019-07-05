#include "server.h"

void do_stuff(int clifd)
{
	size_t	len;
	char	buffer[BUFFER_LEN];

	while ((len = read(clifd, buffer, BUFFER_LEN - 1)) > 0)
	{
		buffer[len] = 0;
		write(1, buffer, len);
		write(clifd, buffer, len);
	}
	close(clifd);
}

int error(char *message)
{
	perror(message);
	return (EXIT_FAILURE);
}

int init_server()
{
	struct sockaddr_in sockaddr;
	int sockfd;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
		return (error("Opening socket: "));
	bzero(&sockaddr, sizeof(struct sockaddr_in));
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_addr.s_addr= htonl(INADDR_ANY);
	sockaddr.sin_port = htons(PORT);
	if (bind(sockfd, (struct sockaddr *)&sockaddr, sizeof(sockaddr)) == -1)
		return (error("Binding socket: "));
	if (listen(sockfd, BACKLOG) == -1)
		return (error("Listening: "));
	return (sockfd);
}


int server_loop(int sockfd)
{
	t_client	*clients;
	fd_set		readfds;
	int		max_sd;
	int		activity;

	clients = NULL;
	while (1)
	{
		FD_ZERO(&readfds);
		FD_SET(sockfd, &readfs);
		max_sd = sockfd;
		// add client to FD_SET
		activity = select(max_sd + 1, &readfs, NULL, NULL, NULL);
		if (activity < 0)
			return (error("select: "));
		read_stuff(&clients, &readfs, &activity);
		write_stuff(&clients, &writefs, &activity);
	}
}

int main(int argc, char **argv)
{
	struct sockaddr_in cliaddr;
	socklen_t clilen;
	int sockfd;
	int newfd;

	if ((sockfd = init_server()) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	printf("listening on port %d\n", PORT);
	server_loop(sockfd);
	return (EXIT_SUCCESS);
	while (1)
	{
		newfd = accept(sockfd, (struct sockaddr *)&cliaddr, &clilen);
		if (newfd > 0)
			do_stuff(newfd);
	}
	return (EXIT_SUCCESS);
}
