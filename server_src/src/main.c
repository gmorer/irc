/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmorer <gmorer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/08 15:52:46 by gmorer            #+#    #+#             */
/*   Updated: 2019/08/08 15:55:37 by gmorer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

int	error(char *message)
{
	perror(message);
	return (EXIT_FAILURE);
}

int	init_server(int isipv6)
{
	struct sockaddr_in	addrv4;
	struct sockaddr_in6 addrv6;
	int					sockfd;

	if ((sockfd = socket(isipv6 ? AF_INET6 : AF_INET, SOCK_STREAM, 0)) == -1)
		return (error("Opening socket: "));
	if (isipv6)
	{
		bzero(&addrv6, sizeof(struct sockaddr_in6));
		addrv6.sin6_family = AF_INET6;
		addrv6.sin6_addr = in6addr_any;
		addrv6.sin6_port = htons(PORT);
		if (bind(sockfd, (struct sockaddr *)&addrv6, sizeof(addrv6)) == -1)
			return (error("Binding socket: "));
	}
	else
	{
		bzero(&addrv4, sizeof(struct sockaddr_in));
		addrv4.sin_family = AF_INET;
		addrv4.sin_addr.s_addr = htonl(INADDR_ANY);
		addrv4.sin_port = htons(PORT);
		if (bind(sockfd, (struct sockaddr *)&addrv4, sizeof(addrv4)) == -1)
			return (error("Binding socket: "));
	}
	return (listen(sockfd, BACKLOG) == -1 ? error("Listening: ") : sockfd);
}

int	main(int argc, char **argv)
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
