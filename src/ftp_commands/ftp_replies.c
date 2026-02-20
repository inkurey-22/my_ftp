/*
** EPITECH PROJECT, 2026
** my_ftp
** File description:
** ftp_replies
*/

// Definitions for FTP reply messages
#include "ftp_replies.h"

// 1xx: Positive Preliminary reply
const char *REPLY_120 = "120 Service ready in nnn minutes.\r\n";
const char *reply_125 =
    "125 Data connection already open; transfer starting.\r\n";
const char *reply_150 =
    "150 File status okay; about to open data connection.\r\n";

// 2xx: Positive Completion reply
const char *reply_200 = "200 Command okay.\r\n";
const char *reply_214 = "214 Help message.\r\n";
const char *reply_220 = "220 Service ready for new user.\r\n";
const char *reply_221 = "221 Service closing control connection.\r\n";
const char *reply_226 = "226 Closing data connection.\r\n";
const char *reply_227 = "227 Entering Passive Mode (h1,h2,h3,h4,p1,p2).\r\n";
const char *reply_230 = "230 User logged in, proceed.\r\n";
const char *reply_250 = "250 Requested file action okay, completed.\r\n";
const char *reply_257 = "257 \"PATHNAME\" created.\r\n";

// 3xx: Positive Intermediate reply
const char *reply_331 = "331 User name okay, need password.\r\n";
const char *reply_332 = "332 Need account for login.\r\n";

// 4xx: Transient Negative Completion reply (custom/extended)
const char *reply_425_use_pasv = "425 Use PASV first.\r\n";
const char *reply_425_cant_open_data = "425 Can't open data connection.\r\n";
const char *reply_426_transfer_aborted =
    "426 Connection closed; transfer aborted.\r\n";

// 5xx: Error replies (RFC compliant, generic and custom)
const char *reply_500 = "500 Syntax error, command unrecognized.\r\n";
const char *reply_501 = "501 Syntax error in parameters or arguments.\r\n";
const char *reply_501_missing_file_path = "501 Missing file path.\r\n";
const char *reply_530 = "530 Not logged in.\r\n";
const char *reply_530_invalid_username = "530 Invalid username.\r\n";
const char *reply_550 = "550 Requested action not taken.\r\n";
const char *reply_550_failed_change_home =
    "550 Failed to change to home directory.\r\n";
const char *reply_550_failed_change_dir =
    "550 Failed to change directory.\r\n";
const char *reply_550_failed_open_file = "550 Failed to open file.\r\n";
