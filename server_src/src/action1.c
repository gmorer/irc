/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmorer <gmorer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/08 13:28:29 by gmorer            #+#    #+#             */
/*   Updated: 2019/08/10 11:10:12 by gmorer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"
#include "error_code.h"

char	*next_word(char *str, int len, int *word_len)
{
	int wd_start;

	wd_start = 0;
	*word_len = 0;
	while (wd_start < len && str[wd_start] == ' ' &&
			str[wd_start] == '\t' && str[wd_start] &&
			str[wd_start] == '\n')
		wd_start += 1;
	while (wd_start + *word_len < len && str[wd_start + *word_len] != ' ' &&
			str[wd_start + *word_len] != '\t' && str[wd_start + *word_len] &&
			str[wd_start + *word_len] != '\n')
		*word_len += 1;
	return (str + wd_start);
}

int		action_leave(t_client **clients, t_client *client, t_response *response)
{
	ft_bzero(client->channel, sizeof(client->channel));
	return (send_to_client(client, response, HELLO_MESSAGE,
				sizeof(HELLO_MESSAGE)));
}

int		action_list(t_client **clients, t_client *client, t_response *response)
{
	t_client	*tmp;
	char		bff[CHANNEL_NAME_LEN + 2];
	int			off;

	ft_bzero(response, sizeof(*response));
	tmp = *clients;
	while (tmp)
	{
		if (tmp->channel[0] && (off = snprintf(bff, sizeof(bff),
						" %s ", tmp->channel)))
			if (off + response->message_length < sizeof(response->buffer) &&
					!ft_strnstr(response->buffer, bff, off))
			{
				ft_strcpy(response->buffer + response->message_length, bff);
				response->message_length += off - 1;
			}
		tmp = tmp->next;
	}
	if (!response->message_length &&
			(response->message_length = sizeof(ERR_NO_CHANNEL)))
		ft_strcpy(response->buffer, ERR_NO_CHANNEL);
	else if (response->message_length + 1 < sizeof(response->buffer))
		response->buffer[response->message_length++] = '\n';
	add_to_queue(client, response);
	return (0);
}

int		action_help(t_client **clients, t_client *client, t_response *response)
{
	return (send_to_client(client, response, HELP_MESSAGE,
				sizeof(HELP_MESSAGE)));
}

int		action(t_client **clients, t_client *client, t_response *message)
{
	const t_action	*actions = ACTION_ARRAY;
	int				index;
	int				len;

	len = sizeof(ACTION_ARRAY) / sizeof(ACTION_ARRAY[0]);
	index = 0;
	while (index < len)
	{
		if (!ft_strncmp(actions[index].action_name, message->buffer + 1,
					ft_strlen(actions[index].action_name)))
			return (actions[index].action(clients, client, message));
		index += 1;
	}
	return (send_to_client(client, message, ERR_INV_COMMAND,
				sizeof(ERR_INV_COMMAND)));
}
