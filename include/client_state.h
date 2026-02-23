/*
** EPITECH PROJECT, 2026
** my_ftp
** File description:
** client_state_t struct definition
*/

// clang-format off
#ifndef CLIENT_STATE_H
    #define CLIENT_STATE_H

    #include <stdbool.h>
    #include <stddef.h>
    #include <netinet/in.h>
// clang-format on

struct client_state_t {
    int fd;
    char buffer[1024];
    size_t buf_used;
    bool logged_in;
    bool to_close;
    char username[64];
    int data_fd;
    struct sockaddr_in active_addr;
    int active_mode;
    char real_cwd[1024];
    char virt_cwd[1024];
};

typedef struct client_state_t client_state_t;

#endif /* CLIENT_STATE_H */
