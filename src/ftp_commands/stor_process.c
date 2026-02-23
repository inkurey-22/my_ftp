/*
** EPITECH PROJECT, 2026
** my_ftp
** File description:
** stor_process.c - process management for STOR
*/

#include "stor_helpers.h"
#include <stdlib.h>
#include <unistd.h>

void stor_child(stor_transfer_ctx_t *ctx, client_state_t *cstate)
{
    handle_file_receive(ctx->file_fd, ctx->data_conn_fd, cstate);
    exit(0);
}

void stor_parent(stor_transfer_ctx_t *ctx)
{
    close(ctx->file_fd);
    close(ctx->data_conn_fd);
}

void stor_fork_and_transfer(stor_transfer_ctx_t *ctx, client_state_t *cstate)
{
    pid_t pid = fork();

    if (pid < 0) {
        close(ctx->file_fd);
        close(ctx->data_conn_fd);
        return;
    }
    if (pid == 0) {
        stor_child(ctx, cstate);
    }
    stor_parent(ctx);
}
