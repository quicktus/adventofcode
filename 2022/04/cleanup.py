import os, re

# read input from file
input = open(os.path.join(os.getcwd(),"input.txt"), "r")
lines = input.readlines()

sum_enclosures = 0
sum_overlaps = 0
for line in lines:
    line = line.strip()
    vals = re.split(r',|-', line)
    a_start, a_end, b_start, b_end = [eval(i) for i in vals]

    # find pairs where one encloses the other
    if (a_start >= b_start and a_end <= b_end) or (a_start <= b_start and a_end >= b_end):
        sum_enclosures += 1
        sum_overlaps += 1

    # find pairs that overlap
    elif (a_start >= b_start and a_end >= b_end and b_end >= a_start) or (a_start <= b_start and a_end <= b_end and b_start <= a_end):
         sum_overlaps += 1

print("sum of enclosures: {}\nsum of overlaps: {}".format(sum_enclosures, sum_overlaps))
