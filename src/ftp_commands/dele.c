/*
** EPITECH PROJECT, 2026
** my_ftp
** File description:
** dele
*/

#include "client_state.h"
#include "commands.h"
#include "ftp.h"
#include "ftp_replies.h"
#include <string.h>
#include <unistd.h>

static char *parse_dele_filepath(char *buffer)
{
    char *filepath = buffer;
    char *end;

    if (strncasecmp(filepath, "DELE ", 5) == 0)
        filepath += 5;
    while (*filepath == ' ' || *filepath == '\t')
        ++filepath;
    end = filepath + strlen(filepath) - 1;
    while (end > filepath
        && (*end == ' ' || *end == '\t' || *end == '\n' || *end == '\r')) {
        *end = '\0';
        --end;
    }
    return filepath;
}

static void resolve_dele_path(struct ftp_server_s *server,
    struct client_state_t *cstate, const char *filepath,
    resolved_path_t *resolved)
{
    if (filepath[0] == '/') {
        snprintf(resolved->resolved, sizeof(resolved->resolved), "%s%s",
            server->home_path, filepath);
    } else {
        snprintf(resolved->resolved, sizeof(resolved->resolved), "%s/%s",
            cstate->real_cwd, filepath);
    }
}

static void handle_file_deletion(struct ftp_server_s *server,
    struct client_state_t *cstate, const char *filepath)
{
    resolved_path_t resolved;

    resolve_dele_path(server, cstate, filepath, &resolved);
    if (unlink(resolved.resolved) == 0) {
        my_send(cstate->fd, REPLY_250, strlen(REPLY_250), 0);
    } else {
        my_send(cstate->fd, REPLY_550, strlen(REPLY_550), 0);
    }
}

void ftp_cmd_dele(struct ftp_server_s *server, struct client_state_t *cstate,
    char *buffer)
{
    char *filepath;

    if (cstate->logged_in == 0) {
        my_send(cstate->fd, REPLY_530, strlen(REPLY_530), 0);
        return;
    }
    filepath = parse_dele_filepath(buffer);
    if (*filepath == '\0') {
        my_send(cstate->fd, REPLY_501_MISSING_FILE_PATH,
            strlen(REPLY_501_MISSING_FILE_PATH), 0);
        return;
    }
    if (!server || !cstate) {
        my_send(cstate->fd, REPLY_550, strlen(REPLY_550), 0);
        return;
    }
    handle_file_deletion(server, cstate, filepath);
}
