#include "client.h"

void display(WINDOW **screen, WINDOW **input)
{
	int screen_x;
	int screen_y;

	cbreak();
	getmaxyx(stdscr, screen_y, screen_x);
	*input = newwin(3, screen_x, screen_y - 3, 0);
	*screen = newwin(screen_y - 3, screen_x, 0, 0);
	box(*input, 0, 0);
	box(*screen, 0, 0);
	mvwprintw(*screen, 0, screen_x / 2 - 2, "IRC");
	move(screen_y - 2, 2);
	refresh();
	wrefresh(*input);
	wrefresh(*screen);
}