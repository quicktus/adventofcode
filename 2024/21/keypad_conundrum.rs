use std::fs::File;
use std::io::Read;
use std::path::Path;
use std::collections::HashMap;

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

    let mut cache: HashMap<((i16, i16), (i16, i16), u32), usize> = HashMap::new();

    let mut res_1: usize = 0;
    let mut res_2: usize = 0;

    for line in input.lines() {
        // get the numeric part
        let num: usize = line.strip_suffix('A').unwrap().parse().unwrap();

        // build the sequence
        let seq = num_2_dir(line.to_owned());
        res_1 += num * find_seq_len(seq.to_owned(), 2, &mut cache);
        res_2 += num * find_seq_len(seq.to_owned(), 25, &mut cache);
    }

    println!("part 1: {}", res_1);
    println!("part 2: {}", res_2);
}

fn num_2_dir(seq: String) -> String {
    let mut current: (i16, i16) = get_button_coords('A', false);
    let mut res: String = String::new();

    for c in seq.chars() {
        let target: (i16, i16) = get_button_coords(c, false);

        let x_diff: i16 = current.0 - target.0;
        let y_diff: i16 = current.1 - target.1;

        let mut postponed: String = String::new();

        // button priority: left>up>down>right>activate
        // left (but don't move into empty square)
        if x_diff > 0 {
            if current.1 == 0 && target.0 == 0 {
                postponed = "<".repeat(x_diff.abs() as usize);
            }
            else {
                res.push_str(&("<".repeat(x_diff.abs() as usize)));
            }
        }

        // up
        if y_diff < 0 {
            res.push_str(&("^".repeat(y_diff.abs() as usize)));
        }

        // down (but don't move into empty square)
        if y_diff > 0 {
            if current.0 == 0 && target.1 == 0 {
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

        // append postponed (avoids empty square)
        res.push_str(&postponed);

        res.push_str("A");

        current = target;
    }
    return res;
}

fn find_seq_len(seq: String, depth: u32, cache: &mut HashMap<((i16, i16), (i16, i16), u32), usize>) -> usize {
    //println!("seq: {seq}; depth: {depth}");

    if depth == 0 {
        return seq.len();
    }

    let mut current: (i16, i16) = get_button_coords('A', true);
    let mut res: usize = 0;

    for c in seq.chars() {
        let target: (i16, i16) = get_button_coords(c, true);

        let cache_res = cache.get(&(current, target, depth));
        if cache_res.is_some() {
            res += cache_res.unwrap();
        }
        else
        {
            let x_diff: i16 = current.0 - target.0;
            let y_diff: i16 = current.1 - target.1;

            let mut next_seq: String = String::new();
            let mut postponed: String = String::new();

            // button priority: left>up>down>right>activate
            // left (but don't move into empty square)
            if x_diff > 0 {
                if current.1 == 1 && target.0 == 0 {
                    postponed = "<".repeat(x_diff.abs() as usize);
                }
                else {
                    next_seq.push_str(&("<".repeat(x_diff.abs() as usize)));
                }
            }

            // up
            if y_diff < 0 {
                if current.0 == 0 {
                    postponed = "^".repeat(y_diff.abs() as usize);
                }
                else {
                    next_seq.push_str(&("^".repeat(y_diff.abs() as usize)));
                }
            }

            // down (but don't move into empty square)
            if y_diff > 0 {
                next_seq.push_str(&("v".repeat(y_diff.abs() as usize)));
            }

            // right
            if x_diff < 0 {
                next_seq.push_str(&(">".repeat(x_diff.abs() as usize)));
            }

            // append postponed (avoids empty square)
            next_seq.push_str(&postponed);

            next_seq.push_str("A");

            let len = find_seq_len(next_seq, depth -1, cache);
            res += len;
            cache.insert((current, target, depth), len);
        }
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
