/*
** EPITECH PROJECT, 2026
** my_ftp
** File description:
** stor_transfer.c - core transfer logic for STOR
*/

#include "stor_helpers.h"
#include <unistd.h>

void handle_file_receive(int file_fd, int data_conn_fd,
    struct client_state_t *cstate)
{
    ssize_t bytes_read;
    char buf[4096];

    bytes_read = read(data_conn_fd, buf, sizeof(buf));
    while (bytes_read > 0) {
        if (write(file_fd, buf, bytes_read) != bytes_read) {
            send_transfer_error_and_close(file_fd, data_conn_fd, cstate);
            return;
        }
        bytes_read = read(data_conn_fd, buf, sizeof(buf));
    }
    close(file_fd);
    close(data_conn_fd);
    if (bytes_read < 0)
        send_transfer_failure(cstate);
    else
        send_transfer_success(cstate);
}
