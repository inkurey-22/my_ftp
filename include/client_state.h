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
    char real_cwd[1024];
    char virt_cwd[1024];
    char buffer[1024];
    char username[64];
    struct sockaddr_in active_addr;
    size_t buf_used;
    int fd;
    int data_fd;
    int active_mode;
    bool logged_in;
    bool to_close;
    char pad[10];
};

typedef struct client_state_t client_state_t;

#endif /* CLIENT_STATE_H */
