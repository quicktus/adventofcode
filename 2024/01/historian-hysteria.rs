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

    let n_lines = input.lines().count();
    let lines = input.lines();

    let mut nrs_l = vec![0; n_lines];
    let mut nrs_r = vec![0; n_lines];


    for (i, l) in lines.enumerate()
    {
        let mut nrs = l.split("   ");
        let nr_l = nrs.next().unwrap().parse::<i32>().unwrap();
        let nr_r = nrs.next().unwrap().parse::<i32>().unwrap();

        nrs_l[i] = nr_l;
        nrs_r[i] = nr_r;
    }

    nrs_l.sort();
    nrs_r.sort();

    let sum_diff = std::iter::zip(&nrs_l, &nrs_r)
        .map(|(l, r)| (l - r).abs())
        .sum::<i32>();
    println!("part 1: {}", sum_diff);

    let mut sum_scores = 0;
    for nr_l in nrs_l
    {
        let n_occurences: i32 = nrs_r.iter().filter(|&n| *n == nr_l).count().try_into().unwrap();
        sum_scores += nr_l * n_occurences;
    }
    println!("part 2: {}", sum_scores);
}
