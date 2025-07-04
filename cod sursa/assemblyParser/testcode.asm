
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
