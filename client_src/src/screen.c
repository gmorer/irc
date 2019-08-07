#include "client.h"

void clear(void)
{
	write(1, CLEAR, sizeof(CLEAR) - 1);
}

void drawline(int y, int width)
{
	char buffer[] = "--------------------";
	int index;
	int buffer_len;

	index = 0;
	buffer_len = 20;
	gotoxy(0, y);
	fflush(stdout);
	while (index + buffer_len < width)
	{
		write(1, buffer, buffer_len);
		index += buffer_len;
	}
	if (width - index)
		write(1, buffer, width - index);
}

void clearline(int y, int width)
{
	char buffer[] = "                    ";
	int index;
	int buffer_len;

	index = 0;
	buffer_len = 20;
	gotoxy(0, y);
	fflush(stdout);
	while (index + buffer_len < width)
	{
		write(1, buffer, buffer_len);
		index += buffer_len;
	}
	if (width - index)
		write(1, buffer, width - index);
}