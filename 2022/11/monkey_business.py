import os, re, math

for rounds in [20, 10000]:
    # read input from file
    input = open(os.path.join(os.getcwd(),"input.txt"), "r")
    lines = input.readlines()

    items, op, test, branches = [], [], [], []

    # parse input
    for i in range(len(lines)):
        if i%7 == 1:
            items.append([int(x) for x in re.findall(r'\d+', lines[i])])
        elif i%7 == 2:
            op.append(lines[i].strip().split()[3:])
        elif i%7 == 3:
            test.append(int(re.search(r'\d+', lines[i]).group()))
        elif i%7 == 4:
            branches.append([0, int(re.search(r'\d+', lines[i]).group())])
        elif i%7 == 5:
            branches[-1][False] = int(re.search(r'\d+', lines[i]).group())


    # simulate rounds
    test_prod = math.prod(test)
    inspection_count = [0,0,0,0,0,0,0,0]

    for r in range(rounds):
        for m in range(len(items)):
            for i in range(len(items[m])):
                # inspect op
                a = items[m][0] if (op[m][0] == "old") else int(op[m][0])
                b = items[m][0] if (op[m][2] == "old") else int(op[m][2])
                items[m][0] = a+b if (op[m][1] == "+") else a*b
                inspection_count[m] += 1

                # worry relief
                if rounds == 20: items[m][0] = math.floor(items[m][0] / 3)

                # shrink numbers
                if rounds == 10000:items[m][0] %= test_prod

                # test & throw
                res = (items[m][0] % test[m] == 0)
                items[branches[m][res]].append(items[m][0])
                items[m].pop(0)

    inspection_count.sort(reverse=True)
    monkey_business = inspection_count[0] * inspection_count[1]
    print(f"Level of monkey business after {rounds} rounds: {monkey_business}")
