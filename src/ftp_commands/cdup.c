/*
** EPITECH PROJECT, 2026
** my_ftp
** File description:
** ftp_cmd_cdup
*/

#include "commands.h"
#include "ftp_replies.h"
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static int get_parent_directory(const char *cwd, const char *home_path,
    char *parent, size_t parent_size)
{
    char *last;

    strncpy(parent, cwd, parent_size - 1);
    parent[parent_size - 1] = '\0';
    last = strrchr(parent, '/');
    if (last && last != parent) {
        *last = '\0';
    } else {
        strncpy(parent, home_path, parent_size - 1);
        parent[parent_size - 1] = '\0';
    }
    return 0;
}

static int validate_and_change_directory(struct ftp_server_s *server,
    struct client_state_t *cstate, const char *parent, char *real)
{
    if (!realpath(parent, real)) {
        my_send(cstate->fd, REPLY_550, strlen(REPLY_550), 0);
        return -1;
    }
    if (strncmp(real, server->home_path, strlen(server->home_path)) != 0) {
        my_send(cstate->fd, REPLY_550_FAILED_CHANGE_HOME,
            strlen(REPLY_550_FAILED_CHANGE_HOME), 0);
        return -1;
    }
    if (chdir(real) != 0) {
        my_send(cstate->fd, REPLY_550_FAILED_CHANGE_HOME,
            strlen(REPLY_550_FAILED_CHANGE_HOME), 0);
        return -1;
    }
    return 0;
}

static void handle_cdup_directory_change(struct ftp_server_s *server,
    struct client_state_t *cstate)
{
    char parent[PATH_MAX];
    char real[PATH_MAX];

    get_parent_directory(cstate->real_cwd, server->home_path, parent,
        sizeof(parent));
    if (validate_and_change_directory(server, cstate, parent, real) == 0) {
        update_client_cwd(server, cstate, real);
        my_send(cstate->fd, REPLY_200, strlen(REPLY_200), 0);
    }
}

void ftp_cmd_cdup(struct ftp_server_s *server, struct client_state_t *cstate,
    [[maybe_unused]] char *buffer)
{
    if (!cstate || !cstate->logged_in) {
        my_send(cstate ? cstate->fd : -1, REPLY_530, strlen(REPLY_530), 0);
        return;
    }
    handle_cdup_directory_change(server, cstate);
}
