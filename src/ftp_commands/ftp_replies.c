/*
** EPITECH PROJECT, 2026
** my_ftp
** File description:
** ftp_replies
*/

// Definitions for FTP reply messages
#include "ftp_replies.h"

// 1xx: Positive Preliminary reply
const char *const REPLY_120 = "120 Service ready in nnn minutes.\r\n";
const char *const REPLY_125 =
    "125 Data connection already open; transfer starting.\r\n";
const char *const REPLY_150 =
    "150 File status okay; about to open data connection.\r\n";

// 2xx: Positive Completion reply
const char *const REPLY_200 = "200 Command okay.\r\n";
const char *const REPLY_214 = "214 Help message.\r\n";
const char *const REPLY_220 = "220 Service ready for new user.\r\n";
const char *const REPLY_221 = "221 Service closing control connection.\r\n";
const char *const REPLY_226 = "226 Closing data connection.\r\n";
const char *const REPLY_227 =
    "227 Entering Passive Mode (h1,h2,h3,h4,p1,p2).\r\n";
const char *const REPLY_230 = "230 User logged in, proceed.\r\n";
const char *const REPLY_250 = "250 Requested file action okay, completed.\r\n";
const char *const REPLY_257 = "257 \"PATHNAME\" created.\r\n";

// 3xx: Positive Intermediate reply
const char *const REPLY_331 = "331 User name okay, need password.\r\n";
const char *const REPLY_332 = "332 Need account for login.\r\n";

// 4xx: Transient Negative Completion reply (custom/extended)
const char *const REPLY_425_USE_PASV = "425 Use PASV first.\r\n";
const char *const REPLY_425_CANT_OPEN_DATA =
    "425 Can't open data connection.\r\n";
const char *const REPLY_426_TRANSFER_ABORTED =
    "426 Connection closed; transfer aborted.\r\n";

// 5xx: Error replies (RFC compliant, generic and custom)
const char *const REPLY_500 = "500 Syntax error, command unrecognized.\r\n";
const char *const REPLY_501 =
    "501 Syntax error in parameters or arguments.\r\n";
const char *const REPLY_501_MISSING_FILE_PATH = "501 Missing file path.\r\n";
const char *const REPLY_530 = "530 Not logged in.\r\n";
const char *const REPLY_530_INVALID_USERNAME = "530 Invalid username.\r\n";
const char *const REPLY_550 = "550 Requested action not taken.\r\n";
const char *const REPLY_550_FAILED_CHANGE_HOME =
    "550 Failed to change to home directory.\r\n";
const char *const REPLY_550_FAILED_CHANGE_DIR =
    "550 Failed to change directory.\r\n";
const char *const REPLY_550_FAILED_OPEN_FILE = "550 Failed to open file.\r\n";
