/*
** EPITECH PROJECT, 2026
** my_ftp
** File description:
** retr
*/

#include "commands.h"
#include "ftp.h"
#include "ftp_replies.h"
#include "retr_helpers.h"
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

static void resolve_filepath(struct ftp_server_s *server,
    struct client_state_t *cstate, char *filepath, resolved_path_t *resolved)
{
    if (filepath[0] == '/') {
        snprintf(resolved->resolved, sizeof(resolved->resolved), "%s%s",
            server->home_path, filepath);
    } else {
        snprintf(resolved->resolved, sizeof(resolved->resolved), "%s/%s",
            cstate->real_cwd, filepath);
    }
}

static int retr_check_and_resolve(retr_check_args_t *args)
{
    if (check_data_connection(args->cstate) < 0)
        return -1;
    if (handle_filepath(args->cstate, args->buffer, args->filepath) < 0)
        return -1;
    if (!args->server || !args->cstate)
        return -1;
    resolve_filepath(args->server, args->cstate, *args->filepath,
        args->resolved);
    return 0;
}

static int retr_check_path_and_open(struct ftp_server_s *server,
    struct client_state_t *cstate, retr_transfer_ctx_t *ctx)
{
    if (strncmp(ctx->filepath, server->home_path, strlen(server->home_path))
        != 0) {
        my_send(cstate->fd, REPLY_550_FAILED_OPEN_FILE,
            strlen(REPLY_550_FAILED_OPEN_FILE), 0);
        return -1;
    }
    if (handle_open_file(cstate, ctx->filepath, &ctx->file_fd) < 0)
        return -1;
    return 0;
}

static int retr_prepare(struct ftp_server_s *server,
    struct client_state_t *cstate, char *buffer, retr_transfer_ctx_t *ctx)
{
    resolved_path_t resolved;
    char *filepath;
    retr_check_args_t check_args = {.server = server,
        .cstate = cstate,
        .buffer = buffer,
        .resolved = &resolved,
        .filepath = &filepath};

    if (retr_check_and_resolve(&check_args) < 0)
        return -1;
    ctx->filepath = resolved.resolved;
    if (retr_check_path_and_open(server, cstate, ctx) < 0)
        return -1;
    my_send(cstate->fd, REPLY_150, strlen(REPLY_150), 0);
    if (handle_accept_data_conn(cstate, ctx->file_fd, &ctx->data_conn_fd) < 0)
        return -1;
    return 0;
}

static void retr_child(retr_transfer_ctx_t *ctx, struct client_state_t *cstate)
{
    int transfer_result =
        send_file_over_data_conn(ctx->file_fd, ctx->data_conn_fd);

    shutdown(ctx->data_conn_fd, SHUT_WR);
    close(ctx->file_fd);
    close(ctx->data_conn_fd);
    handle_transfer_result(cstate, transfer_result);
    exit(0);
}

static void retr_fork_and_transfer(retr_transfer_ctx_t *ctx,
    struct client_state_t *cstate)
{
    pid_t pid = fork();

    if (pid < 0) {
        close(ctx->file_fd);
        close(ctx->data_conn_fd);
        my_send(cstate->fd, "550 Failed to fork for file transfer\r\n",
            strlen("550 Failed to fork for file transfer\r\n"), 0);
        return;
    }
    if (pid == 0) {
        retr_child(ctx, cstate);
    }
}

void ftp_cmd_retr(struct ftp_server_s *server, struct client_state_t *cstate,
    char *buffer)
{
    retr_transfer_ctx_t ctx = {0};

    if (retr_prepare(server, cstate, buffer, &ctx) < 0)
        return;
    retr_fork_and_transfer(&ctx, cstate);
}
