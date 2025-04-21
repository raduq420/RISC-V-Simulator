addi x1, x1, 43701
addi x2, x2, 5
ori x11, x2, 2
slt x6, x2, x0
slti x6, x2, -7
sltiu x6, x2, -7
sw x1, 8(x2)
sh x1, 20(x2)
sb x1, 30(x2) 
lw x5, 8(x2)
lb x5, 8(x2)
lh x5, 8(x2)
lbu x5, 8(x2)
lhu x5, 8(x2)
lwu x5, 8(x2)
ld x5, 8(x2)
and x6, x2, x1
addi x6, x6, -5
addi x7, x7, -10
bltu x6, x7, start
bgeu x6, x7, start
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
lui x5, 20
addi x5, x5, 20
auipc x5, 20
addi x5, x5, 20


