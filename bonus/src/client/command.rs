use super::mode::FtpMode;
use super::prompt::prompt;
use crate::TcpStream;
use std::io::BufRead;

pub fn command_loop(mut reader: impl BufRead, mut writer: TcpStream, mut mode: FtpMode) {
    let mut line = String::new();

    loop {
        let cmd = prompt("ftp> ");
        if cmd.is_empty() {
            super::simple_cmds::handle_quit(&mut reader, &mut writer, &mut line);
            break;
        }
        let args: Vec<&str> = cmd.trim().split_whitespace().collect();
        if args.is_empty() {
            continue;
        }
        match args[0].to_lowercase().as_str() {
            "active" => {
                mode = FtpMode::Active;
                println!("Switched to ACTIVE mode.");
            }
            "passive" => {
                mode = FtpMode::Passive;
                println!("Switched to PASSIVE mode.");
            }
            "quit" | "exit" => {
                super::simple_cmds::handle_quit(&mut reader, &mut writer, &mut line);
                break;
            }
            "put" => {
                super::data_cmds::handle_put_mode(&mut reader, &mut writer, &mut line, args, &mode);
            }
            "get" => {
                super::data_cmds::handle_get_mode(&mut reader, &mut writer, &mut line, args, &mode);
            }
            "ls" => {
                super::data_cmds::handle_ls_mode(&mut reader, &mut writer, &mut line, args, &mode);
            }
            "cd" => {
                super::simple_cmds::handle_cd(&mut reader, &mut writer, &mut line, args);
            }
            "cdup" => {
                super::simple_cmds::handle_cdup(&mut reader, &mut writer, &mut line);
            }
            "pwd" => {
                super::simple_cmds::handle_pwd(&mut reader, &mut writer, &mut line);
            }
            "rm" => {
                super::simple_cmds::handle_dele(&mut reader, &mut writer, &mut line, args);
            }
            _ => {
                super::simple_cmds::handle_unknown(&mut reader, &mut writer, &mut line, &cmd);
            }
        }
    }
}
