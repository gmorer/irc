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
    int     new_nick_len;
    size_t  command_size;

    (void*)clients;
    command_size = sizeof("/nick");
    new_nick = next_word(response->buffer + command_size,
        response->message_length - command_size, &new_nick_len);
    if (!new_nick_len)
        return (0); // error no name supplied
    if (new_nick_len > NICK_LEN)
        return (0); // error name too loong
    bzero(client->nick, sizeof(client->nick));
    memcpy(client->nick, new_nick, new_nick_len);
    return (1);
}

int action_join(t_client **clients, t_client *client, t_response *response)
{
    (void*)clients;
    (void*)client;
    (void*)response;
    printf("join trigered\n");
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
    strcpy(message->buffer, "Invalid command.");
    add_to_queue(client, message);
    return (0);
}