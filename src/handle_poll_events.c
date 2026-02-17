/*
** EPITECH PROJECT, 2026
** my_ftp
** File description:
** handle_poll_events
*/

#include "handle_client_ready.h"
#include "handle_new_connection.h"
#include "poll_loop.h"
#include <poll.h>

void handle_poll_events(server_context_t *ctx)
{
    client_context_t cctx = {0};

    if (ctx->pfds[0].revents & POLLIN)
        handle_new_connection(ctx);
    for (int i = 1; i < ctx->nfds; ++i) {
        if (ctx->pfds[i].fd >= 0 && (ctx->pfds[i].revents & POLLIN)) {
            cctx.server = ctx->server;
            cctx.idx = i;
            cctx.pfds = ctx->pfds;
            cctx.client_states = ctx->client_states;
            handle_client_ready(&cctx);
        }
    }
    for (int i = 0; i < ctx->nfds; ++i)
        ctx->pfds[i].revents = 0;
}
