use std::io::{self, Write};

pub fn prompt(message: &str) -> String {
    print!("{}", message);
    io::stdout().flush().unwrap();
    let mut input = String::new();
    let n = io::stdin().read_line(&mut input);
    match n {
        Ok(0) => "exit".to_string(),
        Ok(_) => input.trim().to_string(),
        Err(_) => {
            println!("\nInput error. Exiting.");
            std::process::exit(1);
        }
    }
}
