// https://adventofcode.com/2023/day/6

import java.io.FileInputStream;
import java.util.Arrays;
import java.util.Scanner;

public class WaitForIt {
    public static void main(String args[]) throws Exception {
        FileInputStream fis = new FileInputStream("./input.txt");
        Scanner sc = new Scanner(fis);
        int[] durations = Arrays.stream(sc.nextLine().split(":\\s+")[1].split("\\s+")).mapToInt(Integer::parseInt).toArray();
        int[] distances = Arrays.stream(sc.nextLine().split(":\\s+")[1].split("\\s+")).mapToInt(Integer::parseInt).toArray();
        sc.close();

        int prod_winning_moves = 1;

        for (int idx_race = 0; idx_race < durations.length; idx_race++) {
            int race_winning_moves = 0;
            for (int charge_time = 1; charge_time < durations[idx_race]; charge_time++) {
                if (distances[idx_race] < charge_time * (durations[idx_race] - charge_time)) {
                    race_winning_moves++;
                }
            }
            prod_winning_moves *= race_winning_moves;
        }

        System.out.println("product of the total numbers of winning moves per race: " + prod_winning_moves);

        long big_duration = Long.parseLong(Arrays.toString(durations).replaceAll("\\D", ""));
        long big_distance = Long.parseLong(Arrays.toString(distances).replaceAll("\\D", ""));

        int big_race_winning_moves = 0;
        for (int charge_time = 1; charge_time < big_duration; charge_time++) {
            big_race_winning_moves += (big_distance < charge_time * (big_duration - charge_time)) ? 1 : 0;
        }

        System.out.println("number of winning moves in the big race: " + big_race_winning_moves);
    }
}
