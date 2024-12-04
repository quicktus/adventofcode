use std::fs::File;
use std::path::Path;
use std::io::Read;


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

    let mut res_1: i32 = 0;
    let mut res_2: i32 = 0;

    let char_grid: Vec<Vec<char>> = input.lines().map(|x| x.chars().collect()).collect();
    let (x_len, y_len) = (char_grid.len(), char_grid[0].len());

    // part 1
    for x_start in 0..x_len
    {
        for y_start in 0..y_len
        {
            for x_diff in vec![-1, 0, 1]
            {
                for y_diff in vec![-1, 0, 1]
                {
                    if x_diff == 0 && y_diff == 0 {continue;}
                    let mut valid: bool = true;
                    let mut x: i32 = x_start as i32;
                    let mut y: i32 = y_start as i32;
                    for c in vec!['X', 'M', 'A', 'S']
                    {
                        if x < 0
                            || x >= x_len as i32
                            || y < 0
                            || y >= y_len as i32
                            || char_grid[x as usize][y as usize] != c
                        {
                            valid = false;
                            break;
                        }
                        x += x_diff;
                        y += y_diff;
                    }
                    if valid {res_1 += 1;}
                }
            }
        }
    }

    // part 2
    for x in 1..(x_len-1)
    {
        for y in 1..(y_len-1)
        {
            if char_grid[x][y] != 'A' {continue;}
            let nw: char = char_grid[x-1][y-1];
            let ne: char = char_grid[x+1][y-1];
            let sw: char = char_grid[x-1][y+1];
            let se: char = char_grid[x+1][y+1];
            let sm: &str = "SM";

            if ((sm.contains(nw) && sm.contains(se)) && nw != se)
                && ((sm.contains(ne) && sm.contains(sw)) && ne != sw)
            {
                println!("found at x:{} y:{}", x, y);
                res_2 += 1;
            }
        }
    }

    println!("part1: {}", res_1);
    println!("part2: {}", res_2);
}
