# my_ftp

## Overview
my_ftp is a custom FTP server and client implementation supporting standard FTP commands, user authentication, and file transfer operations. The project includes a server written in C, mandatory for the school project, and a Rust client as bonus.
Server goal is to comply with RFC959 (yes it is VERY old).

## Features
- FTP server with support for commands: USER, PASS, LIST, RETR, STOR, CWD, CDUP, PWD, QUIT, PASV, PORT
- User authentication via `userdb.txt`
- Passive and active data transfer modes
- Bonus Rust client implementation

## Directory Structure
```
my_ftp/
├── CMakeLists.txt         # CMake build configuration
├── Makefile               # Make build configuration (mandatory because school project)
├── README.md              # Project documentation
├── userdb.txt             # User database for authentication
├── src/                   # Main C source files
│   ├── ...                # FTP server logic and command handlers
│   ├── ftp_commands/      # FTP command implementations
│   └── utilities/         # Utility functions
├── include/               # Header files
├── ftp_commands/          # Additional command sources
├── example/               # Example server implementation (C)
└── bonus/                 # Bonus Rust client implementation
    ├── Cargo.toml         # Rust package config
    └── src/               # Rust source files
```

## Building
### C Server
1. **Using CMake:**
   ```sh
   mkdir build && cd build
   cmake ..
   make
   ```
2. **Using Makefile:**
   ```sh
   make
   ```

### Rust Client
1. Navigate to `bonus/`:
   ```sh
   cd bonus
   cargo build --release
   ```

## Running
### C Server
- After building, run the server:
  ```sh
  ./build/myftp <port> <home-directory>
  ```

### Rust Client
- After building, run the client:
  ```sh
  ./target/release/bonus <ip/hostname> [port]
  ```
- Default port is 21 if not specified
- Supports put, get, ls, cd, cdup.

## Usage
- Connect to the FTP server using the Rust client or telnet/netcat. Other clients won't work because subject requires the use of RETR/STOR from RFC959. These commands aren't standard for GNU ftp, FileZilla or other clients. 
- Authenticate using credentials from `userdb.txt`. (Anonymous has empty password).
- Use FTP commands to interact with the server (see supported commands above).

## License
This project is licensed under EUPL 1.2. See LICENSE for details. 

## Authors
- Myself, Théophile R.

## Contact
For questions or contributions, please open an issue or pull request.
