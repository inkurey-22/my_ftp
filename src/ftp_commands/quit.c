/*
** EPITECH PROJECT, 2026
** my_ftp
** File description:
** quit
*/

#include "commands.h"
#include "ftp.h"

#include <unistd.h>

void ftp_cmd_quit([[maybe_unused]] struct ftp_server_s *server,
    struct client_state_t *cstate, [[maybe_unused]] char *buffer)
{
    if (cstate)
        my_send(cstate->fd, "221 Service closing control connection.\r\n", 62,
            0);
}
