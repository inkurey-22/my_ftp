/*
** EPITECH PROJECT, 2026
** my_ftp
** File description:
** list_helpers
*/

// clang-format off
#ifndef LIST_HELPERS_H
    #define LIST_HELPERS_H

    #include "client_state.h"
    #include <dirent.h>
    #include <sys/types.h>
// clang-format on

// Data connection helpers
int check_data_connection(struct client_state_t *cstate);
int open_data_connection(struct client_state_t *cstate);

// LIST command helpers for logic
int check_logged_in_and_data_conn(struct client_state_t *cstate);
int prepare_data_connection(struct client_state_t *cstate);
char *parse_list_arg(char *buffer);

// Formatting and listing helpers
void get_permissions(mode_t mode, char *perms);
void get_time_str(time_t mtime, char *buf, size_t buflen);
void send_file_listing(int data_fd, const char *path, const char *name);
void send_directory_listing(int data_fd, const char *dirpath);

// Main logic
void handle_list_logic(int data_fd, const char *target, const char *arg);

#endif// LIST_HELPERS_H
