
.section .data

mama: .word 120
tata: .byte 6

.section .text

addi x6, x0, 45
addi x8, x0, 26
#prepare to print registers 6 and 8
addi x2, x0, 13
slli x2, x2, 1
addi x1, x0, 17
slli x1, x1, 8
or x1, x1, x2



success:
addi x1, x1, 1
ebreak x1, x2
failed:
ori x1, x1, 0xFFFFFFFE
ebreak x1, x2