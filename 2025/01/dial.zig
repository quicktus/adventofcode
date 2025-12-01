const std = @import("std");
const input = @embedFile("input.txt");

const ParsingError = error{
    InvalidDirection,
};

pub fn main() !void {
    const stdout_file = std.io.getStdOut().writer();
    var bw = std.io.bufferedWriter(stdout_file);
    const stdout = bw.writer();

    var dial_pos: i32 = 50;
    var result_1: i32 = 0;
    var result_2: i32 = 0;

    var it = std.mem.tokenizeScalar(u8, input, '\n');
    while (it.next()) |token| {
        const direction: i32 = try switch (token[0]) {
            'L' => @as(i32, -1),
            'R' => @as(i32, 1),
            else => ParsingError.InvalidDirection,
        };

        const distance: i32 = try std.fmt.parseInt(i32, token[1..], 10);

        result_2 += part_2(dial_pos, direction, distance);
        result_1 += part_1(dial_pos, direction, distance);

        dial_pos = @mod((dial_pos + direction * distance), 100);
    }
    try stdout.print("part 1: {d}\n", .{result_1});
    try stdout.print("part 2: {d}\n", .{result_2});

    try bw.flush();
}

fn part_1(dial_pos: i32, direction: i32, distance: i32) i32 {
    return if (@mod((dial_pos + direction * distance), 100) == 0) 1 else 0;
}

fn part_2(dial_pos: i32, direction: i32, distance: i32) i32 {
    const full_rotations = @divFloor(distance, 100);
    const offset = direction * @rem(distance, 100);
    var res: i32 = full_rotations;
    if (dial_pos != 0 and (dial_pos + offset > 99 or dial_pos + offset < 1)){
        res += 1;
    }
    return res;
}

test "examples" {
    try std.testing.expect(part_1(50, -1, 68) == 0);
    try std.testing.expect(part_2(50, -1, 68) == 1);

    try std.testing.expect(part_1(82, -1, 30) == 0);
    try std.testing.expect(part_2(82, -1, 30) == 0);

    try std.testing.expect(part_1(52, 1, 48) == 1);
    try std.testing.expect(part_2(52, 1, 48) == 1);

    try std.testing.expect(part_1(0, -1, 5) == 0);
    try std.testing.expect(part_2(0, -1, 5) == 0);

    try std.testing.expect(part_1(95, 1, 60) == 0);
    try std.testing.expect(part_2(95, 1, 60) == 1);

    try std.testing.expect(part_1(55, -1, 55) == 1);
    try std.testing.expect(part_2(55, -1, 55) == 1);

    try std.testing.expect(part_1(0, -1, 1) == 0);
    try std.testing.expect(part_2(0, -1, 1) == 0);

    try std.testing.expect(part_1(99, -1, 99) == 1);
    try std.testing.expect(part_2(99, -1, 99) == 1);

    try std.testing.expect(part_1(0, 1, 14) == 0);
    try std.testing.expect(part_2(0, 1, 14) == 0);

    try std.testing.expect(part_1(14, -1, 82) == 0);
    try std.testing.expect(part_2(14, -1, 82) == 1);
}