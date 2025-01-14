addi x1, x2, 16
slli x1, x1, 2
lebal:
sub x1, x1, x1
sw x1, 5(x1)
beq x1, x2, lebal
lw x4, 5(x1)
addi x2, x4, 10 