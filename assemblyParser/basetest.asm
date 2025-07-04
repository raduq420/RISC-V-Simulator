
success:
addi x1, x1, 1
ebreak x1, x2
failed:
ori x1, x1, 0xFFFFFFFE
ebreak x1, x2