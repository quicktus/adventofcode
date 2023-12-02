// https://adventofcode.com/2023/day/2

import java.io.FileInputStream;
import java.util.Arrays;
import java.util.Scanner;

public class CubeConundrum {
    public static void main(String args[]) throws Exception {
        FileInputStream fis = new FileInputStream("./input.txt");
        Scanner sc = new Scanner(fis);

        final String RED = "red";
        final String GREEN = "green";
        final String BLUE = "blue";

        int id_sum = 0;
        int powers_sum = 0;
        int game_id = 1;

        while (sc.hasNextLine()) {
            String line = sc.nextLine();
            String[] game = line.split(": ")[1].split("[;,] ");
            boolean game_is_possible = true;
            int[] min_set = { 0, 0, 0 }; // RGB

            for (String item : game) {
                String[] item_components = item.split(" ");
                int count = Integer.parseInt(item_components[0]);
                switch (item_components[1]) {
                    case RED:
                        game_is_possible = (game_is_possible && count <= 12);
                        min_set[0] = Math.max(min_set[0], count);
                        break;
                    case GREEN:
                        game_is_possible = (game_is_possible && count <= 13);
                        min_set[1] = Math.max(min_set[1], count);
                        break;
                    case BLUE:
                        game_is_possible = (game_is_possible && count <= 14);
                        min_set[2] = Math.max(min_set[2], count);
                }
            }

            System.out.println(line);
            System.out.println(game_is_possible ? "ok" : "err");
            System.out.println(game_id);
            System.out.println();

            id_sum += game_id++ * (game_is_possible ? 1 : 0);
            powers_sum += Arrays.stream(min_set).reduce(1, (a, b) -> a * b);
        }
        System.out.println("sum of possible game IDs: " + id_sum);
        System.out.println("sum of all game powers:   " + powers_sum);
        sc.close();
    }
}
