/*
** EPITECH PROJECT, 2026
** my_ftp
** File description:
** pwd
*/

#include "commands.h"
#include "ftp.h"
#include "ftp_replies.h"

#include <string.h>
#include <unistd.h>

void ftp_cmd_pwd([[maybe_unused]] struct ftp_server_s *server,
    struct client_state_t *cstate, [[maybe_unused]] char *buffer)
{
    if (cstate == NULL || !cstate->logged_in) {
        my_send(cstate ? cstate->fd : -1, REPLY_530, strlen(REPLY_530), 0);
        return;
    }
    my_send(cstate->fd, "257 \"", strlen("257 \""), 0);
    my_send(cstate->fd, cstate->virt_cwd, strlen(cstate->virt_cwd), 0);
    my_send(cstate->fd, "\" created.\r\n", strlen("\" created.\r\n"), 0);
}
