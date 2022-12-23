import os
import numpy as np

def isSymmetric(matrix):
    for i in range(len(matrix)):
        for j in range(len(matrix[0])):
            if (matrix[i][j] != matrix[j][i]):
                return False
    return True

input = open(os.path.join(os.getcwd(), "input.txt"), "r")

names = {}
valves = []

# parse nodes
idx = 0
for line in input.readlines():
    words = line.split()
    name = words[1]
    flow_rate = int(words[4][5:-1])
    names[name] = idx
    idx += 1
    valves.append(flow_rate)

# parse adjacency matrix
tunnels = np.full((len(valves),len(valves)), np.inf)
input.seek(0, 0)
idx = 0
for line in input.readlines():
    words = line.strip().split()
    for n in words[9:]:
        n = n.strip(",")
        tunnels[idx, names[n]] = 1
    tunnels[idx, idx] = 0
    idx += 1

# build distance matrix
mst_set = [0]
while len(mst_set) < len(valves):
    for i in mst_set:
        for j in range(len(valves)):
            if tunnels[i,j] != np.inf:
                for k in mst_set:
                    if tunnels[k,j] > tunnels[i,j] + tunnels[k,i]:
                        tunnels[k,j] = tunnels[i,j] + tunnels[k,i]
                        tunnels[j,k] = tunnels[i,j] + tunnels[k,i]
                if j not in mst_set: mst_set.append(j)

assert(isSymmetric(tunnels))

# part 1
time = 30
max_released = 0

def dfs(time_remaining, total_pressure, open_valves, current_valve, save_to_list):
    global max_released
    if save_to_list: global all_solutions
    for n in [a for a in range(len(valves)) if (a not in open_valves) and (valves[a] != 0)]:
        if (time_remaining - tunnels[current_valve, n]) -2 > -1:
            tr = int((time_remaining - tunnels[current_valve, n]) -1)
            tp = total_pressure + tr * valves[n]
            new_ovs = open_valves.copy()
            new_ovs.append(n)
            if save_to_list: all_solutions.append([tp, new_ovs.copy()])
            if tp > max_released:
                max_released = tp
            dfs(tr, tp, new_ovs, n, save_to_list)

dfs(time, 0, [], names["AA"], False)

print(f"the most pressure that can be released within {time} mins is: {max_released}")

# part 2
time = 26
all_solutions = []

dfs(time, 0, [], names["AA"], True)

max_released = 0
plausible = [solution for solution in all_solutions if solution[0] > 900]
plausible.sort(reverse=True)
for s in plausible:
    for t in plausible:
        if not len([valve for valve in s[1] if valve in t[1]]):
            if max_released < s[0] + t[0]:
                max_released = s[0] + t[0]
                print(f"current best: {max_released}", end="\r", flush=True)

print(f"the most pressure that can be released by two people within {time} mins is: {max_released}")
