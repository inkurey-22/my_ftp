/*
** EPITECH PROJECT, 2026
** my_ftp
** File description:
** pwd
*/

#include "commands.h"
#include "ftp.h"

#include <string.h>
#include <unistd.h>

static void compute_vpath(const char *cwd, const struct ftp_server_s *server,
    char *vpath, size_t vpath_size)
{
    size_t home_len = 0;
    const char *sub = NULL;

    vpath[0] = '/';
    vpath[1] = '\0';
    if (!(server && server->home_path))
        return;
    home_len = strlen(server->home_path);
    if (strncmp(cwd, server->home_path, home_len) != 0) {
        vpath[1] = '\0';
        return;
    }
    sub = cwd + home_len;
    if (*sub == '\0') {
        vpath[1] = '\0';
        return;
    }
    snprintf(vpath, vpath_size, "/%s", sub[0] == '/' ? sub + 1 : sub);
}

void ftp_cmd_pwd([[maybe_unused]] struct ftp_server_s *server,
    struct client_state_t *cstate, [[maybe_unused]] char *buffer)
{
    char cwd[1024] = {0};
    char vpath[1024] = "/";

    if (cstate == NULL || !cstate->logged_in) {
        my_send(cstate ? cstate->fd : -1, "530 Not logged in.\r\n", 21, 0);
        return;
    }
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        my_send(cstate->fd, "550 Failed to get current directory.\r\n", 39, 0);
        return;
    }
    compute_vpath(cwd, server, vpath, sizeof(vpath));
    my_send(cstate->fd, "257 \"", 5, 0);
    my_send(cstate->fd, vpath, strlen(vpath), 0);
    my_send(cstate->fd, "\" is the current directory.\r\n", 29, 0);
}
