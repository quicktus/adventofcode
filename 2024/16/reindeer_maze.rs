use std::cmp::Ordering;
use std::fs::File;
use std::io::Read;
use std::path::Path;
use std::collections::{BinaryHeap, HashMap};

#[derive(Eq, Hash, PartialEq, Clone)]
struct Position {
    coords: (usize, usize),
    direction: (i32, i32),
}

impl Position {
    fn successors(&self, maze: &Vec<Vec<char>>, initial_cost: u32) -> Vec<(Position, u32)> {
        let mut s: Vec<(Position, u32)> = vec![];

        // forward
        let next = apply_offset(self.coords, self.direction);
        if maze[next.1][next.0] != '#' {
            s.push((
                Position {
                    coords: next,
                    direction: self.direction,
                },
                initial_cost + 1,
            ));
        }

        // left
        s.push((Position {coords: self.coords, direction: rot_left(self.direction),}, initial_cost + 1000,));

        // right
        s.push((Position {coords: self.coords, direction: rot_right(self.direction),}, initial_cost + 1000,));

        s
    }
}
impl Ord for Position {
    fn cmp(&self, other: &Self) -> Ordering {
        other.coords.cmp(&self.coords)
    }
}

impl PartialOrd for Position {
    fn partial_cmp(&self, other: &Self) -> Option<Ordering> {
        Some(self.cmp(other))
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

    let maze: Vec<Vec<char>> = input.lines().map(|x| x.chars().collect()).collect();
    let (x_len, y_len) = (maze[0].len(), maze.len());

    let mut start_idx: (usize, usize) = (0, 0);
    let mut finish_idx: (usize, usize) = (0, 0);

    for x in 0..x_len {
        for y in 0..y_len {
            if maze[y][x] == 'S' {
                start_idx = (x, y);
            }
            if maze[y][x] == 'E' {
                finish_idx = (x, y);
            }
        }
    }

    let start_pos: Position = Position {coords: start_idx, direction: (1, 0),};
    let mut visited: HashMap<Position, (u32, Vec<Position>)> = HashMap::new();
    let mut queue = BinaryHeap::new();
    queue.push((std::cmp::Reverse(0), start_pos.clone(), vec![start_pos]));
    let mut res_1 = u32::MAX;

    let mut coords_in_shortest_paths: Vec<(usize, usize)> = Vec::new();

    while let Some((std::cmp::Reverse(cost), current_pos, path)) = queue.pop() {
        if cost > res_1 {
            break;
        }

        visited.insert(current_pos.clone(), (cost, path.clone()));
        if current_pos.coords == finish_idx {
            res_1 = cost;

            for pos in path {
                coords_in_shortest_paths.push(pos.coords);
            }
            coords_in_shortest_paths.sort();
            coords_in_shortest_paths.dedup();
            continue;
        }

        for (next_pos, next_cost) in current_pos.successors(&maze, cost) {
            if !visited.contains_key(&next_pos) || next_cost < visited[&next_pos].0 {
                let mut new_path = path.clone();
                new_path.push(next_pos.clone());
                queue.push((std::cmp::Reverse(next_cost), next_pos, new_path));
            }
        }
    }
    println!("part 1: {}", res_1);
    println!("part 2: {}", coords_in_shortest_paths.len());

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
