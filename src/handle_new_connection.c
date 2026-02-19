/*
** EPITECH PROJECT, 2026
** my_ftp
** File description:
** handle_new_connection
*/

#include "handle_new_connection.h"
#include "ftp.h"
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

static void initialize_client_state(server_context_t *ctx, int i,
    int client_fd)
{
    const char *greeting = "220 Service ready for new user.\r\n";
    ctx->client_states[i].fd = client_fd;
    ctx->client_states[i].buf_used = 0;
    ctx->client_states[i].logged_in = false;
    ctx->client_states[i].to_close = false;
    ctx->client_states[i].data_fd = -1;
    ctx->pfds[i].fd = client_fd;
    ctx->pfds[i].events = POLLIN;
    my_send(client_fd, greeting, strlen(greeting), 0);
}

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
            initialize_client_state(ctx, i, client_fd);
            return;
        }
    }
    close(client_fd);
}
