use std::net::{IpAddr, TcpListener};

pub fn setup_active_port_command() -> Option<(TcpListener, String)> {
    let listener = TcpListener::bind("0.0.0.0:0").ok()?;
    let local_addr = listener.local_addr().ok()?;
    let ip = get_local_ip().unwrap_or(local_addr.ip());
    let port = local_addr.port();
    let ip_bytes = match ip {
        IpAddr::V4(ipv4) => ipv4.octets(),
        IpAddr::V6(_) => return None,
    };
    let p1 = port / 256;
    let p2 = port % 256;
    let port_cmd = format!(
        "PORT {},{},{},{},{},{}",
        ip_bytes[0], ip_bytes[1], ip_bytes[2], ip_bytes[3], p1, p2
    );
    Some((listener, port_cmd))
}

fn get_local_ip() -> Option<IpAddr> {
    let socket = std::net::UdpSocket::bind("0.0.0.0:0").ok()?;
    socket.connect("8.8.8.8:80").ok()?;
    socket.local_addr().ok().map(|addr| addr.ip())
}
