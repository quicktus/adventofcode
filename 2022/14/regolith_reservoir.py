import os
from numpy import zeros
from time import sleep

input = open(os.path.join(os.getcwd(),"input.txt"), "r")
paths = []
count_1, count_2 = 0, 0
rocks_filled = False
do_animate = False

def print_cave(crop):
    os.system('cls' if os.name == 'nt' else 'clear')
    for y in range(len(cave[0])):
        for x in range(crop, len(cave)-crop):
            symbol = "  " if cave[x][y] == 0 else chr(9616 + int(cave[x][y]))*2
            print(symbol, end="")
        print()
    print(f"units of sand deposited before touching the floor: {count_1}")
    if count_1 != count_2: print(f"units of sand deposited before blocking the source: {count_2}")
    if do_animate: sleep(1/10)

# get cave dimensions
x_min, x_max, y_max = 500, 500, 0
lines = input.readlines()
for line in lines:
    path = []
    for n in line.strip().split(" -> "):
        node = []
        for v in n.split(","):
            val = int(v)
            node.append(val)
        if node[0] > x_max: x_max = node[0] + 2
        if node[0] < x_min: x_min = node[0] - 2
        if node[1] > y_max: y_max = node[1] + 3
        path.append(node)
    paths.append(path)

# expand cave for part 2
if 500 - x_min < y_max: x_min = 500 - y_max
if x_max - 500 < y_max: x_max = 500 + y_max +1

source = [500-x_min, 0]
cave = zeros((x_max-x_min,y_max))

# insert rock structures
for path in paths:
    for n in range(1, len(path)):
        x_dir = 1 if path[n-1][0] < path[n][0] else -1
        y_dir = 1 if path[n-1][1] < path[n][1] else -1
        for x in range(path[n-1][0], path[n][0] + x_dir, x_dir):
            for y in range(path[n-1][1], path[n][1] + y_dir, y_dir):
                cave[x-x_min,y] = 3

# insert floor
for i in range(len(cave)):
    cave[i][y_max-1] = 3

# simulate sand
while True:
    # spawn sand
    coords = source.copy()
    while True:
        cave[coords[0], coords[1]] = 1

        if cave[coords[0], coords[1]+1] <= 1:
            coords[1] += 1
        elif cave[coords[0]-1, coords[1]+1] <= 1:
            coords[0] += -1
            coords[1] += 1
        elif cave[coords[0]+1, coords[1]+1] <= 1:
            coords[0] += 1
            coords[1] += 1
        else:
            # sand is resting
            break
        if coords[1] == y_max-2:
            rocks_filled = True

    # set resting sand
    cave[coords[0], coords[1]] = 2

    if not rocks_filled: count_1 += 1
    count_2 += 1
    if do_animate and coords[0] > 100 and coords[0] < len(cave)-100:
        print_cave(100)

    # stop when sand blocks the source
    if cave[source[0], source[1]] == 2: break

print_cave(0)
