/*
** EPITECH PROJECT, 2026
** my_ftp
** File description:
** noop
*/

#include "client_state.h"
#include "ftp.h"
#include "ftp_replies.h"
#include <string.h>

void ftp_cmd_noop([[maybe_unused]] struct ftp_server_s *server,
    [[maybe_unused]] struct client_state_t *cstate,
    [[maybe_unused]] const char *args)
{
    my_send(cstate->fd, REPLY_200, strlen(REPLY_200), 0);
}
