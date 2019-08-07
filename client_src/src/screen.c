#include "client.h"

void clear(void)
{
	write(1, CLEAR, sizeof(CLEAR) - 1);
}

void put_str(char *str)
{
	int		len;
	char	*res;

	len = strlen(str);
	res = 0;
	if (str[0] == '!')
		write(1, RED, sizeof(RED) - 1);
	else if (str[0] == '*')
		write(1, BLUE, sizeof(BLUE) - 1);
	else if ((res = strchr(str, ':')))
		write(1, BOLD, sizeof(BOLD) - 1);
	if (res)
	{
		write(1, str, res - str + 1);
		write(1, DFLT, sizeof(DFLT) - 1);
		str = res + 1;
		len = strlen(str);
	}
	write(1, str, len);
	write(1, DFLT, sizeof(DFLT) - 1);
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