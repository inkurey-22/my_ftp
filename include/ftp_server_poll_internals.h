/*
** EPITECH PROJECT, 2026
** my_ftp
** File description:
** ftp_server_poll_internals
*/

// clang-format off
#ifndef FTP_SERVER_POLL_INTERNALS_H
    #define FTP_SERVER_POLL_INTERNALS_H

    #include "client_state.h"
    #include "ftp.h"
    #include <poll.h>
// clang-format on

int handle_command(struct ftp_server_s *server, client_state_t *c);
void close_and_reset_client(client_state_t *c, struct pollfd *pfds, int idx);

#endif// FTP_SERVER_POLL_INTERNALS_H
