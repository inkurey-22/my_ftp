/*
** EPITECH PROJECT, 2026
** my_ftp
** File description:
** dele
*/

#include "client_state.h"
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

void ftp_cmd_dele([[maybe_unused]] struct ftp_server_s *server,
    struct client_state_t *cstate, char *buffer)
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
    if (unlink(filepath) == 0) {
        my_send(cstate->fd, REPLY_250, strlen(REPLY_250), 0);
    } else {
        my_send(cstate->fd, REPLY_550, strlen(REPLY_550), 0);
    }
}
