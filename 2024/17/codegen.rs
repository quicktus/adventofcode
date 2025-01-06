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
    let target: Vec<u8> = lines[4][9..]
        .split(',')
        .map(|s| s.trim().parse().unwrap())
        .collect();

    let mut reg_a: u128 = 0;
    reg_a = dfs(&target, reg_a, &target).unwrap();

    println!("part 2: {}", reg_a);
}

fn dfs(target: &[u8], reg_a_init: u128, program: &Vec<u8>) -> Option<u128> {
    if target.len() == 0 {
        return Some(reg_a_init);
    }
    let mut reg_a = reg_a_init << 3;
    for i in 0..8 {
        reg_a = (reg_a & !0b111) | i;
        let mut state: vm::State = vm::State::new(vec![reg_a,0,0]);
        vm::interpret(&mut state, program);

        println!("{:o} -> {:?}", reg_a, state.stdout);
        if state.stdout[0] == target[target.len()-1] {
            let next_step = dfs( target.split_last().unwrap().1, reg_a, program);
            if next_step.is_some() {
                return next_step;
            }
        }
    }
    return None;
}
