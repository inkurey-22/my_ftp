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

static int retr_prepare(struct client_state_t *cstate, char *buffer,
    retr_transfer_ctx_t *ctx)
{
    if (check_data_connection(cstate) < 0)
        return -1;
    if (handle_filepath(cstate, buffer, &ctx->filepath) < 0)
        return -1;
    if (handle_open_file(cstate, ctx->filepath, &ctx->file_fd) < 0)
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

    // Signal EOF to client before closing socket
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

void ftp_cmd_retr([[maybe_unused]] struct ftp_server_s *server,
    struct client_state_t *cstate, char *buffer)
{
    retr_transfer_ctx_t ctx = {0};

    if (retr_prepare(cstate, buffer, &ctx) < 0)
        return;
    retr_fork_and_transfer(&ctx, cstate);
}
