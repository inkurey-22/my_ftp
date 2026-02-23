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

void ftp_cmd_list([[maybe_unused]] struct ftp_server_s *server,
    struct client_state_t *cstate, char *buffer)
{
    int data_fd;
    char *arg = NULL;
    const char *target;

    if (check_logged_in_and_data_conn(cstate) < 0)
        return;
    data_fd = prepare_data_connection(cstate);
    if (data_fd < 0)
        return;
    arg = parse_list_arg(buffer);
    target = arg ? arg : ".";
    handle_list_logic(data_fd, target, arg);
    close(data_fd);
    my_send(cstate->fd, REPLY_226, strlen(REPLY_226), 0);
}
