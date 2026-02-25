/*
** EPITECH PROJECT, 2026
** my_ftp
** File description:
** sigint_handler
*/

// clang-format off
#ifndef SIGINT_HANDLER_H
    #define SIGINT_HANDLER_H
    #include <signal.h>
    #include <unistd.h>
    #define SIGINT_NOTIFY_SIGNAL SIGINT
// clang-format on

void sigint_handler(int signo, siginfo_t *info, void *context);
void setup_sigint_handler(void);
int handle_sigint_loop(pid_t child);

#endif// SIGINT_HANDLER_H
