/*
** EPITECH PROJECT, 2026
** my_ftp
** File description:
** auth
*/

#define _GNU_SOURCE
#include "ftp.h"
#include <openssl/sha.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void sha256_hex(const char *input, char *output)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];

    SHA256((const unsigned char *) input, strlen(input), hash);
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i)
        sprintf(output + (ptrdiff_t) i * 2, "%02x", hash[i]);
    output[64] = '\0';
}

static bool match_user_line(const char *line, const char *username,
    const char *hash)
{
    char fileuser[128] = {0};
    char filehash[65] = {0};

    if (sscanf(line, "%127[^:]:%64s", fileuser, filehash) == 2) {
        if (strcmp(fileuser, username) == 0 && strcmp(filehash, hash) == 0) {
            return true;
        }
    }
    return false;
}

static bool find_user_in_file(FILE *f, const char *username, const char *hash)
{
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    bool found = false;

    read = getline(&line, &len, f);
    while (read != -1) {
        if (match_user_line(line, username, hash)) {
            found = true;
            break;
        }
        read = getline(&line, &len, f);
    }
    free(line);
    return found;
}

bool check_user_password(const char *username, const char *password)
{
    FILE *f;
    char hash[65];
    bool found = false;

    f = fopen(USERDB_PATH, "r");
    if (!f)
        return false;
    sha256_hex(password, hash);
    found = find_user_in_file(f, username, hash);
    fclose(f);
    return found;
}
