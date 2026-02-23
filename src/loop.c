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

static int find_and_execute_command(struct ftp_server_s *server, char *buffer,
    int *logged_in, struct client_state_t *client_state)
{
    int i;
    unsigned int cmd_len;

    for (i = 0; i < FTP_COMMANDS_COUNT; ++i) {
        cmd_len = strlen(FTP_COMMANDS[i].name);
        if (strncmp(buffer, FTP_COMMANDS[i].name, cmd_len) != 0)
            continue;
        if (strcmp(FTP_COMMANDS[i].name, "USER") == 0 && buffer[4] != ' ')
            continue;
        FTP_COMMANDS[i].func(server, client_state, buffer);
        if (logged_in)
            *logged_in = client_state->logged_in;
        return 1;
    }
    return 0;
}

static void handle_ftp_command(struct ftp_server_s *server, int client_fd,
    char *buffer, int *logged_in)
{
    struct client_state_t client_state;

    memset(&client_state, 0, sizeof(client_state));
    client_state.fd = client_fd;
    client_state.logged_in = logged_in ? *logged_in : 0;
    if (!find_and_execute_command(server, buffer, logged_in, &client_state)) {
        my_send(client_fd, "502 Command not implemented.\r\n", 31, 0);
    }
}

void handle_client_session(struct ftp_server_s *server, int client_fd)
{
    const char *greeting = "220 Service ready for new user.\r\n";
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
