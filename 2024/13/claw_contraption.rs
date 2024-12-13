#![feature(f128)]

use std::fs::File;
use std::io::Read;
use std::path::Path;
use std::f128;

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

    let numbers: Vec<f128>= input
        .split_ascii_whitespace()
        .enumerate()
        .filter(|(i, _)| [2,3,6,7,9,10].contains(&(*i % 11)))
        .map(|(_, val)| val.replace(&['X', 'Y', '=', ','][..], "").parse::<i32>().unwrap() as f128)
        .collect();

    let mut total_cost_1: u32 = 0;
    let mut total_cost_2: u64 = 0;

    for machine in numbers.chunks(6) {
        let (bax, bay, bbx, bby, px, py)
            = (machine[0], machine[1], machine[2], machine[3], machine[4], machine[5]);
        //println!("Button A: X{bax}, Y{bay}\nButton B: X{bbx}, Y{bby}\nPrize: X={px}, Y={py}");

        // line a = incline_a * x + offset_a
        // line b = incline_b * x + 0

        let incline_a: f128 = bay / bax;
        let incline_b: f128 = bby / bbx;

        // part 1
        {
            let offset_a: f128 = py - incline_a * px;
            //let offset_b = (py as f128 - incline_b * px as f128);

            let x_midpoint: f128 = offset_a / (incline_b - incline_a);
            let y_midpoint: f128 = incline_b * x_midpoint;

            let origin_to_midpoint: f128 = dist((0.0, 0.0), (x_midpoint, y_midpoint));
            let midpoint_to_target: f128 = dist((x_midpoint, y_midpoint), (px, py));
            let len_a_step: f128 = dist((0.0, 0.0), (bax, bay));
            let len_b_step: f128 = dist((0.0, 0.0), (bbx, bby));

            let na = midpoint_to_target / len_a_step;
            let nb = origin_to_midpoint / len_b_step;

            if is_basically_int(na) && is_basically_int(nb) {
                // it costs 3 tokens to push the A button and 1 token to push the B button
                total_cost_1 += na.round() as u32 * 3 + nb.round() as u32;
            }
        }

        // part 2
        {
            let offset_a: f128 = (py + 10000000000000.0) - incline_a * (px + 10000000000000.0);

            let x_midpoint: f128 = offset_a / (incline_b - incline_a);
            let y_midpoint: f128 = incline_b * x_midpoint;

            let origin_to_midpoint: f128 = dist((0.0, 0.0), (x_midpoint, y_midpoint));
            let midpoint_to_target: f128 = dist((x_midpoint, y_midpoint), ((px + 10000000000000.0), (py + 10000000000000.0)));
            let len_a_step: f128 = dist((0.0, 0.0), (bax, bay));
            let len_b_step: f128 = dist((0.0, 0.0), (bbx, bby));

            let na = midpoint_to_target / len_a_step;
            let nb = origin_to_midpoint / len_b_step;

            if is_basically_int(na) && is_basically_int(nb) {
                total_cost_2 += na.round() as u64 * 3 + nb.round() as u64;
            }
        }
    }

    println!("part 1: {}", total_cost_1);
    println!("part 2: {}", total_cost_2);
}

fn dist(point_a: (f128, f128), point_b: (f128, f128)) -> f128 {
    f128::sqrt(
        f128::powf(point_a.0 - point_b.0, 2 as f128)
        + f128::powf(point_a.1 - point_b.1, 2 as f128)
    )
}

fn is_basically_int(x: f128) -> bool {
    (x - x.round()).abs() < 1e-8
}
