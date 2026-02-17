/*
** EPITECH PROJECT, 2026
** my_ftp
** File description:
** ftp_server_poll
*/

#include "client_state.h"
#include "ftp.h"
#include "poll_loop.h"
#include "server_context.h"

#include <netinet/in.h>
#include <unistd.h>

struct client_state_t *find_client_state(struct client_state_t *arr,
    int arrlen, int fd)
{
    for (int i = 0; i < arrlen; ++i) {
        if (arr[i].fd == fd)
            return &arr[i];
    }
    return nullptr;
}

static void free_clients(struct pollfd *pfds, client_state_t *client_states)
{
    for (int i = 0; i < MAX_CLIENTS + 1; ++i) {
        pfds[i].fd = -1;
        client_states[i].fd = -1;
    }
}

int run_server_poll_loop(struct ftp_server_s *server)
{
    struct pollfd pfds[MAX_CLIENTS + 1] = {0};
    client_state_t client_states[MAX_CLIENTS + 1] = {0};
    int nfds = MAX_CLIENTS + 1;
    server_context_t ctx = {0};

    free_clients(pfds, client_states);
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
