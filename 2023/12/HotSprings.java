import java.io.FileInputStream;
import java.util.Arrays;
import java.util.Scanner;

public class HotSprings {

    private static final char OPERATIONAL = '.';
    private static final char DAMAGED = '#';
    private static final char UNKNOWN = '?';

    private static long[] inserts;
    private static char[] spring_row;
    private static Long[][][] dp;

    public static void main(String args[]) throws Exception {
        FileInputStream fis = new FileInputStream("./input.txt");
        Scanner sc = new Scanner(fis);

        long sum_configs = 0;
        boolean unfold = true;

        while (sc.hasNextLine()) {
            String line = sc.nextLine();
            String[] record = line.split(" ");

            if (unfold) {
                record[0] = (record[0] + "?").repeat(5);
                record[0] = record[0].substring(0, record[0].length() - 1);
                record[1] = (record[1] + ",").repeat(5);
                record[1] = record[1].substring(0, record[1].length() - 1);
            }

            inserts = Arrays.stream(record[1].split(",")).mapToLong(Long::parseLong).toArray();
            spring_row = record[0].toCharArray();
            // spring_row current char, current insert idx, current nr of inserted springs
            dp = new Long[spring_row.length + 1][inserts.length + 1][spring_row.length + 1];

            sum_configs += arrange(0, 0, 0);
        }
        sc.close();

        System.out.println("sum configs: " + sum_configs);
    }

    private static long arrange(long spring_row_idx, long insert_idx, long n_inserted) {
        if (spring_row_idx == spring_row.length) {
            // end reached
            if (insert_idx == inserts.length && n_inserted == 0) {
                // all insertions done -> valid config
                return 1;
            } else if (insert_idx == inserts.length - 1 && inserts[(int)insert_idx] == n_inserted) {
                // all insertions done -> valid config
                return 1;
            } else {
                // could not insert all springs -> invalid config
                return 0;
            }
        }

        if (dp[(int)spring_row_idx][(int)insert_idx][(int)n_inserted] != null) {
            // cache hit -> don't re-calculate result
            return dp[(int)spring_row_idx][(int)insert_idx][(int)n_inserted];
        } else {
            // cache miss -> result needs to be calcualted
            long res = 0;
            for (char c : new char[] { DAMAGED, OPERATIONAL }) {
                char current_type = spring_row[(int)spring_row_idx];
                if (current_type == c || current_type == UNKNOWN) {
                    if (c == OPERATIONAL) {
                        if (n_inserted == 0) {
                            // not currenlty inserting -> move to next position
                            res += arrange(spring_row_idx + 1, insert_idx, 0);
                        } else if (n_inserted > 0 && insert_idx < inserts.length && inserts[(int)insert_idx] == n_inserted) {
                            // insertion successfull -> prepare next insertion
                            res += arrange(spring_row_idx + 1, insert_idx + 1, 0);
                        }
                    } else if (c == DAMAGED) {
                        // insertions are required -> do it
                        res += arrange(spring_row_idx + 1, insert_idx, n_inserted + 1);
                    }
                }
            }

            dp[(int) spring_row_idx][(int)insert_idx][(int)n_inserted] = res;
            return res;
        }
    }
}
