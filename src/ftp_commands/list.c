/*
** EPITECH PROJECT, 2026
** my_ftp
** File description:
** LIST command implementation
*/

#include "commands.h"
#include "ftp_replies.h"
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

static int check_data_connection(struct client_state_t *cstate)
{
    if (!cstate)
        return -1;
    if (cstate->active_mode && cstate->active_addr.sin_port != 0)
        return 0;
    if (cstate->data_fd > 0)
        return 0;
    return -1;
}

static int open_data_connection(struct client_state_t *cstate)
{
    int data_fd = -1;

    if (cstate->active_mode) {
        data_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (data_fd < 0)
            return -1;
        if (connect(data_fd, (struct sockaddr *) &cstate->active_addr,
                sizeof(cstate->active_addr))
            < 0) {
            close(data_fd);
            return -1;
        }
    } else if (cstate->data_fd > 0) {
        data_fd = cstate->data_fd;
        cstate->data_fd = -1;
    }
    return data_fd;
}

static void send_directory_listing(int data_fd)
{
    DIR *dir;
    struct dirent *entry;
    char line[1024];
    struct stat st;

    dir = opendir(".");
    if (!dir) {
        return;
    }
    entry = readdir(dir);
    while (entry != NULL) {
        if (stat(entry->d_name, &st) == 0) {
            snprintf(line, sizeof(line), "%c%s %5ld %s\r\n",
                S_ISDIR(st.st_mode) ? 'd' : '-', entry->d_name,
                (long) st.st_size, "");
            send(data_fd, line, strlen(line), 0);
        }
        entry = readdir(dir);
    }
    closedir(dir);
}

static int try_open_directory_and_respond(int client_fd, int data_fd)
{
    DIR *dir;

    dir = opendir(".");
    if (!dir) {
        my_send(client_fd, REPLY_550, strlen(REPLY_550), 0);
        if (data_fd >= 0)
            close(data_fd);
        return -1;
    }
    closedir(dir);
    return 0;
}

static int check_logged_in_and_data_conn(struct client_state_t *cstate)
{
    if (!cstate || !cstate->logged_in) {
        my_send(cstate ? cstate->fd : -1, REPLY_530, strlen(REPLY_530), 0);
        return -1;
    }
    if (check_data_connection(cstate) < 0) {
        my_send(cstate->fd, REPLY_425_USE_PASV, strlen(REPLY_425_USE_PASV), 0);
        return -1;
    }
    return 0;
}

static int prepare_data_connection(struct client_state_t *cstate)
{
    int data_fd;

    my_send(cstate->fd, REPLY_150, strlen(REPLY_150), 0);
    data_fd = open_data_connection(cstate);
    if (data_fd < 0) {
        my_send(cstate->fd, REPLY_425_CANT_OPEN_DATA,
            strlen(REPLY_425_CANT_OPEN_DATA), 0);
        return -1;
    }
    return data_fd;
}

void ftp_cmd_list([[maybe_unused]] struct ftp_server_s *server,
    struct client_state_t *cstate, [[maybe_unused]] char *buffer)
{
    int data_fd;

    if (check_logged_in_and_data_conn(cstate) < 0)
        return;
    data_fd = prepare_data_connection(cstate);
    if (data_fd < 0)
        return;
    if (try_open_directory_and_respond(cstate->fd, data_fd) < 0)
        return;
    send_directory_listing(data_fd);
    close(data_fd);
    my_send(cstate->fd, REPLY_226, strlen(REPLY_226), 0);
}
