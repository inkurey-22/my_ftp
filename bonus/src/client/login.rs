use std::io::{BufRead, BufReader, Write};
use std::net::TcpStream;
use super::prompt::prompt;

pub fn login(mut reader: BufReader<TcpStream>, mut writer: TcpStream) -> Option<(BufReader<TcpStream>, TcpStream)> {
    let mut line = String::new();
    let username = prompt("Username: ");
    let user_cmd = format!("USER {}\r\n", username.trim());
    writer.write_all(user_cmd.as_bytes()).unwrap();
    writer.flush().unwrap();
    line.clear();
    reader.read_line(&mut line).unwrap();
    println!("Server: {}", line.trim());
    if !line.starts_with("331") && !line.starts_with("230") {
        return None;
    }

    let password = prompt("Password: ");
    let pass_cmd = format!("PASS {}\r\n", password.trim());
    writer.write_all(pass_cmd.as_bytes()).unwrap();
    writer.flush().unwrap();
    line.clear();
    reader.read_line(&mut line).unwrap();
    println!("Server: {}", line.trim());
    if !line.starts_with("230") {
        return None;
    }
    Some((reader, writer))
}
