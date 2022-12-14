import os, math
from time import sleep

def animate():
    size = 12
    gridsize = 5
    os.system('cls' if os.name == 'nt' else 'clear')
    for y in reversed(range(coords[0][1]-size, coords[0][1]+size+1)):
        # print left border
        if y != coords[0][1]-size and y != coords[0][1]+size:
            if y%gridsize == 0: print("├ ", end="")
            else: print("│ ", end="")
        for x in range(coords[0][0]-size, coords[0][0]+size+1):

            # print upper border
            if y == coords[0][1]+size:
                if x == coords[0][0]-size: print("╭─", end="")
                if x%gridsize == 0: print("┬─", end="")
                else: print("──", end="")
                if x == coords[0][0]+size: print("╮")
                continue

            # print lower border
            if y == coords[0][1]-size:
                if x == coords[0][0]-size: print("╰─", end="")
                if x%gridsize == 0: print("┴─", end="")
                else: print("──", end="")
                if x == coords[0][0]+size: print("╯")
                continue

            # print rope body
            is_name = False
            for i in range(len(names)):
                if x == coords[i][0] and y == coords[i][1]:
                    print(names[i], end=" ")
                    is_name = True
                    break

            # print landmarks
            if not is_name:
                if x == 0 and y == 0: print("🜨 ", end="")
                elif (x,y) in visited_1:
                    if (x,y) in visited_9: print("◉ ", end="")
                    else: print("● ", end="")
                elif (x,y) in visited_9: print("○ ", end="")
                elif x%gridsize == 0 and y%gridsize == 0: print("+ ", end="")
                else: print("  ", end="")

        # print right border
        if y != coords[0][1]-size and y != coords[0][1]+size:
            if y%gridsize == 0: print("┤")
            else: print("│")

    # print status text
    win_len = size*4 + 5
    text_len = 6 + 11 + 11 + len(str(step)) + len(str(len(visited_1))) + len(str(len(visited_9)))
    padding = " "*math.floor(0.5*(win_len-text_len))
    print(f"STEP: {step}{padding} T1 COUNT: {len(visited_1)}{padding} T2 COUNT: {len(visited_9)}", flush=True)

    # wait
    if do_animate: sleep(1/15)


try:
    # read input from file
    input = open(os.path.join(os.getcwd(),"input.txt"), "r")
    lines = input.readlines()

    coords = [[0,0], [0,0], [0,0], [0,0], [0,0], [0,0], [0,0], [0,0], [0,0], [0,0]]
    names = ["H", "1", "#", "#", "#", "#", "#", "#", "#", "2"]

    visited_1, visited_9 = set(),set()
    visited_1.add((coords[1][0],coords[1][1]))
    visited_9.add((coords[9][0],coords[9][1]))

    do_animate = True

    step = 0
    for line in lines:
        for i in range(int(line[2:-1])):
            step +=1

            # move head
            coords[0][0] += (line[0] == "R") - (line[0] == "L")
            coords[0][1] += (line[0] == "U") - (line[0] == "D")

            if do_animate:animate()

            # move tail segments
            for node in range(1, len(coords)):
                x_diff = (coords[node-1][0] - coords[node][0])
                y_diff = (coords[node-1][1] - coords[node][1])
                if (abs(x_diff) == 2 or abs(y_diff) == 2):
                    coords[node][0] += x_diff if abs(x_diff) == 1 else x_diff>>1
                    coords[node][1] += y_diff if abs(y_diff) == 1 else y_diff>>1

                # add new tail positions to the sets
                visited_1.add((coords[1][0],coords[1][1]))
                visited_9.add((coords[9][0],coords[9][1]))

                if do_animate:animate()

    animate()

except KeyboardInterrupt:
    exit(0)
