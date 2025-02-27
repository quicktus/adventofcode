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

    let mut res_1: usize = 0;
    // let mut res_2: usize = 0;

    for line in input.lines() {
        // get the numeric part
        let num: usize = line.strip_suffix('A').unwrap().parse().unwrap();

        // build the sequence
        let mut seq = find_seqence(line.to_owned(), false);

        for _ in 1..=2 {
            seq = find_seqence(seq.to_owned(), true);
        }
        res_1 += num * seq.len();

        // for _ in 2..=25 {
        //     seq = find_seqence(seq.to_owned(), true);
        // }
        // res_2 += num * seq.len();
    }

    println!("part 1: {}", res_1);
    // println!("part 2: {}", res_2);
}

fn find_seqence(seq: String, is_directional: bool) -> String {
    let mut current: (i16, i16) = get_button_coords('A', is_directional);
    let mut res: String = String::new();

    for c in seq.chars() {
        let target: (i16, i16) = get_button_coords(c, is_directional);

        let x_diff: i16 = current.0 - target.0;
        let y_diff: i16 = current.1 - target.1;

        let mut postponed: String = String::new();

        // button priority: left>up>down>right>activate
        // left (but don't move into empty square)
        if x_diff > 0 {
            if (!is_directional && current.1 == 0 && target.0 == 0)
                || (is_directional && current.1 == 1 && target.0 == 0) {
                    postponed = "<".repeat(x_diff.abs() as usize);
            }
            else {
                res.push_str(&("<".repeat(x_diff.abs() as usize)));
            }
        }

        // up (but don't move into empty square)
        if y_diff < 0 {
            if is_directional && current.0 == 0 {
                postponed = "^".repeat(y_diff.abs() as usize);
            }
            else {
                res.push_str(&("^".repeat(y_diff.abs() as usize)));
            }
        }

        // down (but don't move into empty square)
        if y_diff > 0 {
            if !is_directional && current.0 == 0 && target.1 == 0 {
                postponed = "v".repeat(y_diff.abs() as usize);
            }
            else {
                res.push_str(&("v".repeat(y_diff.abs() as usize)));
            }
        }

        // right
        if x_diff < 0 {
            res.push_str(&(">".repeat(x_diff.abs() as usize)));
        }

        // append postponed (avoids empy square)
        res.push_str(&postponed);

        res.push_str("A");

        current = target;
    }
    return res;
}

fn get_button_coords(button: char, is_directional: bool) -> (i16, i16) {
    if is_directional {
        return match button {
            'A' => (2, 1),
            '^' => (1, 1),
            '>' => (2, 0),
            'v' => (1, 0),
            '<' => (0, 0),
            _ => panic!("unexpected button")
        };
    }

    return match button {
        '0' => (1, 0),
        'A' => (2, 0),
        '1' => (0, 1),
        '2' => (1, 1),
        '3' => (2, 1),
        '4' => (0, 2),
        '5' => (1, 2),
        '6' => (2, 2),
        '7' => (0, 3),
        '8' => (1, 3),
        '9' => (2, 3),
        _ => panic!("unexpected button")
    };
}

    // numerical:
    // +---+---+---+
    // 3 7 | 8 | 9 |
    // +---+---+---+
    // 2 4 | 5 | 6 |
    // +---+---+---+
    // 1 1 | 2 | 3 |
    // +---+---+---+
    // 0   | 0 | A |
    //   0 +-1-+-2-+

    // directional:
    //     +---+---+
    // 1   | ^ | A |
    // +---+---+---+
    // 0 < | v | > |
    // +-0-+-1-+-2-+
