
.section .data

mama: .word 120
tata: .byte 6

.section .text
addi x2, x3, mama
addi x2, x3, tata
addi x2, x3, 16
hellou:
sub x2, x2, x2
jal x2, failed

success:
add x0, x0, x0
add x2, x2, x2
failed:
sub x0, x0, x0
