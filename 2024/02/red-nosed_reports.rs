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

    let lines = input.lines();

    let mut res_no_error_correction: usize = 0;
    let mut res_error_correction: usize = 0;


    for l in lines
    {
        let report: Vec<i32> = l.split_whitespace().map(|x| x.parse::<i32>().unwrap()).collect();

        // determine if ascending/descending order (accounts for one error)
        let n_asc: usize = report.windows(2).filter(|w| w[0] < w[1]).count();
        let is_asc: bool = n_asc > 1;

        let found_errors = report
            .windows(2)
            .into_iter()
            .enumerate()
            .find(|(_, w)| !is_safe_diff(w[0], w[1], is_asc));

        if found_errors.is_none()
        {
            res_no_error_correction += 1;
            res_error_correction += 1;
        }
        else
        {
            let start_idx: usize = found_errors.unwrap().0;
            for skip_idx in start_idx..report.len()
            {
                let mut report_variant = report.clone();
                report_variant.remove(skip_idx);
                if report_variant.windows(2).all(|w| is_safe_diff(w[0], w[1], is_asc))
                {
                    res_error_correction += 1;
                    break;
                }
            }
        }
    }

    println!("part1: {}", res_no_error_correction);
    println!("part2: {}", res_error_correction);
}

fn is_safe_diff(lhs: i32, rhs: i32, is_asc: bool) -> bool {
    let diff: i32 = (lhs - rhs).abs();
    (diff <= 3 && diff >= 1) && ((lhs < rhs) == is_asc)
}
