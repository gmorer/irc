#include "server.h"

int error(char *message)
{
	perror(message);
	return (EXIT_FAILURE);
}

int init_server(int isipv6)
{
	struct sockaddr_in	sockaddrv4;
	struct sockaddr_in6 sockaddrv6;
	int					sockfd;

	sockfd = socket(isipv6 ? AF_INET6 : AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
		return (error("Opening socket: "));
	if (isipv6)
	{
		bzero(&sockaddrv6, sizeof(struct sockaddr_in6));
		sockaddrv6.sin6_family = AF_INET6;
		sockaddrv6.sin6_addr = in6addr_any;
		sockaddrv6.sin6_port = htons(PORT);
		if (bind(sockfd, (struct sockaddr *)&sockaddrv6, sizeof(sockaddrv6)) == -1)
			return (error("Binding socket: "));
	}
	else
	{
		bzero(&sockaddrv4, sizeof(struct sockaddr_in));
		sockaddrv4.sin_family = AF_INET;
		sockaddrv4.sin_addr.s_addr = htonl(INADDR_ANY);
		sockaddrv4.sin_port = htons(PORT);
		if (bind(sockfd, (struct sockaddr *)&sockaddrv4, sizeof(sockaddrv4)) == -1)
			return (error("Binding socket: "));
	}

	if (listen(sockfd, BACKLOG) == -1)
		return (error("Listening: "));
	return (sockfd);
}

int main(int argc, char **argv)
{
	int			sockfd;
	t_client	*clients;
	int			isipv6;

	clients = NULL;
	isipv6 = 0;
	argc -= 1;
	while (argc)
	{
		if (!strcmp(argv[argc], "-ipv6"))
			isipv6 = 1;
		argc -= 1;
	}
	if ((sockfd = init_server(isipv6)) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	printf("listening on port %d\n", PORT);
	server_loop(sockfd, &clients);
	return (EXIT_SUCCESS);
}
