## decoded instructions:
1. 2,4 bst  B:= A mod 8
2. 1,1 bxl  B:= B xor 1
3. 7,5 cdv  C:= A / 2^B
4. 1,5 bxl  B:= B xor 5
5. 0,3 adv  A:= A / 2^3
6. 4,4 bxc  B:= B xor C
7. 5,5 out  println!(b mod 8)
8. 3,0 jnz  if A != 0 {goto 0}

## pseudocode
b = a mod 8             // get lowest 3 bits from a
b = b xor 1
c = a / 2^b            // result depends on all remaining bits in reg a
b = b xor 0b101
b = b xor c
println!(b mod 8)
a = a / 8              // shift a to the right by 3 bits
if A != 0 {goto 0}

## implications
-> each 3-bit segment produces one output number
-> the remaining digits of reg a influence the output number
-> the input needs to be built backwards starting from the end so all subsequent digits are known
-> go from low to high values to find the lowest integer solution
-> when there are multiple values that map to the target, try each (from low to high) in case there is a dead end (backtracking)
