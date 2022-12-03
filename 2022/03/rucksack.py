import os

# read input from file
input = open(os.path.join(os.getcwd(),"input.txt"), "r")
lines = input.readlines()

n_members = 3
group = []
counter = 0
dupe_prio_sum = 0
badge_prio_sum = 0
for line in lines:
    line = line.strip()
    # split line
    comp_1 = line[:len(line)>>1]
    comp_2 = line[len(line)>>1:]
    # find dupe
    for c in comp_1:
        if comp_2.find(c) != -1:
            # add dupe prio to sum (a = 1; ...; z = 26; A = 27; ...; Z = 52;)
            dupe_prio_sum += ord(c)-0x60 if (ord(c) > 0x60) else ord(c)-0x26
            break
    

    group.append(line)
    # find badge in each group
    if counter % n_members == n_members-1:
        for c in group[0]:
            found = True
            for m in group[1:n_members]:
                if m.find(c) == -1:
                    found = False
            if found:
                # add badge prio to sum
                badge_prio_sum += ord(c)-0x60 if (ord(c) > 0x60) else ord(c)-0x26
                break
        group = []
    counter += 1

print("sum of dupe prioities: {}\nsum of badge priorities: {}".format(dupe_prio_sum, badge_prio_sum))
