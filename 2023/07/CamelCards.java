// https://adventofcode.com/2023/day/7

import java.io.FileInputStream;
import java.util.Collections;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.Scanner;

public class CamelCards {
    public static void main(String args[]) throws Exception {
        FileInputStream fis = new FileInputStream("./input.txt");
        Scanner sc = new Scanner(fis);

        final String CARD_ORDER = "23456789TJQKA";

        List<Hand> hands = new LinkedList<Hand>();

        while (sc.hasNextLine()) {
            String line = sc.nextLine();
            String[] hand_str = line.split(" ");
            int bid = Integer.parseInt(hand_str[1]);
            int[] cards = hand_str[0].chars().map(card -> CARD_ORDER.indexOf(card)).toArray();

            Hand hand = new Hand(bid, cards);

            boolean inserted = false;
            for (int i = 0; i < hands.size(); i++) {
                if (hands.get(i).stronger_than(hand)) {
                    hands.add(i, hand);
                    inserted = true;
                    break;
                }
            }
            if (!inserted) {
                hands.add(hand);
            }

        }
        sc.close();

        int total_winnings = 0;
        for (int i = 0; i < hands.size(); i++) {
            total_winnings += hands.get(i).get_bid() * (i + 1);
        }

        System.out.println("total winnings: " + total_winnings);
    }

}

class Hand {
    private final int bid;
    private final int[] cards;
    private final Map<Integer, Integer> card_counts;
    private int hand_type;

    private final int FIVE_OF_A_KIND = 7;
    private final int FOUR_OF_A_KIND = 6;
    private final int FULL_HOUSE = 5;
    private final int THREE_OF_A_KIND = 4;
    private final int TWO_PAIR = 3;
    private final int ONE_PAIR = 2;
    private final int HIGH_CARD = 1;

    public Hand(int bid, int[] cards) {
        this.bid = bid;
        this.cards = cards;
        this.card_counts = new HashMap<Integer, Integer>();
        for (int c : cards) {
            if (!card_counts.containsKey(c)) {
                card_counts.put(c, 1);
            } else {
                card_counts.put(c, card_counts.get(c) + 1);
            }
        }
        int max_count_same = Collections.max(card_counts.values());
        switch (max_count_same) {
            case 5:
                this.hand_type = FIVE_OF_A_KIND;
                break;
            case 4:
                this.hand_type = FOUR_OF_A_KIND;
                break;
            case 3:
                this.hand_type = (card_counts.values().contains(2))
                        ? FULL_HOUSE
                        : THREE_OF_A_KIND;
                break;
            case 2:
                int n_pairs = 0;
                for (int count : card_counts.values()) {
                    n_pairs += (count == 2) ? 1 : 0;
                }
                this.hand_type = (n_pairs == 2)
                        ? TWO_PAIR
                        : ONE_PAIR;
                break;
            case 1:
                this.hand_type = HIGH_CARD;
                break;
            default:
                assert false : "Unexpected hand type!";
        }

    }

    public int get_bid() {
        return bid;
    }

    public boolean stronger_than(Hand hand) {
        if (this.hand_type == hand.hand_type) {
            for (int i = 0; i < 5; i++) {
                if (this.cards[i] == hand.cards[i]) {
                    continue;
                } else {
                    return this.cards[i] > hand.cards[i];
                }
            }
            assert false : "Hands are equal!";
            return false;
        } else {
            return this.hand_type > hand.hand_type;
        }
    }
}
