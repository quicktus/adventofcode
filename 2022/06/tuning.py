import os
from collections import Counter

def find_seq(signal, seq_len):
    for i in range(seq_len, len(signal)):
        if(len(Counter(signal[(i-seq_len):i])) == seq_len):
            return i

input = open(os.path.join(os.getcwd(),"input.txt"), "r")
line = input.readline()
pgk_seq_len, msg_seq_len = 4, 14

print(f"first package sequence found after {find_seq(line, pgk_seq_len)} chars")
print(f"first message sequence found after {find_seq(line, msg_seq_len)} chars")

# [len(Counter(signal[(i-seq_len):i])) for i in range(seq_len, len(signal))].index(seq_len)
