// https://adventofcode.com/2023/day/3

import java.io.FileInputStream;
import java.util.Arrays;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Scanner;

public class GearRatios {
    public static void main(String args[]) throws Exception {
        FileInputStream fis = new FileInputStream("./input.txt");
        Scanner sc = new Scanner(fis);

        final int MATRIX_SIZE = 142;

        int sum_part_numbers = 0;
        char[][] matrix = new char[MATRIX_SIZE][MATRIX_SIZE];
        HashMap<Integer, List<Integer>> gears = new HashMap<>();
        for (int i = 1; i < MATRIX_SIZE - 1; i++) {
            matrix[i] = ("." + sc.nextLine() + ".").toCharArray();
        }
        Arrays.fill(matrix[0], '.');
        Arrays.fill(matrix[MATRIX_SIZE - 1], '.');
        sc.close();

        for (int i = 0; i < MATRIX_SIZE; i++) {
            for (int j = 0; j < MATRIX_SIZE; j++) {
                if (is_number((matrix[i][j]))) {
                    for (int k = -1; k <= 1; k++) {
                        for (int m = -1; m <= 1; m++) {
                            if (m == 0 && k == 0) {
                                continue;
                            }
                            if (is_symbol(matrix[i + k][j + m])) {
                                int n = 0;
                                String part_number = "";
                                while (is_number(matrix[i][j + --n]));
                                while (is_number(matrix[i][j + ++n])) {
                                    part_number += matrix[i][j + n];
                                }
                                int num = Integer.parseInt(part_number);
                                sum_part_numbers += num;
                                if (matrix[i + k][j + m] == '*') {
                                    int flat_idx = MATRIX_SIZE * (i + k) + (j + m);
                                    if (!gears.containsKey(flat_idx)) {
                                        gears.put(flat_idx, new LinkedList<Integer>());
                                    }
                                    gears.get(flat_idx).add(num);
                                }
                                j += n;
                                k = 2; // breaks k-loop
                                break; // breaks m-loop
                            }
                        }
                    }
                }
            }
        }
        System.out.println("sum of all part numbers: " + sum_part_numbers);

        int sum_gear_ratios = gears.values()
                .stream()
                .filter(gear_list -> gear_list.size() > 1)
                .mapToInt(gear_list -> gear_list.stream().reduce(1, (a, b) -> a * b))
                .sum();

        System.out.println("sum of all gear ratios : " + sum_gear_ratios);
    }

    private static boolean is_symbol(char c) {
        return !(is_number(c) || c == '.');
    }

    private static boolean is_number(char c) {
        return c >= '0' && c <= '9';
    }
}
