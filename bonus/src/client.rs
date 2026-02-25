pub mod command;
pub mod login;
pub mod prompt;

use command::command_loop;
use login::login;
use std::io::{BufRead, BufReader};
use std::net::TcpStream;

pub fn start_client(stream: TcpStream) {
    let mut reader = BufReader::new(stream.try_clone().expect("Failed to clone stream"));
    let mut line = String::new();
    let writer = stream;

    // Read the initial 220 response from the server
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
            command_loop(reader, writer);
        }
        None => {
            println!("Login failed.");
        }
    }
}
