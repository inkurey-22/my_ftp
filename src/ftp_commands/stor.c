/*
** EPITECH PROJECT, 2026
** my_ftp
** File description:
** stor (upload file from client to server)
*/

#include "client_state.h"
#include "ftp.h"
#include "ftp_replies.h"
#include <fcntl.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>

static char *parse_filepath(char *buffer)
{
    char *filepath = buffer;
    char *end;

    if (strncasecmp(filepath, "STOR ", 5) == 0)
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

static void send_transfer_error_and_close(int file_fd, int data_conn_fd,
    struct client_state_t *cstate)
{
    my_send(cstate->fd, REPLY_426_TRANSFER_ABORTED,
        strlen(REPLY_426_TRANSFER_ABORTED), 0);
    close(file_fd);
    close(data_conn_fd);
}

static void send_transfer_success(struct client_state_t *cstate)
{
    my_send(cstate->fd, REPLY_226, strlen(REPLY_226), 0);
}

static void send_transfer_failure(struct client_state_t *cstate)
{
    my_send(cstate->fd, REPLY_426_TRANSFER_ABORTED,
        strlen(REPLY_426_TRANSFER_ABORTED), 0);
}

static void handle_file_receive(int file_fd, int data_conn_fd,
    struct client_state_t *cstate)
{
    ssize_t bytes_read;
    char buf[4096];

    bytes_read = read(data_conn_fd, buf, sizeof(buf));
    while (bytes_read > 0) {
        if (write(file_fd, buf, bytes_read) != bytes_read) {
            send_transfer_error_and_close(file_fd, data_conn_fd, cstate);
            return;
        }
        bytes_read = read(data_conn_fd, buf, sizeof(buf));
    }
    close(file_fd);
    close(data_conn_fd);
    if (bytes_read < 0)
        send_transfer_failure(cstate);
    else
        send_transfer_success(cstate);
}

static int check_data_connection(struct client_state_t *cstate)
{
    if (cstate->data_fd < 0) {
        my_send(cstate->fd, REPLY_425_USE_PASV, strlen(REPLY_425_USE_PASV), 0);
        return 0;
    }
    return 1;
}

static char *get_and_validate_filepath(struct client_state_t *cstate,
    char *buffer)
{
    char *filepath = parse_filepath(buffer);

    if (!filepath) {
        my_send(cstate->fd, REPLY_501_MISSING_FILE_PATH,
            strlen(REPLY_501_MISSING_FILE_PATH), 0);
        return NULL;
    }
    return filepath;
}

static int open_file_for_stor(struct client_state_t *cstate,
    const char *filepath)
{
    int file_fd = open(filepath, O_WRONLY | O_CREAT | O_TRUNC, 0666);

    if (file_fd < 0) {
        close(cstate->data_fd);
        cstate->data_fd = -1;
        my_send(cstate->fd, REPLY_550_FAILED_OPEN_FILE,
            strlen(REPLY_550_FAILED_OPEN_FILE), 0);
        return -1;
    }
    return file_fd;
}

static int accept_data_connection(struct client_state_t *cstate, int file_fd)
{
    int data_conn_fd;

    my_send(cstate->fd, REPLY_150, strlen(REPLY_150), 0);
    data_conn_fd = accept(cstate->data_fd, NULL, NULL);
    close(cstate->data_fd);
    cstate->data_fd = -1;
    if (data_conn_fd < 0) {
        close(file_fd);
        my_send(cstate->fd, REPLY_425_CANT_OPEN_DATA,
            strlen(REPLY_425_CANT_OPEN_DATA), 0);
        return -1;
    }
    return data_conn_fd;
}

void ftp_cmd_stor([[maybe_unused]] struct ftp_server_s *server,
    struct client_state_t *cstate, char *buffer)
{
    char *filepath;
    int file_fd = -1;
    int data_conn_fd = -1;

    if (!check_data_connection(cstate))
        return;
    filepath = get_and_validate_filepath(cstate, buffer);
    if (!filepath)
        return;
    file_fd = open_file_for_stor(cstate, filepath);
    if (file_fd < 0)
        return;
    data_conn_fd = accept_data_connection(cstate, file_fd);
    if (data_conn_fd < 0)
        return;
    handle_file_receive(file_fd, data_conn_fd, cstate);
}
