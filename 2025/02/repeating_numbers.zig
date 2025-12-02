const std = @import("std");
const pow = std.math.pow;
const ArrayList = std.ArrayList;

const input = @embedFile("input.txt");

pub fn main() !void {
    const stdout_file = std.io.getStdOut().writer();
    var bw = std.io.bufferedWriter(stdout_file);
    const stdout = bw.writer();

    var result_1: u64 = 0;
    var result_2: u64 = 0;

    var it = std.mem.tokenizeScalar(u8, input, ',');
    while (it.next()) |range| {
        var nums = std.mem.tokenizeScalar(u8, range, '-');
        const start = try std.fmt.parseInt(u64, nums.next().?, 10);
        const end = try std.fmt.parseInt(u64, nums.next().?, 10);

        result_1 += part_1(start, end);
        result_2 += part_2(start, end);
    }
    try stdout.print("part 1: {d}\n", .{result_1});
    try stdout.print("part 2: {d}\n", .{result_2});

    try bw.flush();
}

fn n_digits(num: u64) u64 {
    if (num == 0) {return 0;}
    return @as(u64, @intFromFloat(@log10(@as(f64, @floatFromInt(num))))) + 1;
}

fn part_1(start: u64, end: u64) u64 {
    const len = n_digits(start);
    const segment_len: u64 = @divFloor(len, 2);
    var sum: u64 = 0;
    var segment: u64 = start / pow(u64, 10, (len - segment_len));

    while (true) {
        const schnapszahl = segment + (segment * pow(u64, 10, n_digits(segment)));
        segment += 1;
        if (schnapszahl < start) {
            continue;
        }
        if (schnapszahl > end) {
            break;
        }
        // std.debug.print("schnapszahl: {d}\n", .{schnapszahl});
        sum += schnapszahl;
    }

    return sum;
}

fn part_2(start: u64, end: u64) u64 {
    const len_start = n_digits(start);
    const len_end = n_digits(end);
    const allocator = std.heap.page_allocator;
    var schnapszahlen = ArrayList(u64).init(allocator);

    const max_segment_len = @divFloor(n_digits(end), 2);
    const min_segment: u64 = 1;
    var current_segment = min_segment;
    while (n_digits(current_segment) <= max_segment_len) {
        var schnapszahl = current_segment;

        // segment must be repeated at least once
        schnapszahl = schnapszahl * pow(u64, 10, n_digits(current_segment)) + current_segment;

        // number must not be shorter than start
        while (n_digits(schnapszahl) < len_start) {
            schnapszahl = schnapszahl * pow(u64, 10, n_digits(current_segment)) + current_segment;
        }
        // number must not be longer than end
        while (n_digits(schnapszahl) <= len_end) {
            if (schnapszahl >= start and schnapszahl <= end) {
                var is_duplicate = false;
                for (schnapszahlen.items) |item| {
                    if (schnapszahl == item) {
                        is_duplicate = true;
                        break;
                    }
                }
                if (!is_duplicate) {
                    schnapszahlen.append(schnapszahl) catch {};
                    // std.debug.print("schnapszahl: {d}\n", .{schnapszahl});
                }
            }
            schnapszahl = schnapszahl * pow(u64, 10, n_digits(current_segment)) + current_segment;
        }

        current_segment += 1;
    }


    var sum: u64 = 0;
    while (schnapszahlen.pop()) |item| {
        sum += item;
    }
    schnapszahlen.deinit();
    return sum;
}

test "examples" {
    try std.testing.expect(part_1(11, 22) == (11 + 22));
    try std.testing.expect(part_2(11, 22) == (11 + 22));

    try std.testing.expect(part_1(95, 115) == 99);
    try std.testing.expect(part_2(95, 115) == (99 + 111));

    try std.testing.expect(part_1(998, 1012) == 1010);
    try std.testing.expect(part_2(998, 1012) == (999 + 1010));

    try std.testing.expect(part_1(1188511880, 1188511890) == 1188511885);
    try std.testing.expect(part_2(1188511880, 1188511890) == 1188511885);

    try std.testing.expect(part_1(222220, 222224) == 222222);
    try std.testing.expect(part_2(222220, 222224) == 222222);

    try std.testing.expect(part_1(1698522, 1698528) == 0);
    try std.testing.expect(part_2(1698522, 1698528) == 0);

    try std.testing.expect(part_1(446443, 446449) == 446446);
    try std.testing.expect(part_2(446443, 446449) == 446446);

    try std.testing.expect(part_1(38593856, 38593862) == 38593859);
    try std.testing.expect(part_2(38593856, 38593862) == 38593859);

    try std.testing.expect(part_1(565653, 565659) == 0);
    try std.testing.expect(part_2(565653, 565659) == 565656);

    try std.testing.expect(part_1(824824821, 824824827) == 0);
    try std.testing.expect(part_2(824824821, 824824827) == 824824824);

    try std.testing.expect(part_1(2121212118, 2121212124) == 0);
    try std.testing.expect(part_2(2121212118, 2121212124) == 2121212121);
}

test "custom" {
    try std.testing.expect(part_1(1, 22) == (11 + 22));
    try std.testing.expect(part_2(1, 22) == (11 + 22));
}
