const std = @import("std");

const input = @embedFile("input.txt");

const Point = struct { x: u32, y: u32 };
var grid: [][]u8 = undefined;
var visited: std.AutoHashMap(Point, u64) = undefined;

pub fn main() !void {
    var stdout_buffer: [1024]u8 = undefined;
    var stdout_writer = std.fs.File.stdout().writer(&stdout_buffer);
    const stdout = &stdout_writer.interface;

    var arena = std.heap.ArenaAllocator.init(std.heap.page_allocator);
    defer arena.deinit();
    const allocator = arena.allocator();
    var array_list = std.array_list.Managed([]u8).init(allocator);

    var lines = std.mem.tokenizeScalar(u8, input, '\n');
    while (lines.next()) |line| {
        const mutable_copy = try allocator.dupe(u8, line);  // allocate mutable copy!
        try array_list.append(mutable_copy);
    }

    grid = try array_list.toOwnedSlice();
    const start_pos = Point{.x = 1, .y = @intCast(std.mem.indexOfAny(u8, grid[0], "S").?)};

    visited = std.AutoHashMap(Point, u64).init(allocator);
    const result_1: u64 = sim_beam(start_pos);

    visited.clearAndFree();
    visited = std.AutoHashMap(Point, u64).init(allocator);

    const result_2: u64 = sim_quantum_beam(start_pos);

    try stdout.print("part 1: {d}\n", .{result_1});
    try stdout.print("part 2: {d}\n", .{result_2});

    try stdout.flush();
}

fn sim_beam(point: Point) u32 {
    if (point.x == grid.len) {
        return 0;
    }
    if (visited.contains(point)) {
        return 0;
    }

    visited.put(point, 0) catch unreachable;

    // std.debug.print("\n'{c}' @ {any}\n", .{grid[point.x][point.y], point});
    // for (0..grid.len) |x| {
    //     for (0..grid[0].len) |y| {
    //         if (x == point.x and y == point.y) {
    //             std.debug.print("o", .{});
    //         } else {
    //             std.debug.print("{c}", .{grid[x][y]});
    //         }
    //     }
    //     std.debug.print("\n", .{});
    // }
    // var stdin_buffer: [1024]u8 = undefined;
    // var stdin = std.fs.File.stdin().reader(&stdin_buffer);
    // var line_buffer: [1024]u8 = undefined;
    // var w: std.io.Writer = .fixed(&line_buffer);
    // _ = stdin.interface.streamDelimiterLimit(&w, @intCast('\n'), .unlimited) catch undefined;

    if (grid[point.x][point.y] == '.') {
        return sim_beam(Point{.x = point.x + 1, .y = point.y});
    }
    else if (grid[point.x][point.y] == '^') {
        return 1 + sim_beam(Point{.x = point.x, .y = point.y - 1}) + sim_beam(Point{.x = point.x, .y = point.y + 1});
    }
    else {
        @panic("unexpected object");
    }
}

fn sim_quantum_beam(point: Point) u64 {
    if (point.x == grid.len) {
        return 1;
    }
    if (visited.contains(point)) {
        return visited.get(point).?;
    }

    var res: u64 = 0;
    if (grid[point.x][point.y] == '.') {
        res =  sim_quantum_beam(Point{.x = point.x + 1, .y = point.y});
    }
    else if (grid[point.x][point.y] == '^') {
        res =  sim_quantum_beam(Point{.x = point.x, .y = point.y - 1}) + sim_quantum_beam(Point{.x = point.x, .y = point.y + 1});
    }
    else {
        @panic("unexpected object");
    }

    visited.put(point, res) catch unreachable;

    return res;
}
