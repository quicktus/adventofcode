const std = @import("std");

const input = @embedFile("input.txt");

pub fn main() !void {
    const stdout_file = std.io.getStdOut().writer();
    var bw = std.io.bufferedWriter(stdout_file);
    const stdout = bw.writer();

    var result_1: u32 = 0;
    var result_2: u64 = 0;

    var it = std.mem.tokenizeScalar(u8, input, '\n');
    while (it.next()) |range| {
        result_1 += part_1(range);
        result_2 += part_2(range);
    }
    try stdout.print("part 1: {d}\n", .{result_1});
    try stdout.print("part 2: {d}\n", .{result_2});

    try bw.flush();
}


fn part_1(line: []const u8) u32 {
    var res: u32 = 0;

    var idx_highest: u32 = 0;
    for (0..(line.len - 1)) |idx| {
        if (line[idx] > line[idx_highest]) {
            idx_highest = @intCast(idx);
        }
    }
    res += 10 * (line[idx_highest] - '0');

    var idx_next_highest: u32 = idx_highest + 1;
    for (idx_next_highest..line.len) |idx| {
        if (line[idx] > line[idx_next_highest]) {
            idx_next_highest = @intCast(idx);
        }
    }
    res += line[idx_next_highest] - '0';
    std.debug.print("res: {d}\n", .{res});

    return res;
}

fn part_2(line: []const u8) u64 {
    const n_digits = 12;
    var res: u64 = 0;
    var idx_highest: u32 = 0;

    for (0..n_digits) |digit_pos| {
        const start: u32 = idx_highest + 1;
        const end: u32 = @as(u32, @intCast(line.len)) - (n_digits - (@as(u32, @intCast(digit_pos)) + 1));

        for (start..end) |idx| {
            if (line[idx] > line[idx_highest]) {
                idx_highest = @intCast(idx);
            }
        }
        res += (line[idx_highest] - '0') * std.math.pow(u64, 10, 11 - @as(u64, @intCast(digit_pos)));
        idx_highest +=1;
    }

    // std.debug.print("res: {d}\n", .{res});
    return res;
}

test "examples" {
    try std.testing.expect(part_1("987654321111111") == (98));
    try std.testing.expect(part_1("811111111111119") == (89));
    try std.testing.expect(part_1("234234234234278") == (78));
    try std.testing.expect(part_1("818181911112111") == (92));

    try std.testing.expect(part_2("987654321111111") == (987654321111));
    try std.testing.expect(part_2("811111111111119") == (811111111119));
    try std.testing.expect(part_2("234234234234278") == (434234234278));
    try std.testing.expect(part_2("818181911112111") == (888911112111));
}
