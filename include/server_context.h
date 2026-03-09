/*
** EPITECH PROJECT, 2026
** my_ftp
** File description:
** server_context_t and client_context_t struct definitions
*/

// clang-format off
#ifndef SERVER_CONTEXT_H
    #define SERVER_CONTEXT_H

    #include "client_state.h"
    #include "ftp.h"
    #include <poll.h>
// clang-format on

typedef struct {
    struct pollfd *pfds;
    client_state_t *client_states;
    struct ftp_server_s *server;
    int listen_fd;
    int nfds;
} server_context_t;

typedef struct {
    struct pollfd *pfds;
    client_state_t *client_states;
    struct ftp_server_s *server;
    int idx;
    int pad;
} client_context_t;

#endif /* SERVER_CONTEXT_H */
