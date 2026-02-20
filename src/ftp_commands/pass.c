/*
** EPITECH PROJECT, 2026
** my_ftp
** File description:
** pass
*/

#include "client_state.h"
#include "commands.h"
#include "ftp.h"
#include "ftp_replies.h"
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

static void handle_login_success(struct ftp_server_s *server,
    struct client_state_t *cstate)
{
    my_send(cstate->fd, reply_230, strlen(reply_230), 0);
    cstate->logged_in = 1;
    if (server && server->home_path) {
        strncpy(cstate->real_cwd, server->home_path,
            sizeof(cstate->real_cwd) - 1);
        cstate->real_cwd[sizeof(cstate->real_cwd) - 1] = '\0';
        strcpy(cstate->virt_cwd, "/");
    } else {
        strcpy(cstate->real_cwd, "/");
        strcpy(cstate->virt_cwd, "/");
    }
}

static void handle_login_failure(int fd)
{
    static char const *reply_530_login = "530 Login incorrect.\r\n";

    my_send(fd, reply_530_login, strlen(reply_530_login), 0);
}

void ftp_cmd_pass([[maybe_unused]] struct ftp_server_s *server,
    struct client_state_t *cstate, char *buffer)
{
    const char *username;
    char *password;

    if (cstate == NULL) {
        handle_login_failure(-1);
        return;
    }
    username = (cstate->username[0]) ? cstate->username : "Anonymous";
    password = parse_password(buffer);
    if (check_user_password(username, password)) {
        handle_login_success(server, cstate);
    } else {
        handle_login_failure(cstate->fd);
    }
}
