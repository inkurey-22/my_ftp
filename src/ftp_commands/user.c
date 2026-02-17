/*
** EPITECH PROJECT, 2026
** my_ftp
** File description:
** user
*/

#include "commands.h"
#include "ftp.h"

#include <string.h>
#include <unistd.h>

static int parse_and_set_username(struct client_state_t *cstate, char *buffer)
{
    char *username = buffer + 5;
    size_t len;

    while (*username == ' ')
        username++;
    if (username == NULL || *username == '\0') {
        my_send(cstate->fd, "530 Invalid username.\r\n", 24, 0);
        cstate->logged_in = 0;
        cstate->username[0] = '\0';
        return 0;
    }
    len = strnlen(username, sizeof(cstate->username) - 1);
    while (len > 0
        && (username[len - 1] == ' ' || username[len - 1] == '\r'
            || username[len - 1] == '\n'))
        --len;
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
    my_send(cstate->fd, "331 Please specify the password.\r\n", 34, 0);
}
