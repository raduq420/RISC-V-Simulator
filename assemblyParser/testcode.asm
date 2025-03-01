addi x1, x1, 10
addi x2, x2, 5
slt x6, x2, x1
and x6, x2, x1
or x6, x2, x1
xor x3, x3, x3
xor x4, x4, x4
blt x1, x2, start
xor x4, x4, x4
xor x4, x4, x4
start:
add x3, x3, x1
addi x2, x2, -1
bne x2, x4, start
add x1, x2, x3