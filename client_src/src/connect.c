#include "cllient.h"

int connect(int fd)
{
	if (fd)
		close(fd);
	return (fd);
}