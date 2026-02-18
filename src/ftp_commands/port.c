/*
** EPITECH PROJECT, 2026
** my_ftp
** File description:
** port (active mode)
*/

#include "client_state.h"
#include "ftp.h"
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>

static int parse_port_params(const char *arg, struct sockaddr_in *out_addr)
{
    int h1;
    int h2;
    int h3;
    int h4;
    int p1;
    int p2;

    if (sscanf(arg, "%d,%d,%d,%d,%d,%d", &h1, &h2, &h3, &h4, &p1, &p2) != 6)
        return -1;
    if ((h1 | h2 | h3 | h4) < 0 || h1 > 255 || h2 > 255 || h3 > 255 || h4 > 255
        || p1 < 0 || p1 > 255 || p2 < 0 || p2 > 255)
        return -1;
    memset(out_addr, 0, sizeof(*out_addr));
    out_addr->sin_family = AF_INET;
    out_addr->sin_addr.s_addr =
        htonl((h1 << 24) | (h2 << 16) | (h3 << 8) | h4);
    out_addr->sin_port = htons((p1 << 8) | p2);
    return 0;
}

void ftp_cmd_port([[maybe_unused]] struct ftp_server_s *server,
    struct client_state_t *cstate, char *buffer)
{
    struct sockaddr_in data_addr;
    char *arg = buffer;

    if (strncasecmp(arg, "PORT ", 5) == 0)
        arg += 5;
    while (*arg == ' ' || *arg == '\t')
        arg++;
    if (parse_port_params(arg, &data_addr) < 0) {
        my_send(cstate->fd, "501 Syntax error in parameters or arguments.\r\n",
            44, 0);
        return;
    }
    cstate->data_fd = -1;
    cstate->active_addr = data_addr;
    cstate->active_mode = 1;
    my_send(cstate->fd, "200 Command okay.\r\n", 18, 0);
}
