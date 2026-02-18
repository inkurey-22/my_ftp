/*
** EPITECH PROJECT, 2026
** my_ftp
** File description:
** pasv (passive mode)
*/

#include "client_state.h"
#include "ftp.h"
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

static int setup_pasv_socket(int control_fd, struct sockaddr_in *data_addr)
{
    socklen_t addr_len = sizeof(*data_addr);

    if (getsockname(control_fd, (struct sockaddr *) data_addr, &addr_len) < 0)
        return -1;
    data_addr->sin_port = htons(0);
    return 0;
}

static int finalize_pasv_socket(int data_fd, struct sockaddr_in *data_addr)
{
    socklen_t addr_len = sizeof(*data_addr);

    if (bind(data_fd, (struct sockaddr *) data_addr, sizeof(*data_addr)) < 0) {
        close(data_fd);
        return -1;
    }
    if (listen(data_fd, 1) < 0) {
        close(data_fd);
        return -1;
    }
    addr_len = sizeof(*data_addr);
    if (getsockname(data_fd, (struct sockaddr *) data_addr, &addr_len) < 0) {
        close(data_fd);
        return -1;
    }
    return data_fd;
}

static int create_pasv_socket(int control_fd, struct sockaddr_in *data_addr)
{
    int data_fd;
    int optval = 1;

    data_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (data_fd < 0)
        return -1;
    setsockopt(data_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
    if (setup_pasv_socket(control_fd, data_addr) < 0) {
        close(data_fd);
        return -1;
    }
    return finalize_pasv_socket(data_fd, data_addr);
}

static void send_pasv_reply(struct client_state_t *cstate,
    struct sockaddr_in *data_addr)
{
    char reply[128];
    uint32_t ip = ntohl(data_addr->sin_addr.s_addr);
    uint16_t port = ntohs(data_addr->sin_port);

    snprintf(reply, sizeof(reply),
        "227 Entering Passive Mode (%u,%u,%u,%u,%u,%u).\r\n",
        (ip >> 24) & 0xFF, (ip >> 16) & 0xFF, (ip >> 8) & 0xFF, ip & 0xFF,
        port / 256, port % 256);
    my_send(cstate->fd, reply, strlen(reply), 0);
}

void ftp_cmd_pasv(struct ftp_server_s *server, struct client_state_t *cstate,
    [[maybe_unused]] char *buffer)
{
    struct sockaddr_in data_addr;
    int data_fd = create_pasv_socket(server->fd, &data_addr);

    if (data_fd < 0) {
        my_send(cstate->fd, "425 Can't open data connection.\r\n", 34, 0);
        return;
    }
    cstate->data_fd = data_fd;
    send_pasv_reply(cstate, &data_addr);
}
