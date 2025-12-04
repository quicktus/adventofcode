const std = @import("std");
const ArrayList = std.ArrayList;

const input = @embedFile("input.txt");

pub fn main() !void {
    const stdout_file = std.io.getStdOut().writer();
    var bw = std.io.bufferedWriter(stdout_file);
    const stdout = bw.writer();

    var it = std.mem.tokenizeScalar(u8, input, '\n');

    const allocator = std.heap.page_allocator;
    var array_list = ArrayList([]u8).init(allocator);

    while (it.next()) |range| {
        const mutable_copy = try allocator.dupe(u8, range);  // allocate mutable copy!
        try array_list.append(mutable_copy);
    }

    const grid = try array_list.toOwnedSlice();

    const result_1: u32 = part_1(grid);
    const result_2: u32 = part_2(grid);

    try stdout.print("part 1: {d}\n", .{result_1});
    try stdout.print("part 2: {d}\n", .{result_2});

    try bw.flush();
}

fn part_1(grid: [][]u8) u32 {
    const max_n_adjacent_rolls: u8 = 3;  // fewer than four rolls of paper in the eight adjacent positions
    const offsets = [3]i8{-1, 0, 1};
    var sum: u32 = 0;

    for (0..grid.len) |row| {
        for (0..grid[0].len) |col| {
            if (grid[row][col] != '@') {
                    // std.debug.print(".", .{});
                    continue;
                }
            var n_adjacent_rolls: u8 = 0;

            for (offsets) |offset_r| {
                const r = @as(i16, @intCast(row)) + offset_r;
                for (offsets) |offset_c| {
                    const c = @as(i16, @intCast(col)) + offset_c;

                    if (r == row and c == col) {continue;}
                    if (r < 0) { continue;}
                    if (r >= grid.len) { continue;}
                    if (c < 0) { continue;}
                    if (c >= grid[0].len) { continue;}

                    if (grid[@as(usize, @intCast(r))][@as(usize, @intCast(c))] == '@') {
                        n_adjacent_rolls += 1;
                    }
                }
            }
            if (n_adjacent_rolls <= max_n_adjacent_rolls) {
                sum += 1;
                // std.debug.print("x", .{});
            }
            else {
                // std.debug.print("@", .{});
            }
        }
        // std.debug.print("\n", .{});
    }
    return sum;
}

fn part_2(grid: [][]u8) u32 {
    const max_n_adjacent_rolls: u8 = 3;  // fewer than four rolls of paper in the eight adjacent positions
    const offsets = [3]i8{-1, 0, 1};

    var sum: i32 = 0;
    var prev_sum: i32 = -1;

    while (prev_sum < sum) {
        prev_sum = sum;

        for (0..grid.len) |row| {
            for (0..grid[0].len) |col| {
                if (grid[row][col] != '@') {
                        // std.debug.print(".", .{});
                        continue;
                    }
                var n_adjacent_rolls: u8 = 0;

                for (offsets) |offset_r| {
                    const r = @as(i16, @intCast(row)) + offset_r;
                    for (offsets) |offset_c| {
                        const c = @as(i16, @intCast(col)) + offset_c;

                        if (r == row and c == col) {continue;}
                        if (r < 0) { continue;}
                        if (r >= grid.len) { continue;}
                        if (c < 0) { continue;}
                        if (c >= grid[0].len) { continue;}

                        if (grid[@as(usize, @intCast(r))][@as(usize, @intCast(c))] == '@') {
                            n_adjacent_rolls += 1;
                        }
                    }
                }
                if (n_adjacent_rolls <= max_n_adjacent_rolls) {
                    sum += 1;
                    grid[row][col] = 'x';
                }
                else {
                    // std.debug.print("@", .{});
                }
            }
            // std.debug.print("\n", .{});
        }
    }
    return @as(u32, @intCast(sum));
}

test "examples" {
    // create a *mutable* array
    var line0 = "..@@.@@@@.".*;
    var line1 = "@@@.@.@.@@".*;
    var line2 = "@@@@@.@.@@".*;
    var line3 = "@.@@@@..@.".*;
    var line4 = "@@.@@@@.@@".*;
    var line5 = ".@@@@@@@.@".*;
    var line6 = ".@.@.@.@@@".*;
    var line7 = "@.@@@.@@@@".*;
    var line8 = ".@@@@@@@@.".*;
    var line9 = "@.@.@@@.@.".*;

    var grid = [_][]u8{&line0, &line1, &line2, &line3, &line4, &line5, &line6, &line7, &line8, &line9,};

    try std.testing.expect(part_1(&grid) == 13);
    try std.testing.expect(part_2(&grid) == 43);
}
