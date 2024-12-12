use std::collections::HashMap;
use std::fs::File;
use std::io::Read;
use std::path::Path;

const ID_UNKNOWN: i32 = -1;

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

    let field_map: Vec<Vec<char>> = input.lines().map(|x| x.chars().collect()).collect();
    let (row_len, col_len) = (field_map[0].len(), field_map.len());

    let mut ids: Vec<Vec<i32>> = vec![vec![ID_UNKNOWN; row_len]; col_len];
    let mut current_id = 0;
    for r in 0..row_len {
        for c in 0..col_len {
            if ids[c][r] == ID_UNKNOWN {
                ids[c][r] = current_id;
                floodfill(&field_map, &mut ids, r, c);
                current_id += 1;
            }
        }
    }

    let mut perimeters: Vec<Vec<(bool, bool, bool, bool)>> = vec![vec![(false, false, false, false); row_len]; col_len];
    for r in 0..row_len {
        for c in 0..col_len {
            let crop_type: char = field_map[c][r];
            let (mut nr, mut nc): (isize, isize);

            // right
            nr = r as isize + 1;
            let perim_right: bool = r+1 == row_len || (r+1 < row_len && field_map[c][nr as usize] != crop_type);

            // down
            nc = c as isize + 1;
            let perim_down: bool = c+1 == col_len || (c+1 < col_len && field_map[nc as usize][r] != crop_type);

            // left
            nr = r as isize - 1;
            let perim_left: bool = r == 0 || (r > 0 && field_map[c][nr as usize] != crop_type);

            // up
            nc = c as isize - 1;
            let perim_up: bool = c == 0 || (c > 0 && field_map[nc as usize][r] != crop_type);

            perimeters[c][r] = (perim_right, perim_down, perim_left, perim_up);
        }
    }

    let mut edges: HashMap<i32, i32> = HashMap::new();
    for r in 0..row_len {
        for c in 0..col_len {
            let id: i32 = ids[c][r];
            let perims = perimeters[c][r];
            if !edges.contains_key(&id) {
                edges.insert(id, 0);
            }

            if perims.0  // right
            {
                // vertical edge -> check up
                let nc: isize = c as isize  - 1;
                if (nc < 0)
                    || (ids[nc as usize][r] != id)
                    || (ids[nc as usize][r] == id && !perimeters[nc as usize][r].0) {
                    edges.insert(id, edges.get(&id).unwrap()+1);
                }
            }

            if perims.1  // down
            {
                // horizontal edge -> check left
                let nr: isize = r as isize - 1;
                if (nr < 0)
                    || (ids[c][nr as usize] != id)
                    || (ids[c][nr as usize] == id && !perimeters[c][nr as usize].1) {
                    edges.insert(id, edges.get(&id).unwrap()+1);
                }
            }

            if perims.2  // left
            {
                // vertical edge -> check up
                let nc: isize = c as isize  - 1;
                if (nc < 0)
                    || (ids[nc as usize][r] != id)
                    || (ids[nc as usize][r] == id && !perimeters[nc as usize][r].2) {
                    edges.insert(id, edges.get(&id).unwrap()+1);
                }
            }

            if perims.3  // up
            {
                // horizontal edge -> check left
                let nr: isize = r as isize - 1;
                if (nr < 0)
                    || (ids[c][nr as usize] != id)
                    || (ids[c][nr as usize] == id && !perimeters[c][nr as usize].3) {
                    edges.insert(id, edges.get(&id).unwrap()+1);
                }
            }
        }
    }

    let mut total_cost_after_discount: i32 = 0;
    for id in edges.keys() {
        let area: i32 = ids.clone().into_iter()
            .flatten()
            .filter(|i| *i == *id)
            .count() as i32;
        let n_edges = *edges.get(id).unwrap();
        total_cost_after_discount += area * n_edges;
    }

    let total_cost: u32 = ids.into_iter().flatten()
        .zip(perimeters.into_iter().flatten())
        .fold(HashMap::new(), |mut acc: HashMap<i32, (u32, u32)>, (id, perim)| {
                let entry = match acc.contains_key(&id) {
                    true => *acc.get_mut(&id).unwrap(),
                    false => (0, 0),
                };
                acc.insert(id, (entry.0 + 1, entry.1 + (perim.0 as u8 + perim.1 as u8 + perim.2 as u8 + perim.3 as u8) as u32));
                acc
            }
        )
        .values()
        .map(|(area, perim)| (*area) * (*perim))
        .sum();

    println!("part 1: {}", total_cost);
    println!("part 2: {}", total_cost_after_discount);
}

/// get indices of non-diagonal neighbors
fn neighbors(grid: &Vec<Vec<char>>, row: usize, col: usize) -> Vec<(usize, usize)> {
    let mut nbrs: Vec<(usize, usize)> = vec![];
    for offset in vec![(1,0), (0,1), (-1,0), (0,-1)] {
        let neighbor = (
            (row as i32 + offset.0) as usize,
            (col as i32 + offset.1) as usize,
        );
        // avoid going oob
        if (row == 0 && offset.0 == -1)
            || (col == 0 && offset.1 == -1)
            || (row == grid.len()-1 && offset.0 == 1)
            || (col == grid[0].len()-1 && offset.1 == 1)
        {
            continue;
        }
        nbrs.push(neighbor);
    }
    nbrs
}

/// set all neighbors with the same crop_type to have the id of the passed idx
fn floodfill(grid: &Vec<Vec<char>>, ids: &mut Vec<Vec<i32>>, r: usize, c: usize) {
    let crop_type: char = grid[c][r];
    let id = ids[c][r];
    for (nr, nc) in neighbors(&grid, r, c) {
        if grid[nc][nr] == crop_type && ids[nc][nr] == ID_UNKNOWN {
            ids[nc][nr] = id;
            floodfill(grid, ids, nr, nc);
        }
    }
}
