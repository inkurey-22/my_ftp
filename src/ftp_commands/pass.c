/*
** EPITECH PROJECT, 2026
** my_ftp
** File description:
** pass
*/

#include "commands.h"
#include "ftp.h"

#include <unistd.h>

void ftp_cmd_pass(int client_fd, char *buffer, int *logged_in)
{
    (void) buffer;
    if (*logged_in)
        my_send(client_fd, "230 User logged in, proceed.\r\n", 31, 0);
    else
        my_send(client_fd, "530 Login incorrect.\r\n", 25, 0);
}
