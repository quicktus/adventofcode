// https://adventofcode.com/2023/day/5

import java.io.FileInputStream;
import java.util.Arrays;
import java.util.Scanner;
import java.util.LinkedList;
import java.util.List;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.atomic.AtomicLong;

public class Fertilizer {
    private static final int DESTINATION_RANGE_START = 0;
    private static final int SOURCE_RANGE_START = 1;
    private static final int RANGE_LENGTH = 2;
    private static final List<List<Long[]>> maps = new LinkedList<>();

    public static void main(String args[]) throws Exception {
        FileInputStream fis = new FileInputStream("./input.txt");
        Scanner sc = new Scanner(fis);

        long[] seeds = Arrays.stream(sc.nextLine().split(": ")[1].split(" ")).mapToLong(Long::parseLong).toArray();
        sc.nextLine();
        for (int i = 0; i < 7; i++) {
            maps.add(getSection(sc));
        }
        sc.close();

        long min_location_1 = Long.MAX_VALUE;
        for (long val : seeds) {
            val = lookup(val);
            min_location_1 = Math.min(min_location_1, val);
        }

        System.out.println("lowest location number (seed-list 1): " + min_location_1);

        AtomicLong min_location_2 = new AtomicLong(Long.MAX_VALUE);
        List<Runnable> task_list = new LinkedList<Runnable>();
        for (int i = 0; i < seeds.length; i += 2) {
            long start = seeds[i];
            long range = seeds[i + 1];
            long end = start + range;
            long mid = start + range / 2;
            task_list.add(new Task(start, mid, min_location_2));
            task_list.add(new Task(mid, end, min_location_2));
        }

        ExecutorService pool = Executors.newFixedThreadPool(20);
        for (Runnable task : task_list) {
            pool.execute(task);
        }

        pool.shutdown();
        pool.awaitTermination(600, java.util.concurrent.TimeUnit.SECONDS);

        System.out.println("\rlowest location number (seed-list 2): " + min_location_2.get());
    }

    public static long lookup(long val) {
        for (List<Long[]> map : maps) {
            for (Long[] mapping : map) {
                if (val >= mapping[SOURCE_RANGE_START]) {
                    long offset = val - mapping[SOURCE_RANGE_START];
                    if (offset < mapping[RANGE_LENGTH]) {
                        val = mapping[DESTINATION_RANGE_START] + offset;
                        break;
                    }
                }
            }
        }
        return val;
    }

    private static List<Long[]> getSection(Scanner sc) {
        List<Long[]> triples = new LinkedList<Long[]>();

        while (sc.hasNextLine()) {
            String line = sc.nextLine();
            if (!line.contains(" ")) {
                break;
            }
            if (line.contains(":")) {
                continue;
            }
            String[] items = line.split(" ");
            Long[] triple = new Long[3];
            for (int i = 0; i < 3; i++) {
                triple[i] = Long.parseLong(items[i]);
            }
            triples.add(triple);
        }

        return triples;
    }
}

class Task implements Runnable {

    private final long start;
    private final long end;
    private final AtomicLong min_location;
    private long local_min_location;

    public Task(long start, long end, AtomicLong min_location) {
        this.start = start;
        this.end = end;
        this.min_location = min_location;
        this.local_min_location = Long.MAX_VALUE;
    }

    public void run() {
        for (long i = start; i < end; i++) {
            local_min_location = Math.min(Fertilizer.lookup(i), local_min_location);
        }
        min_location.updateAndGet(value -> Math.min(value, local_min_location));
        System.out.print(".");
    }
}
