#include "client.h"

int wgetline(WINDOW *input, char *buffer, size_t buffer_len)
{
	size_t index;
	char c;

	index = 0;
	c = 0;
	while (index < buffer_len && (c = mvwgetch(input, 1, index + 2)) != '\n')
	{
		if (c == 127)
		{
			if (!index)
				continue;
			mvwprintw(input, 1, index + 1, "   ");
			index -= 1;
			continue;
		}
		buffer[index] = c;
		index += 1;
	}
	return index;
}