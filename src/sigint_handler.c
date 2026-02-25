/*
** EPITECH PROJECT, 2026
** my_ftp
** File description:
** sigint_handler
*/

#include "sigint_handler.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

void sigint_handler(int signo, [[maybe_unused]] siginfo_t *info,
    [[maybe_unused]] void *context)
{
    int fd = 101;

    if (signo == SIGINT_NOTIFY_SIGNAL && fd != -1) {
        write(fd, "x", 1);
    }
}

void setup_sigint_handler(void)
{
    struct sigaction sa = {0};

    sa.sa_sigaction = sigint_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_SIGINFO;
    sigaction(SIGINT_NOTIFY_SIGNAL, &sa, NULL);
}

static int handle_sigint_event(int sig_pipefd[2], pid_t child, int *status)
{
    printf("\nSIGINT received, exiting gracefully.\n");
    kill(child, SIGTERM);
    waitpid(child, status, 0);
    close(sig_pipefd[0]);
    close(sig_pipefd[1]);
    return 0;
}

static int handle_child_exit(int sig_pipefd[2], int status)
{
    close(sig_pipefd[0]);
    close(sig_pipefd[1]);
    return WIFEXITED(status) ? WEXITSTATUS(status) : 1;
}

int handle_sigint_loop(pid_t child)
{
    fd_set readfds;
    int fixed_pipefd[2] = {100, 101};
    int status = 0;
    int ready = 0;
    pid_t w = 0;

    while (1) {
        FD_ZERO(&readfds);
        FD_SET(fixed_pipefd[0], &readfds);
        status = 0;
        ready = select(fixed_pipefd[0] + 1, &readfds, NULL, NULL, NULL);
        if (ready > 0 && FD_ISSET(fixed_pipefd[0], &readfds)) {
            return handle_sigint_event(fixed_pipefd, child, &status);
        }
        w = waitpid(child, &status, WNOHANG);
        if (w == child) {
            return handle_child_exit(fixed_pipefd, status);
        }
    }
}
