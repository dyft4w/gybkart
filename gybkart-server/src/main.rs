pub mod game;
pub mod util;

use std::net::TcpListener;
use crate::game::game::GameInstance;

fn main() {
    let listener = TcpListener::bind("127.0.0.1:59692");
    
    let game = GameInstance::new();

    for stream in listener.incoming(){
        println!("got something (real)");
        let mut stream=stream.unwrap();
        game.handle_event(&mut stream);
    }
}
