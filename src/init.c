/*
** EPITECH PROJECT, 2026
** my_ftp
** File description:
** loop
*/

#include "ftp.h"

#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

static int create_server_socket(void)
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    return sockfd;
}

static int set_socket_options(int sockfd)
{
    int opt = 1;

    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
        return -1;
    return 0;
}

static void init_sockaddr_in(struct sockaddr_in *addr, uint16_t port)
{
    memset(addr, 0, sizeof(struct sockaddr_in));
    addr->sin_family = AF_INET;
    addr->sin_addr.s_addr = htonl(INADDR_ANY);
    addr->sin_port = htons(port);
}

static int bind_server_socket(int sockfd, uint16_t port)
{
    struct sockaddr_in addr;

    init_sockaddr_in(&addr, port);
    if (bind(sockfd, (struct sockaddr *) &addr, sizeof(addr)) < 0)
        return -1;
    return 0;
}

static int listen_server_socket(int sockfd)
{
    if (listen(sockfd, 5) < 0)
        return -1;
    return 0;
}

static int setup_server_socket(uint16_t port)
{
    int sockfd = create_server_socket();

    if (sockfd < 0)
        return -1;
    if (set_socket_options(sockfd) < 0) {
        close(sockfd);
        return -1;
    }
    if (bind_server_socket(sockfd, port) < 0) {
        close(sockfd);
        return -1;
    }
    if (listen_server_socket(sockfd) < 0) {
        close(sockfd);
        return -1;
    }
    return sockfd;
}

int8_t launch_server(struct ftp_server_s *server)
{
    int sockfd = setup_server_socket(server->port);

    if (sockfd < 0)
        return 84;
    server->fd = sockfd;
    run_server_poll_loop(server->fd);
    close(sockfd);
    return 0;
}
