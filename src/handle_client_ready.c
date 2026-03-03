/*
** EPITECH PROJECT, 2026
** my_ftp
** File description:
** handle_client_ready
*/

#include "handle_client_ready.h"
#include "ftp.h"
#include "ftp_replies.h"
#include "ftp_server_poll_internals.h"
#include <string.h>
#include <unistd.h>

static int process_single_command(client_context_t *ctx, client_state_t *c,
    char *cmd_buffer, size_t line_len)
{
    int cmd_result;

    memcpy(c->buffer, cmd_buffer, line_len + 1);
    cmd_result = handle_command(ctx->server, c);
    if (cmd_result == 0) {
        my_send(c->fd, REPLY_500, strlen(REPLY_500), 0);
        return 1;
    }
    if (cmd_result == 2) {
        close_and_reset_client(c, ctx->pfds, ctx->idx);
        return -1;
    }
    return 1;
}

static void shift_remaining_buffer(client_state_t *c, char *line_start)
{
    c->buf_used = strlen(line_start);
    if (c->buf_used > 0 && line_start != c->buffer) {
        memmove(c->buffer, line_start, c->buf_used);
    }
    c->buffer[c->buf_used] = '\0';
}

static void process_complete_commands(client_context_t *ctx, client_state_t *c)
{
    char *line_start = c->buffer;
    char *crlf_pos;
    size_t line_len;
    char cmd_buffer[1024];

    crlf_pos = strstr(line_start, "\r\n");
    while (crlf_pos != NULL) {
        line_len = crlf_pos - line_start;
        if (line_len >= sizeof(cmd_buffer) - 1)
            line_len = sizeof(cmd_buffer) - 1;
        memcpy(cmd_buffer, line_start, line_len);
        cmd_buffer[line_len] = '\0';
        if (process_single_command(ctx, c, cmd_buffer, line_len) == -1)
            return;
        line_start = crlf_pos + 2;
        crlf_pos = strstr(line_start, "\r\n");
    }
    shift_remaining_buffer(c, line_start);
}

void handle_client_ready(client_context_t *ctx)
{
    client_state_t *c = &ctx->client_states[ctx->idx];
    ssize_t n;
    size_t available_space = sizeof(c->buffer) - c->buf_used - 1;

    if (available_space == 0) {
        c->buf_used = 0;
        c->buffer[0] = '\0';
    }
    n = read(c->fd, c->buffer + c->buf_used, available_space);
    if (n <= 0) {
        return;
    }
    c->buf_used += n;
    c->buffer[c->buf_used] = '\0';
    process_complete_commands(ctx, c);
}
