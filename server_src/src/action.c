#include "server.h"
#include "error_code.h"

char *next_word(char *str, int len, int *word_len)
{
    int word_start;

    word_start = 0;
    *word_len = 0;
    while (word_start < len && str[word_start] == ' ' && str[word_start] == '\t' &&
        str[word_start] && str[word_start] == '\n')
            word_start += 1;
    while (word_start + *word_len < len && str[word_start + *word_len] != ' ' &&
        str[word_start + *word_len] != '\t' && str[word_start + *word_len] && str[word_start + *word_len] != '\n')
        *word_len += 1;
    return (str + word_start);
}

int action_nick(t_client **clients, t_client *client, t_response *response)
{
    char    *new_nick;
	char	old_nick[NICK_LEN];
    int     new_nick_len;
    size_t  command_size;
	size_t	response_len;

	memcpy(old_nick, client->nick, sizeof(old_nick));
    command_size = sizeof("/nick");
    new_nick = next_word(response->buffer + command_size,
        response->message_length - command_size, &new_nick_len);
    if (!new_nick_len)
		return (send_to_client(client, response, ERR_NO_NAME, sizeof(ERR_NO_NAME)));
    if (new_nick_len > NICK_LEN)
		return (send_to_client(client, response, ERR_LONG_NAME, sizeof(ERR_LONG_NAME)));
    bzero(client->nick, sizeof(client->nick));
    memcpy(client->nick, new_nick, new_nick_len);
	bzero(response->buffer, sizeof(response->buffer));
	response->message_length = snprintf(response->buffer, sizeof(response->buffer),
		"* %s as change is nickname to %s.\n", old_nick, client->nick);
	send_to_channel(clients, client->channel, response);
    return (1);
}

int action_join(t_client **clients, t_client *client, t_response *response)
{
    char    *new_channel;
    int     new_channel_len;
    size_t  command_size;
	size_t	response_len;

    command_size = sizeof("/join");
    new_channel = next_word(response->buffer + command_size,
        response->message_length - command_size, &new_channel_len);
    if (!new_channel_len)
        return (0); // error no name supplied
    if (new_channel_len > CHANNEL_NAME_LEN)
        return (0); // error name too loong
    bzero(client->channel, sizeof(client->channel));
    memcpy(client->channel, new_channel, new_channel_len);
	bzero(response->buffer, sizeof(response->buffer));
	response->message_length = snprintf(response->buffer, sizeof(response->buffer),
		"* %s has joined the channel.\n", client->nick);
	send_to_channel(clients, client->channel, response);
    return (1);
}

int action_leave(t_client **clients, t_client *client, t_response *response)
{
	bzero(client->channel, sizeof(client->channel));
	return (send_to_client(client, response, HELLO_MESSAGE, sizeof(HELLO_MESSAGE)));
}

int action_list(t_client **clients, t_client *client, t_response *response)
{
	t_client	*tmp;
	char		buffer[CHANNEL_NAME_LEN + 2];
	int			offset;
	int			tmp_off;

	offset = 0;
	bzero(response->buffer, sizeof(response->buffer));
	tmp = *clients;
	while (tmp)
	{
		if (tmp->channel[0])
		{
			tmp_off = snprintf(buffer, sizeof(buffer), " %s ", tmp->channel);
			if (tmp_off + offset < sizeof(response->buffer) && !strnstr(response->buffer, buffer, tmp_off))
			{
				strcpy(response->buffer + offset, buffer); // index -1 if strlen = 0
			offset += tmp_off - 1;
			}
		}
		tmp = tmp->next;
	}
	if (!offset)
	{
		strcpy(response->buffer, ERR_NO_CHANNEL);
		offset = sizeof(ERR_NO_CHANNEL);
	}
	else if (offset + 1 < sizeof(response->buffer))
	{
		response->buffer[offset] = '\n';
		offset += 1;
	}
	response->message_length = offset;
	response->activity = 0;
	add_to_queue(client, response);
	return (0);
}

int action_help(t_client **clients, t_client *client, t_response *response)
{
	return (send_to_client(client, response, HELP_MESSAGE, sizeof(HELP_MESSAGE)));
}

int action_who(t_client **clients, t_client *client, t_response *response)
{
	char		*channel;
	size_t		command_size;
	int			channel_len;
	size_t		offset;
	t_client	*tmp;
	char		channel_tmp[CHANNEL_NAME_LEN + 1];

	bzero(channel_tmp, sizeof(channel_tmp));
	command_size = sizeof("/who");
    channel = next_word(response->buffer + command_size,
    	response->message_length - command_size, &channel_len);
    if (!channel_len)
	{
		if (!client->channel[0])
		{
			strcpy(response->buffer, ERR_NO_NAME);
			response->message_length = sizeof(ERR_NO_NAME);
			response->activity = 0;
			add_to_queue(client, response);
			return (1);
		}
		memcpy(channel_tmp, client->channel, strlen(client->channel));
		channel_len = strlen(channel_tmp);
	}
	else
	{
		if (channel_len > CHANNEL_NAME_LEN)
			return (send_to_client(client, response, ERR_LONG_NAME, sizeof(ERR_LONG_NAME)));
		memcpy(channel_tmp, channel, channel_len);
	}
	bzero(response->buffer, sizeof(response->buffer));
	offset = snprintf(response->buffer, sizeof(response->buffer), "* people in %s : ", channel_tmp);
	offset -= 1;
	tmp = *clients;
	while (offset < BUFFER_LEN && tmp)
	{
		if (!strncmp(channel_tmp, tmp->channel, channel_len))
		{
			if (offset + 1 + strlen(tmp->nick) > BUFFER_LEN)
				break;
			response->buffer[offset + 1] = ' ';
			memcpy(response->buffer + offset + 2, tmp->nick, strlen(tmp->nick));
			offset += 1 + strlen(tmp->nick);
		}
		tmp = tmp->next;
	}
	if (offset < BUFFER_LEN)
	{
		response->buffer[offset + 1] = '\n';
		offset += 1;
	}
	response->message_length = offset + 1;
	response->activity = 0;
	add_to_queue(client, response);
    return (1);
}

int action_msg(t_client **clients, t_client *client, t_response *response)
{
	char    *dest;
    int     dest_len;
    size_t  command_size;
	t_client *tmp;
	char tmp_buffer[BUFFER_LEN];

    command_size = sizeof("/msg");
    dest = next_word(response->buffer + command_size,
        response->message_length - command_size, &dest_len);
    if (!dest_len)
		return (send_to_client(client, response, ERR_NO_NAME, sizeof(ERR_NO_NAME)));
    if (dest_len > NICK_LEN)
		return (send_to_client(client, response, ERR_LONG_NAME, sizeof(ERR_LONG_NAME)));
	tmp = *clients;
	while (tmp && (strncmp(dest, tmp->nick, dest_len) || tmp == client))
		tmp = tmp->next;
	response->activity = 0;
	if (tmp)
	{
		bzero(tmp_buffer, sizeof(tmp_buffer));
		memcpy(tmp_buffer, dest + dest_len + 1, response->message_length - ((dest - response->buffer) + dest_len + 1));
		response->message_length = snprintf(response->buffer, sizeof(response->buffer),
			"* From %s: %s", client->nick, tmp_buffer);
	}
	else
	{
		bzero(response->buffer, sizeof(response->buffer));
		strcpy(response->buffer, ERR_INV_NICK);
		response->message_length = sizeof(ERR_INV_NICK);
		tmp = client;
	}
	add_to_queue(tmp, response);
    return (1);
}


int action(t_client **clients, t_client *client, t_response *message)
{
    t_action *actions = ACTION_ARRAY;
    int index;
    int len;

    len = sizeof(ACTION_ARRAY) / sizeof(ACTION_ARRAY[0]);
    index = 0;
    while (index < len)
    {
        if (!strncmp(actions[index].action_name, message->buffer + 1,
            strlen(actions[index].action_name)))
            return actions[index].action(clients, client, message);
        index += 1;
    }
	return (send_to_client(client, message, ERR_INV_COMMAND, sizeof(ERR_INV_COMMAND)));
}