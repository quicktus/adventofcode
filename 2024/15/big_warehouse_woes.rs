use std::fs::File;
use std::io::Read;
use std::path::Path;
use std::collections::HashMap;

const ROBOT: char = '@';
const WALL: char = '#';
const BOX_L: char = '[';
const BOX_R: char = ']';
const SMALL_BOX: char = 'O';
const FREE: char = '.';

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

    let idx_break = input.match_indices("\n\n").next().unwrap().0;
    let (grid, moves) = input.split_at(idx_break);

    let expanded_grid = grid
        .chars()
        .map(|c| match c {
            ROBOT => "@.",
            SMALL_BOX => "[]",
            WALL => "##",
            FREE => "..",
            '\n' => "\n",
            _ => panic!()
        }.to_string())
        .reduce(|acc, e| acc + &e)
        .unwrap();

    let mut warehouse_map: Vec<Vec<char>> = expanded_grid.lines().map(|x| x.chars().collect()).collect();
    let (row_len, col_len) = (warehouse_map[0].len(), warehouse_map.len());
    let directions: HashMap<char, (i32, i32)> = HashMap::from([
        ('>', (1, 0)),
        ('v', (0, 1)),
        ('<', (-1, 0)),
        ('^', (0, -1)),
    ]);
    let instructions = moves.replace("\n", "");
    let mut robot_idx: (usize, usize) = (0, 0);

    for col in 0..col_len {
        for row in 0..row_len {
            if warehouse_map[col][row] == ROBOT {
                robot_idx = (row, col);
                break;
            }
        }
    }

    for instruction in instructions.chars() {
        let offset: (i32, i32) = *directions.get(&instruction).unwrap();
        if warehouse_move(&mut warehouse_map, robot_idx, offset) {
            robot_idx = apply_offset(robot_idx, offset);
        }
    }

    for y in 0..col_len {
        for x in 0..row_len {
            print!("{}", warehouse_map[y][x]);
        }
        println!();
    }
    println!();

    let mut res_2: i32 = 0;
    for col in 0..col_len {
        for row in 0..row_len {
            if warehouse_map[col][row] == BOX_L {
                res_2 += 100 * (col as i32) + (row as i32);
            };
        }
    }

    println!("part 2: {}", res_2);
}

fn warehouse_move(map: &mut Vec<Vec<char>>, idx: (usize, usize), offset: (i32, i32)) -> bool {
    let source_content = map[idx.1][idx.0];
    let destination = apply_offset(idx, offset);
    let destination_content = map[destination.1][destination.0];

    if destination_content == WALL {
        return false;
    }

    else if destination_content == FREE {
        map[destination.1][destination.0] = source_content;
        map[idx.1][idx.0] = FREE;
        return true;
    }

    else if destination_content == BOX_L || destination_content == BOX_R {

        if (offset.1).abs() == 1 {
            // vertical move
            let dest_other_half = apply_offset(destination, (if destination_content == BOX_L {1} else {-1}, 0));

            if check_warehouse_move(map, destination, offset) && check_warehouse_move(map, dest_other_half, offset) {
                apply_warehouse_move(map, idx, offset);
                return true;
            }
            return false;
        }
        else {
            // horizontal move
            if !warehouse_move(map, destination, offset) {
                return false;
            }
            assert!(map[destination.1][destination.0] == FREE);
            map[destination.1][destination.0] = source_content;
            map[idx.1][idx.0] = FREE;
            return true;
        }
    }

    panic!();
}

fn check_warehouse_move(map: &mut Vec<Vec<char>>, idx: (usize, usize), offset: (i32, i32)) -> bool {
    let destination = apply_offset(idx, offset);
    let destination_content = map[destination.1][destination.0];

    if destination_content == WALL {
        return false;
    }

    else if destination_content == FREE {
        return true;
    }

    else if destination_content == BOX_L || destination_content == BOX_R {
        let dest_other_half = apply_offset(destination, (if destination_content == BOX_L {1} else {-1}, 0));
        return check_warehouse_move(map, destination, offset) && check_warehouse_move(map, dest_other_half, offset);
    }

    panic!();
}

fn apply_warehouse_move(map: &mut Vec<Vec<char>>, idx: (usize, usize), offset: (i32, i32)) {
    let destination = apply_offset(idx, offset);
    let destination_content = map[destination.1][destination.0];
    let source_content = map[idx.1][idx.0];

    if destination_content == BOX_L || destination_content == BOX_R {
        let dest_other_half = apply_offset(destination, (if destination_content == BOX_L {1} else {-1}, 0));
        apply_warehouse_move(map, destination, offset);
        apply_warehouse_move(map, dest_other_half, offset);
    }

    assert!(map[destination.1][destination.0] == FREE);  // updated destination content must be free now.

    map[destination.1][destination.0] = source_content;
    map[idx.1][idx.0] = FREE;
}

fn apply_offset(point: (usize, usize), offset: (i32, i32)) -> (usize, usize) {
    (
        (point.0 as i32 + offset.0) as usize,
        (point.1 as i32 + offset.1) as usize,
    )
}
