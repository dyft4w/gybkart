
pub mod game{
    use std::time::SystemTime;
    pub struct Player{
        x: f64,
        y: f64,
        rad: f64,
        turn: u8,
        checkpoint:u8
    }
    pub struct GameInstance{
        players: Vec<Player>,
        init_time: SystemTime,
    }
    impl GameInstance {
        pub fn new() -> GameInstance{
            GameInstance{
                players: Vec::new(),
                init_time: SystemTime::now(),
            }
        }
    }
}
