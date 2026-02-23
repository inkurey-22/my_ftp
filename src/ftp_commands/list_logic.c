/*
** EPITECH PROJECT, 2026
** my_ftp
** File description:
** list_logic
*/

#include "list_helpers.h"
#include <sys/stat.h>

void handle_list_logic(int data_fd, const char *target, const char *arg)
{
    struct stat st;

    if (stat(target, &st) == 0) {
        if (S_ISDIR(st.st_mode)) {
            send_directory_listing(data_fd, target);
        } else {
            send_file_listing(data_fd, target, arg);
        }
    }
}
