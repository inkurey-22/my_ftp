use super::prompt::prompt;
use std::io::{BufRead, Write};
use std::net::TcpStream;

fn enter_pasv(
    reader: &mut impl BufRead,
    writer: &mut TcpStream,
    line: &mut String,
) -> Option<String> {
    writer.write_all(b"PASV\r\n").ok()?;
    writer.flush().ok()?;
    line.clear();
    reader.read_line(line).ok()?;
    parse_pasv(line)
}

fn send_and_read(reader: &mut impl BufRead, writer: &mut TcpStream, line: &mut String, cmd: &str) {
    let cmd_with_crlf = format!("{}\r\n", cmd.trim());
    writer.write_all(cmd_with_crlf.as_bytes()).unwrap();
    writer.flush().unwrap();
    line.clear();
    reader.read_line(line).unwrap();
    println!("Server: {}", line.trim());
}

pub fn command_loop(mut reader: impl BufRead, mut writer: TcpStream) {
    let mut line = String::new();

    loop {
        let cmd = prompt("ftp> ");

        if cmd.is_empty() {
            handle_quit(&mut reader, &mut writer, &mut line);
            break;
        }

        let args: Vec<&str> = cmd.trim().split_whitespace().collect();
        if args.is_empty() {
            continue;
        }

        match args[0].to_lowercase().as_str() {
            "quit" | "exit" => {
                handle_quit(&mut reader, &mut writer, &mut line);
                break;
            }
            "put" => {
                handle_put(&mut reader, &mut writer, &mut line, args);
            }
            "get" => {
                handle_get(&mut reader, &mut writer, &mut line, args);
            }
            "ls" => {
                handle_ls(&mut reader, &mut writer, &mut line, args);
            }
            "cd" => {
                handle_cd(&mut reader, &mut writer, &mut line, args);
            }
            "cdup" => {
                handle_cdup(&mut reader, &mut writer, &mut line);
            }
            "pwd" => {
                handle_pwd(&mut reader, &mut writer, &mut line);
            }
            "rm" => {
                handle_dele(&mut reader, &mut writer, &mut line, args);
            }
            _ => {
                handle_unknown(&mut reader, &mut writer, &mut line, &cmd);
            }
        }
    }
}

fn handle_quit(reader: &mut impl BufRead, writer: &mut TcpStream, line: &mut String) {
    send_and_read(reader, writer, line, "QUIT");
    println!("Goodbye!");
}

fn handle_dele(
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

fn handle_put(
    reader: &mut impl BufRead,
    writer: &mut TcpStream,
    line: &mut String,
    args: Vec<&str>,
) {
    if args.len() < 2 {
        println!("Usage: put <filename>");
        return;
    }

    let filename = args[1];
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

fn handle_get(
    reader: &mut impl BufRead,
    writer: &mut TcpStream,
    line: &mut String,
    args: Vec<&str>,
) {
    if args.len() < 2 {
        println!("Usage: get <filename>");
        return;
    }

    let filename = args[1];
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

fn handle_ls(
    reader: &mut impl BufRead,
    writer: &mut TcpStream,
    line: &mut String,
    args: Vec<&str>,
) {
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

fn handle_cd(
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

fn handle_cdup(reader: &mut impl BufRead, writer: &mut TcpStream, line: &mut String) {
    send_and_read(reader, writer, line, "CDUP");
}

fn handle_pwd(reader: &mut impl BufRead, writer: &mut TcpStream, line: &mut String) {
    send_and_read(reader, writer, line, "PWD");
}

fn handle_unknown(reader: &mut impl BufRead, writer: &mut TcpStream, line: &mut String, cmd: &str) {
    send_and_read(reader, writer, line, cmd);
}

fn parse_pasv(line: &str) -> Option<String> {
    let start = line.find('(')?;
    let end = line.find(')')?;
    let nums: Vec<&str> = line[start + 1..end].split(',').collect();
    if nums.len() != 6 {
        return None;
    }
    let ip = format!("{}.{}.{}.{}", nums[0], nums[1], nums[2], nums[3]);
    let port = nums[4].parse::<u16>().ok()? * 256 + nums[5].parse::<u16>().ok()?;
    Some(format!("{}:{}", ip, port))
}
