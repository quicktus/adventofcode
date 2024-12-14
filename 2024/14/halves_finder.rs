use regex::Regex;
use std::fs::File;
use std::io::{stdin, stdout, Read, Write};
use std::path::Path;

const ROOM_SIZE: (i32, i32) = (101, 103);

struct Robot {
    position: (i32, i32),
    velocity: (i32, i32),
}

impl Robot {
    fn parse_str(s: &str) -> Self {
        let re =
            Regex::new(r"p=(?P<px>(\d+)),(?P<py>\d+)\s+v=(?P<vx>[-\d]+),(?P<vy>[-\d]+)").unwrap();

        let cs = re.captures(s).unwrap();
        let position = (
            cs["px"].parse::<i32>().unwrap(),
            cs["py"].parse::<i32>().unwrap(),
        );
        let velocity = (
            cs["vx"].parse::<i32>().unwrap(),
            cs["vy"].parse::<i32>().unwrap(),
        );
        Robot { position, velocity }
    }

    fn simulate(self: &mut Self, n_steps: i32) {
        (*self).position.0 += self.velocity.0 * n_steps;
        match (*self).position.0 {
            x if x >= ROOM_SIZE.0 => (*self).position.0 %= ROOM_SIZE.0,
            x if x < 0 => {
                (*self).position.0 =
                    (ROOM_SIZE.0 - ((*self).position.0.abs() % ROOM_SIZE.0)) % ROOM_SIZE.0
            }
            _ => (),
        }

        (*self).position.1 += self.velocity.1 * n_steps;
        match (*self).position.1 {
            x if x >= ROOM_SIZE.1 => (*self).position.1 %= ROOM_SIZE.1,
            x if x < 0 => {
                (*self).position.1 =
                    (ROOM_SIZE.1 - ((*self).position.1.abs() % ROOM_SIZE.1)) % ROOM_SIZE.1
            }
            _ => (),
        }
    }
}

fn main() {
    let path = Path::new("input.txt");

    let mut file = match File::open(path) {
        Err(why) => panic!("couldn't open {}: {}", path.display(), why),
        Ok(file) => file,
    };

    let mut input = String::new();
    match file.read_to_string(&mut input) {
        Err(why) => panic!("couldn't read {}: {}", path.display(), why),
        Ok(_) => (),
    }


    let mut robots: Vec<Robot> = input.lines().map(|s| Robot::parse_str(s)).collect();

    let mut n_steps: i32 = 0;
    let mut halves: Vec<i32>;
    let mut filled_squares: Vec<(i32, i32)>;

    loop {
        n_steps += 1;
        halves = vec![0, 0];
        for r_idx in 0..robots.len() {
            robots[r_idx].simulate(1);
            if robots[r_idx].position.0 == ROOM_SIZE.0 / 2 {
                // robots that are exactly in the middle (horizontally or vertically) don't count towards any half
                continue;
            }
            let h_idx = (robots[r_idx].position.0 / ((ROOM_SIZE.0 + 1) / 2)) as usize;
            halves[h_idx] += 1;
        }

        filled_squares = vec![];
            for r_idx in 0..robots.len() {
                filled_squares.push((robots[r_idx].position.0, robots[r_idx].position.1));
                filled_squares.sort();
                filled_squares.dedup();
            }

        if halves[0] == halves[1] {
            for x in 0..ROOM_SIZE.0 {
                for y in 0..ROOM_SIZE.1 {
                    let s = if filled_squares.binary_search(&(x, y)).is_ok() {"░░"} else {"▓▓"};
                    print!("{s}");
                }
                print!("\n");
            }
            print!("n steps: {n_steps}\n");
            pause();
        }
    }
}

fn pause() {
    let mut stdout = stdout();
    stdout.write(b"Press Enter to continue...").unwrap();
    stdout.flush().unwrap();
    stdin().read(&mut [0]).unwrap();
}
