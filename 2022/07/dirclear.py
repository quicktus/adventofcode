import os

input = open(os.path.join(os.getcwd(),"input.txt"), "r")
total_disk_space = 70000000
required_disk_space = 30000000
dir_size_threshold = 100000
dict = {}
cwd = []
checked = []
res = 0

cmds = input.read().split("$")[1:]
for cmd in cmds:
    args = cmd.strip().split()
    if args[0] == "cd":
        if args[1] == "..":
            cwd.pop()
        else:
            cwd.append(args[1])
    elif args[0] == "ls":
        nums = [int(i) for i in args[1:] if i.isdigit()]
        if (not cwd in checked):
            checked.append(cwd.copy())
            for i in range(1, len(cwd)+1):
                path = "/".join(cwd[:i])
                dict[path] = dict.get(path, 0) + sum(nums)

for v in dict.values():
    if (v <= dir_size_threshold):
        res += v

free_disk_space = total_disk_space - dict["/"]
name_min, min = "none", total_disk_space
for k in dict.keys():
    if (dict[k] < min and required_disk_space - free_disk_space <= dict[k]):
        name_min, min = str(k), dict[k]

print(f"space gained by deleting dirs smaller than {int(dir_size_threshold/1000)}k: {res} ({int(res/1000)}k)")
print(f"smallest dir that would free up enough additional space for the update ({int((required_disk_space-free_disk_space)/1000)}k): {name_min[1:]} with {min} ({int(min/1000)}k)")
