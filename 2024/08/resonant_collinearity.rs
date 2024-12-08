use std::collections::HashMap;
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
    let (row_len, col_len) = (char_grid[0].len(), char_grid.len());
    let mut antennae: HashMap<char, Vec<(usize, usize)>> = HashMap::new();
    let mut antinodes_1: HashMap<(i32, i32), char> = HashMap::new();
    let mut antinodes_2: HashMap<(i32, i32), char> = HashMap::new();

    for col_idx in 0..col_len {
        for row_idx in 0..row_len {
            let c = char_grid[col_idx][row_idx];
            if c == '.' {
                continue;
            }
            let locations = antennae.get_mut(&c);
            if locations.is_none() {
                antennae.insert(c, vec![(row_idx, col_idx)]);
            } else {
                let locs = locations.unwrap();
                for loc in &mut *locs {
                    let distance = distance((row_idx, col_idx), *loc);
                    let mut factor = 0;
                    loop {
                        let antinode_a = (row_idx as i32 - distance.0 * factor, col_idx as i32 - distance.1 * factor);
                        let antinode_b = (loc.0 as i32 + distance.0 * factor, loc.1 as i32 + distance.1 * factor);
                        let mut oob_counter = 0;
                        for an in [antinode_a, antinode_b] {
                            if an.0 >= 0
                                && an.1 >= 0
                                && an.0 < row_len as i32
                                && an.1 < col_len as i32
                            {
                                if factor == 1 {
                                    antinodes_1.insert(an, c);
                                }
                                antinodes_2.insert(an, c);
                            } else {
                                oob_counter += 1;
                            }
                        }
                        if oob_counter == 2 {
                            break;
                        }
                        factor += 1;
                    }
                }
                locs.push((row_idx, col_idx));
            }
        }
    }

    println!("part 1: {}", antinodes_1.len());
    println!("part 2: {}", antinodes_2.len());
}

fn distance(loc_a: (usize, usize), loc_b: (usize, usize)) -> (i32, i32) {
    let diff_0 = loc_b.0 as i32 - loc_a.0 as i32;
    let diff_1 = loc_b.1 as i32 - loc_a.1 as i32;
    (diff_0, diff_1)
}
