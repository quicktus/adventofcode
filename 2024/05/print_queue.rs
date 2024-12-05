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

    let [r, q]: [&str; 2] = input
        .split("\n\n")
        .collect::<Vec<&str>>()
        .try_into()
        .unwrap();

    let rules: Vec<Vec<i32>> = r.lines().map(|x| x.split("|").map(|s| s.parse().unwrap()).collect()).collect();
    let mut queues: Vec<(Vec<i32>, bool)> = q.lines().map(|x| (x.split(",").map(|s| s.parse::<i32>().unwrap()).collect(), true)).collect();

    // part 1
    for rule in &rules {
        for queue in &mut queues
        {
            if !queue.1 {continue;}

            let pos_a = queue.0.iter().position(|v| v == &rule[0]);
            let pos_b = queue.0.iter().position(|v| v == &rule[1]);
            queue.1 = match (pos_a, pos_b) {
                (Some(a), Some(b)) => a < b,
                _ => true,
            }
        }
    }
    let res_1: i32 = queues
        .clone()
        .into_iter()
        .filter(|(_, is_valid)| *is_valid)
        .map(|(values, _)| values[(values.len()-1)/2])
        .sum();

    // part 2
    for queue in &mut queues
    {
        if queue.1 {continue;}

        let mut sorted: bool = false;

        while !sorted
        {
            sorted = true;
            for rule in &rules
            {
                let pos_a = queue.0.iter().position(|v| v == &rule[0]);
                if pos_a.is_none() {continue;}
                let pos_b = queue.0.iter().position(|v| v == &rule[1]);
                if pos_b.is_none() {continue;}

                let (idx_a, idx_b) = (pos_a.unwrap(), pos_b.unwrap());

                if idx_a > idx_b
                {
                    sorted = false;
                    let tmp = queue.0[idx_b];
                    queue.0.remove(idx_b);
                    queue.0.push(tmp);
                }
            }
        }
    }
    let res_2: i32 = queues
        .into_iter()
        .filter(|(_, is_valid)| !*is_valid)
        .map(|(values, _)| values[(values.len()-1)/2])
        .sum();

    println!("part1: {}", res_1);
    println!("part2: {}", res_2);
}
