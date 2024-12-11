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

    let stones: Vec<u128> = input
        .split_ascii_whitespace()
        .map(|s| s.parse::<u128>().unwrap())
        .collect();

    // k: (stone value, n rounds of sim)
    // v: resulting n stones
    let mut cache: HashMap<(u128, usize), u128> = HashMap::new();

    // part 1
    let sim_len_1: usize = 25;
    let mut res_1: u128 = 0;
    for stone in &stones {
        res_1 += sim(*stone, sim_len_1, &mut cache);
    }
    println!("part 1: {}", res_1);

    // part 2
    let sim_len_2: usize =  75;
    let mut res_2: u128 = 0;
    for stone in &stones {
        res_2 += sim(*stone, sim_len_2, &mut cache);
    }
    println!("part 2: {}", res_2);
}

fn sim(stone: u128, sim_len: usize, cache: &mut HashMap<(u128, usize), u128>) -> u128 {
    if sim_len == 0 {
        // trivial case (after 0 steps there is always just the one stone)
        return 1;
    }
    else if cache.contains_key(&(stone, sim_len)) {
        // -> cache hit
        return *cache.get(&(stone, sim_len)).unwrap();
    }
    else {
        // -> cache miss

        // If the stone is engraved with the number 0, it is replaced by a stone engraved with the number 1.
        if stone == 0 {
            let res = sim(1, sim_len-1, cache);
            cache.insert((stone, sim_len), res);
            return res;
        }

        // If the stone is engraved with a number that has an even number of digits, it is replaced by two stones.
        // The left half of the digits are engraved on the new left stone, and the right half of the digits are engraved on the new right stone.
        // (The new numbers don't keep extra leading zeroes: 1000 would become stones 10 and 0.)
        let stone_ndigits = stone.ilog10() + 1;
        if stone_ndigits % 2 == 0 {
            let ten: u128 = 10;
            let (lhs, rhs): (u128, u128) = (
                stone / ten.pow(stone_ndigits / 2),
                stone % ten.pow(stone_ndigits / 2)
            );
            let res = sim(lhs, sim_len-1, cache) + sim(rhs, sim_len-1, cache);
            cache.insert((stone, sim_len), res);
            return res;
        }

        // If none of the other rules apply, the stone is replaced by a new stone.
        // The old stone's number multiplied by 2024 is engraved on the new stone.
        let res = sim(stone * 2024, sim_len-1, cache);
        cache.insert((stone, sim_len), res);
        return res;
    }
}
