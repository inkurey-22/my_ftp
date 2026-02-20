/*
** EPITECH PROJECT, 2026
** my_ftp
** File description:
** commands
*/

// clang-format off
#ifndef FTP_COMMANDS_H
    #define FTP_COMMANDS_H
    #define PATH_MAX 4096

    #include "client_state.h"
    #include "ftp.h"

// clang-format on

void update_client_cwd(struct ftp_server_s *server,
    struct client_state_t *cstate, const char *real);

typedef void (*ftp_command_func_t)(struct ftp_server_s *,
    struct client_state_t *, char *);

typedef struct {
    const char *name;
    ftp_command_func_t func;
} ftp_command_entry_t;

typedef struct {
    char resolved[PATH_MAX];
} resolved_path_t;

void ftp_cmd_user(struct ftp_server_s *server, struct client_state_t *cstate,
    char *buffer);
void ftp_cmd_pass(struct ftp_server_s *server, struct client_state_t *cstate,
    char *buffer);
void ftp_cmd_quit(struct ftp_server_s *server, struct client_state_t *cstate,
    char *buffer);
void ftp_cmd_pwd(struct ftp_server_s *server, struct client_state_t *cstate,
    char *buffer);
void ftp_cmd_retr(struct ftp_server_s *server, struct client_state_t *cstate,
    char *buffer);
void ftp_cmd_stor(struct ftp_server_s *server, struct client_state_t *cstate,
    char *buffer);

// void ftp_cmd_putr(struct ftp_server_s *server, struct client_state_t *cstate,
//     char *buffer);
void ftp_cmd_pasv(struct ftp_server_s *server, struct client_state_t *cstate,
    char *buffer);

void ftp_cmd_list(struct ftp_server_s *server, struct client_state_t *cstate,
    char *buffer);
void ftp_cmd_port(struct ftp_server_s *server, struct client_state_t *cstate,
    char *buffer);

void ftp_cmd_cwd(struct ftp_server_s *server, struct client_state_t *cstate,
    char *buffer);
void ftp_cmd_cdup(struct ftp_server_s *server, struct client_state_t *cstate,
    char *buffer);

extern const ftp_command_entry_t FTP_COMMANDS[];
extern const int FTP_COMMANDS_COUNT;

#endif /* FTP_COMMANDS_H */
