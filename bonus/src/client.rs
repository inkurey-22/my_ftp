pub mod active;
pub mod command;
pub mod data_cmds;
pub mod login;
pub mod mode;
pub mod prompt;
pub mod simple_cmds;
pub mod util;

use command::command_loop;
use login::login;
use mode::FtpMode;
use std::io::{BufRead, BufReader};
use std::net::TcpStream;

pub fn start_client(stream: TcpStream, mode: FtpMode) {
    let mut reader = BufReader::new(stream.try_clone().expect("Failed to clone stream"));
    let mut line = String::new();
    let writer = stream;

    if let Err(e) = reader.read_line(&mut line) {
        eprintln!("Failed to read from server: {}", e);
        return;
    }
    println!("Server: {}", line.trim());
    if !line.starts_with("220") {
        eprintln!("Unexpected server response: {}", line.trim());
        return;
    }

    match login(reader, writer) {
        Some((reader, writer)) => {
            command_loop(reader, writer, mode);
        }
        None => {
            println!("Login failed.");
        }
    }
}
