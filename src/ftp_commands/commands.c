/*
** EPITECH PROJECT, 2026
** my_ftp
** File description:
** FTP_COMMANDS
*/

#include "commands.h"

const ftp_command_entry_t FTP_COMMANDS[] = {
    {"USER", ftp_cmd_user},
    {"PASS", ftp_cmd_pass},
    {"QUIT", ftp_cmd_quit},
    {"PWD", ftp_cmd_pwd},
    {"RETR", ftp_cmd_retr},
    {"STOR", ftp_cmd_stor},
    {"PASV", ftp_cmd_pasv},
    {"PORT", ftp_cmd_port},
};

const int FTP_COMMANDS_COUNT =
    sizeof(FTP_COMMANDS) / sizeof(ftp_command_entry_t);
