/*
** EPITECH PROJECT, 2026
** my_ftp
** File description:
** commands
*/

// clang-format off
#ifndef FTP_COMMANDS_H
    #define FTP_COMMANDS_H
// clang-format on

struct ftp_server_s;
typedef void (*ftp_command_func_t)(struct ftp_server_s *, int, char *, int *);

typedef struct {
    const char *name;
    ftp_command_func_t func;
} ftp_command_entry_t;

void ftp_cmd_user(struct ftp_server_s *server, int client_fd, char *buffer,
    int *logged_in);
void ftp_cmd_pass(struct ftp_server_s *server, int client_fd, char *buffer,
    int *logged_in);
void ftp_cmd_quit(struct ftp_server_s *server, int client_fd, char *buffer,
    int *logged_in);
void ftp_cmd_pwd(struct ftp_server_s *server, int client_fd, char *buffer,
    int *logged_in);

extern const ftp_command_entry_t FTP_COMMANDS[];
extern const int FTP_COMMANDS_COUNT;

#endif// FTP_COMMANDS_H
