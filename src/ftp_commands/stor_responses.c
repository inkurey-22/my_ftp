/*
** EPITECH PROJECT, 2026
** my_ftp
** File description:
** stor_responses.c - transfer response helpers for STOR
*/

#include "client_state.h"
#include "ftp.h"
#include "ftp_replies.h"
#include <stddef.h>
#include <string.h>
#include <unistd.h>

void send_transfer_error_and_close(int file_fd, int data_conn_fd,
    struct client_state_t *cstate)
{
    my_send(cstate->fd, REPLY_426_TRANSFER_ABORTED,
        strlen(REPLY_426_TRANSFER_ABORTED), 0);
    close(file_fd);
    close(data_conn_fd);
}

void send_transfer_success(struct client_state_t *cstate)
{
    my_send(cstate->fd, REPLY_226, strlen(REPLY_226), 0);
}

void send_transfer_failure(struct client_state_t *cstate)
{
    my_send(cstate->fd, REPLY_426_TRANSFER_ABORTED,
        strlen(REPLY_426_TRANSFER_ABORTED), 0);
}
