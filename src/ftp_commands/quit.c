/*
** EPITECH PROJECT, 2026
** my_ftp
** File description:
** quit
*/

#include "commands.h"
#include "ftp.h"

#include <unistd.h>

void ftp_cmd_quit(int client_fd, char *buffer, int *logged_in)
{
    (void) buffer;
    (void) logged_in;
    my_send(client_fd, "221 Service closing control connection.\r\n", 43, 0);
}
