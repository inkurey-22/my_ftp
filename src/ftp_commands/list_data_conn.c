/*
** EPITECH PROJECT, 2026
** my_ftp
** File description:
** list_data_conn
*/

#include "ftp.h"
#include "ftp_replies.h"
#include "list_helpers.h"
#include <string.h>
#include <unistd.h>

int check_logged_in_and_data_conn(struct client_state_t *cstate)
{
    if (!cstate || !cstate->logged_in) {
        my_send(cstate ? cstate->fd : -1, REPLY_530, strlen(REPLY_530), 0);
        return -1;
    }
    if (check_data_connection(cstate) < 0) {
        my_send(cstate->fd, REPLY_425_USE_PASV, strlen(REPLY_425_USE_PASV), 0);
        return -1;
    }
    return 0;
}

int prepare_data_connection(struct client_state_t *cstate)
{
    int data_fd;

    my_send(cstate->fd, REPLY_150, strlen(REPLY_150), 0);
    data_fd = open_data_connection(cstate);
    if (data_fd < 0) {
        my_send(cstate->fd, REPLY_425_CANT_OPEN_DATA,
            strlen(REPLY_425_CANT_OPEN_DATA), 0);
        return -1;
    }
    return data_fd;
}

char *parse_list_arg(char *buffer)
{
    if (!buffer)
        return NULL;
    while (*buffer == ' ')
        buffer++;
    strtok(buffer, " \r\n");
    return strtok(NULL, " \r\n");
}

int check_data_connection(struct client_state_t *cstate)
{
    if (!cstate)
        return -1;
    if (cstate->active_mode && cstate->active_addr.sin_port != 0)
        return 0;
    if (cstate->data_fd > 0)
        return 0;
    return -1;
}

static int open_active_data_connection(struct client_state_t *cstate)
{
    int data_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (data_fd < 0)
        return -1;
    if (connect(data_fd, (struct sockaddr *) &cstate->active_addr,
            sizeof(cstate->active_addr))
        < 0) {
        close(data_fd);
        return -1;
    }
    return data_fd;
}

static int open_passive_data_connection(struct client_state_t *cstate)
{
    int accepted_fd = accept(cstate->data_fd, NULL, NULL);

    close(cstate->data_fd);
    cstate->data_fd = -1;
    if (accepted_fd < 0)
        return -1;
    return accepted_fd;
}

int open_data_connection(struct client_state_t *cstate)
{
    if (cstate->active_mode) {
        return open_active_data_connection(cstate);
    }
    if (cstate->data_fd > 0) {
        return open_passive_data_connection(cstate);
    }
    return -1;
}
