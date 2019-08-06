#include "client.h"

int connect_to_server(int *fd, char *hostname, char *portstr)
{
	int				ret;
	struct addrinfo	*result;
	struct addrinfo *tmp;

	result = NULL;
	if (*fd)
		close(*fd);
	ret = getaddrinfo(hostname, portstr, NULL, &result);
	if (ret || !result)
	{
		write(2, INVALID_ADDR, sizeof(INVALID_ADDR));
		return (0);
	}
	tmp = result;
	while (tmp)
	{
		*fd = socket(tmp->ai_family, tmp->ai_socktype, tmp->ai_protocol);
		if (*fd == -1)
			continue ;
		if (connect(*fd, tmp->ai_addr, tmp->ai_addrlen) != -1)
			break ;
		close(*fd);
		tmp = tmp->ai_next;
	}
	freeaddrinfo(result);
	if (!tmp) {
		write(2, INVALID_ADDR, sizeof(INVALID_ADDR));
		return (0);
    }
	return (1);
}