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
// clang-format on

struct client_state_t {
    int fd;
    char buffer[1024];
    size_t buf_used;
    bool logged_in;
    bool to_close;
    char username[64];// Store last provided username
};

typedef struct client_state_t client_state_t;

#endif /* CLIENT_STATE_H */
