#include "client.h"

int connect_to_server(int *fd, char *hostname, char *portstr)
{
	int				ret;
	struct addrinfo	*result;
	struct addrinfo *tmp;
	struct addrinfo hints;

	bzero(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = 0;
	hints.ai_flags = 0;
	result = NULL;
	if (*fd)
		close(*fd);
	*fd = 0;
	ret = getaddrinfo(hostname, portstr, &hints, &result);
	if (ret != 0 || !result)
	{
		printf("Invalid return of getaddrinfo\n");
		write(2, INVALID_ADDR, sizeof(INVALID_ADDR));
		return (0);
	}
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
		{
			printf("breaked\n");
			break ;
		}
		close(*fd);
		fd = 0;
		tmp = tmp->ai_next;
	}
	freeaddrinfo(result);
	if (!tmp) {
		write(2, INVALID_ADDR, sizeof(INVALID_ADDR));
		printf("return KO\n");
		return (0);
    }
	printf("return OK\n");
	return (1);
}