/*
** EPITECH PROJECT, 2026
** my_ftp
** File description:
** ftp
*/

// clang-format off
#ifndef FTP_H_
    #define FTP_H_

    #define MAX_CLIENTS 64
    #include <stdint.h>
    #include <sys/types.h>
// clang-format on

struct ftp_server_s {
    uint16_t port;
    int fd;
    char *home_path;
};

int8_t launch_server(struct ftp_server_s *server);
void handle_client_session(struct ftp_server_s *server, int client_fd);
int run_server_poll_loop(struct ftp_server_s *server);
ssize_t my_send(int sockfd, const void *buf, size_t len, int flags);

#endif /* !FTP_H_ */
