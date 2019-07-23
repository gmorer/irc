#include "server.h"

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
        return (0); // error no name supplied
    if (new_nick_len > NICK_LEN)
        return (0); // error name too loong
    bzero(client->nick, sizeof(client->nick));
    memcpy(client->nick, new_nick, new_nick_len);
	bzero(response->buffer, sizeof(response->buffer));
	response->message_length = snprintf(response->buffer, sizeof(response->buffer),
		"* %s as change is nickname to %s.\n", old_nick, client->nick);
	response->activity = 0;
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
		"* %s as joined the channel.\n", client->nick);
	response->activity = 0;
	send_to_channel(clients, client->channel, response);
    return (1);
}
int action_leave(t_client **clients, t_client *client, t_response *response)
{
    (void*)clients;
    (void*)client;
    (void*)response;
    printf("leave trigered\n");
    return (1);
}
int action_who(t_client **clients, t_client *client, t_response *response)
{
    (void*)clients;
    (void*)client;
    (void*)response;
    printf("who trigered\n");
    return (1);
}
int action_msg(t_client **clients, t_client *client, t_response *response)
{
    (void*)clients;
    (void*)client;
    (void*)response;
    printf("msg trigered\n");
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
    bzero(message->buffer, sizeof(message->buffer));
    strcpy(message->buffer, "Invalid command.\n");
	message->message_length = sizeof("Invalid command.\n");
    add_to_queue(client, message);
    return (0);
}