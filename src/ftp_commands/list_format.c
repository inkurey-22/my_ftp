/*
** EPITECH PROJECT, 2026
** my_ftp
** File description:
** list_format
*/

#include "list_helpers.h"
#include <grp.h>
#include <pwd.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

void get_permissions(mode_t mode, char *perms)
{
    perms[0] = S_ISDIR(mode) ? 'd' : '-';
    perms[1] = (mode & S_IRUSR) ? 'r' : '-';
    perms[2] = (mode & S_IWUSR) ? 'w' : '-';
    perms[3] = (mode & S_IXUSR) ? 'x' : '-';
    perms[4] = (mode & S_IRGRP) ? 'r' : '-';
    perms[5] = (mode & S_IWGRP) ? 'w' : '-';
    perms[6] = (mode & S_IXGRP) ? 'x' : '-';
    perms[7] = (mode & S_IROTH) ? 'r' : '-';
    perms[8] = (mode & S_IWOTH) ? 'w' : '-';
    perms[9] = (mode & S_IXOTH) ? 'x' : '-';
    perms[10] = '\0';
}

void get_time_str(time_t mtime, char *buf, size_t buflen)
{
    struct tm *tm_info = localtime(&mtime);

    strftime(buf, buflen, "%b %e %H:%M", tm_info);
}

void format_ls_line(struct stat *st, const char *name, char *line, size_t size)
{
    char perms[11] = {0};
    char timebuf[64] = {0};
    struct passwd *pw = getpwuid(st->st_uid);
    struct group *gr = getgrgid(st->st_gid);

    get_permissions(st->st_mode, perms);
    get_time_str(st->st_mtime, timebuf, sizeof(timebuf));
    snprintf(line, size, "%s %3lu %-8s %-8s %8ld %s %s\r\n", perms,
        (unsigned long) st->st_nlink, pw ? pw->pw_name : "?",
        gr ? gr->gr_name : "?", (long) st->st_size, timebuf, name);
}

void send_file_listing(int data_fd, const char *path, const char *name)
{
    struct stat st;
    char line[1024];

    if (stat(path, &st) == 0) {
        format_ls_line(&st, name ? name : path, line, sizeof(line));
        send(data_fd, line, strlen(line), 0);
    }
}

void send_directory_listing(int data_fd, const char *dirpath)
{
    DIR *dir = opendir(dirpath);
    struct dirent *entry;
    char fullpath[PATH_MAX];

    if (!dir)
        return;
    entry = readdir(dir);
    while (entry != NULL) {
        snprintf(fullpath, sizeof(fullpath), "%s/%s", dirpath, entry->d_name);
        send_file_listing(data_fd, fullpath, entry->d_name);
        entry = readdir(dir);
    }
    closedir(dir);
}
