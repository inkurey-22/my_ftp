/*
** EPITECH PROJECT, 2026
** my_ftp
** File description:
** ftp_server_poll.c
*/

#include "client_state.h"
#include "commands.h"
#include "ftp.h"
#include "server_context.h"
#include <netinet/in.h>
#include <poll.h>

#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

static void close_client(int fd)
{
    if (fd >= 0)
        close(fd);
}

static void handle_new_connection(server_context_t *ctx)
{
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int client_fd =
        accept(ctx->listen_fd, (struct sockaddr *) &client_addr, &client_len);

    if (client_fd < 0)
        return;
    for (int i = 1; i < ctx->nfds; ++i) {
        if (ctx->client_states[i].fd < 0) {
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
    close_client(client_fd);
}

static void close_and_reset_client(client_state_t *c, struct pollfd *pfds,
    int idx)
{
    close_client(c->fd);
    c->fd = -1;
    pfds[idx].fd = -1;
    pfds[idx].events = 0;
}

static int is_quit_command(const char *buffer)
{
    return strncmp(buffer, "QUIT", 4) == 0;
}

static int handle_command(struct ftp_server_s *server, client_state_t *c)
{
    size_t cmd_len = 0;

    for (int i = 0; i < FTP_COMMANDS_COUNT; ++i) {
        cmd_len = strlen(FTP_COMMANDS[i].name);
        if (strncmp(c->buffer, FTP_COMMANDS[i].name, cmd_len) == 0) {
            FTP_COMMANDS[i].func(server, c->fd, c->buffer,
                (int *) &c->logged_in);
            return is_quit_command(c->buffer) ? 2 : 1;
        }
    }
    return 0;
}

static void handle_client_ready(client_context_t *ctx)
{
    client_state_t *c = &ctx->client_states[ctx->idx];
    ssize_t n = read(c->fd, c->buffer, sizeof(c->buffer) - 1);
    int cmd_result;

    if (n <= 0) {
        close_and_reset_client(c, ctx->pfds, ctx->idx);
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

static void handle_poll_events(server_context_t *ctx)
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

static void poll_loop(server_context_t *ctx, struct pollfd *pfds, int nfds)
{
    int ready;

    while (1) {
        ready = poll(pfds, nfds, -1);
        if (ready < 0)
            continue;
        handle_poll_events(ctx);
    }
}

int run_server_poll_loop(struct ftp_server_s *server)
{
    struct pollfd pfds[MAX_CLIENTS + 1] = {0};
    client_state_t client_states[MAX_CLIENTS + 1] = {0};
    int nfds = MAX_CLIENTS + 1;
    server_context_t ctx = {0};

    pfds[0].fd = server->fd;
    pfds[0].events = POLLIN;
    client_states[0].fd = server->fd;
    ctx.server = server;
    ctx.listen_fd = server->fd;
    ctx.pfds = pfds;
    ctx.client_states = client_states;
    ctx.nfds = nfds;
    poll_loop(&ctx, pfds, nfds);
    return 0;
}
