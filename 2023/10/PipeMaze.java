// https://adventofcode.com/2023/day/10

import java.io.FileInputStream;
import java.util.Arrays;
import java.util.Scanner;

public class PipeMaze {
    public static void main(String args[]) throws Exception {
        FileInputStream fis = new FileInputStream("./input.txt");
        Scanner sc = new Scanner(fis);

        final char PIPE_NORTH_SOUTH = '|';
        final char PIPE_EAST_WEST = '-';
        final char PIPE_NORTH_EAST = 'L';
        final char PIPE_NORTH_WEST = 'J';
        final char PIPE_SOUTH_WEST = '7';
        final char PIPE_SOUTH_EAST = 'F';
        final char START = 'S'; // starting position; pipe unknown

        final int MAP_SIZE = 140;
        final int UNREACHABLE = Integer.MAX_VALUE;

        int x_start = 0;
        int y_start = 0;
        char[][] map_pipes = new char[MAP_SIZE][];

        for (int i = MAP_SIZE - 1; i >= 0; i--) {
            String row = sc.nextLine();
            map_pipes[i] = row.toCharArray();

            if (row.indexOf(START) != -1) {
                x_start = row.indexOf(START);
                y_start = i;
            }
        }
        sc.close();

        int[][] map_distances = new int[MAP_SIZE][MAP_SIZE];

        for (int[] row : map_distances) {
            Arrays.fill(row, UNREACHABLE);
        }

        char[] directions = { 'N', 'E', 'S', 'W' };

        for (char d : directions) {
            boolean loop_found = false;

            for (int i = 0; (i == 0) || (i == 1 && loop_found); i++) {
                int x = x_start;
                int y = y_start;
                boolean end_found = false;
                int n_steps = 0;
                char came_from = d;

                while (!end_found) {
                    if (map_pipes[y][x] == START) {
                        if (n_steps != 0) {
                            loop_found = true;
                            break;
                        }
                        // do initial step
                        switch (came_from) {
                            case 'N':
                                y--;
                                break;
                            case 'E':
                                x--;
                                break;
                            case 'S':
                                y++;
                                break;
                            case 'W':
                                x++;
                                break;
                        }
                        n_steps++;
                    }

                    if (loop_found) {
                        map_distances[y][x] = Math.min(map_distances[y][x], n_steps);
                    }

                    switch (came_from) {
                        case 'N':
                            switch (map_pipes[y][x]) {
                                case PIPE_NORTH_SOUTH:
                                    y--;
                                    came_from = 'N';
                                    break;
                                case PIPE_NORTH_EAST:
                                    x++;
                                    came_from = 'W';
                                    break;
                                case PIPE_NORTH_WEST:
                                    x--;
                                    came_from = 'E';
                                    break;
                                default:
                                    end_found = true;
                            }
                            break;
                        case 'E':
                            switch (map_pipes[y][x]) {
                                case PIPE_EAST_WEST:
                                    x--;
                                    came_from = 'E';
                                    break;
                                case PIPE_NORTH_EAST:
                                    y++;
                                    came_from = 'S';
                                    break;
                                case PIPE_SOUTH_EAST:
                                    y--;
                                    came_from = 'N';
                                    break;
                                default:
                                    end_found = true;
                            }
                            break;
                        case 'S':
                            switch (map_pipes[y][x]) {
                                case PIPE_NORTH_SOUTH:
                                    y++;
                                    came_from = 'S';
                                    break;
                                case PIPE_SOUTH_WEST:
                                    x--;
                                    came_from = 'E';
                                    break;
                                case PIPE_SOUTH_EAST:
                                    x++;
                                    came_from = 'W';
                                    break;
                                default:
                                    end_found = true;
                            }
                            break;
                        case 'W':
                            switch (map_pipes[y][x]) {
                                case PIPE_EAST_WEST:
                                    x++;
                                    came_from = 'W';
                                    break;
                                case PIPE_NORTH_WEST:
                                    y++;
                                    came_from = 'S';
                                    break;
                                case PIPE_SOUTH_WEST:
                                    y--;
                                    came_from = 'N';
                                    break;
                                default:
                                    end_found = true;
                            }
                            break;
                    }
                    n_steps++;
                }
            }
        }

        int furthest_point = 0;
        for (int[] row : map_distances) {
            for (int val : row) {
                if (val != UNREACHABLE) {
                    furthest_point = Math.max(furthest_point, val);
                }
            }
        }

        System.out.println("furthest point in the loop: " + furthest_point);

        // idea: z-pass
        // https://developer.nvidia.com/gpugems/gpugems3/part-ii-light-and-shadows/chapter-11-efficient-and-robust-shadow-volumes-using
        int n_tiles_enclosed = 0;
        for (int i = 0; i < MAP_SIZE; i++) {
            int n_intersections = 0;
            for (int j = 0; j < MAP_SIZE; j++) {
                if (map_distances[j][i] != UNREACHABLE) {
                    // tile is part of the loop
                    if ("-FL".indexOf(map_pipes[j][i]) != -1) {
                        n_intersections++;
                    }
                } else {
                    // tile is NOT part of the loop
                    n_tiles_enclosed += n_intersections % 2;
                }
            }
        }

        System.out.println("number of tiles enclosed by the loop: " + n_tiles_enclosed);
    }
}
