#include "client.h"

void window(int height, int width, int index, char **screen)
{
	int i;

	i = 0;
	clear();
	while (i < index && screen[i])
	{
		printf("%s", screen[i]);
		i += 1;
	}
	drawline(height - 1, width);
	gotoxy(0, height);
	fflush(stdout);
}

void add_to_screen(char **screen, char *str, int *index, size_t screen_size)
{
	printf("buffer address:  %p\n", str);
	screen[*index] = strdup(str);
	*index += 1;
}

int loop(int *fd, char **screen, int height, int width)
{
	int		index;
	char	buffer[BUFFER_SIZE];
	fd_set	read_fd;
	int		ret;

	index = 0;
	while (1)
	{
		window(height, width, index, screen);
		bzero(buffer, sizeof(buffer));
		FD_ZERO(&read_fd);
		FD_SET(0, &read_fd);
		if (*fd)
			FD_SET(*fd, &read_fd);
		select(*fd + 1, &read_fd, NULL, NULL, NULL);
		if (*fd && FD_ISSET(*fd, &read_fd))
		{
			read(*fd, buffer, sizeof(buffer));
			add_to_screen(screen, buffer, &index, height - 2);
		}
		else if (FD_ISSET(0, &read_fd))
		{
			ret = read(0, buffer, sizeof(buffer));
			if (*fd)
				write(*fd, buffer, ret);
		}
	}
}

int main(int argc, char **argv)
{
	
	char			**screen;
	struct winsize	term;
	int				fd;

	fd = 0;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &term);
	screen = malloc(sizeof(char*) * (term.ws_row - 2));
	if (!screen)
		return (1);
	bzero(screen, sizeof(char*) * (term.ws_row - 2));
	if (argc > 2)
	{
		if (!connect_to_server(&fd, argv[1], argv[2]))
			printf("Cannot connect to %s:%s", argv[1], argv[2]);
		else
			printf("Successfully connected to %s:%s", argv[1], argv[2]);
	}
	else
	{
		printf("Not connected\n");
	}
	loop(&fd, screen, term.ws_row, term.ws_col);
	return (0);
}