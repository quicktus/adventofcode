import os
from time import sleep

# read input from file
input = open(os.path.join(os.getcwd(),"input.txt"), "r")
lines = input.readlines()

signal_strength = []
reg_x = 1
cycle = 1
res = 0
line_counter = -1
line = None
op = None
add_counter = 0
do_animate = True

while True:
    # check CPU instruction
    if op == None:
        line_counter += 1
        if not line_counter == len(lines):
            # fetch next op
            line = lines[line_counter]
            op = line[0:4]
        else:
            # ran out of ops!
            break

    # simulate one CRT cycle
    if (
        reg_x == (cycle-1)%40 or \
        reg_x-1 == (cycle-1)%40 or \
        reg_x+1 == (cycle-1)%40
        ):
        # draw sprite pixel
        print("██", end="", flush=do_animate)
    else:
        # draw blank pixel
        print("░░", end="", flush=do_animate)
    if cycle%40 == 0:
        # go to next line
        print("")

    # simulate one CPU cycle
    if op == "noop":
        signal_strength.append(reg_x*cycle)
        op = None

    elif op == "addx":
        signal_strength.append(reg_x*cycle)
        if add_counter == 1:
            reg_x += int(line[5:-1])
            add_counter = 0
            op = None
        else:
            add_counter += 1

    # wait
    if do_animate: sleep(1/50)

    # increment cycle
    cycle += 1

# calculate the "interesting" sum
for i in range(20, 221, 40):
    res += signal_strength[i-1]

print(f"\nsum of the interesting six signal strengths: {res}")
