pub mod util{
    use std::net::TcpStream;

    pub fn convert_to_vec(stream: &TcpStream) -> Vec<String> {
            let to_return: Vec<_> = stream.read_buf  
                .lines()
                .map(|thing| thing.unwrap())
                .take_while(|line| !line.is_empty())
                .collect();
            to_return
        }
}
