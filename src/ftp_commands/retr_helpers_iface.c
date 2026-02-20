/*
** EPITECH PROJECT, 2026
** my_ftp
** File description:
** retr_helpers_iface
*/

#include "client_state.h"
#include "ftp.h"
#include "ftp_replies.h"
#include "retr_helpers.h"
#include <string.h>
#include <unistd.h>

int check_data_connection(struct client_state_t *cstate)
{
    if (cstate->data_fd < 0) {
        my_send(cstate->fd, reply_425_use_pasv, strlen(reply_425_use_pasv), 0);
        return -1;
    }
    return 0;
}

int handle_filepath(struct client_state_t *cstate, char *buffer,
    char **filepath)
{
    *filepath = parse_filepath(buffer);
    if (!*filepath) {
        my_send(cstate->fd, reply_501_missing_file_path,
            strlen(reply_501_missing_file_path), 0);
        return -1;
    }
    return 0;
}

int handle_open_file(struct client_state_t *cstate, char *filepath,
    int *file_fd)
{
    if (open_file_for_retr(filepath, file_fd) < 0) {
        close(cstate->data_fd);
        cstate->data_fd = -1;
        my_send(cstate->fd, reply_550_failed_open_file,
            strlen(reply_550_failed_open_file), 0);
        return -1;
    }
    return 0;
}

int handle_accept_data_conn(struct client_state_t *cstate, int file_fd,
    int *data_conn_fd)
{
    if (accept_data_connection(cstate, data_conn_fd) < 0) {
        close(file_fd);
        my_send(cstate->fd, reply_425_cant_open_data,
            strlen(reply_425_cant_open_data), 0);
        return -1;
    }
    return 0;
}

void handle_transfer_result(struct client_state_t *cstate, int transfer_result)
{
    if (transfer_result < 0) {
        my_send(cstate->fd, reply_426_transfer_aborted,
            strlen(reply_426_transfer_aborted), 0);
    } else {
        my_send(cstate->fd, reply_226, strlen(reply_226), 0);
    }
}
