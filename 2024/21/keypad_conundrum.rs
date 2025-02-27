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

    // TODO:
    // - read https://www.reddit.com/r/adventofcode/s/KCJJZVqS2o
    // - understand button priority (left>up>down>right>activate)
    // - generate tables of optimal moves
    // - apply to input
}
