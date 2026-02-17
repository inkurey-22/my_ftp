/*
** EPITECH PROJECT, 2026
** my_ftp
** File description:
** ftp_server_poll_internals
*/

#include "ftp_server_poll_internals.h"
#include "commands.h"
#include <string.h>
#include <unistd.h>

static int is_quit_command(const char *buffer)
{
    return strncmp(buffer, "QUIT", 4) == 0;
}

int handle_command(struct ftp_server_s *server, client_state_t *c)
{
    size_t cmd_len = 0;

    for (int i = 0; i < FTP_COMMANDS_COUNT; ++i) {
        cmd_len = strlen(FTP_COMMANDS[i].name);
        if (strncmp(c->buffer, FTP_COMMANDS[i].name, cmd_len) == 0) {
            FTP_COMMANDS[i].func(server, c, c->buffer);
            return is_quit_command(c->buffer) ? 2 : 1;
        }
    }
    return 0;
}

void close_and_reset_client(client_state_t *c, struct pollfd *pfds, int idx)
{
    if (c->fd >= 0)
        close(c->fd);
    c->fd = -1;
    pfds[idx].fd = -1;
    pfds[idx].events = 0;
}
