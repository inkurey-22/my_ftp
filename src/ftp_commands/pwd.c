/*
** EPITECH PROJECT, 2026
** my_ftp
** File description:
** pwd
*/

#include "commands.h"
#include "ftp.h"

#include <string.h>
#include <unistd.h>

void ftp_cmd_pwd(struct ftp_server_s *server, int client_fd, char *buffer,
    int *logged_in)
{
    char cwd[1024];

    (void) server;
    (void) buffer;
    if (!*logged_in) {
        my_send(client_fd, "530 Not logged in.\r\n", 21, 0);
        return;
    }
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        my_send(client_fd, "550 Failed to get current directory.\r\n", 39, 0);
        return;
    }
    my_send(client_fd, "257 \"", 5, 0);
    my_send(client_fd, cwd, strlen(cwd), 0);
    my_send(client_fd, "\" is the current directory.\r\n", 29, 0);
}
