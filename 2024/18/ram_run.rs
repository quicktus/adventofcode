use std::fs::File;
use std::io::Read;
use std::path::Path;
use pathfinding::prelude::dijkstra;

static mut MAZE: Vec<Vec<bool>> = Vec::new();
static mut LEN: usize = 71;

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
    struct Pos{coords: (usize, usize), corrupted: bool}

    impl Pos {
        unsafe fn successors(&self) -> Vec<(Pos, u32)> {
            let mut s= vec![];
            for nbr in neighbors(LEN, self.coords.0, self.coords.1) {
                if !MAZE[nbr.1][nbr.0] {
                    s.push((Pos {coords: (nbr.0, nbr.1), corrupted: false}, 1));
                }
            }

            s
        }
    }

    let corruption_coords: Vec<Vec<usize>> = input.lines().map(|s| s.split(",").map(|s| s.parse().unwrap()).collect()).collect();

    unsafe {
        MAZE = vec![vec![false;LEN];LEN];

        let mut start_idx: (usize, usize) = (0, 0);
        let mut finish_idx: (usize, usize) = (70, 70);


        for c_idx in 0..1024 {
            MAZE[corruption_coords[c_idx][1]][corruption_coords[c_idx][0]] = true;
        }

        let (_path, result_1) = dijkstra(&Pos{coords: start_idx, corrupted: false}, |p| p.successors(), |p| p.coords == finish_idx).unwrap();
        println!("part 1: {}", result_1);

        let mut previous_path: Vec<Pos> = vec![];
        for c_idx in 1024..3450 {
            MAZE[corruption_coords[c_idx][1]][corruption_coords[c_idx][0]] = true;

            if previous_path.len() == 0 || previous_path.contains(&Pos{coords: (corruption_coords[c_idx][0],corruption_coords[c_idx][1]), corrupted: false})
            {
                let result_2 = dijkstra(&Pos{coords: start_idx, corrupted: false}, |p| p.successors(), |p| p.coords == finish_idx);
                if result_2.is_some() {
                    let (path, _) = result_2.unwrap();
                    previous_path = path;
                }
                else {
                    println!("part 2: {},{}", corruption_coords[c_idx][0],corruption_coords[c_idx][1]);
                    break;
                }
            }

        }

    }
}

fn neighbors(size: usize, row: usize, col: usize) -> Vec<(usize, usize)> {
    let mut nbrs: Vec<(usize, usize)> = vec![];
    for offset in vec![(1,0), (0,1), (-1,0), (0,-1)] {
        // avoid going oob
        if (row == 0 && offset.0 == -1)
        || (col == 0 && offset.1 == -1)
        || (row == ((size as i32)-1) as usize && offset.0 == 1)
        || (col == ((size as i32)-1) as usize && offset.1 == 1)
        {
            continue;
        }
        let neighbor= apply_offset((row, col), offset);
        nbrs.push(neighbor);
    }
    nbrs
}

fn apply_offset(point: (usize, usize), offset: (i32, i32)) -> (usize, usize) {
    (
        (point.0 as i32 + offset.0) as usize,
        (point.1 as i32 + offset.1) as usize,
    )
}