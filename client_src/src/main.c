#include "client.h"

int main(int argc, char **argv)
{
	char buffer[BUFFER_SIZE];
	int fd;
	WINDOW *input;
	WINDOW *screen;
	int index;

	index = 0;
	fd = 0;
	initscr();
	if (argc > 2)
	{
		connect_to_server(&fd, argv[1], argv[2]);
		printf("mahchie: %s, port: %s\n", argv[1], argv[2]);
	}
	display(&screen, &input);
	// testing the ncurses lib
	while (true)
	{
		bzero(buffer, sizeof(buffer));
		wclear(input);
		box(input, 0, 0);
		wrefresh(input);
		wgetline(input, buffer, sizeof(buffer) - 1);
		mvwprintw(screen, index + 1, 2, "%s", buffer);
		wrefresh(screen);
		index += 1;
	}
	endwin();
	return (0);
}