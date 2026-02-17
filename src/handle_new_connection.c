/*
** EPITECH PROJECT, 2026
** my_ftp
** File description:
** handle_new_connection
*/

#include "handle_new_connection.h"
#include "ftp.h"
#include <netinet/in.h>
#include <unistd.h>

void handle_new_connection(server_context_t *ctx)
{
    struct sockaddr_in client_addr = {0};
    socklen_t client_len = sizeof(client_addr);
    int client_fd =
        accept(ctx->listen_fd, (struct sockaddr *) &client_addr, &client_len);

    if (client_fd < 0)
        return;
    for (int i = 1; i < ctx->nfds; ++i) {
        if (ctx->client_states[i].fd == -1) {
            ctx->client_states[i].fd = client_fd;
            ctx->client_states[i].buf_used = 0;
            ctx->client_states[i].logged_in = false;
            ctx->client_states[i].to_close = false;
            ctx->pfds[i].fd = client_fd;
            ctx->pfds[i].events = POLLIN;
            my_send(client_fd, "220 You lost the game :3.\r\n", 27, 0);
            return;
        }
    }
    close(client_fd);
}
