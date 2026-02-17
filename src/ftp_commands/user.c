/*
** EPITECH PROJECT, 2026
** my_ftp
** File description:
** user
*/

#include "commands.h"
#include "ftp.h"

#include <string.h>
#include <unistd.h>

void ftp_cmd_user(struct ftp_server_s *server, int client_fd, char *buffer,
    int *logged_in)
{
    char *username = buffer + 5;

    (void) server;
    if (strncmp(username, "Anonymous", 9) == 0) {
        my_send(client_fd, "230 User logged in, proceed.\r\n", 31, 0);
        *logged_in = 1;
    } else {
        my_send(client_fd, "530 Only anonymous login allowed.\r\n", 37, 0);
        *logged_in = 0;
    }
}
