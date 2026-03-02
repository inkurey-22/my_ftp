use super::mode::FtpMode;
use super::util::{enter_pasv, send_and_read};
use crate::client::active::setup_active_port_command;
use std::io::BufRead;
use std::net::TcpStream;

pub fn handle_put_mode(
    reader: &mut impl BufRead,
    writer: &mut TcpStream,
    line: &mut String,
    args: Vec<&str>,
    mode: &FtpMode,
) {
    if args.len() < 2 {
        println!("Usage: put <filename>");
        return;
    }
    let filename = args[1];
    match mode {
        FtpMode::Passive => {
            let pasv_addr = match enter_pasv(reader, writer, line) {
                Some(addr) => addr,
                None => {
                    println!("Failed to enter PASV mode.");
                    return;
                }
            };
            let stor_cmd = format!("STOR {}", filename);
            send_and_read(reader, writer, line, &stor_cmd);
            if let Ok(mut data_stream) = TcpStream::connect(pasv_addr) {
                if let Ok(mut file) = std::fs::File::open(filename) {
                    std::io::copy(&mut file, &mut data_stream).unwrap();
                    println!("File sent.");
                } else {
                    println!("Failed to open file.");
                }
            } else {
                println!("Failed to connect to data port.");
            }
            line.clear();
            reader.read_line(line).unwrap();
            println!("Server: {}", line.trim());
        }
        FtpMode::Active => {
            let (listener, port_cmd) = match setup_active_port_command() {
                Some(res) => res,
                None => {
                    println!("Failed to set up active data port.");
                    return;
                }
            };
            send_and_read(reader, writer, line, &port_cmd);
            let stor_cmd = format!("STOR {}", filename);
            send_and_read(reader, writer, line, &stor_cmd);
            match listener.accept() {
                Ok((mut data_stream, _)) => {
                    if let Ok(mut file) = std::fs::File::open(filename) {
                        std::io::copy(&mut file, &mut data_stream).unwrap();
                        println!("File sent.");
                    } else {
                        println!("Failed to open file.");
                    }
                }
                Err(e) => {
                    println!("Failed to accept data connection: {}", e);
                }
            }
            line.clear();
            reader.read_line(line).unwrap();
            println!("Server: {}", line.trim());
        }
    }
}

pub fn handle_get_mode(
    reader: &mut impl BufRead,
    writer: &mut TcpStream,
    line: &mut String,
    args: Vec<&str>,
    mode: &FtpMode,
) {
    if args.len() < 2 {
        println!("Usage: get <filename>");
        return;
    }
    let filename = args[1];
    match mode {
        FtpMode::Passive => {
            let pasv_addr = match enter_pasv(reader, writer, line) {
                Some(addr) => addr,
                None => {
                    println!("Failed to enter PASV mode.");
                    return;
                }
            };
            let retr_cmd = format!("RETR {}", filename);
            send_and_read(reader, writer, line, &retr_cmd);
            if let Ok(mut data_stream) = TcpStream::connect(pasv_addr) {
                if let Ok(mut file) = std::fs::File::create(filename) {
                    std::io::copy(&mut data_stream, &mut file).unwrap();
                    println!("File received.");
                } else {
                    println!("Failed to create file.");
                }
            } else {
                println!("Failed to connect to data port.");
            }
            line.clear();
            reader.read_line(line).unwrap();
            println!("Server: {}", line.trim());
        }
        FtpMode::Active => {
            let (listener, port_cmd) = match setup_active_port_command() {
                Some(res) => res,
                None => {
                    println!("Failed to set up active data port.");
                    return;
                }
            };
            send_and_read(reader, writer, line, &port_cmd);
            let retr_cmd = format!("RETR {}", filename);
            send_and_read(reader, writer, line, &retr_cmd);
            match listener.accept() {
                Ok((mut data_stream, _)) => {
                    if let Ok(mut file) = std::fs::File::create(filename) {
                        std::io::copy(&mut data_stream, &mut file).unwrap();
                        println!("File received.");
                    } else {
                        println!("Failed to create file.");
                    }
                }
                Err(e) => {
                    println!("Failed to accept data connection: {}", e);
                }
            }
            line.clear();
            reader.read_line(line).unwrap();
            println!("Server: {}", line.trim());
        }
    }
}

pub fn handle_ls_mode(
    reader: &mut impl BufRead,
    writer: &mut TcpStream,
    line: &mut String,
    args: Vec<&str>,
    mode: &FtpMode,
) {
    match mode {
        FtpMode::Passive => {
            let pasv_addr = match enter_pasv(reader, writer, line) {
                Some(addr) => addr,
                None => {
                    println!("Failed to enter PASV mode.");
                    return;
                }
            };
            let list_cmd = if args.len() > 1 {
                format!("LIST {}", args[1])
            } else {
                "LIST".to_string()
            };
            send_and_read(reader, writer, line, &list_cmd);
            if let Ok(mut data_stream) = TcpStream::connect(pasv_addr) {
                let mut data = String::new();
                std::io::Read::read_to_string(&mut data_stream, &mut data).unwrap();
                println!("{}", data);
            } else {
                println!("Failed to connect to data port.");
            }
            line.clear();
            reader.read_line(line).unwrap();
            println!("Server: {}", line.trim());
        }
        FtpMode::Active => {
            let (listener, port_cmd) = match setup_active_port_command() {
                Some(res) => res,
                None => {
                    println!("Failed to set up active data port.");
                    return;
                }
            };
            send_and_read(reader, writer, line, &port_cmd);
            let list_cmd = if args.len() > 1 {
                format!("LIST {}", args[1])
            } else {
                "LIST".to_string()
            };
            send_and_read(reader, writer, line, &list_cmd);
            match listener.accept() {
                Ok((mut data_stream, _)) => {
                    let mut data = String::new();
                    std::io::Read::read_to_string(&mut data_stream, &mut data).unwrap();
                    println!("{}", data);
                }
                Err(e) => {
                    println!("Failed to accept data connection: {}", e);
                }
            }
            line.clear();
            reader.read_line(line).unwrap();
            println!("Server: {}", line.trim());
        }
    }
}
