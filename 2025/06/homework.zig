const std = @import("std");

const input = @embedFile("input.txt");

const Op = enum { mul, add, unknown };

pub fn main() !void {
    var stdout_buffer: [1024]u8 = undefined;
    var stdout_writer = std.fs.File.stdout().writer(&stdout_buffer);
    const stdout = &stdout_writer.interface;

    var lines = std.mem.tokenizeScalar(u8, input, '\n');

    var arena = std.heap.ArenaAllocator.init(std.heap.page_allocator);
    defer arena.deinit();
    const allocator = arena.allocator();
    var array_list = std.array_list.Managed([]u8).init(allocator);

    while (lines.next()) |line| {
        const mutable_copy = try allocator.dupe(u8, line);  // allocate mutable copy!
        try array_list.append(mutable_copy);
    }

    var grid = try array_list.toOwnedSlice();

    grid = try transpose(grid);

    var result_1: u64 = 0;
    var result_2: u64 = 0;

    var offset: usize = 0;
    for (0..grid.len) |idx| {
        if (std.mem.allEqual(u8, grid[idx], ' ')) {
            const transposed = try transpose(grid[offset..idx]);
            result_1 += solve_math(transposed).?;
            result_2 += solve_math(grid[offset..idx]).?;
            offset = idx;
        }
    }
    // last block doesn't have a separating column
    const transposed = try transpose(grid[offset..]);
    result_1 += solve_math(transposed).?;
    result_2 += solve_math(grid[offset..]).?;

    try stdout.print("part 1: {d}\n", .{result_1});
    try stdout.print("part 2: {d}\n", .{result_2});

    try stdout.flush();
}

fn transpose(matrix: [][]u8) ![][]u8 {
    const n_rows = matrix.len;
    const n_cols = matrix[0].len;

    var arena = std.heap.ArenaAllocator.init(std.heap.page_allocator);
    const allocator = arena.allocator();
    var transposed: [][]u8 = try allocator.alloc([]u8, n_cols);

    for (transposed) |*row| {
        row.* = try allocator.alloc(u8, n_rows);
    }
    for (0..n_cols) |i| {
        for (0..n_rows) |j| {
            transposed[i][j] = matrix[j][i];
        }
    }
    return transposed;
}

fn fold(comptime T: type, list: std.array_list.Managed(T), initial: T, func: *const fn (T, T) T) T {
    var accumulator = initial;
    for (list.items) |element| {
        accumulator = func(accumulator, element);
    }
    return accumulator;
}

fn add(a: u64, b: u64) u64 { return a + b; }

fn mul(a: u64, b: u64) u64 { return a * b; }

pub fn solve_math(matrix: [][]u8) ?u64 {
    var op = Op.unknown;

    const allocator = std.heap.page_allocator;
    var values = std.array_list.Managed(u64).init(allocator);

    for (matrix) |row| {
        var tokens = std.mem.tokenizeScalar(u8, row, ' ');
        while (tokens.next()) |token| {
            var clean_token = token;
            clean_token = std.mem.trimEnd(u8, clean_token, "*");
            clean_token = std.mem.trimEnd(u8, clean_token, "+");
            if (token.len != clean_token.len) {
                op = switch (token[token.len - 1]) {
                    '*' => Op.mul,
                    '+' => Op.add,
                    else => Op.unknown
                };
            }
            if (clean_token.len != 0) {
                const parsed = std.fmt.parseInt(u64, clean_token, 10) catch unreachable;
                values.append(parsed) catch unreachable;
            }
        }
    }

    // std.debug.print("values:   ", .{});
    // for (values.items) |val| {
        // std.debug.print(" {d}", .{val});
    // }
    // std.debug.print("\n", .{});

    if (op == Op.add) {
        // std.debug.print("res (add): {d} \n\n", .{fold(u64, values, 0, add)});
        return fold(u64, values, 0, add);
    }
    if (op == Op.mul) {
        // std.debug.print("res (mul): {d} \n\n", .{fold(u64, values, 1, mul)});
        return fold(u64, values, 1, mul);
    }

    return null;
}
