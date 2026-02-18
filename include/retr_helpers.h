/*
** EPITECH PROJECT, 2026
** my_ftp
** File description:
** retr_helpers
*/

// clang-format off
#ifndef RETR_HELPERS_H
    #define RETR_HELPERS_H

    #include "client_state.h"
// clang-format on

int write_all(int fd, const char *buf, size_t count);
int send_file_over_data_conn(int file_fd, int data_conn_fd);
char *parse_filepath(char *buffer);
int open_file_for_retr(const char *filepath, int *file_fd);
int accept_data_connection(struct client_state_t *cstate, int *data_conn_fd);
int check_data_connection(struct client_state_t *cstate);
int handle_filepath(struct client_state_t *cstate, char *buffer,
    char **filepath);
int handle_open_file(struct client_state_t *cstate, char *filepath,
    int *file_fd);
int handle_accept_data_conn(struct client_state_t *cstate, int file_fd,
    int *data_conn_fd);
void handle_transfer_result(struct client_state_t *cstate,
    int transfer_result);

#endif /* RETR_HELPERS_H */
