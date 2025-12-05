const std = @import("std");
const DoublyLinkedList = std.DoublyLinkedList;

// const input = @embedFile("example.txt");
const input = @embedFile("input.txt");

const Range = struct {
    start: u64,
    end: u64,
    node: DoublyLinkedList.Node,
};

pub fn main() !void {
    var stdout_buffer: [1024]u8 = undefined;
    var stdout_writer = std.fs.File.stdout().writer(&stdout_buffer);
    const stdout = &stdout_writer.interface;

    var segments = std.mem.splitSequence(u8, input, "\n\n");
    var ranges = std.mem.tokenizeScalar(u8, segments.next().?, '\n');
    var available_ids = std.mem.tokenizeScalar(u8, segments.next().?, '\n');

    var gpa: std.heap.DebugAllocator(.{}) = .init;
    const allocator = gpa.allocator();
    var range_list: DoublyLinkedList = .{};

    while (ranges.next()) |new_range_str| {
        var new_range_tokens = std.mem.tokenizeScalar(u8, new_range_str, '-');
        const new_range_start_str = new_range_tokens.next().?;
        const new_range_end_str = new_range_tokens.next().?;
        const new_range_start = try std.fmt.parseInt(u64, new_range_start_str, 10);
        const new_range_end = try std.fmt.parseInt(u64, new_range_end_str, 10);

        var new_range = try allocator.create(Range);
        new_range.* = .{
            .start = new_range_start,
            .end = new_range_end,
            .node = .{},
        };
        range_list.append(&new_range.node);
    }

    var last_len: usize = 0;
    while (range_list.len() != last_len) {
        last_len = range_list.len();
        var node_a = range_list.last;
        while (node_a) |na| {
            var node_b = range_list.first;
            while (node_b) |nb| {
                if (na == nb) {
                    node_b = nb.next;
                    continue;
                }

                const range_a: *Range = @fieldParentPtr("node", na);
                const range_b: *Range = @fieldParentPtr("node", nb);

                // containment
                if (range_a.start <= range_b.start and range_a.end >= range_b.end) {
                    range_list.remove(nb);
                }

                // overlapping
                else if (range_a.start <= range_b.start and range_a.end <= range_b.end and range_a.end >= range_b.start) {
                    range_a.end = range_b.end;
                    range_list.remove(nb);
                }
                node_b = nb.next;
            }
            node_a = na.next;
        }
        node_a = range_list.last;
        while (node_a) |na| {
            var node_b = range_list.last;
            while (node_b) |nb| {
                if (na == nb) {
                    node_b = nb.prev;
                    continue;
                }

                const range_a: *Range = @fieldParentPtr("node", na);
                const range_b: *Range = @fieldParentPtr("node", nb);

                // containment
                if (range_a.start <= range_b.start and range_a.end >= range_b.end) {
                    range_list.remove(nb);
                }

                // overlapping
                else if (range_a.start <= range_b.start and range_a.end <= range_b.end and range_a.end >= range_b.start) {
                    range_a.end = range_b.end;
                    range_list.remove(nb);
                }
                node_b = nb.prev;
            }
            node_a = na.prev;
        }
    }

    var result_1: u64 = 0;
    while (available_ids.next()) |id_str| {
        const id = try std.fmt.parseInt(u64, id_str, 10);

        var node = range_list.first;
        while (node) |n| {
            const range: *Range = @fieldParentPtr("node", n);

            if (id >= range.start and id <= range.end) {
                result_1 += 1;
            }
            node = n.next;
        }
    }

    var result_2: u64 = 0;
    var node = range_list.first;
    while (node) |n| {
        const range: *Range = @fieldParentPtr("node", n);
        std.debug.print("range: {d} - {d}\n", .{range.start, range.end});
        result_2 += (range.end - range.start) + 1;
        node = n.next;
    }

    try stdout.print("part 1: {d}\n", .{result_1});
    try stdout.print("part 2: {d}\n", .{result_2});

    try stdout.flush();
}
