/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connect.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmorer <gmorer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/08 12:28:31 by gmorer            #+#    #+#             */
/*   Updated: 2019/08/10 11:27:41 by gmorer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"

void	init_hints(struct addrinfo *hints)
{
	ft_bzero(hints, sizeof(*hints));
	hints->ai_family = AF_UNSPEC;
	hints->ai_socktype = SOCK_STREAM;
	hints->ai_protocol = 0;
	hints->ai_flags = 0;
}

int		looop(struct addrinfo *result, int *fd)
{
	struct addrinfo *tmp;

	tmp = result;
	while (tmp)
	{
		*fd = socket(tmp->ai_family, tmp->ai_socktype, tmp->ai_protocol);
		if (*fd == -1)
		{
			tmp = tmp->ai_next;
			continue ;
		}
		if (connect(*fd, tmp->ai_addr, tmp->ai_addrlen) != -1)
			break ;
		close(*fd);
		*fd = 0;
		tmp = tmp->ai_next;
	}
	return (tmp ? 1 : 0);
}

int		connect_to_server(int *fd, char *hostname, char *portstr)
{
	int				ret;
	struct addrinfo	*result;
	struct addrinfo hints;

	init_hints(&hints);
	result = NULL;
	if (*fd)
		close(*fd);
	*fd = 0;
	ret = getaddrinfo(hostname, portstr, &hints, &result);
	if (ret != 0 || !result)
	{
		write(2, INVALID_ADDR, sizeof(INVALID_ADDR));
		return (0);
	}
	ret = looop(result, fd);
	freeaddrinfo(result);
	if (!ret)
	{
		write(2, INVALID_ADDR, sizeof(INVALID_ADDR));
		return (0);
	}
	return (1);
}
