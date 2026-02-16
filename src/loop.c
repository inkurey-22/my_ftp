/*
** EPITECH PROJECT, 2026
** my_ftp
** File description:
** loop
*/

#include "commands.h"
#include "ftp.h"

#include <fcntl.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

static void handle_user_command(int client_fd, char *buffer)
{
    if (strstr(buffer, "Anonymous") != NULL) {
        my_send(client_fd, "331 Anonymous login ok, send your password.\r\n",
            44, 0);
    } else {
        my_send(client_fd, "530 Only anonymous login allowed.\r\n", 37, 0);
    }
}

static void handle_pass_command(int client_fd, int *logged_in)
{
    my_send(client_fd, "230 User logged in, proceed.\r\n", 31, 0);
    *logged_in = 1;
}

static void handle_quit_command(int client_fd)
{
    my_send(client_fd, "221 Service closing control connection.\r\n", 43, 0);
}

static void handle_file_command(int client_fd)
{
    pid_t pid = fork();

    if (pid == 0) {
        my_send(client_fd,
            "150 File status okay; about to open data connection.\r\n", 56, 0);
        my_send(client_fd, "226 Closing data connection.\r\n", 33, 0);
        close(client_fd);
        _exit(0);
    }
}

static void handle_ftp_command(int client_fd, char *buffer, int *logged_in)
{
    int found = 0;
    int i;
    unsigned int cmd_len;

    for (i = 0; i < FTP_COMMANDS_COUNT; ++i) {
        cmd_len = strlen(FTP_COMMANDS[i].name);
        if (strncmp(buffer, FTP_COMMANDS[i].name, cmd_len) != 0)
            continue;
        if (strcmp(FTP_COMMANDS[i].name, "USER") == 0 && buffer[4] != ' ')
            continue;
        FTP_COMMANDS[i].func(client_fd, buffer, logged_in);
        found = 1;
        break;
    }
    if (!found) {
        my_send(client_fd, "502 Command not implemented.\r\n", 31, 0);
    }
}

static void handle_client_session(int client_fd)
{
    const char *greeting = "220 You lost the game :3.\r\n";
    int logged_in = 0;
    char buffer[1024];
    ssize_t n = 0;

    my_send(client_fd, greeting, strlen(greeting), 0);
    while (1) {
        memset(buffer, 0, sizeof(buffer));
        n = read(client_fd, buffer, sizeof(buffer) - 1);
        if (n <= 0)
            break;
        handle_ftp_command(client_fd, buffer, &logged_in);
        if (strncmp(buffer, "QUIT", 4) == 0)
            break;
    }
    close(client_fd);
}

int run_server_loop(struct ftp_server_s *server)
{
    int client_fd;
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

    while (1) {
        client_fd =
            accept(server->fd, (struct sockaddr *) &client_addr, &client_len);
        if (client_fd < 0)
            continue;
        handle_client_session(client_fd);
    }
    return 0;
}
