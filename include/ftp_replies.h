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
extern const char *REPLY_120;
extern const char *reply_125;
extern const char *reply_150;

// 2xx: Positive Completion reply
extern const char *reply_200;
extern const char *reply_214;
extern const char *reply_220;
extern const char *reply_221;
extern const char *reply_226;
extern const char *reply_227;
extern const char *reply_230;
extern const char *reply_250;
extern const char *reply_257;

// 3xx: Positive Intermediate reply
extern const char *reply_331;
extern const char *reply_332;

// 4xx: Transient Negative Completion reply (custom/extended)
extern const char *reply_425_use_pasv;
extern const char *reply_425_cant_open_data;
extern const char *reply_426_transfer_aborted;

// 5xx: Error replies (RFC compliant, generic and custom)
extern const char *reply_500;
extern const char *reply_501;
extern const char *reply_501_missing_file_path;
extern const char *reply_530;
extern const char *reply_530_invalid_username;
extern const char *reply_550;
extern const char *reply_550_failed_change_home;
extern const char *reply_550_failed_change_dir;
extern const char *reply_550_failed_open_file;

#endif// FTP_REPLIES_H
