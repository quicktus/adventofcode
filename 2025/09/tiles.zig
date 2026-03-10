const std = @import("std");

const input = @embedFile("input.txt");

const Point = struct { x: u32, y: u32 };

pub fn main() !void {
    var stdout_buffer: [1024]u8 = undefined;
    var stdout_writer = std.fs.File.stdout().writer(&stdout_buffer);
    const stdout = &stdout_writer.interface;

    var arena = std.heap.ArenaAllocator.init(std.heap.page_allocator);
    defer arena.deinit();
    const allocator = arena.allocator();
    var points = std.array_list.Managed(Point).init(allocator);

    var lines = std.mem.tokenizeScalar(u8, input, '\n');
    while (lines.next()) |line| {
        var numbers = std.mem.splitScalar(u8, line, ',');
        const new_point = Point{
            .x = try std.fmt.parseInt(u32, numbers.next().?, 10),
            .y = try std.fmt.parseInt(u32, numbers.next().?, 10)
        };
        try points.append(new_point);
    }

    var max_area: u64 = 0;
    for (points.items) |point_a| {
        for (points.items) |point_b| {
            const dist_x: u64 = @max(point_a.x, point_b.x) - @min(point_a.x, point_b.x) + 1;
            const dist_y: u64 = @max(point_a.y, point_b.y) - @min(point_a.y, point_b.y) + 1;
            std.debug.print("{d} {d}\n", .{dist_x, dist_y});

            const ov = @mulWithOverflow(dist_x, dist_y);
            if (ov[1] != 0) return error.Overflow;

            max_area = @max(ov[0], max_area);
        }
    }
    // const result_1: u64 = sim_beam(start_pos);
    // const result_2: u64 = sim_quantum_beam(start_pos);

    try stdout.print("part 1: {d}\n", .{max_area});
    // try stdout.print("part 2: {d}\n", .{result_2});

    try stdout.flush();
}
