/*
** EPITECH PROJECT, 2026
** my_ftp
** File description:
** syst
*/

#include "commands.h"
#include "ftp.h"
#include <string.h>

void ftp_cmd_syst([[maybe_unused]] struct ftp_server_s *server,
    struct client_state_t *cstate, [[maybe_unused]] char *buffer)
{
    my_send(cstate->fd, "215 UNIX system type.\r\n",
        strlen("215 UNIX system type.\r\n"), 0);
}
