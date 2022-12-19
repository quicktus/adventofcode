import os, re

input = open(os.path.join(os.getcwd(),"input.txt"), "r")
pairs = input.read().split("\n\n")
sum_valid_idx = 0
idx_2, idx_6 = 1, 2

pair_idx = 0
for pair in pairs:
    pair_idx += 1
    pack_l, pack_r = pair.strip().split("\n")
    l_items, r_items = pack_l.split(","), pack_r.split(",")
    l_depth, r_depth = 0, 0

    # comparison rules:
    #   (int int): right order If left < right (==: neutral, <: wrong order)
    #   (list list): right order If left runs out of items first (right runs out first: wrong order)
    #   (list int): convert int to list of one item and compare again

    # part 1
    for i in range(max(len(l_items),len(r_items))):
        l_depth += l_items[i].count("[")
        r_depth += r_items[i].count("[")
        l_temp, r_temp = 0, 0
        l_num, r_num = re.findall(r'\d+', l_items[i]), re.findall(r'\d+', r_items[i])

        if len(l_num) < len(r_num):
            # left has fewer items (none) -> right order
            sum_valid_idx += pair_idx
            break
        elif len(l_num) > len(r_num):
            # right has fewer items (none) -> wrong order
            break

        # both lists have numbers
        elif len(l_num) == 1 and len(r_num)== 1:
            if int(l_num[0]) < int(r_num[0]):
                # left num is smaller -> right order
                sum_valid_idx += pair_idx
                break
            elif int(l_num[0]) > int(r_num[0]):
                # right num is smaller -> wrong order
                break
            elif l_depth < r_depth:
                l_temp += r_depth - l_depth
            elif l_depth > r_depth:
                r_temp += l_depth - r_depth

        # both lists empty
        elif len(l_num) == 0 and len(r_num)== 0:
            if l_depth < r_depth:
                # left is shorter -> right order
                sum_valid_idx += pair_idx
                break
            elif l_depth > r_depth:
                # right is shorter -> wrong order
                break

        # keep evaluating
        if l_items[i].count("]") + l_temp > r_items[i].count("]") + r_temp:
            # left is shorter -> right order
            sum_valid_idx += pair_idx
            break
        elif l_items[i].count("]") + l_temp < r_items[i].count("]") + r_temp:
            # right is shorter -> wrong order
            break

        l_depth -= l_items[i].count("]")
        r_depth -= r_items[i].count("]")

        # end of outer list
        if l_depth == 0 and r_depth != 0:
            # left is shorter -> right order
            sum_valid_idx += pair_idx
            break
        elif r_depth == 0 and l_depth != 0:
            # right is shorter -> wrong order
            break

    # part 2
    for items in [l_items, r_items]:
        nums = re.findall(r'\d+', items[0])
        if len(nums) == 0:
            idx_2 += 1
            idx_6 += 1
        else:
            idx_2 += (int(nums[0]) < 2)
            idx_6 += (int(nums[0]) < 6)

print(f"sum of the indices of all valid pairs: {sum_valid_idx}")
print(f"decoder key: {idx_2*idx_6}")