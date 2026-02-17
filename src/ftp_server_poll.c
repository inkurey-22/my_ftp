/*
** EPITECH PROJECT, 2026
** my_ftp
** File description:
** ftp_server_poll - poll/fork server loop
*/

#include "ftp.h"
#include <netinet/in.h>
#include <poll.h>
#include <signal.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

static void close_client(int fd)
{
    if (fd >= 0)
        close(fd);
}

static void handle_new_connection(struct ftp_server_s *server, int listen_fd,
    struct pollfd *pfds)
{
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int client_fd =
        accept(listen_fd, (struct sockaddr *) &client_addr, &client_len);
    pid_t pid;

    (void) pfds;
    if (client_fd < 0)
        return;
    pid = fork();
    if (pid == 0) {
        handle_client_session(server, client_fd);
        close_client(client_fd);
        _exit(0);
    } else if (pid > 0) {
        close_client(client_fd);
    }
}

static void handle_client_ready(struct ftp_server_s *server, int idx,
    struct pollfd *pfds)
{
    pid_t pid = fork();

    if (pid == 0) {
        handle_client_session(server, pfds[idx].fd);
        close_client(pfds[idx].fd);
        _exit(0);
    } else if (pid > 0) {
        close_client(pfds[idx].fd);
        pfds[idx].fd = -1;
        pfds[idx].events = 0;
    }
}

static void handle_poll_events(struct ftp_server_s *server, int listen_fd,
    struct pollfd *pfds, int nfds)
{
    int i;

    if (pfds[0].revents & POLLIN)
        handle_new_connection(server, listen_fd, pfds);
    for (i = 1; i < nfds; ++i) {
        if (pfds[i].fd >= 0 && (pfds[i].revents & POLLIN))
            handle_client_ready(server, i, pfds);
    }
    for (i = 0; i < nfds; ++i)
        pfds[i].revents = 0;
}

int run_server_poll_loop(struct ftp_server_s *server)
{
    struct pollfd pfds[MAX_CLIENTS + 1];
    int nfds = MAX_CLIENTS + 1;
    int ready;

    memset(pfds, -1, sizeof(pfds));
    pfds[0].fd = server->fd;
    pfds[0].events = POLLIN;
    signal(SIGCHLD, SIG_IGN);
    while (1) {
        ready = poll(pfds, nfds, -1);
        if (ready < 0)
            continue;
        handle_poll_events(server, server->fd, pfds, nfds);
    }
    return 0;
}
