/*
** EPITECH PROJECT, 2026
** my_ftp
** File description:
** poll_loop
*/

// clang-format off
#ifndef POLL_LOOP_H
    #define POLL_LOOP_H

    #include "server_context.h"
    #include <poll.h>
// clang-format on

void poll_loop(server_context_t *ctx, struct pollfd *pfds, int nfds);
void handle_poll_events(server_context_t *ctx);

#endif// POLL_LOOP_H
