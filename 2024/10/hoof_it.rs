use std::fs::File;
use std::io::Read;
use std::path::Path;


const START: u8 = 0;
const END: u8 = 9;


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

    let topo_map: Vec<Vec<u8>> = input
        .lines()
        .map(|x: &str|
            x.chars()
            .map(|c: char| (c as u8) - 48)
            .collect()
        )
        .collect();

    let (x_len, y_len) = (topo_map.len(), topo_map[0].len());
    let mut sum_scores_1: i32 = 0;
    let mut sum_scores_2: i32 = 0;

    // head (0) -> end (9)
    for x in 0..x_len {
        for y in 0..y_len {
            let height = topo_map[y][x];
            if height == START {
                let mut accumulator: Vec<(usize, usize)> = vec![];

                find_trails(&topo_map, x, y, &mut accumulator);

                sum_scores_1 += accumulator.len() as i32;
                accumulator.sort();
                accumulator.dedup();
                sum_scores_2 += accumulator.len() as i32;
            }
        }
    }

    println!("part 1: {}", sum_scores_1);
    println!("part 2: {}", sum_scores_2);
}

fn find_trails(tmap: &Vec<Vec<u8>>, x: usize, y: usize, acc: &mut Vec<(usize, usize)>) {
    let height: u8 = tmap[y][x];

    if height == END {
        acc.push((x, y));
        return;
    }

    for offset in vec![(1,0), (0,1), (-1,0), (0,-1)] {
        if (x == 0 && offset.0 == -1)
            || (x == tmap.len()-1 && offset.0 == 1)
            || (y == 0 && offset.1 == -1)
            || (y == tmap[0].len()-1 && offset.1 == 1)
        {
            continue;  // avoid going oob
        }

        let neighbor_coords = (
            (x as i32 + offset.0) as usize,
            (y as i32 + offset.1) as usize,
        );
        if tmap[neighbor_coords.1][neighbor_coords.0] == height+1 {
            find_trails(tmap, neighbor_coords.0, neighbor_coords.1, acc);
        }
    }

    return;
}
