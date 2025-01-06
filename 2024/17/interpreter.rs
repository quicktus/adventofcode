use std::fs::File;
use std::io::Read;
use std::path::Path;

mod vm;

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

    let lines: Vec<&str> = input.lines().collect();
    let regs: Vec<u128> = lines[0..3]
        .iter()
        .map(|l| l[12..].parse::<u128>().unwrap())
        .collect();
    let mut state: vm::State = vm::State::new(regs);

    let program: Vec<u8> = lines[4][9..]
        .split(',')
        .map(|s| s.trim().parse().unwrap())
        .collect();

    vm::interpret(&mut state, &program);

    let out_string = state.stdout
        .iter()
        .map(|&i| i.to_string())
        .reduce(|acc, i| acc + "," + &i)
        .unwrap_or_else(|| String::new());

    println!("part 1: {}", out_string);
}
