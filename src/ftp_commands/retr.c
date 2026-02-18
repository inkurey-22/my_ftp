/*
** EPITECH PROJECT, 2026
** my_ftp
** File description:
** retr
*/

#include "ftp.h"
#include "retr_helpers.h"
#include <unistd.h>

void ftp_cmd_retr([[maybe_unused]] struct ftp_server_s *server,
    struct client_state_t *cstate, char *buffer)
{
    char *filepath;
    int file_fd;
    int data_conn_fd;
    int transfer_result;

    if (check_data_connection(cstate) < 0)
        return;
    if (handle_filepath(cstate, buffer, &filepath) < 0)
        return;
    if (handle_open_file(cstate, filepath, &file_fd) < 0)
        return;
    my_send(cstate->fd,
        "150 File status okay; about to open data connection.\r\n", 49, 0);
    if (handle_accept_data_conn(cstate, file_fd, &data_conn_fd) < 0)
        return;
    transfer_result = send_file_over_data_conn(file_fd, data_conn_fd);
    close(file_fd);
    close(data_conn_fd);
    handle_transfer_result(cstate, transfer_result);
}
