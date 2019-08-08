/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmorer <gmorer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/08 12:26:10 by gmorer            #+#    #+#             */
/*   Updated: 2019/08/08 12:36:02 by gmorer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"

int		howmuchline(char *str)
{
	int index;
	int count;

	index = 0;
	count = 0;
	while (str[index])
	{
		if (str[index] == '\n')
			count += 1;
		index += 1;
	}
	return (count);
}

void	get_args(char *str, size_t len, char **args1, char **args2)
{
	int i;

	i = 0;
	while (str[i] && str[i] != ' ' && i < BUFFER_SIZE)
		i += 1;
	while (str[i] && str[i] == ' ' && i < BUFFER_SIZE)
		i += 1;
	if (str[i])
		*args1 = str + i;
	while (str[i] && str[i] != ' ' && i < BUFFER_SIZE)
		i += 1;
	if (i < BUFFER_SIZE && str[i])
		str[i] = 0;
	if (i < BUFFER_SIZE - 1)
		i += 1;
	while (str[i] && str[i] == ' ' && i < BUFFER_SIZE)
		i += 1;
	if (str[i])
		*args2 = str + i;
	while (str[i] && str[i] != '\n' && i < BUFFER_SIZE)
		i += 1;
	if (i < BUFFER_SIZE && str[i])
		str[i] = 0;
}

void	ft_disconnect(t_norme *norme)
{
	if (!norme->fd)
	{
		add_to_screen(norme, "!You arn't connected!\n");
		return ;
	}
	close(norme->fd);
	norme->fd = 0;
	add_to_screen(norme, "Successfully disconnected!\n");
}

void	ft_connect(t_norme *norme, char *str)
{
	char *hostname;
	char *portstr;

	hostname = NULL;
	portstr = NULL;
	get_args(str, BUFFER_SIZE, &hostname, &portstr);
	if (!portstr || !hostname)
		add_to_screen(norme,
				"!Invalid argument, usage:  ./connect hostname port\n");
	else if (connect_to_server(&(norme->fd), hostname, portstr))
		add_to_screen(norme, "Successfully connected!\n");
	else
		add_to_screen(norme, "!Error during the connection\n");
}

void	ft_help(t_norme *norme)
{
	add_to_screen(norme, HELP_STR);
}
