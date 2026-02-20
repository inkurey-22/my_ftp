/*
** EPITECH PROJECT, 2026
** my_ftp
** File description:
** my_send
*/

#include "ftp.h"

#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

ssize_t my_send(int sockfd, const void *buf, size_t len,
    [[maybe_unused]] int flags)
{
    size_t total_sent = 0;
    const char *ptr = (const char *) buf;
    ssize_t sent = 0;

    if (len == 0 && buf != NULL) {
        len = strlen(ptr);
    }
    while (total_sent < len) {
        sent = write(sockfd, ptr + total_sent, len - total_sent);
        if (sent < 0) {
            return -1;
        }
        total_sent += sent;
    }
    return (ssize_t) total_sent;
}
