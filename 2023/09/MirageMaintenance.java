// https://adventofcode.com/2023/day/9

import java.io.FileInputStream;
import java.util.Arrays;
import java.util.Scanner;

public class MirageMaintenance {

    private final static int UNKNOWN = Integer.MAX_VALUE;

    public static void main(String args[]) throws Exception {
        FileInputStream fis = new FileInputStream("./input.txt");
        Scanner sc = new Scanner(fis);

        int extrapolated_future_value_sum = 0;
        int extrapolated_historic_value_sum = 0;

        while (sc.hasNextLine()) {
            String[] line_str = (UNKNOWN + " " + sc.nextLine() + " " + UNKNOWN).split(" ");
            int[] line = Arrays.stream(line_str).mapToInt(Integer::parseInt).toArray();
            int[][] dt = diff_triangle(line);
            extrapolated_future_value_sum += dt[0][dt.length - 1];
            extrapolated_historic_value_sum += dt[0][0];
        }
        sc.close();

        System.out.println("sum of extrapolated future values: " + extrapolated_future_value_sum);
        System.out.println("sum of extrapolated historic values: " + extrapolated_historic_value_sum);
    }

    private static int[][] diff_triangle(int[] bottom_row) {
        int[][] triangle = new int[bottom_row.length][];
        triangle[0] = bottom_row;

        for (int r = 1; r < triangle.length; r++) {
            triangle[r] = new int[triangle.length - r];
            Arrays.fill(triangle[r], UNKNOWN);
        }

        triangle[triangle.length - 3][1] = diff_triangle(triangle, triangle.length - 3, 1);

        triangle[triangle.length - 1][0] = 0;
        triangle[triangle.length - 2][0] = 0;
        triangle[triangle.length - 2][1] = 0;

        triangle[0][triangle.length - 1] = extrapolate_forward(triangle, 0, triangle.length - 1);
        triangle[0][0] = extrapolate_backwards(triangle, 0, 0);

        for (int row = 0; row < triangle.length; row++) {
            System.out.print("   ".repeat(row));
            for (int col = 0; col < triangle[row].length; col++) {
                if (Math.abs(triangle[row][col]) < 1_000) {
                    System.out.print(String.format("% 5d ", triangle[row][col]));
                } else if (Math.abs(triangle[row][col]) < 1_000_000) {
                    System.out.print(String.format("% 5dk", (int) (triangle[row][col] / 1_000)));
                } else {
                    System.out.print(String.format("% 5dM", (int) (triangle[row][col] / 1_000_000)));
                }
            }
            System.out.println();
        }
        System.out.println();

        return triangle;
    }

    private static int diff_triangle(int[][] triangle, int row, int col) {
        if (triangle[row - 1][col] == UNKNOWN) {
            triangle[row - 1][col] = diff_triangle(triangle, row - 1, col);
        }
        if (triangle[row - 1][col + 1] == UNKNOWN) {
            triangle[row - 1][col + 1] = diff_triangle(triangle, row - 1, col + 1);
        }
        return triangle[row - 1][col + 1] - triangle[row - 1][col];
    }

    private static int extrapolate_forward(int[][] triangle, int row, int col) {
        if (triangle[row + 1][col - 1] == UNKNOWN) {
            triangle[row + 1][col - 1] = extrapolate_forward(triangle, row + 1, col - 1);
        }
        return triangle[row][col - 1] + triangle[row + 1][col - 1];
    }

    private static int extrapolate_backwards(int[][] triangle, int row, int col) {
        if (triangle[row + 1][col] == UNKNOWN) {
            triangle[row + 1][col] = extrapolate_backwards(triangle, row + 1, col);
        }
        return triangle[row][col + 1] - triangle[row + 1][col];
    }
}
