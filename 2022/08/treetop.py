import os
from numpy import zeros, empty
import matplotlib.pyplot as plt

input = open(os.path.join(os.getcwd(),"input.txt"), "r")
grid = input.readlines()
visible = zeros((99,99))
scenic_score = empty((99,99))

for row in range(99):
    # looking east
    max = -1
    for col in range(99):
        if int(grid[row][col]) > max:
            max = int(grid[row][col])
            visible[row,col] = 1

    # looking west
    max = -1
    for col in range(98, -1, -1):
        if int(grid[row][col]) > max:
            max = int(grid[row][col])
            visible[row,col] = 1

for col in range(99):
    # looking south
    max = -1
    for row in range(99):
        if int(grid[row][col]) > max:
            max = int(grid[row][col])
            visible[row,col] = 1

    # looking north
    max = -1
    for row in range(98, -1, -1):
        if int(grid[row][col]) > max:
            max = int(grid[row][col])
            visible[row,col] = 1

# find ideal tree
highest_score = 0
hs_row, hs_col = 0, 0
for row in range(1, 98):
    for col in range(1, 98):
        height = int(grid[row][col])
        score = 1
        a = row == 23 and col == 52

        # viewing distance east
        for i in range(1, 99-col):
            if (int(grid[row][col+i]) >= height) or (i == 98-col):
                score *= i
                break

        # viewing distance west
        for i in range(1, col+1):
            if (int(grid[row][col-i]) >= height) or (i == col):
                score *= i
                break

        # viewing distance south
        for i in range(1, 99-row):
            if (int(grid[row+i][col]) >= height) or (i == 98-row):
                score *= i
                break

        # viewing distance north
        for i in range(1, row+1):
            if (int(grid[row-i][col]) >= height) or (i == row):
                score *= i
                break

        scenic_score[row,col] = score
        if highest_score < score:
            hs_row, hs_col = row, col
            highest_score = score

print(f"total number of visible trees: {int(visible.sum())}\n highest scenic score: {highest_score} [{hs_row},{hs_col}]")
plt.imshow(scenic_score, cmap="viridis", interpolation="bilinear", norm="symlog")
plt.title("tree's scenic scores")
plt.show()
