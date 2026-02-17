/*
** EPITECH PROJECT, 2026
** my_ftp
** File description:
** log
*/

#include <stdarg.h>
#include <stdio.h>

int logger(FILE *file, const char *format, ...)
{
    va_list args;
    int result;

    va_start(args, format);
    result = vfprintf(file, format, args);
    va_end(args);
    return result;
}
