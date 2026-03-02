use std::io::{BufRead, Write};
use std::net::TcpStream;

pub fn enter_pasv(
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

pub fn send_and_read(
    reader: &mut impl BufRead,
    writer: &mut TcpStream,
    line: &mut String,
    cmd: &str,
) {
    let cmd_with_crlf = format!("{}\r\n", cmd.trim());
    writer.write_all(cmd_with_crlf.as_bytes()).unwrap();
    writer.flush().unwrap();
    line.clear();
    reader.read_line(line).unwrap();
    println!("Server: {}", line.trim());
}

pub fn parse_pasv(line: &str) -> Option<String> {
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
