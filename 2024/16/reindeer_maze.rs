use std::fs::File;
use std::io::Read;
use std::io::{stdin, stdout, Write};
use std::path::Path;
use pathfinding::prelude::dijkstra;

static mut MAZE: Vec<Vec<char>> = Vec::new();

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

    #[derive(Clone, Debug, Eq, Hash, Ord, PartialEq, PartialOrd)]
    struct Pos{coords: (usize, usize), direction: (i32, i32)}

    impl Pos {
        unsafe fn successors(&self) -> Vec<(Pos, u32)> {
            let mut s= vec![
                // left
                (Pos {coords: self.coords, direction: rot_left(self.direction)}, 1000),
                // right
                (Pos {coords: self.coords, direction: rot_right(self.direction)}, 1000),
            ];

            // forward
            let next = apply_offset(self.coords, self.direction);
            if MAZE[next.1][next.0] != '#' {
                s.push((Pos {coords: next, direction: self.direction}, 1));
            }

            s
        }
    }

    unsafe {
        MAZE = input.lines().map(|x| x.chars().collect()).collect();
        let (x_len, y_len) = (MAZE[0].len(), MAZE.len());

        let mut start_idx: (usize, usize) = (0, 0);
        let mut finish_idx: (usize, usize) = (0, 0);

        for x in 0..x_len {
            for y in 0..y_len {
                if MAZE[y][x] == 'S' {
                    start_idx = (x, y);
                }
                if MAZE[y][x] == 'E' {
                    finish_idx = (x, y);
                }
            }
        }

        let (path, result) = dijkstra(&Pos{coords: start_idx, direction: (0, 1)}, |p| p.successors(), |p| p.coords == finish_idx).unwrap();

        for pos in path {
            for x in 0..x_len {
                for y in 0..y_len {
                    if (x, y) == pos.coords {
                        let dirchar = match pos.direction {
                            (0, 1)  => '>',
                            (0, -1) => '<',
                            (1, 0)  => 'v',
                            (-1, 0) => '^',
                            _ => panic!()
                        };
                        print!("\x1b[93m{dirchar}\x1b[0m");
                    }
                    else {
                        print!("{}", MAZE[y][x]);
                    }
                }
                println!();
            }
            pause();
        }

        println!("part 1: {}", result);
    }
}

fn apply_offset(point: (usize, usize), offset: (i32, i32)) -> (usize, usize) {
    (
        (point.0 as i32 + offset.0) as usize,
        (point.1 as i32 + offset.1) as usize,
    )
}

fn rot_right(direction: (i32, i32)) -> (i32, i32) {
    if direction.0 == 0 {
        return (-direction.1, direction.0);
    }
    (direction.1, direction.0)
}

fn rot_left(direction: (i32, i32)) -> (i32, i32) {
    if direction.1 == 0 {
        return (direction.1, -direction.0);
    }
    (direction.1, direction.0)
}

fn pause() {
    let mut stdout = stdout();
    stdout.write(b"Press Enter to continue...").unwrap();
    stdout.flush().unwrap();
    stdin().read(&mut [0]).unwrap();
}
