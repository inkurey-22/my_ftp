use std::env;
use std::net::TcpStream;
use std::process;

mod client;
use client::mode::FtpMode;
use client::start_client;

fn print_usage_and_exit(program: &str) -> ! {
    eprintln!("Usage: {} <IP/hostname> [port]", program);
    process::exit(1);
}

fn parse_args() -> (String, u16) {
    let args: Vec<String> = env::args().collect();
    let program = &args[0];
    if args.len() < 2 || args.len() > 3 {
        print_usage_and_exit(program);
    }
    let host = args[1].clone();
    let port: u16 = if args.len() == 3 {
        match args[2].parse() {
            Ok(p) => p,
            Err(_) => {
                eprintln!("Invalid port: {}", args[2]);
                print_usage_and_exit(program);
            }
        }
    } else {
        21
    };
    (host, port)
}

fn connect_to_server(host: &str, port: u16) -> TcpStream {
    let addr = format!("{}:{}", host, port);
    match TcpStream::connect(&addr) {
        Ok(s) => s,
        Err(e) => {
            eprintln!("Failed to connect to {}: {}", addr, e);
            process::exit(1);
        }
    }
}
fn main() {
    let (host, port) = parse_args();
    let stream = connect_to_server(&host, port);
    start_client(stream, FtpMode::Active);
}
