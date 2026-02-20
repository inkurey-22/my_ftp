/*
** EPITECH PROJECT, 2026
** my_ftp
** File description:
** ftp_replies
*/

// clang-format off
#ifndef FTP_REPLIES_H
    #define FTP_REPLIES_H
// clang-format on
// FTP Reply Messages (RFC 959)

// 1xx: Positive Preliminary reply
extern const char *const REPLY_120;
extern const char *const REPLY_125;
extern const char *const REPLY_150;

// 2xx: Positive Completion reply
extern const char *const REPLY_200;
extern const char *const REPLY_214;
extern const char *const REPLY_220;
extern const char *const REPLY_221;
extern const char *const REPLY_226;
extern const char *const REPLY_227;
extern const char *const REPLY_230;
extern const char *const REPLY_250;
extern const char *const REPLY_257;

// 3xx: Positive Intermediate reply
extern const char *const REPLY_331;
extern const char *const REPLY_332;

// 4xx: Transient Negative Completion reply (custom/extended)
extern const char *const REPLY_425_USE_PASV;
extern const char *const REPLY_425_CANT_OPEN_DATA;
extern const char *const REPLY_426_TRANSFER_ABORTED;

// 5xx: Error replies (RFC compliant, generic and custom)
extern const char *const REPLY_500;
extern const char *const REPLY_501;
extern const char *const REPLY_501_MISSING_FILE_PATH;
extern const char *const REPLY_530;
extern const char *const REPLY_530_INVALID_USERNAME;
extern const char *const REPLY_550;
extern const char *const REPLY_550_FAILED_CHANGE_HOME;
extern const char *const REPLY_550_FAILED_CHANGE_DIR;
extern const char *const REPLY_550_FAILED_OPEN_FILE;

#endif// FTP_REPLIES_H
