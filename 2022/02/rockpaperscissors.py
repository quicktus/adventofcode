import os

# read input from file
input = open(os.path.join(os.getcwd(),"input.txt"), "r")
lines = input.readlines()

# game score = shape points + outcome points
# shape points: R = 1, P = 2, S = 3
# outcome points: loss = 0, draw = 3, win = 6

first_score = 0
second_score = 0
for line in lines:
    their_shape = ord(line[0]) - 64

    # first score
    my_shape = ord(line[2]) - 87
    first_score += my_shape
    first_score += (my_shape - their_shape + 1) % 3 * 3

    # second score
    outcome = (ord(line[2]) - 88) * 3
    second_score += outcome
    tmp = (their_shape + int(outcome/3 - 1)) % 3  # %3 makes 3 + W = 1 (not 4)
    second_score += tmp if tmp!=0 else 3  # else makes 1 + L = 3 (not 0)

print("first score: {}\nsecond score: {}".format(first_score, second_score))
