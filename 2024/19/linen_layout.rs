use std::collections::HashMap;
use std::fs::File;
use std::io::Read;
use std::path::Path;
use std::slice;
use std::str;

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

    let [t, p]: [&str; 2] = input
    .split("\n\n")
    .collect::<Vec<&str>>()
    .try_into()
    .unwrap();

    let towels: Vec<&str> = t.trim().split(", ").collect();
    let mut cache: HashMap<String, u64> = HashMap::new();
    let patterns = p.lines();
    let mut result_1: u64 = 0;
    let mut result_2: u64 = 0;

    for pattern in patterns {
        let n_constructions: u64 = count_constructions(pattern, &towels, &mut cache);
        if n_constructions > 0 {
            result_1 += 1;
            result_2 += n_constructions as u64;
        }
    }

    println!("part 1: {}", result_1);
    println!("part 2: {}", result_2);
}

fn count_constructions(pattern: &str, towels: &Vec<&str>, cache: &mut HashMap<String, u64>) -> u64 {
    if pattern.is_empty() {
        return 1;
    }

    if let Some(&result) = cache.get(pattern) {
        return result;
    }

    let mut res = 0;
    for towel in towels {
        if pattern.starts_with(*towel) {
            let remaining_pattern = unsafe {
                let ptr = pattern.as_ptr().add(towel.len());
                let len = pattern.len() - towel.len();
                let slice = slice::from_raw_parts(ptr, len);
                str::from_utf8_unchecked(slice)
            };
            res += count_constructions(remaining_pattern, towels, cache);
        }
    }

    cache.insert(pattern.to_string(), res);

    res
}
