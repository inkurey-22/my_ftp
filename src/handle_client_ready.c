/*
** EPITECH PROJECT, 2026
** my_ftp
** File description:
** handle_client_ready
*/

#include "handle_client_ready.h"
#include "ftp.h"
#include "ftp_server_poll_internals.h"
#include <unistd.h>

void handle_client_ready(client_context_t *ctx)
{
    client_state_t *c = &ctx->client_states[ctx->idx];
    ssize_t n = read(c->fd, c->buffer, sizeof(c->buffer) - 1);
    int cmd_result;

    if (n <= 0) {
        return;
    }
    c->buffer[n] = '\0';
    cmd_result = handle_command(ctx->server, c);
    if (cmd_result == 0) {
        my_send(c->fd, "502 Command not implemented.\r\n", 31, 0);
    } else if (cmd_result == 2) {
        close_and_reset_client(c, ctx->pfds, ctx->idx);
    }
}
