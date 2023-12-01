// https://adventofcode.com/2023/day/1

import java.io.FileInputStream;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

public class Trebuchet {
    public static void main(String args[]) throws Exception {
        FileInputStream fis = new FileInputStream("./input.txt");
        Scanner sc = new Scanner(fis);

        int sum = 0;
        String[] num_strings = { "zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine" };
        boolean part_two = true;
        while (sc.hasNextLine()) {
            String line = sc.nextLine();
            List<Integer> numbers = new ArrayList<Integer>();
            for (int i = 0; i < line.length(); i++) {
                if (line.charAt(i) >= '0' && line.charAt(i) <= '9') {
                    numbers.add(line.charAt(i) - '0');
                } else if (part_two) {
                    for (int j = 0; j < num_strings.length; j++) {
                        if (line.substring(i).toLowerCase().startsWith(num_strings[j])) {
                            numbers.add(j);
                            break;
                        }
                    }
                }
            }
            sum += numbers.get(0) * 10;
            sum += numbers.get(numbers.size() - 1);
        }
        System.out.println(sum);
        sc.close();
    }
}
