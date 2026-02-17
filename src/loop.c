/*
** EPITECH PROJECT, 2026
** my_ftp
** File description:
** loop
*/

#include "commands.h"
#include "ftp.h"

#include <string.h>
#include <unistd.h>

static void handle_ftp_command(struct ftp_server_s *server, int client_fd,
    char *buffer, int *logged_in)
{
    int found = 0;
    int i = 0;
    unsigned int cmd_len = 0U;
    struct client_state_t client_state;

    memset(&client_state, 0, sizeof(client_state));
    client_state.fd = client_fd;
    client_state.logged_in = logged_in ? *logged_in : 0;
    for (i = 0; i < FTP_COMMANDS_COUNT; ++i) {
        cmd_len = strlen(FTP_COMMANDS[i].name);
        if (strncmp(buffer, FTP_COMMANDS[i].name, cmd_len) != 0)
            continue;
        if (strcmp(FTP_COMMANDS[i].name, "USER") == 0 && buffer[4] != ' ')
            continue;
        FTP_COMMANDS[i].func(server, &client_state, buffer);
        found = 1;
        break;
    }
    if (!found)
        my_send(client_fd, "502 Command not implemented.\r\n", 31, 0);
}

void handle_client_session(struct ftp_server_s *server, int client_fd)
{
    const char *greeting = "220 You lost the game :3.\r\n";
    int logged_in = 0;
    char buffer[1024] = {0};
    ssize_t n = 0;

    my_send(client_fd, greeting, strlen(greeting), 0);
    while (1) {
        memset(buffer, 0, sizeof(buffer));
        n = read(client_fd, buffer, sizeof(buffer) - 1);
        if (n <= 0)
            break;
        handle_ftp_command(server, client_fd, buffer, &logged_in);
        if (strncmp(buffer, "QUIT", 4) == 0)
            break;
    }
    close(client_fd);
}
