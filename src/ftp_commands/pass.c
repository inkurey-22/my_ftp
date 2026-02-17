/*
** EPITECH PROJECT, 2026
** my_ftp
** File description:
** pass
*/

#include "client_state.h"
#include "commands.h"
#include "ftp.h"
#include <string.h>

static char *parse_password(char *buffer)
{
    char *password = buffer + 4;
    char *end;

    while (*password == ' ')
        password++;
    if (*password == '\0' || *password == '\r' || *password == '\n') {
        *password = '\0';
        return password;
    }
    end = password + strlen(password) - 1;
    while (end > password && (*end == '\r' || *end == '\n' || *end == ' ')) {
        *end = '\0';
        --end;
    }
    return password;
}

void ftp_cmd_pass([[maybe_unused]] struct ftp_server_s *server,
    struct client_state_t *cstate, char *buffer)
{
    const char *username;
    char *password;

    if (cstate == NULL) {
        my_send(-1, "530 Login incorrect.\r\n", 25, 0);
        return;
    }
    username = (cstate->username[0]) ? cstate->username : "Anonymous";
    password = parse_password(buffer);
    if (check_user_password(username, password)) {
        my_send(cstate->fd, "230 User logged in, proceed.\r\n", 31, 0);
        cstate->logged_in = 1;
    } else {
        my_send(cstate->fd, "530 Login incorrect.\r\n", 25, 0);
    }
}
