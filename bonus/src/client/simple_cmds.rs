use std::io::BufRead;
use std::net::TcpStream;
use super::util::send_and_read;

pub fn handle_quit(reader: &mut impl BufRead, writer: &mut TcpStream, line: &mut String) {
    send_and_read(reader, writer, line, "QUIT");
    println!("Goodbye!");
}

pub fn handle_dele(
    reader: &mut impl BufRead,
    writer: &mut TcpStream,
    line: &mut String,
    args: Vec<&str>,
) {
    if args.len() < 2 {
        println!("Usage: rm <filename>");
        return;
    }
    let dele_cmd = format!("DELE {}", args[1]);
    send_and_read(reader, writer, line, &dele_cmd);
}

pub fn handle_cd(
    reader: &mut impl BufRead,
    writer: &mut TcpStream,
    line: &mut String,
    args: Vec<&str>,
) {
    if args.len() < 2 {
        println!("Usage: cd <directory>");
        return;
    }
    let cwd_cmd = format!("CWD {}", args[1]);
    send_and_read(reader, writer, line, &cwd_cmd);
}

pub fn handle_cdup(reader: &mut impl BufRead, writer: &mut TcpStream, line: &mut String) {
    send_and_read(reader, writer, line, "CDUP");
}

pub fn handle_pwd(reader: &mut impl BufRead, writer: &mut TcpStream, line: &mut String) {
    send_and_read(reader, writer, line, "PWD");
}

pub fn handle_unknown(reader: &mut impl BufRead, writer: &mut TcpStream, line: &mut String, cmd: &str) {
    send_and_read(reader, writer, line, cmd);
}
