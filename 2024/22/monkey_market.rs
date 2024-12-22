#![feature(map_try_insert)]

use std::fs::File;
use std::io::Read;
use std::path::Path;
use std::collections::{HashMap, VecDeque};


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

    let starting_secrets: Vec<usize> = input.lines().map(|s| s.parse().unwrap()).collect();

    let mut res_1 = 0;
    let mut combined_sequences: HashMap<VecDeque<i16>, i16> = HashMap::with_capacity(200);
    for starting_secret in starting_secrets {
        let mut sequences: HashMap<VecDeque<i16>, i16> = HashMap::new();
        let mut secret = starting_secret;
        let mut price = (secret % 10) as i16;
        let mut price_changes: VecDeque<i16> = VecDeque::from([0, 0, 0, 0]);
        for i in 0..2000 {
            let prev_price = price;

            secret = next_secret(secret);

            price = (secret % 10) as i16;
            price_changes.rotate_right(1);
            price_changes[0] = price - prev_price;
            if i >= 3 {
                let _ = sequences.try_insert(price_changes.clone(), price);
            }
        }
        res_1 += secret;

        // merge
        for (key, value) in sequences {
            let res = combined_sequences.try_insert(key.clone(), value);
            if res.is_err() {
                let sum = value + combined_sequences.get(&key).unwrap();
                combined_sequences.insert(key, sum);
            }
        }
    }
    println!("part 1: {}", res_1);
    println!("part 2: {}", combined_sequences.values().max().unwrap());
}


fn next_secret(secret: usize) -> usize {
    let step_1 = mix_and_prune(secret, secret * 64);
    let step_2 = mix_and_prune(step_1, step_1 / 32);
    let step_3 = mix_and_prune(step_2, step_2 * 2048);

    step_3
}

fn mix_and_prune (secret: usize, mix_in: usize) -> usize {
    (secret ^ mix_in) % 16777216
}
