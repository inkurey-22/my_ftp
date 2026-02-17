/*
** EPITECH PROJECT, 2026
** my_ftp
** File description:
** poll_loop
*/

#include "poll_loop.h"
#include <poll.h>

void poll_loop(server_context_t *ctx, struct pollfd *pfds, int nfds)
{
    int ready = 0;

    while (1) {
        ready = poll(pfds, nfds, 30);
        if (ready < 0)
            continue;
        handle_poll_events(ctx);
    }
}
