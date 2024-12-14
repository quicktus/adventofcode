use std::fs::File;
use std::io::Read;
use std::path::Path;
use regex::Regex;

const ROOM_SIZE: (i32, i32) = (101, 103);

struct Robot {
    position: (i32, i32),
    velocity: (i32, i32),
}

impl Robot {
    fn parse_str(s: &str) -> Self {
        let re = Regex::new(r"p=(?P<px>(\d+)),(?P<py>\d+)\s+v=(?P<vx>[-\d]+),(?P<vy>[-\d]+)").unwrap();

        let cs= re
            .captures(s)
            .unwrap();
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
            x if x < 0 => (*self).position.0 = (ROOM_SIZE.0 - ((*self).position.0.abs() % ROOM_SIZE.0)) % ROOM_SIZE.0,
            _ => (),
        }

        (*self).position.1 += self.velocity.1 * n_steps;
        match (*self).position.1 {
            x if x >= ROOM_SIZE.1 => (*self).position.1 %= ROOM_SIZE.1,
            x if x < 0 => (*self).position.1 = (ROOM_SIZE.1 - ((*self).position.1.abs() % ROOM_SIZE.1)) % ROOM_SIZE.1,
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

    let n_steps: i32 = 100;
    let mut safety_factors: Vec<i32> = vec![0; 4];

    let robots: Vec<Robot>= input
        .lines()
        .map(|s| Robot::parse_str(s))
        .collect();

    for mut robot in robots {

        print!("p{:?} v{:?} ", robot.position, robot.velocity);

        robot.simulate(n_steps);

        print!("--[{n_steps}]--> p{:?} ", robot.position);

        if robot.position.0 == ROOM_SIZE.0 / 2 || robot.position.1 == ROOM_SIZE.1 / 2 {
            // robots that are exactly in the middle (horizontally or vertically) don't count towards any quadrant
            println!("q: on axis!");
            continue;
        }

        let quad_idx: i32 =
        robot.position.1 / ((ROOM_SIZE.1 + 1) / 2) * 2
        + robot.position.0 / ((ROOM_SIZE.0 + 1) / 2);

        println!("q: {quad_idx}");

        safety_factors[quad_idx as usize] += 1;
    }
    println!();

    let res_1: i32 = safety_factors.into_iter().product();
    println!("part 1: {}", res_1);
}
