/*
** EPITECH PROJECT, 2026
** my_ftp
** File description:
** user
*/

#include "commands.h"
#include "ftp.h"
#include "ftp_replies.h"

#include <string.h>
#include <unistd.h>

static void trim_username(char **username, size_t *len)
{
    while (**username == ' ')
        (*username)++;
    *len = strnlen(*username, 255);
    while (*len > 0
        && ((*username)[*len - 1] == ' ' || (*username)[*len - 1] == '\r'
            || (*username)[*len - 1] == '\n'))
        --(*len);
}

static int parse_and_set_username(struct client_state_t *cstate, char *buffer)
{
    char *username = buffer + 5;
    size_t len = 0;

    trim_username(&username, &len);
    if (username == NULL || *username == '\0' || len == 0) {
        my_send(cstate->fd, reply_530_invalid_username,
            strlen(reply_530_invalid_username), 0);
        cstate->logged_in = 0;
        cstate->username[0] = '\0';
        return 0;
    }
    if (len >= sizeof(cstate->username))
        len = sizeof(cstate->username) - 1;
    strncpy(cstate->username, username, len);
    cstate->username[len] = '\0';
    return 1;
}

void ftp_cmd_user([[maybe_unused]] struct ftp_server_s *server,
    struct client_state_t *cstate, char *buffer)
{
    if (cstate == NULL)
        return;

    if (!parse_and_set_username(cstate, buffer))
        return;

    cstate->logged_in = 0;
    my_send(cstate->fd, reply_331, strlen(reply_331), 0);
}
