import os

# read input from file
input = open(os.path.join(os.getcwd(),"input.txt"), "r")
lines = input.readlines()

elf_list = []
# calculate sums
cal_sum = 0
for line in lines:
    line = line.strip()
    if line == "":
        elf_list.append(cal_sum)
        cal_sum = 0
    else:
        cal_sum += int(line)

elf_list.sort(reverse=True)
n = 3
print("top {} elfs: {}  sum: {}".format(n,elf_list[:n], sum(elf_list[:n])))
