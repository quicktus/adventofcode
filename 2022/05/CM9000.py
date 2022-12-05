import os
from collections import deque

input = open(os.path.join(os.getcwd(),"input.txt"), "r")
lines = input.readlines()

stacks = [ deque([]) for _ in range(9) ]

# read starting positions
for line in range(8):
    for c in range(9):
        if lines[line][1+c*4] != " ":
            stacks[c].appendleft(lines[line][1+c*4])

# moves
for line in range(10, len(lines)):
    # read instruction
    nums = [int(i) for i in lines[line].split() if i.isdigit()]
    count = nums[0]
    src = nums[1] -1
    dest= nums[2] -1

    # do moves
    for i in range(count):
        stacks[dest].append(stacks[src].pop())



print("{l}[ CrateMover 9000 ]{l}\n".format(l="-"*8))

#print final positions
height = max([len(i) for i in stacks])
for i in range(height-1, -1, -1):
    for stack in stacks:
        try:
            val = stack[i]
            print("[{}] ".format(val), end="")
        except:
            print(" "*4, end="")
    print()
print(" 1   2   3   4   5   6   7   8   9 \n")

topcrates = [s[-1] for s in stacks]
print("result: {}".format("".join(topcrates)))
