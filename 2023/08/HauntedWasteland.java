// https://adventofcode.com/2023/day/8

import java.io.FileInputStream;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Scanner;

public class HauntedWasteland {
    public static void main(String args[]) throws Exception {
        FileInputStream fis = new FileInputStream("./input.txt");
        Scanner sc = new Scanner(fis);

        final String START = "AAA";
        final String END = "ZZZ";

        int n_steps = 0;
        Map<String, String[]> network = new HashMap<String, String[]>();

        char[] turn_instructions = sc.nextLine().toCharArray();
        sc.nextLine();

        while (sc.hasNextLine()) {
            String line = sc.nextLine();
            String[] node = line.split("[^A-Z]+");
            network.put(node[0], new String[] { node[1], node[2] });
        }
        sc.close();

        String current_node = START;
        int n_turns = turn_instructions.length;

        while (!current_node.equals(END)) {
            char next_turn = turn_instructions[n_steps++ % n_turns];
            current_node = network.get(current_node)[(next_turn == 'R') ? 1 : 0];
        }

        System.out.println("number of steps required from " + START + " to " + END + ": " + n_steps);

        Map<String, Integer[]> loops = new HashMap<String, Integer[]>();
        for (String node : network.keySet()) {
            if (node.endsWith("A")) {
                current_node = node;
                n_steps = 0;
                int z_idx = -1;
                int loop_start = -1;
                int loop_length = -1;

                List<String> visited_nodes = new ArrayList<String>();
                visited_nodes.add(node);
                System.out.println(node);

                while (loop_length == -1) {
                    char next_turn = turn_instructions[n_steps++ % n_turns];
                    current_node = network.get(current_node)[(next_turn == 'R') ? 1 : 0];
                    if (current_node.endsWith("Z")) {
                        z_idx = n_steps;
                    }
                    if (n_steps % n_turns == 0) {
                        for (int i = 0; i < visited_nodes.size(); i++) {
                            if (visited_nodes.get(i).equals(current_node)) {
                                loop_start = i * n_turns;
                                loop_length = n_steps - loop_start;
                            }
                        }
                        System.out.print(" -> " + current_node);
                        visited_nodes.add(current_node);
                    }
                }
                System.out.println("\n");

                Integer[] loop = new Integer[] { loop_start, loop_length, z_idx };
                loops.put(node, loop);
            }
        }

        long[] lvals = new long[loops.size()];
        int i = 0;
        for (String key : loops.keySet()) {
            lvals[i++] = loops.get(key)[1];
            System.out.println(key + ": " + loops.get(key)[1]);
        }

        long n_steps_long = lcm(lvals);

        System.out.println("\nnumber of ghoststeps required to reach a state where all nodes end on Z: " + n_steps_long);
    }

    private static long lcm(long a, long b) {
        return a * (b / gcd(a, b));
    }

    private static long lcm(long[] input) {
        long result = input[0];
        for (int i = 1; i < input.length; i++)
            result = lcm(result, input[i]);
        return result;
    }

    private static long gcd(long a, long b) {
        while (b > 0) {
            long temp = b;
            b = a % b;
            a = temp;
        }
        return a;
    }
}
