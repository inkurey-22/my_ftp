/*
** EPITECH PROJECT, 2026
** my_ftp
** File description:
** ftp_server_poll
*/

#include "poll_loop.h"
#include "poll_setup.h"
#include "server_context.h"
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>

struct client_state_t *find_client_state(struct client_state_t *arr,
    int arrlen, int fd)
{
    int i;

    for (i = 0; i < arrlen; ++i) {
        if (arr[i].fd == fd)
            return &arr[i];
    }
    return nullptr;
}

static void free_clients(struct pollfd *pfds, client_state_t *client_states,
    int nfds)
{
    int i;

    for (i = 0; i < nfds; ++i) {
        pfds[i].fd = -1;
        client_states[i].fd = -1;
    }
}

static int alloc_poll_arrays(poll_alloc_t *alloc)
{
    int result = 0;

    *alloc->pfds = calloc(alloc->nfds, sizeof(struct pollfd));
    *alloc->client_states = calloc(alloc->nfds, sizeof(client_state_t));
    if (!*alloc->pfds || !*alloc->client_states) {
        free(*alloc->pfds);
        free(*alloc->client_states);
        result = -1;
    }
    return result;
}

static void init_poll_arrays(poll_init_t *init)
{
    free_clients(init->pfds, init->client_states, init->nfds);
    init->pfds[0].fd = init->server_fd;
    init->pfds[0].events = POLLIN;
    init->client_states[0].fd = init->server_fd;
}

static void fill_server_context(ctx_fill_t *fill)
{
    fill->ctx->server = fill->server;
    fill->ctx->listen_fd = fill->server->fd;
    fill->ctx->pfds = fill->pfds;
    fill->ctx->client_states = fill->client_states;
    fill->ctx->nfds = fill->nfds;
}

static int allocate_and_init_poll_arrays(poll_setup_t *setup, int nfds)
{
    poll_alloc_t alloc;
    poll_init_t init;

    alloc.nfds = nfds;
    alloc.pfds = setup->pfds_out;
    alloc.client_states = setup->client_states_out;
    if (alloc_poll_arrays(&alloc) != 0)
        return -1;
    init.pfds = *setup->pfds_out;
    init.client_states = *setup->client_states_out;
    init.nfds = nfds;
    init.server_fd = setup->server->fd;
    init_poll_arrays(&init);
    return 0;
}

static void fill_context_from_setup(poll_setup_t *setup, int nfds)
{
    ctx_fill_t fill;

    fill.ctx = setup->ctx_out;
    fill.server = setup->server;
    fill.pfds = *setup->pfds_out;
    fill.client_states = *setup->client_states_out;
    fill.nfds = nfds;
    fill_server_context(&fill);
}

static int setup_poll_resources(poll_setup_t *setup)
{
    int nfds = MAX_CLIENTS + 1;

    if (allocate_and_init_poll_arrays(setup, nfds) != 0)
        return -1;
    *setup->nfds_out = nfds;
    fill_context_from_setup(setup, nfds);
    return 0;
}

static void cleanup_poll_resources(struct pollfd *pfds,
    client_state_t *client_states)
{
    free(pfds);
    free(client_states);
}

int run_server_poll_loop(struct ftp_server_s *server)
{
    struct pollfd *pfds = NULL;
    client_state_t *client_states = NULL;
    server_context_t ctx = {0};
    int nfds = 0;
    poll_setup_t setup;

    setup.server = server;
    setup.pfds_out = &pfds;
    setup.client_states_out = &client_states;
    setup.ctx_out = &ctx;
    setup.nfds_out = &nfds;
    if (setup_poll_resources(&setup) != 0)
        return -1;
    poll_loop(&ctx, pfds, nfds);
    cleanup_poll_resources(pfds, client_states);
    return 0;
}
