/*
** EPITECH PROJECT, 2026
** my_ftp
** File description:
** main
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ftp.h"

void print_usage(const char *prog)
{
    printf("USAGE: %s port path\n", prog);
    puts("\tport is the port number on which the server socket listens");
    puts("\tpath is the path to the home directory for the Anonymous user");
}

int8_t parse_args(int ac, char **av, struct ftp_server_s *server)
{
    if (ac >= 2 && (!strcmp(av[1], "-h") || !strcmp(av[1], "--help"))) {
        print_usage(av[0]);
        return 1;
    }
    if (ac != 3) {
        print_usage(av[0]);
        return 2;
    }
    server->port = atol(av[1]);
    server->home_path = av[2];
    server->fd = -1;
    return 0;
}

int main(int ac, char **av)
{
    struct ftp_server_s server;

    switch (parse_args(ac, av, &server)) {
        case 1:
            return 0;
        case 2:
            return 84;
        default:
            return launch_server(&server);
    }
}
