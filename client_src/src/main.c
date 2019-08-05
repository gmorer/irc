#include "client.h"

int main(int argc, char **argv)
{
	char	buffer[BUFFER_SIZE];
	int		ipv6;
	int		fd;

	fd = 0;
	ipv6 = 0;
	if (argc > 2)
	{
		printf("mahchie: %s, port: %s\n", argv[1], argv[2]);
	}
	(void)argc;
	(void)argv;
	bzero(buffer, sizeof(buffer));
	// write(1, "Hello world: ", 13);
	// printf("Hello world!: ");
	// fflush(stdin);
	// fgets(buffer, BUFFER_SIZE, stdin);
	// read(0, buffer, BUFFER_SIZE);
	// printf("input : %s\n", buffer);
	return (0);
}