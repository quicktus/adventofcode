use std::fs::File;
use std::io::Read;
use std::path::Path;

const MIN_TIME_SAVE: usize = 100;

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
    let len = char_grid.len();
    let mut start_idx: (usize, usize) = (0, 0);
    let mut finish_idx: (usize, usize) = (0, 0);

    // find start / end
    for x in 0..len {
        for y in 0..len {
            if char_grid[y][x] == 'S' {
                start_idx = (x, y);
            }
            if char_grid[y][x] == 'E' {
                finish_idx = (x, y);
            }
        }
    }

    let mut curr_pos: (usize, usize) = start_idx;
    let mut path: Vec<(usize, usize)> = vec![start_idx];

    // construct the path
    while curr_pos != finish_idx {
        for nbr in generate_manhattan_points(curr_pos, 1, len) {
            let prev_pos = path.len().checked_sub(2).map(|i| path[i]);
            if "S.E".contains(char_grid[nbr.1][nbr.0]) && (prev_pos.is_none() || nbr != prev_pos.unwrap()) {
                curr_pos = nbr;
                path.push(nbr);
                break;
            }
        }
    }

    let mut res_1 = 0;
    let jump_time = 2;
    for pos_idx in 0..(path.len()-(MIN_TIME_SAVE+jump_time)) {
        let pos = path[pos_idx];
        for jump_target_idx in generate_manhattan_points(pos, 2, len) {
            if path[(pos_idx+(MIN_TIME_SAVE+jump_time))..].contains(&jump_target_idx) {
                res_1 += 1;
            }
        }
    }
    println!("part 1: {}", res_1);

    let mut res_2 = 0;
    for pos_idx in 0..(path.len()-MIN_TIME_SAVE) {
        let pos = path[pos_idx];
        for jump_target_idx in (pos_idx + MIN_TIME_SAVE)..(path.len()) {
            let time_save = jump_target_idx - pos_idx;
            let md = manhattan_distance(path[pos_idx], path[jump_target_idx]);
            if (md <= 20 && time_save - md >= MIN_TIME_SAVE) {
                res_2 += 1;
            }
        }
    }
    println!("part 2: {}", res_2);
}

fn checked_apply_offset(point: (usize, usize), offset: (i32, i32), size: usize) -> Option<(usize, usize)> {
    if (point.0 == 0 && offset.0 < 0)
        || (point.1 == 0 && offset.1 < 0)
        || (point.0 == ((size as i32)-1) as usize && offset.0 > 0)
        || (point.1 == ((size as i32)-1) as usize && offset.1 > 0)
    {
        return None;
    }
    else {
        Some((
            (point.0 as i32 + offset.0) as usize,
            (point.1 as i32 + offset.1) as usize,
        ))
    }
}

fn generate_manhattan_points(center: (usize, usize), distance: i32, grid_size: usize) -> Vec<(usize, usize)> {
    let mut points: Vec<(usize, usize)> = vec![];
    for dist in 0..distance {
        let dist_a = dist as i32;
        let dist_b = distance as i32 - dist_a;
        let dr = checked_apply_offset(center, (dist_a, dist_b), grid_size);
        let ur = checked_apply_offset(center, (dist_b, -dist_a), grid_size);
        let ul = checked_apply_offset(center, (-dist_a, -dist_b), grid_size);
        let dl = checked_apply_offset(center, (-dist_b, dist_a), grid_size);
        [dr, ur, ul, dl].iter().filter(|x| x.is_some()).for_each(|x| points.push(x.unwrap()));
    }

    points
}

fn manhattan_distance(start_point: (usize, usize), end_point: (usize, usize)) -> usize {
    ((start_point.0 as i32 - end_point.0 as i32).abs() + (start_point.1 as i32 - end_point.1 as i32).abs()) as usize
}
