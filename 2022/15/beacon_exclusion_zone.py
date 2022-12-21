import os

input = open(os.path.join(os.getcwd(), "input.txt"), "r")
lines = input.readlines()

sensors, beacons, bez_radius = [], [], []
read_row = 2000000

def manhattan_dist(ax, bx, ay, by) -> int:
    return abs(ax-bx) + abs(ay-by)

for line in lines:
    words = line.split()
    sensor_x = int(words[2][2:-1])
    sensor_y = int(words[3][2:-1])
    beacon_x = int(words[8][2:-1])
    beacon_y = int(words[9][2:])

    sensors.append([sensor_x, sensor_y])
    beacons.append([beacon_x, beacon_y])
    bez_radius.append(manhattan_dist(sensor_x, beacon_x, sensor_y, beacon_y))

min_x = min([s[0] for s in sensors]) - max(bez_radius)
max_x = max([s[0] for s in sensors]) + max(bez_radius)

bez_count = 0
beacons_2m_x = [b[0] for b in beacons if b[1] == read_row]
sensors_2m = [(abs(sensors[s][1]-read_row) <= bez_radius[s]) for s in range(len(sensors))]

# part 1
for x in range(min_x-1, max_x+2):
    if (x-min_x) % 40000 == 0: print(f"[{int((x-min_x)/(max_x-min_x)*100)}%]", end="\r")
    if not x in beacons_2m_x:
        for s in range(len(sensors)):
            if sensors_2m[s]:
                if manhattan_dist(x, sensors[s][0], read_row, sensors[s][1]) <= bez_radius[s]:
                    bez_count +=  1
                    break

print(f"number of positions which cannot contain a beacon in the row where y={read_row}: {bez_count}")

# part 2
d_beacon_x, d_beacon_y = -1, -1
win_min, win_max = 0, 4000000
found = False

for s in range(len(sensors)):
    print(f"[{int(s/len(sensors)*100)}%]", end="\r")
    center_x, center_y, radius = sensors[s][0], sensors[s][1], bez_radius[s]
    bot_x = max(min(center_x-radius, win_max), win_min)
    top_x = max(min(center_x+radius, win_max), win_min)
    bot_y = max(min(center_y-radius, win_max), win_min)
    top_y = max(min(center_y+radius, win_max), win_min)
    edge_r = zip(list(range(center_x+1, top_x+1))+list(range(top_x+1, center_x, -1)), list(range(bot_y, top_y+1)))
    edge_l = zip(list(range(center_x-1, bot_x-1, -1))+list(range(bot_x-1, center_x)), list(range(bot_y, top_y+1)))
    border = list(edge_r) + list(edge_l)
    for b in border:
        found = True
        for n in range(len(sensors)):
            if manhattan_dist(b[0], sensors[n][0], b[1], sensors[n][1]) <= bez_radius[n]:
                found = False
                break
        if found:
            d_beacon_x = b[0]
            d_beacon_y = b[1]
            break
    if found:
        break

print(f"the distress beacon (x={d_beacon_x}, y={d_beacon_y}) has a tuning frequency of: {d_beacon_x*win_max + d_beacon_y}")
