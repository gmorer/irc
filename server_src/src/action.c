#include "server.h"

int action_nick(t_client **clients, t_client *client, t_response *response)
{
    (void*)clients;
    (void*)client;
    (void*)response;
    printf("nick trigered\n");
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
        if (strncmp(actions[index].action_name, message->buffer + 1,
            strlen(actions[index].action_name)))
            return actions[index].action(clients, client, message);
        len += 1;
    }
    bzero(message->buffer, sizeof(message->buffer));
    strcpy(message->buffer, "Invalid command.");
    add_to_queue(client, message);
    return (0);
}