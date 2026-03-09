/*
** EPITECH PROJECT, 2026
** my_ftp
** File description:
** poll_setup
*/

// clang-format off
#ifndef POLL_SETUP_H
    #define POLL_SETUP_H

    #include "ftp.h"
    #include "server_context.h"
    #include <poll.h>
// clang-format on

// Structure for poll setup

typedef struct poll_setup_s {
    struct ftp_server_s *server;
    struct pollfd **pfds_out;
    client_state_t **client_states_out;
    server_context_t *ctx_out;
    int *nfds_out;
} poll_setup_t;

typedef struct {
    struct pollfd **pfds;
    client_state_t **client_states;
    int nfds;
    int pad;
} poll_alloc_t;

typedef struct {
    struct pollfd *pfds;
    client_state_t *client_states;
    int server_fd;
    int nfds;
} poll_init_t;

typedef struct {
    struct pollfd *pfds;
    client_state_t *client_states;
    server_context_t *ctx;
    struct ftp_server_s *server;
    int nfds;
    int pad;
} ctx_fill_t;

#endif// POLL_SETUP_H
