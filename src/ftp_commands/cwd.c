/*
** EPITECH PROJECT, 2026
** my_ftp
** File description:
** ftp_cmd_cwd
*/

#include "commands.h"
#include "ftp_replies.h"
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

static int check_logged_in(struct client_state_t *cstate)
{
    if (!cstate || !cstate->logged_in) {
        my_send(cstate ? cstate->fd : -1, reply_530, strlen(reply_530), 0);
        return 0;
    }
    return 1;
}

static char *parse_path(char *buffer)
{
    char *path = buffer;
    char *end;

    if (strncasecmp(path, "CWD ", 4) == 0) {
        path += 4;
    }
    while (*path == ' ')
        path++;
    end = path + strlen(path) - 1;
    while (end > path
        && (*end == ' ' || *end == '\n' || *end == '\r' || *end == '\t')) {
        *end = '\0';
        end--;
    }
    return path;
}

static int validate_path(struct client_state_t *cstate, const char *path)
{
    if (*path == '\0') {
        my_send(cstate->fd, reply_501, strlen(reply_501), 0);
        return 0;
    }
    return 1;
}

static int is_access_allowed(const char *newcwd, const char *home_path)
{
    return newcwd && strncmp(newcwd, home_path, strlen(home_path)) == 0;
}

// clang-format off
static int resolve_path(struct ftp_server_s *server,
    struct client_state_t *cstate, char *path, resolved_path_t *out)
{
    if (path[0] == '/') {
        return snprintf(out->resolved, sizeof(out->resolved), "%s%s",
            server->home_path, path)
            < (int) sizeof(out->resolved);
    }
    return snprintf(out->resolved, sizeof(out->resolved), "%s/%s",
        cstate->real_cwd, path)
        < (int) sizeof(out->resolved);
}
// clang-format on

static int canonicalize_and_validate(const char *resolved, char *real,
    struct ftp_server_s *server)
{
    struct stat st;

    if (!realpath(resolved, real))
        return 0;
    if (stat(real, &st) == 0 && S_ISDIR(st.st_mode)
        && is_access_allowed(real, server->home_path))
        return 1;
    return 0;
}

void update_client_cwd(struct ftp_server_s *server,
    struct client_state_t *cstate, const char *real)
{
    size_t home_len;
    const char *sub;

    strncpy(cstate->real_cwd, real, sizeof(cstate->real_cwd) - 1);
    cstate->real_cwd[sizeof(cstate->real_cwd) - 1] = '\0';
    home_len = strlen(server->home_path);
    if (strncmp(real, server->home_path, home_len) == 0) {
        sub = real + home_len;
        if (*sub == '\0') {
            strcpy(cstate->virt_cwd, "/");
        } else {
            snprintf(cstate->virt_cwd, sizeof(cstate->virt_cwd), "/%s",
                sub[0] == '/' ? sub + 1 : sub);
        }
    } else {
        strcpy(cstate->virt_cwd, "/");
    }
}

static void try_change_dir(struct ftp_server_s *server,
    struct client_state_t *cstate, char *path)
{
    resolved_path_t resolved;
    char real[PATH_MAX];

    if (!resolve_path(server, cstate, path, &resolved)) {
        my_send(cstate->fd, reply_550_failed_change_dir,
            strlen(reply_550_failed_change_dir), 0);
        return;
    }
    if (canonicalize_and_validate(resolved.resolved, real, server)) {
        update_client_cwd(server, cstate, real);
        my_send(cstate->fd, reply_250, strlen(reply_250), 0);
        return;
    }
    my_send(cstate->fd, reply_550_failed_change_dir,
        strlen(reply_550_failed_change_dir), 0);
}

void ftp_cmd_cwd(struct ftp_server_s *server, struct client_state_t *cstate,
    char *buffer)
{
    char *path;

    if (!check_logged_in(cstate))
        return;
    path = parse_path(buffer);
    if (!validate_path(cstate, path))
        return;
    try_change_dir(server, cstate, path);
}
