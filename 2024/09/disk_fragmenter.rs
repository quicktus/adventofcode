use std::fs::File;
use std::io::Read;
use std::path::Path;

const EMPTY: i32 = -1;

#[derive(Clone)]
struct Segment {
    id: u16,
    size_data: u16,
    size_padding: u16,
}

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

    let disk_map: Vec<u8> = input
    .trim()
    .chars()
    .map(|c| (c as u8) - 48)
    .collect();

    let mut blocks: Vec<i32> = Vec::new();

    for dm_idx in 0..disk_map.len() {
        for _ in 0..disk_map[dm_idx] {
            let idx_block: i32 = if dm_idx % 2 == 0 {(dm_idx / 2) as i32} else {EMPTY};
            blocks.push(idx_block);
        }
    }

    while blocks.contains(&EMPTY) {
        for i in 0..blocks.len() {
            if *blocks.get(i).unwrap() == EMPTY {
                let idx_last = blocks.len() - 1;
                blocks.swap(i, idx_last);
                blocks.pop();
                break;
            }
        }
    }

    let checksum_1: i64 = blocks
        .into_iter()
        .enumerate()
        .map(|(pos,file_id)| pos as i64 * file_id as i64)
        .sum();

    println!("part 1: {}", checksum_1);


    let mut segments: Vec<Segment> = input
        .trim()
        .chars()
        .map(|c| (c as u16) - 48)
        .collect::<Vec<u16>>()
        .chunks(2)
        .enumerate()
        .map(|(i, chunk)| Segment {
            id: i as u16,
            size_data: chunk[0],
            size_padding: *chunk.get(1).unwrap_or(&0)
        })
        .collect();

    for id in (0..segments.len()).rev() {
        let idx = (0..segments.len())
            .find(|&i| segments[i].id == id as u16)
            .unwrap();
        let mut current = segments[idx].clone();

        // for i in 0..segments.len() {
        //     if i == idx {print!("\x1b[93m");};
        //     for _ in 0..segments[i].size_data {
        //         print!("{}", segments[i].id);
        //     }
        //     for _ in 0..segments[i].size_padding {
        //         print!(".");
        //     };
        //     if i == idx {print!("\x1b[0m");};
        // }
        // println!();

        for lower_idx in 0..idx {
            if segments[lower_idx].size_padding >= current.size_data {
                segments[idx-1].size_padding += current.size_padding + current.size_data;
                current.size_padding = segments[lower_idx].size_padding - current.size_data;
                segments[lower_idx].size_padding = 0;
                segments.remove(idx);
                segments.insert(lower_idx+1, current);
                break;
            }
        }
    }

    let mut checksum_2: i64 = 0;
    let mut block_idx: i64 = 0;
    for seg in segments {
        for _ in 0..seg.size_data {
            checksum_2 += block_idx * seg.id as i64;
            block_idx += 1;
        }
        block_idx +=  seg.size_padding as i64;
    }

    println!("part 2: {}", checksum_2);
}
