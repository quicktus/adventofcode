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

    let mut res_1 = 0;
    let mut res_2 = 0;

    let equations: Vec<Vec<u64>> = input
        .lines()
        .map(|l: &str| l.replace(":", ""))
        .map(|l: String| l.split(" ").map(|s| s.parse().unwrap()).collect())
        .collect();

    for numbers in equations {
        let expected: u64 = numbers[0];
        let operands =  &numbers[1..numbers.len()];

        if find_solution_1(expected, operands[0], &operands[1..operands.len()]) {
            res_1 += expected;
        }
        if find_solution_2(expected, operands[0], &operands[1..operands.len()]) {
            res_2 += expected;
        }
    }

    println!("part 1: {}", res_1);
    println!("part 2: {}", res_2);
}

fn find_solution_1(expected: u64, accumulator: u64, operands: &[u64]) -> bool {
    if accumulator > expected {
        return false;
    }

    if operands.is_empty() {
        return accumulator == expected;
    }

    let sum = accumulator + operands[0];
    if find_solution_1(expected, sum, &operands[1..operands.len()]) {
        return true;
    }

    let mult = accumulator * operands[0];
    if find_solution_1(expected, mult, &operands[1..operands.len()]) {
        return true;
    }

    false
}

fn find_solution_2(expected: u64, accumulator: u64, operands: &[u64]) -> bool {
    if accumulator > expected {
        return false;
    }

    if operands.is_empty() {
        return accumulator == expected;
    }

    let sum = accumulator + operands[0];
    if find_solution_2(expected, sum, &operands[1..operands.len()]) {
        return true;
    }

    let concat = numconcat(accumulator, operands[0]);
    if find_solution_2(expected, concat, &operands[1..operands.len()]) {
        return true;
    }

    let mult = accumulator * operands[0];
    if find_solution_2(expected, mult, &operands[1..operands.len()]) {
        return true;
    }

    false
}

fn numconcat (front: u64, back: u64) -> u64 {
    let n_digits = back.ilog10() + 1;
    let ten: u64 = 10;

    front * ten.pow(n_digits) + back
}
