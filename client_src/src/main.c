#include "client.h"

int main(int argc, char **argv)
{
	char buffer[BUFFER_SIZE];
	char incomming[BUFFER_SIZE];
	int fd;
	WINDOW *input;
	WINDOW *screen;
	int index;
	int ret;

	index = 0;
	fd = 0;
	// initscr();
	// display(&screen, &input);
	if (argc > 2)
	{
		if (!connect_to_server(&fd, argv[1], argv[2]))
		{
			printf("ok\n");
		}
		else
		{
			printf("ko\n");
		}
		
			// mvwprintw(screen, index + 1, 2, "Cannot connect to %s:%s", argv[1], argv[2]);
		// else
			// mvwprintw(screen, index + 1, 2, "Successfully connected to %s:%s", argv[1], argv[2]);
		index += 1;
	}
	else
	{
		printf("no args\n");
		// mvwprintw(screen, index + 1, 2, "Not connected");
		index += 1;
	}
	// wrefresh(screen);
	// testing the ncurses lib
	while(1) ;
	// ret = wgetline(input, buffer, sizeof(buffer) - 1);
	// while (true)
	// {
	// 	if (fd)
	// 	{
	// 		bzero(incomming, sizeof(incomming));
	// 		if (recv(fd, incomming, sizeof(incomming), MSG_DONTWAIT))
	// 		{
	// 			mvwprintw(screen, index + 1, 2, "%s", incomming);
	// 			wrefresh(screen);
	// 		}
	// 	}
	// 	bzero(buffer, sizeof(buffer));
	// 	wclear(input);
	// 	box(input, 0, 0);
	// 	wrefresh(input);
	// 	ret = wgetline(input, buffer, sizeof(buffer) - 1);
	// 	// mvwprintw(screen, index + 1, 2, "%s", buffer);
	// 	// wrefresh(screen);
	// 	if (fd)
	// 		write(fd, buffer, ret);
	// 	index += 1;
	// }
	// endwin();
	return (0);
}