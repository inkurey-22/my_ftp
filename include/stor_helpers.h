/*
** EPITECH PROJECT, 2026
** my_ftp
** File description:
** stor_helpers
*/

// clang-format off
#ifndef STOR_HELPERS_H
    #define STOR_HELPERS_H

    #include "client_state.h"
    #include "commands.h"
// clang-format on

void stor_child(stor_transfer_ctx_t *ctx, client_state_t *cstate);
void stor_parent(stor_transfer_ctx_t *ctx);
void stor_fork_and_transfer(stor_transfer_ctx_t *ctx, client_state_t *cstate);
void handle_file_receive(int file_fd, int data_conn_fd,
    struct client_state_t *cstate);
void send_transfer_success(struct client_state_t *cstate);
void send_transfer_failure(struct client_state_t *cstate);
void send_transfer_error_and_close(int file_fd, int data_conn_fd,
    struct client_state_t *cstate);

#endif// STOR_HELPERS_H
