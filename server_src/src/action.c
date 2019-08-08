/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmorer <gmorer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/08 13:38:13 by gmorer            #+#    #+#             */
/*   Updated: 2019/08/08 15:46:30 by gmorer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"
#include "error_code.h"

int		action_nick(t_client **clients, t_client *client, t_response *resp)
{
	char	*new_nick;
	char	old_nick[NICK_LEN];
	int		new_nick_len;
	size_t	command_size;
	size_t	response_len;

	memcpy(old_nick, client->nick, sizeof(old_nick));
	command_size = sizeof("/nick");
	new_nick = next_word(resp->buffer + command_size,
			resp->message_length - command_size, &new_nick_len);
	if (!new_nick_len)
		return (send_to_client(client, resp, ER_NO_NAM, sizeof(ER_NO_NAM)));
	if (new_nick_len > NICK_LEN)
	{
		return (send_to_client(client, resp, ER_LG_NAM,
					sizeof(ER_LG_NAM)));
	}
	bzero(client->nick, sizeof(client->nick));
	memcpy(client->nick, new_nick, new_nick_len);
	bzero(resp->buffer, sizeof(resp->buffer));
	resp->message_length = snprintf(resp->buffer, sizeof(resp->buffer),
			"* %s as change is nickname to %s.\n", old_nick, client->nick);
	send_to_channel(clients, client->channel, resp);
	return (1);
}

int		action_join(t_client **clients, t_client *client, t_response *resp)
{
	char	*new_channel;
	int		new_channel_len;
	size_t	command_size;
	size_t	response_len;

	command_size = sizeof("/join");
	new_channel = next_word(resp->buffer + command_size,
			resp->message_length - command_size, &new_channel_len);
	if (!new_channel_len)
		return (0);
	if (new_channel_len > CHANNEL_NAME_LEN)
		return (0);
	bzero(client->channel, sizeof(client->channel));
	memcpy(client->channel, new_channel, new_channel_len);
	bzero(resp->buffer, sizeof(resp->buffer));
	resp->message_length = snprintf(resp->buffer, sizeof(resp->buffer),
			"* %s has joined the channel.\n", client->nick);
	send_to_channel(clients, client->channel, resp);
	return (1);
}

int		who_next(t_client **clis, t_client *cli, t_response *resp, char *chan)
{
	size_t		off;
	t_client	*tmp;

	bzero(resp->buffer, sizeof(resp->buffer));
	off = snprintf(resp->buffer, BUFFER_LEN, "* people in %s : ", chan);
	off -= 1;
	tmp = *clis;
	while (off < BUFFER_LEN && tmp)
	{
		if (!strncmp(chan, tmp->channel, strlen(chan)))
		{
			if (off + 1 + strlen(tmp->nick) > BUFFER_LEN)
				break ;
			resp->buffer[off + 1] = ' ';
			memcpy(resp->buffer + off + 2, tmp->nick, strlen(tmp->nick));
			off += 1 + strlen(tmp->nick);
		}
		tmp = tmp->next;
	}
	if (off + 1 < BUFFER_LEN)
		resp->buffer[off++ + 1] = '\n';
	resp->message_length = off + 1;
	resp->activity = 0;
	add_to_queue(cli, resp);
	return (1);
}

int		action_who(t_client **clients, t_client *cli, t_response *resp)
{
	char		*channel;
	size_t		command_size;
	int			channel_len;
	char		channel_tmp[CHANNEL_NAME_LEN + 1];

	bzero(channel_tmp, sizeof(channel_tmp));
	command_size = sizeof("/who");
	channel = next_word(resp->buffer + command_size,
			resp->message_length - command_size, &channel_len);
	if (!channel_len)
	{
		if (!cli->channel[0])
			return (send_to_client(cli, resp, ER_NO_NAM, sizeof(ER_NO_NAM)));
		memcpy(channel_tmp, cli->channel, strlen(cli->channel));
		channel_len = strlen(channel_tmp);
	}
	else
	{
		if (channel_len > CHANNEL_NAME_LEN)
			return (send_to_client(cli, resp, ER_LG_NAM, sizeof(ER_LG_NAM)));
		memcpy(channel_tmp, channel, channel_len);
	}
	who_next(clients, cli, resp, channel_tmp);
	return (1);
}

int		action_msg(t_client **clients, t_client *cli, t_response *resp)
{
	char		*dest;
	int			dest_len;
	t_client	*tmp;
	char		buffer[BUFFER_LEN];

	dest = next_word(resp->buffer + sizeof("/msg"),
			resp->message_length - sizeof("/msg"), &dest_len);
	if ((tmp = *clients) && !dest_len)
		return (send_to_client(cli, resp, ER_NO_NAM, sizeof(ER_NO_NAM)));
	if ((resp->activity = 0) || dest_len > NICK_LEN)
		return (send_to_client(cli, resp, ER_LG_NAM, sizeof(ER_LG_NAM)));
	while (tmp && (strncmp(dest, tmp->nick, dest_len) || tmp == cli))
		tmp = tmp->next;
	bzero(tmp ? buffer : resp->buffer, BUFFER_LEN);
	if (tmp)
		memcpy(buffer, dest + dest_len + 1, resp->message_length -
				((dest - resp->buffer) + dest_len + 1));
	if (tmp)
		resp->message_length = snprintf(resp->buffer, sizeof(resp->buffer),
				"* From %s: %s", cli->nick, buffer);
	else
		strcpy(resp->buffer, ERR_INV_NICK);
	if (!tmp)
		resp->message_length = sizeof(ERR_INV_NICK);
	return (add_to_queue(tmp ? tmp : cli, resp));
}
