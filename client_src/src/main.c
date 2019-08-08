/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmorer <gmorer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/08 12:38:43 by gmorer            #+#    #+#             */
/*   Updated: 2019/08/08 12:44:01 by gmorer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"

void	window(t_norme *norme)
{
	int i;

	i = 0;
	clear();
	while (i < norme->mem_index && norme->screen[i])
	{
		put_str(norme->screen[i]);
		i += 1;
	}
	write(1, "\n", 1);
	drawline(norme->height - 1, norme->width);
	GOTOXY(0, norme->height);
	fflush(stdout);
}

void	add_to_screen(t_norme *norme, char *str)
{
	norme->screen[norme->mem_index] = strdup(str);
	norme->mem_index += 1;
}

int		get_input(t_norme *norme, char *buffer)
{
	int ret;

	ret = read(0, buffer, sizeof(buffer));
	if (!ret)
		return (0);
	if (!strncmp(buffer, "/disconnect", sizeof("/disconnect") - 1))
		ft_disconnect(norme);
	else if (!strncmp(buffer, "/connect", sizeof("/connect") - 1))
		ft_connect(norme, buffer);
	else if (!norme->fd && !strncmp(buffer, "/help", sizeof("/help") - 1))
		ft_help(norme);
	else if (norme->fd && buffer[1])
		write(norme->fd, buffer, ret);
	return (1);
}

int		loop(t_norme *norme)
{
	char	buffer[BUFFER_SIZE];
	fd_set	read_fd;
	int		ret;

	ret = 1;
	while (1)
	{
		if (ret)
			window(norme);
		bzero(buffer, sizeof(buffer));
		FD_ZERO(&read_fd);
		FD_SET(0, &read_fd);
		if (norme->fd)
			FD_SET(norme->fd, &read_fd);
		select(norme->fd + 1, &read_fd, NULL, NULL, NULL);
		if (norme->fd && FD_ISSET(norme->fd, &read_fd))
		{
			if (!(ret = read(norme->fd, buffer, sizeof(buffer))))
				ft_disconnect(norme);
			else
				add_to_screen(norme, buffer);
		}
		else if (FD_ISSET(0, &read_fd))
			ret = get_input(norme, buffer);
	}
}

int		main(int argc, char **argv)
{
	struct winsize	term;
	t_norme			norme;

	norme.fd = 0;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &term);
	norme.height = term.ws_row;
	norme.width = term.ws_col;
	norme.mem_index = 0;
	norme.screen = malloc(sizeof(char*) * (term.ws_row - 2));
	if (!norme.screen)
		return (1);
	bzero(norme.screen, sizeof(char*) * (term.ws_row - 2));
	if (argc > 2)
	{
		if (!connect_to_server(&(norme.fd), argv[1], argv[2]))
			add_to_screen(&norme, "! Not connected !\n");
		else
			add_to_screen(&norme, "\033[0;32mConnected !\n");
	}
	else
		add_to_screen(&norme, "! Not connected !\n");
	loop(&norme);
	return (0);
}
