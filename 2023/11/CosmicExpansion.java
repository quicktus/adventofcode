// https://adventofcode.com/2023/day/11

import java.io.FileInputStream;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Scanner;

public class CosmicExpansion {
    public static void main(String args[]) throws Exception {
        FileInputStream fis = new FileInputStream("./input.txt");
        Scanner sc = new Scanner(fis);

        int MATRIX_SIZE = 140;

        List<Long[]> galaxy_2 = new ArrayList<Long[]>();
        List<Long[]> galaxy_1M = new ArrayList<Long[]>();

        for (int y = 0; sc.hasNextLine(); y++) {
            String line = sc.nextLine();
            for (int x = line.indexOf('#', 0); x != -1; x = line.indexOf('#', ++x)) {
                galaxy_2.add(new Long[] { (long) x, (long) y });
                galaxy_1M.add(new Long[] { (long) x, (long) y });
            }
        }
        sc.close();

        HashMap<Integer, List<Long[]>> galaxies_map = new HashMap<>();
        galaxies_map.put(1, galaxy_2);
        galaxies_map.put(999_999, galaxy_1M);

        for (int expansion_factor : galaxies_map.keySet()) {
            for (int dim = 0; dim < 2; dim++) {
                for (long idx = 0; idx < MATRIX_SIZE * (expansion_factor + 1); idx++) {
                    boolean idx_empty = true;
                    for (Long[] g : galaxies_map.get(expansion_factor)) {
                        if (g[dim] == idx) {
                            idx_empty = false;
                            break;
                        }
                    }
                    if (idx_empty) {
                        for (Long[] g : galaxies_map.get(expansion_factor)) {
                            if (g[dim] > idx) {
                                g[dim] += expansion_factor;
                            }
                        }
                        idx += expansion_factor;
                    }
                }
            }

            long sum_dist = 0;

            for (int a = 0; a < galaxies_map.get(expansion_factor).size(); a++) {
                for (int b = a + 1; b < galaxies_map.get(expansion_factor).size(); b++) {
                    long ax = galaxies_map.get(expansion_factor).get(a)[0];
                    long ay = galaxies_map.get(expansion_factor).get(a)[1];
                    long bx = galaxies_map.get(expansion_factor).get(b)[0];
                    long by = galaxies_map.get(expansion_factor).get(b)[1];

                    sum_dist += Math.abs(ax - bx);
                    sum_dist += Math.abs(ay - by);
                }
            }

            System.out.println(
                    "sum of distances between all pairs of galaxies (expansion=" + (expansion_factor + 1) + "): "
                            + sum_dist);
        }
    }
}
