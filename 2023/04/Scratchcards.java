// https://adventofcode.com/2023/day/4

import java.io.FileInputStream;
import java.util.Arrays;
import java.util.Scanner;

public class Scratchcards {
    public static void main(String args[]) throws Exception {
        FileInputStream fis = new FileInputStream("./input.txt");
        Scanner sc = new Scanner(fis);

        final int CARD_COUNT = 205;

        int sum_values = 0;
        int[] card_values = new int[CARD_COUNT];

        for (int i = 0; i < CARD_COUNT; i++) {
            String line = sc.nextLine();
            String[] card = line.split(":\\s+")[1].split("\\s+\\|\\s+");
            String[] card_l = card[0].split("\\s+");
            String[] card_r = card[1].split("\\s+");
            int card_val = 0;
            for (String l_str : card_l) {
                int l_val = Integer.parseInt(l_str);
                for (String r_str : card_r) {
                    int r_val = Integer.parseInt(r_str);
                    if (l_val == r_val) {
                        card_val++;
                    }
                }
            }
            sum_values += (card_val == 0) ? card_val : (int) Math.pow(2, card_val - 1);
            card_values[i] = card_val;
        }

        int n_cards = CARD_COUNT;
        int[] card_counts = new int[CARD_COUNT];
        Arrays.fill(card_counts, 1);

        for (int idx = 0; idx < CARD_COUNT; idx++) {
            for (int i = 1; i <= card_values[idx]; i++) {
                card_counts[idx + i] += card_counts[idx];
                n_cards += card_counts[idx];
            }
        }

        System.out.println("sum scratchcard points: " + sum_values);
        System.out.println("total number of cards : " + n_cards);
        sc.close();
    }
}
