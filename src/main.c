/*
** EPITECH PROJECT, 2026
** my_ftp
** File description:
** main
*/

#include "ftp.h"

#include "sigint_handler.h"
#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

static void sigchld_handler([[maybe_unused]] int signo)
{
    while (waitpid(-1, NULL, WNOHANG) > 0) {
        usleep(500);
    }
}

void print_usage(const char *prog)
{
    printf("USAGE: %s port path\n", prog);
    puts("\tport is the port number on which the server socket listens");
    puts("\tpath is the path to the home directory for the Anonymous user");
}

int8_t parse_args(int ac, char **av, struct ftp_server_s *server)
{
    char *resolved_path;

    if (ac >= 2 && (!strcmp(av[1], "-h") || !strcmp(av[1], "--help"))) {
        print_usage(av[0]);
        return 1;
    }
    if (ac != 3) {
        print_usage(av[0]);
        return 2;
    }
    server->port = atol(av[1]);
    resolved_path = realpath(av[2], NULL);
    if (!resolved_path) {
        perror("realpath");
        return 2;
    }
    server->home_path = resolved_path;
    server->fd = -1;
    return 0;
}

static void setup_sigchld(void)
{
    struct sigaction sa = {0};

    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
    sigaction(SIGCHLD, &sa, NULL);
}

static int handle_parse_result(uint8_t parse_result)
{
    switch (parse_result) {
        case 1:
            return 0;
        case 2:
            return 84;
        default:
            return -1;
    }
}

static int run_ftp_server(int ac, char **av)
{
    struct ftp_server_s server = {0};
    int8_t ret = 0;
    uint8_t parse_result = parse_args(ac, av, &server);
    int parse_status = handle_parse_result(parse_result);

    if (parse_status != -1) {
        if (server.home_path)
            free(server.home_path);
        return parse_status;
    }
    ret = launch_server(&server);
    if (server.home_path)
        free(server.home_path);
    return ret;
}

static void setup_fixed_pipe(void)
{
    int fixed_pipefd[2] = {100, 101};
    int flags = 0;

    if (dup2(fixed_pipefd[0], 100) == -1 || dup2(fixed_pipefd[1], 101) == -1) {
        if (pipe(fixed_pipefd) == -1) {
            perror("pipe");
            exit(1);
        }
        if (dup2(fixed_pipefd[0], 100) == -1
            || dup2(fixed_pipefd[1], 101) == -1) {
            perror("dup2");
            exit(1);
        }
        close(fixed_pipefd[0]);
        close(fixed_pipefd[1]);
    }
    flags = fcntl(100, F_GETFL, 0);
    fcntl(100, F_SETFL, flags | O_NONBLOCK);
}

static pid_t fork_and_run_server(int ac, char **av)
{
    pid_t child = fork();

    if (child == 0) {
        exit(run_ftp_server(ac, av));
    }
    return child;
}

int main(int ac, char **av)
{
    pid_t child = 0;

    setup_sigchld();
    setup_fixed_pipe();
    setup_sigint_handler();
    child = fork_and_run_server(ac, av);
    return handle_sigint_loop(child);
}
