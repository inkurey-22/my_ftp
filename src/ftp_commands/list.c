/*
** EPITECH PROJECT, 2026
** my_ftp
** File description:
** LIST command implementation
*/

#include "commands.h"
#include "ftp_replies.h"
#include "list_helpers.h"
#include <string.h>
#include <unistd.h>

static void handle_ftp_list(struct ftp_server_s *server,
    struct client_state_t *cstate, char *buffer)
{
    int data_fd;
    char *arg = NULL;
    const char *target;
    size_t home_len;

    data_fd = prepare_data_connection(cstate);
    if (data_fd < 0)
        return;
    arg = parse_list_arg(buffer);
    target = arg ? arg : cstate->real_cwd;
    home_len = strlen(server->home_path);
    if (strncmp(target, server->home_path, home_len) != 0) {
        my_send(cstate->fd, REPLY_550_FAILED_OPEN_FILE,
            strlen(REPLY_550_FAILED_OPEN_FILE), 0);
        close(data_fd);
        return;
    }
    handle_list_logic(data_fd, target, arg);
    close(data_fd);
    my_send(cstate->fd, REPLY_226, strlen(REPLY_226), 0);
}

void ftp_cmd_list(struct ftp_server_s *server, struct client_state_t *cstate,
    char *buffer)
{
    if (check_logged_in_and_data_conn(cstate) < 0)
        return;
    handle_ftp_list(server, cstate, buffer);
}
