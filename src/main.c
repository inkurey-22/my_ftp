/*
** EPITECH PROJECT, 2026
** my_ftp
** File description:
** main
*/

#include "ftp.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int main(int ac, char **av)
{
    struct ftp_server_s server = {0};
    int8_t ret;
    uint8_t parse_result = parse_args(ac, av, &server);

    switch (parse_result) {
        case 1:
            return 0;
        case 2:
            return 84;
        default:
            break;
    }
    ret = launch_server(&server);
    if (server.home_path) {
        free(server.home_path);
    }
    return ret;
}
