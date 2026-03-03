/*
** EPITECH PROJECT, 2026
** my_ftp
** File description:
** help
*/

#include "client_state.h"
#include "ftp.h"
#include <string.h>

void ftp_cmd_help([[maybe_unused]] struct ftp_server_s *server,
    [[maybe_unused]] struct client_state_t *cstate,
    [[maybe_unused]] char *buffer)
{
    const char *help_msg =
        "214 The following commands are recognized:\r\n"
        " USER PASS QUIT PWD RETR STOR DELE PASV PORT LIST CWD CDUP NOOP\r\n"
        "214 Help message.\r\n";

    my_send(cstate->fd, help_msg, strlen(help_msg), 0);
}
