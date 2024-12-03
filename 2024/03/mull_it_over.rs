use regex::Regex;
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

    let rules = vec![
        r"mul\([0-9]+,[0-9]+\)",
        r"do\(\)",
        r"don\'t\(\)"
    ];
    let mul_exp = Regex::new(&rules.join("|")).unwrap();
    let mut res_1: i32 = 0;
    let mut res_2: i32 = 0;

    let expressions = mul_exp.find_iter(&input);
    let mut flag_math_enabled = true;

    for exp in expressions {
        let exp_string = exp.as_str();
        if exp_string.starts_with("mul")
        {
            let factor_1_start = exp_string.find('(').unwrap() + 1;
            let factor_1_end = exp_string[factor_1_start..].find(',').unwrap() + factor_1_start;
            let factor_2_start = factor_1_end + 1;
            let factor_2_end = exp_string[factor_2_start..].find(')').unwrap() + factor_2_start;

            let factor_1 = exp_string[factor_1_start..factor_1_end].parse::<i32>().unwrap();
            let factor_2 = exp_string[factor_2_start..factor_2_end].parse::<i32>().unwrap();

            let product = factor_1 * factor_2;
            res_1 += product;
            if flag_math_enabled
            {
                res_2 += product;
            }
        }
        else if exp_string.eq("do()")
        {
            flag_math_enabled = true;
        }
        else if exp_string.eq("don't()")
        {
            flag_math_enabled = false;
        }
    }

    println!("part1: {}", res_1);
    println!("part2: {}", res_2);
}
