use std::fs::File;
use std::io::Read;
use std::path::Path;

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

    let char_grid: Vec<Vec<char>> = input.lines().map(|x| x.chars().collect()).collect();
    let (x_len, y_len) = (char_grid.len(), char_grid[0].len());
    let mut obstacles: Vec<(usize, usize)> = vec![];
    let mut visited: Vec<(usize, usize)> = vec![];
    let mut guard_start: (usize, usize) = (0, 0);

    for x in 0..x_len {
        for y in 0..y_len {
            if char_grid[y][x] == '#' {
                obstacles.push((x, y));
            }
            if char_grid[y][x] == '^' {
                guard_start = (x, y)
            };
        }
    }
    visited.push(guard_start);

    // simulate guard movement
    let mut guard_pos: (usize, usize) = guard_start;
    let mut guard_facing: (i32, i32) = (0, -1);

    loop {
        let next = (
            (guard_pos.0 as i32 + guard_facing.0) as usize,
            (guard_pos.1 as i32 + guard_facing.1) as usize,
        );

        // out of bounds
        if !((0..x_len).contains(&next.0) && (0..y_len).contains(&next.1)) {
            break;
        }

        // If there is something directly in front of you, turn right 90 degrees.
        if obstacles.contains(&next) {
            rot_right(&mut guard_facing.0, &mut guard_facing.1);
        }
        // Otherwise, take a step forward.
        else {
            guard_pos = next;
            if !visited.contains(&guard_pos) {
                visited.push(guard_pos)
            };
        }
    }

    println!("part 1: {}", visited.len());

    let mut res_2: i32 = 0;
    for potential_obstacle in visited {
        if potential_obstacle == guard_start {
            continue;
        }

        guard_pos = guard_start;
        guard_facing = (0, -1);
        let mut trail: Vec<((usize, usize), (i32, i32))> = vec![];

        loop {
            let next = (
                (guard_pos.0 as i32 + guard_facing.0) as usize,
                (guard_pos.1 as i32 + guard_facing.1) as usize,
            );

            // out of bounds
            if !((0..x_len).contains(&next.0) && (0..y_len).contains(&next.1)) {
                break;
            }

            // If there is something directly in front of you, turn right 90 degrees.
            if obstacles.contains(&next) || next == potential_obstacle {
                rot_right(&mut guard_facing.0, &mut guard_facing.1);
            }
            // Otherwise, take a step forward.
            else {
                guard_pos = next;
                if trail.contains(&(guard_pos, guard_facing)) {
                    res_2 += 1;
                    break;
                }
                trail.push((guard_pos, guard_facing));
            }
        }
    }
    println!("part 2: {}", res_2);
}

fn rot_right(x: &mut i32, y: &mut i32) {
    if *x == 0 {
        *y *= -1;
    }
    *x += *y;
    *y = *x - *y;
    *x -= *y;
}
