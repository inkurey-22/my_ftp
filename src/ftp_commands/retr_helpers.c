/*
** EPITECH PROJECT, 2026
** my_ftp
** File description:
** retr_helpers
*/

#include "client_state.h"
#include <fcntl.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

static int write_all(int fd, const char *buf, size_t count)
{
    ssize_t written = 0;
    ssize_t chunk;

    while ((size_t) written < count) {
        chunk = write(fd, buf + written, count - written);
        if (chunk <= 0)
            return -1;
        written += chunk;
    }
    return 0;
}

int send_file_over_data_conn(int file_fd, int data_conn_fd)
{
    char buf[4096];
    ssize_t bytes_read;
    ssize_t read_result = 0;

    while (1) {
        read_result = read(file_fd, buf, sizeof(buf));
        if (read_result <= 0)
            break;
        bytes_read = read_result;
        if (write_all(data_conn_fd, buf, bytes_read) < 0)
            return -1;
    }
    if (read_result < 0)
        return -1;
    return 0;
}

char *parse_filepath(char *buffer)
{
    char *filepath = buffer;
    char *end;

    if (strncasecmp(filepath, "RETR ", 5) == 0)
        filepath += 5;
    while (*filepath == ' ' || *filepath == '\t')
        ++filepath;
    end = filepath + strlen(filepath) - 1;
    while (end > filepath
        && (*end == ' ' || *end == '\t' || *end == '\n' || *end == '\r')) {
        *end = '\0';
        --end;
    }
    if (*filepath == '\0')
        return NULL;
    return filepath;
}

int open_file_for_retr(const char *filepath, int *file_fd)
{
    *file_fd = open(filepath, O_RDONLY);
    return *file_fd >= 0 ? 0 : -1;
}

int accept_data_connection(struct client_state_t *cstate, int *data_conn_fd)
{
    *data_conn_fd = accept(cstate->data_fd, NULL, NULL);
    close(cstate->data_fd);
    cstate->data_fd = -1;
    return *data_conn_fd >= 0 ? 0 : -1;
}
