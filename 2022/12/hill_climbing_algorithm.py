import os
import numpy as np

input = open(os.path.join(os.getcwd(),"input.txt"), "r")
grid = input.readlines()

y_len = len(grid)
x_len = len(grid[0].strip())
heights = np.empty((y_len,x_len))
distances = np.full((y_len,x_len), np.inf)
start, end = (0,0), (0,0)

# find start and end point and fill heights
for row in range(y_len):
    for col in range(x_len):
        if grid[row][col] == "S":
            start = (row, col)
            heights[row,col] = ord("a")
        elif grid[row][col] == "E":
            end = (row, col)
            heights[row,col] = ord("z")
        else:
            heights[row,col] = ord(grid[row][col])

# part 1: find shortest path up from S to E
distances[start] = 0
to_visit = [start]

while len(to_visit):
    for n in [(0,1), (1,0) ,(0,-1), (-1,0)]:
        neighbour = (to_visit[0][0]+n[0], to_visit[0][1]+n[1])

        # check grid range
        if neighbour[0] >= y_len or neighbour[0] < 0 or \
            neighbour[1] >= x_len or neighbour[1] < 0:
            continue

        # check height diff
        if heights[to_visit[0]] - heights[neighbour] < -1:
            continue

        # check improvement
        if distances[to_visit[0]]+1 < distances[neighbour]:
            distances[neighbour] = distances[to_visit[0]]+1
            if not neighbour in to_visit:
                to_visit.append(neighbour)
    to_visit.pop(0)

print(f"length of the shortest path S -> E: {int(distances[end])}")

# part 2: find shortest path down from E to any a
min_path = int(distances[end])
distances = np.full((y_len,x_len), np.inf)
distances[end] = 0
to_visit = [end]

while len(to_visit):
    for n in [(0,1), (1,0) ,(0,-1), (-1,0)]:
        neighbour = (to_visit[0][0]+n[0], to_visit[0][1]+n[1])

        # check grid range
        if neighbour[0] >= y_len or neighbour[0] < 0 or \
            neighbour[1] >= x_len or neighbour[1] < 0:
            continue

        # check height diff
        if heights[neighbour] - heights[to_visit[0]] < -1:
            continue

        # check improvement
        if distances[to_visit[0]]+1 < distances[neighbour]:
            distances[neighbour] = distances[to_visit[0]]+1
            if chr(int(heights[neighbour])) == "a" and distances[neighbour] < min_path:
                min_path = distances[neighbour]
            if not neighbour in to_visit:
                to_visit.append(neighbour)
    to_visit.pop(0)

print(f"length of the shortest path a -> E: {int(min_path)}")
